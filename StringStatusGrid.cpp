// StringStatusGrid.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "StringStatusGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "StringStatusDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CStringStatusGrid

CStringStatusGrid::CStringStatusGrid()
{
	m_pStringStatusDlg = 0;
}

CStringStatusGrid::~CStringStatusGrid()
{
}


BEGIN_MESSAGE_MAP(CStringStatusGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CStringStatusGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStringStatusGrid message handlers

int CStringStatusGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	// this is from another grid -- leave it here in case we need later.
	// take care of a little cell refresh problem we were having
	//m_bCacheCurrentCell = FALSE ;
	//m_bRefreshOnSetCurrentCell = TRUE;

	// allow text input which fits the choice list
	RegisterControl(GX_IDS_CTRL_TEXTFIT,
			  	    new CGXComboBox(this, 
							        GX_IDS_CTRL_TEXTFIT, 
									GX_IDS_CTRL_TEXTFIT+1, 
									GXCOMBO_TEXTFIT));

	return 0;
}



void CStringStatusGrid::OnInitialUpdate()
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

	// this is from another grid -- leave it here in case we need later.
	// take care of a little cell refresh problem we were having
	//m_bCacheCurrentCell = FALSE ;
	//m_bRefreshOnSetCurrentCell = TRUE;

	// allow text input which fits the choice list
	RegisterControl(GX_IDS_CTRL_TEXTFIT,
			  	    new CGXComboBox(this, 
							        GX_IDS_CTRL_TEXTFIT, 
									GX_IDS_CTRL_TEXTFIT+1, 
									GXCOMBO_TEXTFIT));

}




void CStringStatusGrid::OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol)
{
	if ( m_pStringStatusDlg != 0 )
		m_pStringStatusDlg->OnMovedCurrentCell(nrow,ncol);
}
