// HeatLockDlg.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "HeatLockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeatLockDlg dialog
//
//  A dialog to change the heat status and numLockHeats of a string.
//
//
//  Usage:
//
//  
//		CHeatLockDlg dlg;
//		dlg.m_heatStatus = <initial value> ;
//		dlg.m_numLockedHeats = <initial value>;
//
//		dlg.m_bClientPos = <true, if you want to position the window> ;
//		dlg.m_x = <x coord of initial position> ;
//		dlg.m_y = <y coord of initial position> ;
//		
//		if ( dlg.DoModal() == IDOK ) {
//			// dlg.m_heatStatus and dlg.m_numLockedHeats have updated values
//		}
//
///////////////////////////////////////////////////////////////////////////


CHeatLockDlg::CHeatLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeatLockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeatLockDlg)
	m_numLockedHeats = 0;
	//}}AFX_DATA_INIT


	m_bClientPos = false;
	m_x = 0;
	m_y = 0;
}


void CHeatLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeatLockDlg)
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_editNum);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_numLockedHeats);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeatLockDlg, CDialog)
	//{{AFX_MSG_MAP(CHeatLockDlg)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_SOME, OnButtonSome)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeatLockDlg message handlers


void CHeatLockDlg::OnButtonAll() 
{
	m_heatStatus = CCastString::HEATS_ALL_LOCKED;
	SetState();
}

void CHeatLockDlg::OnButtonNone() 
{
	m_heatStatus = CCastString::HEATS_NONE_LOCKED;
	SetState();
}

void CHeatLockDlg::OnButtonSome() 
{
	m_heatStatus = CCastString::HEATS_SOME_LOCKED;
	SetState();
}

void CHeatLockDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}



BOOL CHeatLockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( m_numLockedHeats >= 0 );
	assert( m_numLockedHeats <= 99 );
	assert( m_heatStatus == CCastString::HEATS_ALL_LOCKED
		    ||
			m_heatStatus == CCastString::HEATS_NONE_LOCKED
			||
			m_heatStatus == CCastString::HEATS_SOME_LOCKED);

	m_spin.SetRange(0,99);

	m_buttonNone.SubclassDlgItem(IDC_BUTTON_NONE,this);
	m_buttonSome.SubclassDlgItem(IDC_BUTTON_SOME,this);
	m_buttonAll.SubclassDlgItem(IDC_BUTTON_ALL,this);

	vector<int> noneIds;
	vector<int> someIds;
	vector<int> allIds;

	noneIds.push_back(IDB_UNLOCKED_HEATS_OFF);
	noneIds.push_back(IDB_UNLOCKED_HEATS);
	someIds.push_back(IDB_SEMILOCKED_HEATS_OFF);
	someIds.push_back(IDB_SEMILOCKED_HEATS);
	allIds.push_back(IDB_LOCKED_HEATS_OFF);
	allIds.push_back(IDB_LOCKED_HEATS);

	m_buttonNone.SetBitmaps(noneIds);
	m_buttonSome.SetBitmaps(someIds);
	m_buttonAll.SetBitmaps(allIds);
	
	SetState();

	
	if ( m_bClientPos ) {

		// this is a very specialized function.
		// we want this dialog to appear at the lower left corner
		//   of its parent.

		SetWindowPos(&wndTop,
					 m_x,m_y,
					 0,0,
					 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CHeatLockDlg::SetState()
{
	switch ( m_heatStatus ) {
	case CCastString::HEATS_ALL_LOCKED:
		m_buttonNone.SetCurrentState(0);
		m_buttonSome.SetCurrentState(0);
		m_buttonAll.SetCurrentState(1);
		m_spin.EnableWindow(FALSE);
		m_editNum.EnableWindow(FALSE);
		break;
	case CCastString::HEATS_SOME_LOCKED:
		m_buttonNone.SetCurrentState(0);
		m_buttonSome.SetCurrentState(1);
		m_buttonAll.SetCurrentState(0);
		m_spin.EnableWindow(TRUE);
		m_editNum.EnableWindow(TRUE);
		break;
	case CCastString::HEATS_NONE_LOCKED:
		m_buttonNone.SetCurrentState(1);
		m_buttonSome.SetCurrentState(0);
		m_buttonAll.SetCurrentState(0);
		m_spin.EnableWindow(FALSE);
		m_editNum.EnableWindow(FALSE);
		break;
	}
}

