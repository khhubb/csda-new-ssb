#if !defined(AFX_CASTERSTRINGHEATCHEMSSET_H__DDA7A9DC_C192_11D1_806F_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGHEATCHEMSSET_H__DDA7A9DC_C192_11D1_806F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringHeatChemsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringHeatChemsSet recordset

class CCasterStringHeatChemsSet : public CRecordset
{
public:
	CCasterStringHeatChemsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringHeatChemsSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringHeatChemsSet, CRecordset)
	int		m_Caster;
	CString	m_Element;
	int		m_HeatSeqNum;
	double	m_MaxValue;
	double	m_MinValue;
	long	m_ScenId;
	int		m_StringId;
	int		m_Version;
	int		m_Week;
	int		m_Year;
	double	m_AimValue;
	//}}AFX_FIELD


	long	m_paramScenId;
	int		m_paramVersion;
	int		m_paramYear;
	int		m_paramWeek;
	int		m_paramCaster;
	int		m_paramStringId;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterStringHeatChemsSet)
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

#endif // !defined(AFX_CASTERSTRINGHEATCHEMSSET_H__DDA7A9DC_C192_11D1_806F_006097B38214__INCLUDED_)
