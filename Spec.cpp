// Spec.cpp: implementation of the CSpec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "Spec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "SpecDPASet.h"



//////////////////////////////////////////////////////////////////////
// 
//	CSpec -- ctors, d-tor
//
//////////////////////////////////////////////////////////////////////


CSpec::CSpec(CString name_)
: m_name(name_)
{
	for (int i = Caster::FirstCaster; i<=Caster::LastCaster; ++i)
		m_forCaster[i] = false;

	m_isBloomSpec = false;
}



CSpec::CSpec(const CSpec& x)
: m_name(x.m_name)
{	
	m_dpaChems = x.m_dpaChems;
	
	for (int i = Caster::FirstCaster; i <= Caster::LastCaster; ++i)
		m_forCaster[i] = x.m_forCaster[i];

	m_isBloomSpec = x.m_isBloomSpec;
}


CSpec::CSpec(const CSpecDPASet& dpaSpec)
{
	m_name = dpaSpec.m_SPEC_NAME;

	m_forCaster[Caster::C1] = (dpaSpec.m_CASTER_INDICATOR.Find('1') != -1);
	m_forCaster[Caster::C2] = (dpaSpec.m_CASTER_INDICATOR.Find('2') != -1);
	m_forCaster[Caster::C3] = (dpaSpec.m_CASTER_INDICATOR.Find('3') != -1);
	m_forCaster[Caster::C4] = (dpaSpec.m_CASTER_INDICATOR.Find('4') != -1);
	m_forCaster[Caster::C5] = (dpaSpec.m_CASTER_INDICATOR.Find('5') != -1);
	m_isBloomSpec = (dpaSpec.m_CASTER_INDICATOR.Find('B') != -1);
}



const CChemRange* CSpec::GetChemRange(CChem::Element elt) const
{
	for ( vector<CChemRange>::const_iterator ic = m_dpaChems.begin();
	      ic != m_dpaChems.end();
		  ++ic )
	  if ( (*ic).Element() == elt )
		  return &(*ic);

	return 0;
}



CChem::Element CSpec::FindSignifEl() const
{
	// find the first non-C, non-Mn element which is app-specific (open range)

	for ( vector<CChemRange>::const_iterator ic = m_dpaChems.begin();
	      ic != m_dpaChems.end();
		  ++ic ) {
		if ( (*ic).IsAppSpecific() 
			  &&
			  (*ic).Element() != CChem::ELT_C
			  &&
			  (*ic).Element() != CChem::ELT_MN ) {

			return (*ic).Element();
		}
	}

	return CChem::Element(-1);
}



