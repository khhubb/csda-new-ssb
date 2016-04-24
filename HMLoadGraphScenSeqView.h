#if !defined(AFX_HMLOADGRAPHSCENSEQVIEW_H__FC54F9CB_060B_11D2_808C_006097B38214__INCLUDED_)
#define AFX_HMLOADGRAPHSCENSEQVIEW_H__FC54F9CB_060B_11D2_808C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMLoadGraphScenSeqView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphScenSeqView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CHMLoadGraphDoc;
#include "HMLoadGraphMvc.h"


class CHMLoadGraphScenSeqView : public CFormView
{
protected:
	CHMLoadGraphScenSeqView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHMLoadGraphScenSeqView)

// Form Data
public:

	SECListBoxEditor m_lbeStrings;

	CHMLoadGraphScenSeqViewport m_VP;
		// the viewport managing the display

	CHMLoadGraphDoc* GetDocument();

	void SetCaster(int caster)
	{
		m_VP.SetCaster(caster);
	}


	//{{AFX_DATA(CHMLoadGraphScenSeqView)
	enum { IDD = IDD_DLG_HM_LOAD_SCEN_SEQ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLoadGraphScenSeqView)
	public:
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


// Implementation
protected:
	virtual ~CHMLoadGraphScenSeqView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHMLoadGraphScenSeqView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLOADGRAPHSCENSEQVIEW_H__FC54F9CB_060B_11D2_808C_006097B38214__INCLUDED_)
