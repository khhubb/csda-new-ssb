// HMLoadGraphDoc.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "HMLoadGraphDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SuperScen.h"
#include "CasterScen.h"
#include "CastString.h"
#include "CastStringHeat.h"
#include "ScenMgr.h"
#include "MessageBus.h"



/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphDoc
//
//  This is not the usual dummy document we use for MVC.
//
//  Rather, we maintain state here for the display.
//  This could have been otherwise.  Specifically, we could have
//    put certain information on the individual scenarios,
//    and certain information on super scenarios,
//  but since it ain't broke, I decided not to fix it.
//
//  The biggest problem is that the display is dependent on the current 
//    selection of caster scenarios.  When an outside force changes this,
//    by promoting a scenario, we need to update the data here and update
//    the various displays.  If we viewports only listen to the promotion
//    messages, how do we prevent multiple computations of the data here,
//    each viewport making the request. If the document is also listening
//    how do we make sure the document updates before the viewports redraw.
//  It is simplest to have the document listen for promotion messages, have
//    the viewports ignore promotion messages, and have the document send out
//    a special message (MSG_ID_HM_SCEN_UPDATE) after it has updated its display.
//
///////////////////////////////////////////////////////////////////////////////   

IMPLEMENT_DYNCREATE(CHMLoadGraphDoc, CDocument)

CHMLoadGraphDoc::CHMLoadGraphDoc()
{

	TheBus().AddObserver(this);

	// eventually, change this to false (true for now for debugging)
	m_show910s = true;

	m_fixedTitle = "HM Load";
}


BOOL CHMLoadGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

//	Init();

	return TRUE;
}

CHMLoadGraphDoc::~CHMLoadGraphDoc()
{
	//TheBus().RemoveObserver(this);
}


BEGIN_MESSAGE_MAP(CHMLoadGraphDoc, CDocument)
	//{{AFX_MSG_MAP(CHMLoadGraphDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphDoc diagnostics

#ifdef _DEBUG
void CHMLoadGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHMLoadGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphDoc serialization

/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphDoc commands



///////////////////////////////////////////////////////////
//
//  Notifications
//
///////////////////////////////////////////////////////////

void CHMLoadGraphDoc::CUpdateMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CHMLoadGraph: Update");
}


//////////////////////////////////////////////////////////
//
//  Accessors
//
//////////////////////////////////////////////////////////


int CHMLoadGraphDoc::GetCasterScenIndex(int casterNum) const
{
	return CScenMgr::CasterScenIndex(casterNum,GetCurrentCasterScen(casterNum));
}


vector<CHMSupplyRecord>& CHMLoadGraphDoc::GetSupplyRecords()
{	
	return TheSnapshot.HMSupplyMgr().GetRecords();	
}


vector<CCastString*>& CHMLoadGraphDoc::GetStrings(int caster)
{
	return m_strings[caster];
}



CTime CHMLoadGraphDoc::CasterStartTime(int caster)

{
	return ComputeEarliestTime(caster);
}



// static
CString CHMLoadGraphDoc::FindSpecToUse( CCastString* pString )
{
	//  Use the first heat spec assigned, if any

	for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
		  ih != pString->Heats().end();
		  ++ih ) {
			  
		if ( (*ih).Spec() != ""  && (*ih).Spec() != "       " ) 
			return (*ih).Spec();
	}

	// no heat spec assigned, look at a lot, if there is one
	
	if ( pString->Strand(1).size() != 0 )
		return (*(pString->StrandBegin(1)))->LotSpec();
	else if ( pString->Strand(2).size() != 0 )
		return (*(pString->StrandBegin(2)))->LotSpec();

	// no lots either, so use the designated spec for the string
	return pString->Props().InitSpec();
}


// static
void CHMLoadGraphDoc::FindWidthsToUse( CCastString* pString,
									   long& rStrand1Begin,
									   long& rStrand1End,
									   long& rStrand2Begin,
									   long& rStrand2End)
{
	if ( pString->Strand(1).size() != 0 ) {
		rStrand1Begin = long(pString->Strand(1).front()->SlabWidth());
		rStrand1End   = long(pString->Strand(1).back()->SlabWidth());
	}
	else {
		rStrand1Begin = pString->Props().StartWidth(1);
		rStrand1End   = pString->Props().EndWidth(1);
	}
	
	if ( pString->Id().Caster() == 2 ) {
		rStrand2Begin = 0;
		rStrand2End   = 0;
	}
	else if ( pString->Strand(2).size() != 0 ) {
		rStrand2Begin = long(pString->Strand(2).front()->SlabWidth());
		rStrand2End   = long(pString->Strand(2).back()->SlabWidth());
	}
	else {
		rStrand2Begin = pString->Props().StartWidth(2);
		rStrand2End   = pString->Props().EndWidth(2);
	}
}


///////////////////////////////////////////////////////////
//
//  Updates
//
///////////////////////////////////////////////////////////

void CHMLoadGraphDoc::OnUpdate(IMvcSubject* pSubject,
									IMvcMessage* pMsg)
{
	switch ( pMsg->GetTypeID() )
	{	
	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
		Update();
		break;


	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
	case MSG_ID_MRU_SELECT_CAST_STRING:
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


void CHMLoadGraphDoc::ActivationChange(BOOL bActivate)
{
	if (  bActivate ) {
		// we are being activated
		// update our display
		Update();

	}
}


//////////////////////////////////////////////////////////////////
//
//  Initialization
//
//////////////////////////////////////////////////////////////////


//
//  The only difference between initialize and update is initializing m_show910s.
//
//  Otherwise, both recompute the sequence of strings and start times,
//  and recompute the load information for each caster and cumulative.
//
//  Notification is sent to the viewports so they can update their displays.
//

void CHMLoadGraphDoc::Init()
{
	// eventually, return this to the correct initial value (false)
	//  set to this now for debugging only.

	m_show910s = true;
	SetStrings();
	ComputeLoad();

	CUpdateMsg msg;
	TheBus().UpdateAllObservers(this,&msg);
}


void CHMLoadGraphDoc::Update()
{
	SetStrings();
	ComputeLoad();

	CUpdateMsg msg;
	TheBus().UpdateAllObservers(this,&msg);
}






void CHMLoadGraphDoc::SetStrings()
{
	SetStrings(1);
	SetStrings(2);
	SetStrings(3);
}

void CHMLoadGraphDoc::SetStrings(int caster)
{
	if ( IsEditing() ) {
		m_strings[caster] = GetSuperScen()->CasterScen(caster)->CastStrings(); // copy
		EnforceTurnarounds(caster);
	}
	else
		m_strings[caster].clear();
}



void CHMLoadGraphDoc::EnforceTurnarounds(int caster)
{
	vector<CCastString*> strings = m_strings[caster];	// copy
	
	// filter out invalid start times
	strings.erase( remove_if( strings.begin(),
							  strings.end(),
							  CCastString::CFilterInvalidStartTimes() ),
					strings.end() );

	// sort by start times
	sort(strings.begin(),
		 strings.end(),
		 CCastString::CStartTimeCmp());

	// now enforce turnaround times.

	if ( strings.size() <= 1 )
		return;

	for ( vector<CCastString*>::iterator is = strings.begin()+1;
		  is != strings.end();
		  ++is ) {
			 
		vector<CCastString*>::iterator ip = is-1;
		CTime nextStart = (*ip)->CastStartTime() 
							+ CTimeSpan(0,0,0,(*ip)->TotalDuration()) 
							+ (*ip)->InterStringTime();
		if ( (*is)->CastStartTime() < nextStart )
			(*is)->SetCastStartTime(nextStart);
	}

}



////////////////////////////////////////////////////////////////
//
//  Operations
//
////////////////////////////////////////////////////////////////


//  
// The user has requested a different sequence of strings.
// Set the new sequence

void CHMLoadGraphDoc::ResequenceStrings(int caster, vector<int>& indexes)
{	
	
	vector<CCastString*> newSeq;
	vector<CCastString*>& oldSeq = GetStrings(caster);

#ifdef _DEBUG
	{
		afxDump << "CHMLoadGraphDoc::ResequenceStrings: seq = \n";
		for ( vector<int>::iterator ii = indexes.begin();
			  ii != indexes.end();
			  ++ii ) 
			afxDump << "#" <<  (*ii)
					<< " = " 
					<< oldSeq[(*ii)]->Id().Week()
					<< "-"
					<< oldSeq[(*ii)]->Id().StringId()
					<< "\n";
		afxDump << "end seq\n";
	}
#endif

	for ( vector<int>::iterator ii = indexes.begin();
		  ii != indexes.end();
		  ++ii )
		newSeq.push_back( oldSeq[ (*ii) ] );
	
	m_strings[caster] = newSeq;	//copy
	EnforceTurnarounds(caster);  //  is this correct?

#ifdef _DEBUG
	{
		afxDump << "CHMLoadGraphDoc::ResequenceStrings: reseq = \n";
		for ( vector<CCastString*>::iterator is = GetStrings(caster).begin();
			  is != GetStrings(caster).end();
			  ++is )
			afxDump << (*is)->Id().Week()
					<< "-"
					<< (*is)->Id().StringId()
					<< "\n";
		afxDump << "end seq\n";
	}
#endif

	ComputeLoad();

	CUpdateMsg msg;
	TheBus().UpdateAllObservers(this,&msg);
}

//
//  A change has been made to start time on this caster
//  Recompute relevant information.
//

void CHMLoadGraphDoc::RefreshStrings(int caster)
{
	SetStrings(caster);
	ComputeLoad();

	CUpdateMsg msg;
	TheBus().UpdateAllObservers(this,&msg);
}


//
//  One of the views wants to delete a string.
//  Confirm, do it, notify.
//

void CHMLoadGraphDoc::DeleteString(CCastString* pString)
{
	CString str;
	str.Format("Are you sure you want to delete string # %d (week %d, caster %d)?",
				pString->Id().StringId(),
				pString->Id().Week(),
				pString->Id().Caster());
	if ( IDYES == AfxMessageBox(LPCTSTR(str),MB_YESNO) ) {
		CCasterScen* pScen = pString->CasterScen();
		pScen->DeleteString(pString);
		pScen->SetModified(true);
		SetStrings(pString->Id().Caster());
		ComputeLoad();

		CUpdateMsg msg;
		TheBus().UpdateAllObservers(this,&msg);
	}
}


void CHMLoadGraphDoc::CreateString(CCastStringId& id,CCastStringMiscProps& props)
{
	CCastString* pNewString = GetCurrentCasterScen(id.Caster())->CreateNewString(id,props);
	GetScenMru().SetCurrentCastString(pNewString);
	SetStrings();
	ComputeLoad();

	CUpdateMsg msg;
	TheBus().UpdateAllObservers(this,&msg);
}



/////////////////////////////////////////////////////////////
//
//  HM Load computation details
//
/////////////////////////////////////////////////////////////


//
//  Truncate minutes and seconds.
//

static CTime HourFloor(CTime t)
{
	return CTime( t.GetYear(),
				  t.GetMonth(),
				  t.GetDay(),
				  t.GetHour(),
				  0,
				  0);
}


//
// Get the next hour after this time
//

static CTime HourCeiling(CTime t)
{
	if ( t.GetMinute() == 0 && t.GetSecond() == 0 )
		return t;
	else
		return HourFloor(t) + CTimeSpan(0,1,0,0);
}


//
// Given an interval, expand to nearest whole hours on each end.
//

void CHMLoadGraphDoc::ComputeTimeRange(CTime& et, CTime& lt)
{
	et = TimeGrainFloor(ComputeEarliestTime());
	lt = TimeGrainCeiling(ComputeLatestTime());

}


//
//  I don't remember why I made what are essentially synonyms.
//

CTime CHMLoadGraphDoc::TimeGrainFloor(CTime t)
{
	return HourFloor(t);
}


CTime CHMLoadGraphDoc::TimeGrainCeiling(CTime t)
{
	return HourCeiling(t);
}


// 
//  Earliest/latest time overall is the min/max 
//    of earliest/latest times on all casters.
//

CTime CHMLoadGraphDoc::ComputeEarliestTime()
{
	return min(ComputeEarliestTime(1),	
				min(ComputeEarliestTime(2),
					ComputeEarliestTime(3)));
}

CTime CHMLoadGraphDoc::ComputeLatestTime()
{
	return max(ComputeLatestTime(1),	
				max(ComputeLatestTime(2),
					ComputeLatestTime(3)));
}



//
//  Compute the minimum valid start time.
//  (If no valid start times, use NOW).
//

CTime CHMLoadGraphDoc::ComputeEarliestTime(int caster)
{
	vector<CCastString*>& strings = GetStrings(caster);

	bool valid = false;
	CTime time(CTime::GetCurrentTime());

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {
		if ( (*is)->IsStartTimeValid() ) {

			if ( ! valid ) {
				time = (*is)->CastStartTime();
				valid = true;
			}
			else
				time = min(time,(*is)->CastStartTime());
		}
	}

	return time;
}


//
//  Compute the maximum valid finish time (= start time + duration)
//  If none, use the earliest time

CTime CHMLoadGraphDoc::ComputeLatestTime(int caster)
{
	vector<CCastString*>& strings = GetStrings(caster);

	bool valid = false;
	CTime time = ComputeEarliestTime(caster);

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {
		if ( (*is)->IsStartTimeValid() ) {
			if ( ! valid ) {
				time = (*is)->CastStartTime() + (*is)->TotalDuration();
				valid = true;
			}
			else
				time = max(time,(*is)->CastStartTime() + (*is)->TotalDuration());
		}
	}

	return time;
}



///////////////////////////////////////////////////////
//
//  Computing the load graph
//
///////////////////////////////////////////////////////


//
//  The loads need to be recomputed after every start time or sequencing change
//
//  Compute the loads on each caster, then the cumulative load across all casters
//

void CHMLoadGraphDoc::ComputeLoad()
{
	ComputeLoad(1);
	ComputeLoad(2);
	ComputeLoad(3);

	ComputeCumulativeLoad();
}


//
//  Get rid of any strings with bad start times.
//  Add a load record for each heat in each string in order by time.
//

void CHMLoadGraphDoc::ComputeLoad(int caster)
{
	m_loads[caster].clear();

	vector<CCastString*> strings = GetStrings(caster);	// copy

	strings.erase(remove_if(strings.begin(),strings.end(),CCastString::CFilterInvalidStartTimes()),
				  strings.end());

	stable_sort(strings.begin(),strings.end(),CCastString::CStartTimeCmp());

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {
		
		if ( ! (*is)->IsStartTimeValid() )
			continue;

		for ( vector<CCastStringHeat>::iterator ih = (*is)->Heats().begin();
			  ih != (*is)->Heats().end();
			  ++ih ) {

			 AddLoad(caster,
					(*is)->HotMetalTonsPerHeat(),
					(*is)->CastStartTime() + (CTimeSpan((*ih).RelStartTime()) - (*is)->PourToOpenTime()));

			  }
	}

#if 0
	{

		for ( vector<CHMLoadRecord>::iterator ir = m_loads[caster].begin();
			  ir != m_loads[caster].end();
			  ++ir ) {
			CString strTime;
			strTime = (*ir).m_time.Format( "%X %x");
			afxDump << "Caster #" << caster << ": "
					<< LPCTSTR(strTime) << " = " 
					<< (*ir).m_tons
					<< "\n";
		}
	}
#endif
}


//
//  Add a load record for a heat.
//  Since the heats are done in time sequence,
//   we need only look at the last load record.
//  If the hour on the load record is the same as the hour of the start-pour time 
//    of the heat, then update that record, else add a new record.
//

void CHMLoadGraphDoc::AddLoad(int caster,
							  Weight tons,
							  CTime t)
{
	CTime h = HourFloor(t);

	if ( m_loads[caster].size() != 0 
		 &&
		 m_loads[caster].back().m_time == h )
		m_loads[caster].back().m_tons += tons;
	else
		m_loads[caster].push_back( CHMLoadRecord(h,tons) );
}


//
//  Computing the cumulative load involves a tricky four way iteration.
//  Three iterations are across the caster load vectors,
//   the fourth is across the hot metal supply records.
//  The iterations begin with the first record in each sequence
//   that is after the last load count time.
//  The last load count gives the initial value of tons of hot metal available.
//
//  The iteration proceeds until all iterators are at the end.
//  If an iterator is at the end, the assumed value is 0.
//  We are computing the cumulative load without time gaps.
//  Thus, on each iteration, we advance by one hour.
//  Because the load records have time gaps, on a given iteration a given record
//    may not match the time we are computing.  The assumed value is 0.
//  When advancing the iterators, only the iterators at that time need to be advanced.

//
//  We define a bunch of macros to help encode the iterator tests.
//  I hope this helps make the code clearer.
//

#define at_end(i) (iters[i] == m_loads[i].end())
#define maybe_advance(i,t) if ( ! at_end(i) && (*(iters[i])).m_time <= (t) ) ++(iters[i])
#define safe_value(i,t) ( ( at_end(i) || (*(iters[i])).m_time != (t) ) ? 0.0 : (*(iters[i])).m_tons )

#define supply_at_end (isupply == supplyRecords.end())
#define supply_maybe_advance(t) if ( ! supply_at_end && (*isupply).GetDate() <= (t) ) ++isupply
#define supply_safe_value(t) ( ( supply_at_end || (*isupply).GetDate() != (t) ) ? 0.0 : (*isupply).GetTonsPerHour() )
		
void CHMLoadGraphDoc::ComputeCumulativeLoad()
{
	m_loads[0].clear();

	CHMSupplyMgr& supplyMgr = TheSnapshot.HMSupplyMgr();

	CTime lastTime = HourFloor(supplyMgr.LastLCTime());
	Weight tons = supplyMgr.TonsPerPugh() * supplyMgr.LastLoadCount();

	m_loads[0].push_back( CHMLoadRecord(lastTime,tons) );

	vector<CHMLoadRecord>::iterator iters[4];
	{
		for ( int i=1; i<= 3; ++i ) {
			
			for ( iters[i] = m_loads[i].begin();
				  iters[i] != m_loads[i].end();
				  ++(iters[i]) )
				if ( (*iters[i]).m_time >= lastTime )
					break;
		}
	}

	vector<CHMSupplyRecord>& supplyRecords = GetSupplyRecords();
	vector<CHMSupplyRecord>::iterator isupply = supplyRecords.begin();

	for ( ; isupply != supplyRecords.end(); ++isupply ) {
		if ( (*isupply).GetDate() >= lastTime )
			break;
	}

	while ( !( at_end(1) && at_end(2) && at_end(3) && supply_at_end ) ) {

		if ( m_loads[0].size() > 10000 )
			break;

		
		Weight delta = supply_safe_value(lastTime)
					 	 - ( safe_value(1,lastTime)
							 + safe_value(2,lastTime)
							 + safe_value(3,lastTime) );

		lastTime += CTimeSpan(0,1,0,0);
		tons += delta;

		m_loads[0].push_back( CHMLoadRecord(lastTime,tons) );

		maybe_advance(1,lastTime);
		maybe_advance(2,lastTime);
		maybe_advance(3,lastTime);
		supply_maybe_advance(lastTime);



	}


#if 0
	{
		vector<CHMSupplyRecord>& supplyRecords = GetSupplyRecords();
		vector<CHMSupplyRecord>::iterator isupply = supplyRecords.begin();

		for ( ; isupply != supplyRecords.end(); ++isupply ) {
			CString strTime;
			strTime = (*isupply).GetDate().Format("%X %x");
			afxDump << "Supply: "
					<< LPCTSTR(strTime) << " = "
					<< (*isupply).GetTonsPerHour()
					<< "\n";
		}

		int caster = 0;
		for ( vector<CHMLoadRecord>::iterator ir = m_loads[caster].begin();
			  ir != m_loads[caster].end();
			  ++ir ) {
			CString strTime;
			strTime = (*ir).m_time.Format( "%X %x");
			afxDump << "Cumulative: "
					<< LPCTSTR(strTime) << " = " 
					<< (*ir).m_tons
					<< "\n";
		}
	}
#endif
}





#if 0 
// I think this is no longer needed
// originally used by the now-defunct CHMLoadScenMgrView
void CHMLoadGraphDoc::ChangeCasterScen(int whichCaster, CCasterScen* pScen)
{
	//  We better check to make sure this is okay
	//  We are only going to switch displayed scenarios
	//     if the caster on which the selected scenario is changing
	//     is the same as the caster currently begin displayed.

	if ( whichCaster == GetCasterNum() 
		 &&
		 GetCurrentCasterScen(whichCaster) == pScen )
		 // nochange
		 return;

	GetScenMru().PromoteCasterScen(pScen);

	//  Any update will happen indirectly via our CScenMruListerner.
}
#endif

