#if !defined(AFX_PSHEETAVAILORDERS_H__17D9DC96_ACB6_11D0_9762_0000C0C2484C__INCLUDED_)
#define AFX_PSHEETAVAILORDERS_H__17D9DC96_ACB6_11D0_9762_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PSheetAvailOrders.h : header file
//

class COrderSelection;
class COrder;
class CCastStringEditorTextViewport;
class CSuperScen;

#include "PpgCICodeSelect.h"
#include "PpgOrderRange.h"
#include "PpgOrderChemRange.h"


/////////////////////////////////////////////////////////////////////////////
// CPSheetAvailOrders

class CPSheetAvailOrders : public CPropertySheet
{
	DECLARE_DYNAMIC(CPSheetAvailOrders)

// Construction
public:
	CPSheetAvailOrders(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPSheetAvailOrders(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

	COrderSelection* m_pOrderSelection;
		// the object being edited

	CCastStringEditorTextViewport* m_pCastStringEditorTextViewport;
		// backlink to the invoker, needed so we can do the Apply.

//////////// DM CHANGE: 2002-09-14: Added superScen member.
	CSuperScen* m_pSuperScen;
		// needed so we can update the order selection

	CPpgCICodeSelect	codes;
	CPpgOrderRange		ranges;
	CPpgOrderChemRange	chemRanges;
		// the pages

//////////// DM CHANGE: 2002-09-14: Added superScen parameter.
	
	void Init(COrderSelection* pSelect,
		      CSuperScen* pSS,
			  CCastStringEditorTextViewport* pViewport);
	void AddPages();

	afx_msg void OnApplyNow();
	afx_msg void OnOK();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSheetAvailOrders)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPSheetAvailOrders();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPSheetAvailOrders)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSHEETAVAILORDERS_H__17D9DC96_ACB6_11D0_9762_0000C0C2484C__INCLUDED_)
