#if !defined(AFX_EMAILDLG_H__8D372E56_EAEE_11D2_85DF_00104B2D39AC__INCLUDED_)
#define AFX_EMAILDLG_H__8D372E56_EAEE_11D2_85DF_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EMailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEMailDlg dialog

class CEMailDlg : public CDialog
{
// Construction
public:
	CEMailDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEMailDlg)
	enum { IDD = IDD_DLG_EMAIL };
	CString	m_strBody;
	CString	m_strSubject;
	//}}AFX_DATA

	void AddCC(const CString& strCC);
	void AddRecipient(const CString& strTo);
	void SetBody(const CString& body);
	void SetSubject(const CString& subj);

	CString	m_strBodyOrig;
	CString	m_strSubjectOrig;
		// the original body and subject texts
		// saved so we can restore if asked

	SECListBoxEditor m_LBEditorTo;
	SECListBoxEditor m_LBEditorCC;
		// controls to subclass

	vector<CString> m_vecTo;
	vector<CString> m_vecCC;
		// the strings of TO: and CC: values

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEMailDlg)
	afx_msg void OnReset();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILDLG_H__8D372E56_EAEE_11D2_85DF_00104B2D39AC__INCLUDED_)
