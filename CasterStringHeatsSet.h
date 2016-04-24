#if !defined(AFX_CASTERSTRINGHEATSSET_H__DDA7A9DB_C192_11D1_806F_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGHEATSSET_H__DDA7A9DB_C192_11D1_806F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringHeatsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringHeatsSet recordset

class CCasterStringHeatsSet : public CRecordset
{
public:
	CCasterStringHeatsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringHeatsSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringHeatsSet, CRecordset)
	CString	m_Comment;
	int		m_CondnCode;
	int		m_DispCode;
	BOOL	m_IsMarked;
	long	m_ScenId;
	int		m_SeqNum;
	CString	m_SignifEl;
	CString	m_Spec;
	CString	m_Status;
	int		m_StringCaster;
	int		m_StringId;
	int		m_StringWeek;
	int		m_StringYear;
	long	m_TabNum;
	long	m_Tons;
	int		m_Version;
	//}}AFX_FIELD

	
	long	m_paramScenId;
	int		m_paramVersion;
	int		m_paramYear;
	int		m_paramWeek;
	int		m_paramCaster;
	int		m_paramStringId;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterStringHeatsSet)
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

#endif // !defined(AFX_CASTERSTRINGHEATSSET_H__DDA7A9DB_C192_11D1_806F_006097B38214__INCLUDED_)
