#if !defined(AFX_MILLORDERCHEMSSET_H__B4BA9EED_58B7_11D1_8054_006097B38214__INCLUDED_)
#define AFX_MILLORDERCHEMSSET_H__B4BA9EED_58B7_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MillOrderChemsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMillOrderChemsSet recordset

class CMillOrderChemsSet : public CRecordset
{
public:
	CMillOrderChemsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMillOrderChemsSet)

// Field/Param Data
	//{{AFX_FIELD(CMillOrderChemsSet, CRecordset)
	CString	m_FP_M_ORDER_NO;
	long	m_MILL_ORDER_NO;
	CString	m_MORD_INHSE_ORDER_CHEM_EL;
	float	m_MORD_INHSE_ORDER_PCT_MAX;
	float	m_MORD_INHSE_ORDER_PCT_MIN;
	CString	m_MORD_INHSE_ORDER_REAS_CD;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMillOrderChemsSet)
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

#endif // !defined(AFX_MILLORDERCHEMSSET_H__B4BA9EED_58B7_11D1_8054_006097B38214__INCLUDED_)
