// PSheetAvailOrders.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "PSheetAvailOrders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "OrderSelection.h"
#include "CastStringEditorMvc.h"



/////////////////////////////////////////////////////////////////////////////
//	CPSheetAvailOrders
//
//  Edits a COrderSelection.
//  Property sheet with three pages.
//
//  Usage:
//
//		CPSheetAvailOrders dlg(<CString, a label> ,
//							   <CWnd* parent> );
//
//		dlg.Init(<COrderSelection* to be edited>,
//				 <CCastStringEditorTextViewport* to update, could be 0> );
//		dlg.DoModal();
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CPSheetAvailOrders, CPropertySheet)

CPSheetAvailOrders::CPSheetAvailOrders(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
	m_pOrderSelection = 0;
}



CPSheetAvailOrders::CPSheetAvailOrders(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages();
	m_pOrderSelection = 0;
}




CPSheetAvailOrders::~CPSheetAvailOrders()
{
}


BEGIN_MESSAGE_MAP(CPSheetAvailOrders, CPropertySheet)
	//{{AFX_MSG_MAP(CPSheetAvailOrders)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	// I don't know why ClassWizard doesn't support this.
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	ON_COMMAND(IDOK, OnOK)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSheetAvailOrders message handlers




void CPSheetAvailOrders::AddPages()
{
	AddPage(&ranges);
	AddPage(&chemRanges);
	AddPage(&codes);

	ranges.SetSheet(this);
}

//////////// DM CHANGE: 2002-09-14: Added superScen argument & init

void CPSheetAvailOrders::Init(COrderSelection* pSelect,
							  CSuperScen* pSS,
							  CCastStringEditorTextViewport* pViewport)
{
	m_pOrderSelection = pSelect;
	m_pCastStringEditorTextViewport = pViewport;
	m_pSuperScen = pSS;

	codes.SetOrderSelection(pSelect);
	ranges.SetOrderSelection(pSelect);
	chemRanges.SetOrderSelection(pSelect);
}


//
//  To respond to the Apply button,
//    move data from individal pages back to the order selection
//    (quit if invalid)
//    If orderSelection has been modified,, update the view
//	  Mark the pages as unmodified.
// 

void CPSheetAvailOrders::OnApplyNow()
{

	if ( codes.HasBeenActivated() ) {
		if ( ! codes.LocalToOrderSelection() )
			return;
	}

	if ( ranges.HasBeenActivated() ) {
		if ( ! ranges.LocalToOrderSelection() )
			return;
	}

	if ( chemRanges.HasBeenActivated() ) {
		if ( ! chemRanges.LocalToOrderSelection() ) 
			return;
	}

	if ( ! m_pOrderSelection->IsModified() )
		return;
	
//////////// DM CHANGE: 2002-09-14: Added call to init if no viewport
///////////  Needed for SSB use.

	if ( m_pCastStringEditorTextViewport != 0 )
		m_pCastStringEditorTextViewport->ReinitOrderSelection();
	else if ( m_pSuperScen != 0 )
		m_pOrderSelection->Init(m_pSuperScen);


	codes.SetModified(false);
	ranges.SetModified(false);
	chemRanges.SetModified(false);
	SendMessage(PSM_CANCELTOCLOSE);
}


void CPSheetAvailOrders::OnOK()
{
	OnApplyNow();

	EndDialog(IDOK);
}
