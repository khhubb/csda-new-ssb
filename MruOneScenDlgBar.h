// MruOneScenDlgBar.h: interface for the CMruOneScenDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRUONESCENDLGBAR_H__BDE8DE68_DED2_11D2_85DF_00104B2D39AC__INCLUDED_)
#define AFX_MRUONESCENDLGBAR_H__BDE8DE68_DED2_11D2_85DF_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMessageBus;
#include "MruOneScenMVC.h"


class CMruOneScenDlgBar : public SECDialogBar
{
public:
	CMruOneScenDlgBar();
	virtual ~CMruOneScenDlgBar();

	SECBitmapButton m_btnDlg;
		// for subclassing

	CMessageBus* GetModel() { return m_pBus;	}

	CMessageBus* m_pBus;
		// the model

	CMruOneScenViewport m_VP;
		// backlink to viewport


	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) ;
	BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


protected:

    afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);

	//{{AFX_MSG(CMruOneScenDlgBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateViewScenarios(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MRUONESCENDLGBAR_H__BDE8DE68_DED2_11D2_85DF_00104B2D39AC__INCLUDED_)
