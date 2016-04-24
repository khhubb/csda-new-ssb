#if !defined(AFX_CSDAEVENTLOGSET_H__A39F2A16_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_)
#define AFX_CSDAEVENTLOGSET_H__A39F2A16_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CsdaEventLogSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet recordset

class CCsdaEventLogSet : public CRecordset
{
public:
	CCsdaEventLogSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCsdaEventLogSet)

// Field/Param Data
	//{{AFX_FIELD(CCsdaEventLogSet, CRecordset)
	long	m_EventId;
	CString	m_EventType;
	CTime	m_EventTimestamp;
	CString	m_Comment;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCsdaEventLogSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL


	CString	m_paramEventType;
	long	m_paramLastId;
	CTime	m_paramLastTimestamp;




// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDAEVENTLOGSET_H__A39F2A16_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_)
