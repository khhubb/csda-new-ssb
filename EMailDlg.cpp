// EMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "EMailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "msoutl85.h"

/////////////////////////////////////////////////////////////////////////////
//   CEMailDlg dialog
//
//   Allows user to send e-mail
//   Can specify subject/body/to/from fields.
//
//   Usage:
//
//		CEMailDlg dlg;
//	
//		dlg.SetBody( <Some CString -- body text> );
//		ostr.freeze(false);
//	
//		dlg.SetSubject( <Some CString -- subject text> );
//		dlg.AddRecipient( <Some CString -- TO: > );  // can be called multiple times
//		dlg.AddCopy( <Some CString -- CC: > );		 // can be called multiple times
//		if ( dlg.DoModal() == IDOK )
//			// mail was sent
//
//
///////////////////////////////////////////////////////////////////////////////



CEMailDlg::CEMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEMailDlg)
	m_strBody = _T("");
	m_strSubject = _T("");
	//}}AFX_DATA_INIT
}


void CEMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEMailDlg)
	DDX_Text(pDX, IDC_EDIT_BODY, m_strBody);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, m_strSubject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEMailDlg, CDialog)
	//{{AFX_MSG_MAP(CEMailDlg)
	ON_BN_CLICKED(ID_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMailDlg message handlers


BOOL CEMailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);

	// subclass the editable list boxes
    m_LBEditorTo.Initialize(this, IDC_LIST_TO,LBE_DEFAULT);
    m_LBEditorTo.SetWindowText("To:");
    m_LBEditorCC.Initialize(this, IDC_LIST_CC,LBE_DEFAULT);
    m_LBEditorCC.SetWindowText("CC:");

	CFont font;
	font.CreatePointFont(100,"Courier New");
	GetDlgItem(IDC_EDIT_BODY)->SetFont(&font,FALSE);
	font.Detach();

	OnReset();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//
//  Set display to match initial values from user
//

void CEMailDlg::OnReset() 
{
	m_LBEditorTo.GetListBoxPtr()->ResetContent();
	m_LBEditorCC.GetListBoxPtr()->ResetContent();

	for ( vector<CString>::iterator iTo = m_vecTo.begin();
		  iTo != m_vecTo.end();
		  ++iTo ) 
		m_LBEditorTo.GetListBoxPtr()->AddString( LPCTSTR( (*iTo) ) );

	for ( vector<CString>::iterator iCC = m_vecCC.begin();
		  iCC != m_vecCC.end();
		  ++iCC ) 
		m_LBEditorCC.GetListBoxPtr()->AddString( LPCTSTR( (*iCC) ) );

	m_strSubject = m_strSubjectOrig;
	m_strBody = m_strBodyOrig;

	UpdateData(FALSE);
}


//
//  Try to send the e-mail
//

void CEMailDlg::OnOK() 
{
	if ( ! UpdateData(TRUE) )
		return;

	if ( m_LBEditorTo.GetListBoxPtr()->GetCount() == 0 ) {
		MessageBox("You need to have at least one entry in To:");
		return;
	}


	_Application app;

	if ( ! app.CreateDispatch("Outlook.Application") ) {

		MessageBox("Error connecting to Outlook to create email.");
		return;
	}
	
	_MailItem item = 0;
	Recipients recs = 0;
	try
	{	
		// 0 = mailitem
		item.AttachDispatch(app.CreateItem(0));
		recs.AttachDispatch(item.GetRecipients());

		{
			for ( int i=0; i<m_LBEditorTo.GetListBoxPtr()->GetCount(); ++i ) {
				CString str;
				m_LBEditorTo.GetListBoxPtr()->GetText(i,str);
				if ( str != "" )
					recs.Add( LPCTSTR( str ) );
			}
		}

		CString strCC = "";

		{
			for ( int i=0; i<m_LBEditorCC.GetListBoxPtr()->GetCount(); ++i ) {
				CString str;
				m_LBEditorCC.GetListBoxPtr()->GetText(i,str);
				strCC += str;
			}
		}

		if ( strCC != "" )
			item.SetCc( LPCTSTR(strCC) );

		item.SetSubject(m_strSubject);
		item.SetBody(m_strBody);

		try  
		{
			CWaitCursor wc;
			item.Send();
		} 
		catch ( COleDispatchException* e ) 
		{
			MessageBox( e->m_strDescription + "\nError sending e-mail.\nMail probably not sent.",   
						"Sigh",
						MB_ICONEXCLAMATION );
			e->Delete();
		}
	}
	catch ( COleDispatchException* e )
	{
		MessageBox( e->m_strDescription + "\nError initializing e-mail parameters.\nMail not sent.",   
				    "Sigh",
					MB_ICONEXCLAMATION );
		e->Delete();
	}



	if ( recs.m_lpDispatch != 0 )
		recs.ReleaseDispatch();

	if ( item.m_lpDispatch != 0 )
		item.ReleaseDispatch();

	app.ReleaseDispatch();


	CDialog::OnOK();
}




//  These are to be used prior to DoModal 
//    to give initial conditions.

void CEMailDlg::SetSubject(const CString& subj)
{
	m_strSubject = subj;
	m_strSubjectOrig = subj;
}

void CEMailDlg::SetBody(const CString& body)
{	
	m_strBody = body;
	m_strBodyOrig = body;
}

void CEMailDlg::AddRecipient(const CString& strTo)
{
	m_vecTo.push_back(strTo);
}

void CEMailDlg::AddCC(const CString& strCC)
{	
	m_vecCC.push_back(strCC);
}


