// HeatChemsGrid.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "HeatChemsGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "HeatDetailsDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CHeatChemsGrid

CHeatChemsGrid::CHeatChemsGrid()
{
}

CHeatChemsGrid::~CHeatChemsGrid()
{
}


BEGIN_MESSAGE_MAP(CHeatChemsGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CHeatChemsGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHeatChemsGrid message handlers

int CHeatChemsGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGXGridWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	if (GetParam() == NULL)
	{
		CString sText;
		GetWindowText(sText);

		if (!sText.IsEmpty())
		{
			// check if this is a layout resource
			CGXGridParam* pParam = CGXGridParam::CreateFromResource(sText);

			// pParam will be non-zero if layout could be successfully loaded
			if (pParam != NULL) {
				SetParam(pParam);
				GetParam()->SetPrintDevice(new CGXPrintDevice());
			}
		}

		Initialize();
	}

	return 0;
}



void CHeatChemsGrid::OnInitialUpdate()
{
	// if caption text is specified

	if (GetParam() == NULL)
	{
		CString sText;
		GetWindowText(sText);

		if (!sText.IsEmpty())
		{
			// check if this is a layout resource
			CGXGridParam* pParam = CGXGridParam::CreateFromResource(sText);

			// pParam will be non-zero if layout could be successfully loaded
			if (pParam != NULL) {
				SetParam(pParam);
				GetParam()->SetPrintDevice(new CGXPrintDevice());
			}
		}
	}

	CGXGridWnd::OnInitialUpdate();
}



BOOL CHeatChemsGrid::OnEndEditing(ROWCOL nrow, ROWCOL ncol)
{

	if ( m_pHDDlg != 0 )
		return m_pHDDlg->OnEndEditingChemCell(nrow,ncol);
	else
		return TRUE;
}
