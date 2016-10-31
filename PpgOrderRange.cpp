// PpgOrderRange.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "PpgOrderRange.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "OrderSelection.h"
#include "snapshot.h"
#include "PSheetAvailOrders.h"


/////////////////////////////////////////////////////////////////////////////
//	CPpgOrderRange property page
//
//  Property page to display order range selection criteria for a COrderSelection.
//
//  Used as part of CPSheetAvailOrders
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CPpgOrderRange, CPropertyPage)

CPpgOrderRange::CPpgOrderRange() : CPropertyPage(CPpgOrderRange::IDD)
{
	//{{AFX_DATA_INIT(CPpgOrderRange)
	m_bUseWidthRange = FALSE;
	m_maxWidth = 0.0;
	m_minWidth = 0.0;
	m_bUse80HSMLuType = FALSE;
	m_bUsePriorityCode = FALSE;
	m_80HSMLuType = _T("");
	m_orderPriorityMax = 0;
	m_bUseAllSpecs = FALSE;
	m_spec1 = _T("");
	m_spec2 = _T("");
	m_spec3 = _T("");
	m_spec4 = _T("");
	m_spec5 = _T("");
	m_bUseCrossApp = FALSE;
	m_maxCommPrio = 0;
	m_minCommPrio = 0;
	m_bIncludeNewOrders = FALSE;
	m_eIncludeOrders = -1;
	m_bPst15to21 = FALSE;
	m_bPst22to28 = FALSE;
	m_bPst29to35 = FALSE;
	m_bPst8to14 = FALSE;
	m_bPstGt35 = FALSE;
	m_bPstLt8 = FALSE;
	m_bHrwkCurrentBack = FALSE;
	m_bHrwkFuture = FALSE;
	m_bHrwkPlus1 = FALSE;
	m_bHrwkPlus2 = FALSE;
	m_bHrwkPlus3 = FALSE;
	m_bHrwkPlus4 = FALSE;
	m_currentWeek = 0;
	m_currentYear = 0;
	m_strPst15to21Tons = _T("");
	m_strPst22to28Tons = _T("");
	m_strPst29to35Tons = _T("");
	m_strPst8to14Tons = _T("");
	m_strPstGt35Tons = _T("");
	m_strPstLt8Tons = _T("");
	m_bPstPrio = FALSE;
	m_strPstPrioTons = _T("");
	//}}AFX_DATA_INIT

	m_pOrderSelection = 0;
	m_pSheet = 0;
	m_hasBeenActivated = false;
}

CPpgOrderRange::~CPpgOrderRange()
{
}



void CPpgOrderRange::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPpgOrderRange)
	DDX_Check(pDX, IDC_CHECK_USE_WIDTH_RANGE, m_bUseWidthRange);
	DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_maxWidth);
	DDV_MinMaxDouble(pDX, m_maxWidth, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_minWidth);
	DDV_MinMaxDouble(pDX, m_minWidth, 0., 99.9);
	DDX_Check(pDX, IDC_CHECK_USE_80HSM_LU_TYPE, m_bUse80HSMLuType);
	DDX_Check(pDX, IDC_CHECK_USE_PRIORITY_CODE, m_bUsePriorityCode);
	DDX_Text(pDX, IDC_EDIT_80HSM_LU_TYPE, m_80HSMLuType);
	DDX_Text(pDX, IDC_EDIT_ORDER_PRIORITY, m_orderPriorityMax);
	DDV_MinMaxInt(pDX, m_orderPriorityMax, 0, 999);
	DDX_Check(pDX, IDC_CHECK_USE_ALL_SPECS, m_bUseAllSpecs);
	DDX_CBString(pDX, IDC_COMBO_SPEC1, m_spec1);
	DDV_MaxChars(pDX, m_spec1, 7);
	DDX_CBString(pDX, IDC_COMBO_SPEC2, m_spec2);
	DDV_MaxChars(pDX, m_spec2, 7);
	DDX_CBString(pDX, IDC_COMBO_SPEC3, m_spec3);
	DDV_MaxChars(pDX, m_spec3, 7);
	DDX_CBString(pDX, IDC_COMBO_SPEC4, m_spec4);
	DDV_MaxChars(pDX, m_spec4, 7);
	DDX_CBString(pDX, IDC_COMBO_SPEC5, m_spec5);
	DDV_MaxChars(pDX, m_spec5, 7);
	DDX_Check(pDX, IDC_CHECK_CROSS_APP, m_bUseCrossApp);
	DDX_Text(pDX, IDC_EDIT_MAX_CI_PRIORITY, m_maxCommPrio);
	DDV_MinMaxInt(pDX, m_maxCommPrio, 0, 99);
	DDX_Text(pDX, IDC_EDIT_MIN_CI_PRIORITY, m_minCommPrio);
	DDV_MinMaxInt(pDX, m_minCommPrio, 0, 99);
	DDX_Check(pDX, IDC_CHECK_NEW_ORDERS, m_bIncludeNewOrders);
	DDX_Radio(pDX, IDC_RADIO_SHOW_THIS_CASTER, m_eIncludeOrders);
	DDX_Check(pDX, IDC_CHECK_PST_15_21, m_bPst15to21);
	DDX_Check(pDX, IDC_CHECK_PST_22_28, m_bPst22to28);
	DDX_Check(pDX, IDC_CHECK_PST_29_35, m_bPst29to35);
	DDX_Check(pDX, IDC_CHECK_PST_8_14, m_bPst8to14);
	DDX_Check(pDX, IDC_CHECK_PST_GT_35, m_bPstGt35);
	DDX_Check(pDX, IDC_CHECK_PST_LT_8, m_bPstLt8);
	DDX_Check(pDX, IDC_CHECK_HRWK_CURRENT_BACK, m_bHrwkCurrentBack);
	DDX_Check(pDX, IDC_CHECK_HRWK_FUTURE, m_bHrwkFuture);
	DDX_Check(pDX, IDC_CHECK_HRWK_PLUS_1, m_bHrwkPlus1);
	DDX_Check(pDX, IDC_CHECK_HRWK_PLUS_2, m_bHrwkPlus2);
	DDX_Check(pDX, IDC_CHECK_HRWK_PLUS_3, m_bHrwkPlus3);
	DDX_Check(pDX, IDC_CHECK_HRWK_PLUS_4, m_bHrwkPlus4);
	DDX_Text(pDX, IDC_EDIT_CURRENT_WEEK, m_currentWeek);
	DDV_MinMaxInt(pDX, m_currentWeek, 0, 54);
	DDX_Text(pDX, IDC_EDIT_CURRENT_YEAR, m_currentYear);
	DDV_MinMaxInt(pDX, m_currentYear, 1900, 2100);
	DDX_Text(pDX, IDC_STATIC_PST_15_21_TONS, m_strPst15to21Tons);
	DDX_Text(pDX, IDC_STATIC_PST_22_28_TONS, m_strPst22to28Tons);
	DDX_Text(pDX, IDC_STATIC_PST_29_35_TONS, m_strPst29to35Tons);
	DDX_Text(pDX, IDC_STATIC_PST_8_14_TONS, m_strPst8to14Tons);
	DDX_Text(pDX, IDC_STATIC_PST_GT_35_TONS, m_strPstGt35Tons);
	DDX_Text(pDX, IDC_STATIC_PST_LT_8_TONS, m_strPstLt8Tons);
	DDX_Check(pDX, IDC_CHECK_PRIORITY, m_bPstPrio);
	DDX_Text(pDX, IDC_STATIC_PRIO_TONS, m_strPstPrioTons);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPpgOrderRange, CPropertyPage)
	//{{AFX_MSG_MAP(CPpgOrderRange)
	ON_BN_CLICKED(IDC_CHECK_USE_WIDTH_RANGE, OnCheckUseWidthRange)
	ON_BN_CLICKED(IDC_CHECK_USE_80HSM_LU_TYPE, OnCheckUse80hsmLuType)
	ON_BN_CLICKED(IDC_CHECK_USE_PRIORITY_CODE, OnCheckUsePriorityCode)
	ON_BN_CLICKED(IDC_CHECK_USE_ALL_SPECS, OnCheckUseAllSpecs)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC1, OnEditchangeComboSpecAll)
	ON_EN_CHANGE(IDC_EDIT_80HSM_LU_TYPE, OnChangeEditMisc)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC1, OnSelchangeComboSpecAll)
	ON_BN_CLICKED(IDC_CHECK_CROSS_APP, OnCheckCrossApp)
	ON_BN_CLICKED(IDC_CHECK_NEW_ORDERS, OnCheckNewOrders)
	ON_BN_CLICKED(IDC_RADIO_SHOW_ALL, OnRadioShowAll)
	ON_BN_CLICKED(IDC_RADIO_SHOW_SWING_TONS, OnRadioShowSwingTons)
	ON_BN_CLICKED(IDC_RADIO_SHOW_THIS_CASTER, OnRadioShowThisCaster)
	ON_BN_CLICKED(IDC_CHECK_PST_15_21, OnCheckPstRange)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC2, OnEditchangeComboSpecAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC3, OnEditchangeComboSpecAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC4, OnEditchangeComboSpecAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC5, OnEditchangeComboSpecAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_WIDTH, OnChangeEditMisc)
	ON_EN_CHANGE(IDC_EDIT_MIN_WIDTH, OnChangeEditMisc)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC2, OnSelchangeComboSpecAll)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC3, OnSelchangeComboSpecAll)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC4, OnSelchangeComboSpecAll)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC5, OnSelchangeComboSpecAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CI_PRIORITY, OnChangeEditMisc)
	ON_EN_CHANGE(IDC_EDIT_MIN_CI_PRIORITY, OnChangeEditMisc)
	ON_BN_CLICKED(IDC_CHECK_PST_22_28, OnCheckPstRange)
	ON_BN_CLICKED(IDC_CHECK_PST_29_35, OnCheckPstRange)
	ON_BN_CLICKED(IDC_CHECK_PST_8_14, OnCheckPstRange)
	ON_BN_CLICKED(IDC_CHECK_PST_GT_35, OnCheckPstRange)
	ON_BN_CLICKED(IDC_CHECK_PST_LT_8, OnCheckPstRange)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_WEEK, OnChangeEditMisc)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_YEAR, OnChangeEditMisc)
	ON_BN_CLICKED(IDC_CHECK_HRWK_FUTURE, OnCheckHrwkButton)
	ON_BN_CLICKED(IDC_CHECK_HRWK_PLUS_1, OnCheckHrwkButton)
	ON_BN_CLICKED(IDC_CHECK_HRWK_PLUS_2, OnCheckHrwkButton)
	ON_BN_CLICKED(IDC_CHECK_HRWK_PLUS_3, OnCheckHrwkButton)
	ON_BN_CLICKED(IDC_CHECK_HRWK_PLUS_4, OnCheckHrwkButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CPpgOrderRange message handlers



//
//  This needs to be called during initialization of the property sheet.
//

void CPpgOrderRange::SetOrderSelection(COrderSelection* pSelect)
{
	m_pOrderSelection = pSelect;
	m_hasBeenActivated = false;
}



//
//  We allow specification of up to 5 spec names
//  This populates each of the 5 combo boxes with the options, 
//    namely, the available steel specs
//
//  If caster is 4 or 5 and m_eIncludeOrders == INCLUDE_THIS_CASTER_ONLY, then we display available west specs

void CPpgOrderRange::SetSpecListEntries()
{
	CComboBox* lists[5];
	lists[0] = (CComboBox*) GetDlgItem(IDC_COMBO_SPEC1);
	lists[1] = (CComboBox*) GetDlgItem(IDC_COMBO_SPEC2);
	lists[2] = (CComboBox*) GetDlgItem(IDC_COMBO_SPEC3);
	lists[3] = (CComboBox*) GetDlgItem(IDC_COMBO_SPEC4);
	lists[4] = (CComboBox*) GetDlgItem(IDC_COMBO_SPEC5);

	for ( int i =0; i<5; ++i ) 
		lists[i]->ResetContent();

	const set<CString>& specs = TheSnapshot.AvailSteelSpecs();

	for ( set<CString>::const_iterator is = specs.begin();
	      is != specs.end();
		  ++is ) 

		for ( int i = 0; i<5; ++i ) 
		
			lists[i]->AddString((*is));

}

		
//
//  There are several flags which determine if certain range parameters
//     are being used.
//  These flags control whether the corresponding display elements are enabled.
//

void CPpgOrderRange::SetDisplayState()
{
	GetDlgItem(IDC_EDIT_MIN_WIDTH)->EnableWindow(m_bUseWidthRange);
	GetDlgItem(IDC_EDIT_MAX_WIDTH)->EnableWindow(m_bUseWidthRange);
	GetDlgItem(IDC_EDIT_80HSM_LU_TYPE)->EnableWindow(m_bUse80HSMLuType);
	GetDlgItem(IDC_EDIT_CI_PRIORITY)->EnableWindow(m_bUsePriorityCode);
	GetDlgItem(IDC_EDIT_ORDER_PRIORITY)->EnableWindow(m_bUsePriorityCode);

	GetDlgItem(IDC_COMBO_SPEC1)->EnableWindow(! m_bUseAllSpecs);
	GetDlgItem(IDC_COMBO_SPEC2)->EnableWindow(! m_bUseAllSpecs && ! m_bUseCrossApp );
	GetDlgItem(IDC_COMBO_SPEC3)->EnableWindow(! m_bUseAllSpecs && ! m_bUseCrossApp );
	GetDlgItem(IDC_COMBO_SPEC4)->EnableWindow(! m_bUseAllSpecs && ! m_bUseCrossApp );
	GetDlgItem(IDC_COMBO_SPEC5)->EnableWindow(! m_bUseAllSpecs && ! m_bUseCrossApp );

}



BOOL CPpgOrderRange::OnInitDialog() 
{
	assert( m_pSheet != 0 );

	CPropertyPage::OnInitDialog();
	
	SetSpecListEntries();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


////////////////////////////////////////////////////////////////
//	
//	Callbacks
//	
////////////////////////////////////////////////////////////////

//  we have decided to update the pst range tons after every operation.
//  so every operation will update the OrderSelection and cause a recompute.


void CPpgOrderRange::OnCheckUseWidthRange() 
{
	// update local var state, then update display state
	UpdateData(true);
	SetDisplayState();
	SetModified(true);
	UpdateOrderSelection();
}

void CPpgOrderRange::OnCheckUse80hsmLuType() 
{
	UpdateData(true);
	SetDisplayState();
	SetModified(true);
	UpdateOrderSelection();
}

void CPpgOrderRange::OnCheckUsePriorityCode() 
{
	UpdateData(true);
	SetDisplayState();
	SetModified(true);
	UpdateOrderSelection();
}


void CPpgOrderRange::OnCheckUseAllSpecs() 
{
	UpdateData(true);
	SetDisplayState();
	SetModified(true);
	UpdateOrderSelection();
}


void CPpgOrderRange::OnCheckCrossApp() 
{
	UpdateData(true);
	SetDisplayState();
	SetModified(true);
	UpdateOrderSelection();
}



void CPpgOrderRange::OnCheckHrwkButton() 
{
	SetModified(true);
	UpdateOrderSelection();
}


void CPpgOrderRange::OnCheckPstRange() 
{
	SetModified(true);
	UpdateOrderSelection();
}


void CPpgOrderRange::OnEditchangeComboSpecAll() 
{
	SetModified(true);	
	UpdateOrderSelection();
}

void CPpgOrderRange::OnChangeEditMisc() 
{
	SetModified(true);	
	UpdateOrderSelection();
}

void CPpgOrderRange::OnSelchangeComboSpecAll() 
{
	SetModified(true);	
	UpdateOrderSelection();
}


void CPpgOrderRange::OnCheckNewOrders() 
{
	SetModified(true);	
	UpdateOrderSelection();
}



void CPpgOrderRange::OnRadioShowAll() 
{
	SetModified(true);	
	UpdateOrderSelection();
}

void CPpgOrderRange::OnRadioShowSwingTons() 
{
	SetModified(true);	
	UpdateOrderSelection();
}

void CPpgOrderRange::OnRadioShowThisCaster() 
{
	SetModified(true);	
	UpdateOrderSelection();
}



void CPpgOrderRange::UpdateOrderSelection()
{
	m_pSheet->OnApplyNow();
	OrderSelectionToLocal();
	UpdateData(false);
}


////////////////////////////////////////////////////////////////
//	
//	Moving data
//	
////////////////////////////////////////////////////////////////


//
// Move data from controls to local data members to OrderSelection
// Return true if no invalid data
//

bool CPpgOrderRange::LocalToOrderSelection()
{
	if ( ! UpdateData(true) )
		return false;

	vector<CString> specs;
	specs.push_back(m_spec1);
	specs.push_back(m_spec2);
	specs.push_back(m_spec3);
	specs.push_back(m_spec4);
	specs.push_back(m_spec5);

	m_pOrderSelection->SetSelectedSpecs(specs);
	m_pOrderSelection->SetUseAllSpecs(m_bUseAllSpecs);
	m_pOrderSelection->SetUseCrossApp(m_bUseCrossApp);
	
	m_pOrderSelection->SetUseWidthRange(m_bUseWidthRange);
	m_pOrderSelection->SetMaxWidth(m_maxWidth);
	m_pOrderSelection->SetMinWidth(m_minWidth);

	m_pOrderSelection->SetIncludeOrders(COrderSelection::E_IncludeOrderType(m_eIncludeOrders));

	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_CURRENT_BACK,	m_bHrwkCurrentBack	);
	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_PLUS_1,		m_bHrwkPlus1		);
	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_PLUS_2,		m_bHrwkPlus2		);
	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_PLUS_3,		m_bHrwkPlus3		);
	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_PLUS_4,		m_bHrwkPlus4		);
	m_pOrderSelection->SetUseHrwk( COrderSelection::HR_WK_FUTURE,		m_bHrwkFuture		);
	CWeek wkYr(m_currentYear, m_currentWeek);
	m_pOrderSelection->SetCurrentHrYrWk(wkYr);

	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_PRIO,  m_bPstPrio   );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_LT_8,  m_bPstLt8    );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_8_14,  m_bPst8to14  );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_15_21, m_bPst15to21 );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_22_28, m_bPst22to28 );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_29_35, m_bPst29to35 );
	m_pOrderSelection->SetUsePstRange( COrderSelection::PST_GT_35, m_bPstGt35   );

	m_pOrderSelection->SetUse80HsmLuType(m_bUse80HSMLuType);
	m_pOrderSelection->SetMegaLuType(m_80HSMLuType);

	m_pOrderSelection->SetUsePriorityCode(m_bUsePriorityCode);
	m_pOrderSelection->SetMaxCIPriority(m_maxCommPrio);
	m_pOrderSelection->SetMinCIPriority(m_minCommPrio);
//	m_pOrderSelection->SetMaxCIPriority(m_CIPriorityMax);
//	m_pOrderSelection->SetMaxOrderPriority(m_orderPriorityMax);

	m_pOrderSelection->SetIncludeNewOrders(m_bIncludeNewOrders);

	SetModified(m_pOrderSelection->IsModified());

	return true;
}



//
// Move data from OrderSelection to local data members
//

void CPpgOrderRange::OrderSelectionToLocal()
{

	m_spec1				= m_pOrderSelection->SelectedSpec(0);
	m_spec2             = m_pOrderSelection->SelectedSpec(1);
	m_spec3             = m_pOrderSelection->SelectedSpec(2);
	m_spec4             = m_pOrderSelection->SelectedSpec(3);
	m_spec5             = m_pOrderSelection->SelectedSpec(4);
	m_bUseAllSpecs		= m_pOrderSelection->UseAllSpecs();
	m_bUseCrossApp		= m_pOrderSelection->UseCrossApp();

	m_bUseWidthRange	= m_pOrderSelection->UseWidthRange();
	m_maxWidth			= m_pOrderSelection->MaxWidth();
	m_minWidth			= m_pOrderSelection->MinWidth();


	m_eIncludeOrders	 = m_pOrderSelection->IncludeOrders();

	m_bHrwkCurrentBack	= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_CURRENT_BACK);	
	m_bHrwkFuture		= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_FUTURE);		
	m_bHrwkPlus1		= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_PLUS_1);		
	m_bHrwkPlus2		= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_PLUS_2);		
	m_bHrwkPlus3		= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_PLUS_3);		
	m_bHrwkPlus4		= m_pOrderSelection->UseHrwk(COrderSelection::HR_WK_PLUS_4);	
	m_currentWeek      = m_pOrderSelection->CurrentHrYrWk().Week();
	m_currentYear      = m_pOrderSelection->CurrentHrYrWk().Year();

	m_bPstPrio		= m_pOrderSelection->UsePstRange(COrderSelection::PST_PRIO);
	m_bPstLt8		= m_pOrderSelection->UsePstRange(COrderSelection::PST_LT_8);
	m_bPst8to14		= m_pOrderSelection->UsePstRange(COrderSelection::PST_8_14);
	m_bPst15to21	= m_pOrderSelection->UsePstRange(COrderSelection::PST_15_21);
	m_bPst22to28	= m_pOrderSelection->UsePstRange(COrderSelection::PST_22_28);
	m_bPst29to35	= m_pOrderSelection->UsePstRange(COrderSelection::PST_29_35);
	m_bPstGt35		= m_pOrderSelection->UsePstRange(COrderSelection::PST_GT_35);

	char buf[20];
	
	m_strPstPrioTons   = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_PRIO),  buf, 10 );
	m_strPstLt8Tons    = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_LT_8),  buf, 10 );
	m_strPst8to14Tons  = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_8_14),  buf, 10 );
	m_strPst15to21Tons = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_15_21), buf, 10 );
	m_strPst22to28Tons = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_22_28), buf, 10 );
	m_strPst29to35Tons = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_29_35), buf, 10 );
	m_strPstGt35Tons   = _ltoa( m_pOrderSelection->PstRangeTons(COrderSelection::PST_GT_35), buf, 10 );

	m_bUse80HSMLuType	= m_pOrderSelection->Use80HsmLuType();		
	m_80HSMLuType		= m_pOrderSelection->MegaLuType();			

	m_bUsePriorityCode	= m_pOrderSelection->UsePriorityCode();	
	m_maxCommPrio		= m_pOrderSelection->MaxCIPriority();		
	m_minCommPrio		= m_pOrderSelection->MinCIPriority();	

	m_bIncludeNewOrders	= m_pOrderSelection->IncludeNewOrders();

}




BOOL CPpgOrderRange::OnSetActive() 
{
	//  MS documentation says to call CPropertyPage::OnSetActive() first,
	//  but it does not call UpdateData the first time.

	if ( ! m_hasBeenActivated ) {
		OrderSelectionToLocal();
		UpdateData(false);
	}

	SetDisplayState();

	m_hasBeenActivated = true;

	return TRUE;
}

