#if !defined(AFX_CIBOGIESET_H__F60F11BE_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CIBOGIESET_H__F60F11BE_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CIBogieSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCIBogieSet recordset

class CCIBogieSet : public CRecordset
{
public:
	CCIBogieSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCIBogieSet)

// Field/Param Data
	//{{AFX_FIELD(CCIBogieSet, CRecordset)
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
	//{{AFX_VIRTUAL(CCIBogieSet)
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

#endif // !defined(AFX_CIBOGIESET_H__F60F11BE_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
