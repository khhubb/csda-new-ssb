#if !defined(AFX_PPGORDERRANGE_H__25595556_B122_11D0_9766_0000C0C2484C__INCLUDED_)
#define AFX_PPGORDERRANGE_H__25595556_B122_11D0_9766_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PpgOrderRange.h : header file
//


class COrderSelection;
class CPSheetAvailOrders;


/////////////////////////////////////////////////////////////////////////////
// CPpgOrderRange dialog

class CPpgOrderRange : public CPropertyPage
{
	DECLARE_DYNCREATE(CPpgOrderRange)

// Construction
public:
	CPpgOrderRange();
	~CPpgOrderRange();

// Dialog Data
	//{{AFX_DATA(CPpgOrderRange)
	enum { IDD = IDD_PPG_ORDER_RANGE };
	BOOL	m_bUseWidthRange;
	double	m_maxWidth;
	double	m_minWidth;
	BOOL	m_bUse80HSMLuType;
	BOOL	m_bUsePriorityCode;
	CString	m_80HSMLuType;
	int		m_orderPriorityMax;
	BOOL	m_bUseAllSpecs;
	CString	m_spec1;
	CString	m_spec2;
	CString	m_spec3;
	CString	m_spec4;
	CString	m_spec5;
	BOOL	m_bUseCrossApp;
	int		m_maxCommPrio;
	int		m_minCommPrio;
	BOOL	m_bIncludeNewOrders;
	int		m_eIncludeOrders;
	BOOL	m_bPst15to21;
	BOOL	m_bPst22to28;
	BOOL	m_bPst29to35;
	BOOL	m_bPst8to14;
	BOOL	m_bPstGt35;
	BOOL	m_bPstLt8;
	BOOL	m_bHrwkCurrentBack;
	BOOL	m_bHrwkFuture;
	BOOL	m_bHrwkPlus1;
	BOOL	m_bHrwkPlus2;
	BOOL	m_bHrwkPlus3;
	BOOL	m_bHrwkPlus4;
	int		m_currentWeek;
	int		m_currentYear;
	CString	m_strPst15to21Tons;
	CString	m_strPst22to28Tons;
	CString	m_strPst29to35Tons;
	CString	m_strPst8to14Tons;
	CString	m_strPstGt35Tons;
	CString	m_strPstLt8Tons;
	BOOL	m_bPstPrio;
	CString	m_strPstPrioTons;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPpgOrderRange)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	COrderSelection* m_pOrderSelection;
		// the object being edited

	bool m_hasBeenActivated;
		// whether or not this page has been activated since the last Apply.

	CPSheetAvailOrders* m_pSheet;
		// the parent sheet.

	void SetSpecListEntries();

public:

	bool HasBeenActivated() { return m_hasBeenActivated; }
	void SetOrderSelection(COrderSelection* pSelect);
	void SetDisplayState();
	void SetSheet(CPSheetAvailOrders* pSheet) { m_pSheet = pSheet; }

	void OrderSelectionToLocal();
	bool LocalToOrderSelection();

	void UpdateOrderSelection();


	// Generated message map functions
	//{{AFX_MSG(CPpgOrderRange)
	afx_msg void OnCheckUseWidthRange();
	afx_msg void OnCheckUse80hsmLuType();
	afx_msg void OnCheckUsePriorityCode();
	afx_msg void OnCheckUseAllSpecs();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeComboSpecAll();
	afx_msg void OnChangeEditMisc();
	afx_msg void OnSelchangeComboSpecAll();
	afx_msg void OnCheckCrossApp();
	afx_msg void OnCheckNewOrders();
	afx_msg void OnRadioShowAll();
	afx_msg void OnRadioShowSwingTons();
	afx_msg void OnRadioShowThisCaster();
	afx_msg void OnCheckPstRange();
	afx_msg void OnCheckHrwkButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGORDERRANGE_H__25595556_B122_11D0_9766_0000C0C2484C__INCLUDED_)
