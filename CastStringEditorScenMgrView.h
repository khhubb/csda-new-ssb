#if !defined(AFX_CASTSTRINGEDITORSCENMGRVIEW_H__D1505337_4B0F_11D1_804B_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORSCENMGRVIEW_H__D1505337_4B0F_11D1_804B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorScenMgrView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorScenMgrView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CastStringEditorMvc.h"
class CCastStringEditorDoc;


class CCastStringEditorScenMgrView : public CFormView
{
protected:
	CCastStringEditorScenMgrView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorScenMgrView)

// Form Data
public:
	//{{AFX_DATA(CCastStringEditorScenMgrView)
	enum { IDD = IDD_DLG_EMBEDDED_SCEN_MGR };
	int		m_casterNumMinus1;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

	CCastStringEditorScenMgrViewport m_VP;

	CCastStringEditorDoc* GetDocument();




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorScenMgrView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


// Implementation
protected:
	virtual ~CCastStringEditorScenMgrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorScenMgrView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORSCENMGRVIEW_H__D1505337_4B0F_11D1_804B_006097B38214__INCLUDED_)

