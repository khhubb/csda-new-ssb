// DragSourceButton.cpp : implementation file
//

#include "stdafx.h"

 
#include "csda.h"
#include "DragSourceButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Notify.h"


/////////////////////////////////////////////////////////////////////////////
//	CDragSourceButton
//
//	This is used to subclass a CWnd (not necessarily a button)
//     and give it a callback for a left click.
//
//  It is provided with a callback object of type CNotify.
//  When left-clicked, it calls the Notify() method of the callback object.
//
////////////////////////////////////////////////////////////////////////////


CDragSourceButton::CDragSourceButton()
{
	m_pNotify = 0;
}

CDragSourceButton::~CDragSourceButton()
{
}


BEGIN_MESSAGE_MAP(CDragSourceButton, CButton)
	//{{AFX_MSG_MAP(CDragSourceButton)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragSourceButton message handlers


//
//  Use this to set the notification object for callbacks.
//

void CDragSourceButton::SetNotify(CNotify* p)
{
	m_pNotify = p;
}



//
//  The only event we handle.
//

void CDragSourceButton::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if ( m_pNotify != 0 )
		m_pNotify->Notify();
}
