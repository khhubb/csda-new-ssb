// CastStringEditorScenMgrView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CastStringEditorScenMgrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastStringEditorDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorScenMgrView
//  ScenMgr View for the cast string editor.
//
//  Display managed by CCastStringEditorScenMgrViewport
//  Events  managed by CCastStringEditorScenMgrController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCastStringEditorScenMgrView, CFormView)

CCastStringEditorScenMgrView::CCastStringEditorScenMgrView()
	: CFormView(CCastStringEditorScenMgrView::IDD)
{
	//{{AFX_DATA_INIT(CCastStringEditorScenMgrView)
	m_casterNumMinus1 = -1;
	//}}AFX_DATA_INIT
}

CCastStringEditorScenMgrView::~CCastStringEditorScenMgrView()
{
}

void CCastStringEditorScenMgrView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCastStringEditorScenMgrView)
	DDX_Radio(pDX, IDC_RB_CASTER_1, m_casterNumMinus1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCastStringEditorScenMgrView, CFormView)
	//{{AFX_MSG_MAP(CCastStringEditorScenMgrView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorScenMgrView diagnostics

#ifdef _DEBUG
void CCastStringEditorScenMgrView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCastStringEditorScenMgrView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorScenMgrView message handlers


CCastStringEditorDoc* CCastStringEditorScenMgrView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCastStringEditorDoc)));
	return  (CCastStringEditorDoc*)(m_pDocument);
}



int CCastStringEditorScenMgrView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}





void CCastStringEditorScenMgrView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_VP.SetView(this);
	m_VP.OnInitialUpdate();	
	
}


//
//  Standard delegations to viewport
//

void CCastStringEditorScenMgrView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}




void CCastStringEditorScenMgrView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CCastStringEditorScenMgrView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CCastStringEditorScenMgrView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}