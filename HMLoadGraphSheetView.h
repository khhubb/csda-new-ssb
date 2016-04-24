#if !defined(AFX_HMLOADGRAPHSHEETVIEW_H__979CB116_A0FE_11D2_85D5_00104B2D39AC__INCLUDED_)
#define AFX_HMLOADGRAPHSHEETVIEW_H__979CB116_A0FE_11D2_85D5_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMLoadGraphSheetView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphSheetView view


class CHMLoadGraphDoc;
#include "HMLoadGraphMvc.h"


class CHMLoadGraphSheetView : public CView
{
protected:
	CHMLoadGraphSheetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHMLoadGraphSheetView)

// Attributes
public:

	CHMLoadGraphSheetViewport m_VP;
		// the viewport managing the display

	CHMLoadGraphDoc* GetDocument();


// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLoadGraphSheetView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


// Implementation
protected:
	virtual ~CHMLoadGraphSheetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHMLoadGraphSheetView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLOADGRAPHSHEETVIEW_H__979CB116_A0FE_11D2_85D5_00104B2D39AC__INCLUDED_)
