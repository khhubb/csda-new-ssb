#if !defined(AFX_UPLOADVALIDNERRORSDLG_H__E0CB5D96_265F_11D3_85E2_00104B2D39AC__INCLUDED_)
#define AFX_UPLOADVALIDNERRORSDLG_H__E0CB5D96_265F_11D3_85E2_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UploadValidnErrorsDlg.h : header file
//


class CCastString;



/////////////////////////////////////////////////////////////////////////////
// CUploadValidnErrorsDlg dialog



class CUploadValidnErrorsDlg : public CDialog
{
// Construction
public:
	CUploadValidnErrorsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUploadValidnErrorsDlg)
	enum { IDD = IDD_DLG_VALIDN_ERRORS_POPUP };
	CListBox	m_listStrings;
	CListCtrl	m_listValidation;
	CString	m_msg;
	//}}AFX_DATA


	vector<CCastString*> m_strings;
		// CCastStrings containing validation errors

	bool m_bFatalErr;
		// are any of the errors fatal?

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUploadValidnErrorsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUploadValidnErrorsDlg)
	afx_msg void OnSelchangeListStrings();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLOADVALIDNERRORSDLG_H__E0CB5D96_265F_11D3_85E2_00104B2D39AC__INCLUDED_)
