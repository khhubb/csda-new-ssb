#if !defined(AFX_STOCKAPPLICRULESCHECKSET_H__A953DAA0_00BE_4831_8EA7_3389AC151FFE__INCLUDED_)
#define AFX_STOCKAPPLICRULESCHECKSET_H__A953DAA0_00BE_4831_8EA7_3389AC151FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StockApplicRulesCheckSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StockApplicRulesCheckSet recordset

class StockApplicRulesCheckSet : public CRecordset
{
public:
	StockApplicRulesCheckSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(StockApplicRulesCheckSet)

// Field/Param Data
	//{{AFX_FIELD(StockApplicRulesCheckSet, CRecordset)
	int		m_CASTER_UNIT;
	CString	m_HEAT_SPEC;
	long	m_HEAT_SPEC_NUMERIC_5;
	int		m_HEAT_SPEC_6TH_DIGIT;
	int		m_HEAT_SPEC_7TH_DIGIT;
	int		m_QUALITY;
	CString	m_INCLUSION_MODIFIED;
	CString	m_SLAB_BLOOM_MODE;
	long	m_NUMBER_OF_FURNACES;
	int		m_MINIMUM_WIDTH_INCHES;
	int		m_MAXIMUM_WIDTH_INCHES;
	int		m_PLANNED_HTS_PER_TD_MIN;
	int		m_PLANNED_HTS_PER_TD_MAX;
	int		m_PLANNED_HTS_PER_TD_AIM;
	int		m_PLANNED_SHROUDCHANGE_MIN;
	int		m_PLANNED_SHROUDCHANGE_MAX;
	int		m_PLANNED_SHROUDCHANGE_AIM;
	CString	m_COMMENT;
	CTime	m_DATE_ENTERED;
	CString	m_AUTHOR;
	long	m_SERIAL_NUMBER;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StockApplicRulesCheckSet)
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

#endif // !defined(AFX_STOCKAPPLICRULESCHECKSET_H__A953DAA0_00BE_4831_8EA7_3389AC151FFE__INCLUDED_)
