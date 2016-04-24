// CastStringEditorStrandSummaryView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CastStringEditorStrandSummaryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastStringEditorDoc.h"



/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorStrandSummaryView
//  Strand summary view for the cast string editor.
//
//  Display managed by CCastStringEditorStrandSummaryViewport
//  Events  managed by CCastStringEditorStrandSummaryController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCastStringEditorStrandSummaryView, CFormView)

CCastStringEditorStrandSummaryView::CCastStringEditorStrandSummaryView()
	: CFormView(CCastStringEditorStrandSummaryView::IDD)
{
	//{{AFX_DATA_INIT(CCastStringEditorStrandSummaryView)
	m_showAvgWidth = FALSE;
	m_showCmsFlips = FALSE;
	m_showCurrentBackFuture = FALSE;
	m_showSlitsSplits = FALSE;
	m_showStock = FALSE;
	m_showHeatStrands = FALSE;
	//}}AFX_DATA_INIT

	m_showAvgWidth = FALSE;
	m_showCmsFlips = TRUE;
	m_showCurrentBackFuture = FALSE;
	m_showSlitsSplits = TRUE;
	m_showStock = FALSE;
	m_showHeatStrands = TRUE;
}

CCastStringEditorStrandSummaryView::~CCastStringEditorStrandSummaryView()
{
}

void CCastStringEditorStrandSummaryView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCastStringEditorStrandSummaryView)
	DDX_Control(pDX, IDC_LIST_SUMMARY, m_listSummary);
	DDX_Check(pDX, IDC_CHECK_AVG_WIDTH, m_showAvgWidth);
	DDX_Check(pDX, IDC_CHECK_CMS_FLIPS, m_showCmsFlips);
	DDX_Check(pDX, IDC_CHECK_CURRENT_BACK_FUTURE, m_showCurrentBackFuture);
	DDX_Check(pDX, IDC_CHECK_SLITS_SPLITS, m_showSlitsSplits);
	DDX_Check(pDX, IDC_CHECK_STOCK, m_showStock);
	DDX_Check(pDX, IDC_CHECK_HEAT_STRANDS, m_showHeatStrands);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCastStringEditorStrandSummaryView, CFormView)
	//{{AFX_MSG_MAP(CCastStringEditorStrandSummaryView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorStrandSummaryView diagnostics

#ifdef _DEBUG
void CCastStringEditorStrandSummaryView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCastStringEditorStrandSummaryView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorStrandSummaryView message handlers



CCastStringEditorDoc* CCastStringEditorStrandSummaryView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCastStringEditorDoc)));
	return  (CCastStringEditorDoc*)(m_pDocument);
}


int CCastStringEditorStrandSummaryView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());
	
	return 0;
}







void CCastStringEditorStrandSummaryView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_VP.SetSummaryListCtrl(&m_listSummary);
	m_VP.SetView(this);

	m_VP.OnInitialUpdate();	
	
}



//
//  Standard delegations to viewport
//

void CCastStringEditorStrandSummaryView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}


void CCastStringEditorStrandSummaryView::OnSize(UINT nType, int cx, int cy) 
{
	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CCastStringEditorStrandSummaryView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CCastStringEditorStrandSummaryView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}




