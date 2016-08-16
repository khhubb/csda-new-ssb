// PickCasterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "PickCasterDlg.h"
#include "Caster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPickCasterDlg dialog
//
// The user can pick a caster, other than a designated one.
//
//
//	Usage:
//
//		CPickCasterDlg dlg;
//		dlg.m_ignoreCaster = <Caster# which is not the allowed choice, can be 0 if all ok> ;
//		dlg.m_caster =  <Caster# which is the initial choice> ;
//		dlg.m_label = <CString to display as label>;
//	
//		if ( dlg.DoModal() == IDOK ) {
//			// dlg.m_caster has the user's choice
//			// dlg.m_caster != dlg.m_ignoreCaster if dlg.m_ignoreCaster != 0
//		}
//
//
//////////////////////////////////////////////////////////////////////////////


CPickCasterDlg::CPickCasterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickCasterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPickCasterDlg)
	m_caster = -1;
	m_label = _T("");
	//}}AFX_DATA_INIT

	m_ignoreCaster = 0;
}


void CPickCasterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickCasterDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_caster);
	DDX_Text(pDX, IDC_STATIC_LABEL, m_label);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickCasterDlg, CDialog)
	//{{AFX_MSG_MAP(CPickCasterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickCasterDlg message handlers

BOOL CPickCasterDlg::OnInitDialog() 
{
	// caster is 1,2,3, but we need index of 0,1,2
	--m_caster;

	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_RADIO1)->EnableWindow(m_ignoreCaster != Caster::C1);
	GetDlgItem(IDC_RADIO2)->EnableWindow(m_ignoreCaster != Caster::C2);
	GetDlgItem(IDC_RADIO3)->EnableWindow(m_ignoreCaster != Caster::C3);
	GetDlgItem(IDC_RADIO3)->EnableWindow(m_ignoreCaster != Caster::C4);
	GetDlgItem(IDC_RADIO3)->EnableWindow(m_ignoreCaster != Caster::C5);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPickCasterDlg::OnOK() 
{
	CDialog::OnOK();
	// index is 0,1,2,3,4 but we need caster = 1,2,3,4,5
	++m_caster;
}
