// Week.cpp: implementation of the CWeek class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "Week.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "nTime.h"


////////////////////////////////////////////////////////////////
//	
//	Accessors/modifiers
//	
////////////////////////////////////////////////////////////////


void CWeek::Set(int year, int week)
{
//	assert( 0 <= week && week <= 53 );
//	assert( 1000 <= year && year <= 9999 ); // Y10K problem

	m_year = year;
	m_week = week;
}




////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////

CWeek::CWeek(CTime time)
{
	NTime::WeekOfYear(time,m_week,m_year);
}

CWeek::CWeek()
{
	NTime::WeekOfYear(CTime::GetCurrentTime(),m_week,m_year);
}

////////////////////////////////////////////////////////////////
//	
//	Operators
//	
////////////////////////////////////////////////////////////////


bool CWeek::operator<(const CWeek& x) const
{
	return m_year < x.m_year
		   ||
		   m_year == x.m_year && m_week < x.m_week;
}


bool CWeek::operator<=(const CWeek& x) const
{
	return m_year <= x.m_year
		   ||
		   m_year == x.m_year && m_week <= x.m_week;
}


bool CWeek::operator>(const CWeek& x) const
{
	return x < *this;
}

bool CWeek::operator>=(const CWeek& x) const
{
	return x <= *this;
}


bool CWeek::operator==(const CWeek& x) const
{
	return m_year == x.m_year && m_week == x.m_week;
}


// TODO -- make this right

int CWeek::operator-(const CWeek& x) const
{
	return 52*(m_year - x.m_year) + m_week - x.m_week;
}		

