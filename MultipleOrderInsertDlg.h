#if !defined(AFX_MULTIPLEORDERINSERTDLG_H__2ADDE8B3_59E0_11D1_8054_006097B38214__INCLUDED_)
#define AFX_MULTIPLEORDERINSERTDLG_H__2ADDE8B3_59E0_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MultipleOrderInsertDlg.h : header file
//

#include "MultipleOrderInsertGrid.h"


class CSuperScen;
class CSnapshot;
class CCSOrder;
#include "CSOrder.h"

/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertDlg dialog

class CMultipleOrderInsertDlg : public CDialog
{

public:

	enum COLS {
		COL_ORD_WIDTH = 1,
		COL_ORD_LENGTH,
		COL_ORD_NUM_PIECES,
		COL_ORDER_NUM,
		COL_SLAB_WIDTH,
		COL_SLAB_LENGTH,
		COL_NUM_PIECES,
		COL_SLAB_CONDN_CODE,
		COL_SPEC,
		COL_COMMENT };

	int m_strandNum;
	int m_casterNum;
	CSuperScen* m_pSuperScen;

	vector< CCSOrder* > m_orders;

	SECBitmapButton m_secBtnOrderInquiry;

	void InitializeCellContents();
	void UpdateRowFromOrder( ROWCOL row, CCSOrder* pCSOrder );
	void UpdateAllOrders();
	void UpdateOrderFromRow(ROWCOL row, CCSOrder* pCSOrder);
	void AddNewOrder(CCSOrder* pCSOrder);

	BOOL OnValidateCell(ROWCOL nrow, ROWCOL ncol);
	BOOL OnEndEditingCell(ROWCOL nrow, ROWCOL ncol);

// Construction
public:
	CMultipleOrderInsertDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultipleOrderInsertDlg)
	enum { IDD = IDD_DLG_MULTIPLE_ORDER_INSERT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	CMultipleOrderInsertGrid m_wndGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultipleOrderInsertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultipleOrderInsertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonSplit();
	afx_msg void OnButtonJoin();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonNewStock();
	afx_msg void OnButtonOrderInquiry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLEORDERINSERTDLG_H__2ADDE8B3_59E0_11D1_8054_006097B38214__INCLUDED_)
