// ScenMgrView.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "ScenMgrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "ScenMgrDoc.h"
#include "MessageBus.h"



/////////////////////////////////////////////////////////////////////////////
// CScenMgrView
//  View for the scen mgr editor.
//
//  Display managed by CScenMgrViewport
//  Events  managed by CScenMgrController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CScenMgrView, CFormView)

CScenMgrView::CScenMgrView()
	: CFormView(CScenMgrView::IDD)
{
	//{{AFX_DATA_INIT(CScenMgrView)
	//}}AFX_DATA_INIT
}

CScenMgrView::~CScenMgrView()
{
}

void CScenMgrView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenMgrView)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScenMgrView, CFormView)
	//{{AFX_MSG_MAP(CScenMgrView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMgrView diagnostics

#ifdef _DEBUG
void CScenMgrView::AssertValid() const
{
	CFormView::AssertValid();
}

void CScenMgrView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScenMgrView message handlers


CScenMgrDoc* CScenMgrView::GetDocument()
{	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScenMgrDoc)));
	return  (CScenMgrDoc*)(m_pDocument);
}

int CScenMgrView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
} 




void CScenMgrView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();


	// Setup the headers in the lists

	BOOL subclassOk = m_treeScens.SubclassTreeCtrlId(IDC_TREE_SCENS,this);
	assert(subclassOk);

	m_VP.SetTreeCtrl(&m_treeScens);
	m_VP.OnInitialUpdate();	

}




//
//  Standard delegations to viewport
//

void CScenMgrView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CScenMgrView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CScenMgrView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CScenMgrView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}







