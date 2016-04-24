// LongOpn.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "LongOpn.h"
#include "LongOpnFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "resource2.h"
#include "LongOpnFrameWnd.h"

/////////////////////////////////////////////////////////////////////////////
//
// CLongOpn
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
//
//	Usage:
//
//		CLongOpn opn;
//
//		opn.m_strings.push_back("Step 1");
//		opn.m_strings.push_back("Step 2");
//		// ...
//		opn.m_strings.push_back("Done");
//		opn.SetCurrentStep(0);
//		opn.Go();
//
//		// ... code to first step
//
//		opn.SetCurrentStep(1);
//
//		// ... code for next step
//
//		// etc
//	
//		opn.Done();
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CLongOpn, CWinThread)

CLongOpn::CLongOpn()
{
	m_bInitialized = false;
	m_pendingStep = -1;
}

CLongOpn::~CLongOpn()
{
#ifdef _DEBUG
	afxDump << "Deleting CLongOpn: " << this << "\n";
#endif
}


// Thread is starting up
// Create the GUI and initialize it.
// Set the step if one has been specified.
// Bring up the window.

BOOL CLongOpn::InitInstance()
{
	m_pFrame= new CLongOpnFrameWnd;
	m_pMainWnd = m_pFrame;

	m_pFrame = (CLongOpnFrameWnd*)m_pMainWnd;
	m_pFrame->SetStrings(m_strings);
	m_pFrame->SetWindowText(m_title);

	m_bInitialized = true;

	if ( m_pendingStep != -1 ) {
		SetCurrentStep(m_pendingStep);
		m_pendingStep = -1;
	}
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	m_pMainWnd->SetFocus();
	return TRUE;
}



// Cleanup when done.

int CLongOpn::ExitInstance()
{
	int retval = CWinThread::ExitInstance();

	delete this;

	return retval;
}


// Start the thread

void CLongOpn::Go()
{
	CreateThread();
}


//  We are done.
//  Close the window and cleanup after pausing

void CLongOpn::Done(int millisecs)
{
	::Sleep(millisecs);
	m_pFrame->PostMessage(WM_CLOSE);
}


// Update the current step

void CLongOpn::SetCurrentStep(int stepNum)
{
	if ( m_bInitialized ) 
		m_pFrame->PostMessage(WM_USER_STEP_OPN,0,stepNum);
	else
		m_pendingStep = stepNum;
}


BEGIN_MESSAGE_MAP(CLongOpn, CWinThread)
	//{{AFX_MSG_MAP(CLongOpn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLongOpn message handlers
