#if !defined(AFX_PSHEETMISCCONSTANTS_H__642EE489_E2B3_11D0_8019_006097B38214__INCLUDED_)
#define AFX_PSHEETMISCCONSTANTS_H__642EE489_E2B3_11D0_8019_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PSheetMiscConstants.h : header file
//


class CMiscConstants;
#include "PpgHeatSizes.h"


/////////////////////////////////////////////////////////////////////////////
// CPSheetMiscConstants

class CPSheetMiscConstants : public CPropertySheet
{
	DECLARE_DYNAMIC(CPSheetMiscConstants)

// Construction
public:
	CPSheetMiscConstants(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPSheetMiscConstants(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSheetMiscConstants)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPSheetMiscConstants();

public:

		void SetMiscConstantsObject(CMiscConstants* pConsts);

private:

		void AddPages();
		CPpgHeatSizes m_pSizesPage;

		CMiscConstants* m_pConsts;
			// what is being edited.



	// Generated message map functions
protected:
	//{{AFX_MSG(CPSheetMiscConstants)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSHEETMISCCONSTANTS_H__642EE489_E2B3_11D0_8019_006097B38214__INCLUDED_)
