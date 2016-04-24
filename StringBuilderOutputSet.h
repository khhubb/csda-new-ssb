#if !defined(AFX_STRINGBUILDEROUTPUTSET_H__75DCB615_6802_11D3_85F4_00104B2D39AC__INCLUDED_)
#define AFX_STRINGBUILDEROUTPUTSET_H__75DCB615_6802_11D3_85F4_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringBuilderOutputSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderOutputSet recordset

class CStringBuilderOutputSet : public CRecordset
{
public:
	CStringBuilderOutputSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStringBuilderOutputSet)

// Field/Param Data
	//{{AFX_FIELD(CStringBuilderOutputSet, CRecordset)
	long	m_PLAN_WEEK;
	CString	m_PLAN_CAST_UNIT_CODE;
	long	m_PLAN_CAST_SEQUENCE_NO;
	long	m_PLAN_STRING_ID;
	long	m_PLAN_HEAT_GROUP;
	long	m_PLAN_HEAT_SEQUENCE_NO;
	long	m_PLAN_HEAT_TAB_NUMBER;
	long	m_PLAN_HEAT_SPEC;
	CString	m_PLAN_HEAT_SIXTH_DIGIT_SPEC;
	long	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC;
	long	m_PLAN_STEEL_EARLY_START_DATE;
	long	m_PLAN_STEEL_EARLY_START_TIME;
	long	m_PLAN_STEEL_LATEST_END_DATE;
	long	m_PLAN_STEEL_LATEST_END_TIME;
	long	m_PLAN_HEAT_PROD_TIME_MINUTES;
	long	m_PLAN_HEAT_DISPOSITION_CODE;
	long	m_PLAN_HEAT_BACKUP_MOLD_CODE;
	float	m_PLAN_HEAT_CARBON_MIN;
	float	m_PLAN_HEAT_CARBON_MAX;
	float	m_PLAN_HEAT_MANGANESE_MIN;
	float	m_PLAN_HEAT_MANGANESE_MAX;
	CString	m_PLAN_HEAT_STATUS_CODE;
	long	m_PLAN_LINEUP_NUMBER;
	long	m_PLAN_LINEUP_ITEM_SEQ_NO;
	CString	m_PLAN_LINEUP_COMMENTS;
	CString	m_PLAN_910_REPORT_IND;
	long	m_PLAN_LOT_NUMBER;
	long	m_MILL_ORDER_NO;
	float	m_PLAN_STEEL_WIDTH;
	float	m_PLAN_STEEL_THICKNESS;
	float	m_PLAN_STEEL_LENGTH;
	float	m_PLAN_STEEL_WGT_TONS;
	long	m_PLAN_NUMBER_OF_PIECES;
	CString	m_PLAN_LOT_SPEC;
	long	m_PLAN_LOT_DISP_CODE;
	CString	m_PLAN_LOT_TYPE_SLAB;
	long	m_PLAN_SLAB_CONDITION_CD;
	CString	m_PLAN_EXPOSURE_CODE;
	long	m_PLAN_HOT_MILL_CD;
	CString	m_PLAN_CASTER_STRAND_NO;
	CString	m_PLAN_CHANGE_INDICATOR;
	CString	m_PLAN_ORIGINAL_PLAN_ID;
	CString	m_PLAN_EXPECTED_APPLIED_SPEC;
	long	m_PLAN_EXPECTED_APPLIED_COND_CD;
	long	m_PLAN_TO_BOF_DATE;
	CString	m_PLAN_LOT_TYPE_SLIT_CODE;
	long	m_PROV_SLABS_DUE;
	long	m_PROV_SLAB_WGT_DUE;
	CString	m_PLAN_909_LOCK_IND;
	float	m_PLAN_STEEL_LENGTH_MIN;
	float	m_PLAN_STEEL_LENGTH_MAX;
	CString	m_FILLER0;
	CString	m_FP_M_ORDER_NO;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringBuilderOutputSet)
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

#endif // !defined(AFX_STRINGBUILDEROUTPUTSET_H__75DCB615_6802_11D3_85F4_00104B2D39AC__INCLUDED_)
