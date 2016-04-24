// ScenMasterCtrlView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "ScenMasterCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "ScenMasterDoc.h"



/////////////////////////////////////////////////////////////////////////////
// CScenMasterCtrlView
//  Control View for the scen master editor.
//
//  Display managed by CScenMasterCtrlViewport
//  Events  managed by CScenMasterCtrlController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CScenMasterCtrlView, CFormView)

CScenMasterCtrlView::CScenMasterCtrlView()
	: CFormView(CScenMasterCtrlView::IDD)
{
	//{{AFX_DATA_INIT(CScenMasterCtrlView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScenMasterCtrlView::~CScenMasterCtrlView()
{
}

void CScenMasterCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenMasterCtrlView)
	DDX_Control(pDX, IDC_LIST_SELECTED_SCENS, m_listSelScens);
	DDX_Control(pDX, IDC_LIST_AVAILABLE_SCENS, m_listAvailScens);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ONE, m_btnSelectOne);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ALL, m_btnSelectAll);
	DDX_Control(pDX, IDC_BUTTON_DESELECT_ONE, m_btnDeselectOne);
	DDX_Control(pDX, IDC_BUTTON_DESELECT_ALL, m_btnDeselectAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScenMasterCtrlView, CFormView)
	//{{AFX_MSG_MAP(CScenMasterCtrlView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMasterCtrlView diagnostics

#ifdef _DEBUG
void CScenMasterCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CScenMasterCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScenMasterCtrlView message handlers


CScenMasterDoc* CScenMasterCtrlView::GetDocument()
{	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScenMasterDoc)));
	return  (CScenMasterDoc*)(m_pDocument);
}

int CScenMasterCtrlView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}




void CScenMasterCtrlView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_VP.SetSelScensListCtrl(&m_listSelScens);
	m_VP.SetAvailScensListCtrl(&m_listAvailScens);

	m_VP.SetSelectOneBtn(&m_btnSelectOne);
	m_VP.SetSelectAllBtn(&m_btnSelectAll);
	m_VP.SetDeselectOneBtn(&m_btnDeselectOne);
	m_VP.SetDeselectAllBtn(&m_btnDeselectAll);

	m_VP.OnInitialUpdate();	
	
}


//
//  Standard delegations to viewport
//

void CScenMasterCtrlView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CScenMasterCtrlView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CScenMasterCtrlView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CScenMasterCtrlView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}
