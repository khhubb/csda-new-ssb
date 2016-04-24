// SSBInput.h : Implementation of the CSSBInput class



// CSSBInput implementation

// code generated on Sunday, April 24, 2016, 7:32 AM

#include "stdafx.h"
#include "SSBInput.h"
IMPLEMENT_DYNAMIC(CSSBInput, CRecordset)

CSSBInput::CSSBInput(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_MILL_ORDER_NO = 0;
	m_MORD_HOT_ROLL_YEAR = 0;
	m_MORD_HOT_ROLL_WEEK = 0;
	m_MORD_PRODUCT_CODE = 0;
	m_MORD_SLAB_CONDITION_CD = 0;
	m_MORD_ORDERED_GAUGE = 0.0;
	m_MORD_ORDERED_WIDTH = 0.0;
	m_MORD_ORDERED_LENGTH = 0.0;
	m_MORD_ORDERED_WEIGHT = 0;
	m_MORD_ORDERED_SPEC = L"";
	m_MORD_SIXTH_DIGIT_OF_SPEC = L"";
	m_MORD_CARBON_MIN = 0.0;
	m_MORD_CARBON_MAX = 0.0;
	m_MORD_MANGANESE_MIN = 0.0;
	m_MORD_MANGANESE_MAX = 0.0;
	m_MORD_SIGNIF_EL_MIN = 0.0;
	m_MORD_SIGNIF_EL_MAX = 0.0;
	m_MORD_SIGNIF_EL_SYM = L"";
	m_MORD_PRODUCT_CMDTY_CD = 0;
	m_MORD_HOT_BAND_WIDTH = 0.0;
	m_MORD_HOT_BAND_WIDTH_MIN = 0.0;
	m_MORD_HOT_BAND_WIDTH_MAX = 0.0;
	m_MORD_HOT_BAND_GAUGE = 0.0;
	m_MORD_HOT_BAND_GAUGE_MIN = 0.0;
	m_MORD_HOT_BAND_GAUGE_MAX = 0.0;
	m_MORD_COLD_BAND_WIDTH = 0.0;
	m_MORD_MINIMUM_SLAB_LENGTH = 0.0;
	m_MORD_MAXIMUM_SLAB_LENGTH = 0.0;
	m_MORD_CMS_YIELD_LOSS_FACTOR = 0.0;
	m_PROV_SLABS_DUE = 0;
	m_MORD_PLAN_PRTY_WK_DAY = 0;
	m_REPROV_SLABS_DUE = 0;
	m_PROV_SLAB_WIDTH = 0.0;
	m_PROV_SLAB_THICKNESS = 0.0;
	m_PROV_SLAB_LENGTH = 0;
	m_PROV_SLAB_WGT_DUE = 0;
	m_PROV_INDV_SLAB_WGT = 0;
	m_PROV_HB_PROV_TOL = 0;
	m_SM_PLAN_STEEL_SPEC = L"";
	m_SM_PLAN_SLAB_UNIT_CD = L"";
	m_SM_PLAN_FACL_PREF_1_CD = L"";
	m_SM_PLAN_FACL_PREF_2_CD = L"";
	m_SM_PLAN_FACL_PREF_3_CD = L"";
	m_SM_PLAN_FACL_PREF_4_CD = L"";
	m_SM_PLAN_CAST_SPEC = L"";
	m_SM_PLAN_FP_PST_DATE = 0;
	m_SM_PLAN_FP_PST_TIME = 0;
	m_FP_M_ORDER_NO = L"";
	m_USER_ID = 0;
	m_nFields = 49;
	m_nDefaultType = snapshot;
}

CString CSSBInput::GetDefaultConnect()
{
	return _T("ODBC;DSN=dpaTest");
}

CString CSSBInput::GetDefaultSQL()
{
	return _T("[dbo].[SSBInput]");
}

void CSSBInput::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_YEAR]"), m_MORD_HOT_ROLL_YEAR);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_WEEK]"), m_MORD_HOT_ROLL_WEEK);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CODE]"), m_MORD_PRODUCT_CODE);
	RFX_Long(pFX, _T("[MORD_SLAB_CONDITION_CD]"), m_MORD_SLAB_CONDITION_CD);
	RFX_Single(pFX, _T("[MORD_ORDERED_GAUGE]"), m_MORD_ORDERED_GAUGE);
	RFX_Single(pFX, _T("[MORD_ORDERED_WIDTH]"), m_MORD_ORDERED_WIDTH);
	RFX_Single(pFX, _T("[MORD_ORDERED_LENGTH]"), m_MORD_ORDERED_LENGTH);
	RFX_Long(pFX, _T("[MORD_ORDERED_WEIGHT]"), m_MORD_ORDERED_WEIGHT);
	RFX_Text(pFX, _T("[MORD_ORDERED_SPEC]"), m_MORD_ORDERED_SPEC);
	RFX_Text(pFX, _T("[MORD_SIXTH_DIGIT_OF_SPEC]"), m_MORD_SIXTH_DIGIT_OF_SPEC);
	RFX_Single(pFX, _T("[MORD_CARBON_MIN]"), m_MORD_CARBON_MIN);
	RFX_Single(pFX, _T("[MORD_CARBON_MAX]"), m_MORD_CARBON_MAX);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MIN]"), m_MORD_MANGANESE_MIN);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MAX]"), m_MORD_MANGANESE_MAX);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MIN]"), m_MORD_SIGNIF_EL_MIN);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MAX]"), m_MORD_SIGNIF_EL_MAX);
	RFX_Text(pFX, _T("[MORD_SIGNIF_EL_SYM]"), m_MORD_SIGNIF_EL_SYM);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CMDTY_CD]"), m_MORD_PRODUCT_CMDTY_CD);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH]"), m_MORD_HOT_BAND_WIDTH);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MIN]"), m_MORD_HOT_BAND_WIDTH_MIN);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MAX]"), m_MORD_HOT_BAND_WIDTH_MAX);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE]"), m_MORD_HOT_BAND_GAUGE);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MIN]"), m_MORD_HOT_BAND_GAUGE_MIN);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MAX]"), m_MORD_HOT_BAND_GAUGE_MAX);
	RFX_Single(pFX, _T("[MORD_COLD_BAND_WIDTH]"), m_MORD_COLD_BAND_WIDTH);
	RFX_Single(pFX, _T("[MORD_MINIMUM_SLAB_LENGTH]"), m_MORD_MINIMUM_SLAB_LENGTH);
	RFX_Single(pFX, _T("[MORD_MAXIMUM_SLAB_LENGTH]"), m_MORD_MAXIMUM_SLAB_LENGTH);
	RFX_Single(pFX, _T("[MORD_CMS_YIELD_LOSS_FACTOR]"), m_MORD_CMS_YIELD_LOSS_FACTOR);
	RFX_Long(pFX, _T("[PROV_SLABS_DUE]"), m_PROV_SLABS_DUE);
	RFX_Long(pFX, _T("[MORD_PLAN_PRTY_WK_DAY]"), m_MORD_PLAN_PRTY_WK_DAY);
	RFX_Long(pFX, _T("[REPROV_SLABS_DUE]"), m_REPROV_SLABS_DUE);
	RFX_Single(pFX, _T("[PROV_SLAB_WIDTH]"), m_PROV_SLAB_WIDTH);
	RFX_Single(pFX, _T("[PROV_SLAB_THICKNESS]"), m_PROV_SLAB_THICKNESS);
	RFX_Long(pFX, _T("[PROV_SLAB_LENGTH]"), m_PROV_SLAB_LENGTH);
	RFX_Long(pFX, _T("[PROV_SLAB_WGT_DUE]"), m_PROV_SLAB_WGT_DUE);
	RFX_Long(pFX, _T("[PROV_INDV_SLAB_WGT]"), m_PROV_INDV_SLAB_WGT);
	RFX_Long(pFX, _T("[PROV_HB_PROV_TOL]"), m_PROV_HB_PROV_TOL);
	RFX_Text(pFX, _T("[SM_PLAN_STEEL_SPEC]"), m_SM_PLAN_STEEL_SPEC);
	RFX_Text(pFX, _T("[SM_PLAN_SLAB_UNIT_CD]"), m_SM_PLAN_SLAB_UNIT_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_1_CD]"), m_SM_PLAN_FACL_PREF_1_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_2_CD]"), m_SM_PLAN_FACL_PREF_2_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_3_CD]"), m_SM_PLAN_FACL_PREF_3_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_4_CD]"), m_SM_PLAN_FACL_PREF_4_CD);
	RFX_Text(pFX, _T("[SM_PLAN_CAST_SPEC]"), m_SM_PLAN_CAST_SPEC);
	RFX_Long(pFX, _T("[SM_PLAN_FP_PST_DATE]"), m_SM_PLAN_FP_PST_DATE);
	RFX_Long(pFX, _T("[SM_PLAN_FP_PST_TIME]"), m_SM_PLAN_FP_PST_TIME);
	RFX_Text(pFX, _T("[FP_M_ORDER_NO]"), m_FP_M_ORDER_NO);
	RFX_Long(pFX, _T("[USER_ID]"), m_USER_ID);

}
/////////////////////////////////////////////////////////////////////////////
// CSSBInput diagnostics

#ifdef _DEBUG
void CSSBInput::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSSBInput::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


