// Chem.cpp: implementation of the CChem class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"

#pragma warning(disable: 4786)


#include "Chem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////
//	
//	Static data member definitions
//	
////////////////////////////////////////////////////////////////

CChem::ElementToCStringMap CChem::e2sMap;
CChem::CStringToElementMap CChem::s2eMap;



//////////////////////////////////////////////////////////////////////
//
// Construction/Destruction
//
//////////////////////////////////////////////////////////////////////

//	Don't implement any c-tors
//	This is a static-only class.
//  We declare the default and copy c-tors 
//		so that the compiler won't supply them.




//////////////////////////////////////////////////////////////////////
//
//  Operations
//
//////////////////////////////////////////////////////////////////////



CChem::Element CChem::ToElement(const CString& str)
{
	InitializeMaps();

	CString strUp(str);
	strUp.MakeUpper();

	CStringToElementMap::iterator i = s2eMap.find(strUp);

	if ( i != s2eMap.end() )
		
		return Element((*i).second);
	else
		return ELT_S; // bogus, but didn't want to fail
}


// a somewhat safer version

bool CChem::ToElement(const CString& str, CChem::Element& elt)
{
	InitializeMaps();

	CString strUp(str);
	strUp.MakeUpper();

	CStringToElementMap::iterator i = s2eMap.find(strUp);

	if ( i == s2eMap.end() )
		return false;
	else { 
		elt = Element((*i).second);
		return true;
	}
}



const CString& CChem::ToString(Element elt)
{
	InitializeMaps();

	ElementToCStringMap::iterator i = e2sMap.find(elt);

	assert( i != e2sMap.end() );

	return (*i).second;
}



////////////////////////////////////////////////////////////////
//	
//	Implementation
//	
////////////////////////////////////////////////////////////////

void CChem::InitializeMaps()
{
	if ( e2sMap.size() != 0 ) return;

	e2sMap.insert(ElementToCStringMap::value_type( ELT_C,	"C"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_MN,	"Mn" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_P,	"P"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_S,	"S"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_SI,	"Si" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_TI,	"Ti" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_CB,	"Cb" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_V,	"V"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_AL,	"Al" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_N,	"N"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_NI,	"Ni" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_CU,	"Cu" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_CR,	"Cr" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_MO,  "Mo" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_SN,	"Sn" ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_B,	"B"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_CA,	"Ca"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_SB,	"Sb"  ));
	e2sMap.insert(ElementToCStringMap::value_type( ELT_PB,	"Pb"  ));


	s2eMap.insert(CStringToElementMap::value_type(	"C"		,ELT_C)  );
	s2eMap.insert(CStringToElementMap::value_type(	"C "	,ELT_C)  );
	s2eMap.insert(CStringToElementMap::value_type(	"MN"	,ELT_MN) );
	s2eMap.insert(CStringToElementMap::value_type(	"Mn"	,ELT_MN) );
	s2eMap.insert(CStringToElementMap::value_type(	"P"		,ELT_P)  );
	s2eMap.insert(CStringToElementMap::value_type(	"P "	,ELT_P)  );
	s2eMap.insert(CStringToElementMap::value_type(	"S"		,ELT_S)  );
	s2eMap.insert(CStringToElementMap::value_type(	"S "	,ELT_S)  );
	s2eMap.insert(CStringToElementMap::value_type(	"SI"	,ELT_SI) );
	s2eMap.insert(CStringToElementMap::value_type(	"Si"	,ELT_SI) );
	s2eMap.insert(CStringToElementMap::value_type(	"TI"	,ELT_TI) );
	s2eMap.insert(CStringToElementMap::value_type(	"Ti"	,ELT_TI) );
	s2eMap.insert(CStringToElementMap::value_type(	"CB"	,ELT_CB) );
	s2eMap.insert(CStringToElementMap::value_type(	"Cb"	,ELT_CB) );
	s2eMap.insert(CStringToElementMap::value_type(	"V"		,ELT_V)  );
	s2eMap.insert(CStringToElementMap::value_type(	"V "	,ELT_V)  );
	s2eMap.insert(CStringToElementMap::value_type(	"AL"	,ELT_AL) );
	s2eMap.insert(CStringToElementMap::value_type(	"Al"	,ELT_AL) );
	s2eMap.insert(CStringToElementMap::value_type(	"N"		,ELT_N)  );
	s2eMap.insert(CStringToElementMap::value_type(	"N "	,ELT_N)  );
	s2eMap.insert(CStringToElementMap::value_type(	"NI"	,ELT_NI) );
	s2eMap.insert(CStringToElementMap::value_type(	"Ni"	,ELT_NI) );
	s2eMap.insert(CStringToElementMap::value_type(	"CU"	,ELT_CU) );
	s2eMap.insert(CStringToElementMap::value_type(	"Cu"	,ELT_CU) );
	s2eMap.insert(CStringToElementMap::value_type(	"CR"	,ELT_CR) );
	s2eMap.insert(CStringToElementMap::value_type(	"Cr"	,ELT_CR) );
	s2eMap.insert(CStringToElementMap::value_type(	"MO"	,ELT_MO) );
	s2eMap.insert(CStringToElementMap::value_type(	"Mo"	,ELT_MO) );
	s2eMap.insert(CStringToElementMap::value_type(	"SN"	,ELT_SN) );
	s2eMap.insert(CStringToElementMap::value_type(	"Sn"	,ELT_SN) );
	s2eMap.insert(CStringToElementMap::value_type(	"B"		,ELT_B)  );
	s2eMap.insert(CStringToElementMap::value_type(	"B "	,ELT_B)  );
	s2eMap.insert(CStringToElementMap::value_type(	"CA"	,ELT_CA) );
	s2eMap.insert(CStringToElementMap::value_type(	"Ca"	,ELT_CA) );
	s2eMap.insert(CStringToElementMap::value_type(	"SB"	,ELT_SB) );
	s2eMap.insert(CStringToElementMap::value_type(	"Sb"	,ELT_SB) );
	s2eMap.insert(CStringToElementMap::value_type(	"PB"	,ELT_PB) );
	s2eMap.insert(CStringToElementMap::value_type(	"Pb"	,ELT_PB) );

}		


