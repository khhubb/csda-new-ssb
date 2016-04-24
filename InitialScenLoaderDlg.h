#if !defined(AFX_INITIALSCENLOADERDLG_H__C28D3540_CDA9_11D1_8071_006097B38214__INCLUDED_)
#define AFX_INITIALSCENLOADERDLG_H__C28D3540_CDA9_11D1_8071_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InitialScenLoaderDlg.h : header file
//


#include "AvailCasterScenId.h"


/////////////////////////////////////////////////////////////////////////////
// CInitialScenLoaderDlg dialog

class CInitialScenLoaderDlg : public CDialog
{
// Construction
public:
	CInitialScenLoaderDlg(CWnd* pParent = NULL);   // standard constructor


	enum ShowType { SHOW_MOST_RECENT, 
		            SHOW_MOST_RECENT_DAY, 
					SHOW_ALL };




// Dialog Data
	//{{AFX_DATA(CInitialScenLoaderDlg)
	enum { IDD = IDD_DLG_INITIAL_SCEN_LOADER };
	CListCtrl	m_listCaster3;
	CListCtrl	m_listCaster2;
	CListCtrl	m_listCaster1;
	int		m_showCategory;
	//}}AFX_DATA


	vector<CAvailCasterScenId> m_availScens;
		// the currently available scens

	vector<CAvailCasterScenId> m_showScens[4];  // caster = 1,2,3
		// which scenarios to display, per caster.

	void FilterScens();
	void SetListHeaders();
	void SetListHeader(CListCtrl& list);
	void SetLists();
	void SetList(CListCtrl& list, int caster);
	void SetListItem(CListCtrl& list, 
					 int itemNum, 
					 CAvailCasterScenId& id);
	void LoadScens(CListCtrl& list, int caster);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitialScenLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitialScenLoaderDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCategory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIALSCENLOADERDLG_H__C28D3540_CDA9_11D1_8071_006097B38214__INCLUDED_)
