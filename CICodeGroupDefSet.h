#if !defined(AFX_CICODEGROUPDEFSET_H__95005B8B_A82B_11D1_8068_006097B38214__INCLUDED_)
#define AFX_CICODEGROUPDEFSET_H__95005B8B_A82B_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CICodeGroupDefSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupDefSet recordset

class CCICodeGroupDefSet : public CRecordset
{
public:
	CCICodeGroupDefSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCICodeGroupDefSet)

// Field/Param Data
	//{{AFX_FIELD(CCICodeGroupDefSet, CRecordset)
	long	m_CMDTY_XREF_CMDTY_GRP_CD;
	CString	m_CMDTY_GRP_NAME;
	int		m_CI_CODE;
	long	m_BOGIE;
	long	m_ROLLED;
	long	m_ON_LINEUP;
	long	m_SLABBED_AVAIL;
	long	m_CMS;
	long	m_SHAVES;
	long	m_OTHER_UNAVAIL;
	long	m_TONS_910;
	long	m_TONS_909;
	long	m_TONS_REM_PLAN; 
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCICodeGroupDefSet)
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

#endif // !defined(AFX_CICODEGROUPDEFSET_H__95005B8B_A82B_11D1_8068_006097B38214__INCLUDED_)
