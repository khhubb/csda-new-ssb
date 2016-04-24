// SuperScen.h: interface for the CSuperScen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERSCEN_H__F60F11C7_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SUPERSCEN_H__F60F11C7_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

 
class CCasterScen;
class CProdnScen;
class CSnapshot;
class COrder;
class CDalyPlanSet;
class CCastStringId;


class CSuperScen  
{
	////////////////////////////////////////////////////////////////
	//	
	//	A collection of one each of caster scens (1,2,3) and a prodn scen
	//	
	////////////////////////////////////////////////////////////////
	
	//  This class manages creation/destruction of these objects
	friend class CScenMgr;


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////
	
private:

	bool			m_bForSnap;
		//  The snapshot holds one superscen that holds the snapshot caster scens.
		//  this flag marks it.

	CCasterScen*	m_pCScens[4];  // 0 unused, 1,2,3 = caster
		//  The caster scens in this superscen

	CProdnScen*		m_pProdnScen;
		// the prodn scen in this superscen


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:
	
	bool ForSnap() const
	{	return m_bForSnap;	}

	void ForSnap(bool val)
	{	m_bForSnap = val;	}

	CCasterScen* CasterScen(int i) const;
	int CasterScenIndex(int i) const;

	CProdnScen* ProdnScen() const
	{	return m_pProdnScen;	}

	int ProdnScenIndex() const;


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

private:

	//  new/delete controlled by CScenMgr,
	//  no stack allocation

	CSuperScen();
	~CSuperScen();

	// not implemented
	CSuperScen(const CSuperScen&);
	CSuperScen& operator=(const CSuperScen&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	CString IdString() const;

	bool InitFromDalyPlan();
	bool LoadDalyPlanUpdates();

	int NumSlabsScheduled(const COrder* pOrder) const;
	int NumSlabsDue(const COrder* pOrder) const;

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	bool CreateAndLoadCasterScen(int casterNum,CDalyPlanSet& planSet);
	void LoadUpdated910( CCastStringId& id );
	long GetUpdated910Ids(long lastId, CTime lastTime, vector<CCastStringId>& ids);
	//void LoadString(int caster, int year, int week, int stringId);
	CTime LastDailyPlanDownloadTime();

};


#endif // !defined(AFX_SUPERSCEN_H__F60F11C7_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
