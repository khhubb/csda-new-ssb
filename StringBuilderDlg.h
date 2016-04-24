//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_STRINGBUILDERDLG_H__B686D113_68CC_11D3_85F4_00104B2D39AC__INCLUDED_)
#define AFX_STRINGBUILDERDLG_H__B686D113_68CC_11D3_85F4_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringBuilderDlg.h : header file
//


class CCasterScen;
class CSuperScen;


#include "CastStringId.h"
#include "OrderSelection.h"


/////////////////////////////////////////////////////////////////////////////
// CStringBuilderDlg dialog

class CStringBuilderDlg : public CDialog
{
// Construction
public:
	CStringBuilderDlg(CWnd* pParent = NULL);   // standard constructor
	~CStringBuilderDlg();

// Dialog Data
	//{{AFX_DATA(CStringBuilderDlg)
	enum { IDD = IDD_DLG_STRINGBUILDER };
	CButton	m_btnCreateString;
	CString	m_strId;
	int		m_numOrders;
//	CSBDriver	m_sbDriver;
	//}}AFX_DATA

	CCasterScen* m_casterScen[4];
		// the caster scenarios for caster=1,2,3 
		// used to make sure we don't get id overlap.

	CCasterScen* m_pScen;
		// the scenario in which we will create the string

	int m_caster;
		// the caster of m_pScen;

	CSuperScen* m_pSuperScen;
		// the context


	CCastStringId m_id;
		// the id selected for the new string.

	void SetControls();

	COrderSelection* m_pOrderSelection;
		// the order selection object used to select orders to send to the stringbuilder

	int m_heatSizeAim;
		// the aim heat size selected by the user.

	//static const CString& BaseInputTableName();
	//static const CString& BaseOutputTableName();
	//static CString InputTableName();
	//static CString OutputTableName();

	static const CString& BaseNewInputTableName();
	static const CString& BaseNewOutputTableName();
	static CString NewInputTableName();
	static CString NewOutputTableName();

	//static void CreateInputTable(CDatabase*);
	//static void ClearInputTable(CDatabase*);
	//static void CreateOutputTable(CDatabase*);
	//static void ClearOutputTable(CDatabase*);

	static void ClearNewInputTable(CDatabase*,int);
	static void ClearNewOutputTable(CDatabase*,int);
	static void ClearNewTable(CDatabase*,const CString& tableName, int userId);

	//static void CreateCopyTable(CDatabase*,
	//							const CString& copyName,
	//							const CString& srcName);
	//static void ClearTable(CDatabase*,const CString& tableName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringBuilderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStringBuilderDlg)
//	afx_msg void OnCancelSbdriver1();
//	afx_msg void OnDoneSbdriver1();
	afx_msg void OnButtonCreateString();
	afx_msg void OnButtonId();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonStartSb();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGBUILDERDLG_H__B686D113_68CC_11D3_85F4_00104B2D39AC__INCLUDED_)
