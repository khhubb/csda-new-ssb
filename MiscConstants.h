// MiscConstants.h: interface for the CMiscConstants class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISCCONSTANTS_H__642EE487_E2B3_11D0_8019_006097B38214__INCLUDED_)
#define AFX_MISCCONSTANTS_H__642EE487_E2B3_11D0_8019_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "modelTypes.h"
#include "Caster.h"
 
class CMiscConstants  
{

	////////////////////////////////////////////////////////////////
	//	
	//	Various constants that don't fit anywhere else
	//  The intention that there is to be one copy of these.
	//  The values are archived in csdaTest db. 
	//	
	////////////////////////////////////////////////////////////////

	

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	int m_minHeatSizes[Caster::CasterArrayLen];  
	int m_aimHeatSizes[Caster::CasterArrayLen];
	int m_maxHeatSizes[Caster::CasterArrayLen];
		//  The size of a heat, in tons. min/aim/max
		//  on a per-caster basis, so index = 1,2,3,4,5

	CTimeSpan m_turnaroundTimes[Caster::CasterArrayLen];
		//  Standard time between strings
		//  on a per-caster basis, so index = 1,2,3,4,5

	bool m_initialized;
		// Shouldn't try to access the data unless initialized

	bool m_modified;
		// no point saving if nothing has changed.

	CString m_loginName;
		// Who this is for.

	static bool m_oneExists;
		// don't create more than one of these.

	static double m_steelDensity;
	static Length m_casterSlabLengthMaxes[Caster::CasterArrayLen];
	static Length m_max80HsmRunoutLength;

	static Length m_minCastableSlabLengths[Caster::CasterArrayLen];
		// minimum slab length
		//  on a per-caster basis, so index = 1,2,3,4,5


	static int	m_archiveVersionRetentionCount;

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:
	
	int MinHeatSize(int caster) const;
	int AimHeatSize(int caster) const;
	int MaxHeatSize(int caster) const;
	void SetMinHeatSize(int caster, int size);
	void SetAimHeatSize(int caster, int size);
	void SetMaxHeatSize(int caster, int size);

	CTimeSpan TurnaroundTime(int caster) const;
	
	static double SteelDensity() 
	{	return m_steelDensity;	}

	static Length CasterSlabLengthMax(int caster) 
	{	return m_casterSlabLengthMaxes[caster];	}

	static Length Max80HsmRunoutLength()
	{	return m_max80HsmRunoutLength;	}

	static Length MinCastableSlabLength(int caster)
	{	return m_minCastableSlabLengths[caster];	}

	static int ArchiveVersionRetentionCount()
	{	return m_archiveVersionRetentionCount;	}


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void Init(CString loginName);
	void Save();


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tors	
	//	
	////////////////////////////////////////////////////////////////

public:

	CMiscConstants();
	virtual ~CMiscConstants();

	// supplied copy c-tor is fine.



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

	void ValidateCaster(int caster) const 
	{  	assert( Caster::IsValidCasterValue(caster) ); }

};


extern CMiscConstants GlobalConstants;


#endif // !defined(AFX_MISCCONSTANTS_H__642EE487_E2B3_11D0_8019_006097B38214__INCLUDED_)
