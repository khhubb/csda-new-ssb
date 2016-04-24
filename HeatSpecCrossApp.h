// HeatSpecCrossApp.h: interface for the CHeatSpecCrossApp class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_HEATSPECCROSSAPP_H__8911D847_9521_11D1_8065_006097B38214__INCLUDED_)
#define AFX_HEATSPECCROSSAPP_H__8911D847_9521_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef multimap<CString,CString> THeatSpecCrossAppMap;
typedef set<CString> THeatSpecCrossAppSet;

class CHeatSpecCrossApp  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Holds a multimap from specs to cross-applicable specs
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	THeatSpecCrossAppMap m_xappMap;

	
	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	// the map is not directly accessible


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CHeatSpecCrossApp() {}
	~CHeatSpecCrossApp() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	bool Init(CDatabase*);

	THeatSpecCrossAppSet GetCrossAppSpecs(const CString& spec,int caster) const;
	THeatSpecCrossAppSet GetCrossAppSpecs(THeatSpecCrossAppSet& specs, int caster) const;
	bool IsCompatible(const CString& spec1, const CString& spec2, int caster) const;


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void Clear();
	void PrintMap();
	

};

#endif // !defined(AFX_HEATSPECCROSSAPP_H__8911D847_9521_11D1_8065_006097B38214__INCLUDED_)
