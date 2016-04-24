#if !defined(AFX_PPGCICODESELECT_H__4545D596_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_PPGCICODESELECT_H__4545D596_A6F5_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PpgCICodeSelect.h : header file
//


class COrderSelection;
#include "CICode.h"



/////////////////////////////////////////////////////////////////////////////
// CPpgCICodeSelect dialog

class CPpgCICodeSelect : public CPropertyPage
{
	DECLARE_DYNCREATE(CPpgCICodeSelect)

// Construction
public:
	CPpgCICodeSelect();
	~CPpgCICodeSelect();

// Dialog Data
	//{{AFX_DATA(CPpgCICodeSelect)
	enum { IDD = IDD_PPG_CI_CODE_SELECT };
	CButton	m_showAllCheckbox;
	CButton	m_allCodesCheckbox;
	CListCtrl	m_ciCodesList;
	BOOL	m_bUseAllCodes;
	BOOL	m_bShowAllColumns;
	CString	m_selectedCodes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPpgCICodeSelect)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

private:


public:
	void SetOrderSelection(COrderSelection* pOrderSelect);

	bool LocalToOrderSelection();
	void OrderSelectionToLocal();
	bool HasBeenActivated() { return m_hasBeenActivated; }

private:
	bool m_hasBeenActivated;
		// flag whether page has been activated since last Apply

	COrderSelection* m_pOrderSelection;
		// the object being edited

	CCICodeBogieMgr* m_pBogieMgr;
		// part of the order selection, the part we are really interested in here.

	const CCICodeBogieMgr::T_BogiesVec* m_pSelectedBogies;
		// Which bogies have been selected

	void SetCodeListHeaderWidths();
	void SetCodeListItems();


protected:
	// Generated message map functions
	//{{AFX_MSG(CPpgCICodeSelect)
	afx_msg void OnCheckAllCodes();
	afx_msg void OnClickListCiCodes(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckShowAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGCICODESELECT_H__4545D596_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
