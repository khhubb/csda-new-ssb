#if !defined(AFX_DAILYPLANLINEUPSUPLOADSET_H__2566984C_A3B5_11D1_8067_006097B38214__INCLUDED_)
#define AFX_DAILYPLANLINEUPSUPLOADSET_H__2566984C_A3B5_11D1_8067_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DailyPlanLineupsUploadSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDailyPlanLineupsUploadSet recordset

class CDailyPlanLineupsUploadSet : public CRecordset
{
public:
	CDailyPlanLineupsUploadSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDailyPlanLineupsUploadSet)

// Field/Param Data
	//{{AFX_FIELD(CDailyPlanLineupsUploadSet, CRecordset)
	CString	m_DESC_UNIT_STATUS_TAB_KEY;
	CString	m_DESC_UNIT_TAB_STATUS_KEY;
	CString	m_ELEMENT_SYMBOL;
	CString	m_FP_M_ORDER_NO;
	long	m_LIQUIDOUS_TEMP_C;
	CString	m_LU_RECORD_TYPE;
	long	m_MILL_ORDER_NO;
	CString	m_MORD_ORDERED_SPEC_WITH_DASHES;
	long	m_PLAN_CAST_SEQUENCE_NO;
	CString	m_PLAN_CAST_UNIT_CODE;
	long	m_PLAN_CAST_UNIT_CODE_FULL;
	CString	m_PLAN_CASTER_STRAND_NO;
	long	m_PLAN_DELETE_DATE;
	long	m_PLAN_DELETE_TIME;
	CString	m_PLAN_EXPOSURE_CODE;
	long	m_PLAN_HEAT_BACKUP_MOLD_CODE;
	long	m_PLAN_HEAT_EARLY_START_DATE;
	long	m_PLAN_HEAT_EARLY_START_TIME;
	long	m_PLAN_HEAT_GROUP;
	long	m_PLAN_HEAT_LATEST_END_DATE;
	long	m_PLAN_HEAT_LATEST_END_TIME;
	long	m_PLAN_HEAT_SEQUENCE_NO;
	CString	m_PLAN_HEAT_SPEC_WITH_DASHES;
	CString	m_PLAN_HEAT_STATUS_CODE;
	long	m_PLAN_HEAT_TAB_NUMBER;
	long	m_PLAN_HOT_MILL;
	CString	m_PLAN_LINEUP_COMMENTS;
	long	m_PLAN_LOT_DISP_CODE;
	long	m_PLAN_LOT_NUMBER;
	CString	m_PLAN_LOT_SPEC_WITH_DASHES;
	long	m_PLAN_LOT_STL_EARLY_START_DATE;
	long	m_PLAN_LOT_STL_EARLY_START_TIME;
	long	m_PLAN_LOT_STL_LATEST_END_DATE;
	long	m_PLAN_LOT_STL_LATEST_END_TIME;
	CString	m_PLAN_LOT_TYPE_SLIT_CODE;
	long	m_PLAN_MAX_SLAB_LENGTH_MM;
	long	m_PLAN_MIN_SLAB_LENGTH_MM;
	long	m_PLAN_NUMBER_OF_PIECES;
	long	m_PLAN_SLAB_CONDITION_CD;
	long	m_PLAN_STEEL_LENGTH_MM;
	long	m_PLAN_STEEL_THICKNESS_MM;
	long	m_PLAN_STEEL_WIDTH_MM;
	long	m_PLAN_STRING_EARLY_START_DATE;
	long	m_PLAN_STRING_EARLY_START_TIME;
	long	m_PLAN_STRING_ID;
	long	m_PLAN_STRING_LATEST_END_DATE;
	long	m_PLAN_STRING_LATEST_END_TIME;
	long	m_PLAN_WEEK;
	CString	m_RECORD_DESCRIPTOR;
	long	m_REQUIRED_QUALITY_POINTS;
	CString	m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT;
	CString	m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT;
	CString	m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT;
	long	m_SOLIDOUS_TEMP_C;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDailyPlanLineupsUploadSet)
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

#endif // !defined(AFX_DAILYPLANLINEUPSUPLOADSET_H__2566984C_A3B5_11D1_8067_006097B38214__INCLUDED_)
