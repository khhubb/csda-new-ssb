// CastScen.h: interface for the CCastScen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTSCEN_H__F60F11C5_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CASTSCEN_H__F60F11C5_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Scenario.h"


class CCastString;
class CDalyPlanSet;
class CSSBOutput;
class CSnapshot;
class COrder;

#include "Snapshot.h"
#include "CastString.h"
#include "OrderSelection.h"



typedef vector<CCastString*> TCastStringVec;

// pointer comparisions should work
typedef map<COrder*,int> TOrderIntMap;


class CCasterScen : public CScenario  
{
	//////////////////////////////////////////////////////////////
	//
	//	Represents a scenario for a caster.
	//
	//////////////////////////////////////////////////////////////

	//  CScenMgr will be responsible for creation and destruction
	//    of CCasterScen objects.

	friend class CScenMgr;


	//////////////////////////////////////////////////////////////
	//
	// Data members:
	//
	//////////////////////////////////////////////////////////////

private:

	int m_caster;
		//	Which caster the scenario is for, = 1,2,3.

	TCastStringVec m_castStrings;
		//	The sequence of strings scheduled for the caster.
	
	TOrderIntMap m_orderSlabsMap;
		//  A map from orders scheduled on strings in this scenario
		//     tracking total slabs scheduled.
		//  Needs to be maintained as orders are added/deleted


	CCasterScen* m_pBackup;
		// We make an exact copy of the scenario at creation and each time we checkpoint.
		// Later, we can compare this scenario to its backup to determine if any 
		//   changes have occurred, necessitating another checkpoint.

	bool m_isPublic;
		// We originally had a scheme where the user could publish certain scenarios,
		// making them 'public'. Not currently used.


	//////////////////////////////////////////////////////////////
	//
	// Accessors
	//
	//////////////////////////////////////////////////////////////

public:

	int Caster() const 
	{ return m_caster; }
	
	vector<CCastString*>& CastStrings() 
	{ return m_castStrings; }
	
	bool IsPublic() const
	{	return m_isPublic;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tors
	//	
	////////////////////////////////////////////////////////////////

private:

	//  creation/destruction controlled by CScenMgr
	//	no stack allocation

	CCasterScen(int caster_);
	CCasterScen(const CCasterScen&,
				bool isSnapScen); // copy c-tor
	CCasterScen(const CCasterScen&, 
		        vector<int>& indexes909); // copy c-tor, modified
	~CCasterScen();

	// do not implement
	CCasterScen(const CCasterScen&);
	CCasterScen& operator=(const CCasterScen&);
	// do not implement



	//////////////////////////////////////////////////////////////
	//	
	//	Operations
	//
	//////////////////////////////////////////////////////////////

public:

	bool LoadFromDalyPlan(CDalyPlanSet& planSet);
	//bool LoadFromStringBuilderOutput(CStringBuilderOutputSet& sbSet,
	//								 vector<CCastString*>& newStrings);
	bool LoadFromNewStringBuilderOutput(CSSBOutput& sbSet,
		vector<CCastString*>& newStrings);

	CCastString* CreateNewString(CCastStringId& id,
 				     CCastStringMiscProps& props);

	CCastString* CreateCopyString(CCastStringId& id,
   										  CCastStringMiscProps& props,
										   CCastString* pOrigString);

	void DeleteString(CCastString* pString);


	void RegisterCSOrder(CCSOrder*);
	void UnregisterCSOrder(CCSOrder*);

	int NumSlabsScheduled(const COrder* pOrder) const;

	// FP change
	//COrder* FindOrder(int orderNum) const;
	COrder* FindOrder(CString& fpOrderNum) const;


	CCastString* FindCastString(const CCastStringId& id) const;

	void GetLineupIds(vector<CCastStringId>& ids,
					  const CCastStringId* pIdToIgnore = 0);

	void ModifyCastStringId(CCastString* pString,
							const CCastStringId& newId);


	// ## Changed int to __int64
	void FindMarkedHeats(vector< pair<__int64, __int64> >& markedHeats,
					      CCastString* pIgnoreString );



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void Cleanup();

	bool OpenDalyPlanSet(CDalyPlanSet& planSet);

};



#endif // !defined(AFX_CASTSCEN_H__F60F11C5_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
