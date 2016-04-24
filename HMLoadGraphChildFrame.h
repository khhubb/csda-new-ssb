#if !defined(AFX_HMLOADGRAPHCHILDFRAME_H__AA31A9C4_FF34_11D1_8088_006097B38214__INCLUDED_)
#define AFX_HMLOADGRAPHCHILDFRAME_H__AA31A9C4_FF34_11D1_8088_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMLoadGraphChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphChildFrame frame


#include "ChildFrm.h"
#include "GanttDisplay.h"



class CHMLoadGraphChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CHMLoadGraphChildFrame)
protected:
	CHMLoadGraphChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

	SECToolBar m_wndToolBar;

	CSplitterWnd m_wndSplitter;
	//CSplitterWnd m_wndSplitter2;
	SECTabWnd* m_pTabWnd2;
	SECTabWnd* m_pTabWnd3;
	CGanttDisplay m_ganttDisplay;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLoadGraphChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHMLoadGraphChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CHMLoadGraphChildFrame)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnViewScenarios();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLOADGRAPHCHILDFRAME_H__AA31A9C4_FF34_11D1_8088_006097B38214__INCLUDED_)
