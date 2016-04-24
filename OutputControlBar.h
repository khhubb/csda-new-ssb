#if !defined(AFX_OUTPUTCONTROLBAR_H__1B0305B6_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
#define AFX_OUTPUTCONTROLBAR_H__1B0305B6_325B_11D3_85E7_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputControlBar.h : header file
//

#include "OutputWnd.h"


/////////////////////////////////////////////////////////////////////////////
// COutputControlBar window

class COutputControlBar : public SECControlBar
{
// Construction
public:
	COutputControlBar();

	DECLARE_DYNAMIC(COutputControlBar)

// Attributes
public:

	enum E_WndId { OCB_LOAD, OCB_VALIDN, OCB_CHECKPOINT	};

	COutputWnd& LoadWnd()
	{	return m_wndLoad;	}

	COutputWnd& ValidnWnd()
	{	return m_wndValidn;	}

	COutputWnd& CheckpointWnd()
	{	return m_wndCheckpoint;	}

	HWND LoadHWnd()
	{	return m_hwndLoad;	}

	HWND ValidnHWnd()
	{	return m_hwndValidn;	}

	HWND CheckpointHWnd()
	{	return m_hwndCheckpoint;	}

	CWnd& GetWnd(E_WndId id);
	HWND GetHWnd(E_WndId id);

	void ShowWnd(E_WndId id);

protected:

	COutputWnd	m_wndLoad;
	COutputWnd	m_wndValidn;
	COutputWnd	m_wndCheckpoint;
		// the various output windows in this control bar

	SECTabWnd	m_wndTab;
		// the tab window holding those output windows.
	
	COutputWnd* m_pWndLoad;
	COutputWnd* m_pWndValidn;
	COutputWnd* m_pWndCheckpoint;
	SECTabWnd*	m_pWndTab;
		// pointers to the windows, 
		// read in the NOutputWnd.cpp  to find why we need three ways (direct, pointer, handle)
		//   to access these windows.

public:

	HWND m_hwndLoad;
	HWND m_hwndValidn;
	HWND m_hwndCheckpoint;
	HWND m_hwndTab;
		// in order to work cross-thread, we need to have the handles available.

// Operations
public:

	virtual void OnExtendContextMenu(CMenu* pMenu);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputControlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputControlBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClearWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// ## change return type to LRESULT
	afx_msg LRESULT OnActivateTab(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMTOutputControlBar window
//
//  See the .cpp file for comments
//

class CMTOutputControlBar : public COutputControlBar
{
// Construction
public:
	CMTOutputControlBar();

// Attributes
public:

	void Attach(COutputControlBar& bar);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMTOutputControlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMTOutputControlBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMTOutputControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTCONTROLBAR_H__1B0305B6_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
