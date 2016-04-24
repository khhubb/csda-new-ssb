//// UploadDlg.cpp : implementation file
////
// 
#include "stdafx.h"
//
//#include "csda.h"
//#include "UploadDlg.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
///////////////////////////////////////////////////////////////////////////////
////  CUploadDlg dialog
////
////  Dialog exists solely to display the upload control (remoteDL.ocx)
////
////  Responds to the messages returning from that.
////
////
////  Usage:
////
////      // You already have written upload records to the database
////
////		CUploadDlg uDlg;
////		uDlg.m_caster = <the caster of interest>;
////		
////		if ( IDOK == uDlg.DoModal() ) {
////			// upload succeeded.
////		}
////		else {
////			// user cancelled
////		}
////
////
/////////////////////////////////////////////////////////////////////////////////
//
//
//CUploadDlg::CUploadDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CUploadDlg::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CUploadDlg)
//	//}}AFX_DATA_INIT
//
//	m_caster = 0;
//}
// 
//
//void CUploadDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CUploadDlg)
//	DDX_Control(pDX, IDC_REMOTEDL1, m_remote);
//	//}}AFX_DATA_MAP
//}
//
//
//BEGIN_MESSAGE_MAP(CUploadDlg, CDialog)
//	//{{AFX_MSG_MAP(CUploadDlg)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()
//
///////////////////////////////////////////////////////////////////////////////
//// CUploadDlg message handlers
//
//BEGIN_EVENTSINK_MAP(CUploadDlg, CDialog)
//    //{{AFX_EVENTSINK_MAP(CUploadDlg)
//	ON_EVENT(CUploadDlg, IDC_REMOTEDL1, 3 /* CancelOut */, OnCancelOutRemotedl1, VTS_NONE)
//	ON_EVENT(CUploadDlg, IDC_REMOTEDL1, 2 /* Done */, OnDoneRemotedl1, VTS_NONE)
//	ON_EVENT(CUploadDlg, IDC_REMOTEDL1, 1 /* GotMessage */, OnGotMessageRemotedl1, VTS_PBSTR)
//	//}}AFX_EVENTSINK_MAP
//END_EVENTSINK_MAP()
//
//
//
//BOOL CUploadDlg::OnInitDialog() 
//{
//	assert( m_caster == 1 || m_caster == 2 || m_caster == 3 );
//	
//	CDialog::OnInitDialog();
//	
//	CString str;
//	str.Format("%d",m_caster);
//
//	m_remote.SetCaster(LPCTSTR(str));
//	m_remote.SetLogin("George");
//	m_remote.SetDsn("DPATest");
//	m_remote.SetCommand("Caster");
//	
//	return TRUE;  // return TRUE unless you set the focus to a control
//	              // EXCEPTION: OCX Property Pages should return FALSE
//}
//
//
//
//void CUploadDlg::OnCancelOutRemotedl1() 
//{
//	EndDialog(IDCANCEL);
//	
//}
//
//void CUploadDlg::OnDoneRemotedl1() 
//{
//	EndDialog(IDOK);	
//}
//
//void CUploadDlg::OnGotMessageRemotedl1(BSTR FAR* msg) 
//{
//	
//}
//
