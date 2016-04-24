// MsgDlg.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "MsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog
//
//
//  Simple message box containing a non-editable multi-line text box.
//
//  Can be used modally or non-modally.
//
//  Usage: modal
//
//		CMsgDlg dlg;
//		dlg.m_title = <CString, the title><CString, the title> ;
//		dlg.m_message = <CString, the message to display> ;
//		dlg.DoModal();
//
//
//  Usage: non-modal
//
//		// create
//		CMsgDlg pDlg = new CMsgDlg;
//
//		pMsgDlg->m_title = <CString, the title>;
//		pMsgDlg->m_message = <CString, the message to display initially>;
//		pMsgDlg->Create(0);
//
//
//		// update while displayed
//		pMsgDlg->AddText( <a char*, text to add at the end> );
//
//		// done
//		pMsgDlg->DestroyWindow();
//		delete pMsgDlg;
//
////////////////////////////////////////////////////////////////////////////


CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgDlg)
	m_message = _T("");
	//}}AFX_DATA_INIT
}


void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

BOOL CMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_title);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMsgDlg::AddText(LPCTSTR text)
{
	if ( m_hWnd == 0 )
		return;

	// What is an easy way to determine the length of text.
	// I don't know, but we can play some games with lines

	// kill any selection
	m_edit.SetSel(-1,0);

	// find index of last line.
	int lastLine = m_edit.GetLineCount()-1;
	int firstPos = m_edit.LineIndex(lastLine);

	char buf[1000];
	int count = m_edit.GetLine(lastLine,buf,1000);

	int lastPos = firstPos + count;
	m_edit.SetSel(lastPos,lastPos);
	m_edit.ReplaceSel(text);
	m_edit.SetSel(-1,0);
}


BOOL CMsgDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
