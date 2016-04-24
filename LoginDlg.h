#if !defined(AFX_LOGINDLG_H__D1505334_4B0F_11D1_804B_006097B38214__INCLUDED_)
#define AFX_LOGINDLG_H__D1505334_4B0F_11D1_804B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LoginDlg.h : header file
//


class CUserInfo;


/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	~CLoginDlg();


	CUserInfo* m_pUserInfo;
		// pointer to the user info structure selected, if any

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_DLG_LOGIN };
	CComboBox	m_comboUsername;
	CString	m_password;
	CString	m_username;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__D1505334_4B0F_11D1_804B_006097B38214__INCLUDED_)
