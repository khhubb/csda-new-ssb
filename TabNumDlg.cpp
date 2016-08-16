/////////////////////////////////////////////////////////////////////////////////////////////
// TabNumDlg.cpp : implementation file
//
// K. Hubbard     03-08-10   Prevent Tab Numbers created that exceed 5 digits, to prevent handoff malfunctions.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "TabNumDlg.h"
#include "Caster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabNumDlg dialog
//
// User can enter a tab #.
// Enforces proper first and second digits.
//
//
//	Usage:
//
//		CTabNumDlg dlg;
//
//		dlg.m_caster = <the caster we are picking the tab# for>;
//
//		if ( dlg.DoModal() == IDOK ) {
//			//  dlg.m_tabNum has the user's selection
//		}
//
//
/////////////////////////////////////////////////////////////////////////////


CTabNumDlg::CTabNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabNumDlg)
	m_tabNum = 0;
	//}}AFX_DATA_INIT
}


void CTabNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabNumDlg)
	DDX_Text(pDX, IDC_EDIT_TAB_NUM, m_tabNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabNumDlg, CDialog)
	//{{AFX_MSG_MAP(CTabNumDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabNumDlg message handlers



BOOL CTabNumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( Caster::IsValidCasterValue(m_caster) );

	SetWindowText(m_prompt);

	// CASTER TODO: do we force second digit on tab# for casters 4,5?
	m_force2ndDigit = m_caster != 1;

	InitTabNum();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTabNumDlg::InitTabNum()
{
	CString str;

	// CASTER TODO:  What is starting digit for tab#s for casters 4,5?
	if ( m_caster == 3 )
		str = "3";
	else if ( m_caster == 2 )
		str = "2";
	else str = "4";
	
	if ( m_force2ndDigit )
		str += "9";

	m_tabNum = atol(LPCTSTR(str));

}



void CTabNumDlg::OnOK() 
{
	if ( ! UpdateData(TRUE) )
		return;

	if ( ! ValidateTabNum() )
		return;

	CDialog::OnOK();
}


bool CTabNumDlg::ValidateTabNum()
{
	if ( m_tabNum == 0 )
		return true;

	CString str;

	str.Format("%05d",m_tabNum);
	
	char firstDigit = str[0];

	// CASTER TODO: What are valid tab #s for casters 4,5?

	if ( ! (firstDigit == '9' 
			||
			(m_caster == 1  && firstDigit != 2 && firstDigit != 3)
			||
			firstDigit == '2' && m_caster == 2 
			||
			firstDigit == '3' && m_caster == 3 ) ) {
	
		MessageBox("First digit invalid for caster.",
				   "Bad TAB #",
					MB_ICONHAND | MB_OK);
		return false;
	}

//	if ( m_force2ndDigit && str[1] != '9' ) {
//		MessageBox("Second digit must be 9 (for slabs at #2BOF).",
//				   "Bad TAB #",
//					MB_ICONHAND | MB_OK);
//		return false;
//	}

//   Tab number control test 3-15-10 k. hubbard
	if ( m_tabNum >= 99999 ) {
		MessageBox("Too many digits. Tab Number must be 5 digits only.",
				   "Bad TAB #",
					MB_ICONHAND | MB_OK);
		return false;
	}


	return true;
}

