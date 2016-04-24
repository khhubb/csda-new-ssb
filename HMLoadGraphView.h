#if !defined(AFX_HMLOADGRAPHVIEW_H__AA31A9C5_FF34_11D1_8088_006097B38214__INCLUDED_)
#define AFX_HMLOADGRAPHVIEW_H__AA31A9C5_FF34_11D1_8088_006097B38214__INCLUDED_

#if _MSC_VER >= 1000 
#pragma once
#endif // _MSC_VER >= 1000
// HMLoadGraphView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphView view



#include "GanttDisplay.h"
#include "HMLoadGraphMvc.h"



class CHMLoadGraphView : public CGanttChart
{
protected:
	CHMLoadGraphView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHMLoadGraphView)

// Attributes
public:

	CHMLoadGraphGanttViewport m_VP;
		// the viewport managing the display

	CHMLoadGraphDoc* GetDocument();




// Operations
public:



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLoadGraphView)
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


// Implementation
protected:
	virtual ~CHMLoadGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHMLoadGraphView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLOADGRAPHVIEW_H__AA31A9C5_FF34_11D1_8088_006097B38214__INCLUDED_)
