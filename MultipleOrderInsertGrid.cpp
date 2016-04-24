// MultipleOrderInsertGrid.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "MultipleOrderInsertGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MultipleOrderInsertDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertGrid

CMultipleOrderInsertGrid::CMultipleOrderInsertGrid()
{
	m_pMOIDlg = 0;
}

CMultipleOrderInsertGrid::~CMultipleOrderInsertGrid()
{
}


BEGIN_MESSAGE_MAP(CMultipleOrderInsertGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CMultipleOrderInsertGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertGrid message handlers

int CMultipleOrderInsertGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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


void CMultipleOrderInsertGrid::OnInitialUpdate()
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



BOOL CMultipleOrderInsertGrid::OnEndEditing(ROWCOL nrow, ROWCOL ncol)
{

	if ( m_pMOIDlg != 0 )
		return m_pMOIDlg->OnEndEditingCell(nrow,ncol);
	else
		return TRUE;
}


BOOL CMultipleOrderInsertGrid::OnValidateCell(ROWCOL nrow, ROWCOL ncol)
{

	if ( m_pMOIDlg != 0 )
		return m_pMOIDlg->OnValidateCell(nrow,ncol);
	else
		return TRUE;
}