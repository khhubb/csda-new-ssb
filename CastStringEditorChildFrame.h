#if !defined(AFX_CASTSTRINGEDITORCHILDFRAME_H__15713302_13EB_11D1_8027_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORCHILDFRAME_H__15713302_13EB_11D1_8027_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorChildFrame frame


#include "ChildFrm.h"
#include "GanttDisplay.h"
#include "MessageBus.h"


class CCastStringEditorActivatedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ACTIVATED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
	};



class CCastStringEditorChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CCastStringEditorChildFrame)
protected:
	CCastStringEditorChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

	SEC3DTabWnd m_tabWnd;
	CSplitterWnd m_wndSplitter;

	SECTabWnd* m_pTabWnd2;
//	CSplitterWnd m_wndSplitter2;
	CGanttDisplay m_ganttDisplay;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCastStringEditorChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORCHILDFRAME_H__15713302_13EB_11D1_8027_006097B38214__INCLUDED_)
