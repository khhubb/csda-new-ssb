#if !defined(AFX_SPECSELECTIONDLG_H__8911D84B_9521_11D1_8065_006097B38214__INCLUDED_)
#define AFX_SPECSELECTIONDLG_H__8911D84B_9521_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpecSelectionDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSpecSelectionDlg dialog

class CSpecSelectionDlg : public CDialog
{
// Construction
public:
	CSpecSelectionDlg(CWnd* pParent = NULL);   // standard constructor

	set<CString> m_specs;
		// the set of spec names for cross-app specs

	set<CString> m_orderedSpecs;
		// the set of spec name for specs actually on orders

	int m_caster;
		// the caster this is for

	void SetSpecFromList(CListBox& lb);


	bool m_bClientPos;
		// we are to position the dialog at the lower-right corner of the parent

	void SetInitialPos();

// Dialog Data
	//{{AFX_DATA(CSpecSelectionDlg)
	enum { IDD = IDD_DLG_SELECT_SPEC };
	CListBox	m_listCrossAppSpecs;
	CListBox	m_listOrderedSpecs;
	CString	m_caption;
	CString	m_selectedSpec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpecSelectionDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListCrossappSpecs();
	afx_msg void OnSelchangeListOrderedSpecs();
	afx_msg void OnButtonSetAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECSELECTIONDLG_H__8911D84B_9521_11D1_8065_006097B38214__INCLUDED_)
