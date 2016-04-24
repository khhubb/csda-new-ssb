// csda.h : main header file for the CSDA application
//

#if !defined(AFX_CSDA_H__F60F118A_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CSDA_H__F60F118A_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CScenMgrDoc;
class CScenMasterDoc;
class CCastStringEditorDoc;
class CHMLoadGraphDoc;
class CMainFrame;

 
/////////////////////////////////////////////////////////////////////////////
// CCsdaApp:
// See csda.cpp for the implementation of this class
//

class CCsdaApp : public CWinApp
{
public:
	CCsdaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCsdaApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation


private:

	// we keep track of the toplevel doc/view/frame relations.


	CMultiDocTemplate* m_pScenMgrDocTmplt;
	CScenMgrDoc*			m_pScenMgrDoc;

	CMultiDocTemplate* m_pScenMasterDocTmplt;
	CScenMasterDoc*			m_pScenMasterDoc;

	CMultiDocTemplate* m_pHMLoadGraphDocTmplt;
	CHMLoadGraphDoc*		m_pHMLoadGraphDoc;

	CMultiDocTemplate* m_pCastStringEditorDocTmplt;
	CCastStringEditorDoc*	m_pCastStringEditorDoc;

	//  We need a pointer to our class of window for the main frame,
	//  not just the CWnd* type available from CWinApp::m_pMainWnd 

	CMainFrame* m_pMainFrame;


public:

	// accessor

	CMainFrame* MainFrame() 
	{	return m_pMainFrame;	}


	CMultiDocTemplate* GetScenMgrDocTmplt() { return m_pScenMgrDocTmplt; }
	CMultiDocTemplate* GetScenMasterDocTmplt() { return m_pScenMasterDocTmplt; }
	CMultiDocTemplate* GetHMLoadGraphDocTmplt() { return m_pHMLoadGraphDocTmplt; }
	CMultiDocTemplate* GetCastStringEditorDocTmplt() { return m_pCastStringEditorDocTmplt; }

	void ActivateCastStringEditor();


	//{{AFX_MSG(CCsdaApp)
	afx_msg void OnAppAbout();
	afx_msg void OnViewScenarioManager();
	afx_msg void OnViewMiscConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CCsdaApp TheApp;

CCsdaApp* GetMyApp();


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.






#endif // !defined(AFX_CSDA_H__F60F118A_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
