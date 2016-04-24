// Chem.h: interface for the CChem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHEM_H__D61C7931_FF8D_11D0_801F_006097B38214__INCLUDED_)
#define AFX_CHEM_H__D61C7931_FF8D_11D0_801F_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


////////////////////////////////////////////////////////////////
//	
//	class CChem
//
//	Class providing (static) utility functions for manipulating
//     an enumerated class of elements.
//	
////////////////////////////////////////////////////////////////



class CChem  
{

	////////////////////////////////////////////////////////////////
	//	
	//	Enumerations, typedefs	
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef double Bound;
		//	Used to indicate min/max/aim percentages of chemistries
		//	Range is 0.0 to 100.0
		//	Could use a float, but who cares?


	//  The various elements 

	enum Element { 
				   ELT_AL,
				   ELT_B,
				   ELT_C, 
				   ELT_CA,
				   ELT_CB,
				   ELT_CR,
				   ELT_CU,
				   ELT_MN, 
				   ELT_MO,
				   ELT_N,
				   ELT_NI,
				   ELT_P, 
				   ELT_PB,
				   ELT_S,
				   ELT_SB,
				   ELT_SI,
				   ELT_SN,
				   ELT_TI,
				   ELT_V,
	};

	enum { NumElements = ELT_V+1 };


	//  These int's should really be Element, but the compiler can't cope.

	typedef map<int,CString> ElementToCStringMap;
	typedef map<CString,int> CStringToElementMap;


	////////////////////////////////////////////////////////////////
	//	
	//	static data members
	//	
	////////////////////////////////////////////////////////////////


	static ElementToCStringMap e2sMap;
	static CStringToElementMap s2eMap;



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//		
	////////////////////////////////////////////////////////////////

private:
	
	//  No need to create an instance of this class.

	// do not implement
	CChem();
	CChem(const CChem&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	// conversion

public:

	static Element ToElement(const CString&);
	static const CString& ToString(Element);
	static bool ToElement(const CString&, Element&);


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	static void InitializeMaps();


};

#endif // !defined(AFX_CHEM_H__D61C7931_FF8D_11D0_801F_006097B38214__INCLUDED_)
