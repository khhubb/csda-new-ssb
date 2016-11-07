#if !defined(AFX_CSORDERPARMDLG_H__534C5697_2922_11D1_802E_006097B38214__INCLUDED_)
#define AFX_CSORDERPARMDLG_H__534C5697_2922_11D1_802E_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CSOrderParmDlg.h : header file
//

#include "modelTypes.h"


class CCSOrder;
class COrder;
class CSuperScen;


/////////////////////////////////////////////////////////////////////////////
// CCSOrderParmDlg dialog

class CCSOrderParmDlg : public CDialog
{
// Construction
public:
	CCSOrderParmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCSOrderParmDlg)
	enum { IDD = IDD_DLG_STRING_ORDER_PARMS };
	CSpinButtonCtrl	m_spinDispCode;
	CSpinButtonCtrl	m_spinExpandedDispCode;
	CEdit	m_editCondnCode;
	CEdit	m_editSpec;
	CSpinButtonCtrl	m_spinCastWidth;
	CSpinButtonCtrl	m_spinCastLength;
	CSpinButtonCtrl	m_spinNumPieces;
	double	m_castLength;
	double	m_castWidth;  
	int		m_condnCode;
	int		m_numPieces;
	CString	m_operation;
	CString	m_orderNum;
	CString	m_orderedCastLength;
	CString	m_orderedCastWidth;
	CString	m_orderedCondnCode;
	CString	m_orderedNumPieces;
	CString	m_spec;
	CString	m_maxCastLength;
	CString	m_maxCastWidth;
	CString	m_minCastLength;
	CString	m_minCastWidth;
	CString	m_slitTypeCode;
	int		m_dispoCode;
	int     m_expandedDispCode;
	CString	m_changeIndic;
	CString	m_expCode;
	int		m_ciCode;
	CString	m_hbWidth;
	BOOL	m_useTransWidths;
	CString	m_maxCastWidthTrans;
	CString	m_minCastWidthTrans;
	double	m_planSteelLengthMin;
	double	m_planSteelLengthMax;
    double	m_provCastSlabWidthMin; // added these 2 fields 11-11-03 k. hubbard
    double	m_provCastSlabWidthMax;
	CString	m_hotrollUnitName;
	CString	m_stockReason;     // added following 3 reason fields 5-5-09 k. hubbard
	CString m_stockCommercialReason;
	CString	m_slitReason;
	//}}AFX_DATA



	CCSOrder*	m_pCSOrder;
	COrder*		m_pOrder;
	CSuperScen*	m_pSuperScen;
	int			m_casterNum;
	bool		m_bStock;
	bool		m_bCreate;

	Width m_precedingWidth;
//	Length m_planSteelLengthMin;
//	Length m_planSteelLengthMax;

	void InitOrderedFields();
	void InitCurrentFields();
	void InitMiscFields();

	bool XferToCSOrder( CCSOrder* pCSOrder );

	void UpdateWidthRelatedParms(int width);

// CBrush *m_editBkBrush;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSOrderParmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSOrderParmDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditCastWidth();
	afx_msg void OnDeltaposSpinCastWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClearChangeIndicator();
	afx_msg void OnCheckUseTransWidths();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSORDERPARMDLG_H__534C5697_2922_11D1_802E_006097B38214__INCLUDED_)
