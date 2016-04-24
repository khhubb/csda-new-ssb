#if !defined(AFX_PERUSERDEFAULTVALUESSET_H__0C2DE447_52CD_11D1_804F_006097B38214__INCLUDED_)
#define AFX_PERUSERDEFAULTVALUESSET_H__0C2DE447_52CD_11D1_804F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PerUserDefaultValuesSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPerUserDefaultValuesSet recordset

class CPerUserDefaultValuesSet : public CRecordset
{
public:
	CPerUserDefaultValuesSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPerUserDefaultValuesSet)

// Field/Param Data
	//{{AFX_FIELD(CPerUserDefaultValuesSet, CRecordset)
	CString	m_LoginName;
	int		m_Caster1HeatSizeMin;
	int		m_Caster1HeatSizeAim;
	int		m_Caster1HeatSizeMax;
	int		m_Caster2HeatSizeMin;
	int		m_Caster2HeatSizeAim;
	int		m_Caster2HeatSizeMax;
	int		m_Caster3HeatSizeMin;
	int		m_Caster3HeatSizeAim;
	int		m_Caster3HeatSizeMax;
	//}}AFX_FIELD


	CString m_loginNameParm;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPerUserDefaultValuesSet)
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

#endif // !defined(AFX_PERUSERDEFAULTVALUESSET_H__0C2DE447_52CD_11D1_804F_006097B38214__INCLUDED_)
