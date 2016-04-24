#if !defined(AFX_MULTISELECTDLG_H__9117717B_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
#define AFX_MULTISELECTDLG_H__9117717B_CE15_11D4_8649_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiSelectDlg dialog

class CMultiSelectDlg : public CDialog
{
// Construction
public:
	CMultiSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiSelectDlg)
	enum { IDD = IDD_DLG_MULTISELECT };
	CCheckListBox	m_listBox;
	//}}AFX_DATA

	vector<CString> m_strings;
		//  input: the set of strings which are the choices

	vector<int> m_posns;
		//  input/output: the positions of the selected strings
		//  used to initialize which strings are selected at entrance
		//  used to indicate which strings are selected at exit
	
	CString m_title;
		//  input: a value to use as the dialog title
		//  empty string = don't change


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiSelectDlg)
	afx_msg void OnButtonDeselectAll();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectHilited();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISELECTDLG_H__9117717B_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
