#if !defined(AFX_STEELSPECCROSSAPPSET_H__B4BA9EEC_58B7_11D1_8054_006097B38214__INCLUDED_)
#define AFX_STEELSPECCROSSAPPSET_H__B4BA9EEC_58B7_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SteelSpecCrossAppSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSteelSpecCrossAppSet recordset

class CSteelSpecCrossAppSet : public CRecordset
{
public:
	CSteelSpecCrossAppSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSteelSpecCrossAppSet)

// Field/Param Data
	//{{AFX_FIELD(CSteelSpecCrossAppSet, CRecordset)
	CString	m_SPECARD_SPEC;
	CString	m_SPECARD_CROSS_APPL_SPEC_CD;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSteelSpecCrossAppSet)
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

#endif // !defined(AFX_STEELSPECCROSSAPPSET_H__B4BA9EEC_58B7_11D1_8054_006097B38214__INCLUDED_)
