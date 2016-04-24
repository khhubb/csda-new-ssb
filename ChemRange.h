
#if !defined(CHEMRANGE_H__INCLUDED_)
#define CHEMRANGE_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "Chem.h"

class CChemRange
{

	////////////////////////////////////////////////////////////////
	//	
	//	Each spec carries one of these for each of the elements.
	//	Encodes min/aim/max for a chemistry, plus a flag for appSpecific.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:

	CChem::Element elt;
	CChem::Bound minVal;
	CChem::Bound maxVal;
	CChem::Bound aimVal;
	bool isAppSpecific;



	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CChem::Element Element() const 
	{ return elt; }

	CChem::Bound MinVal() const
	{ return minVal; }

	CChem::Bound MaxVal() const
	{ return maxVal; }

	CChem::Bound AimVal() const
	{ return aimVal; }

	bool IsAppSpecific() const
	{ return isAppSpecific; }



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CChemRange( CChem::Element elt_,
				CChem::Bound min_,
				CChem::Bound aim_,
				CChem::Bound max_,
				bool isAppSpecific_ = false )
	:	elt(elt_),
		minVal(min_),
		aimVal(aim_),
		maxVal(max_),
		isAppSpecific(isAppSpecific_)
	{}


	// default c-tor required by vector

	CChemRange()
		: elt(CChem::ELT_C),
		  minVal(0.0),
		  maxVal(25.0),
		  aimVal(1.0),
		  isAppSpecific(false)
	  {}

	// default d-tor, copy c-tor okay


	////////////////////////////////////////////////////////////////
	//	
	//	operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CChemRange& x) const
	{
		return elt < x.elt;
	}

	bool operator==(const CChemRange& x) const
	{
		return elt == x.elt;
	}

};



#endif  // _INCLUDED_