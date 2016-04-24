// Notify.h: interface for the CNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFY_H__1C24B8A1_53DE_11D1_804F_006097B38214__INCLUDED_)
#define AFX_NOTIFY_H__1C24B8A1_53DE_11D1_804F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////
//
//  CNotify
//
//  Used to provide a callback.  See CDragSourceButton.
//
//////////////////////////////////////////////////////////////////////

class CNotify  
{
public:
	CNotify() {}
	virtual ~CNotify() {}

	virtual void Notify() = 0;

};

#endif // !defined(AFX_NOTIFY_H__1C24B8A1_53DE_11D1_804F_006097B38214__INCLUDED_)
