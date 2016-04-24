// OutputWnd.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "OutputWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputWnd
//
// A special CEdit designed for inclusion in a COutputControlBar.
//
//  Designed to be multiline, read-only,
//  with some helper methods to add text, etc.
//
/////////////////////////////////////////////////////////////////////////////

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}


IMPLEMENT_DYNAMIC(COutputWnd,CEdit)


BEGIN_MESSAGE_MAP(COutputWnd, CEdit)
	//{{AFX_MSG_MAP(COutputWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputWnd message handlers



BOOL COutputWnd::PreCreateWindow(CREATESTRUCT& cs) 
{

	cs.style |= (ES_MULTILINE | ES_READONLY | WS_VSCROLL )   ;
	
	return CEdit::PreCreateWindow(cs);
}




void COutputWnd::ClearText()
{
	SetWindowText("");
}


void COutputWnd::AddText(ostrstream& ostr)
{
	AddText(ostr.str());
	ostr.freeze(false);
}


void COutputWnd::AddText(LPCTSTR text)
{
	if ( m_hWnd == 0 )
		return;

	// What is an easy way to determine the length of text?
	// I don't know, but we can play some games with lines

	// kill any selection
	SetSel(-1,0);

	// find index of last line.
	int lastLine = GetLineCount()-1;
	int firstPos = LineIndex(lastLine);

	char buf[1000];
	int count = GetLine(lastLine,buf,1000);

	int lastPos = firstPos + count;
	SetSel(lastPos,lastPos);

	CString str(text);
	str.Replace("\n","\r\n");
	ReplaceSel(LPCTSTR(str));
	SetSel(-1,0);

	RemoveLinesIfNecessary();
}


void COutputWnd::RemoveLinesIfNecessary()
{
	int count = GetLineCount();

	if ( count > 2000 ) {
		int lastPos = LineIndex( count - 2000 );
		SetSel(0,lastPos);
		ReplaceSel("");
		SetSel(-1,0);
	}
}

