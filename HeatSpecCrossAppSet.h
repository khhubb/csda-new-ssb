#if !defined(AFX_HEATSPECCROSSAPPSET_H__8911D848_9521_11D1_8065_006097B38214__INCLUDED_)
#define AFX_HEATSPECCROSSAPPSET_H__8911D848_9521_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HeatSpecCrossAppSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeatSpecCrossAppSet recordset

class CHeatSpecCrossAppSet : public CRecordset
{
public:
	CHeatSpecCrossAppSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CHeatSpecCrossAppSet)

// Field/Param Data
	//{{AFX_FIELD(CHeatSpecCrossAppSet, CRecordset)
	CString	m_SPECARD_SPEC;
	CString	m_SPECARD_CROSS_APPL_SPEC_CD;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatSpecCrossAppSet)
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

#endif // !defined(AFX_HEATSPECCROSSAPPSET_H__8911D848_9521_11D1_8065_006097B38214__INCLUDED_)
