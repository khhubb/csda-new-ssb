// CastStringEditorGanttView.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CastStringEditorGanttView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "CastStringEditorDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttView
//  View for the Cast string editor Gantt display
//
//  Display managed by CCastStringEdtiorGanttViewport
//  Events  managed by CCastStringEdtiorGanttController
//
//  Nothing special.
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CCastStringEditorGanttView, CGanttChart)


CCastStringEditorGanttView::CCastStringEditorGanttView()
{
}


CCastStringEditorGanttView::~CCastStringEditorGanttView()
{
}


BEGIN_MESSAGE_MAP(CCastStringEditorGanttView, CGanttChart)
	//{{AFX_MSG_MAP(CCastStringEditorGanttView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttView drawing

void CCastStringEditorGanttView::OnInitialUpdate()
{
	CGanttChart::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_VP.SetGanttDisplay(this->GanttDisplay());
	m_VP.SetView(this);
	m_VP.OnInitialUpdate();

}




/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttView diagnostics

#ifdef _DEBUG
void CCastStringEditorGanttView::AssertValid() const
{
	CGanttChart::AssertValid();
}

void CCastStringEditorGanttView::Dump(CDumpContext& dc) const
{
	CGanttChart::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttView message handlers



CCastStringEditorDoc* CCastStringEditorGanttView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCastStringEditorDoc)));
	return  (CCastStringEditorDoc*)(m_pDocument);
}




int CCastStringEditorGanttView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGanttChart::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());

	return 0;
}




void CCastStringEditorGanttView::OnSize(UINT nType, int cx, int cy) 
{
	CGanttChart::OnSize(nType, cx, cy);
	
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CGanttChart::OnSize(nType, cx, cy);
	
}


BOOL CCastStringEditorGanttView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CGanttChart::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CCastStringEditorGanttView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CGanttChart::OnWndMsg(message,wParam,lParam,pResult);
}



void CCastStringEditorGanttView::ShowContextMenu(CPoint point)
{
	ClientToScreen(&point);

	CMenu menu;
	menu.LoadMenu(IDR_CASTER_SCEN_GANTT_CHART_VIEW_CONTEXT_MENU);
	CMenu* pContextMenu = menu.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
								 point.x, 
								 point.y, 
								 this);
}




