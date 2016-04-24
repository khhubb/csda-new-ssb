// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "ReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog
//
//	Display an HTML-based report in an embedded browser window.
//  Optionally allow printing.
//
//	Usage:
//
//		CReportDlg dlg;
//		dlg.m_filename = <path of file to be printed>;
//		dlg.DoModal();
//
//////////////////////////////////////////////////////////////////////////////


CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_enablePrint = false;
}


void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDlg)
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_EXPLORER1, m_browser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	//{{AFX_MSG_MAP(CReportDlg)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDlg message handlers


//
//  Set up the layout of the window
//

void CReportDlg::InitLayout()
{
	//	The layout is fairly simple
	//
	//         +------------------------------+
	//	       |                              |
	//         | browser                      |
	//	       |                              |
	//         +------------------------------+
	//         | some buttons                 |
	//         +------------------------------+
	//  
	//	we want the buttons area to be of fixed size.
	//  Outermost is a relative pane.
	//	It contains a node for the buttons, fixed size.
	//  It contains a node for the browser, attached.
	//
	//	Here goes.


	// Create the outermost node, a relative node

	SECLNRelative* pRel
		= (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));

	


	// create a general layout node to hold the buttons.
	// the initial positions will do just fine.

	SECLayoutNode* pBtnsNode = m_LFactory.CreateNode(NODE_CLASS(SECLayoutNode));

	UINT uids[] = { IDOK, IDC_BUTTON_PRINT };
	m_LFactory.CreateNodeWndArray(uids,sizeof(uids)/sizeof(UINT),this,pBtnsNode);
	pBtnsNode->SetInitialRect();
	pBtnsNode->ModifyNodeStyleEx(0,SEC_LNODE_EX_NOSIZE_VERT,TRUE);

	// create a wnd node to hold the browser control
	
	// Note: this fails because CreateNodeWnd calls GetDescendantWindow,
	//   which fails to find IDC_EXPLORER1.  GetDlgItem finds it, so it does exist.
	// I have no explanation.
	// In other words, in the following code
	//		CWnd* pWnd1 = GetDescendantWindow(IDC_EXPLORER1);
	//		CWnd* pWnd2 = GetDlgItem(IDC_EXPLORER1);
	// pWnd1 will be 0, pWnd2 will not.


	SECLayoutNodeWnd* pBrowser = m_LFactory.CreateNodeWnd( IDC_EXPLORER1, this );


	//  Add buttons and browser to rel node and set the constraints
	pRel->AddLayoutNode(pBtnsNode);
	pRel->AddLayoutNode(pBrowser);

	pRel->SetConstraint( pBtnsNode, REL_MOVEL,  pRel,      REL_LEFT,    10 );
	pRel->SetConstraint( pBtnsNode, REL_MOVEB,  pRel,      REL_BOTTOM, -10 );
	pRel->SetConstraint( pBrowser,  REL_MOVEL,  pRel,      REL_LEFT,    10 );
	pRel->SetConstraint( pBrowser,  REL_MOVEB,  pBtnsNode, REL_TOP,    -10 );
	pRel->SetConstraint( pBrowser,  REL_RIGHT,  pRel,      REL_RIGHT,  -10 );
	pRel->SetConstraint( pBrowser,  REL_TOP,    pRel,      REL_TOP,     10 );

	pRel->SetInitialRect();


	SECLayoutWndListener* pListener = m_LFactory.CreateLayoutWndListener();
	pListener->AutoInit(pRel,this);
}

BOOL CReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// initialize window layout

	InitLayout();


	//  show/hide the print button
	//  We don't actually use this anymore, in favor of an activex control
	//    we put on pages that allows the user to print.

	m_btnPrint.ShowWindow(m_enablePrint?SW_SHOW:SW_HIDE);


	//  Make the embedded browser display the page in question

	COleVariant empty;
	m_browser.Navigate(LPCTSTR(m_filename),empty,empty,empty,empty);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_EVENTSINK_MAP(CReportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CReportDlg)
	ON_EVENT(CReportDlg, IDC_EXPLORER1, 104 /* DownloadComplete */, OnDownloadCompleteExplorer1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


//
//  Initially the print button is disabled (if it is visible at all).
//  Enable it after we have a document to display
//

void CReportDlg::OnDownloadCompleteExplorer1() 
{
	m_btnPrint.EnableWindow(TRUE);	
	
}


//
// Standard code from Microsoft to make an embedded browser print
//

void CReportDlg::OnButtonPrint() 
{
	OLECMDF query;

	query = static_cast<OLECMDF>(m_browser.QueryStatusWB(OLECMDID_PRINT));

	if ( query & OLECMDF_ENABLED ) {

		try
		{
			m_browser.ExecWB(OLECMDID_PRINT,OLECMDEXECOPT_PROMPTUSER,0,0);
		}
		catch (COleDispatchException* pExcept)
		{
            // if the error code is nonzero we want the error to occur
            if (pExcept->m_wCode) {
				pExcept->Delete ();
               throw;
            }
            // if the error code is zero then we just eat it
            else
               pExcept->Delete (); 
		}
	}	
	else
		MessageBox("Can't print!");
}
