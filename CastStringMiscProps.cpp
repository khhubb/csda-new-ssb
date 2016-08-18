////////////////////////////////////////////////////////////////
//	
//	class CCastStringMiscProps
//	
////////////////////////////////////////////////////////////////


#include "StdAfx.h"


#include "CastStringMiscProps.h"

#include "MiscConstants.h"
#include "Snapshot.h"


////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////


//
//  Default values for some properties come from the global constants,
//    as you can see.
//

CCastStringMiscProps::CCastStringMiscProps(int h,
										   int casterNum)  //### caster-specific
		: m_numHeats(h), 
		  m_minHeatSize(GlobalConstants.MinHeatSize(casterNum)), //### caster-specific
		  m_aimHeatSize(GlobalConstants.AimHeatSize(casterNum)), //### caster-specific
		  m_maxHeatSize(GlobalConstants.MaxHeatSize(casterNum)), //### caster-specific
		  m_turnaroundTime(GlobalConstants.TurnaroundTime(casterNum)), //### caster-specific
		  m_initSpec(""),
		  m_pInitSpec(0),
		  m_minC(0.0),
		  m_maxC(0.0),
		  m_minMn(0.0),
		  m_maxMn(0.0),
		  m_startTime(CTime::GetCurrentTime()),
		  m_isStartTimeValid (false)
{
	for ( int i=1; i<=2; ++i ) { // strands
		m_startWidths[i] = 0;
		m_endWidths[i] = 0;
	}
}




CCastStringMiscProps::CCastStringMiscProps(int h, 
										   int minSize,
										   int aimSize,
										   int maxSize,
										   CTimeSpan turnaroundTime)
		: m_numHeats(h), 
		  m_minHeatSize(minSize),
		  m_aimHeatSize(aimSize),
		  m_maxHeatSize(maxSize),
		  m_turnaroundTime(turnaroundTime),
		  m_initSpec(""),
		  m_pInitSpec(0),
		  m_minC(0.0),
		  m_maxC(0.0),
		  m_minMn(0.0),
		  m_maxMn(0.0),
		  m_startTime(CTime::GetCurrentTime()),
		  m_isStartTimeValid (false)
{
	for ( int i=1; i<=2; ++i ) { // strands
		m_startWidths[i] = 0;
		m_endWidths[i] = 0;
	}
}




void CCastStringMiscProps::SetInitSpec(const CString& newVal,int caster) //### caster-specific
{
	static CString zeros("0000000");

	m_initSpec  = newVal;

	if ( m_initSpec == "" )
		m_pInitSpec = 0;
	else {
		if ( m_initSpec.GetLength() < 7 )
			m_initSpec = (m_initSpec + "XXXXXXX").Left(7);
		else if ( m_initSpec.GetLength() > 7 )
			m_initSpec = m_initSpec.Left(7);

		CString lookup = m_initSpec;
		if ( lookup[6] != '0' && lookup[6] != 5 )
			lookup.SetAt(6,'0');
		if ( lookup[5] != '0' && lookup[5] != '4' && lookup[5] != '9' )
			lookup.SetAt(5,'0');
		if ( lookup[3] != '0' 
			 && lookup[3] != '2' 
			 && lookup[3] != '4' 
			 && lookup[3] != '7' )
			 lookup.SetAt(3,'0');

		m_pInitSpec = TheSnapshot.SpecMgr().FindSpecMaybe(lookup,caster); //### caster-specific
	}
}
