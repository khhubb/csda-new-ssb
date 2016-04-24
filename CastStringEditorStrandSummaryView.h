#if !defined(AFX_CASTSTRINGEDITORSTRANDSUMMARYVIEW_H__A06B9DAD_29DC_11D1_8032_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORSTRANDSUMMARYVIEW_H__A06B9DAD_29DC_11D1_8032_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorStrandSummaryView.h : header file
//


#include "CastStringEditorMvc.h"
class CCastStringEditorDoc;

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorStrandSummaryView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCastStringEditorStrandSummaryView : public CFormView
{
protected:
	CCastStringEditorStrandSummaryView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorStrandSummaryView)

// Form Data
public:
	//{{AFX_DATA(CCastStringEditorStrandSummaryView)
	enum { IDD = IDD_DLG_STRAND_SUMMARY };
	CListCtrl	m_listSummary;
	BOOL	m_showAvgWidth;
	BOOL	m_showCmsFlips;
	BOOL	m_showCurrentBackFuture;
	BOOL	m_showSlitsSplits;
	BOOL	m_showStock;
	BOOL	m_showHeatStrands;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

	CCastStringEditorStrandSummaryViewport  m_VP;

	CCastStringEditorDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorStrandSummaryView)
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
	virtual ~CCastStringEditorStrandSummaryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorStrandSummaryView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORSTRANDSUMMARYVIEW_H__A06B9DAD_29DC_11D1_8032_006097B38214__INCLUDED_)

