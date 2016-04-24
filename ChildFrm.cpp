// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"


#include "csda.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame
//
//  We use the Stingray WDI - workbook document interface.
//  See Stingray's documentation for more details.
//  Most important to this is that the main frame for the application
//    be an SECWorkbook and each CMDIChildFrame be an SECWorksheet.
//  We actually require each CMDIChildFrame to be a CChildFrame,
//    so that we can capture certain actions consistently.
//  Primary among these is preventing windows from closing.
//
////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CChildFrame, SECWorksheet)


BEGIN_MESSAGE_MAP(CChildFrame, SECWorksheet)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return SECWorksheet::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	SECWorksheet::AssertValid();
}	

void CChildFrame::Dump(CDumpContext& dc) const
{
	SECWorksheet::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	//  Catch the system close command and prevent it.
	if ( nID == SC_CLOSE ) {
		//afxDump << "Close!\n";
		return;
	}
	
	SECWorksheet::OnSysCommand(nID, lParam);
}
