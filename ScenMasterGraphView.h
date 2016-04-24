#if !defined(AFX_SCENMASTERGRAPHVIEW_H__BEF847DA_AAFE_11D1_8068_006097B38214__INCLUDED_)
#define AFX_SCENMASTERGRAPHVIEW_H__BEF847DA_AAFE_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMasterGraphView.h : header file
//


#include "ScenMasterMvc.h"
class CScenMasterDoc;



/////////////////////////////////////////////////////////////////////////////
// CScenMasterGraphView view

class CScenMasterGraphView : public SECPanView
{
protected:
	CScenMasterGraphView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScenMasterGraphView)




// Attributes
public:

	CScenMasterGraphViewport m_VP;
		// backlink to viewport

	CScenMasterDoc* GetDocument();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMasterGraphView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


// Implementation
protected:
	virtual ~CScenMasterGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScenMasterGraphView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMASTERGRAPHVIEW_H__BEF847DA_AAFE_11D1_8068_006097B38214__INCLUDED_)
