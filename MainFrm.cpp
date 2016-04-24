// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "csda.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "toolbars.h"
#include "ScenMgr.h"
#include "resource2.h"



/////////////////////////////////////////////////////////////////////////////
//	CMainFrame
//
//  We use the Stingray WDI - workbook document interface.
//  See Stingray's documentation for more details.
//  Most important to this is that the main frame for the application
//    be an SECWorkbook and each CMDIChildFrame be an SECWorksheet.
//  See class CChildFrame for more information on the SECWorksheet part.
//
//  CMainFrame is the main frame for the application.
//  It is an SECWorkbook.
//  Here we construct the main components of the outer window,
//    such as special toolbars and output windows.
//  The main frame also hold the timer to check asynchronously for
//    such things as auto-save of scenarios.
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CMainFrame, SECWorkbook)

BEGIN_MESSAGE_MAP(CMainFrame, SECWorkbook)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_OUTPUT_WINDOW, OnViewOutputWindow)
	ON_COMMAND(ID_VIEW_VIEW_OUTPUT_WINDOW, OnViewOutputWindow)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SHOW_OUTPUT_WINDOW,OnShowOutputWindow)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction



CMainFrame::CMainFrame()
{
	// Note: these are members of the base class!
	// We need to do these initializations to get toolbars and menubars
	//    working properly in the context of SECWorkbook.
	m_pControlBarManager = new SECToolBarManager(this);
	m_pMenuBar           = new SECMDIMenuBar;	// or SECMenuBar for SDI

	// Enable bitmap menu support.
	EnableBmpMenus();

	m_timerId = 0;
	
}

CMainFrame::~CMainFrame()
{
	delete m_pControlBarManager;
	delete m_pMenuBar;
}



void CMainFrame::OnClose() 
{
	// If not using a workspace manager, this code will 
	// save the position of the toolbars on app shutdown (not as powerful as workspace
	// manager, but a little easier to implement). Also Load the state
	// in the OnCreate handler.
	// SaveBarState(_T("VizBarState"));
	// SECToolBarManager* pToolBarMgr=(SECToolBarManager *)m_pControlBarManager;	
	// pToolBarMgr->SaveState(_T("VizBarState"));
	
	SECWorkbook::OnClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (SECWorkbook::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Load the master bitmap for ALL toolbars administrated by the
	// toolbar manager (and the large bitmap counterpart). All toolbars
	// (now and future) will be indices into this bitmap.
	SECToolBarManager* pToolBarMgr=(SECToolBarManager *)m_pControlBarManager;	
	VERIFY(pToolBarMgr->LoadToolBarResource(MAKEINTRESOURCE(IDR_MAINFRAME2),
											MAKEINTRESOURCE(IDR_MAINFRAME2_LG)));

	// configure the button map for drawing non-standard buttons
	pToolBarMgr->SetButtonMap(btnMap);


	// establish the default toolbar groupings, these button groupings
	// may be radically different once a workspace is loaded 
//	pToolBarMgr->DefineDefaultToolBar(AFX_IDW_TOOLBAR,
//									  _T("File"),
//									  NUMELEMENTS(fileButtons),
//									  fileButtons,
//									  CBRS_ALIGN_ANY,
//									  AFX_IDW_DOCKBAR_TOP);
//
//	pToolBarMgr->DefineDefaultToolBar(AFX_IDW_TOOLBAR + 5,
//									  _T("Edit"),
//									  NUMELEMENTS(editButtons),
//									  editButtons,
//									  CBRS_ALIGN_ANY,
//									  AFX_IDW_DOCKBAR_TOP,
//									  AFX_IDW_TOOLBAR);

//	pToolBarMgr->DefineDefaultToolBar(AFX_IDW_TOOLBAR + 6,
//									  _T("ScenMRU1"),
//									  NUMELEMENTS(scenMru1Buttons),
//									  editButtons,
//									  CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM,
//									  AFX_IDW_DOCKBAR_TOP,
//									  AFX_IDW_TOOLBAR);

	// the boring old toolbars won't cut it, let's see something cool!
	pToolBarMgr->EnableCoolLook(TRUE);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	EnableDocking(CBRS_ALIGN_ANY);


	// Initialize menubar support
	// Warning! Warning! Warning!
	// If you add any new doc/scen/views that use a new menu resource,
	//   you must add that menu resource ID to these two calls.

	LoadAdditionalMenus(6,IDR_SCEN_MGR,
						  IDR_SCEN_MASTER,
						  IDR_SCEN_LIST_MASTER,
						  IDR_HM_LOAD_GRAPH,
						  IDR_CASTER_SCEN,
						  IDR_CAST_STRING_EDITOR);
	pToolBarMgr->SetMenuInfo(8,IDR_MAINFRAME,
							    IDR_CSDATYPE,
								IDR_SCEN_MGR,
								IDR_SCEN_MASTER,
								IDR_SCEN_LIST_MASTER,
								IDR_HM_LOAD_GRAPH,
								IDR_CASTER_SCEN,
								IDR_CAST_STRING_EDITOR);


	//  Create the dialog bar holding the output windows

	if (!m_wndOutput.Create(this, _T("Output Window"),
		CBRS_BOTTOM | WS_VISIBLE | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS,
		CBRS_EX_STDCONTEXTMENU | CBRS_EX_ALLOW_MDI_FLOAT | CBRS_EX_COOL | CBRS_EX_BORDERSPACE,
		ID_OUTPUTWINDOW)) {
		TRACE(_T("Failed to create dialog bar\n"));
		return -1;      
	} 
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarEx(&m_wndOutput, AFX_IDW_DOCKBAR_BOTTOM, 0, 0, (float)1.0, 300);
//	FloatControlBar(&m_wndOutput,CPoint(100,100),0);


	// Comment out if you don't want the application to start in
	// workbook mode.

	SetWorkbookMode(TRUE);

	// Use the gradient caption
	//EnableCustomCaption(TRUE);


	// Call this to position the default toolbars as configured by
	// the DefineDefaultToolBar	commands above. Don't do this
	// if you are going immediately use LoadBarState/LoadState below,
	// as these functions will call it anyways on nonexistant state info.
	pToolBarMgr->SetDefaultDockState();


	//  Create the CMruOneScenDlgBar

	if ( ! m_mru1Bar.Create(this,
							IDD_DLG_MRU_SCEN1,
							CBRS_TOP 
							| WS_VISIBLE 
							| CBRS_SIZE_DYNAMIC 
							| CBRS_TOOLTIPS,
							CBRS_EX_STDCONTEXTMENU 
							| CBRS_EX_ALLOW_MDI_FLOAT 
							//| CBRS_EX_COOL 
							| CBRS_EX_BORDERSPACE,
							IDD_DLG_MRU_SCEN1) ) {
		TRACE(_T("Failed to create dialog bar\n"));
		return -1;
	}
	m_mru1Bar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarEx(&m_mru1Bar,AFX_IDW_DOCKBAR_TOP,1,1,(float)0.5,30);


	// Initialize the extended workspace mgr. Set the second parm to TRUE
	// to save workspaces to the registry. See Also the WrkspcEx sample.
	InitWorkspaceMgrEx(_T("Software\\AIP\\CSDA\\Editor"),FALSE);

	// If not using a workspace manager, these 2 lines of code will 
	// position the toolbars on app startup (not as powerful as workspace
	// manager, but a little easier to implement). Also Save the state
	// in the OnClose handler.
	// LoadBarState(_T("VizBarState"));
	// pToolBarMgr->LoadState(_T("VizBarState"));


	// TODO: make the time constant editable by the user.
	m_timerId = 0;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return SECWorkbook::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
#ifdef MDI_ENABLED
	//  Here is the code for standard MDI
	CMDIFrameWnd::AssertValid();
#endif

	// Here is the code for SEC's WDI
	SECWorkbook::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
#ifdef MDI_ENABLED
	//  Here is the code for standard MDI
	CMDIFrameWnd::Dump(dc);
#endif

	// Here is the code for SEC's WDI
	SECWorkbook::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


// ## Change UINT to UINT_PTR
void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// don't know how long it will take to do the action here,
	//  so disable the timer so that we don't get another timer event 
	//  while doing this.

	DeactivateTimer();

	//  Perform timer-based actions.

	CScenMgr::ArchiveAllScens();
	TheSnapshot.LoadNew910s();

	//  Get ready to do it again.

	ActivateTimer();
}

void CMainFrame::ActivateTimer()
{	
	if ( m_timerId != 0 )
		// timer already activated
		return;
	
	m_timerId = SetTimer(1000,UINT(10*60*1000),0);
}

void CMainFrame::DeactivateTimer()
{
	if ( m_timerId == 0 )
		// no timer active
		return;

	KillTimer(m_timerId);
	m_timerId = 0;
}

//  We sometimes need to bring up the output window from a different (worker) thread.
//  We do that by sending a message, WM_USER_SHOW_OUTPUT_WINDOW, which this handles.

// ## change return type to LRESULT
LRESULT CMainFrame::OnShowOutputWindow(WPARAM wp, LPARAM lp)
{
	ShowMainOutputControlBar();
	return 0;
}


//  Bring up the output control bar

void CMainFrame::ShowMainOutputControlBar()
{
	ShowControlBar(&m_wndOutput,TRUE,FALSE);
}






void CMainFrame::OnViewOutputWindow() 
{
	ShowMainOutputControlBar();	
}
