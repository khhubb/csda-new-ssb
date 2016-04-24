#if !defined(AFX_SPECDPACHEMSSET_H__D945FBDB_1310_11D2_808F_006097B38214__INCLUDED_)
#define AFX_SPECDPACHEMSSET_H__D945FBDB_1310_11D2_808F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpecDPAChemsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecDPAChemsSet recordset

class CSpecDPAChemsSet : public CRecordset
{
public:
	CSpecDPAChemsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSpecDPAChemsSet)

// Field/Param Data
	//{{AFX_FIELD(CSpecDPAChemsSet, CRecordset)
	long	m_STEEL_SPECIFICATION;
	CString	m_SIXTH_DIGIT_OF_SPEC;
	long	m_SEVENTH_DIGIT_OF_SPEC;
	CString	m_CASTER_INDICATOR;
	CString	m_ELEMENT_SYMBOL;
	float	m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT;
	float	m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT;
	float	m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT;
	CString	m_SM_SPEC_OPEN_RANGE_IND;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecDPAChemsSet)
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

#endif // !defined(AFX_SPECDPACHEMSSET_H__D945FBDB_1310_11D2_808F_006097B38214__INCLUDED_)
