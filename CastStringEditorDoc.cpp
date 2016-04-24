// CastStringEditorDoc.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CastStringEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorDoc
//
//  Standard dummy document for MVC.
//
/////////////////////////////////////////////////////////////////////////////



IMPLEMENT_DYNCREATE(CCastStringEditorDoc, CDocument)

CCastStringEditorDoc::CCastStringEditorDoc()
{
}



CCastStringEditorDoc::~CCastStringEditorDoc()
{
}


BEGIN_MESSAGE_MAP(CCastStringEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CCastStringEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorDoc diagnostics

#ifdef _DEBUG
void CCastStringEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCastStringEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorDoc serialization


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorDoc commands


// we have to override the docbase's override
void CCastStringEditorDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CDocument::SetTitle(lpszTitle);
}




#if 0




void CCastStringEditorDoc::Init()
{
	UpdateTitle();
	ValidateOrderSelection();

	if ( GetCurrentCasterScen() != 0 )
		CScenMgr::BackupCasterScen(GetCurrentCasterScen());

	UpdateAllViews(0,INITIAL_UPDATE);
	UpdateAllViews(0,SCEN_SEL_CHANGED);
}






//  !!!!!!!!!!!!
//  We should NOT be calling BackupCasterScen directly from this class!!!!
//  !!!!!!!!!!!!


// totally bogus, this next one

void CCastStringEditorDoc::StandardUpdate(int casterNum, bool doBackup,CCastStringEditorDoc::UpdateReason r)
{
	ValidateOrderSelection();
	if ( doBackup)
		CScenMgr::BackupCasterScen(GetCurrentCasterScen(casterNum));
	UpdateTitle();
	UpdateAllViews(0,r);
}


void CCastStringEditorDoc::OnUpdate(IMvcSubject* pSubject,
									IMvcMessage* pMsg)
{
	switch ( pMsg->GetTypeID() )
	{	
	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
		{
			CScenMru::CPromoteCasterScenMsg* pM = dynamic_cast<CScenMru::CPromoteCasterScenMsg*>(pMsg);
			assert( pM != 0 );
			
			if ( pM->CasterNum() == GetCasterNum() ) {
				StandardUpdate(pM->CasterNum(),true,SCEN_SEL_CHANGED);
			}
			else {
				UpdateAllViews(0,SUPER_SCEN_CHANGED);
			}
		}
		break;

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
		{
			CScenMgr::CCreateCasterScenMsg* pM = dynamic_cast<CScenMgr::CCreateCasterScenMsg*>(pMsg);
			assert( pM != 0 );
			
			if ( pM->CasterScen()->Caster() == GetCasterNum() ) {
				StandardUpdate(pM->CasterScen()->Caster(),true,SCEN_SEL_CHANGED);
			}
			else {
				UpdateAllViews(0,SUPER_SCEN_CHANGED);
			}
		}
		break;

	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
		{
			CScenMgr::CCasterScenInfoChangedMsg* pM = dynamic_cast<CScenMgr::CCasterScenInfoChangedMsg*>(pMsg);
			assert( pM != 0 );
			
			if ( pM->CasterScen()->Caster() == GetCasterNum() ) {
				StandardUpdate(pM->CasterScen()->Caster(),true,SCEN_SEL_CHANGED);
			}
			else {
				UpdateAllViews(0,SUPER_SCEN_CHANGED);
			}
		}
		break;


	case MSG_ID_MRU_SELECT_CASTER:
		{
			CScenMru::CSelectCasterMsg* pM = dynamic_cast<CScenMru::CSelectCasterMsg*>(pMsg);
			assert( pM != 0 );

			StandardUpdate(pM->NewCasterNum(),true,CASTER_SEL_CHANGED);
		}
		break;

	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
		{
			CScenMgr::CDeleteCasterScenMsg* pM = dynamic_cast<CScenMgr::CDeleteCasterScenMsg*>(pMsg);
			assert( pM != 0 );

			if ( pM->CasterScen()->Caster() == GetCasterNum() ) {
				StandardUpdate(pM->CasterScen()->Caster(),false,SCEN_SEL_CHANGED);
			}
		}
		break;

	case MSG_ID_MRU_SELECT_CAST_STRING:
		StandardUpdate(0,false,STRING_SEL_CHANGED);
		break;


	case MSG_ID_SMGR_SAVE_CASTER_SCEN:
	case MSG_ID_SMGR_CREATE_PRODN_SCEN:
	case MSG_ID_SMGR_DELETE_PRODN_SCEN:
	case MSG_ID_SMGR_SAVE_PRODN_SCEN:
	case MSG_ID_SMGR_PRODN_SCEN_INFO_CHANGED:
	case MSG_ID_SMGR_CREATE_SUPER_SCEN:
	case MSG_ID_SMGR_DELETE_SUPER_SCEN:
	case MSG_ID_SMGR_SUPER_SCEN_INFO_CHANGED:
	case MSG_ID_SMGR_CREATE_MASTER_SCEN:
	case MSG_ID_SMGR_SNAPSHOT_LOADED:

	case MSG_ID_MRU_PROMOTE_PRODN_SCEN:

		// do nothing
		break;
	}
}



void CCastStringEditorDoc::ChangeCastString(int index)
{
	GetScenMru().SetCurrentCastString(index);
}



void CCastStringEditorDoc::SetCasterNum(int newCasterNum)
{
	if ( newCasterNum == GetCasterNum() )
		return;

	//  We better check to make sure this is okay
	
	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;

	GetScenMru().SetCasterNum(newCasterNum);
}





void CCastStringEditorDoc::DeleteCurrentString()
{
	// if not editing, do nothing.
	if ( ! IsEditing() )
		return;

	CCasterScen* pCScen = GetCurrentCasterScen();

	CCastString* pString = GetCurrentCastString();

	// if no string selected, do nothing.
	if ( pString == 0 )
		return;

	// Verify this is okay
	if ( IDYES != AfxMessageBox("Okay to delete this string?",MB_YESNO) )
		return;

	pCScen->DeleteString(pString);

	pCScen->SetModified(true);


	//  modify this to MVC-update

	// ScenMRU is index-based -- it will adjust automatically
	//    to the deletion of the string
	  
	ValidateOrderSelection();
	UpdateTitle();
	UpdateAllViews(0,STRING_SEL_CHANGED);
}



void CCastStringEditorDoc::DeleteCurrentCasterScen()
{
	//  if we are not editing, then nothing to delete anyway.
	if ( ! IsEditing() )
		return;

	if ( IDYES != AfxMessageBox("Are you sure you want to delete this scenario?",
								MB_ICONQUESTION|MB_YESNO) )
		return;

	CCasterScen* pScen = GetCurrentCasterScen();

	CScenMgr::DeleteCasterScen(pScen);

	// CScenMRU will be notified of the change
	//    and we will be selecting the next most recent
	// We will get notifcation
}





void CCastStringEditorDoc::CopyCurrentCasterScen()
{
	//  We may not be editing, but we can copy the current one anyway.
	//  We better check to make sure this is okay>>>

	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;

	//  However, there must already be a caster scen on the current caster.

	CCasterScen* pScen = GetCurrentCasterScen();

	if ( pScen == 0 ) 
		return;

	CCasterScen* pNewScen = CScenMgr::CopyCasterScen(pScen,true,false);

	GetScenMru().PromoteCasterScen(pNewScen);

	// we will be notified indirectly
}


void CCastStringEditorDoc::CreateCasterScen()
{
	//  We may not be editing, but we can copy the current one anyway.
	//  We better check to make sure this is okay>>>

	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;
		
	//  if we ever support multiple snapshots
	//		we'll have to do a dialog to select one

	CCasterScen* pNewScen = TheSnapshot.CopyInitialCasterScen(GetCasterNum());

	if ( pNewScen == 0 ) return;

	GetScenMru().PromoteCasterScen(pNewScen);

	// We will be notified indirectly
}






void CCastStringEditorDoc::OnEditHeatDetails() 
{

}

void CCastStringEditorDoc::OnUpdateEditHeatDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCurrentCastString() != 0);		
}



 

void CCastStringEditorDoc::ActivationChange(BOOL bActivate)
{
	if ( ! bActivate ) {
		// we are being deactivated
		// let's see if the user wants to save changes.

		CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(false);

	}
	else
		UpdateAllViews(0,FRAME_ACTIVATED);


}





#include "CastString.h"
#include "SuperScen.h"
#include "ScenMgr.h"
#include "CasterScen.h"
#include "Snapshot.h"
#include "MsgDlg.h"
#include "OrderInquiryDlg.h"
#include "CastStringIdDlg.h"
#include "OrderSelection.h"
#include "CasterScenArchiver.h"
#include "CSOrderParmDlg.h"
#include "StringStatusDlg.h"
#include "HeatLockDlg.h"
#include "CastStringValidator.h"
#include "PickCasterDlg.h"
#include "MessageBus.h"


// from the c-tor
	m_pOrderForInsert	= 0;
	TheBus().AddObserver(this);


// from the d-tor
	TheBus().RemoveObserver(this);


// from the message map



#endif // move

