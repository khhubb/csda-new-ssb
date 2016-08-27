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
	m_caster45HeatSizeAim = 0;
	m_caster45HeatSizeMax = 0;
	m_caster45HeatSizeMin = 0;
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
	DDX_Text(pDX, IDC_EDIT_CASTER45_HEAT_SIZE_AIM, m_caster45HeatSizeAim);
	DDV_MinMaxInt(pDX, m_caster45HeatSizeAim, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER45_HEAT_SIZE_MAX, m_caster45HeatSizeMax);
	DDV_MinMaxInt(pDX, m_caster45HeatSizeMax, 0, 500);
	DDX_Text(pDX, IDC_EDIT_CASTER45_HEAT_SIZE_MIN, m_caster45HeatSizeMin);
	DDV_MinMaxInt(pDX, m_caster45HeatSizeMin, 0, 500);
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
	else if (m_caster45HeatSizeAim < m_caster45HeatSizeMin)
		msg += "For caster 4,5: Min should be <= Aim\n";
	else if (m_caster45HeatSizeMax < m_caster45HeatSizeAim)
		msg += "For caster 4,5: Min should be <= Aim\n";
	else if (m_caster45HeatSizeMax < m_caster45HeatSizeMin)
		msg += "For caster 4,5: Min should be <= Aim\n";


	if ( msg.GetLength() > 0  ) {
		MessageBox(msg,"Bad values",MB_ICONERROR);
		return FALSE;
	}
		
	m_pConsts->SetMinHeatSize(Caster::C1, m_caster1HeatSizeMin);
	m_pConsts->SetAimHeatSize(Caster::C1, m_caster1HeatSizeAim);
	m_pConsts->SetMaxHeatSize(Caster::C1, m_caster1HeatSizeMax);

	m_pConsts->SetMinHeatSize(Caster::C2, m_caster23HeatSizeMin);
	m_pConsts->SetAimHeatSize(Caster::C2, m_caster23HeatSizeAim);
	m_pConsts->SetMaxHeatSize(Caster::C2, m_caster23HeatSizeMax);

	m_pConsts->SetMinHeatSize(Caster::C3, m_caster23HeatSizeMin);
	m_pConsts->SetAimHeatSize(Caster::C3, m_caster23HeatSizeAim);
	m_pConsts->SetMaxHeatSize(Caster::C3, m_caster23HeatSizeMax);

	m_pConsts->SetMinHeatSize(Caster::C4, m_caster45HeatSizeMin);
	m_pConsts->SetAimHeatSize(Caster::C4, m_caster45HeatSizeAim);
	m_pConsts->SetMaxHeatSize(Caster::C4, m_caster45HeatSizeMax);

	m_pConsts->SetMinHeatSize(Caster::C5, m_caster45HeatSizeMin);
	m_pConsts->SetAimHeatSize(Caster::C5, m_caster45HeatSizeAim);
	m_pConsts->SetMaxHeatSize(Caster::C5, m_caster45HeatSizeMax);

	m_pConsts->Save();

	return TRUE;
}

 
BOOL CPpgHeatSizes::OnSetActive() 
{

	//  Because casters 2 & 3 are both at the same shop,
	//   heat sizes are the same.
	//  We could split out if we wanted, but won't for now.

	m_caster1HeatSizeMin  = m_pConsts->MinHeatSize(Caster::C1);
	m_caster1HeatSizeAim = m_pConsts->AimHeatSize(Caster::C1);
	m_caster1HeatSizeMax = m_pConsts->MaxHeatSize(Caster::C1);

	m_caster23HeatSizeMin = m_pConsts->MinHeatSize(Caster::C2);
	m_caster23HeatSizeAim = m_pConsts->AimHeatSize(Caster::C2);
	m_caster23HeatSizeMax = m_pConsts->MaxHeatSize(Caster::C2);
	
	m_caster45HeatSizeMin = m_pConsts->MinHeatSize(Caster::C4);
	m_caster45HeatSizeAim = m_pConsts->AimHeatSize(Caster::C4);
	m_caster45HeatSizeMax = m_pConsts->MaxHeatSize(Caster::C4);

	UpdateData(false);

	return TRUE;
}

void CPpgHeatSizes::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}
