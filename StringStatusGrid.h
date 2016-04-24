#if !defined(AFX_STRINGSTATUSGRID_H__09E93CEB_B711_11D1_806D_006097B38214__INCLUDED_)
#define AFX_STRINGSTATUSGRID_H__09E93CEB_B711_11D1_806D_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StringStatusGrid.h : header file
//


class CStringStatusDlg;


/////////////////////////////////////////////////////////////////////////////
// CStringStatusGrid window

class CStringStatusGrid : public CGXGridWnd
{
// Construction
public:
	CStringStatusGrid();

// Attributes
public:
	
	CStringStatusDlg* m_pStringStatusDlg;

	virtual void OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringStatusGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStringStatusGrid();


	afx_msg	virtual void OnInitialUpdate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStringStatusGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGSTATUSGRID_H__09E93CEB_B711_11D1_806D_006097B38214__INCLUDED_)
