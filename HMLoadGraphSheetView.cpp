// HMLoadGraphSheetView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "HMLoadGraphSheetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HMLoadGraphDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphSheetView
//  View for the HMLoad Sheet editor.
//
//  Display managed by CHMLoadSheetViewport
//  Events  managed by CHMLoadSheetController
//
//  Nothing special.
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CHMLoadGraphSheetView, CView)

CHMLoadGraphSheetView::CHMLoadGraphSheetView()
{
}

CHMLoadGraphSheetView::~CHMLoadGraphSheetView()
{
}


BEGIN_MESSAGE_MAP(CHMLoadGraphSheetView, CView)
	//{{AFX_MSG_MAP(CHMLoadGraphSheetView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphSheetView diagnostics

#ifdef _DEBUG
void CHMLoadGraphSheetView::AssertValid() const
{
	CView::AssertValid();
}

void CHMLoadGraphSheetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphSheetView message handlers



CHMLoadGraphDoc* CHMLoadGraphSheetView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHMLoadGraphDoc)));
	return (CHMLoadGraphDoc*)(m_pDocument);
}


void CHMLoadGraphSheetView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_VP.SetDocument(GetDocument());
	m_VP.OnInitialUpdate();
}


BOOL CHMLoadGraphSheetView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (WS_HSCROLL | WS_VSCROLL);	
	return CView::PreCreateWindow(cs);
}



void CHMLoadGraphSheetView::OnSize(UINT nType, int cx, int cy) 
{
	
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CView::OnSize(nType, cx, cy);
	
}


void CHMLoadGraphSheetView::OnDraw(CDC* pDC)
{
	m_VP.Draw(pDC);
}




int CHMLoadGraphSheetView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}



BOOL CHMLoadGraphSheetView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CHMLoadGraphSheetView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CView::OnWndMsg(message,wParam,lParam,pResult);
}


