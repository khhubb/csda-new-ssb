#if !defined(AFX_SCENMASTERCTRLVIEW_H__BEF847DC_AAFE_11D1_8068_006097B38214__INCLUDED_)
#define AFX_SCENMASTERCTRLVIEW_H__BEF847DC_AAFE_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMasterCtrlView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScenMasterCtrlView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ScenMasterMvc.h"
class CScenMasterDoc;


class CScenMasterCtrlView : public CFormView
{
protected:
	CScenMasterCtrlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScenMasterCtrlView)

// Form Data
public:
	//{{AFX_DATA(CScenMasterCtrlView)
	enum { IDD = IDD_DLG_SCEN_MASTER_CTRL_VIEW };
	CListBox	m_listSelScens;
	CListBox	m_listAvailScens;
	CButton	m_btnSelectOne;
	CButton	m_btnSelectAll;
	CButton	m_btnDeselectOne;
	CButton	m_btnDeselectAll;
	//}}AFX_DATA

// Attributes
public:

	CScenMasterCtrlViewport m_VP;
		// backlink to viewport

	CScenMasterDoc* GetDocument();

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMasterCtrlView)
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
	virtual ~CScenMasterCtrlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScenMasterCtrlView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMASTERCTRLVIEW_H__BEF847DC_AAFE_11D1_8068_006097B38214__INCLUDED_)
