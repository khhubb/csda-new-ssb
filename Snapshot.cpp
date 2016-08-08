// Snapshot.cpp: implementation of the CSnapshot class.
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"


#include "csda.h"
#include "Snapshot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <math.h>

#include "CICode.h"
#include "DBConn.h"
#include "Order.h"
#include "UnappOrderSet.h"
#include "DalyPlanOrdersOnlySet.h"
#include "ScenMgr.h"
#include "SuperScen.h"
#include "SpecCmp.h"
#include "SteelSpecCrossAppSet.h"
#include "MillOrderChemsSet.h"
#include "SpecChemRangePresetSet.h"
#include "nTime.h"
#include "CsdaEventLogSet.h"
#include "CsdaEventLogMaxId.h"
#include "CasterScenArchiver.h"
#include "ScenMru.h"
#include "NOutputWnd.h"

#include "LongOpn.h"

#include "CheckStockApplicationRules.h"   // added k. hubbard 4-4-08

/////////////////////////////////////////////////////////////////////
//
//  class CSnapshot
//
//
//  There will be exactly one object of this class, 
//	available through the global variable TheSnapshot.
//
//	All non-editable information from the database is collected here.
//  This includes the order book itself, the strings on each caster,
//	CI bogies, specs, etc.
//
//  TheSnapshot will be initialized from the database(s) at startup.
//  The cast strings will be updated when new 910s are made or old 910s 
//     are updated.
//
/////////////////////////////////////////////////////////////////////


//  singleton creation semantics.

// static
bool CSnapshot::m_bCreated = false;


// global variable representing THE snapshot.

CSnapshot TheSnapshot;



//  c-tor needs only enforce singleton semantics
//  and provide some contained class c-tor args


CSnapshot::CSnapshot() 
: m_bogieMgr(m_ciMgr)
{
	assert( ! m_bCreated );
	m_bCreated = true;

	m_pSuperScen = CScenMgr::CreateSuperScen();
}




// As part of destruction, get rid of all external objects 
//   which reference objects allocated here.
// Primarily, this means we must delete all scenarios.


CSnapshot::~CSnapshot()
{
	//CScenMgr::DeleteAllCasterScens();
	Cleanup();
	
}


//  Cleaning up involves getting rid of all allocated objects
//  This is done either for d-tor or for Initialize.
//  We need not worry about the embedded manager objects,
//     such as the ciMgr and bogieMgr instances --
//     if we are d-tor, they will d-tor on their own shortly.
//     if we are initializing, we will soon initialize them from the db.


void CSnapshot::Cleanup()
{
	release(m_orders.begin(),m_orders.end());
	m_orders.clear();
}


////////////////////////////////////////////////////////////////////
//
//  Snapshot loading
//
////////////////////////////////////////////////////////////////////


//  This involves initializing a variety of tables 
//   from the database.

static const int STEP_SPECS				=  0;
static const int STEP_BOGIES			=  1;
static const int STEP_UNAPPLIED_ORDERS	=  2;
static const int STEP_DAILY_PLAN_ORDERS	=  3;
static const int STEP_STEEL_SPECS		=  4;
static const int STEP_CROSS_APPS		=  5;
static const int STEP_MILL_ORDER_CHEMS	=  6;
static const int STEP_PLAN				=  7;
static const int STEP_HOT_METAL			=  8;
static const int STEP_CAST_RULES		=  9;  // added k. hubbard 4-4-08
static const int STEP_FINALIZE			= 10;
static const int STEP_DONE				= 11;

static CLongOpn* PSnapLongOpn = 0;

bool CSnapshot::Initialize()
{
	Cleanup();

	CDBConn dbcsda(CDBConn::DB_CSDA);
	dbcsda.Connect();

	CDBConn dbdpa(CDBConn::DB_DPA);
	dbdpa.Connect();

	CDBConn dbc(CDBConn::DB_RULES);   // added 2 lines k. hubbard 4-4-08
	dbc.Connect();

	CDatabase* pDBCSDA	= dbcsda.GetDB();
	CDatabase* pDBPDA	= dbdpa.GetDB();

	CDatabase* pDBRULES	= dbc.GetDB();   // added k. hubbard 4-4-08

	PSnapLongOpn = new CLongOpn;
	PSnapLongOpn->m_strings.push_back("Read specs");
	PSnapLongOpn->m_strings.push_back("Read CI bogies");
	PSnapLongOpn->m_strings.push_back("Read unapplied orders");
	PSnapLongOpn->m_strings.push_back("Read daily plan orders");
	PSnapLongOpn->m_strings.push_back("Find steel specs");
	PSnapLongOpn->m_strings.push_back("Read spec cross app table");
	PSnapLongOpn->m_strings.push_back("Read order chems.");
	PSnapLongOpn->m_strings.push_back("Read daily plan");
	PSnapLongOpn->m_strings.push_back("Read hot metal supply");
	PSnapLongOpn->m_strings.push_back("Read cast rules");  // added k. hubbard 4-4-08
	PSnapLongOpn->m_strings.push_back("Doing final initializations");
	PSnapLongOpn->m_title = "Loading snapshot";
	PSnapLongOpn->Go();


	// Read in specs

	PSnapLongOpn->SetCurrentStep(STEP_SPECS);
	NOutputWnd::ShowMainLoadWnd();	
	NOutputWnd::ClearText(COutputControlBar::OCB_LOAD);
	NOutputWnd::PostLoadMsg("Begin loading snapshot.");

	if ( ! m_specMgr.Initialize(pDBPDA) )
		return false;

	if ( ! m_heatSpecCrossApp.Init(pDBPDA) )
		return false;
	

	//  Read in CI Codes and bogies

	PSnapLongOpn->SetCurrentStep(STEP_BOGIES);

	if ( ! m_ciMgr.CreateGroups(pDBPDA) )   //test comm out 5-3-10 k. hubbard
		return false;                       //test comm out 5-3-10 k. hubbard


//	if ( ! m_bogieMgr.Initialize80HSMBogies(pDBPDA) ) // Added 5-4-10 k. hubbard
//		return false;

//	if ( ! m_bogieMgr.InitializeBogies(CDatabase* pDB) ) 
	if ( ! m_bogieMgr.InitializeBogies(pDBPDA) ) 
		return false;

	// Read in orders

	if ( ! ReadOrders(pDBPDA) ) 
		return false;

  
    //Location 1

    // Read in spec chem range presets

	if ( ! PopulateSpecChemRangePresets(pDBCSDA) )
		return false;
	

	// Read in current lineups

 
	//Location 2

	PSnapLongOpn->SetCurrentStep(STEP_PLAN);

	if ( ! m_pSuperScen->InitFromDalyPlan() )
		return false;
	

	//Location 3

	if ( ! m_pSuperScen->LoadDalyPlanUpdates() )
		return false;




	// Location 4

	// Read in hot metal supply information

	PSnapLongOpn->SetCurrentStep(STEP_HOT_METAL);

	//CTime today(NTime::ThisDayMidnight(CTime::GetCurrentTime().GetTime()));
	//CTime yesterday = today - CTimeSpan(1,0,0,0);
	//if ( ! m_hmSupplyMgr.Initialize(yesterday) )
	//	return false;



	// Location 5

	if ( ! LoadHMSupplyInfo() )
		return false;


	// Read in Cast Rules   added 2 lines below k. hubbard 4-4-08
//	if ( ! CasterRuleCheck(pDBRULES) ) 
	if ( ! CasterRuleCheck(pDBPDA) ) 
		return false;

	// Do various miscellaneous initialization

	// Location 6

	PSnapLongOpn->SetCurrentStep(STEP_FINALIZE);

	NOutputWnd::PostLoadMsg("Doing final initializations ... ");
	CopyInitialCasterScens();
	NOutputWnd::PostLoadMsg("done.\n");

	PSnapLongOpn->SetCurrentStep(STEP_DONE);
	PSnapLongOpn->Done(500);

	PSnapLongOpn = 0;

	return true;
}



//  We read in the current set of orders.	
//     plus some other goodies

bool CSnapshot::ReadOrders(CDatabase* pDB)
{
	PSnapLongOpn->SetCurrentStep(STEP_UNAPPLIED_ORDERS);
	
	NOutputWnd::PostLoadMsg("Loading orders: ");

	bool retval = true;
	CUnappOrderSet orderSet1(pDB);
	CDalyPlanOrdersOnlySet orderSet2(pDB);

///	CCheckStockApplicationRules orderSet3(pDB);  // added k. hubbard 4-4-08
	
	long count = 0;

	try
	{
		{
			//orderSet.m_strSort = "[MILL_ORDER_NO]";
	
			orderSet1.Open(CRecordset::forwardOnly,
						  0,
						  CRecordset::readOnly);
				
			while ( ! orderSet1.IsEOF() ) {
			
				COrder* pOrder = new COrder;
				pOrder->Init(orderSet1);
				m_orders.push_back(pOrder);
	
				orderSet1.MoveNext();

				++count;
				NOutputWnd::WriteProgress(count);
			}
		
		}

		PSnapLongOpn->SetCurrentStep(STEP_DAILY_PLAN_ORDERS);



		{
			//orderSet.m_strSort = "[MILL_ORDER_NO]";
	
			orderSet2.Open(CRecordset::forwardOnly,
						  0,
						  CRecordset::readOnly);
				
			while ( ! orderSet2.IsEOF() ) {
			
				COrder* pOrder = new COrder;
				pOrder->Init(orderSet2);
				m_orders.push_back(pOrder);
	
				orderSet2.MoveNext();

				++count;
				NOutputWnd::WriteProgress(count);
			}
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

	   retval = false;
	}

	if ( orderSet1.IsOpen() )
		orderSet1.Close();

	if ( orderSet2.IsOpen() )
		orderSet2.Close();

	sort(m_orders.begin(),
		 m_orders.end(),
		 less_p<COrder*>());

	{
		ostrstream ostr;
		ostr << "read " << count << " orders.\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}


	PSnapLongOpn->SetCurrentStep(STEP_STEEL_SPECS);

	NOutputWnd::PostLoadMsg("Computing available steel specs ... ");
	PopulateAvailSteelSpecs();
	NOutputWnd::PostLoadMsg(" done.\n");

	PSnapLongOpn->SetCurrentStep(STEP_CROSS_APPS);

	NOutputWnd::PostLoadMsg("Computing steel spec cross apps ... ");
	PopulateSteelSpecCrossAppMap(pDB);
	NOutputWnd::PostLoadMsg(" done.\n");

	PSnapLongOpn->SetCurrentStep(STEP_MILL_ORDER_CHEMS);

	ReadMillOrderChems(pDB);

	return retval;
}



//
//  The available steel specs are just the steel spec values from all the orders,
//		unique-ified.
//

void CSnapshot::PopulateAvailSteelSpecs()
{
	for ( vector<COrder*>::const_iterator io = m_orders.begin();
		  io != m_orders.end();
		  ++io )

		m_availSteelSpecs.insert((*io)->SteelSpec());
}



//
//  For each steel spec calculated above,
//    we compute the steel specs which can be cross-applied with it.
//

void CSnapshot::PopulateSteelSpecCrossAppMap(CDatabase* pDB)
{
	// make entries in map for each steel spec

	static set<CString> emptyVec;

	for ( set<CString>::const_iterator is = AvailSteelSpecs().begin();
		  is != AvailSteelSpecs().end();
		  ++is ) 
		m_steelSpecCrossAppMap.insert( TSteelSpecCrossAppMap::value_type((*is),emptyVec));


	CSteelSpecCrossAppSet xSet(pDB);

	try
	{
		// get the table of cross apps
		xSet.Open(CRecordset::forwardOnly,
				  0,
				  CRecordset::readOnly);

		while ( ! xSet.IsEOF() ) {
			
			//  Use the left 3 chars of the spec in the table
			//    in order to find a lower bound of possibly applicable steel specs

			CString lowerKey = xSet.m_SPECARD_SPEC.Left(3);
			CSpecCmp cmp(xSet.m_SPECARD_SPEC);

			for ( TSteelSpecCrossAppMap::iterator im = m_steelSpecCrossAppMap.lower_bound(lowerKey);
			      im != m_steelSpecCrossAppMap.end() && (*im).first.Left(3) == lowerKey;
				  ++im ) {
				if ( cmp((*im).first) )
					// we have a match -- add this spec to the vector
					(*im).second.insert(xSet.m_SPECARD_CROSS_APPL_SPEC_CD);
			}
				  
			xSet.MoveNext();
		}

	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

	}
	
	if ( xSet.IsOpen() )
		xSet.Close();

#if _DEBUG

	{
		afxDump << "\n\nSteelSpecCrossAppMap --- BEGIN \n";

		for ( TSteelSpecCrossAppMap::const_iterator im = m_steelSpecCrossAppMap.begin();
		      im != m_steelSpecCrossAppMap.end();
			  ++im ) {

			afxDump << (*im).first << " -- ";

			for ( set<CString>::const_iterator is = (*im).second.begin();
				  is != (*im).second.end();
				  ++is ) 

				afxDump << (*is) << " " ;

			afxDump << "\n";
		}

		afxDump << "SteelSpecCrossAppMap --- END\n\n\n";
	}
#endif
}


//
//  The look up function
//

const set<CString>& CSnapshot::FindSteelSpecCrossApp(CString& steelSpec) const
{
	static set<CString> emptySet;

	TSteelSpecCrossAppMap::const_iterator im = m_steelSpecCrossAppMap.find(steelSpec);

	if ( im == m_steelSpecCrossAppMap.end() )
		return emptySet;
	else
		return (*im).second;
}


//
//  there is a table which gives some default carbon/manganese ranges
//    for certain specs.
//  read the records
//

bool CSnapshot::PopulateSpecChemRangePresets(CDatabase* pDB)
{
	bool retval = true;

	NOutputWnd::PostLoadMsg("Loading spec chem range presets: ");

	CSpecChemRangePresetSet xSet(pDB);

	long count = 0;
	try
	{
		// get the table of preset 
		xSet.Open(CRecordset::forwardOnly,
				  0,
				  CRecordset::readOnly);

		while ( ! xSet.IsEOF() ) {

			m_specChemRangePresets.push_back(CSpecChemRangePreset(xSet.m_Spec,
																  xSet.m_MinCarbon,
																  xSet.m_MaxCarbon,
																  xSet.m_MinMn,
																  xSet.m_MaxMn));				  
			xSet.MoveNext();
			++count;
		}

	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();


		retval =  false;
	}

	if ( xSet.IsOpen() )
		xSet.Close();

	{
		ostrstream ostr;
		ostr << "read " << count << " records\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}

	return retval;

}


void CSnapshot::ReadMillOrderChems(CDatabase* pDB)
{
	NOutputWnd::PostLoadMsg("Reading MillOrder chems: ");

	CMillOrderChemsSet chemSet(pDB);

	long count = 0;

	try
	{
		// orders are sorted by orderNum
		// chems are sorted by orderNum/element
		// do a double ordered sequence traversal


		chemSet.Open(CRecordset::forwardOnly,
					 0,
					 CRecordset::readOnly);

		vector<COrder*>::iterator io = m_orders.begin();
				
		while ( ! chemSet.IsEOF() && io != m_orders.end() ) {

			// FP change
			//if ( (*io)->OrderNum() < chemSet.m_MILL_ORDER_NO )
			if ( (*io)->FpOrderNum() < chemSet.m_FP_M_ORDER_NO )

				++io;

			// FP change
			//else if ( (*io)->OrderNum() > chemSet.m_MILL_ORDER_NO ) {
			else if ( (*io)->FpOrderNum() > chemSet.m_FP_M_ORDER_NO ) {

				chemSet.MoveNext();
				++count;
				NOutputWnd::WriteProgress(count);

			}

			else {

				CChem::Element elt;
				if ( CChem::ToElement(chemSet.m_MORD_INHSE_ORDER_CHEM_EL,elt) )

					(*io)->SetEltRange(elt,
									   chemSet.m_MORD_INHSE_ORDER_PCT_MIN,
									   chemSet.m_MORD_INHSE_ORDER_PCT_MAX,
									   (chemSet.IsFieldNull(&chemSet.m_MORD_INHSE_ORDER_REAS_CD)
									    ||
										chemSet.m_MORD_INHSE_ORDER_REAS_CD.GetLength() < 1
									    ? ' '
										: chemSet.m_MORD_INHSE_ORDER_REAS_CD[0]));
				else {
#ifdef _DEBUG
					afxDump << "Missing element: " 
							<< chemSet.m_MORD_INHSE_ORDER_CHEM_EL
							<< "\n";
#endif
				}
				chemSet.MoveNext();
				++count;
				NOutputWnd::WriteProgress(count);
			}
			
		}

		chemSet.Close();

	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   if ( chemSet.IsOpen() )
		   chemSet.Close();
	}

	{
		ostrstream ostr;
		ostr << "Read " << count << " records.\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}
}




void CSnapshot::CopyInitialCasterScens()
{
	CCasterScen* p1 = CopyInitialCasterScen(Caster::C1);
	CCasterScen* p2 = CopyInitialCasterScen(Caster::C2);
	CCasterScen* p3 = CopyInitialCasterScen(Caster::C3);
	CCasterScen* p4 = CopyInitialCasterScen(Caster::C4);
	CCasterScen* p5 = CopyInitialCasterScen(Caster::C5);
	CProdnScen*  pp = CScenMgr::CreateProdnScen(true);

	p1->SetComment("Copy of daily plan");
	p2->SetComment("Copy of daily plan");
	p3->SetComment("Copy of daily plan");
	p4->SetComment("Copy of daily plan");
	p5->SetComment("Copy of daily plan");

	CScenMgr::SuperScen(p1,p2,p3,p4,p5,pp);
}




CCasterScen* CSnapshot::CopyInitialCasterScen(int casterNum)
{
	return CScenMgr::CopyCasterScen(m_pSuperScen->CasterScen(casterNum),true,true);
}


CCasterScen* CSnapshot::CopyInitialCasterScen(int casterNum,
											  vector<int>& indexes909)
{
	return CScenMgr::CopyCasterScen(m_pSuperScen->CasterScen(casterNum),
															  true,
															  indexes909);
}


bool CSnapshot::LoadHMSupplyInfo()
{
	return m_hmSupplyMgr.Initialize();
}





void CSnapshot::MaybeLoadNew910s()
{
	if ( m_last910UpdateTime + CTimeSpan(0,0,2,0) < CTime::GetCurrentTime() )
		LoadNew910s();
}


void CSnapshot::LoadNew910s()
{
	bool casterSeen[Caster::CasterArrayLen] = { false, false, false, false, false, false };

	CCasterScen* pScens[Caster::CasterArrayLen];

	{
		for ( int i=Caster::FirstCaster; i<=Caster::LastCaster; ++i )
			pScens[i] = CScenMgr::CreateMasterCasterScen(i);
	}

	vector<CCastStringId> ids;
	CTime lastTime(1980,1,1,0,0,0);
	long lastId = TheSnapshot.GetUpdated910Ids(m_last910RecordId,lastTime,ids);

	if ( ids.size() > 0 ) {
		ostrstream msg;
		msg << "The following 910s have been created or modified:\n";
		for ( vector<CCastStringId>::iterator iid = ids.begin();
			  iid != ids.end();
			  ++iid ) {
			msg << "Caster #" << (*iid).Caster() << ": "
				<< setw(4) << (*iid).Year() << " "
				<< setw(2) << (*iid).Week() << " "
				<< setw(3) << (*iid).StringId() << "\n";

			casterSeen[ (*iid).Caster() ] = true;
		}

		msg << "\nLoad now?" << ends;

		int answer = AfxMessageBox(msg.str(),MB_YESNO);
		msg.freeze(false);

		if ( IDYES != answer )
			goto finish;
	}


	{
		for ( vector<CCastStringId>::iterator iid = ids.begin();
			  iid != ids.end();
			  ++iid )
			LoadNew910( pScens[ (*iid).Caster() ], (*iid) );
	}
	CopyNew910sIntoAllScens(pScens);

	{
		for ( int caster=Caster::FirstCaster; caster <= Caster::LastCaster; ++caster )
			if ( casterSeen[caster] ) {
				CScenMgr::CCasterScenInfoChangedMsg msg(0);
				TheBus().UpdateAllObservers(0,&msg);

				CScenMru::GlobalScenMru.PromoteCasterScen(CScenMru::GlobalScenMru.CurrentCasterScen(caster));
			}
	}
			
	m_last910RecordId = max(m_last910RecordId,lastId);
	m_last910UpdateTime = CTime::GetCurrentTime();
	
finish:

	{
		for ( int i=Caster::FirstCaster; i<=Caster::LastCaster; ++i )
			CScenMgr::DeleteMasterCasterScen(pScens[i]);
	}
}

//static
long CSnapshot::GetUpdated910Ids(long lastId, CTime lastTime, vector<CCastStringId>& ids)
{

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();

	CCsdaEventLogMaxId maxlogRS(dbc.GetDB());
	CCsdaEventLogSet logRS(dbc.GetDB());
	logRS.m_nParams = 3;
	logRS.m_paramEventType = "910";
	logRS.m_paramLastId = lastId;
	logRS.m_paramLastTimestamp = lastTime;

	logRS.m_strFilter = "[EventType] = ? AND [EventId] > ? AND [EventTimestamp] > ?";

	long lastLoadedId = 0;

	try 
	{
		logRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);

		if ( logRS.IsEOF() ) {

			maxlogRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly|CRecordset::executeDirect);
			if ( ! maxlogRS.IsEOF() )
				lastLoadedId = maxlogRS.m_MaxOfEventId;
		}

		while ( ! logRS.IsEOF() ) {

			lastLoadedId = max(lastLoadedId,logRS.m_EventId);

			istrstream istr( LPCTSTR( logRS.m_Comment ) );

			int caster;
			int week;
			int year;
			int stringId;

			istr >> caster >> year >> week >> stringId;

			bool bad;
			CString msg;

			if ( ! Caster::IsValidCasterValue(caster) ) {
				bad = true;
				msg.Format("Bad caster (%d) in EventLog # %ld",caster,logRS.m_EventId);
			}
			else if ( year < 1990 || year > 2037 ) {
				bad = true;
				msg.Format("Bad year (%d) in EventLog # %ld",year,logRS.m_EventId);
			}
			else if ( week < 0 || week > 53 ) {
				bad = true;
				msg.Format("Bad week (%d) in EventLog # %ld",week,logRS.m_EventId);
			}
			else if ( stringId < 0 || stringId > 999 ) {
				bad = true;
				msg.Format("Bad string id (%d) in EventLog # %ld",stringId,logRS.m_EventId);
			}
			else {
				bad = false;
			}

			if ( bad )
				AfxMessageBox(LPCTSTR(msg));
			else
				ids.push_back( CCastStringId(year,week,caster,stringId) );

			logRS.MoveNext();
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   
	}

	if ( logRS.IsOpen() )
		logRS.Close();

	if ( maxlogRS.IsOpen() )
		maxlogRS.Close();

	
	return lastLoadedId;
}


void CSnapshot::LoadNew910( CCasterScen* pScen, CCastStringId& id)
{
	int caster = id.Caster();

	CCastString* pString = pScen->FindCastString(id);
	if ( pString != 0 )
		pScen->DeleteString(pString);

	CCasterScenArchiver::LoadStringFromMasterScen(pScen,id);
}


void CSnapshot::CopyNew910sIntoAllScens(CCasterScen* pScens[Caster::CasterArrayLen])
{
	for ( int caster=Caster::FirstCaster; caster<=Caster::LastCaster; ++caster ) {

		for ( vector<CCastString*>::iterator is = pScens[caster]->CastStrings().begin();
		      is != pScens[caster]->CastStrings().end();
			  ++is )

		CopyNew910IntoAllScens( caster, (*is) );
	}
}


void CSnapshot::CopyNew910IntoAllScens( int caster, CCastString* pString )
{
	CCastString* pOrigString = CopyNew910IntoScen(m_pSuperScen->CasterScen(caster),pString,0);

	for ( vector<CCasterScen*>::iterator is = CScenMgr::CasterScensBegin(caster);
		  is != CScenMgr::CasterScensEnd(caster);
		  ++is )
		CopyNew910IntoScen( (*is), pString, pOrigString );
}


//static
CCastString* CSnapshot::CopyNew910IntoScen( CCasterScen* pScen, CCastString* pString, CCastString* pOrigString )
{
	CCastString* pOldString = pScen->FindCastString(pString->Id());

	if ( pOldString != 0 )
		// TODO: offer to rename this string, non910.
		pScen->DeleteString(pOldString);

	CCastString* pNewString = new CCastString(pScen,*pString,pOrigString);

	pScen->CastStrings().push_back(pNewString);
	// TODO:  check for order imbalances

	sort(pScen->CastStrings().begin(),
		 pScen->CastStrings().end(),
		 less_p<CCastString*>());

	return pNewString;
}


#if 0 
// not needed?

void CSnapshot::UpdateDBs( CCasterScen* pScen)
{
	//  The order/slabs map in the scenarios does not separate 909s from 910s from new strings.
	//  We need counts for 909s and 910s explicitly and separately.

	OrderUsageMap orm;

	ComputeORM(1,orm);
	ComputeORM(2,orm);
	ComputeORM(3,orm);

	OrderUseRecord zeroRec;

	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CDatabase* pDB = dbc.GetDB();

	CString query;
			
	for ( vector<COrder*>::iterator io = m_orders.begin();
		  io != m_orders.end();
		  ++io ) {

		OrderUsageMap::iterator im = orm.find( (*io) );

		OrderUseRecord& rec = (im == orm.end() ? zeroRec : (*im).second);

		if ( (*io)->SmPlanSlabsOn909() != rec.num909Slabs ) {

			query.Format("UPDATE [P_STEEL_MAKE_PLAN] SET"
						  " [SM_PLAN_SLABS_ON_909] = %ld,"
						  " [SM_PLAN_ON_909_IND] = '%s',"
						  " [SL_APPLY_909_PLAN_SL_WGT] = %f,"
						  " [SM_PLAN_SLABS_TO_PLAN_IND] = '%s'"
						  " WHERE MILL_ORDER_NO = %ld;",
						  long(rec.num909Slabs),
						  (rec.num909Slabs == 0 ? " " : "Y"),
						  rec.wgt909Slabs,
						  ((*io)->NumSlabsDue() > rec.num909Slabs ? "Y" : " "),
						  (*io)->OrderNum() );
#if _DEBUG
			afxDump << query << "\n";
#endif

			pDB->ExecuteSQL(query);

			(*io)->SmPlanSlabsOn909() = rec.num909Slabs;
		}

		if ( fabs((*io)->SlApply909PlanHbWgt() - 0.982 * rec.wgt909Slabs) > 10.0 ) {

			CString query;
			query.Format("UPDATE [P_MILL_ORDER] SET"
						  " [SL_APPLY_909_PLAN_HB_WGT] = %f"
						  " WHERE MILL_ORDER_NO = %ld;",
						 rec.wgt909Slabs * 0.982,
						  (*io)->OrderNum() );

#if _DEBUG
			afxDump << query << "\n";
#endif

			pDB->ExecuteSQL(query);
			(*io)->SlApply909PlanHbWgt() = rec.wgt909Slabs * 0.982;

		}
	}
}
	

static void AddORM( OrderUsageMap& orm,
				    CCSOrder* pOrder,
					bool is909 )
{
	if ( pOrder->Order() == 0 ) 
		return;



	pair<OrderUsageMap::iterator,bool> pr = orm.insert(OrderUsageMap::value_type(pOrder->Order(),OrderUseRecord()));

	OrderUseRecord& rRec = (*(pr.first)).second;

	if ( is909 ) {
		rRec.num909Slabs += pOrder->NumSlabs();
		rRec.wgt909Slabs += pOrder->NumPieces()*pOrder->PieceWgt();
	}
	else {
		rRec.num910Slabs += pOrder->NumSlabs();
		rRec.wgt910Slabs += pOrder->NumPieces()*pOrder->PieceWgt();
	}
}


void CSnapshot::ComputeORM( int casterNum,
						    OrderUsageMap& orm )
{
	CCasterScen* pScen = m_pSuperScen->GetCasterScen(casterNum);

	vector<CCastString*>& strings = pScen->CastStrings();

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {
		
		for ( vector<CCastStringHeat>::iterator ih = (*is)->Heats().begin();
			  ih != (*is)->Heats().end();
			  ++ih ) {

			if ( (*ih).Status() == CCastStringHeat::HEAT_STATUS_909 
				 ||
				 (*ih).Status() == CCastStringHeat::HEAT_STATUS_910 ) {

				for ( vector<CCSOrder*>::iterator io1 = (*ih).StrandBegin(1);
				      io1 != (*ih).StrandEnd(1);
					  ++io1 )

					AddORM(orm,(*io1),(*ih).Status() == CCastStringHeat::HEAT_STATUS_909 );

				for ( vector<CCSOrder*>::iterator io2 = (*ih).StrandBegin(2);
				      io2 != (*ih).StrandEnd(2);
					  ++io2 )

					AddORM(orm,(*io2),(*ih).Status() == CCastStringHeat::HEAT_STATUS_909 );
			}
		}
	}
}

#endif // not needed

///#ifdef DEPRECATED
// get rid of this, then get rid of CRuleCheckLineupSet.
bool CSnapshot::CasterRuleCheck(CDatabase* pDB)       /// added k. hubbard 4-4-08
{
	PSnapLongOpn->SetCurrentStep(STEP_CAST_RULES);
	
	NOutputWnd::PostLoadMsg("Loading rules: ");

	bool retval = true;

//	CDBConn dbc(CDBConn::DB_RULES);
//	dbc.Connect();

	CCheckStockApplicationRules rset1(pDB);
///	CCheckStockApplicationRules rset(dbc.GetDB());

///	rset.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);

///	static CString strStatus = "3";
	long count = 0;

	try 
	{ 
			//rset.m_strSort = "[MILL_ORDER_NO]";
		{
			rset1.Open(CRecordset::forwardOnly,
						  0,
						  CRecordset::readOnly);
				
			while ( ! rset1.IsEOF() ) {
			
///				COrder* pOrder = new COrder;
///				Init(rset1);
///				pOrder->Init(rset1);
///				pOrder->Init(orderSet1);
///				m_orders.push_back(pOrder);
//	            rset1.AddNew();
//				rset1.push_back
				rset1.MoveNext();

				++count;
				NOutputWnd::WriteProgress(count);
			}
		
		}

///		dbc.GetDB()->BeginTrans();
///		dbc.GetDB()->ExecuteSQL("DELETE FROM CSDAInput");

///		int heatIndex = 0;
///		for ( vector<CCastStringHeat>::iterator ih = Heats().begin();
///			  ih != Heats().end();
///			  ++ih, ++heatIndex ) {


///			int lotIndex = 0;
///			{
				
///				for ( T_CSOrderVec::iterator io = (*ih).StrandBegin(1);
///					  io != (*ih).StrandEnd(1);
///					  ++io, ++lotIndex )
///					UploadCSOrderForRuleCheck(rset,strStatus,heatIndex,(*ih),1,lotIndex,(*io));
///			}

///			{
				
///				for ( T_CSOrderVec::iterator io = (*ih).StrandBegin(2);
///					  io != (*ih).StrandEnd(2);
///					  ++io, ++lotIndex )
///					UploadCSOrderForRuleCheck(rset,strStatus,heatIndex,(*ih),2,lotIndex,(*io));
///			}
///		}
	}
	catch ( CDBException* e )
	{

	   AfxMessageBox( "Unable to upload for rules check due to database error: \n" + e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

///	   dbc.GetDB()->Rollback();

	   return false;
	}

	if ( rset1.IsOpen() )
		rset1.Close();

	return true;
}
///#endif



