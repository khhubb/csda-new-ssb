// MaintainListCtrlPosn.h: interface for the CMaintainListCtrlPosn class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_MAINTAINLISTCTRLPOSN_H__08C1E7D5_C5D0_11D1_8070_006097B38214__INCLUDED_)
#define AFX_MAINTAINLISTCTRLPOSN_H__08C1E7D5_C5D0_11D1_8070_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMaintainListCtrlPosn  
{
public:
	CMaintainListCtrlPosn(CListCtrl& rCtrl,bool isAuto);
	~CMaintainListCtrlPosn();

	CListCtrl& m_rListCtrl;
		// the control whose position is being maintained

	bool m_bAuto;
		// if true, position will be automatically restored on destruction
		//   (when goes out of scope)
		// if false, user will have to call RestorePosition() explicitly.
			
	bool m_bValid;
		// flag to prevent multiple calls to RestorePosition.

	int m_posn1;
	int m_posn2;
		// indexes of first and last visible positions of control when constructed.
		// these are the positions we try to restore.


	void RestorePosition();

private:

	//  copy c-tor and assignment -- DO NOT IMPLEMENT

	CMaintainListCtrlPosn(const CMaintainListCtrlPosn&);
	bool operator=(const CMaintainListCtrlPosn&);
};

#endif // !defined(AFX_MAINTAINLISTCTRLPOSN_H__08C1E7D5_C5D0_11D1_8070_006097B38214__INCLUDED_)
