// HMLoadGraphView.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "HMLoadGraphView.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HMLoadGraphDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphView
//  View for the HMLoad Gantt editor.
//
//  Display managed by CHMLoadGanttViewport
//  Events  managed by CHMLoadGanttController
//
//  Nothing special.
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CHMLoadGraphView, CGanttChart)


CHMLoadGraphView::CHMLoadGraphView()
{
}


CHMLoadGraphView::~CHMLoadGraphView()
{
}


BEGIN_MESSAGE_MAP(CHMLoadGraphView, CGanttChart)
	//{{AFX_MSG_MAP(CHMLoadGraphView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphView drawing


void CHMLoadGraphView::OnInitialUpdate()
{
	CGanttChart::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_VP.SetGanttDisplay(this->GanttDisplay());
	m_VP.SetDocument(GetDocument());
	m_VP.OnInitialUpdate();


}




/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphView diagnostics

#ifdef _DEBUG
void CHMLoadGraphView::AssertValid() const
{
	CGanttChart::AssertValid();
}

void CHMLoadGraphView::Dump(CDumpContext& dc) const
{
	CGanttChart::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphView message handlers


CHMLoadGraphDoc* CHMLoadGraphView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHMLoadGraphDoc)));
	return (CHMLoadGraphDoc*)(m_pDocument);
}




int CHMLoadGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGanttChart::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());

	return 0;
}




void CHMLoadGraphView::OnSize(UINT nType, int cx, int cy) 
{
	CGanttChart::OnSize(nType, cx, cy);
	
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CGanttChart::OnSize(nType, cx, cy);
	
}


BOOL CHMLoadGraphView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CGanttChart::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CHMLoadGraphView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CGanttChart::OnWndMsg(message,wParam,lParam,pResult);
}

