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

	m_force2ndDigit = m_caster != Caster::C1;

	InitTabNum();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTabNumDlg::InitTabNum()
{
	CString str;

	switch (m_caster) {
	case Caster::C1:
		str = "4";
		break;

	case Caster::C2:
		str = "29";
		break;

	case Caster::C3:
		str = "39";
		break;

	case Caster::C4:
		str = "11";
		break;

	case Caster::C5:
		str = "52";
		break;

	default:
		assert(false);
	}

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
	char secondDigit = str[1];
	char thirdDigit = str[2];

	bool firstDigitOkay = true;
	bool secondDigitOkay = true;
	bool thirdDigitOkay = true;

	switch (m_caster) {

	case Caster::C1:
	case Caster::C2:
	case Caster::C3:

		// Original code for caster 1,2,3
		{

			if (!(firstDigit == '9'
				||
				(m_caster == Caster::C1  && firstDigit != 2 && firstDigit != 3)
				||
				firstDigit == '2' && m_caster == Caster::C2
				||
				firstDigit == '3' && m_caster == Caster::C3)) {
				firstDigitOkay = false;
			}
		}
		break;

	case Caster::C4:
		firstDigitOkay = firstDigit == '1';
		secondDigitOkay = secondDigit == '1';
		thirdDigitOkay = '0' <= thirdDigit && thirdDigit <= '4';
		// Digits 3,4 shoudl be in range 00-49
		break;

	case Caster::C5:
		firstDigitOkay = firstDigit == '5';
		secondDigitOkay = secondDigit == '1';
		thirdDigitOkay = '5' <= thirdDigit && thirdDigit <= '9';
		// Digits 3,4 shoudl be in range 50-99
		break;

	default: 
		assert(false);
	}

	if (!firstDigitOkay) {

		MessageBox("First digit invalid for caster.",
			"Bad TAB #",
			MB_ICONHAND | MB_OK);
		return false;
	}


	if (!secondDigitOkay) {

		MessageBox("Second digit invalid for caster.",
			"Bad TAB #",
			MB_ICONHAND | MB_OK);
		return false;
	}


	if (!thirdDigitOkay) {

		MessageBox("Third digit invalid for caster.",
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

