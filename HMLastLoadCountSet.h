#if !defined(AFX_HMLASTLOADCOUNTSET_H__CE568AA9_4094_11D2_809C_006097B38214__INCLUDED_)
#define AFX_HMLASTLOADCOUNTSET_H__CE568AA9_4094_11D2_809C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMLastLoadCountSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLastLoadCountSet recordset

class CHMLastLoadCountSet : public CRecordset
{
public:
	CHMLastLoadCountSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CHMLastLoadCountSet)

// Field/Param Data
	//{{AFX_FIELD(CHMLastLoadCountSet, CRecordset)
	long	m_Actual_L_C;
	CTime	m_Date;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLastLoadCountSet)
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

#endif // !defined(AFX_HMLASTLOADCOUNTSET_H__CE568AA9_4094_11D2_809C_006097B38214__INCLUDED_)
