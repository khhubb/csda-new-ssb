// StringBuilderInputSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "StringBuilderInputSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CStringBuilderInputSet

IMPLEMENT_DYNAMIC(CStringBuilderInputSet, CRecordset)

CStringBuilderInputSet::CStringBuilderInputSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CStringBuilderInputSet)
	m_MILL_ORDER_NO = 0;
	m_MORD_CARBON_MAX = 0.0f;
	m_MORD_CARBON_MIN = 0.0f;
	m_MORD_CMS_YIELD_LOSS_FACTOR = 0.0f;
	m_MORD_COLD_BAND_WIDTH = 0.0f;
	m_MORD_HOT_BAND_GAUGE = 0.0f;
	m_MORD_HOT_BAND_GAUGE_MAX = 0.0f;
	m_MORD_HOT_BAND_GAUGE_MIN = 0.0f;
	m_MORD_HOT_BAND_WIDTH = 0.0f;
	m_MORD_HOT_BAND_WIDTH_MAX = 0.0f;
	m_MORD_HOT_BAND_WIDTH_MIN = 0.0f;
	m_MORD_HOT_ROLL_WEEK = 0;
	m_MORD_HOT_ROLL_YEAR = 0;
	m_MORD_MANGANESE_MAX = 0.0f;
	m_MORD_MANGANESE_MIN = 0.0f;
	m_MORD_MAXIMUM_SLAB_LENGTH = 0.0f;
	m_MORD_MINIMUM_SLAB_LENGTH = 0.0f;
	m_MORD_ORDERED_GAUGE = 0.0f;
	m_MORD_ORDERED_LENGTH = 0.0f;
	m_MORD_ORDERED_SPEC = _T("");
	m_MORD_ORDERED_WEIGHT = 0;
	m_MORD_ORDERED_WIDTH = 0.0f;
	m_MORD_PLAN_PRTY_WK_DAY = 0;
	m_MORD_PRODUCT_CMDTY_CD = 0;
	m_MORD_PRODUCT_CODE = 0;
	m_MORD_SIGNIF_EL_MAX = 0.0f;
	m_MORD_SIGNIF_EL_MIN = 0.0f;
	m_MORD_SIGNIF_EL_SYM = _T("");
	m_MORD_SIXTH_DIGIT_OF_SPEC = _T("");
	m_MORD_SLAB_CONDITION_CD = 0;
	m_PROV_HB_PROV_TOL = 0;
	m_PROV_INDV_SLAB_WGT = 0;
	m_PROV_SLAB_LENGTH = 0;
	m_PROV_SLAB_THICKNESS = 0.0f;
	m_PROV_SLAB_WGT_DUE = 0;
	m_PROV_SLAB_WIDTH = 0.0f;
	m_PROV_SLABS_DUE = 0;
	m_REPROV_SLABS_DUE = 0;
	m_SM_PLAN_CAST_SPEC = _T("");
	m_SM_PLAN_FACL_PREF_1_CD = _T("");
	m_SM_PLAN_FACL_PREF_2_CD = _T("");
	m_SM_PLAN_FACL_PREF_3_CD = _T("");
	m_SM_PLAN_FACL_PREF_4_CD = _T("");
	m_SM_PLAN_FP_PST_DATE = 0;
	m_SM_PLAN_FP_PST_TIME = 0;
	m_SM_PLAN_SLAB_UNIT_CD = _T("");
	m_SM_PLAN_STEEL_SPEC = _T("");
	m_FP_M_ORDER_NO = _T("");
	m_nFields = 48;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CStringBuilderInputSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=dpaTest");
}

CString CStringBuilderInputSet::GetDefaultSQL()
{
	return _T("[StringBuilderInput]");
}

void CStringBuilderInputSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStringBuilderInputSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Single(pFX, _T("[MORD_CARBON_MAX]"), m_MORD_CARBON_MAX);
	RFX_Single(pFX, _T("[MORD_CARBON_MIN]"), m_MORD_CARBON_MIN);
	RFX_Single(pFX, _T("[MORD_CMS_YIELD_LOSS_FACTOR]"), m_MORD_CMS_YIELD_LOSS_FACTOR);
	RFX_Single(pFX, _T("[MORD_COLD_BAND_WIDTH]"), m_MORD_COLD_BAND_WIDTH);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE]"), m_MORD_HOT_BAND_GAUGE);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MAX]"), m_MORD_HOT_BAND_GAUGE_MAX);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MIN]"), m_MORD_HOT_BAND_GAUGE_MIN);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH]"), m_MORD_HOT_BAND_WIDTH);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MAX]"), m_MORD_HOT_BAND_WIDTH_MAX);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MIN]"), m_MORD_HOT_BAND_WIDTH_MIN);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_WEEK]"), m_MORD_HOT_ROLL_WEEK);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_YEAR]"), m_MORD_HOT_ROLL_YEAR);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MAX]"), m_MORD_MANGANESE_MAX);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MIN]"), m_MORD_MANGANESE_MIN);
	RFX_Single(pFX, _T("[MORD_MAXIMUM_SLAB_LENGTH]"), m_MORD_MAXIMUM_SLAB_LENGTH);
	RFX_Single(pFX, _T("[MORD_MINIMUM_SLAB_LENGTH]"), m_MORD_MINIMUM_SLAB_LENGTH);
	RFX_Single(pFX, _T("[MORD_ORDERED_GAUGE]"), m_MORD_ORDERED_GAUGE);
	RFX_Single(pFX, _T("[MORD_ORDERED_LENGTH]"), m_MORD_ORDERED_LENGTH);
	RFX_Text(pFX, _T("[MORD_ORDERED_SPEC]"), m_MORD_ORDERED_SPEC);
	RFX_Long(pFX, _T("[MORD_ORDERED_WEIGHT]"), m_MORD_ORDERED_WEIGHT);
	RFX_Single(pFX, _T("[MORD_ORDERED_WIDTH]"), m_MORD_ORDERED_WIDTH);
	RFX_Long(pFX, _T("[MORD_PLAN_PRTY_WK_DAY]"), m_MORD_PLAN_PRTY_WK_DAY);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CMDTY_CD]"), m_MORD_PRODUCT_CMDTY_CD);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CODE]"), m_MORD_PRODUCT_CODE);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MAX]"), m_MORD_SIGNIF_EL_MAX);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MIN]"), m_MORD_SIGNIF_EL_MIN);
	RFX_Text(pFX, _T("[MORD_SIGNIF_EL_SYM]"), m_MORD_SIGNIF_EL_SYM);
	RFX_Text(pFX, _T("[MORD_SIXTH_DIGIT_OF_SPEC]"), m_MORD_SIXTH_DIGIT_OF_SPEC);
	RFX_Long(pFX, _T("[MORD_SLAB_CONDITION_CD]"), m_MORD_SLAB_CONDITION_CD);
	RFX_Long(pFX, _T("[PROV_HB_PROV_TOL]"), m_PROV_HB_PROV_TOL);
	RFX_Long(pFX, _T("[PROV_INDV_SLAB_WGT]"), m_PROV_INDV_SLAB_WGT);
	RFX_Long(pFX, _T("[PROV_SLAB_LENGTH]"), m_PROV_SLAB_LENGTH);
	RFX_Single(pFX, _T("[PROV_SLAB_THICKNESS]"), m_PROV_SLAB_THICKNESS);
	RFX_Long(pFX, _T("[PROV_SLAB_WGT_DUE]"), m_PROV_SLAB_WGT_DUE);
	RFX_Single(pFX, _T("[PROV_SLAB_WIDTH]"), m_PROV_SLAB_WIDTH);
	RFX_Long(pFX, _T("[PROV_SLABS_DUE]"), m_PROV_SLABS_DUE);
	RFX_Long(pFX, _T("[REPROV_SLABS_DUE]"), m_REPROV_SLABS_DUE);
	RFX_Text(pFX, _T("[SM_PLAN_CAST_SPEC]"), m_SM_PLAN_CAST_SPEC);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_1_CD]"), m_SM_PLAN_FACL_PREF_1_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_2_CD]"), m_SM_PLAN_FACL_PREF_2_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_3_CD]"), m_SM_PLAN_FACL_PREF_3_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_4_CD]"), m_SM_PLAN_FACL_PREF_4_CD);
	RFX_Long(pFX, _T("[SM_PLAN_FP_PST_DATE]"), m_SM_PLAN_FP_PST_DATE);
	RFX_Long(pFX, _T("[SM_PLAN_FP_PST_TIME]"), m_SM_PLAN_FP_PST_TIME);
	RFX_Text(pFX, _T("[SM_PLAN_SLAB_UNIT_CD]"), m_SM_PLAN_SLAB_UNIT_CD);
	RFX_Text(pFX, _T("[SM_PLAN_STEEL_SPEC]"), m_SM_PLAN_STEEL_SPEC);
	RFX_Text(pFX, _T("[FP_M_ORDER_NO]"), m_FP_M_ORDER_NO);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderInputSet diagnostics

#ifdef _DEBUG
void CStringBuilderInputSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStringBuilderInputSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
