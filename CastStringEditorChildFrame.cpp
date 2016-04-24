// CastStringEditorChildFrame.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CastStringEditorChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "CastStringEditorTextView.h"
#include "CastStringEditorGanttView.h"
#include "CastStringEditorStrandSummaryView.h"
#include "CastStringEditorDoc.h"
#include "CastStringEditorScenMgrView.h"
#include "CastStringEditorValidnView.h"


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorChildFrame

IMPLEMENT_DYNCREATE(CCastStringEditorChildFrame, CChildFrame)

CCastStringEditorChildFrame::CCastStringEditorChildFrame()
{
}

CCastStringEditorChildFrame::~CCastStringEditorChildFrame()
{
}


BEGIN_MESSAGE_MAP(CCastStringEditorChildFrame, CChildFrame)
	//{{AFX_MSG_MAP(CCastStringEditorChildFrame)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorChildFrame message handlers

BOOL CCastStringEditorChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, 
												 CCreateContext* pContext) 
{
	if ( ! m_wndSplitter.CreateStatic(this,2,1) )
		return FALSE;

	// the massive text view in the top
	
	if ( ! m_wndSplitter.CreateView(0,0,
		                            RUNTIME_CLASS(CCastStringEditorTextView),
									CSize(800,500),
									pContext) )
		return FALSE;

	// tabbed window in  the bottom.

	if ( ! m_wndSplitter.CreateView(1,0,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(800,200),
									 pContext) )
		return FALSE;


	m_pTabWnd2 = (SECTabWnd*) m_wndSplitter.GetPane(1,0);

	if ( ! m_ganttDisplay.Create(m_pTabWnd2,
								 RUNTIME_CLASS(CCastStringEditorGanttView),
								 CSize(40,200),
								 CSize(150,200),
								 pContext,
								 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
								 1000) ) 
		return FALSE;

	m_pTabWnd2->AddTab(m_ganttDisplay.GetSplitter(),"Gantt");
	
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorStrandSummaryView),
					   "Summary", 
					   pContext);
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorScenMgrView),
					   "ScenMgr", 
					   pContext);
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorValidnView),
					   "Validn",
					   pContext);

	m_pTabWnd2->ActivateTab(0);
	m_pTabWnd2->ScrollToTab(0);


#if 0
	// used to do this, before putting the gantt in the tabbed window
	//  we have a splitter with a gantt on one side, a tabbed window with three views on the other

	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter,  // our parent window is the first splitter
		                             1, 2,            // the new splitter is 1 row, 2 columns
									 WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
									 m_wndSplitter.IdFromRowCol(1, 0)
									// new splitter is in the 2nd row, 1st column of first splitter
									 ))
		return FALSE;
	
//	if ( ! m_wndSplitter2.CreateView(0,0,
//									 RUNTIME_CLASS(CCastStringEditorGanttView),
//									 CSize(400,200),
//									 pContext) )
//		return FALSE;

	if ( ! m_ganttDisplay.Create(&m_wndSplitter2,
								 RUNTIME_CLASS(CCastStringEditorGanttView),
								 CSize(40,200),
								 CSize(150,200),
								 pContext,
								 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
								 m_wndSplitter.IdFromRowCol(0,1)) ) 
		return FALSE;

	if ( ! m_wndSplitter2.CreateView(0,0,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(600,200),
									 pContext) )
		return FALSE;

	m_pTabWnd2 = (SECTabWnd*) m_wndSplitter2.GetPane(0,0);
	
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorStrandSummaryView),
					   "Summary", 
					   pContext);
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorScenMgrView),
					   "ScenMgr", 
					   pContext);
	m_pTabWnd2->AddTab(RUNTIME_CLASS(CCastStringEditorValidnView),
					   "Validn",
					   pContext);

	m_pTabWnd2->ActivateTab(0);
	m_pTabWnd2->ScrollToTab(0);
#endif // used to


	return TRUE;
}



int CCastStringEditorChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CChildFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//EnableDocking(CBRS_ALIGN_ANY);

	//if (!m_dlgBar.Create(this, IDD_SCENARIO_DLGBAR,
	//	CBRS_BOTTOM|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_SCENARIO_DLGBAR))
	//{
	//	TRACE0("Failed to create DlgBar\n");
	//	return -1;      // fail to create
	//}

	return 0;

	SetTimer(1000,15000,0);
}

void CCastStringEditorChildFrame::ActivateFrame(int nCmdShow) 
{
	CChildFrame::ActivateFrame(SW_SHOWMAXIMIZED);
}

 

void CCastStringEditorChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CChildFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	static bool activated = false;
	if ( ! activated ) {
		activated = true;
		CCastStringEditorActivatedMsg msg;
		TheBus().UpdateAllObservers(0,&msg);
	}
}


void CCastStringEditorActivatedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: activated");
}
