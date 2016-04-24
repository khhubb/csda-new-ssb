#if !defined(AFX_HEATCHEMSGRID_H__BA9FA46F_8F57_11D1_8065_006097B38214__INCLUDED_)
#define AFX_HEATCHEMSGRID_H__BA9FA46F_8F57_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HeatChemsGrid.h : header file
//


class CHeatDetailsDlg;

/////////////////////////////////////////////////////////////////////////////
// CHeatChemsGrid window

class CHeatChemsGrid : public CGXGridWnd
{
// Construction
public:
	CHeatChemsGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatChemsGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeatChemsGrid();


	CHeatDetailsDlg* m_pHDDlg;
	virtual BOOL OnEndEditing(ROWCOL nrow, ROWCOL ncol);
	virtual void OnInitialUpdate();



	// Generated message map functions
protected:
	//{{AFX_MSG(CHeatChemsGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATCHEMSGRID_H__BA9FA46F_8F57_11D1_8065_006097B38214__INCLUDED_)
