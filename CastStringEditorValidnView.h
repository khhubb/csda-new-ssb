#if !defined(AFX_CASTSTRINGEDITORVALIDNVIEW_H__FDB739B3_EE9C_11D2_85E0_00104B2D39AC__INCLUDED_)
#define AFX_CASTSTRINGEDITORVALIDNVIEW_H__FDB739B3_EE9C_11D2_85E0_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CastStringEditorValidnView.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorValidnView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CastStringEditorMvc.h"
class CCastStringEditorDoc;


class CCastStringEditorValidnView : public CFormView
{
protected:
	CCastStringEditorValidnView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorValidnView)

// Form Data
public:
	//{{AFX_DATA(CCastStringEditorValidnView)
	enum { IDD = IDD_DLG_VALIDN_ERRORS };
	CListCtrl	m_listValidation;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

	CCastStringEditorValidnViewport  m_VP;

	CCastStringEditorDoc* GetDocument();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorValidnView)
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
	virtual ~CCastStringEditorValidnView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorValidnView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORVALIDNVIEW_H__FDB739B3_EE9C_11D2_85E0_00104B2D39AC__INCLUDED_)
