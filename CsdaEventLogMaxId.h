#if !defined(AFX_CSDAEVENTLOGMAXID_H__ADCAEF05_7264_11D2_85C3_00104B2D39AC__INCLUDED_)
#define AFX_CSDAEVENTLOGMAXID_H__ADCAEF05_7264_11D2_85C3_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CsdaEventLogMaxId.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogMaxId recordset

class CCsdaEventLogMaxId : public CRecordset
{
public:
	CCsdaEventLogMaxId(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCsdaEventLogMaxId)

// Field/Param Data
	//{{AFX_FIELD(CCsdaEventLogMaxId, CRecordset)
	long	m_MaxOfEventId;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCsdaEventLogMaxId)
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

#endif // !defined(AFX_CSDAEVENTLOGMAXID_H__ADCAEF05_7264_11D2_85C3_00104B2D39AC__INCLUDED_)
