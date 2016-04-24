// SpecChemRangePreset.h: interface for the CSpecChemRangePreset class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_SPECCHEMRANGEPRESET_H__25484152_66DF_11D1_8055_006097B38214__INCLUDED_)
#define AFX_SPECCHEMRANGEPRESET_H__25484152_66DF_11D1_8055_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Chem.h"

class CSpecChemRangePreset  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Holds a set of of min/max carbon and manganese values for a spec.
	//  Used to provide some standard preset ranges for certain specs.
	//  Initialized from a database table containing one record per instance
	//    of this class.
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

	CString m_spec;
	CChem::Bound m_minC;
	CChem::Bound m_maxC;
	CChem::Bound m_minMn;
	CChem::Bound m_maxMn;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	const CString& Spec() const
	{	return m_spec;	}

	CChem::Bound MinC() const
	{	return m_minC;	}

	CChem::Bound MaxC() const
	{	return m_maxC;	}

	CChem::Bound MinMn() const
	{	return m_minMn;	}

	CChem::Bound MaxMn() const
	{	return m_maxMn;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CSpecChemRangePreset()
		: m_minC (   0.0 ),
		  m_maxC ( 100.0 ),
		  m_minMn(   0.0 ),
		  m_maxMn( 100.0 )
	{}

	CSpecChemRangePreset(CString& spec,
						 CChem::Bound minC,
						 CChem::Bound maxC,
						 CChem::Bound minMn,
						 CChem::Bound maxMn)
	: m_spec(spec),
	  m_minC(minC),
	  m_maxC(maxC),
	  m_minMn(minMn),
	  m_maxMn(maxMn)
	{}

	~CSpecChemRangePreset() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CSpecChemRangePreset& x) const
	{
		return m_spec < x.m_spec
			   ||
			   ( m_spec == x.m_spec 
				 &&
				 (m_minC < x.m_minC
				  ||
				  (m_minC == x.m_minC
				   &&
				   m_minMn < x.m_minMn)));
	}

	bool operator==(const CSpecChemRangePreset& x) const
	{
		return m_spec == x.m_spec 
				 &&
				 m_minC == x.m_minC
				 &&
				 m_minMn == x.m_minMn;
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

#endif // !defined(AFX_SPECCHEMRANGEPRESET_H__25484152_66DF_11D1_8055_006097B38214__INCLUDED_)
