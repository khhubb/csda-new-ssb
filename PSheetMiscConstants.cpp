// PSheetMiscConstants.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "PSheetMiscConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MiscConstants.h"


/////////////////////////////////////////////////////////////////////////////
//	CPSheetMiscConstants
//
//	A property sheet editor designed to edit the CMiscConstants values.
//
//	At the moment, only one page in the property sheet.
//	I thought this might be bigger.
//
//	Usage:
//
//		CPSheetMiscConstants dlg("Edit constants",<CWnd* parent>);
//		dlg.SetMiscConstantsObject(&GlobalConstants);
//		dlg.DoModal();
//
//
//////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CPSheetMiscConstants, CPropertySheet)

CPSheetMiscConstants::CPSheetMiscConstants(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
	m_pConsts = 0;
}

CPSheetMiscConstants::CPSheetMiscConstants(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
	m_pConsts = 0;
}

CPSheetMiscConstants::~CPSheetMiscConstants()
{
}


BEGIN_MESSAGE_MAP(CPSheetMiscConstants, CPropertySheet)
	//{{AFX_MSG_MAP(CPSheetMiscConstants)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSheetMiscConstants message handlers


void CPSheetMiscConstants::AddPages()
{
	AddPage(&m_pSizesPage);

}


void CPSheetMiscConstants::SetMiscConstantsObject(CMiscConstants* pConsts)
{
	m_pConsts = pConsts;

	// set pointer to CMiscConstant on the embedded page
	m_pSizesPage.SetMiscConstantsObject(m_pConsts);
}
