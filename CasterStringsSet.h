#if !defined(AFX_CASTERSTRINGSSET_H__C6D2F26A_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGSSET_H__C6D2F26A_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringsSet recordset

class CCasterStringsSet : public CRecordset
{
public:
	CCasterStringsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringsSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringsSet, CRecordset)
	int		m_AimHeatSize;
	BOOL	m_CanEdit;
	int		m_Caster;
	BOOL	m_FreezeHeats;
	int		m_MaxHeatSize;
	int		m_MinHeatSize;
	int		m_NumHeats;
	int		m_OrigStringStringId;
	int		m_OrigStringWeek;
	int		m_OrigStringYear;
	long	m_ScenId;
	CString	m_Status;
	int		m_StringId;
	int		m_Version;
	int		m_Week;
	int		m_Year;
	BOOL	m_UseOriginal;
	int		m_HeatLockStatus;
	int		m_NumHeatsLocked;
	double	m_EndWidth1;
	double	m_EndWidth2;
	BOOL	m_IsValidStartTime;
	double	m_MaxCarbon;
	double	m_MaxMn;
	double	m_MinCarbon;
	double	m_MinMn;
	CString	m_Spec;
	CTime	m_StartTime;
	double	m_StartWidth1;
	double	m_StartWidth2;
	long	m_TurnaroundTime;
	//}}AFX_FIELD


	long	m_paramScenId;
	int		m_paramVersion;
	int		m_paramYear;
	int		m_paramWeek;
	int		m_paramCaster;
	int		m_paramStringId;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterStringsSet)
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

#endif // !defined(AFX_CASTERSTRINGSSET_H__C6D2F26A_46B9_11D1_804A_006097B38214__INCLUDED_)
