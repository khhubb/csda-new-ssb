// HMSupplyMgr.h: interface for the CHMSupplyMgr class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_HMSUPPLYMGR_H__3D844ABF_27B3_11D2_8097_006097B38214__INCLUDED_)
#define AFX_HMSUPPLYMGR_H__3D844ABF_27B3_11D2_8097_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CHMSupplyActual;
class CHMSupplyProjected;

enum E_BlastFce { BF_5 = 5, BF_6, BF_7 };

class CHMSupplyRecord
{
	////////////////////////////////////////////////////////////////
	//	
	//	Represents data for one hour of hot metal supply, all blast furnaces
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CTime	m_date;
		// the hour for which this applies

	float	m_tonsPerHour[3];  // 0 => BF_5, 1 => BF_6, 2 => BF_7
		// the projected or actual tons per hour for that our for each furnace


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CTime GetDate() const 
	{	return m_date;	}

	float GetTonsPerHour(E_BlastFce f) const
	{
		assert ( BF_5 <= f && f <= BF_7 );
		return m_tonsPerHour[f - BF_5];
	}

	float GetTonsPerHour() const
	{
		return m_tonsPerHour[0] + m_tonsPerHour[1] + m_tonsPerHour[2];
	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CHMSupplyRecord(CTime date, 
		            float rate5, 
					float rate6, 
					float rate7)
					: m_date(date)
	{	
		m_tonsPerHour[0] = rate5;
		m_tonsPerHour[1] = rate6;
		m_tonsPerHour[2] = rate7;
	}

	CHMSupplyRecord( CHMSupplyProjected& dbrec );
	CHMSupplyRecord( CHMSupplyActual& dbrec );


	~CHMSupplyRecord() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	// sort by date
	bool operator<(const CHMSupplyRecord& x) const
	{
		return m_date < x.m_date;
	}

	bool operator==(const CHMSupplyRecord& x) const
	{
		return m_date == x.m_date;
	}	
	

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

};




class CHMSupplyMgr  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Manages a set of CHMSupplyRecord records
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	vector<CHMSupplyRecord> m_records;
		// all the supply records

	CTime m_lastLCTime;
		// the time the last load count is for

	long m_lastLoadCount;
		// the last load count -- how many pughs of material is ready
		//    at the last time at which we have a count


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	vector<CHMSupplyRecord>& GetRecords() { return m_records; }

	CTime LastLCTime() const { return m_lastLCTime; }

	long LastLoadCount() const { return m_lastLoadCount; }

	static double TonsPerPugh() { return 178.0; }
		// fixed calculation

	float GetTonsPerHour(CTime time) const;


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CHMSupplyMgr() 
		: m_lastLCTime(CTime::GetCurrentTime()), 
		  m_lastLoadCount(0) 
	
	{  }

	~CHMSupplyMgr() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	bool Initialize();

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////


};

#endif // !defined(AFX_HMSUPPLYMGR_H__3D844ABF_27B3_11D2_8097_006097B38214__INCLUDED_)
