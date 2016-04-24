#if !defined(AFX_DAILYPLANLASTDOWNLOADTIMESET_H__A39F2A18_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_)
#define AFX_DAILYPLANLASTDOWNLOADTIMESET_H__A39F2A18_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DailyPlanLastDownloadTimeSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDailyPlanLastDownloadTimeSet recordset

class CDailyPlanLastDownloadTimeSet : public CRecordset
{
public:
	CDailyPlanLastDownloadTimeSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDailyPlanLastDownloadTimeSet)

// Field/Param Data
	//{{AFX_FIELD(CDailyPlanLastDownloadTimeSet, CRecordset)
	CTime	m_Last_Download;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDailyPlanLastDownloadTimeSet)
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

#endif // !defined(AFX_DAILYPLANLASTDOWNLOADTIMESET_H__A39F2A18_4BE4_11D2_85B8_00104B2D39AC__INCLUDED_)
