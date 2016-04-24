// CastStringEditorValidnView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CastStringEditorValidnView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastStringEditorDoc.h"



/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorValidnView
//  Validation View for the cast string editor.
//
//  Display managed by CCastStringEditorValidnViewport
//  Events  managed by CCastStringEditorValidnController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCastStringEditorValidnView, CFormView)

CCastStringEditorValidnView::CCastStringEditorValidnView()
	: CFormView(CCastStringEditorValidnView::IDD)
{
	//{{AFX_DATA_INIT(CCastStringEditorValidnView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCastStringEditorValidnView::~CCastStringEditorValidnView()
{
}

void CCastStringEditorValidnView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCastStringEditorValidnView)
	DDX_Control(pDX, IDC_LIST_RESULTS, m_listValidation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCastStringEditorValidnView, CFormView)
	//{{AFX_MSG_MAP(CCastStringEditorValidnView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorValidnView diagnostics

#ifdef _DEBUG
void CCastStringEditorValidnView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCastStringEditorValidnView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorValidnView message handlers


CCastStringEditorDoc* CCastStringEditorValidnView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCastStringEditorDoc)));
	return  (CCastStringEditorDoc*)(m_pDocument);
}

int CCastStringEditorValidnView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}






void CCastStringEditorValidnView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_VP.SetValidnListCtrl(&m_listValidation);

	m_VP.OnInitialUpdate();	
	
}



//
//  Standard delegations to viewport
//

void CCastStringEditorValidnView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CCastStringEditorValidnView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CCastStringEditorValidnView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CCastStringEditorValidnView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}




