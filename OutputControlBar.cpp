// OutputControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "OutputControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "resource2.h"



/////////////////////////////////////////////////////////////////////////////
//	COutputControlBar
//
//  A control bar to be located at the bottom of the main frame.
//  Similar to the output window in Visual C++.
//
//  Has a tab window containing several COutputWnds.
//
//  Only one of these should be created, in the main thread.
//  See CMTOutputControlBar for how to access these windows from other threads.
//
//////////////////////////////////////////////////////////////////////////////



COutputControlBar::COutputControlBar()
{
}

COutputControlBar::~COutputControlBar()
{
}



IMPLEMENT_DYNAMIC(COutputControlBar, SECControlBar)


BEGIN_MESSAGE_MAP(COutputControlBar, SECControlBar)
	//{{AFX_MSG_MAP(COutputControlBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_CLEAR_WINDOW, OnClearWindow)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ACTIVATE_TAB,OnActivateTab)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COutputControlBar message handlers

int COutputControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BOOL rtn_val;
	rtn_val = m_wndTab.Create(this);
	m_wndLoad.Create(WS_CHILD 
					 | WS_VISIBLE 
					 | WS_VSCROLL
					 | WS_HSCROLL
					 | ES_MULTILINE 
					 | ES_AUTOVSCROLL 
					 | ES_WANTRETURN, 
					 CRect(0,0,0,0), 
					 &m_wndTab, 
					 3094);	
	m_wndLoad.LimitText(0);
	m_wndTab.AddTab(&m_wndLoad, _T("Load"));
	//m_wndTab.SetScrollStyle(0, WS_HSCROLL | WS_VSCROLL);
	m_wndTab.SetScrollStyle(0, 0);

	m_wndValidn.Create(WS_CHILD 
					  | WS_VISIBLE 
					  | WS_VSCROLL
					  | WS_HSCROLL
					  | ES_MULTILINE 
					  | ES_AUTOVSCROLL 
					  | ES_WANTRETURN, 
					  CRect(0,0,0,0), 
					  &m_wndTab, 
					  3095);	
	m_wndValidn.LimitText(0);
	m_wndTab.AddTab(&m_wndValidn, _T("Validn"));
	//m_wndTab.SetScrollStyle(1, WS_HSCROLL | WS_VSCROLL);
	m_wndTab.SetScrollStyle(1, 0);

	m_wndCheckpoint.Create(WS_CHILD 
					  | WS_VISIBLE 
					  | WS_VSCROLL
					  | WS_HSCROLL
					  | ES_MULTILINE 
					  | ES_AUTOVSCROLL 
					  | ES_WANTRETURN, 
					  CRect(0,0,0,0), 
					  &m_wndTab, 
					  3095);	
	m_wndCheckpoint.LimitText(0);
	m_wndTab.AddTab(&m_wndCheckpoint, _T("Checkpoint"));
	//m_wndTab.SetScrollStyle(2, WS_HSCROLL | WS_VSCROLL);
	m_wndTab.SetScrollStyle(2, 0);


	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);

	m_hwndLoad		= m_wndLoad.m_hWnd;
	m_hwndValidn	= m_wndValidn.m_hWnd;
	m_hwndCheckpoint= m_wndCheckpoint.m_hWnd;
	m_hwndTab		= m_wndTab.m_hWnd;

	m_pWndLoad		= &m_wndLoad;
	m_pWndValidn	= &m_wndValidn;
	m_pWndCheckpoint= &m_wndCheckpoint;
	m_pWndTab		= &m_wndTab;

	return 0;
}

BOOL COutputControlBar::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);

	// Fill background with APPWORKSPACE
	CBrush backBrush(GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	CRect rectInside;
	GetInsideRect(rectInside);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rectInside.top - rect.top, PATCOPY);
	pDC->PatBlt(rect.left, rectInside.bottom, rect.Width(), rect.bottom - rectInside.bottom, PATCOPY);
	pDC->PatBlt(rect.left, rect.top, rectInside.left - rect.left, rect.Height(), PATCOPY);
	pDC->PatBlt(rectInside.right, rect.top, rect.right - rectInside.right, rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

void COutputControlBar::OnSize(UINT nType, int cx, int cy) 
{
	CRect rectInside;
	GetInsideRect(rectInside);
	::SetWindowPos(m_hwndTab, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW);
	SECControlBar::OnSize(nType, cx, cy);
}

void COutputControlBar::OnExtendContextMenu(CMenu* pMenu)
{
	CString strMenu;
	VERIFY(strMenu.LoadString(ID_CLEAR_WINDOW));
	pMenu->AppendMenu(MF_SEPARATOR);
	pMenu->AppendMenu(MF_STRING, ID_CLEAR_WINDOW, strMenu);
}

void COutputControlBar::OnClearWindow()
{
	AfxMessageBox(_T("Insert your code here."));
}


CWnd& COutputControlBar::GetWnd(COutputControlBar::E_WndId id)
{
	CWnd* pWnd;

	switch ( id ) {
	case OCB_LOAD:
		pWnd = m_pWndLoad;
		break;
	case OCB_VALIDN:
		pWnd = m_pWndValidn;
		break;
	case OCB_CHECKPOINT:
		pWnd = m_pWndCheckpoint;
		break;
	default:
		assert(0);
	}

	return *pWnd;
}


HWND COutputControlBar::GetHWnd(COutputControlBar::E_WndId id)
{
	HWND hwnd;

	switch ( id ) {
	case OCB_LOAD:
		hwnd = m_hwndLoad;
		break;
	case OCB_VALIDN:
		hwnd = m_hwndValidn;
		break;
	case OCB_CHECKPOINT:
		hwnd = m_hwndCheckpoint;
		break;
	default:
		assert(0);
	}

	return hwnd;
}


void COutputControlBar::ShowWnd(COutputControlBar::E_WndId id)
{	
//	afxDump << "Implement COutputControlBar::ShowWnd!!!\n";
	m_pWndTab->ActivateTab(id);
	m_pWndTab->ScrollToTab(id);
}

// ## change return type to LRESULT
LRESULT COutputControlBar::OnActivateTab(WPARAM wParam, LPARAM lParam)
{
	ShowWnd( E_WndId(wParam) );
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CMTOutputControlBar

CMTOutputControlBar::CMTOutputControlBar()
{
}

CMTOutputControlBar::~CMTOutputControlBar()
{
}


BEGIN_MESSAGE_MAP(CMTOutputControlBar, COutputControlBar)
	//{{AFX_MSG_MAP(CMTOutputControlBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMTOutputControlBar message handlers


void CMTOutputControlBar::Attach(COutputControlBar &bar)
{
	// Note that we do not actually attach the CMTOutputControlBar itself.
	// We only need its children.

	m_hwndLoad			= bar.m_hwndLoad;
	m_hwndValidn		= bar.m_hwndValidn;
	m_hwndCheckpoint	= bar.m_hwndCheckpoint;
	m_hwndTab			= bar.m_hwndTab;

	// if we have a window already in this thread for a given hwnd,
	//   then we use it.
	// else we attach our window and use it.

	CWnd* pWnd;

	pWnd = CWnd::FromHandlePermanent(m_hwndLoad);
	if ( pWnd == 0 ) {
		m_wndLoad.Attach(m_hwndLoad);
		m_pWndLoad = &m_wndLoad;
	}
	else {
		ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)) );
		m_pWndLoad = (COutputWnd*)pWnd;
	}

	pWnd = CWnd::FromHandlePermanent(m_hwndValidn);
	if ( pWnd == 0 ) {
		m_wndValidn.Attach(m_hwndValidn);
		m_pWndValidn = &m_wndValidn;
	}
	else {
		ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)) );
		m_pWndValidn = (COutputWnd*)pWnd;
	}

	pWnd = CWnd::FromHandlePermanent(m_hwndCheckpoint);
	if ( pWnd == 0 ) {
		m_wndCheckpoint.Attach(m_hwndCheckpoint);
		m_pWndCheckpoint = &m_wndCheckpoint;
	}
	else {
		ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)) );
		m_pWndCheckpoint = (COutputWnd*)pWnd;
	}

	pWnd = CWnd::FromHandlePermanent(m_hwndTab);
	if ( pWnd == 0 ) {
		m_wndTab.Attach(m_hwndTab);
		m_pWndTab = &m_wndTab;
	}
	else {
		ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(SECTabWnd)) );
		m_pWndTab = (SECTabWnd*)pWnd;
	}
}


int CMTOutputControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	assert(0);

	if (COutputControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

