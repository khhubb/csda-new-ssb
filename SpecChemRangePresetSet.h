#if !defined(AFX_SPECCHEMRANGEPRESETSET_H__25484151_66DF_11D1_8055_006097B38214__INCLUDED_)
#define AFX_SPECCHEMRANGEPRESETSET_H__25484151_66DF_11D1_8055_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpecChemRangePresetSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecChemRangePresetSet recordset

class CSpecChemRangePresetSet : public CRecordset
{
public:
	CSpecChemRangePresetSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSpecChemRangePresetSet)

// Field/Param Data
	//{{AFX_FIELD(CSpecChemRangePresetSet, CRecordset)
	CString	m_Spec;
	int		m_Group;
	double	m_MinCarbon;
	double	m_MaxCarbon;
	double	m_MinMn;
	double	m_MaxMn;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecChemRangePresetSet)
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

#endif // !defined(AFX_SPECCHEMRANGEPRESETSET_H__25484151_66DF_11D1_8055_006097B38214__INCLUDED_)
