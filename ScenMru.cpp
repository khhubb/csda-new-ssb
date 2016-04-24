// ScenMru.cpp: implementation of the CScenMru class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "ScenMru.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CasterScen.h"
#include "ProdnScen.h"
#include "ScenMgr.h"
#include "MessageBus.h"
#include "CasterScenArchiver.h"


//
// CScenMru == Scenario Most-Recently-Used
//
//	CScenMgr keeps track of all currently created scenarios.
//
//  CScenMru enhances this by also keeping track of the scenarios
//    on each caster (or the prodn scens) in order of most recently
//    used.  In addition, for each caster scenario, we keep track of 
//    the most recently accessed string.  There is also a notion of 
//	  of the "current caster".
//
//  The CScenMrus cache themselves on a static variable.
//
//  Needs to observe certain scenario changes in order to update.
//
//  CScenMru sends notifications itself for certain operations:
//		A scenario has been promoted.
//         the most-recent scenario is the one typically being acted on the editors.
//         the user switches to another scenario of the same type by 'promoting' it
//		   when a scenario is promoted, we must notify everyone so they can update their displays.
//		The currently selected caster has changed.
//		   the user might switch from caster 1 to caster 2.  
//         This also affects editors.
//		A new string within a caster scenario has been selected
//			again, editors must be notified.
//	


/////////////////////////////////////////////////////////
//
//   Creation, etc.
//
/////////////////////////////////////////////////////////


// Keeps track of all created CScenMru's.

// static
vector<CScenMru*> CScenMru::Mrus;


// We share a common CScenMru for most interfaces

// static
CScenMru CScenMru::GlobalScenMru;



CScenMru::CScenMru()
{
	// Register this on the list of all
	Mrus.push_back(this);

	// Bring this up-to-date on all currently existing scenarios
	CreateCasterScenEntries(1);
	CreateCasterScenEntries(2);
	CreateCasterScenEntries(3);
	CreateProdnScenEntries();

	//  Default to caster 1 selected
	m_casterNum		= 1;


	//  Synchronize the superScen
	m_pSuperScen	= 0;
	SetSuperScen();

	// observe future changes to scenarios
	TheBus().AddObserver(this);
}


CScenMru::~CScenMru()
{
	// Remove this from the list of all
	vector<CScenMru*>::iterator iv = find(Mrus.begin(),
										  Mrus.end(),
										  this);

	if ( iv != Mrus.end() )
		Mrus.erase(iv);

	// time to get off the bus
	// the only time we delete one of these is at system shutdown.
	// we were getting some race conditions on the deletions.
	// just don't do it.
	//TheBus().RemoveObserver(this);

}


//  Used during creation to synchronize with the existing CCasterScens and CProdnScens

void CScenMru::CreateCasterScenEntries(int casterNum)
{
	for ( vector<CCasterScen*>::const_iterator is = CScenMgr::CasterScensBegin(casterNum);
		  is != CScenMgr::CasterScensEnd(casterNum);
		  ++is )
		m_casterScens[casterNum].push_back( CCasterScenInfo((*is),0) );
}

void CScenMru::CreateProdnScenEntries()
{
	for ( vector<CProdnScen*>::const_iterator is = CScenMgr::ProdnScensBegin();
		  is != CScenMgr::ProdnScensEnd();
		  ++is )
		m_prodnScens.push_back( (*is) );
}





///////////////////////////////////////////////////////////////////////
//
//  Public accessors
//
///////////////////////////////////////////////////////////////////////

//
//  the vector of prodn scens is ordered from least to most recent.
//  the current prodn scen is the last in the vector.
//
CProdnScen* CScenMru::CurrentProdnScen() const
{
	if ( m_prodnScens.size() == 0 )
		return 0;
	else 
		return m_prodnScens.back();
}


//
//  The current CCasterScen is the current CCasterScen on the current caster
//
CCasterScen* CScenMru::CurrentCasterScen() const
{
	return CurrentCasterScen(m_casterNum);
}


//
//  The current CCasterScen on a given caster is the last one in the vector.
//  (vector sorted least recent to most recent).
//
CCasterScen* CScenMru::CurrentCasterScen(int casterNum) const
{
	if ( m_casterScens[casterNum].size() == 0 )
		return 0;
	else
		return m_casterScens[casterNum].back().CasterScen();
}


//
// The current CCastString is the current CCastString on the current caster
//
CCastString* CScenMru::CurrentCastString()
{
	return CurrentCastString(m_casterNum);
}


//
//  The current CCastString on a given caster is the CCastString recorded
//    on the current CCasterScen on that caster.
//  The current CastString is indicated by its CCastStringId.
//  It might be the case that the string has had its id changed 
//    and we weren't notified.  (Is this still true?)
//  We have version returning the string or the index.
//
CCastString* CScenMru::CurrentCastString(int casterNum)
{
	int index =	CurrentCastStringIndex(casterNum);

	if ( index == -1 ) 
		return 0;
	else
		return m_casterScens[casterNum].back().CasterScen()->CastStrings()[index];
}


//
//  Helper function to retrieve the index of the current cast string.
//  The Canonicalize method does the real work.
//
int CScenMru::CurrentCastStringIndex(int casterNum)
{
	if ( m_casterScens[casterNum].size() == 0 )
		return -1;
	else {
		CCasterScenInfo& info = m_casterScens[casterNum].back();

		int canonicalId = info.CanonicalizeStringId();

		return	canonicalId;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//  Notifications
//
//////////////////////////////////////////////////////////////////////////

void CScenMru::CPromoteCasterScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Promoting caster %d scen %d",
				  m_pCasterScen->Caster(),
				  m_pCasterScen->Id());
}

void CScenMru::CPromoteProdnScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Promoting prodn scen %d",
				  m_pProdnScen->Id());
}

void CScenMru::CSelectCasterMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Selecting caster %d",
				  m_newCasterNum);
}

void CScenMru::CSelectCastStringMsg::Sprint(CString& strCmd)
{
	ostrstream ostr;
	ostr << m_pCastString->Id().Year()
		 << "-"
		 << m_pCastString->Id().Week()
		 << "-"
		 << m_pCastString->Id().StringId()
		 << ends;

	strCmd.Format("Selecting caster %d scen %d string %s",
				  m_casterNum,
				  m_pCasterScen->Id(),
				  ostr.str());
	ostr.freeze(false);
}


//////////////////////////////////////////////////////////////////////////
//
//  Listening to scenario events from CScenMgr
//
//////////////////////////////////////////////////////////////////////////

//
//  As an observer, need to implement this
//  We need to update our lists depending on what happened.
//
void CScenMru::OnUpdate(IMvcSubject* pSubject, 
						IMvcMessage* pMsg)
{
	bool notifyPromote = false;
	bool isProdn = false;
	int casterNum = 0;
	
	switch ( pMsg->GetTypeID() )
	{	
	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
		{
			// A new caster scenario has been created.
			// Create an entry for it, and add it to the front (least-recently-used).
			// If this is the only one created, it is necessarily promoted.
			
			CScenMgr::CCreateCasterScenMsg* pM = dynamic_cast<CScenMgr::CCreateCasterScenMsg*>(pMsg);
			assert( pM != 0 );
			casterNum = pM->CasterScen()->Caster();
			m_casterScens[casterNum].insert(m_casterScens[casterNum].begin(),CCasterScenInfo(pM->CasterScen(),0));
			notifyPromote = m_casterScens[casterNum].size() == 1;
			isProdn = false;
		}
		break;
		
	case MSG_ID_SMGR_CREATE_PRODN_SCEN:
		{
			// A new prodn scenario has been created.
			// Create an entry for it, and add it to the front (least-recently-used).
			// If this is the only one created, it is necessarily promoted.
			
			CScenMgr::CCreateProdnScenMsg* pM = dynamic_cast<CScenMgr::CCreateProdnScenMsg*>(pMsg);
			assert( pM != 0 );
			
			m_prodnScens.insert(m_prodnScens.begin(),pM->ProdnScen());
			notifyPromote = m_prodnScens.size() == 1;
			isProdn = true;
		}
		break;
		
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
		{	
			// An existing caster scenario is being deleted.
			// We need to remove its entry.
			// If this scenario is currently the most-recent,
			//   then the next one will be promoted, so we need to notify everyone.
			
			CScenMgr::CDeleteCasterScenMsg* pM = dynamic_cast<CScenMgr::CDeleteCasterScenMsg*>(pMsg);
			assert( pM != 0 );
			
			casterNum = pM->CasterScen()->Caster();
			notifyPromote = CurrentCasterScen(casterNum) == pM->CasterScen();
			isProdn = false;
			RemoveCasterScen(pM->CasterScen());
		}
		break;
		
	case MSG_ID_SMGR_DELETE_PRODN_SCEN:
		{	
			// An existing prodn scenario is being deleted.
			// We need to remove its entry.
			// If this scenario is currently the most-recent,
			//   then the next one will be promoted, so we need to notify everyone.
			
			CScenMgr::CDeleteProdnScenMsg* pM = dynamic_cast<CScenMgr::CDeleteProdnScenMsg*>(pMsg);
			assert( pM != 0 );
			
			notifyPromote = CurrentProdnScen() == pM->ProdnScen();
			isProdn = true;
			RemoveProdnScen(pM->ProdnScen());
		}
		break;
		
		
	case MSG_ID_SMGR_CREATE_SUPER_SCEN:
		// do nothing
		break;
		
	case MSG_ID_SMGR_DELETE_SUPER_SCEN:
		{
			// we keep track of the current superscen that matches the current selected scenarios
			// if the CScenMgr wants to kill that super scen (perhaps at system shutdown)
			//    we want to make sure we don't have a pointer to it
			CScenMgr::CDeleteSuperScenMsg* pM = dynamic_cast<CScenMgr::CDeleteSuperScenMsg*>(pMsg);
			assert( pM != 0 );
			if ( m_pSuperScen == pM->SuperScen() )
				m_pSuperScen = 0;
		}
		break;
		
	default:
		// do nothing
		break;
	}
	
	//  After any of the changes, we want to synchronize the superScen
	SetSuperScen();
	
	//  Adding or deleting a scenario may cause a promotion, which will be noted above.
	//  if necessary, notify that a promotion has occured.
	
	if ( notifyPromote ) {
		if ( isProdn ) {
			CPromoteProdnScenMsg msg( m_prodnScens.size() == 0 ? 0 : m_prodnScens.back() );
			TheBus().UpdateAllObservers(this,&msg);
		}
		else {
			CPromoteCasterScenMsg msg( m_casterScens[casterNum].size() == 0 
				? 0 
				: m_casterScens[casterNum].back().CasterScen(),
				casterNum);
			TheBus().UpdateAllObservers(this,&msg);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//
//  Public operations 
//
//    These operations result in notifications to the outside world.
//
/////////////////////////////////////////////////////////////////////////////


//
//  We have a variety of operations we can perform on the casterScens
//     and a similar list for prodnScens.
//
//      Promote
//      SetString
//      ChangeScenInfo
//      ChangeStringInfo
//
//



//  
//  Change the currently selected caster
//

void CScenMru::SetCasterNum(int newVal) 
{ 
	assert( newVal == 1 || newVal == 2 || newVal == 3 );

	CSelectCasterMsg msg(newVal,m_casterNum);
	m_casterNum = newVal;

	TheBus().UpdateAllObservers(this,&msg);
}



//
//  Change the selected string for the currently selected caster's scenario,
//    the selected string can be indicated by either index or by the string itself.
//

void CScenMru::SetCurrentCastString(int index)
{
	SetCurrentCastString(m_casterNum,index);
}

void CScenMru::SetCurrentCastString(CCastString* pString)
{
	SetCurrentCastString(m_casterNum,pString);
}


//
// Change the selected string for the current scenaario on the given caster.
//    the selected string can be indicated by either index or by the string itself.
//

void CScenMru::SetCurrentCastString(int casterNum, int index)
{
	if ( m_casterScens[casterNum].size() == 0 )
		return;

	
	CCasterScenInfo& info = m_casterScens[casterNum].back();
	info.SetCurrentCastStringId(index);

	int newIndex = info.StringIndex();
	CCasterScen* pScen = info.CasterScen();
	CCastString* pString = pScen->CastStrings().size() <= newIndex ? 0 : pScen->CastStrings()[newIndex];
	CSelectCastStringMsg msg(casterNum,pScen,pString);
	TheBus().UpdateAllObservers(this,&msg);
}


void CScenMru::SetCurrentCastString(int casterNum, CCastString* pString)
{
	if ( m_casterScens[casterNum].size() == 0 )
		return;

	CCasterScenInfo& info = m_casterScens[casterNum].back();
	info.SetCurrentCastStringId(pString);

	CSelectCastStringMsg msg(casterNum,info.CasterScen(),pString);
	TheBus().UpdateAllObservers(this,&msg);
}




//
//  Promoting a scenario makes it the selected one, typically for editing purposes
//
//  The lists of scenarios are ordered by most-recent-last,
//    so the promoted one always goes on the end.
//

void CScenMru::PromoteProdnScen(CProdnScen* pScen)
{
	RemoveProdnScen(pScen);

	m_prodnScens.push_back(pScen);

	SetSuperScen();

	CPromoteProdnScenMsg msg(pScen);
	TheBus().UpdateAllObservers(this,&msg);
}


void CScenMru::PromoteCasterScen(CCasterScen* pScen)
{
	int casterNum = pScen->Caster();

	CCasterScenInfo info = RemoveCasterScen(pScen);

	m_casterScens[casterNum].push_back(info);

	SetSuperScen();

	CPromoteCasterScenMsg msg(pScen,casterNum);
	TheBus().UpdateAllObservers(this,&msg);
}




/////////////////////////////////////////////////////////////
//
//  Implementation details
//
/////////////////////////////////////////////////////////////


//
// Remove a prodn scen entry from the list
//

void CScenMru::RemoveProdnScen(CProdnScen* pScen)
{
	vector<CProdnScen*>::iterator is = find(m_prodnScens.begin(),
									       m_prodnScens.end(),
										   pScen);

	if ( is != m_prodnScens.end() )
		m_prodnScens.erase(is);

	SetSuperScen();
}


//
//  Remove a caster scen entry from its list
//  We return the info because we may just be deleting it during promotion,
//     so we need to preserve the selected string info
//

CScenMru::CCasterScenInfo CScenMru::RemoveCasterScen(CCasterScen* pScen)
{
	int casterNum = pScen->Caster();

	vector<CCasterScenInfo>::iterator is;

	for ( is = m_casterScens[casterNum].begin();
		  is != m_casterScens[casterNum].end();
		  ++ is) {

		if ( (*is).CasterScen() == pScen )
			break;

	}

	//  if we haven't found info, generate one

	CCasterScenInfo info 
		= (is == m_casterScens[casterNum].end()
		   ? CCasterScenInfo(pScen,-1)
		   : (*is));

	//  get rid of it, if we found it

	if ( is != m_casterScens[casterNum].end() )
		m_casterScens[casterNum].erase(is);

	SetSuperScen();

	return info;
}


//
//  Make sure we have a valid super scenario.
//	When we implement ProdnScens, we will need to add here.
//

void CScenMru::SetSuperScen()
{
	if ( CurrentCasterScen(1) != 0
		 &&
		 CurrentCasterScen(2) != 0
		 &&
		 CurrentCasterScen(3) != 0 )
		 m_pSuperScen = CScenMgr::SuperScen(CurrentCasterScen(1),
											CurrentCasterScen(2),
											CurrentCasterScen(3),
											CurrentProdnScen());
	else
		m_pSuperScen = 0;
}



//
//  The CCasterScenInfo is assigned to a specific caster scenario.
//  It tracks the id of the string selected by the user.
//  This can be changed (see above).
//  However, sometimes the id changes, or the string is deleted,
//    so we must deal with this.
//
//  Now, we use the string id.  This allows us to deal with changed to the id.
//  More importantly, we have to deal with new strings being added due to 910
//  updates, which would mess us up.  So we use the string id.  To canoncalize,
//  we find the first string with an id >= the selected id, and use its id.
//  We return the index of the string selected.



int CScenMru::CCasterScenInfo::CanonicalizeStringId()
{
	static CCastStringId emptyId;

	vector<CCastString*>& strings = m_pScen->CastStrings();

	// ## add cast to int
	int count = (int)strings.size();

	if ( count == 0 ) {
		m_stringId = emptyId;
		return -1;
	}

	for ( int i=0; i<count; ++i ) {
		if ( strings[i]->Id() == m_stringId )
			return i;
	}

	// didn't find it.

	for ( int i=0; i<count; ++i ) {
		if ( m_stringId < strings[i]->Id() ) {
			m_stringId = strings[i]->Id();
			return i;
		}
	}

	// didn't find anything bigger
	// use the last one.
	m_stringId = strings.back()->Id();
	return count-1;
}

	

void CScenMru::CCasterScenInfo::SetCurrentCastStringId(int index)
{
	vector<CCastString*>& strings = m_pScen->CastStrings();

	if ( 0 <= index && index < strings.size() )
		SetCurrentCastStringId( strings[index] );
	else
		SetCurrentCastStringId( (CCastString*) 0);
}


void CScenMru::CCasterScenInfo::SetCurrentCastStringId(CCastString* pString)
{
	vector<CCastString*>& strings = m_pScen->CastStrings();

	vector<CCastString*>::iterator is = find(strings.begin(),
											 strings.end(),
											 pString);
	if (  is == strings.end() ) {
		CCastStringId empty;
		m_stringId = empty;
	}
	else 
		m_stringId = pString->Id();
}



////////////////////////////////////////////////////////////////
//
//  A handy utility
//
////////////////////////////////////////////////////////////////

int CScenMru::MaybeCasterScenChangeOk(bool cancelOk)
{
	//  if we are not editing at the moment,
	//     then changing the scenario is fine.

	if ( CurrentSuperScen() == 0 )
		return IDYES;

	//  if there are no changes to the scenario being edited,
	//     then there is also no problem.

	if ( ! CurrentCasterScen()->IsModified() )
		return IDYES;

	//  Now comes the sticky point -- changes have been made
	//     to the scenario being edited.
	//  Present our options.

	int result = AfxMessageBox("Save changes to scenario?",
								MB_ICONQUESTION|(cancelOk ? MB_YESNOCANCEL : MB_YESNO));

	switch ( result ) {

	case IDYES:
		// save, then go ahead.
		CCasterScenArchiver::Archive(CurrentCasterScen(),true);
		CScenMgr::BackupCasterScen(CurrentCasterScen());
		return result;

	case IDNO:
		// user wants to discard changes, go ahead.
		{
			CCasterScen* pBackup = CScenMgr::RestoreCasterScenBackup(CurrentCasterScen());
			PromoteCasterScen(pBackup);
			return IDNO;
		}

	case IDCANCEL:
		// cancel the operation causing a change.
		return IDCANCEL;
	}

	return IDYES;
}

