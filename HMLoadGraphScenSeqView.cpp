// HMLoadGraphScenSeqView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "HMLoadGraphScenSeqView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HMLoadGraphDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphScenSeqView
//  View for the HMLoad Scenario Sequence editor.
//
//  Display managed by CHMLoadScenSeqViewport
//  Events  managed by CHMLoadScenSeqController
//
//  Nothing special.
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CHMLoadGraphScenSeqView, CFormView)

CHMLoadGraphScenSeqView::CHMLoadGraphScenSeqView()
	: CFormView(CHMLoadGraphScenSeqView::IDD)
{
	//{{AFX_DATA_INIT(CHMLoadGraphScenSeqView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHMLoadGraphScenSeqView::~CHMLoadGraphScenSeqView()
{
}

void CHMLoadGraphScenSeqView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHMLoadGraphScenSeqView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHMLoadGraphScenSeqView, CFormView)
	//{{AFX_MSG_MAP(CHMLoadGraphScenSeqView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphScenSeqView diagnostics

#ifdef _DEBUG
void CHMLoadGraphScenSeqView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHMLoadGraphScenSeqView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphScenSeqView message handlers


CHMLoadGraphDoc* CHMLoadGraphScenSeqView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHMLoadGraphDoc)));
	return (CHMLoadGraphDoc*)(m_pDocument);
} 



void CHMLoadGraphScenSeqView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
		//  Set up the SECListBoxEditor for the group/sort field lists
	m_lbeStrings.Initialize(this,
							IDC_LIST_STRING_SEQ,
							LBE_DRAGNDROP|LBE_BUTTONS|LBE_UPBUTTON|LBE_DOWNBUTTON);
	m_lbeStrings.SetWindowText("Strings");

	m_VP.SetDocument(GetDocument());
	m_VP.SetLbeStrings(&m_lbeStrings);
	m_VP.OnInitialUpdate();
}


int CHMLoadGraphScenSeqView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	SetCaster(1);
	
	return 0;
}



//
//  Standard delegations to viewport
//

void CHMLoadGraphScenSeqView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CHMLoadGraphScenSeqView::OnSize(UINT nType, int cx, int cy) 
{
	
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
	
}





BOOL CHMLoadGraphScenSeqView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CHMLoadGraphScenSeqView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}


