#if !defined(AFX_CASTERSCENINFODLG_H__E262DF54_4C20_11D1_804C_006097B38214__INCLUDED_)
#define AFX_CASTERSCENINFODLG_H__E262DF54_4C20_11D1_804C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterScenInfoDlg.h : header file
//

#include "MessageBus.h"
class CCasterScen;

/////////////////////////////////////////////////////////////////////////////
// CCasterScenInfoDlg dialog

class CCasterScenInfoDlg : public CDialog
{
// Construction
public:
	CCasterScenInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCasterScenInfoDlg)
	enum { IDD = IDD_DLG_CASTER_SCEN_INFO };
	int		m_casterNum;
	CString	m_comment;
	COleDateTime	m_creationDate;
	long	m_id;
	COleDateTime	m_lastSaveDate;
	CString	m_owner;
	int		m_version;
	//}}AFX_DATA

	class CCasterScenInfoChangedMsg : public CCsdaMsg_Impl<MSG_ID_CSI_CASTER_SCEN_INFO_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal change in CSOrder parms within this editor
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CCasterScenInfoChangedMsg(CCasterScen* pScen)
			: m_pCasterScen(pScen)
		{}
		
		CCasterScen* CasterScen() { return m_pCasterScen; }

	protected:
		CCasterScen* m_pCasterScen;
		// the caster scen that was modified.
	};


	CCasterScen* m_pCasterScen;
	bool m_bCommentModified;

	void SetButtonStates();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterScenInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCasterScenInfoDlg)
	afx_msg void OnButtonSave();
	afx_msg void OnButtonUpdateComment();
	afx_msg void OnChangeEditComment();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTERSCENINFODLG_H__E262DF54_4C20_11D1_804C_006097B38214__INCLUDED_)
