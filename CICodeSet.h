#if !defined(AFX_CICODESET_H__F60F11B9_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CICODESET_H__F60F11B9_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CICodeSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCICodeSet recordset

class CCICodeSet : public CRecordset
{
public:
	CCICodeSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCICodeSet)

// Field/Param Data
	//{{AFX_FIELD(CCICodeSet, CRecordset)
	int		m_CI_CODE;
	CString	m_COMMODITY_NAME;
	int		m_CI_CODE_PARENT;
	int		m_SORT_ORDER;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCICodeSet)
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

#endif // !defined(AFX_CICODESET_H__F60F11B9_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
