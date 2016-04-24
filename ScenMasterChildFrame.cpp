// ScenMasterChildFrame.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "ScenMasterChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ScenMasterDoc.h"
#include "ScenMasterGraphView.h"
#include "ScenMasterCtrlView.h"


/////////////////////////////////////////////////////////////////////////////
// CScenMasterChildFrame

IMPLEMENT_DYNCREATE(CScenMasterChildFrame, CChildFrame)

CScenMasterChildFrame::CScenMasterChildFrame()
{
}

CScenMasterChildFrame::~CScenMasterChildFrame()
{
}


BEGIN_MESSAGE_MAP(CScenMasterChildFrame, CChildFrame)
	//{{AFX_MSG_MAP(CScenMasterChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMasterChildFrame message handlers

static UINT BASED_CODE buttons[] = 
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_SEPARATOR,
	ID_ZOOM_PERCENT,
	ID_ZOOM_IN,
	ID_ZOOM_OUT,
	ID_ZOOM_POINTIN,
	ID_ZOOM_POINTOUT,
	ID_ZOOM_RECTIN,
	ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
};

BOOL CScenMasterChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// this editor has a splitter containing
	//   a CScenMasterGraphView in the upper half
	//   and a CScenMasterCtrlView in the lower half.

	if ( ! m_wndSplitter.CreateStatic(this,2,1) )
		return FALSE;


	if ( ! m_wndSplitter.CreateView(0,0,
									RUNTIME_CLASS(CScenMasterGraphView),
									CSize(800,500),
									pContext) )
		return FALSE;

	if ( ! m_wndSplitter.CreateView(1,0,
									RUNTIME_CLASS(CScenMasterCtrlView),
									CSize(800,500),
									pContext) )
		return FALSE;

	//  Why aren't we seeing this toolbar?

	if ( ! m_wndToolBar.Create(this) 
		 ||
		 ! m_wndToolBar.LoadBitmap(IDR_SCEN_MASTER)
		 ||
		 ! m_wndToolBar.SetButtons(buttons,sizeof(buttons)/sizeof(UINT)) ) 
		 return FALSE;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY );


	return TRUE;
}



void CScenMasterChildFrame::ActivateFrame(int nCmdShow) 
{
	CChildFrame::ActivateFrame(SW_SHOWMAXIMIZED);
}
