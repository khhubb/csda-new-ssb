#if !defined(AFX_SCENMGRVIEW_H__4545D595_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SCENMGRVIEW_H__4545D595_A6F5_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMgrView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScenMgrView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ScenMgrMvc.h"
class CScenMgrDoc;

class CScenMgrView : public CFormView
{
protected:
	CScenMgrView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScenMgrView)

// Form Data
public:
	//{{AFX_DATA(CScenMgrView)
	enum { IDD = IDD_SCEN_MGR_DLG };
	//}}AFX_DATA

// Attributes
public:


	SECTreeCtrl m_treeScens;
		// for subclassing

	CScenMgrViewport m_VP;
		// backlink to viewport

	CScenMgrDoc* GetDocument();





// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMgrView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL
 
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );

// Implementation
protected:
	virtual ~CScenMgrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScenMgrView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMGRVIEW_H__4545D595_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
