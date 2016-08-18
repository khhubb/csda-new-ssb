// ScenMgr.cpp: implementation of the CScenMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "ScenMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CasterScen.h"
#include "ProdnScen.h"
#include "SuperScen.h"
#include "Snapshot.h"
#include "CasterScenArchiver.h"
#include "ScenMru.h"


///////////////////////////////////////////////////////////////////////////////////
//
//  CScenMgr controls creation of scenarios and superScenarios.
//  It maintains lists of existing scens and superScens.
//
//  A scenario is either editable or not.
//  Non-editable scenarios are the initial scenarios created
//     during snapshot loading that represent the current daily plan.
//
//	A superScen is either for a snapshot or not.
//  A superScen for the snapshot is a placeholder to hold the initial scenarios.
//
//  It should be the case that the non-editable scenarios only appear
//  in the snapshot-only superscen
//
//  CScenMgr only tracks in its lists the scenarios that are editable,
//    and the superScens that are not snapshot-only.
//
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//
//  Notifications
//
///////////////////////////////////////////////////////////////////////


void CScenMgr::CCreateCasterScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Create caster scenario %d.",m_pCasterScen->Id());
}
 
void CScenMgr::CDeleteCasterScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Delete caster scenario %d.",m_pCasterScen->Id());
}
 
void CScenMgr::CCreateProdnScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Create prodn scenario %d.",m_pProdnScen->Id());
}
 
void CScenMgr::CDeleteProdnScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Delete prodn scenario %d.",m_pProdnScen->Id());
}


void CScenMgr::CCreateSuperScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Create superScen %s.",LPCTSTR(m_pSuperScen->IdString()));
}
 
void CScenMgr::CDeleteSuperScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Delete superScen %s.",LPCTSTR(m_pSuperScen->IdString()));
}


void CScenMgr::CSaveCasterScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Save caster scen %d.",m_pCasterScen->Id());
}


void CScenMgr::CSaveProdnScenMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Save prodn scen %d.",m_pProdnScen->Id());
}


void CScenMgr::CCasterScenInfoChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Caster scen %d info changed",m_pCasterScen->Id());
}

void CScenMgr::CProdnScenInfoChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Prodn scen %d info changed",m_pProdnScen->Id());
}


void CScenMgr::CSuperScenInfoChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Superscen info %s changed",LPCTSTR(m_pSuperScen->IdString()));
}




////////////////////////////////////////////////////////////////
//	
//	Static member variable definitions
//	
////////////////////////////////////////////////////////////////

TCasterScenVec	CScenMgr::m_casterScens[Caster::CasterArrayLen];
TProdnScenVec	CScenMgr::m_prodnScens;
TSuperScenVec	CScenMgr::m_superScens;

CArchiveQueue	CScenMgr::m_archiveQueue;



////////////////////////////////////////////////////////////////
//	
//	Basic operations requiring notification to observers
//	
////////////////////////////////////////////////////////////////

//
//  Create a caster scenario.
//  If editable, assign an id, add it to the vector and notify.
//

CCasterScen* CScenMgr::CreateCasterScen(int casterNum, //### caster-specific
										bool canEdit)
{
  CCasterScen* pScen = new CCasterScen(casterNum); //### caster-specific

	pScen->SetCanEdit(canEdit);

	if ( canEdit ) {
		CCasterScenArchiver::AssignCasterScenId(pScen);
		m_casterScens[casterNum].push_back(pScen); //### caster-specific
		CCreateCasterScenMsg msg(pScen);
		TheBus().UpdateAllObservers(0,&msg);
	}
	
	return pScen;
}


//
//  Create a caster scenario, id is given.
//  Used by archival code.
//

CCasterScen* CScenMgr::CreateCasterScen(int casterNum, //### caster-specific
										long id)
{
	assert( CasterScenById(casterNum,id) == 0 ); //### caster-specific

	CCasterScen* pScen = new CCasterScen(casterNum); //### caster-specific

	pScen->SetCanEdit(true);
	pScen->SetId(id);

	m_casterScens[casterNum].push_back(pScen); //### caster-specific

	CCreateCasterScenMsg msg(pScen);
	TheBus().UpdateAllObservers(0,&msg);

	return pScen;
}



//
//  Create a copy of a caster scen
//  

CCasterScen* CScenMgr::CopyCasterScen(CCasterScen* pBase, bool canEdit,bool isSnapScen)
{
	CCasterScen* pNewScen =	new CCasterScen(*pBase,isSnapScen); // copy c-tor

	pNewScen->SetCanEdit(canEdit);

	if ( canEdit ) {
		CCasterScenArchiver::AssignCasterScenId(pNewScen);
		m_casterScens[pNewScen->m_caster].push_back(pNewScen);

		CCreateCasterScenMsg msg(pNewScen);
		TheBus().UpdateAllObservers(0,&msg);
	}

	return pNewScen;
}




//
//  Create a copy of caster scen, using all the 910s but only selected 909/new.
//

CCasterScen* CScenMgr::CopyCasterScen(CCasterScen* pBase, 
									  bool canEdit,
									  vector<int>& indexes909)
{
	CCasterScen* pNewScen =	new CCasterScen(*pBase,indexes909); // copy c-tor, modified

	pNewScen->SetCanEdit(canEdit);

	if ( canEdit ) {
		CCasterScenArchiver::AssignCasterScenId(pNewScen);
		m_casterScens[pNewScen->m_caster].push_back(pNewScen);

		CCreateCasterScenMsg msg(pNewScen);
		TheBus().UpdateAllObservers(0,&msg);
	}

	return pNewScen;
}




//
//  Deleting a caster scenario requires deleting any super scen containing it,
//    plus dealing with notifications
//

void CScenMgr::DeleteCasterScen(CCasterScen* pScen)
{
	// it can happen that the scenario is null.
	// In particular, when you Cancel out of the initial login screen ...

	if ( pScen == 0 )
		return;

	DeleteContainingSuperScens(pScen);


	int caster = pScen->m_caster; //### caster-specific

	assert(Caster::IsValidCasterValue(caster)); //### caster-specific

	TCasterScenVec::iterator is = find(CasterScensBegin(caster), //### caster-specific
									  CasterScensEnd(caster), //### caster-specific
									  pScen);


	
	if ( is != CasterScensEnd(caster) ) //### caster-specific
		m_casterScens[caster].erase(is); //### caster-specific

	// we should notify before we d-tor this object
	CDeleteCasterScenMsg msg(pScen);
	TheBus().UpdateAllObservers(0,&msg);

	delete pScen;
}


//
//  We don't do much with prodn scens yet.
//

CProdnScen* CScenMgr::CreateProdnScen(bool canEdit)
{
	CProdnScen* pScen = new CProdnScen;

	pScen->SetCanEdit(canEdit);

	if ( canEdit ) {
		m_prodnScens.push_back(pScen);

		CCreateProdnScenMsg msg(pScen);
		TheBus().UpdateAllObservers(0,&msg);

	}

	return pScen;
}




//
//  Delete a prodn scen.
//  Must delete any containing super scens.
//  Must notify.
//

void CScenMgr::DeleteProdnScen(CProdnScen* pScen)
{
	DeleteContainingSuperScens(pScen);

	TProdnScenVec::iterator is = find(ProdnScensBegin(),
									 ProdnScensEnd(),
									 pScen);

	if ( is != ProdnScensEnd() )
		m_prodnScens.erase(is);


	//  must notify before d-tor-ing this object
	CDeleteProdnScenMsg msg(pScen);
	TheBus().UpdateAllObservers(0,&msg);

	delete pScen;
}






//
//  Create a superScen for the given combination of scenarios.
//  A Superscen must have all the caster scens in it from the same snapshot.
//  (We may never have multiple snapshots, so who knows?).
//

CSuperScen* CScenMgr::CreateSuperScen(CCasterScen* pCScen1,
									  CCasterScen* pCScen2,
									  CCasterScen* pCScen3,
									  CCasterScen* pCScen4,
									  CCasterScen* pCScen5,
									  CProdnScen*  pProdnScen,
									  bool forSnap /* = false */)
{
	CSuperScen* pSS = new CSuperScen;

        //### caster-specific
	pSS->m_pCScens[Caster::C1] = pCScen1;
	pSS->m_pCScens[Caster::C2] = pCScen2;
	pSS->m_pCScens[Caster::C3] = pCScen3;
	pSS->m_pCScens[Caster::C4] = pCScen4;
	pSS->m_pCScens[Caster::C5] = pCScen5;
	pSS->m_pProdnScen = pProdnScen;

	pSS->ForSnap(forSnap);
	
	if ( ! forSnap) {
		m_superScens.push_back(pSS);

		CCreateSuperScenMsg msg(pSS);
		TheBus().UpdateAllObservers(0,&msg);
	}

#if 0
	if ( pCScen1 != 0 && pCScen2 != 0 && pCScen3 != 0 )

		afxDump << "Creating super scen: \n"
				<< " 1. " << pCScen1->Id()
				<< ", " << (pCScen1->CanEdit() ? "can edit" : "not edit")
				<< ", " << pCScen1->Owner()
				<< ", " << pCScen1->Comment()
				<< "\n"
				<< " 2. " << pCScen2->Id()
				<< ", " << (pCScen2->CanEdit() ? "can edit" : "not edit")
				<< ", " << pCScen2->Owner()
				<< ", " << pCScen2->Comment()
				<< "\n"
				<< " 3. " << pCScen3->Id()
				<< ", " << (pCScen3->CanEdit() ? "can edit" : "not edit")
				<< ", " << pCScen3->Owner()
				<< ", " << pCScen3->Comment()
				<< "\n\n";

	else
		afxDump << "Creating initially empty super scen.\n\n";
#endif

	return pSS;
}



void CScenMgr::DeleteSuperScen(CSuperScen* pSS)
{
	TSuperScenVec::iterator is = find(SuperScensBegin(),
									 SuperScensEnd(),
									 pSS);

	if ( is != SuperScensEnd() )
		m_superScens.erase(is);

	// send notification before d-tor on the object
	CDeleteSuperScenMsg msg(pSS);
	TheBus().UpdateAllObservers(0,&msg);

	delete pSS;
}




///////////////////////////////////////////////////////
//
//  Informational
//
///////////////////////////////////////////////////////

//
// Retrieve a caster scenario based on caster and its position in the vector
//

CCasterScen* CScenMgr::CasterScen(int casterNum, int index) //### caster-specific
{
	assert ( Caster::IsValidCasterValue(casterNum) ); //### caster-specific

	assert ( 0 <= index && index < CasterScensCount(casterNum) ); //### caster-specific

	return m_casterScens[casterNum][index]; //### caster-specific
}




//
//  Find the index of a caster scen in the vector
//

int CScenMgr::CasterScenIndex(int casterNum, CCasterScen* pScen) //### caster-specific
{
	TCasterScenVec::const_iterator ib = CScenMgr::CasterScensBegin(casterNum); //### caster-specific
	TCasterScenVec::const_iterator ie = CScenMgr::CasterScensEnd(casterNum);   //### caster-specific

	TCasterScenVec::const_iterator is = find(ib,ie,pScen);
	
	if ( is == ie )
		return -1;
	else 
		return is - ib;
}


//
//  Find a caster scenario from its id
//

CCasterScen* CScenMgr::CasterScenById(int casterNum, long id) //### caster-specific
{
	for ( TCasterScenVec::const_iterator is = CasterScensBegin(casterNum); //### caster-specific
		  is != CasterScensEnd(casterNum); //### caster-specific
		  ++is ) {
	
		if ( (*is)->Id() == id )
			return (*is);
	}

	return 0;
}



//
//  Find the index of a prodn scen in the vector
//

int CScenMgr::ProdnScenIndex(CProdnScen* pScen)
{
	TProdnScenVec::const_iterator ib = CScenMgr::ProdnScensBegin();
	TProdnScenVec::const_iterator ie = CScenMgr::ProdnScensEnd();

	TProdnScenVec::const_iterator is = find(ib,ie,pScen);
	
	if ( is == ie )
		return -1;
	else
		return is - ib;
}


//
//  Look for a superScen already created for
//   this particular combination of scenarios.
//  or create one if necessary.
//

CSuperScen* CScenMgr::SuperScen(CCasterScen* pCScen1,
								   CCasterScen* pCScen2,
								   CCasterScen* pCScen3,
								   CCasterScen* pCScen4,
								   CCasterScen* pCScen5,
								   CProdnScen*  pProdnScen)
{
	CSuperScen* pSS = FindSuperScen(pCScen1,
								   pCScen2,
								   pCScen3,
								   pCScen4,
								   pCScen5,
								   pProdnScen);

	if ( pSS == 0 )
		return CreateSuperScen(pCScen1,
							   pCScen2,
							   pCScen3,
							   pCScen4,
							   pCScen5,
							   pProdnScen);
	else
		return pSS;
}


//
//  Find a superScen for this particular combination of scenarios,
//     return 0 if not found.
//

CSuperScen* CScenMgr::FindSuperScen(CCasterScen* pCScen1,
								    CCasterScen* pCScen2,
								    CCasterScen* pCScen3,
									CCasterScen* pCScen4,
									CCasterScen* pCScen5,
									CProdnScen*  pProdnScen)
{
	for ( TSuperScenVec::iterator is = SuperScensBegin();
		  is != SuperScensEnd();
		  ++is ) {
		//### caster-specific
		if ((*is)->m_pCScens[Caster::C1] == pCScen1
			&&
			(*is)->m_pCScens[Caster::C2] == pCScen2
			&&
			(*is)->m_pCScens[Caster::C3] == pCScen3
			&&
			(*is)->m_pCScens[Caster::C4] == pCScen4
			&&
			(*is)->m_pCScens[Caster::C5] == pCScen5)
			//&&
			//(*is)->pProdnScen == pProdnScen )
			return (*is);
	}

	return 0;
}


//
//  The boolean version of the preceding
//

bool CScenMgr::SuperScenExists(CCasterScen* pCScen1,
							   CCasterScen* pCScen2,
							   CCasterScen* pCScen3,
							   CCasterScen* pCScen4,
							   CCasterScen* pCScen5,
							   CProdnScen*  pProdnScen)
{
	return FindSuperScen(pCScen1,pCScen2,pCScen3,pCScen4,pCScen5,pProdnScen) != 0;
}


//
//  I think this is used only by snapshots for a bootstrap at snapshot creation.
//  Thus we mark it as forSnap.
//

CSuperScen* CScenMgr::CreateSuperScen()
{
	return CreateSuperScen(0,0,0,0,0,0,true);
}




//////////////////////////////////////////////////////////
//
//  Master caster scenarios
//
//////////////////////////////////////////////////////////


//
//  We keep a master caster scenario for each caster.
//  Its id is the same as the caster number.
//  Read elsehere for more information on its use.
//  Not necessary to notify on this
//
//  This keeps track of global 910 strings for initialization
//

CCasterScen* CScenMgr::CreateMasterCasterScen(int casterNum) //### caster-specific
{
	CCasterScen* pScen = new CCasterScen(casterNum); //### caster-specific

	pScen->SetCanEdit(false);
	pScen->SetId(casterNum); //### caster-specific

	return pScen;
}


void CScenMgr::DeleteMasterCasterScen(CCasterScen* pScen)
{
	DeleteCasterScen(pScen);
}




////////////////////////////////////////////////////////
//
//  Support for archiving
//
////////////////////////////////////////////////////////


//
//  Create a backup copy
//  Not necessary to notify
//

CCasterScen* CScenMgr::CopyCasterScenForArchival(CCasterScen* pScen)
{
	CCasterScen* pNewScen = new CCasterScen(*pScen,false);
	pNewScen->SetId(pScen->Id());
	return pNewScen;
}



void CScenMgr::DeleteArchivalCasterScen(CCasterScen* pScen)
{
	delete pScen;
}


void CScenMgr::BackupCasterScen(CCasterScen* pBase)
{
	if ( pBase == 0 )
		return;

	DeleteCasterScenBackup(pBase);

	pBase->m_pBackup = new CCasterScen(*pBase,false);
	pBase->m_pBackup->m_id			= pBase->m_id;
	pBase->m_pBackup->m_idAssigned	= pBase->m_idAssigned;
	pBase->m_pBackup->m_canEdit		= pBase->m_canEdit;
	pBase->m_pBackup->m_isModified	= pBase->m_isModified;
	pBase->m_pBackup->m_isPublic		= pBase->m_isPublic;
}


CCasterScen* CScenMgr::RestoreCasterScenBackup(CCasterScen* pBase)
{
	assert( pBase->m_pBackup != 0 );

	CCasterScen* pBkup = pBase->m_pBackup;
	pBase->m_pBackup = 0;
	DeleteCasterScen( pBase );

	m_casterScens[pBkup->m_caster].push_back(pBkup);

	CCreateCasterScenMsg msg(pBkup);
	TheBus().UpdateAllObservers(0,&msg);

	CScenMru::GlobalScenMru.PromoteCasterScen(pBkup);
		
	return pBkup;
}


void CScenMgr::DeleteCasterScenBackup(CCasterScen* pBase)
{
	if ( pBase != 0 && pBase->m_pBackup != 0 ) {
		delete pBase->m_pBackup;
		pBase->m_pBackup = 0;
	}
}




//
//  Find all scenarios which have been modified and add them to the archive queue.
//

//static
void CScenMgr::ArchiveAllScens()
{
	vector<CCasterScen*> copyScens;
	vector<CCasterScen*> origScens;

	FindModifiedCasterScens(1,copyScens,origScens);
	FindModifiedCasterScens(2,copyScens,origScens);
	FindModifiedCasterScens(3,copyScens,origScens);
	FindModifiedCasterScens(4, copyScens, origScens);
	FindModifiedCasterScens(5, copyScens, origScens);

	for ( int i=0; i<copyScens.size(); ++i )
		m_archiveQueue.AddBackground(origScens[i],copyScens[i]);
}


//
//  Find all scenarios on a given caster which have been modified.
//	Give back vectors of the original scenario and a copy to use for archiving.
//  The archive copy is necessary because we may continue to edit the original.
//

// static
void CScenMgr::FindModifiedCasterScens(int casterNum, //### caster-specific
									   vector<CCasterScen*>& copyScens,
									   vector<CCasterScen*>& origScens)
{
	for ( TCasterScenVec::const_iterator is = CasterScensBegin(casterNum); //### caster-specific
		  is != CasterScensEnd(casterNum); //### caster-specific
		  ++is ) {
		if ( (*is)->IsModified() ) {

			CCasterScen* pCopy = CopyCasterScenForArchival( (*is) );
			copyScens.push_back( pCopy );
			origScens.push_back( (*is) );
			(*is)->SetModified(false);
		}
	}
}



////////////////////////////////////////////////////////////
//
//  Implementation details
//
////////////////////////////////////////////////////////////


//
//  Delete all scenarios.
//  Used when quitting.
//

void CScenMgr::DeleteAllCasterScens()
{
	for ( int i=Caster::FirstCaster; i<=Caster::LastCaster; ++i ) 

		while ( CasterScensBegin(i) != CasterScensEnd(i) )
		  DeleteCasterScen( (*CasterScensBegin(i)) );
	
	// we have to delete the snapshot's direct scenarios by hand,
	//   because they are not in the lists.

	DeleteCasterScen(TheSnapshot.SuperScen()->CasterScen(Caster::C1));
	DeleteCasterScen(TheSnapshot.SuperScen()->CasterScen(Caster::C2));
	DeleteCasterScen(TheSnapshot.SuperScen()->CasterScen(Caster::C3));
	DeleteCasterScen(TheSnapshot.SuperScen()->CasterScen(Caster::C4));
	DeleteCasterScen(TheSnapshot.SuperScen()->CasterScen(Caster::C5));

}


void CScenMgr::DeleteAllProdnScens()
{
	while ( ProdnScensBegin() != ProdnScensEnd() )
		DeleteProdnScen( (*ProdnScensBegin()) );
}




//
//  When deleting a scenario, we must delete any superscenarios
//    that contain a reference to it.

void CScenMgr::DeleteContainingSuperScens(CScenario* pScen)
{
	TSuperScenVec targets;

	for ( TSuperScenVec::iterator is = SuperScensBegin();
		  is != SuperScensEnd();
		  ++is ) {
		if ((*is)->m_pCScens[Caster::C1] == pScen 
			||
			(*is)->m_pCScens[Caster::C2] == pScen
			||
			(*is)->m_pCScens[Caster::C3] == pScen
			||
			(*is)->m_pCScens[Caster::C4] == pScen
			||
			(*is)->m_pCScens[Caster::C5] == pScen
			||
			(*is)->m_pProdnScen == pScen)
		
			targets.push_back(*is);
	}

	for ( TSuperScenVec::iterator iss = targets.begin();
		  iss != targets.end();
		  ++iss )

		DeleteSuperScen((*iss));
}


