#if !defined(AFX_ORDERINQUIRYDLG_H__D61C792F_FF8D_11D0_801F_006097B38214__INCLUDED_)
#define AFX_ORDERINQUIRYDLG_H__D61C792F_FF8D_11D0_801F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OrderInquiryDlg.h : header file
//


class CSnapshot;
class COrder;
class CSuperScen;

#include "Chem.h"


/////////////////////////////////////////////////////////////////////////////
// COrderInquiryDlg dialog

class COrderInquiryDlg : public CDialog
{
// Construction
public:
	COrderInquiryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderInquiryDlg)
	enum { IDD = IDD_DIALOG_ORDER_INQUIRY };
	CComboBox	m_comboMatchingOrders;
	CListCtrl	m_listChems;
	CListCtrl	m_listLineups;
	CString	m_castSpec;
	int		m_ciCode;
	CString	m_custName;
	double	m_hbGauge;
	double	m_hbWidth;
	CString	m_megaLuCode;
	int		m_numPieces;
	CString	m_orderedSpec;
	int		m_slabCondnCode;
	double	m_slabLength;
	double	m_slabWeight;
	double	m_slabWidth;
	CString	m_steelSpec;
	CString	m_unitPref;
	CString	m_plannedStartDate;
	int		m_expCode;
	double	m_cbWidth;
	CString	m_hrWkYr;
	CString	m_priorityCode;
	long	m_custCode;
	long	m_maxSlabLength;
	double	m_maxSlabWidth;
	long	m_minSlabLength;
	double	m_minSlabWidth;
	long	m_tons;
	double	m_maxTransSlabWidth;
	double	m_orderedGauge;
	CString	m_fpOrderNum;
	int		m_fpIshroPrio;
	CString	m_fpMtoFlow;
	int		m_indexMatchingOrders;
	long	m_fpMeltPst;
	int		m_numSegments;
	CString	m_piecesDueOnLu;
	CString	m_tonsDueOnLu;
	//}}AFX_DATA

	CSuperScen*	m_pSuperScen;
		// the context
	COrder*		m_pOrder;	
		// the order to display information about.

	vector<COrder*> m_matchingOrders;
		// a list of orders matching m_fpOrderNum




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderInquiryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void SetFields();
	void AddChem(int index, CString& elt, double minVal, double maxVal);
	void SetLineupListHeader();
	void SetChemListHeader();
	void SetLineupList();
	void AddScenLineups(int casterNum,int& count);
	void SetChemList();
	void AddEltToChemList(CChem::Element elt, int& count);

	// FP Change
	void SetOrderList();
	void SetParentData();


	// Generated message map functions
	//{{AFX_MSG(COrderInquiryDlg)
	afx_msg void OnButtonLookup();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMatchingOrders();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERINQUIRYDLG_H__D61C792F_FF8D_11D0_801F_006097B38214__INCLUDED_)
