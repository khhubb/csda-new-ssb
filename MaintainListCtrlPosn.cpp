// MaintainListCtrlPosn.cpp: implementation of the CMaintainListCtrlPosn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

 
#include "csda.h"
#include "MaintainListCtrlPosn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//	CMaintainListCtrlPosn
//
//	Restore a CListCtrl's position after an operation that loses it,
//    such as clearing the control and adding back in updated items.
//
//  Typically stack-allocated, so d-tor restores position when the
//    object goes out-of-scope.
//
//	Usage:
//
//		CMaintainListCtrlPosn x(ctrl,true);
//
//
//////////////////////////////////////////////////////////////////////



CMaintainListCtrlPosn::CMaintainListCtrlPosn(CListCtrl& rCtrl,bool isAuto)
:	m_rListCtrl(rCtrl),
	m_bAuto(isAuto),
	m_bValid(true)
{
	// compute the indexes of the first and last visible items

	m_posn1 = m_rListCtrl.GetTopIndex();
	m_posn2 = m_posn1 + m_rListCtrl.GetCountPerPage() - 1;
}


CMaintainListCtrlPosn::~CMaintainListCtrlPosn()
{
	//  restore on destruction if automatic
	if ( m_bAuto )
		RestorePosition();
}


void CMaintainListCtrlPosn::RestorePosition()
{
	// m_bValid locks out multiple calls to this method

	if ( m_bValid ) {

		m_bValid = false;

		int listCount = m_rListCtrl.GetItemCount();

		int now1 = m_rListCtrl.GetTopIndex();

		//  try to make the stored top position at the top
		//  of the stored bottom position at the bottom.

		if ( m_posn1 < now1 )
			m_rListCtrl.EnsureVisible(m_posn1,false);
		else
			m_rListCtrl.EnsureVisible(min(m_posn2,listCount-1),false);
		
	}
}
