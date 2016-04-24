//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_REPORTDLG_H__91177173_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
#define AFX_REPORTDLG_H__91177173_CE15_11D4_8649_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog

class CReportDlg : public CDialog
{
// Construction
public:
	CReportDlg(CWnd* pParent = NULL);   // standard constructor


	CString m_filename;
		//  The filename of the file holding the report

	bool m_enablePrint;
		// is printing enabled?

// Dialog Data
	//{{AFX_DATA(CReportDlg)
	enum { IDD = IDD_DLG_REPORT };
	CButton	m_btnPrint;
	CWebBrowser2	m_browser;
	//}}AFX_DATA

	// LAYOUT MANAGER	
	SECLayoutFactory m_LFactory;
	void InitLayout();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportDlg)
	afx_msg void OnButtonPrint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDownloadCompleteExplorer1();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDLG_H__91177173_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
