 
// SpecMgr.h: interface for the CSpecMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SPECMGR_H__INCLUDED_)
#define SPECMGR_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000




#include "bofspecmgr.h"
#include "modelTypes.h"
#include "Chem.h"


class CSpec;
class CSpecDPASet;
class CSpecDPAChemsSet;



class CSpecMgr
{

	////////////////////////////////////////////////////////////////
	//	
	//	Manages a complete set of specs
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////


public:

	//  We index specs by name
	typedef map<CString,CSpec*> T_SpecMap;


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:
	
	T_SpecMap specMap;

	// originally had this as an embedded object, not a pointer.
	// Problem: there is a global CSnapshot object, which embeds a CSpecMgr,
	//    leading to the c-tor of the IBOFSpeedCalc being initialized during
	//    global variable startup, which was before OLE was initialized.
	// Well, come to think of it, it wasn't the construction that was causing
	//    the problem -- that only created the COLeDispatchDriver object and
	//    didn't actually do the CreateDispatch -- but rather the d-tor was
	//    releasing the dispatch AFTER OLE had shut down.
	// Sigh.

	IBOFSpeedCalc* m_pBOFSpecMgr; 



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////
		
public:

	CSpecMgr() { m_pBOFSpecMgr = 0; }

	~CSpecMgr() { Cleanup(); }


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	bool Initialize(CDatabase* pDBDPA);

	CSpec* FindSpecMaybe(CString& specName, int caster);

	double SlabCastTime(const CString& specName,
						int caster,
						int strand,
						int position,
						Weight weight,
						Width width,
						Thick thickness,
						CChem::Bound carbon,
						CChem::Bound manganese,
						int heatSize,
						bool* isValid);

	void CleanupDispatch();
	
	
	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:
	
	void Cleanup();

	bool InitializeBOFSpecs();
	void InitializeSpecs(CSpecDPASet& specSet);
	void InitializeDPAChems(CSpecDPAChemsSet& chemSet);

	void RecordSpec(CSpec* pSpec);
	void RecordSpec(const CString& keyName, CSpec* pSpec);

	CSpec* FindSpecAux(CString& specName);
	CSpec* FindSpec(CString& specName);

};

#endif
