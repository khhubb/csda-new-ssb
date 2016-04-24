#if !defined(AFX_CASTERSTRINGTEMPLATESSET_H__C6D2F26E_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGTEMPLATESSET_H__C6D2F26E_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringTemplatesSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplatesSet recordset

class CCasterStringTemplatesSet : public CRecordset
{
public:
	CCasterStringTemplatesSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringTemplatesSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringTemplatesSet, CRecordset)
	int		m_Caster;
	int		m_CurrentHrWeek;
	int		m_CurrentHrYear;
	BOOL	m_IncludeNewOrders;
	int		m_IncludeOrders;
	int		m_MaxCIPriority;
	long	m_MaxWidth;
	CString	m_MegaLuType;
	int		m_MinCIPriority;
	double	m_MinWidth;
	long	m_ScenId;
	int		m_StringId;
	BOOL	m_Use80HsmLuType;
	BOOL	m_UseAllCICodes;
	BOOL	m_UseAllSpecs;
	BOOL	m_UseCrossApp;
	BOOL	m_UseHrWkCurrentBack;
	BOOL	m_UseHrWkFuture;
	BOOL	m_UseHrWkPlus1;
	BOOL	m_UseHrWkPlus2;
	BOOL	m_UseHrWkPlus3;
	BOOL	m_UseHrWkPlus4;
	BOOL	m_UsePriorityCode;
	BOOL	m_UsePst15to21;
	BOOL	m_UsePst22to28;
	BOOL	m_UsePst29to35;
	BOOL	m_UsePst8to14;
	BOOL	m_UsePstGt35;
	BOOL	m_UsePstLt8;
	BOOL	m_UseWidthRange;
	int		m_Version;
	int		m_Week;
	int		m_Year;
	//}}AFX_FIELD


	long	m_paramScenId;
	int		m_paramVersion;
	int		m_paramYear;
	int		m_paramWeek;
	int		m_paramCaster;
	int		m_paramStringId;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterStringTemplatesSet)
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

#endif // !defined(AFX_CASTERSTRINGTEMPLATESSET_H__C6D2F26E_46B9_11D1_804A_006097B38214__INCLUDED_)
