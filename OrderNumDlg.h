#if !defined(AFX_ORDERNUMDLG_H__C855BAF9_5DE8_11D1_8054_006097B38214__INCLUDED_)
#define AFX_ORDERNUMDLG_H__C855BAF9_5DE8_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OrderNumDlg.h : header file
//

class CSnapshot;
class COrder;

/////////////////////////////////////////////////////////////////////////////
// COrderNumDlg dialog

class COrderNumDlg : public CDialog
{
// Construction
public:
	COrderNumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderNumDlg)
	enum { IDD = IDD_DLG_ORDER_NUM };
	CString	m_orderNum;
	CString	m_caption;
	//}}AFX_DATA

	enum DlgType { TYPE_UNSPECIFIED, TYPE_ORDER_NUM, TYPE_LOT_SPEC };
	
	DlgType		m_type;
	CString		m_spec;
	COrder*		m_pOrder;
	int			m_casterNum;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderNumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderNumDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERNUMDLG_H__C855BAF9_5DE8_11D1_8054_006097B38214__INCLUDED_)
