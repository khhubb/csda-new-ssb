// MruOneScenMVC.cpp: implementation of the CMruOneScenViewport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "MruOneScenMVC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "CasterScen.h"
#include "ScenMgr.h"
#include "ScenMru.h"
#include "CastString.h"
#include "SuperScen.h"
#include "MessageBus.h"



//////////////////////////////////////////////////////////////////////
//
//  CMruOneScenViewport
//
//  Manages display for CMruOneScenView
//  Associated with CMruOneScenController
//
//  Display has:
//     combo box for caster scenario type (= caster#)
//     combo box for scenarios of a given caster
//     combo box for strings of a given scenario
//
////////////////////////////////////////////////////////////////////////


//
//  Part of the 'attach' experiment
//  Detach before leaving
//

CMruOneScenViewport::~CMruOneScenViewport()
{
	if ( ::IsWindow(m_cbCasterNum.m_hWnd) )
		m_cbCasterNum.Detach();

	if ( ::IsWindow(m_cbScenList.m_hWnd) )
		m_cbScenList.Detach();

	if ( ::IsWindow(m_cbStrings.m_hWnd) )
		m_cbStrings.Detach();

}


BOOL CMruOneScenViewport::SetCBCasterNum(UINT nId, CWnd* pParent)
{	
	if ( ::IsWindow(m_cbCasterNum.m_hWnd) )
		m_cbCasterNum.Detach();

	return m_cbCasterNum.SubclassDlgItem(nId,pParent);
}


BOOL CMruOneScenViewport::SetCBScenList(UINT nId, CWnd* pParent)
{
	if ( ::IsWindow(m_cbScenList.m_hWnd) )
		m_cbScenList.Detach();

	return m_cbScenList.SubclassDlgItem(nId,pParent);
}


BOOL CMruOneScenViewport::SetCBStrings(UINT nId, CWnd* pParent)
{
	if ( ::IsWindow(m_cbStrings.m_hWnd) )
		m_cbStrings.Detach();

	return m_cbStrings.SubclassDlgItem(nId,pParent);
}


CMessageBus* CMruOneScenViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(m_pModel);
}


BOOL CMruOneScenViewport::OnCreate()
{
	return TRUE;
}


BOOL CMruOneScenViewport::CreateController()
{
	m_pCtlr = new CMruOneScenController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}

void CMruOneScenViewport::OnInitialUpdate()
{
	InitializeCasterNumBox();
	InitializeListBox();
	InitializeStringsBox();
}


void CMruOneScenViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_SMGR_CREATE_SUPER_SCEN:
	case MSG_ID_SMGR_DELETE_SUPER_SCEN:
		break;

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
		{
			CScenMgr::CCreateCasterScenMsg* pM
				= dynamic_cast<CScenMgr::CCreateCasterScenMsg*>(pMsg);
			if ( pM == 0 )
				break;
			if ( pM->CasterScen()->Caster() == CScenMru::GlobalScenMru.CasterNum() )
				UpdateListBox();
		}
		break;

	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
		{
			CScenMgr::CDeleteCasterScenMsg* pM
				= dynamic_cast<CScenMgr::CDeleteCasterScenMsg*>(pMsg);
			if ( pM == 0 )
				break;
			if ( pM->CasterScen()->Caster() == CScenMru::GlobalScenMru.CasterNum() )
				UpdateListBox();
		}
		break;


	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
		{
			CScenMru::CPromoteCasterScenMsg* pPMsg 
				= dynamic_cast<CScenMru::CPromoteCasterScenMsg*>(pMsg);
			if ( pPMsg == 0 )
				break;
			if ( pPMsg->CasterNum() == CScenMru::GlobalScenMru.CasterNum() ) {
			
				 UpdateListBox();
				 UpdateCasterNumBox();
				 UpdateStringsBox();
			}
		}
		break;

	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_CSI_CASTER_SCEN_INFO_CHANGED:
		UpdateCasterNumBox();
		UpdateListBox();
		UpdateStringsBox();
		break;

	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_CSE_STRING_INFO_CHANGED:
		UpdateStringsBox();
		break;

	}
}



void CMruOneScenViewport::InitializeCasterNumBox()
{
	m_cbCasterNum.ResetContent();

	m_cbCasterNum.AddString("#1 CC");
	m_cbCasterNum.AddString("#2 SC");
	m_cbCasterNum.AddString("#3 CC");

	SetCasterNumBoxSel();
}


void CMruOneScenViewport::UpdateCasterNumBox()
{
	SetCasterNumBoxSel();
}


void CMruOneScenViewport::SetCasterNumBoxSel()
{
	int num = CScenMru::GlobalScenMru.CasterNum();
	
	m_cbCasterNum.SetCurSel(num-1);
}


void CMruOneScenViewport::InitializeListBox()
{
	m_cbScenList.ResetContent();

	int casterNum = CScenMru::GlobalScenMru.CasterNum();

	ostrstream ostr;
	
	for ( vector<CCasterScen*>::iterator is = CScenMgr::CasterScensBegin(casterNum);
		  is != CScenMgr::CasterScensEnd(casterNum);
		  ++is ) {
		ostr << "#" << setw(6) << (*is)->Id()
			 << ", " << LPCTSTR((*is)->Comment())
			 << ends;

		m_cbScenList.AddString( ostr.str() );
		ostr.freeze(false);
		ostr.seekp(0);
	}

	SetListBoxSel();
}


void CMruOneScenViewport::SetListBoxSel()
{
	int casterNum = CScenMru::GlobalScenMru.CasterNum();
	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pScen == 0 ) 
		m_cbScenList.SetCurSel(-1);
	else
		m_cbScenList.SetCurSel( CScenMgr::CasterScenIndex(casterNum,pScen) );
}

void CMruOneScenViewport::UpdateListBox()
{
	InitializeListBox();
}


void CMruOneScenViewport::InitializeStringsBox()
{
	m_cbStrings.ResetContent();

	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pScen == 0 )
		return;

	ostrstream ostr;

	vector<CCastString*>& strings = pScen->CastStrings();
	
	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {
		ostr << (*is)->Id().Year()
			 << " "
			 << (*is)->Id().Week()
			 << " "
			 << (*is)->Id().StringId()
			 << ends;

		m_cbStrings.AddString( ostr.str() );
		ostr.freeze(false);
		ostr.seekp(0);
	}

	SetStringsBoxSel();
}


void CMruOneScenViewport::SetStringsBoxSel()
{
	int casterNum = CScenMru::GlobalScenMru.CasterNum();

	m_cbStrings.SetCurSel(CScenMru::GlobalScenMru.CurrentCastStringIndex(casterNum));
}


void CMruOneScenViewport::UpdateStringsBox()
{
	InitializeStringsBox();
}




BEGIN_MESSAGE_MAP(CMruOneScenController, MvcController)
	//{{AFX_MSG_MAP(CMruOneScenController)
	ON_BN_CLICKED(ID_VIEW_SCENARIOS, OnViewScenarios)
	ON_COMMAND(ID_VIEW_SCENARIOS, OnViewScenarios)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_CASTER_NUM,OnCasterNumChange)
	ON_CBN_SELCHANGE(IDC_COMBO_SCENS,OnScenChange)
	ON_CBN_SELCHANGE(IDC_COMBO_STRINGS,OnStringChange)
END_MESSAGE_MAP()




CMessageBus* CMruOneScenController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}

BOOL CMruOneScenController::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return MvcController::OnCmdMsg(nID,nCode,pExtra,pHandlerInfo);
}

void CMruOneScenController::OnViewScenarios()
{
	AfxMessageBox("CMruOneScenController::OnOpenMruDlg");
}

void CMruOneScenController::OnCasterNumChange()
{
	int index = GetViewport()->CBCasterNum()->GetCurSel();
	if ( index == -1 )
		return;

	CScenMru::GlobalScenMru.SetCasterNum(index+1);
}


void CMruOneScenController::OnScenChange()
{
	int index = GetViewport()->CBScenList()->GetCurSel();
	if ( index == -1 )
		return;

	int casterNum = CScenMru::GlobalScenMru.CasterNum();

	CCasterScen* pScen = CScenMgr::CasterScen(casterNum,index);

	CScenMru::GlobalScenMru.PromoteCasterScen(pScen);
}


void CMruOneScenController::OnStringChange()
{
	int index = GetViewport()->CBStrings()->GetCurSel();
	if ( index == -1 )
		return;

	CScenMru::GlobalScenMru.SetCurrentCastString(index);
}


