#if !defined(AFX_SELECT909SDLG_H__09E93CE9_B711_11D1_806D_006097B38214__INCLUDED_)
#define AFX_SELECT909SDLG_H__09E93CE9_B711_11D1_806D_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Select909sDlg.h : header file
//


class CCasterScen;
#include "CastString.h"

/////////////////////////////////////////////////////////////////////////////
// CSelect909sDlg dialog

class CSelect909sDlg : public CDialog
{
// Construction
public:
	CSelect909sDlg(CWnd* pParent = NULL);   // standard constructor

	CCasterScen* m_pScen910;
	CCasterScen* m_pScen909;
	vector<int> m_910sVec;
	vector<int> m_909sVec;
	vector<int> m_indexes909;

	void PopulateList(CCasterScen* pScen,
					  CCastString::E_StringStatus status,
					  vector<int>& vec,
					  CListBox& listBox);


// Dialog Data
	//{{AFX_DATA(CSelect909sDlg)
	enum { IDD = IDD_DLG_SELECT_909s };
	CListBox	m_list910s;
	CListBox	m_list909s;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelect909sDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelect909sDlg)
	afx_msg void OnButtonDeselectAll();
	afx_msg void OnButtonSelectAll();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECT909SDLG_H__09E93CE9_B711_11D1_806D_006097B38214__INCLUDED_)
