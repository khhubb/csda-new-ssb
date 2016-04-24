// HMLoadGraphChildFrame.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "HMLoadGraphChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HMLoadGraphView.h"
#include "HMLoadGraphScenSeqView.h"
#include "HMLoadGraphDoc.h"
#include "HMLoadGraphSheetView.h"


/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphChildFrame

IMPLEMENT_DYNCREATE(CHMLoadGraphChildFrame, CChildFrame)

CHMLoadGraphChildFrame::CHMLoadGraphChildFrame()
{
}

CHMLoadGraphChildFrame::~CHMLoadGraphChildFrame()
{
}


BEGIN_MESSAGE_MAP(CHMLoadGraphChildFrame, CChildFrame)
	//{{AFX_MSG_MAP(CHMLoadGraphChildFrame)
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_VIEW_SCENARIOS, OnViewScenarios)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphChildFrame message handlers


static UINT BASED_CODE buttons[] = 
{
	// same order as in the bitmap 'toolbar.bmp'
#if 0
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_SEPARATOR,
#endif

	ID_EDIT_CREATE_STRING,
	ID_VIEW_SET_DATE,

	ID_SEPARATOR,

	ID_VIEW_DAY_PLUS,
	ID_VIEW_DAY_MINUS,
	ID_VIEW_WEEK_PLUS,
	ID_VIEW_WEEK_MINUS,

	ID_SEPARATOR,

	ID_VIEW_ADD_ROW,
	ID_VIEW_DEL_ROW,
	ID_VIEW_ADD_COL,
	ID_VIEW_DEL_COL,

	ID_SEPARATOR,
	ID_VIEW_SHOW910S,

	ID_SEPARATOR,

	ID_VIEW_SCENARIOS,
	ID_EDIT_LOADHM,
	ID_EDIT_SENDHM,

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

BOOL CHMLoadGraphChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if ( ! m_wndSplitter.CreateStatic(this,1,2) )
		return FALSE;


	if ( ! m_wndSplitter.CreateView(0,0,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(150,650),
									 pContext) )
		return FALSE;

	m_pTabWnd2 = (SECTabWnd*) m_wndSplitter.GetPane(0,0);

	SECTab* pTab1 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#1SC",
									   pContext);
	SECTab* pTab2 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#2SC",
									   pContext);
	SECTab* pTab3 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#3CC",
									   pContext);

	((CHMLoadGraphScenSeqView*)pTab1->m_pClient)->SetCaster(1);
	((CHMLoadGraphScenSeqView*)pTab2->m_pClient)->SetCaster(2);
	((CHMLoadGraphScenSeqView*)pTab3->m_pClient)->SetCaster(3);

	m_pTabWnd2->ActivateTab(0);
	m_pTabWnd2->ScrollToTab(0);


	if ( ! m_wndSplitter.CreateView(0,1,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(700,800),
									 pContext) )
		return FALSE;

	m_pTabWnd3 = (SECTabWnd*) m_wndSplitter.GetPane(0,1);

	m_pTabWnd3->AddTab(RUNTIME_CLASS(CHMLoadGraphSheetView),
					   "Icon",
					   pContext);


	if ( ! m_ganttDisplay.Create(m_pTabWnd3,
								 RUNTIME_CLASS(CHMLoadGraphView),
								 CSize(40,800),
								 CSize(700,800),
								 pContext,
								 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
								 m_wndSplitter.IdFromRowCol(0,1)) ) 
		return FALSE;

	m_pTabWnd3->AddTab(m_ganttDisplay.GetSplitter(),"Gantt");

	m_pTabWnd3->ActivateTab(0);
	m_pTabWnd3->ScrollToTab(0);



	
	if ( ! m_wndToolBar.Create(this) 
		 ||
		 ! m_wndToolBar.LoadBitmap(IDR_HM_LOAD_GRAPH)
		 ||
		 ! m_wndToolBar.SetButtons(buttons,sizeof(buttons)/sizeof(UINT)) ) 
		 return FALSE;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY );

	m_wndSplitter.SetColumnInfo(0,250,150);


#if 0
	// before getting rid of CHMLoadGraphScenMgrView

	if ( ! m_wndSplitter.CreateStatic(this,1,2) )
		return FALSE;


	if ( ! m_wndSplitter2.CreateStatic(&m_wndSplitter,	// our parent window if the first splitter
									   2,1,				// the new splitter is 2 rows, 1 column
									   WS_CHILD | WS_VISIBLE | WS_BORDER,	// style, WS_BORDER is needed
									   // new spliiter is in 1st row, 1st column of first splitter
									   m_wndSplitter.IdFromRowCol(0,0) ) )
		return FALSE;

	if ( ! m_wndSplitter2.CreateView(0,0,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(150,650),
									 pContext) )
		return FALSE;

	m_pTabWnd2 = (SECTabWnd*) m_wndSplitter2.GetPane(0,0);

	SECTab* pTab1 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#1SC",
									   pContext);
	SECTab* pTab2 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#2SC",
									   pContext);
	SECTab* pTab3 = m_pTabWnd2->AddTab(RUNTIME_CLASS(CHMLoadGraphScenSeqView),
									   "#3CC",
									   pContext);

	((CHMLoadGraphScenSeqView*)pTab1->m_pClient)->SetCaster(1);
	((CHMLoadGraphScenSeqView*)pTab2->m_pClient)->SetCaster(2);
	((CHMLoadGraphScenSeqView*)pTab3->m_pClient)->SetCaster(3);

	m_pTabWnd2->ActivateTab(0);
	m_pTabWnd2->ScrollToTab(0);

	if ( ! m_wndSplitter2.CreateView(1,0,
									 RUNTIME_CLASS(CHMLoadGraphScenMgrView),
									 CSize(150,150),
									 pContext) )
		return FALSE;


	if ( ! m_wndSplitter.CreateView(0,1,
									 RUNTIME_CLASS(SECTabWnd),
									 CSize(700,800),
									 pContext) )
		return FALSE;

	m_pTabWnd3 = (SECTabWnd*) m_wndSplitter.GetPane(0,1);


	//if ( ! m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CHMLoadGraphView),CSize(800,800),pContext) )
	//	return FALSE;

	//if ( ! m_ganttDisplay.Create(&m_wndSplitter,
	//							 RUNTIME_CLASS(CHMLoadGraphView),
	//							 CSize(40,800),
	//							 CSize(700,800),
	//							 pContext,
	//							 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
	//							 m_wndSplitter.IdFromRowCol(0,1)) ) 
	//	return FALSE;

	m_pTabWnd3->AddTab(RUNTIME_CLASS(CHMLoadGraphSheetView),
					   "Icon",
					   pContext);


	if ( ! m_ganttDisplay.Create(m_pTabWnd3,
								 RUNTIME_CLASS(CHMLoadGraphView),
								 CSize(40,800),
								 CSize(700,800),
								 pContext,
								 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
								 m_wndSplitter.IdFromRowCol(0,1)) ) 
		return FALSE;

	m_pTabWnd3->AddTab(m_ganttDisplay.GetSplitter(),"Gantt");

	m_pTabWnd3->ActivateTab(0);
	m_pTabWnd3->ScrollToTab(0);



	
	if ( ! m_wndToolBar.Create(this) 
		 ||
		 ! m_wndToolBar.LoadBitmap(IDR_HM_LOAD_GRAPH)
		 ||
		 ! m_wndToolBar.SetButtons(buttons,sizeof(buttons)/sizeof(UINT)) ) 
		 return FALSE;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY );

	m_wndSplitter.SetColumnInfo(0,250,150);
	m_wndSplitter2.SetColumnInfo(0,250,150);
	m_wndSplitter2.SetRowInfo(0,500,500);
	m_wndSplitter2.SetRowInfo(1,150,150);

#endif // before

//	WINDOWPLACEMENT plc;
//	plc.showCmd = SW_SHOWMAXIMIZED;
//
//	SetWindowPlacement(&plc);

	return TRUE;
}




void CHMLoadGraphChildFrame::ActivateFrame(int nCmdShow) 
{	
	CChildFrame::ActivateFrame(SW_SHOWMAXIMIZED);
}

void CHMLoadGraphChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CDocument* pDoc = GetActiveDocument();

	if( pDoc != 0 
	    &&
		pDoc->IsKindOf( RUNTIME_CLASS( CHMLoadGraphDoc ) ) ) 


		((CHMLoadGraphDoc*)pDoc)->ActivationChange(bActivate);

	
	CChildFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	
}

void CHMLoadGraphChildFrame::OnViewScenarios() 
{
	// TODO: Add your command handler code here
	
}
