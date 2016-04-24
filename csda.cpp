// csda.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "csda.h"

#include "MainFrm.h"
#include "ChildFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MiscConstants.h"
#include "PSheetMiscConstants.h"

#include "Snapshot.h"
#include "ScenMgr.h"

#include "ScenMgrDoc.h"
#include "ScenMgrView.h"
#include "ScenMgrChildFrame.h"

#include "ScenMasterDoc.h"
#include "ScenMasterGraphView.h"
#include "ScenMasterChildFrame.h"

#include "HMLoadGraphDoc.h"
#include "HMLoadGraphView.h"
#include "HMLoadGraphScenSeqView.h"
#include "HMLoadGraphChildFrame.h"

#include "CastStringEditorDoc.h"
#include "CastStringEditorTextView.h"
#include "CastStringEditorChildFrame.h"

#include "LoginDlg.h"
#include "UserInfo.h"
#include "InitialScenLoaderDlg.h"
#include "CasterScenArchiver.h"



/////////////////////////////////////////////////////////////////////////////
// CCsdaApp

BEGIN_MESSAGE_MAP(CCsdaApp, CWinApp)
	//{{AFX_MSG_MAP(CCsdaApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_SCENARIO_MANAGER, OnViewScenarioManager)
	ON_COMMAND(ID_VIEW_MISC_CONSTANTS, OnViewMiscConstants)
	//}}AFX_MSG_MAP
	// Standard file based document commands

	//	I commented out these entries to prevent standard processing

	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCsdaApp construction

CCsdaApp::CCsdaApp()
{
	m_pMainFrame = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCsdaApp object

CCsdaApp TheApp;

CCsdaApp* GetMyApp() { return &TheApp;	}



// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {F60F1185-A60D-11D0-975C-0000C0C2484C}
static const CLSID clsid =
{ 0xf60f1185, 0xa60d, 0x11d0, { 0x97, 0x5c, 0x0, 0x0, 0xc0, 0xc2, 0x48, 0x4c } };

/////////////////////////////////////////////////////////////////////////////
// CCsdaApp initialization

BOOL CCsdaApp::InitInstance()
{
	// begin standard boilerplate from the AppWizard,
	//   with a few unneccesary things commented out.
	// look further down for actions more specific to this app.

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.

	// We check the command line for an entry of the form /RegKey=CSDA or /RegKey=CSDATest

	SetRegistryKey(_T("AIP"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.


	//  Ok.
	//  Here is where we begin

	// ObjectGrid initialization

	GXInit();

	//  Create templates for all the basic doc/frame/view combinations
	//    that appear as top-level windows.
	//  We will open these up down below.

	m_pScenMgrDocTmplt
		= new CMultiDocTemplate(IDR_SCEN_MGR,
								RUNTIME_CLASS(CScenMgrDoc),
								RUNTIME_CLASS(CScenMgrChildFrame),
								RUNTIME_CLASS(CScenMgrView));
	AddDocTemplate(m_pScenMgrDocTmplt);


	m_pScenMasterDocTmplt
		= new CMultiDocTemplate(IDR_SCEN_MASTER,
							    RUNTIME_CLASS(CScenMasterDoc),
								RUNTIME_CLASS(CScenMasterChildFrame),
								RUNTIME_CLASS(CScenMasterGraphView));
	AddDocTemplate(m_pScenMasterDocTmplt);

	m_pHMLoadGraphDocTmplt
		= new CMultiDocTemplate(IDR_HM_LOAD_GRAPH,
								RUNTIME_CLASS(CHMLoadGraphDoc),
								RUNTIME_CLASS(CHMLoadGraphChildFrame),
								RUNTIME_CLASS(CHMLoadGraphScenSeqView));
	AddDocTemplate(m_pHMLoadGraphDocTmplt);


	m_pCastStringEditorDocTmplt
		= new CMultiDocTemplate(IDR_CAST_STRING_EDITOR,
							    RUNTIME_CLASS(CCastStringEditorDoc),
								RUNTIME_CLASS(CCastStringEditorChildFrame),
								RUNTIME_CLASS(CCastStringEditorTextView));
	AddDocTemplate(m_pCastStringEditorDocTmplt);



	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	//m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	//COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	// note that we store it in our own data member 
	//   because we need to access some methods 
	//   specific to its particular class.

	m_pMainFrame = new CMainFrame;
	if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = m_pMainFrame;


	// Enable drag/drop open
	//m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	//EnableShellOpen();
	//RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	//  DON'T display a new MDI child window during startup !!!

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	//  Much automation/server code commented out below.

	// Check to see if launched as OLE server
	//if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	//{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
	//	return TRUE;
	//}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	//m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	//COleObjectFactory::UpdateRegistryAll();


	// We DON'T need this
	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	// The main window has been initialized, so show and update it.
	m_pMainFrame->ShowWindow(/*m_nCmdShow*/ SW_SHOWMAXIMIZED);
	m_pMainFrame->UpdateWindow();


	//  Do the login dance with the user.

	CLoginDlg loginDlg;

	if ( loginDlg.DoModal() != IDOK )
		return FALSE;

	// Logged in successfully, so record the info.

	CUserInfo::TheUser = *loginDlg.m_pUserInfo;	// copy
	
	if ( ! CCasterScenArchiver::MaybeCreatePublicScensForUser(CUserInfo::TheUser.LoginName()) )
		AfxMessageBox("Error creating public scenarios for user.\nContact system maintainer.");

	
	// initialize misc constants from the database

	// someday, we may do this on a per-user basis.
	// for new, we just use a default user.
	//GlobalConstants.Init(CUserInfo::TheUser.LoginName());
	GlobalConstants.Init("Default");


	// Initialize the snapshot
	if ( ! TheSnapshot.Initialize() ) 
		return FALSE;

	{
		CInitialScenLoaderDlg dlg;
		dlg.DoModal();
	}


	// Eventually, do this before log-in

	// Now create the main editing windows
	// by doing the equivalent of FileNew (open a document file with no name).

	m_pScenMasterDoc		= (CScenMasterDoc*)			m_pScenMasterDocTmplt->OpenDocumentFile(0);
	m_pScenMgrDoc			= (CScenMgrDoc*)			m_pScenMgrDocTmplt->OpenDocumentFile(0);
	m_pHMLoadGraphDoc		= (CHMLoadGraphDoc*)		m_pHMLoadGraphDocTmplt->OpenDocumentFile(0);
	m_pCastStringEditorDoc	= (CCastStringEditorDoc*)	m_pCastStringEditorDocTmplt->OpenDocumentFile(0);


	//  Activate the CastStringEditor as the first view to be seen.


	POSITION posn = m_pCastStringEditorDoc->GetFirstViewPosition();
	CView* pView = m_pCastStringEditorDoc->GetNextView(posn);
	CWnd* pParent = pView->GetParentFrame();
	
	if ( pParent->IsKindOf(RUNTIME_CLASS(CFrameWnd)) ) {
		((CFrameWnd*)pParent)->ActivateFrame();
		((CFrameWnd*)pParent)->SetActiveView(pView);
	}


	m_pMainFrame->FloatControlBar(&m_pMainFrame->OutputBar(),CPoint(100,100),0);


	// Start the timer for cross-user notification and background checks.
	m_pMainFrame->ActivateTimer();


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCsdaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCsdaApp commands

int CCsdaApp::ExitInstance() 
{

	// Let's clean up the mess we've made.

	//we're quitting, why bother with this-- causes lots of updates that we don't want.

	//CScenMgr::DeleteAllProdnScens();

	TheSnapshot.SpecMgr().CleanupDispatch();
	//TheSnapshot.CastRulesMgr().CleanupDispatch();

	GXTerminate();

	return CWinApp::ExitInstance();
}


void CCsdaApp::OnViewScenarioManager() 
{

	// TODO -- make sure only one is ever open.

	m_pScenMgrDocTmplt->OpenDocumentFile(0);
	
}

void CCsdaApp::OnViewMiscConstants() 
{
	// TODO -- make sure only one is ever open.

	CPSheetMiscConstants dlg("Edit constants",m_pMainWnd);
	
	dlg.SetMiscConstantsObject(&GlobalConstants);
	
	dlg.DoModal();

}


void CCsdaApp::ActivateCastStringEditor()
{
	//m_pCastStringEditorDoc->ActivateFrame();
}
