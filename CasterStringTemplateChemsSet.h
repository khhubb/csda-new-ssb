#if !defined(AFX_CASTERSTRINGTEMPLATECHEMSSET_H__DC4266CA_477B_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGTEMPLATECHEMSSET_H__DC4266CA_477B_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringTemplateChemsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplateChemsSet recordset

class CCasterStringTemplateChemsSet : public CRecordset
{
public:
	CCasterStringTemplateChemsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringTemplateChemsSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringTemplateChemsSet, CRecordset)
	int		m_Caster;
	CString	m_Element;
	double	m_MaxValue;
	double	m_MinValue;
	long	m_ScenId;
	int		m_StringId;
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
	//{{AFX_VIRTUAL(CCasterStringTemplateChemsSet)
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

#endif // !defined(AFX_CASTERSTRINGTEMPLATECHEMSSET_H__DC4266CA_477B_11D1_804A_006097B38214__INCLUDED_)
