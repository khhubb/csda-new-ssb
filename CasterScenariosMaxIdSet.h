#if !defined(AFX_CASTERSCENARIOSMAXIDSET_H__ACF293D6_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_)
#define AFX_CASTERSCENARIOSMAXIDSET_H__ACF293D6_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterScenariosMaxIdSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosMaxIdSet recordset

class CCasterScenariosMaxIdSet : public CRecordset
{
public:
	CCasterScenariosMaxIdSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterScenariosMaxIdSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterScenariosMaxIdSet, CRecordset)
	long	m_MaxOfId;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterScenariosMaxIdSet)
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

#endif // !defined(AFX_CASTERSCENARIOSMAXIDSET_H__ACF293D6_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_)
