// NOutputWnd.h: interface for the NOutputWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOUTPUTWND_H__1B0305B8_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
#define AFX_NOUTPUTWND_H__1B0305B8_325B_11D3_85E7_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputWnd;
#include "OutputControlBar.h"

namespace NOutputWnd  
{
	void ShowMainOutputControlBar();
	void ShowMainLoadWnd(bool activateBar=true);
	void ShowMainValidnWnd(bool activateBar=true);
	void ShowMainCheckpointWnd(bool activateBar=true);

	void WriteProgress(long count);

	void ClearText(COutputControlBar::E_WndId id);
	void AddText(COutputControlBar::E_WndId id,LPCTSTR text);
	void AddText(COutputControlBar::E_WndId id,ostrstream& ostr);


	void PostLoadMsg(ostrstream& ostr);
	void PostLoadMsg(const CString& str);
	void PostLoadMsg(const char* str);
	void PostLoadMsg(const string str);


};

#endif // !defined(AFX_NOUTPUTWND_H__1B0305B8_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
