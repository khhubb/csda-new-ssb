// MruOneScenDlgBar.cpp: implementation of the CMruOneScenDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "MruOneScenDlgBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "MessageBus.h"


//////////////////////////////////////////////////////////////////////
//	
//	CMruOneScenDlgBar
//
//  This dialog bar sits at the top of the display
//  and allows the user to select the current procLineScen,
//   current lineup, and current prodnScen, and to initiate a simulation.
//
//	Display managed by CMruOneScenViewport
//  Events  managed by CMruOneScenController
//
//////////////////////////////////////////////////////////////////////

CMruOneScenDlgBar::CMruOneScenDlgBar()
{
	m_pBus = &TheBus();
}

CMruOneScenDlgBar::~CMruOneScenDlgBar()
{

}




BEGIN_MESSAGE_MAP(CMruOneScenDlgBar, SECDialogBar)
	//{{AFX_MSG_MAP(CMruOneScenDlgBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCENARIOS, OnUpdateViewScenarios)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()



int CMruOneScenDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	// Create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetModel());

	return 0;
}



LRESULT CMruOneScenDlgBar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = SECDialogBar::HandleInitDialog(wParam, lParam);
	
	UpdateData(FALSE);

	//  Do the subclassing
	m_btnDlg.AttachButton(ID_VIEW_SCENARIOS,SECBitmapButton::Al_Center,IDB_MRU_DLG,this);

	//  A slightly different approach than most of the viewports
	//  Normally, just passed a pointer to the control.
	//  Experimented here with attaching.

	m_VP.SetCBCasterNum(IDC_COMBO_CASTER_NUM,this);
	m_VP.SetCBScenList(IDC_COMBO_SCENS,this);
	m_VP.SetCBStrings(IDC_COMBO_STRINGS,this);
	m_VP.OnInitialUpdate();

	return lRes;
}


//
//  Standard delegations to the viewport
//

void CMruOneScenDlgBar::OnSize(UINT nType, int cx, int cy) 
{	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	SECDialogBar::OnSize(nType, cx, cy);	
}


BOOL CMruOneScenDlgBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return SECDialogBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMruOneScenDlgBar::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return SECDialogBar::OnWndMsg(message,wParam,lParam,pResult);
}


//
// Without this here, I could not get the button to light up.
//

void CMruOneScenDlgBar::OnUpdateViewScenarios(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


