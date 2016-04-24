#if !defined(AFX_STRINGSTATUSDLG_H__09E93CEC_B711_11D1_806D_006097B38214__INCLUDED_)
#define AFX_STRINGSTATUSDLG_H__09E93CEC_B711_11D1_806D_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StringStatusDlg.h : header file
//

#include "StringStatusGrid.h"

class CCasterScen;
class CCastString;
class CCastStringHeat;
class CSuperScen;
class CLongOpn;

#include "CastString.h"
#include "Snapshot.h"


/////////////////////////////////////////////////////////////////////////////
// CStringStatusDlg dialog


class CSnapScenChangeRecord
{
public:

	CCastString::E_StringStatus m_snapStatus;	
	CCastString* m_pSnapString;
	CCastString* m_pScenString;
	UploadChangeType m_action;
	CString m_strStatus;
	CString m_strChoices;
	CString m_strAction;
	bool m_bEqual;

	CSnapScenChangeRecord( CCastString* pSnapString, CCastString* pScenString );
	CSnapScenChangeRecord();  // default c-tor needed for vector

	bool operator<(const CSnapScenChangeRecord& x) const { return this < &x; }
	bool operator==(const CSnapScenChangeRecord& x) const { return this == &x; }
};


class CStringStatusDlg : public CDialog
{
// Construction
public:
	CStringStatusDlg(CWnd* pParent = NULL);   // standard constructor

	enum COLUMNS {
		 COL_CASTER = 1,
		 COL_ID,
		 COL_TONS,
		 COL_HEATS,
		 COL_ORIG_ID,
		 COL_STATUS,
		 COL_ACTION,
		 COL_TAB_NUM,
		 COL_SPEC,
		 COL_CREATE_DATE
	};

	CStringStatusGrid m_wndGrid;

	CCasterScen* m_pCasterScen;
	CCasterScen* m_pSnapScen;
	CSuperScen*	 m_pSuperScen;

	vector<CSnapScenChangeRecord> m_changeRecords;

	int m_currentStringIndex;

	void AnalyzeScenario();

	void SetRows();
	void SetRow(int nRow, CSnapScenChangeRecord& record);
	
	afx_msg BOOL OnNcActivate(BOOL bActive);

	CCastString* GetCurrentCastString();

	void SetHeatListHeader();
	void SetHeatListItems();
	void AddItemForHeat(const CCastStringHeat& rHeat, int index);

	void OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol);

	void WriteUploadRecords(CLongOpn* pOpn);
	void FixHeatStatuses();
	void FixSnapScen();
	void FixOtherScens();
	void FixOtherScen( CCasterScen* pScen );
	void Archive910s();


// Dialog Data
	//{{AFX_DATA(CStringStatusDlg)
	enum { IDD = IDD_DLG_STRING_STATUS };
	CListCtrl	m_listHeats;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStringStatusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonHeatDetails();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGSTATUSDLG_H__09E93CEC_B711_11D1_806D_006097B38214__INCLUDED_)
