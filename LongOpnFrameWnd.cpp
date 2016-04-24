// LongOpnFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "LongOpnFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "resource2.h"


/////////////////////////////////////////////////////////////////////////////
// CLongOpnFrameWnd
//
//
//  This class is a CFrameWnd which creates the following:
//
//		m_list -- a CListCtrl used to display a list of strings
//				  describing steps in an operation
//		
//		m_imageList -- a CImageList which contains the images used
//				  to animate the currently selected step.
//
//	This class is designed for use strictly by CLongOpn.
//
//	The intent is that we want to display that progress is occurring
//	  on a long operation consisting of a number of steps.
//
//	A list of strings identifies the different steps. 
//
//  Because a given step may take some time, we want to indicate that
//  some progress is occurring, without trying to use something like
//	a progress bar.  So, we display a sequence of images on the currently
//  selected item.  The timing of the animation comes from a timer.
//  The image list must be constructed so that the images are
//		index = 1 -- item pending
//	    index = 2 -- item completed
//		index = 3-8 -- animation of six steps showing in progress
//
//  CLongOpn and CLongOpnFrameWnd are designed to work in the foreground,
//  as a UserInterface thread, while the operation that is occurring
//	presumably is running in a worker thread in the background.
//
//	Communication between threads is handled by sending a message.
//
//  Since this class was designed to work with CLongOpn, 
//    see that class for an example of how to use this one.


IMPLEMENT_DYNCREATE(CLongOpnFrameWnd, CFrameWnd)


// when constructed, create the window, which in turn
// will trigger OnCreate to create the controls.

CLongOpnFrameWnd::CLongOpnFrameWnd()
{
	Create(0,0);
}

CLongOpnFrameWnd::~CLongOpnFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CLongOpnFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CLongOpnFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP

	ON_MESSAGE( WM_USER_STEP_OPN, OnStep )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLongOpnFrameWnd message handlers


//  Set the strings to be used.
//  Mark that the display is not initialized.

void CLongOpnFrameWnd::SetStrings(vector<CString>& strings)
{
	m_strings = strings;
	m_bInitialized = false;
}



// On creation, create the child CListCtrl and load in the list of images.
// Center it.

int CLongOpnFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_list.Create(LVS_REPORT|LVS_NOSORTHEADER|WS_CHILD|WS_VISIBLE,
				  CRect(10,10,510,510),
				  this,
				  IDC_LIST1);

	m_list.InsertColumn(0,"Operation");

	BOOL createOk = m_imageList.Create(IDB_LONG_OPN_IMAGE_LIST,10,0,RGB(192,192,192));
	assert(createOk);
	m_list.SetImageList(&m_imageList,LVSIL_STATE);
	

	m_step = 0;
	m_incr = 0;
	m_timerId = 0;
	m_bInitialized = false;

	SetWindowPos(0,0,0,200,300,SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow(GetDesktopWindow());

	return 0;
}



//  Called in response to the user message indicating that
//    another step is to be selected.
//	The lParam gives the index of the selected step.

// ## change return type to LRESULT
LRESULT CLongOpnFrameWnd::OnStep(WPARAM wParam, LPARAM lParam)
{
	//  If active, kill the timer
	if ( m_timerId != 0 )
		KillTimer(m_timerId);

	// we are not really in existence yet, nothing to do
	if ( ! ::IsWindow(m_list.m_hWnd) )
		return 0;

	// This is first time, or strings have changed.
	// Update the list

	if ( ! m_bInitialized ) {
		m_list.DeleteAllItems();

		int size = 0;

		int itemNum = 0;

		for ( vector<CString>::const_iterator is = m_strings.begin();
			  is != m_strings.end();
			  ++is, ++itemNum ) {

			m_list.InsertItem( itemNum, (*is), 1 );
			size = max( size, 35+m_list.GetStringWidth( (*is) ) );
		}

		m_list.SetColumnWidth(0,size);

		m_bInitialized = true;
	}

	//  Set the image on each item in the list.
	//  Items before the current step have an image indicating completion (index 2)
	//  Items after the current step have have an image indication pending (index 1)
	//  The item for the current step has the first in sequence of animation images (index 3).

	int numSteps = m_list.GetItemCount();

	m_step = min(numSteps,lParam);
	m_incr = 0;


	for ( int i=0; i<numSteps; ++i )
		m_list.SetItemState(i,
						    INDEXTOSTATEIMAGEMASK( i< m_step 
							                       ? 2 
												   : i==m_step 
												     ? 3 
												     : 1 ), 
							LVIS_STATEIMAGEMASK);

	// update after one second
	m_timerId = SetTimer(1,1000,0);

	return 0;
}


// Time to update 
// increment m_incr, use to index the animation image

// ## Change UINT to UINT_PTR
void CLongOpnFrameWnd::OnTimer(UINT_PTR nIDEvent) 
{
	if ( nIDEvent == m_timerId ) {

		m_incr = (m_incr+1) % 6;
		if ( 0 <= m_step && m_step < m_list.GetItemCount() )
			m_list.SetItemState(m_step,
							    INDEXTOSTATEIMAGEMASK(3+m_incr),
								LVIS_STATEIMAGEMASK);
		m_timerId = SetTimer(1,1000,0);
	}
	else
		CFrameWnd::OnTimer(nIDEvent);
		
}
