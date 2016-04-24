#if !defined(AFX_HEATDETAILSGRID_H__923F7F42_7E17_11D1_805C_006097B38214__INCLUDED_)
#define AFX_HEATDETAILSGRID_H__923F7F42_7E17_11D1_805C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HeatDetailsGrid.h : header file
//


class CHeatDetailsDlg;

/////////////////////////////////////////////////////////////////////////////
// CHeatDetailsGrid window

class CHeatDetailsGrid : public CGXGridWnd
{
// Construction
public:
	CHeatDetailsGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatDetailsGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeatDetailsGrid();


	CHeatDetailsDlg* m_pHDDlg;

	virtual void OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol);
	virtual BOOL OnEndEditing(ROWCOL nrow, ROWCOL ncol);
	virtual BOOL OnValidateCell(ROWCOL nrow, ROWCOL ncol);
	virtual void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);

	virtual void OnInitialUpdate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeatDetailsGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATDETAILSGRID_H__923F7F42_7E17_11D1_805C_006097B38214__INCLUDED_)
