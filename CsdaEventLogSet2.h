#if !defined(AFX_CSDAEVENTLOGSET2_H__84455B45_50EC_11D2_85B9_00104B2D39AC__INCLUDED_)
#define AFX_CSDAEVENTLOGSET2_H__84455B45_50EC_11D2_85B9_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CsdaEventLogSet2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet2 recordset

class CCsdaEventLogSet2 : public CRecordset
{
public:
	CCsdaEventLogSet2(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCsdaEventLogSet2)

// Field/Param Data
	//{{AFX_FIELD(CCsdaEventLogSet2, CRecordset)
	CString	m_Comment;
	CString	m_EventType;
	CTime	m_EventTimestamp;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCsdaEventLogSet2)
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

#endif // !defined(AFX_CSDAEVENTLOGSET2_H__84455B45_50EC_11D2_85B9_00104B2D39AC__INCLUDED_)
