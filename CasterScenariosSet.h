#if !defined(AFX_CASTERSCENARIOSSET_H__C6D2F26C_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSCENARIOSSET_H__C6D2F26C_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterScenariosSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosSet recordset

class CCasterScenariosSet : public CRecordset
{
public:
	CCasterScenariosSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterScenariosSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterScenariosSet, CRecordset)
	int		m_Caster;
	CString	m_Comment;
	CTime	m_CreateTime;
	long	m_Id;
	BOOL	m_IsPublic;
	CTime	m_LastSaveTime;
	CString	m_Owner;
	long	m_Version;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterScenariosSet)
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

#endif // !defined(AFX_CASTERSCENARIOSSET_H__C6D2F26C_46B9_11D1_804A_006097B38214__INCLUDED_)
