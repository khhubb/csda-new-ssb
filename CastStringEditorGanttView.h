#if !defined(AFX_CASTSTRINGEDITORGANTTVIEW_H__15713301_13EB_11D1_8027_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORGANTTVIEW_H__15713301_13EB_11D1_8027_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorGanttView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttView view



class CCastStringEditorDoc;
#include "GanttDisplay.h"
#include "CastStringEditorMvc.h"



class CCastStringEditorGanttView : public CGanttChart
{
protected:
	CCastStringEditorGanttView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorGanttView)

// Attributes
public:

	CCastStringEditorGanttViewport m_VP;
		// the viewport managing the display

	CCastStringEditorDoc* GetDocument();




// Operations
public:

	void ShowContextMenu(CPoint point);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorGanttView)
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


// Implementation
protected:
	virtual ~CCastStringEditorGanttView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorGanttView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORGANTTVIEW_H__15713301_13EB_11D1_8027_006097B38214__INCLUDED_)


