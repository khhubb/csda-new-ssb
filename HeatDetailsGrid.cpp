// HeatDetailsGrid.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "HeatDetailsGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "HeatDetailsDlg.h"



/////////////////////////////////////////////////////////////////////////////
// CHeatDetailsGrid

CHeatDetailsGrid::CHeatDetailsGrid()
{
}

CHeatDetailsGrid::~CHeatDetailsGrid()
{
}


BEGIN_MESSAGE_MAP(CHeatDetailsGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CHeatDetailsGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHeatDetailsGrid message handlers

int CHeatDetailsGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	// take care of a little cell refresh problem we were having
	m_bCacheCurrentCell = FALSE ;
	m_bRefreshOnSetCurrentCell = TRUE;



	return 0;
}



void CHeatDetailsGrid::OnInitialUpdate()
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

	// take care of a little cell refresh problem we were having
	m_bCacheCurrentCell = FALSE ;
	m_bRefreshOnSetCurrentCell = TRUE;
}



BOOL CHeatDetailsGrid::OnEndEditing(ROWCOL nrow, ROWCOL ncol)
{

	if ( m_pHDDlg != 0 )
		return m_pHDDlg->OnEndEditingCell(nrow,ncol);
	else
		return TRUE;
}


BOOL CHeatDetailsGrid::OnValidateCell(ROWCOL nrow, ROWCOL ncol)
{

	if ( m_pHDDlg != 0 )
		return m_pHDDlg->OnValidateCell(nrow,ncol);
	else
		return TRUE;
}


void CHeatDetailsGrid::OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol)
{
	if ( m_pHDDlg != 0 )
		m_pHDDlg->OnMovedCurrentCell(nrow,ncol);
}


void CHeatDetailsGrid::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	if ( m_pHDDlg != 0 )
		m_pHDDlg->OnClickedButtonRowCol(nRow,nCol);
}