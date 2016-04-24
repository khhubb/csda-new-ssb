#if !defined(AFX_HMSTARTTIMEDLG_H__F29CF535_0F62_11D2_808E_006097B38214__INCLUDED_)
#define AFX_HMSTARTTIMEDLG_H__F29CF535_0F62_11D2_808E_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMStartTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMStartTimeDlg dialog

class CCastString;

class CHMStartTimeDlg : public CDialog
{
// Construction
public:
	CHMStartTimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHMStartTimeDlg)
	enum { IDD = IDD_DLG_HM_START_TIME };
	CEdit	m_editFinishTime;
	CEdit	m_editStartTime;
	CStatic	m_staticId;
	//}}AFX_DATA


	CCastString* m_pString;
		// The castString whose start time is being tracked.
	
	void SetIdText();
	void SetTime(CTime startTime,CTime finishTime);
	void SetTimeInvalid();
	BOOL Create(const CPoint& pt, CWnd* pParentWnd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMStartTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHMStartTimeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMSTARTTIMEDLG_H__F29CF535_0F62_11D2_808E_006097B38214__INCLUDED_)
