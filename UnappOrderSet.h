#if !defined(AFX_UNAPPORDERSET_H__F60F11C1_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_UNAPPORDERSET_H__F60F11C1_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UnappOrderSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUnappOrderSet recordset

class CUnappOrderSet : public CRecordset
{
public:
	CUnappOrderSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CUnappOrderSet)

// Field/Param Data
	//{{AFX_FIELD(CUnappOrderSet, CRecordset)
	CString	m_HM_PLAN_MEGA_LINEUP_CD;
	long	m_MILL_ORDER_NO;
	long	m_MORD_AUTOMOTIVE_CODE;
	long	m_MORD_BOOK_MILL;
	float	m_MORD_CARBON_MAX;
	float	m_MORD_CARBON_MIN;
	float	m_MORD_COLD_BAND_WIDTH;
	long	m_MORD_CUST_COIL_REQ;
	long	m_MORD_CUST_PIECE_WGT;
	long	m_MORD_CUSTOMER_CODE;
	CString	m_MORD_CUSTOMER_NAME;
	long	m_MORD_DRAWING_CODE;
	long	m_MORD_GRADE_CODE;
	long	m_MORD_HEAT_CODE;
	float	m_MORD_HOT_BAND_GAUGE;
	float	m_MORD_HOT_BAND_GAUGE_MAX;
	float	m_MORD_HOT_BAND_GAUGE_MIN;
	float	m_MORD_HOT_BAND_WIDTH;
	float	m_MORD_HOT_BAND_WIDTH_MAX;
	float	m_MORD_HOT_BAND_WIDTH_MIN;
	long	m_MORD_HOT_ROLL_WEEK;
	long	m_MORD_HOT_ROLL_YEAR;
	long	m_MORD_HS_TEMPERATURE;
	long	m_MORD_HS_TOLERANCE;
	float	m_MORD_MANGANESE_MAX;
	float	m_MORD_MANGANESE_MIN;
	float	m_MORD_MAXIMUM_SLAB_LENGTH;
	long	m_MORD_MINIMUM_LOAD;
	float	m_MORD_MINIMUM_SLAB_LENGTH;
	long	m_MORD_MISC_PROCESS_CD;
	float	m_MORD_ORDERED_GAUGE;
	float	m_MORD_ORDERED_LENGTH;
	CString	m_MORD_ORDERED_SPEC;
	long	m_MORD_ORDERED_WEIGHT;
	float	m_MORD_ORDERED_WIDTH;
	long	m_MORD_PGM_EXPOSURE;
	long	m_MORD_PGM_PRODUCT_MOD;
	long	m_MORD_PGM_QUALITY_GRADE;
	long	m_MORD_PLAN_PRTY_WK_DAY;
	long	m_MORD_PLAN_PRTY_YR;
	long	m_MORD_PROCESS_CODE;
	long	m_MORD_PRODUCT_CMDTY_CD;
	long	m_MORD_PRODUCT_CODE;
	float	m_MORD_PROV_80_HB_ING_CNT;
	long	m_MORD_PROV_80_HB_ING_POUR_WGT;
	long	m_MORD_PROV_80_HB_ING_SIZE;
	float	m_MORD_PROV_80_HB_LO_PCT_LIM;
	long	m_MORD_PROV_80_HB_PROV_TOL;
	long	m_MORD_PROV_80_HB_SL_CNT;
	long	m_MORD_PROV_80_HB_SL_LNTH;
	long	m_MORD_PROV_80_HB_SL_PC_WGT;
	float	m_MORD_PROV_80_HB_SL_THK;
	float	m_MORD_PROV_80_HB_SL_THK_MAX;
	float	m_MORD_PROV_80_HB_SL_THK_MIN;
	float	m_MORD_PROV_80_HB_SL_WTH;
	float	m_MORD_PROV_80_HB_YF_ACCUM;
	long	m_MORD_PROV_80_HB_YF_PROD;
	float	m_MORD_PROV_80_HB_YLD_FCTR;
	float	m_MORD_PROV_APPLY_WTH;
	long	m_MORD_PROV_CAST_MAX_SLAB_WGT;
	long	m_MORD_PROV_CAST_MIN_SLAB_WGT;
	long	m_MORD_PROV_CAST_SL_LNTH;
	long	m_MORD_PROV_CAST_SL_PC_WGT;
	float	m_MORD_PROV_CAST_SL_THK;
	float	m_MORD_PROV_CAST_SL_WTH;
	long	m_MORD_PROV_HR_PLAN_AMT;
	long	m_MORD_PROV_HR_TOT_PROD;
	long	m_MORD_PROV_MAX_SLAB_WGT;
	long	m_MORD_PROV_MIN_SLAB_WGT;
	long	m_MORD_PROV_PIW_WGT;
	CString	m_MORD_RESTRICTION_CD;
	long	m_MORD_ROUTING_CODE;
	long	m_MORD_SHIP_MILL;
	float	m_MORD_SIGNIF_EL_MAX;
	float	m_MORD_SIGNIF_EL_MIN;
	CString	m_MORD_SIGNIF_EL_SYM;
	CString	m_MORD_SLAB_APPLY_FULL_CD;
	long	m_MORD_SLAB_CONDITION_CD;
	long	m_MORD_STEEL_APPLY_CODE;
	long	m_MORD_STEEL_TYPE_CODE;
	long	m_MORD_TEMPER_ROLL_CD;
	long	m_PROV_ALT_ING_SIZE_CD;
	float	m_PROV_HB_YLD_FCTR;
	long	m_PROV_INDV_SLAB_WGT;
	long	m_PROV_ING_POUR_WGT;
	long	m_PROV_ING_SIZE_CD;
	float	m_PROV_INGOTS_DUE;
	long	m_PROV_NO_SLABS_RLD;
	long	m_PROV_SLAB_LENGTH;
	float	m_PROV_SLAB_THICKNESS;
	long	m_PROV_SLAB_WGT_DUE;
	float	m_PROV_SLAB_WIDTH;
	float	m_PROV_SLAB_YIELD;
	long	m_PROV_SLABS_DUE;
	float	m_SL_APPLY_909_PLAN_HB_WGT;
	float	m_SL_APPLY_910_PLAN_HB_WGT;
	float	m_SL_APPLY_910_PLAN_INGOT_HB_WGT;
	float	m_SL_APPLY_911_PLAN_HB_WGT;
	long	m_SL_APPLY_COND_LEVEL;
	CString	m_SM_PLAN_CAST_SPEC;
	CString	m_SM_PLAN_DEMAND_DUE_IND;
	CString	m_SM_PLAN_FACL_PREF_1_CD;
	CString	m_SM_PLAN_FACL_PREF_2_CD;
	CString	m_SM_PLAN_FACL_PREF_3_CD;
	CString	m_SM_PLAN_FACL_PREF_4_CD;
	CString	m_SM_PLAN_ON_909_IND;
	CString	m_SM_PLAN_ON_910_IND;
	CString	m_SM_PLAN_SLAB_UNIT_CD;
	long	m_SM_PLAN_SLABS_ON_909;
	CString	m_SM_PLAN_SLABS_TO_PLAN_IND;
	CString	m_SM_PLAN_STEEL_SPEC;
	CString	m_SL_APPLY_NEW_ORDER_CODE;
	float	m_MORD_CMS_YIELD_LOSS_FACTOR;
	long	m_MORD_PGM_PRODUCT;
	CString	m_HM_PLAN_HOT_CHARGE_IND;
	long	m_PLAN_910_NO_SL_ORD;
	float	m_SL_APPLY_910_PLAN_SL_WGT;
	float	m_HB_TO_SLAB_YIELD;
	long	m_SM_PLAN_FP_PST_DATE2;
	long	m_SM_PLAN_FP_PST_TIME2;
	long	m_PROV_HB_PROV_TOL;
	int		m_FP_ISHRO_PRTY;
	long	m_FP_LPST_DATE;
	CString	m_FP_M_ORDER_NO;
	long	m_FP_MELT_PST_DATE;
	CString	m_FP_MTO_FLOW_ORD_IND;
	long	m_FP_PLAN_NO_PCS;
	long	m_FP_CAST_DEMAND_YR_WK;     // Added 2-08-05 k. hubbard
	long	m_FP_PLAN_SLAB_WGT;
	long	m_FP_TMSTP_DATE_CHGE;
	long	m_FP_TMSTP_TIME_CHGE;
	float	m_MORD_PROV_CAST_SL_WTH_MAX;   // Added 2 fields k. hubbard 11-11-03
	float	m_MORD_PROV_CAST_SL_WTH_MIN;
	CString	m_MORD_CRITICAL_CUST_IND;  // critical cust req. added 12-1-05 k. hubbard
	CString	m_MORD_HOTROLL_UNIT_NAME;
	CString	m_HEAT_SPEC;
	CString	m_INCLUSION_MODIFIED;
	CString m_MORD_IH_CAST_UNIT_NAME;
	CString m_MORD_AXIOM_ORDER;
	CString m_MORD_SMK_SPEC_WEST;
	CString m_MORD_SMK_COND_WEST;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnappOrderSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNAPPORDERSET_H__F60F11C1_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
