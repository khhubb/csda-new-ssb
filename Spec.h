// Spec.h: interface for the CSpec class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_SPEC_H__E360706B_C71F_11D0_8013_006097B38214__INCLUDED_)
#define AFX_SPEC_H__E360706B_C71F_11D0_8013_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "modelTypes.h"
#include "Chem.h"
#include "ChemRange.h"
#include "Caster.h"


class CSpecDPASet;


class CSpec  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Represents a spec.
	//	
	////////////////////////////////////////////////////////////////

	
	friend class CSpecMgr;


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_PdtType { SLAB, BLOOM };



	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:
	
	bool m_forCaster[Caster::CasterArrayLen]; // caster = 1,2,3,4,5
		// flags indicating which casters the spec if for
	bool m_isBloomSpec;
		// flag indicating this is a bloom spec
	
	CString m_name;
		// the name of the spec XXX-XX-XX

	vector<CChemRange> m_dpaChems;
		// Chemistries for the spec coming from the DPA system.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	bool ForCaster(int caster) const
	{
		assert( Caster::IsValidCasterValue(caster));
		return m_forCaster[caster];
	}

	const CString& Name() const
	{	return m_name;	}

	const vector<CChemRange>& DPAChems() const
	{	return m_dpaChems; }



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CSpec(CString name);

	CSpec(const CSpec& x);

	CSpec::CSpec(const CSpecDPASet& dpaSpec);

	~CSpec() 
	{ 
		// do nothing 
	}



	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	const CChemRange* GetChemRange(CChem::Element elt) const;

	CChem::Element FindSignifEl() const;

	
	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CSpec& x) const
	{
		return m_name < x.m_name;
	}
	
	bool operator==(const CSpec& x) const
	{
		return m_name == x.m_name;
	}


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:


};



#endif // !defined(AFX_SPEC_H__E360706B_C71F_11D0_8013_006097B38214__INCLUDED_)
