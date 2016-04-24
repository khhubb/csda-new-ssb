// NOutputWnd.cpp: implementation of the NOutputWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "NOutputWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "OutputControlBar.h"
#include "MainFrm.h"
#include "OutputWnd.h"

// Everything here has to be guaranteed to work multi-threaded.
// This is MAJOR pain.



#include "resource2.h"



static COutputControlBar& MainOutputControlBar()
{
	return TheApp.MainFrame()->OutputBar();
}


static HWND GetMainOutputControlBarHwnd()
{
	return TheApp.MainFrame()->OutputBar().m_hWnd;
}

static HWND GetMainFrameHwnd()
{	
	return TheApp.MainFrame()->m_hWnd;
}



void NOutputWnd::ShowMainOutputControlBar()
{
	TheApp.MainFrame()->SendMessage(WM_USER_SHOW_OUTPUT_WINDOW);
}



static void ShowMainOutputTab(bool activateBar, COutputControlBar::E_WndId id)
{
	if ( activateBar )
		NOutputWnd::ShowMainOutputControlBar();

	MainOutputControlBar().SendMessage(WM_USER_ACTIVATE_TAB,id,0);

}


void NOutputWnd::ShowMainLoadWnd(bool activateBar)
{
	ShowMainOutputTab(activateBar,COutputControlBar::OCB_LOAD);
}


void NOutputWnd::ShowMainValidnWnd(bool activateBar)
{
	ShowMainOutputTab(activateBar,COutputControlBar::OCB_VALIDN);
}


void NOutputWnd::ShowMainCheckpointWnd(bool activateBar)
{
	ShowMainOutputTab(activateBar,COutputControlBar::OCB_CHECKPOINT);
}


void NOutputWnd::WriteProgress(long count)
{	
	if ( count % 100 != 0 )
		return;

	ostrstream ostr;


	if ( count % 500 == 0 )

		ostr << count;

	else 
		ostr << ".";

	if ( count % 10000 == 0 ) 
		ostr << "\n" ;

	ostr << ends;

	AddText(COutputControlBar::OCB_LOAD,ostr);

}



void NOutputWnd::ClearText(COutputControlBar::E_WndId id)
{
	HWND hwnd = MainOutputControlBar().GetHWnd(id);
	CWnd* pWnd = CWnd::FromHandlePermanent(hwnd);
	if ( pWnd == 0 ) {
		COutputWnd ownd;
		ownd.Attach(hwnd);
		ownd.ClearText();
		ownd.Detach();
	}
	else {
		ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)));
		((COutputWnd*)pWnd)->ClearText();
	}

}

void NOutputWnd::AddText(COutputControlBar::E_WndId id,LPCTSTR text)
{
	HWND hwnd = MainOutputControlBar().GetHWnd(id);
	CWnd* pWnd = CWnd::FromHandlePermanent(hwnd);
	if ( pWnd == 0 ) {
		COutputWnd ownd;
		ownd.Attach(hwnd);
		ownd.AddText(text);
		ownd.Detach();
	}
	else {
		ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)));
		((COutputWnd*)pWnd)->AddText(text);
	}
}

void NOutputWnd::AddText(COutputControlBar::E_WndId id,ostrstream& ostr)
{
	HWND hwnd = MainOutputControlBar().GetHWnd(id);
	CWnd* pWnd = CWnd::FromHandlePermanent(hwnd);
	if ( pWnd == 0 ) {
		COutputWnd ownd;
		ownd.Attach(hwnd);
		ownd.AddText(ostr);
		ownd.Detach();
	}
	else {
		ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(COutputWnd)));
		((COutputWnd*)pWnd)->AddText(ostr);
	}
}




// A little helper function.
// Displays the message in an output window on the screen


void NOutputWnd::PostLoadMsg(ostrstream& ostr)
{
	PostLoadMsg(ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);
}


void NOutputWnd::PostLoadMsg(const CString& str)
{
	PostLoadMsg(LPCTSTR(str));
}



void NOutputWnd::PostLoadMsg(const string str)
{	
	PostLoadMsg(str.data());
}



void NOutputWnd::PostLoadMsg(const char* str)
{
#ifdef _DEBUG
	afxDump << str;	
#endif
	NOutputWnd::AddText(COutputControlBar::OCB_LOAD,str);
}

