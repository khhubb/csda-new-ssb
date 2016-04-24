#if !defined(AFX_TABNUMDLG_H__E7A2045F_8BD4_11D1_8065_006097B38214__INCLUDED_)
#define AFX_TABNUMDLG_H__E7A2045F_8BD4_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabNumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabNumDlg dialog

class CTabNumDlg : public CDialog
{
// Construction
public:
	CTabNumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabNumDlg)
	enum { IDD = IDD_DLG_TAB_NUM };
	long	m_tabNum;
	//}}AFX_DATA


	int m_caster;
		// the caster we are choosing a tab# for

	bool m_force2ndDigit;
		// computed internally, whether the second digit is forced to 9

	CString m_prompt;
		// string to display

	void InitTabNum();
	bool ValidateTabNum();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabNumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabNumDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABNUMDLG_H__E7A2045F_8BD4_11D1_8065_006097B38214__INCLUDED_)
