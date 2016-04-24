#if !defined(AFX_STRINGBUILDERINPUTSET_H__75DCB614_6802_11D3_85F4_00104B2D39AC__INCLUDED_)
#define AFX_STRINGBUILDERINPUTSET_H__75DCB614_6802_11D3_85F4_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringBuilderInputSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderInputSet recordset

class CStringBuilderInputSet : public CRecordset
{
public:
	CStringBuilderInputSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStringBuilderInputSet)

// Field/Param Data
	//{{AFX_FIELD(CStringBuilderInputSet, CRecordset)
	long	m_MILL_ORDER_NO;
	float	m_MORD_CARBON_MAX;
	float	m_MORD_CARBON_MIN;
	float	m_MORD_CMS_YIELD_LOSS_FACTOR;
	float	m_MORD_COLD_BAND_WIDTH;
	float	m_MORD_HOT_BAND_GAUGE;
	float	m_MORD_HOT_BAND_GAUGE_MAX;
	float	m_MORD_HOT_BAND_GAUGE_MIN;
	float	m_MORD_HOT_BAND_WIDTH;
	float	m_MORD_HOT_BAND_WIDTH_MAX;
	float	m_MORD_HOT_BAND_WIDTH_MIN;
	long	m_MORD_HOT_ROLL_WEEK;
	long	m_MORD_HOT_ROLL_YEAR;
	float	m_MORD_MANGANESE_MAX;
	float	m_MORD_MANGANESE_MIN;
	float	m_MORD_MAXIMUM_SLAB_LENGTH;
	float	m_MORD_MINIMUM_SLAB_LENGTH;
	float	m_MORD_ORDERED_GAUGE;
	float	m_MORD_ORDERED_LENGTH;
	CString	m_MORD_ORDERED_SPEC;
	long	m_MORD_ORDERED_WEIGHT;
	float	m_MORD_ORDERED_WIDTH;
	long	m_MORD_PLAN_PRTY_WK_DAY;
	long	m_MORD_PRODUCT_CMDTY_CD;
	long	m_MORD_PRODUCT_CODE;
	float	m_MORD_SIGNIF_EL_MAX;
	float	m_MORD_SIGNIF_EL_MIN;
	CString	m_MORD_SIGNIF_EL_SYM;
	CString	m_MORD_SIXTH_DIGIT_OF_SPEC;
	long	m_MORD_SLAB_CONDITION_CD;
	long	m_PROV_HB_PROV_TOL;
	long	m_PROV_INDV_SLAB_WGT;
	long	m_PROV_SLAB_LENGTH;
	float	m_PROV_SLAB_THICKNESS;
	long	m_PROV_SLAB_WGT_DUE;
	float	m_PROV_SLAB_WIDTH;
	long	m_PROV_SLABS_DUE;
	long	m_REPROV_SLABS_DUE;
	CString	m_SM_PLAN_CAST_SPEC;
	CString	m_SM_PLAN_FACL_PREF_1_CD;
	CString	m_SM_PLAN_FACL_PREF_2_CD;
	CString	m_SM_PLAN_FACL_PREF_3_CD;
	CString	m_SM_PLAN_FACL_PREF_4_CD;
	long	m_SM_PLAN_FP_PST_DATE;
	long	m_SM_PLAN_FP_PST_TIME;
	CString	m_SM_PLAN_SLAB_UNIT_CD;
	CString	m_SM_PLAN_STEEL_SPEC;
	CString	m_FP_M_ORDER_NO;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringBuilderInputSet)
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
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGBUILDERINPUTSET_H__75DCB614_6802_11D3_85F4_00104B2D39AC__INCLUDED_)
