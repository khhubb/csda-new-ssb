// Snapshot.h: interface for the CSnapshot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNAPSHOT_H__F60F11BF_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SNAPSHOT_H__F60F11BF_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


 
#include "CICode.h"
#include "SuperScen.h"
#include "Order.h"
#include "Spec.h"
#include "SpecMgr.h"
#include "SpecChemRangePreset.h"
#include "HeatSpecCrossApp.h"
#include "HMSupplyMgr.h"


#include "CastString.h"



#if 0
	//  this is no longer being used?

class OrderUseRecord
{
public:

	long num909Slabs;
	long num910Slabs;
	Weight wgt909Slabs;
	Weight wgt910Slabs;

	OrderUseRecord() 
		: num909Slabs(0),
		  num910Slabs(0),
		  wgt909Slabs(0.0),
		  wgt910Slabs(0.0)
	{
	}
};



typedef map<COrder*,OrderUseRecord> OrderUsageMap;

#endif	// not used



enum UploadChangeType {
	UPLOAD_NO_CHANGE,
	UPLOAD_DELETE,
	UPLOAD_MODIFY,
	UPLOAD_PROMOTE_909,
	UPLOAD_PROMOTE_910
};



typedef map<CString,set<CString> > TSteelSpecCrossAppMap;



class CSnapshot  
{
	////////////////////////////////////////////////////////////////
	//	
	//	A snapshot of the database.
	//  
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CCICodeGroupMgr	m_ciMgr;
		// Holds the groups of CI codes

	CCICodeBogieMgr	m_bogieMgr;
		// Holds the bogies for the CI code groups

	CSpecMgr		m_specMgr;
		// Holds the specs

	CSuperScen*		m_pSuperScen;
		// A superScen containing the casterScens for this snapshot

	CHMSupplyMgr	m_hmSupplyMgr;
		// Tracks the hot metal supply figures

	vector<COrder*>	m_orders;
		// the order book

	set<CString>			m_availSteelSpecs;
		//  all the steel specs listed in all the orders

	TSteelSpecCrossAppMap	m_steelSpecCrossAppMap; 
		//  what it says.

	CHeatSpecCrossApp		m_heatSpecCrossApp;	
		// ditto

	vector<CSpecChemRangePreset>	m_specChemRangePresets;
		// loaded from a table

	static bool m_bCreated;

	long m_last910RecordId;
	CTime m_last910UpdateTime;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:


	CCICodeGroupMgr& CIMgr()
	{	return m_ciMgr;		}

	CCICodeBogieMgr& BogieMgr()
	{	return m_bogieMgr;	}

	CSpecMgr& SpecMgr()
	{	return m_specMgr;		}

	vector<COrder*>& Orders()
	{	return m_orders;	}

	const set<CString>& AvailSteelSpecs() const
	{	return m_availSteelSpecs;	}

	CHMSupplyMgr& HMSupplyMgr()
	{	return m_hmSupplyMgr;	}


	const TSteelSpecCrossAppMap& SteelSpecCrossAppMap() const
	{	return m_steelSpecCrossAppMap;	}

	const CSuperScen* SuperScen() const
	{	return m_pSuperScen;	}

	const vector<CSpecChemRangePreset>& SpecChemRangePresets() const
	{	return m_specChemRangePresets;	}


	const CHeatSpecCrossApp& HeatSpecCrossApp() const
	{	return m_heatSpecCrossApp;	}

	CSuperScen* SuperScen()
	{	return m_pSuperScen;	}

	long& Last910RecordId() 
	{	return m_last910RecordId;	}

	CTime& Last910UpdateTime()
	{	return m_last910UpdateTime;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	// Singleton creation semantics

	CSnapshot();
	~CSnapshot();
	
private:	
	//  not implemented
	CSnapshot(const CSnapshot&);
	CSnapshot& operator=(const CSnapshot&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:
	
	bool Initialize();
	CCasterScen* CopyInitialCasterScen(int casterNum);
	CCasterScen* CopyInitialCasterScen(int casterNum,
									   vector<int>& indexes909);
	
	const set<CString>& FindSteelSpecCrossApp(CString& steelSpec) const;

	void MaybeLoadNew910s();
	void LoadNew910s();
	static long GetUpdated910Ids(long lastId, CTime lastTime, vector<CCastStringId>& ids);


	bool LoadHMSupplyInfo();

	
	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void Cleanup();
	bool ReadOrders(CDatabase* pDB);
	void ReadMillOrderChems(CDatabase* pDB);
	void PopulateAvailSteelSpecs();
	void PopulateSteelSpecCrossAppMap(CDatabase* pDB);
	bool PopulateSpecChemRangePresets(CDatabase* pDB);
	void AdjustOrderSlabsDue();
	void CopyInitialCasterScens();
	void CopyNew910sIntoAllScens(CCasterScen* pScens[4]);
	void CopyNew910IntoAllScens( int caster, CCastString* pString );
	static CCastString* CopyNew910IntoScen( CCasterScen* pScen, CCastString* pString, CCastString* pOrigString );
	void LoadNew910( CCasterScen* pScen, CCastStringId& id);

	bool CasterRuleCheck(CDatabase* pDB);  // added k. hubbard 4-4-08

// not used?
#if 0
	void UpdateDBs( CCasterScen* pScen);
	void ComputeORM( int casterNum,
				     OrderUsageMap& orm );
#endif // not used


};


// global
extern CSnapshot TheSnapshot;

#endif // !defined(AFX_SNAPSHOT_H__F60F11BF_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
