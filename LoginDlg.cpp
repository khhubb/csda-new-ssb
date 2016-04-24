// LoginDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "UsersSet.h"
#include "UserInfo.h"
#include "DBConn.h"


/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog
//
//	Used at system startup to identify the user.
//  Looks up user names in database and displays in dropdowns.
//  Verifies password.
//
//  
//  Usage:
//
//		CLoginDlg loginDlg;
//
//		if ( loginDlg.DoModal() == IDOK ) {
//			// login successful
//			// loginDlg.m_pUserInfo has the information about the logged-in user
//			// for example:
//			CUserInfo::TheUser = *loginDlg.m_pUserInfo;	// copy
//			// note that m_pUserInfo will be deleted when loginDlg is destroyed.
//		}
//		else {
//			// login unsuccessful
//		}
//
//
///////////////////////////////////////////////////////////////////////////////


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_password = _T("");
	m_username = _T("");
	//}}AFX_DATA_INIT

	m_pUserInfo = 0;
}


CLoginDlg::~CLoginDlg()
{
	if ( m_pUserInfo != 0 ){
		delete m_pUserInfo;
		m_pUserInfo = 0;
	}
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_USERNAME, m_comboUsername);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_CBString(pDX, IDC_USERNAME, m_username);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers



BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// populate combo box with names of users

	m_comboUsername.ResetContent();

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();

	CUsersSet us(dbc.GetDB());

	try 
	{

		us.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);

		while ( ! us.IsEOF() ) {

			m_comboUsername.AddString(us.m_LoginName);
		
			us.MoveNext();
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	}

	if ( us.IsOpen() )
		us.Close();

	// make sure this window pops to the top
	SetForegroundWindow();
 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CLoginDlg::OnOK() 
{

	// okay, let's check out the password

	if ( ! UpdateData(true) )
		return;

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();

	CUsersSet us(dbc.GetDB());

	try 
	{
		CString query;
		query.Format("Select * from [Users] where [LoginName] = '%s'",
					 (LPCTSTR)m_username);
		us.Open(CRecordset::snapshot,query,CRecordset::readOnly);

		if ( us.IsEOF() && us.IsBOF() ) {
			// missing record
			AfxMessageBox("Wonder of wonders!\n"
				          "That user is not in the database.\n"
						  "Try again.");
			us.Close();
			return;
		}

		if ( us.m_Password != m_password ) {

			AfxMessageBox("Incorrect password, try again.");
			us.Close();
			return;
		}

		// password okay, record the user's info.
		m_pUserInfo = new CUserInfo(us.m_LoginName,
									us.m_LastName,
									us.m_FirstName,
									us.m_SBId);
		us.Close();
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   
	   if ( us.IsOpen() )
		   us.Close();

	   return;
	}

	CDialog::OnOK();
}

