#if !defined(AFX_HEATDETAILSDLG_H__D61C7930_FF8D_11D0_801F_006097B38214__INCLUDED_)
#define AFX_HEATDETAILSDLG_H__D61C7930_FF8D_11D0_801F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HeatDetailsDlg.h : header file
//

class CCastString;
class CCasterScen;
class CSuperScen;
class CSnapshot;
class CCastStringHeat;
class CCSOrder;


#include "HeatDetailsGrid.h"
#include "HeatChemsGrid.h"
#include "Chem.h"
#include "CastStringHeat.h"
#include "CastStringHeatValidnError.h"
#include "CastStringValidator.h"


/////////////////////////////////////////////////////////////////////////////
// CHeatDetailsDlg dialog




class CHeatDetailsDlg : public CDialog
{

public:

	enum HEAT_COLS {
		HEAT_COL_MARK = 1,
		HEAT_COL_TAB_NUM,
		HEAT_COL_TONS,
		HEAT_COL_DURATION,
		HEAT_COL_SPEC,
		//HEAT_COL_CONDN,
		HEAT_COL_C_MIN,
		HEAT_COL_C_MAX,
		HEAT_COL_MN_MIN,
		HEAT_COL_MN_MAX,
		HEAT_COL_SIG_EL,
		HEAT_COL_SIG_EL_MIN,
		HEAT_COL_SIG_EL_MAX,
		HEAT_COL_COMMENT };

	enum { LAST_HEAT_COL = HEAT_COL_COMMENT };

	enum CHEM_COLS {
		CHEM_COL_ELT = 1,
		CHEM_COL_MIN,
		CHEM_COL_MAX,
		CHEM_COL_AIM };

	enum DetailType { 
		DETAIL_ORDERS,
		DETAIL_CHEMS,
		DETAIL_VALIDATION };

	enum E_GetHeatSpecAction {
		GHSA_CONTINUE,
		GHSA_SET_ALL,
		GHSA_QUIT };


	DetailType m_detailType;
		// which tab to display

	int m_currentHeatIndex;
		// which heat is currently selected

	CCastString* m_pCastString;
	CCasterScen* m_pCasterScen;
	CSuperScen* m_pSuperScen;
		// the cast string being displayed, and its context

	CCastStringValidator m_validator;
		// keep our own validator for use.

	static vector<int> m_eltSeq;
		// the order in which to display elements.

	CWnd* m_pActiveTabWnd;
		// which tab is currently active.

	CHeatDetailsGrid m_wndDetailsGrid;
	CHeatChemsGrid m_wndChemsGrid;
		// the grids displayed in two of the tabs.

	vector<CCastStringHeat>& EditedHeats();
		
	// heat details grid

	void InitializeHeatDetailsRows();
	void UpdateDetailRowFromHeat(ROWCOL nRow, const CCastStringHeat& rHeat);

	void OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol);
	BOOL OnEndEditingCell(ROWCOL nrow, ROWCOL ncol);
	BOOL OnValidateCell(ROWCOL nrow, ROWCOL ncol);
	void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);

	void SetHeatDetailRowSelection();
	void ResetHeatDetailRowSelection();


	// orders list

	void SetOrdersListHeader();
	void SetOrdersListItems();
	void SetOrdersListItem(int itemNum,
						   int hIndex,
						   const CCSOrder* pOrder,
						   int strandNum,
						   int orderNum);

	// validation strings list
	void SetValidationListHeader();
	void SetValidationListItems();
	void SetValidationComboItems();


	// chems grid
	void InitializeHeatChemsRows();
	void UpdateHeatChemRow(int hIndex,
						   ROWCOL nrow,
						   CChem::Element elt);


	BOOL OnEndEditingChemCell(ROWCOL nrow, ROWCOL ncol);

	bool MarkedHeatsExist(bool displayWarning);


	bool IntersectChems(int strandNum,
						CCSOrder* pOrder,
						vector<bool>&badChems,
						ostrstream& ostr);

	E_GetHeatSpecAction GetHeatSpecFromUser(CCastStringHeat& rHeat, int heatIndex, CString& rSpecName );
	CString AdjustSpecFor05(CCastStringHeat& rHeat, CString& rSpecName);

	void OnButtonEditOrders(int strandNum) ;

	void SetChemsForHeat(CCastStringHeat& rHeat,
						 int heatIndex);
	bool SetChemsForHeat(CCastStringHeat& rHeat,
						 int heatIndex,
						 int caster,
						 vector<bool>& badChems,
						 ostrstream& ostr);



// Construction
public:
	CHeatDetailsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHeatDetailsDlg)
	enum { IDD = IDD_CASTER_SCEN_HEAT_DETAILS_DLG };
	CComboBox	m_comboValidnError;
	CListCtrl	m_listValidation;
	CListCtrl	m_listOrders;
	CTabCtrl	m_tabDetail;
	CString	m_stringId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeatDetailsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSetChems();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetSpecs();
	afx_msg void OnButtonSetTabNums();
	afx_msg void OnButtonValidate();
	virtual void OnOK();
	afx_msg void OnButtonMarkAll();
	afx_msg void OnButtonClearMarks();
	afx_msg void OnSelchangeComboValidationError();
	afx_msg void OnDblclkListOrders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEditOrders1();
	afx_msg void OnButtonEditOrders2();
	afx_msg void OnItemchangedListValidationStrings(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATDETAILSDLG_H__D61C7930_FF8D_11D0_801F_006097B38214__INCLUDED_)
