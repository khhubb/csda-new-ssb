#if !defined(AFX_HMLOADGRAPHDOC_H__AA31A9C3_FF34_11D1_8088_006097B38214__INCLUDED_)
#define AFX_HMLOADGRAPHDOC_H__AA31A9C3_FF34_11D1_8088_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMLoadGraphDoc.h : header file
//

#include "modelTypes.h"

class CSuperScen;
class CCastString;
class CHMSupplyRecord;
class CCastStringMiscProps;

#include "SuperScen.h"
#include "ScenMru.h"
#include "DocBase.h"
#include "MessageBus.h"


/////////////////////////////////////////////////////////////////////////////
// CHMLoadGraphDoc document


class CHMLoadRecord
{
public:

	CTime m_time;
	Weight m_tons;

	CHMLoadRecord(CTime time, Weight tons)
		: m_time(time), m_tons(tons)
	{}

	CHMLoadRecord() {}

	bool operator<(const CHMLoadRecord& x) const
	{
		return m_time < x.m_time;
	}

	bool operator==(const CHMLoadRecord& x) const
	{
		return m_time == x.m_time;
	}

};


class CHMLoadGraphDoc : public CDocBase, public IUnknownImpl_T<IMvcObserver>
{
protected:
	CHMLoadGraphDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHMLoadGraphDoc)

// Attributes
public:

	//  Rather than making this the usual dummy document,
	//  we will keep editor state in the document
	//  as a central access point for the various views/viewports/controllers


		// required to provide implementation of IQueryGuid::QueryGuid (stingray)
	BEGIN_GUID_MAP(CHMLoadGraphDoc)
	END_GUID_MAP


	class CUpdateMsg: public CCsdaMsg_Impl<MSG_ID_HM_SCEN_UPDATE>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CUpdateMsg()
		{}

	protected:
	};





	CScenMru& GetScenMru()
	{	
		return CScenMru::GlobalScenMru;
	}

	const CScenMru& GetScenMru() const
	{	
		return CScenMru::GlobalScenMru;
	}

	int GetCasterNum() const 
	{	return GetScenMru().CasterNum();	}

	CProdnScen* GetCurrentProdnScen() const
	{	return GetScenMru().CurrentProdnScen();	}


	CCasterScen* GetCurrentCasterScen(int i) const
	{	return GetScenMru().CurrentCasterScen(i);	}


	CSuperScen* GetSuperScen() const
	//{ return pSuperScen;	}
	{	return GetScenMru().CurrentSuperScen();	}

		bool IsEditing() const
	{	return GetScenMru().CurrentSuperScen() != 0;	}

	int GetCasterScenIndex(int casterNum) const;


#if 0 
// I think this is no longer needed
// originally used by the now-defunct CHMLoadScenMgrView

	void ChangeCasterScen(int whichCaster, CCasterScen* pScen);
#endif

private:
	vector<CHMLoadRecord> m_loads[4];  // caster = 1,2,3, 0 = OVERALL LOAD COUNT
	vector<CCastString*> m_strings[4];

	bool m_show910s;

public:


// Operations
public:


	vector<CCastString*>& GetStrings(int caster);

	void Init();
	void Update();
	void SetStrings();
	void SetStrings(int caster);
	void EnforceTurnarounds(int caster);
	void ResequenceStrings(int caster, vector<int>& indexes);
	void RefreshStrings(int caster);
	void CreateString(CCastStringId& id,CCastStringMiscProps& props);


	bool Show910s() const { return m_show910s;} 
	void ToggleShow910s() { m_show910s = ! m_show910s; Update(); }

	void ActivationChange(BOOL bActivate);


	void ComputeTimeRange(CTime& et, CTime& lt);

	CTime TimeGrainFloor(CTime t);
	CTime TimeGrainCeiling(CTime t);
	CTime ComputeEarliestTime();
	CTime ComputeLatestTime();
	CTime ComputeEarliestTime(int caster);
	CTime ComputeLatestTime(int caster);

	void ComputeLoad();
	void ComputeLoad(int);
	void AddLoad(int caster, Weight tons, CTime t);
	CTime CasterStartTime(int caster);
	void ComputeCumulativeLoad();

	vector<CHMLoadRecord>& GetLoadRecords(int caster)
	{	
		assert( caster == 0 || caster == 1 || caster == 2 || caster == 3);
		return m_loads[caster];
	}

	vector<CHMSupplyRecord>& GetSupplyRecords();

	static void FindWidthsToUse( CCastString* pString,
								 long& rStrand1Begin,
								 long& rStrand1End,
								 long& rStrand2Begin,
								 long& rStrand2End);
	static CString FindSpecToUse( CCastString* pString );

	void DeleteString(CCastString* pString);

	void OnUpdate(IMvcSubject* pSubject, 
 				IMvcMessage* pMsg);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLoadGraphDoc)
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHMLoadGraphDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMLoadGraphDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLOADGRAPHDOC_H__AA31A9C3_FF34_11D1_8088_006097B38214__INCLUDED_)
