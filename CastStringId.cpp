/////////////////////////////////////////////////////////////
//
//  CCastStringId
//
/////////////////////////////////////////////////////////////


#include "StdAfx.h" 


#include "CastStringId.h"

#include "DalyPlanSet.h"
#include "StringBuilderOutputSet.h"


CCastStringId::CCastStringId()
: m_year(0), 
  m_week(0), 
  m_caster(0), 
  m_stringId(0)
{
}



CCastStringId::CCastStringId(int year,
						   int week,
						   int caster,
						   int stringId)
: m_year(year),
  m_week(week),
  m_caster(caster),
  m_stringId(stringId)
{
}


CCastStringId::CCastStringId(const CDalyPlanSet& planSet)
{
	// m_PLAN_WEEK is YYYYWW
	m_year		= planSet.m_PLAN_WEEK/100%10000;	// YYYYww
	m_week		= planSet.m_PLAN_WEEK%100;			// yyyyWW
	m_caster	= atoi(planSet.m_PLAN_CAST_UNIT_CODE);  
	m_stringId	= planSet.m_PLAN_STRING_ID;  
}

CCastStringId::CCastStringId(const CStringBuilderOutputSet& sbSet)
{
	// m_PLAN_WEEK is YYYYWW
	m_year		= sbSet.m_PLAN_WEEK/100%10000;	// YYYYww
	m_week		= sbSet.m_PLAN_WEEK%100;			// yyyyWW
	m_caster	= atoi(sbSet.m_PLAN_CAST_UNIT_CODE);  
	m_stringId	= sbSet.m_PLAN_STRING_ID;  
}



bool CCastStringId::operator<(const CCastStringId& x) const
{
	//  usually we wont' be comparing CastStrings from different
	//   casters, but we put the code here anyway

	if ( m_caster < x.m_caster ) return true;
	if ( m_caster > x.m_caster ) return false;

	// m_caster == x.m_caster

	if ( m_year < x.m_year ) return true;
	if ( m_year > x.m_year ) return false;

	// m_year == x.m_year

	if ( m_week < x.m_week ) return true;
	if ( m_week > x.m_week ) return false;

	// m_week == x.m_week
			
	return m_stringId < x.m_stringId;
}


bool CCastStringId::operator==(const CCastStringId& x) const
{
	return m_caster == x.m_caster
		   &&
		   m_year == x.m_year
		   &&
		   m_week == x.m_week
		   &&
		   m_stringId == x.m_stringId;
}


bool CCastStringId::operator!=(const CCastStringId& x) const
{
	return ! ( *this == x );
}
