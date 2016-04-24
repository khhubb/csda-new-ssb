#if !defined(AFX_DATETIMEDLG_H__8DF9D8F8_B52E_11D2_85D9_00104B2D39AC__INCLUDED_)
#define AFX_DATETIMEDLG_H__8DF9D8F8_B52E_11D2_85D9_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDateTimeDlg dialog

class CDateTimeDlg : public CDialog
{
// Construction
public:
	CDateTimeDlg(CWnd* pParent = NULL);   // standard constructor

	SECDateTimeCtrl m_dateTimeCtrl;
		// subclass control on dialog

	CTime m_dateTime;
		// the time displayed

	bool m_bDateOnly;
		// do we show date/time or just date


// Dialog Data
	//{{AFX_DATA(CDateTimeDlg)
	enum { IDD = IDD_DLG_DATETIME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDateTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReset();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATETIMEDLG_H__8DF9D8F8_B52E_11D2_85D9_00104B2D39AC__INCLUDED_)
