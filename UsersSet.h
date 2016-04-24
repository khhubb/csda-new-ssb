#if !defined(AFX_USERSSET_H__D1505333_4B0F_11D1_804B_006097B38214__INCLUDED_)
#define AFX_USERSSET_H__D1505333_4B0F_11D1_804B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UsersSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsersSet recordset

class CUsersSet : public CRecordset
{
public:
	CUsersSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CUsersSet)

// Field/Param Data
	//{{AFX_FIELD(CUsersSet, CRecordset)
	CString	m_LoginName;
	CString	m_Password;
	CString	m_FirstName;
	CString	m_LastName;
	CString	m_SBId;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsersSet)
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

#endif // !defined(AFX_USERSSET_H__D1505333_4B0F_11D1_804B_006097B38214__INCLUDED_)
