#if !defined(AFX_CASTERSTRINGTEMPLATESPECSSET_H__C6D2F271_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGTEMPLATESPECSSET_H__C6D2F271_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringTemplateSpecsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplateSpecsSet recordset

class CCasterStringTemplateSpecsSet : public CRecordset
{
public:
	CCasterStringTemplateSpecsSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringTemplateSpecsSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringTemplateSpecsSet, CRecordset)
	int		m_Caster;
	long	m_ScenId;
	int		m_SeqNum;
	CString	m_Spec;
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
	//{{AFX_VIRTUAL(CCasterStringTemplateSpecsSet)
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

#endif // !defined(AFX_CASTERSTRINGTEMPLATESPECSSET_H__C6D2F271_46B9_11D1_804A_006097B38214__INCLUDED_)
