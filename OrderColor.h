// OrderColor.h: interface for the COrderColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERCOLOR_H__C913E566_E799_11D0_801B_006097B38214__INCLUDED_)
#define AFX_ORDERCOLOR_H__C913E566_E799_11D0_801B_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCSOrder;
class COrder;

//
//  Provides standard colors based on first two digits of condition code.
//

class COrderColor  
{
public:
	COrderColor();
	virtual ~COrderColor();

	static COLORREF colors[10][10];
	static COLORREF GetColor(CCSOrder* pOrder);
	static COLORREF GetColor(COrder* pOrder);
	static COLORREF GetColor(int condnCode);
	static COLORREF GetColor(int i, int j) { return colors[i][j]; }

};

#endif // !defined(AFX_ORDERCOLOR_H__C913E566_E799_11D0_801B_006097B38214__INCLUDED_)
