#if !defined(AFX_HEATLOCKDLG_H__ACF293D5_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_)
#define AFX_HEATLOCKDLG_H__ACF293D5_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HeatLockDlg.h : header file
//


#include "CastString.h"
#include "NStateBitmapButton.h"


/////////////////////////////////////////////////////////////////////////////
// CHeatLockDlg dialog

class CHeatLockDlg : public CDialog
{
// Construction
public:
	CHeatLockDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHeatLockDlg)
	enum { IDD = IDD_DLG_HEAT_LOCK };
	CEdit	m_editNum;
	CSpinButtonCtrl	m_spin;
	int		m_numLockedHeats;
	//}}AFX_DATA


	CNStateBitmapButton m_buttonAll;
	CNStateBitmapButton m_buttonSome;
	CNStateBitmapButton m_buttonNone;
		// to subclass some buttons on the interface.

	CCastString::E_HeatLockStatus m_heatStatus;
		// the heat status value

	bool m_bClientPos;
		// should we position the dialog, using the given ccordinates?

	int m_x;
	int m_y;
		// where to pop up the dialog, coordinates relative to screen

	void SetState();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatLockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeatLockDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonSome();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATLOCKDLG_H__ACF293D5_7E8E_11D2_85CB_00104B2D39AC__INCLUDED_)
