#if !defined(AFX_PICKCASTERDLG_H__918BD996_D379_11D4_8649_00104B2D39AC__INCLUDED_)
#define AFX_PICKCASTERDLG_H__918BD996_D379_11D4_8649_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickCasterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPickCasterDlg dialog

class CPickCasterDlg : public CDialog
{
// Construction
public:
	CPickCasterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPickCasterDlg)
	enum { IDD = IDD_DLG_PICK_CASTER };
	int		m_caster;
	CString	m_label;
	//}}AFX_DATA

	int m_ignoreCaster;
		// the caster we should not pick.


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPickCasterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPickCasterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKCASTERDLG_H__918BD996_D379_11D4_8649_00104B2D39AC__INCLUDED_)
