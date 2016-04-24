#if !defined(AFX_SPECDPASET_H__D945FBDA_1310_11D2_808F_006097B38214__INCLUDED_)
#define AFX_SPECDPASET_H__D945FBDA_1310_11D2_808F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpecDPASet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecDPASet recordset

class CSpecDPASet : public CRecordset
{
public:
	CSpecDPASet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSpecDPASet)

// Field/Param Data
	//{{AFX_FIELD(CSpecDPASet, CRecordset)
	CString	m_SPEC_NAME;
	CString	m_CASTER_INDICATOR;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecDPASet)
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

#endif // !defined(AFX_SPECDPASET_H__D945FBDA_1310_11D2_808F_006097B38214__INCLUDED_)
