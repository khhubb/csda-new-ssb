// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_MAINFRM_H__F60F118F_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_MAINFRM_H__F60F118F_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MruOneScenDlgBar.h"
#include "OutputControlBar.h"


class CMainFrame : public SECWorkbook
{
	DECLARE_DYNAMIC(CMainFrame)


public:
	CMainFrame();

// Attributes
public:

	// accessors

	CMruOneScenDlgBar& Mru1Bar()
	{	return m_mru1Bar;	}

	COutputControlBar& OutputBar()
	{	return m_wndOutput;	}
	
	
	// an action!

	void ShowMainOutputControlBar();

protected:

	CMruOneScenDlgBar m_mru1Bar;
		//  the most-recently used scenario dialog bar
		//	displayed at the top

	COutputControlBar m_wndOutput;
		//	the control bar containing various output windows.
		//  displayed at the bottom

	

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


	int m_timerId;

	void ActivateTimer();
	void DeactivateTimer();


// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members


#ifdef NORMAL_MAINFRAME
	// Here is the code for standard MDI
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
#endif

	//  Here is the code for SEC's WDI
	SECStatusBar  m_wndStatusBar;
	SECToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ## Change UINT to UINT_PTR
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnViewOutputWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// ## change return type to LRESULT
	// # change args from (UINT,LONG) to (WPARAM,LPARAM)
	afx_msg LRESULT OnShowOutputWindow(WPARAM,LPARAM);

};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F60F118F_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
