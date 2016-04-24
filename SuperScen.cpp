// SuperScen.cpp: implementation of the CSuperScen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "SuperScen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CSOrder.h"
#include "CastString.h"
#include "DalyPlanSet.h"
#include "CasterScen.h"
#include "ProdnScen.h"
#include "DBConn.h"
#include "ScenMgr.h"
#include "DailyPlanLastDownloadTimeSet.h"
#include "CsdaEventLogSet.h"
#include "DBConn.h"
#include "nTime.h"
#include "CasterScenArchiver.h"
#include "NOutputWnd.h"


////////////////////////////////////////////////////////////////
//	
//	Accessors
//	
////////////////////////////////////////////////////////////////

// most are inline


CCasterScen* CSuperScen::CasterScen(int i) const
{
	assert( 1 <= i && i <= 3 );

	return m_pCScens[i];
}



int CSuperScen::ProdnScenIndex() const
{	
	CProdnScen* pScen = ProdnScen();

	TProdnScenVec::const_iterator ib = CScenMgr::ProdnScensBegin();
	TProdnScenVec::const_iterator ie = CScenMgr::ProdnScensEnd();

	TProdnScenVec::const_iterator is = find(ib,ie,pScen);
	
	assert ( is != ie );

	return is - ib;

}

int CSuperScen::CasterScenIndex(int i) const
{
	CCasterScen* pScen = CasterScen(i);

	TCasterScenVec::const_iterator ib = CScenMgr::CasterScensBegin(i);
	TCasterScenVec::const_iterator ie = CScenMgr::CasterScensEnd(i);

	TCasterScenVec::const_iterator is = find(ib,ie,pScen);
	
	assert ( is != ie );

	return is - ib;
}




////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////


CSuperScen::CSuperScen()
{
	for ( int i=0; i<4; ++i )
		m_pCScens[i] = 0;

	m_pProdnScen = 0;

	m_bForSnap = false;
}


CSuperScen::~CSuperScen()
{
}



////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////

//
//  Create a little identifying string
//

CString CSuperScen::IdString() const
{
	ostrstream ostr;
	ostr << "<SScen " 
		<< (m_pCScens[1] == 0 ? -1 : m_pCScens[1]->Id() )
		<< (m_pCScens[2] == 0 ? -1 : m_pCScens[2]->Id() )
		<< (m_pCScens[3] == 0 ? -1 : m_pCScens[3]->Id() )
		<< (m_pProdnScen == 0 ? -1 : m_pProdnScen->Id() )
		<< ">"
		<< ends;
	CString result(ostr.str());
	ostr.freeze(false);
	return result;
}


//
//  This is used by the snapshot to create the initial scenarios
//    from the download database.
//

bool CSuperScen::InitFromDalyPlan()
{
	NOutputWnd::PostLoadMsg("Loading daily plan ... ");

	assert( m_pCScens[1] == 0
			&&
			m_pCScens[2] == 0
			&& 
			m_pCScens[3] == 0
			&&
			m_pProdnScen == 0 );

	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CDalyPlanSet planSet(dbc.GetDB());

	NOutputWnd::PostLoadMsg(" 1 ... ");
	bool result1 = CreateAndLoadCasterScen(1,planSet);

	NOutputWnd::PostLoadMsg(" 2 ... ");
	bool result2 = CreateAndLoadCasterScen(2,planSet);

	NOutputWnd::PostLoadMsg(" 3 ... ");
	bool result3 = CreateAndLoadCasterScen(3,planSet);

	m_pProdnScen = CScenMgr::CreateProdnScen(false);

	planSet.Close();
	
	NOutputWnd::PostLoadMsg(" done.\n");

	return result1 && result2 && result3;	
}


bool CSuperScen::CreateAndLoadCasterScen(int casterNum,CDalyPlanSet& planSet)
{
	m_pCScens[casterNum] = CScenMgr::CreateCasterScen(casterNum,false);
	bool result = m_pCScens[casterNum]->LoadFromDalyPlan(planSet);
	m_pCScens[casterNum]->SetComment("DALYPLAN");

	return result;
}


//
//  Load any changes since the daily plan was downloaded.
//

bool CSuperScen::LoadDalyPlanUpdates()
{
	NOutputWnd::PostLoadMsg("Loading daily plan updates: " );

	bool retval = true;

	CTime lastTime = LastDailyPlanDownloadTime();

	vector<CCastStringId> ids;
	TheSnapshot.Last910RecordId() = CSnapshot::GetUpdated910Ids(0,lastTime,ids);
	TheSnapshot.Last910UpdateTime() = CTime::GetCurrentTime();

	long count = 0;

	for ( vector<CCastStringId>::iterator iid = ids.begin();
		  iid != ids.end();
		  ++iid ) {
		LoadUpdated910( (*iid) );
		++count;
		NOutputWnd::WriteProgress(count);
	}

	{
		ostrstream ostr;
		ostr << "read " << count << " records\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}
	
	return true;
}


void CSuperScen::LoadUpdated910( CCastStringId& id )
{
	int caster = id.Caster();

	CCasterScen* pScen = CasterScen(caster);
	
	CCastString* pString = pScen->FindCastString(id);
	if ( pString != 0 )
		pScen->DeleteString(pString);

	CCasterScenArchiver::LoadStringFromMasterScen(pScen,id);
}
	



//void CSuperScen::LoadString(int caster, int year, int week, int stringId)
//{
//
//}


//
//  The download database contains a table with one row/one field
//    that give the last download time.
//  

CTime CSuperScen::LastDailyPlanDownloadTime()
{
	CTime lastTime(NTime::ThisDayMidnight(CTime::GetCurrentTime().GetTime()));

	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CDailyPlanLastDownloadTimeSet rs(dbc.GetDB());

	try 
	{ 
		rs.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	
		if ( ! rs.IsEOF() )
			lastTime = rs.m_Last_Download;
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	}


	if ( rs.IsOpen() )
		rs.Close();

	return lastTime;
}



int CSuperScen::NumSlabsScheduled(const COrder* pOrder) const
{
	return m_pCScens[1]->NumSlabsScheduled(pOrder)
		 + m_pCScens[2]->NumSlabsScheduled(pOrder)
		 + m_pCScens[3]->NumSlabsScheduled(pOrder);
}


int CSuperScen::NumSlabsDue(const COrder* pOrder) const
{
	return pOrder->NumSlabsDue() - NumSlabsScheduled(pOrder);
}
