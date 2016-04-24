// SSBInput.h : Declaration of the CSSBInput

#pragma once

// code generated on Sunday, April 24, 2016, 7:32 AM

class CSSBInput : public CRecordset
{
public:
	CSSBInput(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSSBInput)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_MILL_ORDER_NO;
	long	m_MORD_HOT_ROLL_YEAR;
	long	m_MORD_HOT_ROLL_WEEK;
	long	m_MORD_PRODUCT_CODE;
	long	m_MORD_SLAB_CONDITION_CD;
	float	m_MORD_ORDERED_GAUGE;
	float	m_MORD_ORDERED_WIDTH;
	float	m_MORD_ORDERED_LENGTH;
	long	m_MORD_ORDERED_WEIGHT;
	CStringW	m_MORD_ORDERED_SPEC;
	CStringW	m_MORD_SIXTH_DIGIT_OF_SPEC;
	float	m_MORD_CARBON_MIN;
	float	m_MORD_CARBON_MAX;
	float	m_MORD_MANGANESE_MIN;
	float	m_MORD_MANGANESE_MAX;
	float	m_MORD_SIGNIF_EL_MIN;
	float	m_MORD_SIGNIF_EL_MAX;
	CStringW	m_MORD_SIGNIF_EL_SYM;
	long	m_MORD_PRODUCT_CMDTY_CD;
	float	m_MORD_HOT_BAND_WIDTH;
	float	m_MORD_HOT_BAND_WIDTH_MIN;
	float	m_MORD_HOT_BAND_WIDTH_MAX;
	float	m_MORD_HOT_BAND_GAUGE;
	float	m_MORD_HOT_BAND_GAUGE_MIN;
	float	m_MORD_HOT_BAND_GAUGE_MAX;
	float	m_MORD_COLD_BAND_WIDTH;
	float	m_MORD_MINIMUM_SLAB_LENGTH;
	float	m_MORD_MAXIMUM_SLAB_LENGTH;
	float	m_MORD_CMS_YIELD_LOSS_FACTOR;
	long	m_PROV_SLABS_DUE;
	long	m_MORD_PLAN_PRTY_WK_DAY;
	long	m_REPROV_SLABS_DUE;
	float	m_PROV_SLAB_WIDTH;
	float	m_PROV_SLAB_THICKNESS;
	long	m_PROV_SLAB_LENGTH;
	long	m_PROV_SLAB_WGT_DUE;
	long	m_PROV_INDV_SLAB_WGT;
	long	m_PROV_HB_PROV_TOL;
	CStringW	m_SM_PLAN_STEEL_SPEC;
	CStringW	m_SM_PLAN_SLAB_UNIT_CD;
	CStringW	m_SM_PLAN_FACL_PREF_1_CD;
	CStringW	m_SM_PLAN_FACL_PREF_2_CD;
	CStringW	m_SM_PLAN_FACL_PREF_3_CD;
	CStringW	m_SM_PLAN_FACL_PREF_4_CD;
	CStringW	m_SM_PLAN_CAST_SPEC;
	long	m_SM_PLAN_FP_PST_DATE;
	long	m_SM_PLAN_FP_PST_TIME;
	CStringW	m_FP_M_ORDER_NO;
	long	m_USER_ID;

// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


