#if !defined(AFX_SCENMASTERCHILDFRAME_H__BEF847DB_AAFE_11D1_8068_006097B38214__INCLUDED_)
#define AFX_SCENMASTERCHILDFRAME_H__BEF847DB_AAFE_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMasterChildFrame.h : header file
//


#include "ChildFrm.h"


/////////////////////////////////////////////////////////////////////////////
// CScenMasterChildFrame frame

class CScenMasterChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CScenMasterChildFrame)
protected:
	CScenMasterChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

	CSplitterWnd m_wndSplitter;
		// window is a splitter containing the two views, 
		//    CScenMasterCtrlView and CScenMasterGraphView

	SECToolBar m_wndToolBar;
		// just for fun, we have our own toolbar


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMasterChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScenMasterChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CScenMasterChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMASTERCHILDFRAME_H__BEF847DB_AAFE_11D1_8068_006097B38214__INCLUDED_)
