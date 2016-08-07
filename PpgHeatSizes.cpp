// PpgHeatSizes.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "PpgHeatSizes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MiscConstants.h"


/////////////////////////////////////////////////////////////////////////////
//  CPpgHeatSizes property page
//
//	A property page designed to be used inside CPSheetMiscConstants
//  For editing the heat sizes in a CMiscConstants object.
//
//  Usage: Need to call method SetMiscConstantsObject() to set the pointer
//    to the CMiscConstants object being edited.
//
//
//  Allows setting of min/aim/max heat size  for caster 1 and casters 2/3.
//
//  Makes sure that min <= aim <= max.
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CPpgHeatSizes, CPropertyPage)

CPpgHeatSizes::CPpgHeatSizes() : CPropertyPage(CPpgHeatSizes::IDD)
{
	//{{AFX_DATA_INIT(CPpgHeatSizes)
	m_caster1HeatSizeAim = 0;
	m_caster1HeatSizeMax = 0;
	m_caster1HeatSizeMin = 0;
	m_caster23HeatSizeAim = 0;
	m_caster23HeatSizeMax = 0;
	m_caster23HeatSizeMin = 0;
	m_caster4HeatSizeAim = 0;
	m_caster4HeatSizeMax = 0;
	m_caster4HeatSizeMin = 0;
	m_caster5HeatSizeAim = 0;
	m_caster5HeatSizeMax = 0;
	m_caster5HeatSizeMin = 0;
	//}}AFX_DATA_INIT
}

CPpgHeatSizes::~CPpgHeatSizes()
{
}

void CPpgHeatSizes::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPpgHeatSizes)
	DDX_Text(pDX, IDC_EDIT_CASTER1_HEAT_SIZE_AIM, m_caster1HeatSizeAim);
	DDV_MinMaxInt(pDX, m_caster1HeatSizeAim, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER1_HEAT_SIZE_MAX, m_caster1HeatSizeMax);
	DDV_MinMaxInt(pDX, m_caster1HeatSizeMax, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER1_HEAT_SIZE_MIN, m_caster1HeatSizeMin);
	DDV_MinMaxInt(pDX, m_caster1HeatSizeMin, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER23_HEAT_SIZE_AIM, m_caster23HeatSizeAim);
	DDV_MinMaxInt(pDX, m_caster23HeatSizeAim, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER23_HEAT_SIZE_MAX, m_caster23HeatSizeMax);
	DDV_MinMaxInt(pDX, m_caster23HeatSizeMax, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER23_HEAT_SIZE_MIN, m_caster23HeatSizeMin);
	DDV_MinMaxInt(pDX, m_caster23HeatSizeMin, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER4_HEAT_SIZE_AIM, m_caster4HeatSizeAim);
	DDV_MinMaxInt(pDX, m_caster4HeatSizeAim, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER4_HEAT_SIZE_MAX, m_caster4HeatSizeMax);
	DDV_MinMaxInt(pDX, m_caster4HeatSizeMax, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER4_HEAT_SIZE_MIN, m_caster4HeatSizeMin);
	DDV_MinMaxInt(pDX, m_caster4HeatSizeMin, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER5_HEAT_SIZE_AIM, m_caster5HeatSizeAim);
	DDV_MinMaxInt(pDX, m_caster5HeatSizeAim, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER5_HEAT_SIZE_MAX, m_caster5HeatSizeMax);
	DDV_MinMaxInt(pDX, m_caster5HeatSizeMax, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER5_HEAT_SIZE_MIN, m_caster5HeatSizeMin);
	DDV_MinMaxInt(pDX, m_caster5HeatSizeMin, 0, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPpgHeatSizes, CPropertyPage)
	//{{AFX_MSG_MAP(CPpgHeatSizes)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPpgHeatSizes message handlers

BOOL CPpgHeatSizes::OnKillActive() 
{
	// Move data back from controls to data members

	if ( ! CPropertyPage::OnKillActive() )
		return FALSE;

	CString msg;

	if ( m_caster1HeatSizeAim < m_caster1HeatSizeMin ) 
		msg += "For caster 1: Min should be <= Aim\n";
	else if ( m_caster1HeatSizeMax < m_caster1HeatSizeAim ) 
		msg += "For caster 1: Min should be <= Aim\n";
	else if ( m_caster1HeatSizeMax < m_caster1HeatSizeMin ) 
		msg += "For caster 1: Min should be <= Aim\n";
	else if ( m_caster23HeatSizeAim < m_caster23HeatSizeMin ) 
		msg += "For casters 2,3: Min should be <= Aim\n";
	else if ( m_caster23HeatSizeMax < m_caster23HeatSizeAim ) 
		msg += "For casters 2,3: Min should be <= Aim\n";
	else if ( m_caster23HeatSizeMax < m_caster23HeatSizeMin ) 
		msg += "For casters 2,3: Min should be <= Aim\n";
	else if (m_caster4HeatSizeAim < m_caster4HeatSizeMin)
		msg += "For caster 4: Min should be <= Aim\n";
	else if (m_caster4HeatSizeMax < m_caster4HeatSizeAim)
		msg += "For caster 4: Min should be <= Aim\n";
	else if (m_caster4HeatSizeMax < m_caster4HeatSizeMin)
		msg += "For caster 4: Min should be <= Aim\n";
	else if (m_caster5HeatSizeAim < m_caster5HeatSizeMin)
		msg += "For caster 5: Min should be <= Aim\n";
	else if (m_caster5HeatSizeMax < m_caster5HeatSizeAim)
		msg += "For caster 5: Min should be <= Aim\n";
	else if (m_caster5HeatSizeMax < m_caster5HeatSizeMin)
		msg += "For caster 5: Min should be <= Aim\n";

	if ( msg.GetLength() > 0  ) {
		MessageBox(msg,"Bad values",MB_ICONERROR);
		return FALSE;
	}
		
	m_pConsts->SetMinHeatSize(1,m_caster1HeatSizeMin);
	m_pConsts->SetAimHeatSize(1,m_caster1HeatSizeAim);
	m_pConsts->SetMaxHeatSize(1,m_caster1HeatSizeMax);

	m_pConsts->SetMinHeatSize(2,m_caster23HeatSizeMin);
	m_pConsts->SetAimHeatSize(2,m_caster23HeatSizeAim);
	m_pConsts->SetMaxHeatSize(2,m_caster23HeatSizeMax);

	m_pConsts->SetMinHeatSize(3,m_caster23HeatSizeMin);
	m_pConsts->SetAimHeatSize(3,m_caster23HeatSizeAim);
	m_pConsts->SetMaxHeatSize(3,m_caster23HeatSizeMax);

	m_pConsts->SetMinHeatSize(4, m_caster4HeatSizeMin);
	m_pConsts->SetAimHeatSize(4, m_caster4HeatSizeAim);
	m_pConsts->SetMaxHeatSize(4, m_caster4HeatSizeMax);

	m_pConsts->SetMinHeatSize(5, m_caster5HeatSizeMin);
	m_pConsts->SetAimHeatSize(5, m_caster5HeatSizeAim);
	m_pConsts->SetMaxHeatSize(5, m_caster5HeatSizeMax);

	m_pConsts->Save();

	return TRUE;
}

 
BOOL CPpgHeatSizes::OnSetActive() 
{

	//  Because casters 2 & 3 are both at the same shop,
	//   heat sizes are the same.
	//  We could split out if we wanted, but won't for now.

	m_caster1HeatSizeMin  = m_pConsts->MinHeatSize(1);
	m_caster1HeatSizeAim  = m_pConsts->AimHeatSize(1);
	m_caster1HeatSizeMax  = m_pConsts->MaxHeatSize(1);

	m_caster23HeatSizeMin  = m_pConsts->MinHeatSize(2);
	m_caster23HeatSizeAim  = m_pConsts->AimHeatSize(2);
	m_caster23HeatSizeMax  = m_pConsts->MaxHeatSize(2);
	
	m_caster4HeatSizeMin = m_pConsts->MinHeatSize(4);
	m_caster4HeatSizeAim = m_pConsts->AimHeatSize(4);
	m_caster4HeatSizeMax = m_pConsts->MaxHeatSize(4);

	m_caster5HeatSizeMin = m_pConsts->MinHeatSize(5);
	m_caster5HeatSizeAim = m_pConsts->AimHeatSize(5);
	m_caster5HeatSizeMax = m_pConsts->MaxHeatSize(5);

	UpdateData(false);

	return TRUE;
}

void CPpgHeatSizes::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}
