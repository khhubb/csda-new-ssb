#if !defined(AFX_HMSUPPLYACTUAL_H__3D844ABD_27B3_11D2_8097_006097B38214__INCLUDED_)
#define AFX_HMSUPPLYACTUAL_H__3D844ABD_27B3_11D2_8097_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMSupplyActual.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMSupplyActual recordset

class CHMSupplyActual : public CRecordset
{
public:
	CHMSupplyActual(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CHMSupplyActual)

// Field/Param Data
	//{{AFX_FIELD(CHMSupplyActual, CRecordset)
	CTime	m_Date;
	float	m_5_BOF_tons_hr;
	float	m_6_BOF_tons_hr;
	float	m_7_BOF_tons_hr;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMSupplyActual)
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

#endif // !defined(AFX_HMSUPPLYACTUAL_H__3D844ABD_27B3_11D2_8097_006097B38214__INCLUDED_)
