// HMSupplyMgr.cpp: implementation of the CHMSupplyMgr class.
//  Hot Metal Connection
//  
//  Maint:
//
//  k. hubbard  5-12-05	 For SQL conversion changed file access from fowardOnly and append
//                       to dynaset and none in order to avoid runtime hstmt error.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "HMSupplyMgr.h"

#include "DBConn.h"
#include "HMSupplyActual.h"
#include "HMSupplyProjected.h"
#include "HMLastLoadCountSet.h"
#include "nTime.h"
#include "NOutputWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////
//
//  CHMSupplyMgr & CHMSupplyRecord
//
//  CHMSupplyMgr manages a set of CHMSupplyRecords giving the 
//   projected or actual tons per hour on an hourly basis.
//
//  Initial data for this comes from the Blast Furnace spreadsheet,
//    dumped to the hotmetal database
//
//////////////////////////////////////////////////////////////////


//
//  Construct for database record.
//

CHMSupplyRecord::CHMSupplyRecord( CHMSupplyActual& dbrec )
{
	m_date = dbrec.m_Date;
	m_tonsPerHour[0] = dbrec.m_5_BOF_tons_hr;
	m_tonsPerHour[1] = dbrec.m_6_BOF_tons_hr;
	m_tonsPerHour[2] = dbrec.m_7_BOF_tons_hr;
}


//
//  Construct for database record.
//

CHMSupplyRecord::CHMSupplyRecord( CHMSupplyProjected& dbrec )
{
	m_date = dbrec.m_Date;
	m_tonsPerHour[0] = dbrec.m_5_BOF_tons_hr;
	m_tonsPerHour[1] = dbrec.m_6_BOF_tons_hr;
	m_tonsPerHour[2] = dbrec.m_7_BOF_tons_hr;
}



//
//  Initialize a CHMSupplyMgr by reading data from the hotmetal database.
//

bool CHMSupplyMgr::Initialize()
{
	NOutputWnd::PostLoadMsg("Initializing hot metal supply data ... ");

	CDBConn dbc(CDBConn::DB_HM);
	dbc.Connect();

	bool retval = true;

	CHMLastLoadCountSet lcRS(dbc.GetDB());
	CHMSupplyProjected projRS(dbc.GetDB());
	CHMSupplyActual actualRS(dbc.GetDB());

	try
	{	
		//  get the last load count and its time from the table.
		//  For a default, use 2 days before today.

		m_lastLCTime = CTime(NTime::ThisDayMidnight(CTime::GetCurrentTime().GetTime()));
		m_lastLCTime -= CTimeSpan(2,0,0,0);
		m_lastLoadCount = 0;

//		lcRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly|CRecordset::executeDirect);  // 5-12-05	k. hubbard SQL conversion to avoid runtime hstmt error
//		lcRS.Open(CRecordset::dynaset,0,CRecordset::none);
		lcRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);  
		if ( ! lcRS.IsEOF() ) {
			if ( lcRS.m_Date > m_lastLCTime ) {
				m_lastLCTime = lcRS.m_Date;
				m_lastLoadCount = lcRS.m_Actual_L_C;
			}
		}

		lcRS.Close();


		//  We need only look at records that follow the last load count time

//	k. hubbard SQL conversion does not allow use of # signs on dates or times: CString strAfter = m_lastLCTime.Format("Date >= #%m/%d/%Y %I:%M:00 %p#");

 		CString strAfter = m_lastLCTime.Format("Date >= '%m/%d/%Y %I:%M:00 %p'");

		projRS.m_strFilter = strAfter;
		actualRS.m_strFilter = strAfter;

//		actualRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);    5-12-05 k. hubbard comm. out and changed to lines below for SQL conversion to avoid runtime hstmt error
//		projRS.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);      5-12-05 k. hubbard comm. out and changed to lines below for SQL conversion to avoid runtime hstmt error
		actualRS.Open(CRecordset::dynaset,0,CRecordset::none);
		projRS.Open(CRecordset::dynaset,0,CRecordset::none);


		// Actual records trump projected
		// Do a bi-iteration through both record sets,
		// Create a record for the earlier record.
		// If two records have the same date, use the actual.

		m_records.clear();

		while ( ! projRS.IsEOF() && ! actualRS.IsEOF() ) {

			if ( projRS.m_Date == actualRS.m_Date ) {
				m_records.push_back( CHMSupplyRecord(actualRS) );
				projRS.MoveNext();
				actualRS.MoveNext();
			}
			else if ( projRS.m_Date < actualRS.m_Date) {
				m_records.push_back( CHMSupplyRecord( projRS ) );
				projRS.MoveNext();
			}
			else {
				m_records.push_back( CHMSupplyRecord(actualRS) );
				actualRS.MoveNext();
			}

		}

		//  Finish off any trailing records

		while ( ! projRS.IsEOF() ) {
			m_records.push_back( CHMSupplyRecord( projRS ) );
			projRS.MoveNext();
		}

		while ( ! actualRS.IsEOF() ) {
			m_records.push_back( CHMSupplyRecord(actualRS) );
			actualRS.MoveNext();
		}

	}
	catch ( CDBException* e )
	{
		AfxMessageBox( e->m_strError,   
			MB_ICONEXCLAMATION );
		e->Delete();
		retval = false;
	}

	if ( projRS.IsOpen() )
		projRS.Close();

	if ( actualRS.IsOpen() )
		actualRS.Close();

	NOutputWnd::PostLoadMsg("done.\n");

	return retval;
}



//
//  Find the tons/hour for a given hour.
//  Requires iterating to find the record,
//   or the first record bigger than the desired time.

float CHMSupplyMgr::GetTonsPerHour(CTime time) const
{
	for ( vector<CHMSupplyRecord>::const_iterator ir = m_records.begin();
		  ir != m_records.end();
		  ++ir ) {

		if ( (*ir).GetDate() == time )
			return (*ir).GetTonsPerHour();
		else if ( (*ir).GetDate() > time )
			break;
	}

	return 0.0;
}
