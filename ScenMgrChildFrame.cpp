// ScenMgrChildFrame.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "ScenMgrChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenMgrChildFrame





IMPLEMENT_DYNCREATE(CScenMgrChildFrame, CChildFrame) 

CScenMgrChildFrame::CScenMgrChildFrame()
{
}

CScenMgrChildFrame::~CScenMgrChildFrame()
{
}


BEGIN_MESSAGE_MAP(CScenMgrChildFrame, CChildFrame)
	//{{AFX_MSG_MAP(CScenMgrChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMgrChildFrame message handlers

int CScenMgrChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if ( CChildFrame::OnCreate(lpCreateStruct) == -1 )
		return -1;

		
	return 0;
}

void CScenMgrChildFrame::ActivateFrame(int nCmdShow) 
{
	CChildFrame::ActivateFrame(SW_SHOWMAXIMIZED);
}
