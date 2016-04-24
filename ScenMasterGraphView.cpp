// ScenMasterGraphView.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "ScenMasterGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "ScenMasterDoc.h"



/////////////////////////////////////////////////////////////////////////////
// CScenMasterGraphView
//
//  Graph view for the scen master.
//
//  Display managed by CScenMasterGraphViewport
//  Events  managed by CScenMasterGraphController
//
/////////////////////////////////////////////////////////////////////////////



IMPLEMENT_DYNCREATE(CScenMasterGraphView, SECPanView)

CScenMasterGraphView::CScenMasterGraphView()
{
	SetScrollSizes(MM_TEXT,CSize(0,0));
	// Turn on zooming, defaut is zoom off
	SetZoomMode(SEC_ZOOMNORMAL);
}

CScenMasterGraphView::~CScenMasterGraphView()
{
}


BEGIN_MESSAGE_MAP(CScenMasterGraphView, SECPanView)
	//{{AFX_MSG_MAP(CScenMasterGraphView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CScenMasterGraphView diagnostics

#ifdef _DEBUG
void CScenMasterGraphView::AssertValid() const
{
	SECPanView::AssertValid();
}

void CScenMasterGraphView::Dump(CDumpContext& dc) const
{
	SECPanView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScenMasterGraphView message handlers



CScenMasterDoc* CScenMasterGraphView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScenMasterDoc)));
	return  (CScenMasterDoc*)(m_pDocument);
}


int CScenMasterGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECPanView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}


void CScenMasterGraphView::OnInitialUpdate()
{
	SECPanView::OnInitialUpdate();

	m_VP.SetView(this);
	m_VP.OnInitialUpdate();	
}



//
//  Standard delegations to viewport
//

void CScenMasterGraphView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CScenMasterGraphView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	SECPanView::OnSize(nType, cx, cy);
}


BOOL CScenMasterGraphView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return SECPanView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CScenMasterGraphView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return SECPanView::OnWndMsg(message,wParam,lParam,pResult);
}





