#if !defined(AFX_GANTTCOLORCHARTDLG_H__C913E565_E799_11D0_801B_006097B38214__INCLUDED_)
#define AFX_GANTTCOLORCHARTDLG_H__C913E565_E799_11D0_801B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GanttColorChartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGanttColorChartDlg dialog

class CGanttColorChartDlg : public CDialog
{
// Construction
public:
	CGanttColorChartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGanttColorChartDlg)
	enum { IDD = IDD_DIALOG_GANTT_COLORS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA



	CSize m_stringSize;
	int m_margin;
	int m_topY;
	int m_leftX;

	static vector<CString> sm_strings;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttColorChartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGanttColorChartDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTCOLORCHARTDLG_H__C913E565_E799_11D0_801B_006097B38214__INCLUDED_)
