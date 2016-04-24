#if !defined(AFX_MULTIPLEORDERINSERTGRID_H__2ADDE8B2_59E0_11D1_8054_006097B38214__INCLUDED_)
#define AFX_MULTIPLEORDERINSERTGRID_H__2ADDE8B2_59E0_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MultipleOrderInsertGrid.h : header file
//

class CMultipleOrderInsertDlg;

/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertGrid window

class CMultipleOrderInsertGrid : public CGXGridWnd
{
// Construction
public:
	CMultipleOrderInsertGrid();

// Attributes
public:

	CMultipleOrderInsertDlg* m_pMOIDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultipleOrderInsertGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMultipleOrderInsertGrid();

	virtual BOOL OnEndEditing(ROWCOL nrow, ROWCOL ncol);
	virtual BOOL OnValidateCell(ROWCOL nrow, ROWCOL ncol);


	virtual void OnInitialUpdate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMultipleOrderInsertGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLEORDERINSERTGRID_H__2ADDE8B2_59E0_11D1_8054_006097B38214__INCLUDED_)
