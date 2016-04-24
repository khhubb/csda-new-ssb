#if !defined(AFX_SCENMGRCHILDFRAME_H__25595553_B122_11D0_9766_0000C0C2484C__INCLUDED_)
#define AFX_SCENMGRCHILDFRAME_H__25595553_B122_11D0_9766_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMgrChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScenMgrChildFrame frame


#include "ChildFrm.h"

class CScenMgrChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CScenMgrChildFrame)
protected:
	CScenMgrChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMgrChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScenMgrChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CScenMgrChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMGRCHILDFRAME_H__25595553_B122_11D0_9766_0000C0C2484C__INCLUDED_)
