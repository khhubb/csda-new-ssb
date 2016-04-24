// DateTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "DateTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CDateTimeDlg dialog
//
//  Display date/time and allow editing
//  Just embeds a an SECDateTimeCtrl.
//
//  Usage:
//
//		CDateTimeDlg dlg;
//		dlg.m_bDateOnly = <true/false depending>;
//		dlg.m_dateTime = <initial CTime value to display>;
//	 
//	 	if ( dlg.DoModal() == IDOK ) 
//			//dlg.m_dateTime has modified value
//
/////////////////////////////////////////////////////////////////////////////


CDateTimeDlg::CDateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDateTimeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDateTimeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDateTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CDateTimeDlg)
	ON_BN_CLICKED(IDB_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateTimeDlg message handlers



BOOL CDateTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dateTimeCtrl.AttachDateTimeCtrl(IDC_EDIT_DATETIME,
									  this,
									  SEC_DTS_UPDOWN | SEC_DTS_CALENDAR);
	m_dateTimeCtrl.SetDateTime(COleDateTime(m_dateTime.GetTime()));

	if ( m_bDateOnly ) 
		m_dateTimeCtrl.SetFormat(SECDateTimeCtrl::ShortDate);
	else
		m_dateTimeCtrl.SetFormat("MMM/dd/yyyy hh:mm tt");
	m_dateTimeCtrl.EnableFastEntry();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDateTimeDlg::OnReset() 
{
	m_dateTimeCtrl.SetDateTime(COleDateTime(m_dateTime.GetTime()));
}


void CDateTimeDlg::OnOK() 
{
	SYSTEMTIME stime;
	COleDateTime oleDateTime = m_dateTimeCtrl.GetDateTime();
	if ( ! oleDateTime.GetAsSystemTime(stime) ) {
		MessageBox("Wow, what year was that?");
		return;
		//MessageBox("Wow, what year was that?");mmedreturn;
	}
	m_dateTime = CTime(stime);

	CDialog::OnOK();
}
