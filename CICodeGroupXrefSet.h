#if !defined(AFX_CICODEGROUPXREFSET_H__95005B8C_A82B_11D1_8068_006097B38214__INCLUDED_)
#define AFX_CICODEGROUPXREFSET_H__95005B8C_A82B_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CICodeGroupXrefSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupXrefSet recordset

class CCICodeGroupXrefSet : public CRecordset
{
public:
	CCICodeGroupXrefSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCICodeGroupXrefSet)

// Field/Param Data
	//{{AFX_FIELD(CCICodeGroupXrefSet, CRecordset)
	long	m_CMDTY_XREF_CMDTY_GRP_CD;
	CString	m_CMDTY_XREF_CMDTY_CD;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCICodeGroupXrefSet)
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

#endif // !defined(AFX_CICODEGROUPXREFSET_H__95005B8C_A82B_11D1_8068_006097B38214__INCLUDED_)
