// ScenMru.h: interface for the CScenMru class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENMRU_H__E262DF55_4C20_11D1_804C_006097B38214__INCLUDED_)
#define AFX_SCENMRU_H__E262DF55_4C20_11D1_804C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CCasterScen;
class CProdnScen;
class CCastString;
class CSuperScen;

#include "ProdnScen.h"
#include "CastStringId.h"
#include "MessageBus.h"
 


	
class CScenMru : public IUnknownImpl_T<IMvcObserver>
{
	////////////////////////////////////////////////////////////////
	//	
	//	Maintains most-recently-used list for scenarios
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

private:

	class CCasterScenInfo
	{
		////////////////////////////////////////////////////////////////
		//	
		//	A helper class for CScenMru
		//  Keeps track of the information for one CCasterScen
		//	
		////////////////////////////////////////////////////////////////

		friend CScenMru;

		////////////////////////////////////////////////////////////////
		//	
		//	Data
		//	
		////////////////////////////////////////////////////////////////
	
	private:
			CCasterScen* m_pScen;
				// The CasterScen this is keeping information on
	
			CCastStringId m_stringId;
				// Id of string most recently being edited
	
	
		////////////////////////////////////////////////////////////////
		//	
		//	Accessors
		//	
		////////////////////////////////////////////////////////////////
	
	public:
	
		CCasterScen* CasterScen() const { return m_pScen; }
		int StringIndex() { return CanonicalizeStringId(); }
	
	
		////////////////////////////////////////////////////////////////
		//	
		//	C-tors, d-tor
		//	
		////////////////////////////////////////////////////////////////
	
	public:
	
		CCasterScenInfo() : m_pScen(0) {}
	
		CCasterScenInfo(CCasterScen* pScen,
						int index)
			: m_pScen(pScen)
						   
		{ SetCurrentCastStringId(index); }

	
		////////////////////////////////////////////////////////////////
		//	
		//	Operators
		//	
		////////////////////////////////////////////////////////////////
	
	public:
	
		bool operator<(const CCasterScenInfo& x) const 
		{
			return m_pScen < x.m_pScen;
		}
	
		bool operator==(const CCasterScenInfo& x) const
		{
			return m_pScen == x.m_pScen;
		}
	
	
		////////////////////////////////////////////////////////////////
		//	
		//	Operations
		//	
		////////////////////////////////////////////////////////////////
	
	public:
	
		void SetCurrentCastStringId(int index);
		void SetCurrentCastStringId(CCastString* pString);
		int CanonicalizeStringId();
	
	
		////////////////////////////////////////////////////////////////
		//	
		//	Implementation
		//	
		////////////////////////////////////////////////////////////////
	
	protected:
	
	
	};  // end class CCasterScenInfo
	

	public:

	//  Notification message classes for operations of general interest

	class CPromoteCasterScenMsg : public CCsdaMsg_Impl<MSG_ID_MRU_PROMOTE_CASTER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CPromoteCasterScenMsg(CCasterScen* pCasterScen,int casterNum)
			: m_pCasterScen(pCasterScen),m_casterNum(casterNum)
		{}

		CCasterScen* CasterScen() { return m_pCasterScen;	}
		int CasterNum() { return m_casterNum;	}

	protected:
		CCasterScen* m_pCasterScen;
			// the scenario that has been promoted

		int m_casterNum;
			// the caster# of the that scenario (redundant)
	};


	class CPromoteProdnScenMsg : public CCsdaMsg_Impl<MSG_ID_MRU_PROMOTE_PRODN_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CPromoteProdnScenMsg(CProdnScen* pProdnScen)
			: m_pProdnScen(pProdnScen)
		{}

		CProdnScen* ProdnScen() { return m_pProdnScen;	}

	protected:
		CProdnScen* m_pProdnScen;
			// the scenario that has been promoted
	};


	class CSelectCasterMsg : public CCsdaMsg_Impl<MSG_ID_MRU_SELECT_CASTER>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CSelectCasterMsg(int newCasterNum, int oldCasterNum)
			: m_newCasterNum(newCasterNum),
			  m_oldCasterNum(oldCasterNum)
		{}

		int OldCasterNum() const
		{	return m_oldCasterNum;	}

		int NewCasterNum() const
		{	return m_newCasterNum;	}
		

	protected:
		int m_oldCasterNum;
		int m_newCasterNum;
			//  We have switched the selected caster.
			//  these record the old and new values.
	};


	class CSelectCastStringMsg : public CCsdaMsg_Impl<MSG_ID_MRU_SELECT_CAST_STRING>
	{	
	public:

		virtual void Sprint(CString& strCmd);

		CSelectCastStringMsg(int casterNum,
						 CCasterScen* pCasterScen,
						 CCastString* pString)
			: m_casterNum(casterNum),
			  m_pCasterScen(pCasterScen),
			  m_pCastString(pString)
		{}

		int CasterNum() const
		{	return m_casterNum;	}
	
		CCasterScen* CasterScen() 
		{	return m_pCasterScen;	}

		CCastString* CastString() 
		{	return m_pCastString;	}
		

	protected:
		int m_casterNum;
			// the caster number of the scenario
		CCasterScen* m_pCasterScen;
			// the scenario upon which the action occurred
		CCastString* m_pCastString;
			// the newly selected cast string
	};
		

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	vector<CCasterScenInfo> m_casterScens[4];  // using 1,2,3
		//  info on each caster scen that exists

	vector <CProdnScen*> m_prodnScens;
		// info on each prodn scen that exists.
	
	CSuperScen* m_pSuperScen;
		// the CSuperScen corresponding to the currently selected
		//    caster scens (for each caster) and prodn scen.
		// the currently selected caster scen is at index 0 in its vector.

	int m_casterNum;
		// the currently selected caster

	static vector<CScenMru*> Mrus;
		// we allow for more than one Mru to exist
		// the c-tor will register each one here

public:

	static CScenMru GlobalScenMru;
		// we need at least one, easily accessible


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	int CasterNum() const 
	{ return m_casterNum; }
	
	void SetCasterNum(int newVal);

	CProdnScen* CurrentProdnScen() const;

	CCasterScen* CurrentCasterScen() const;
	CCastString* CurrentCastString();

	CCasterScen* CurrentCasterScen(int casterNum) const;
	CCastString* CurrentCastString(int casterNum);

	int CurrentCastStringIndex(int casterNum);

	CSuperScen* CurrentSuperScen() const 
	{ return m_pSuperScen; }

	void SetSuperScen();

	void SetCurrentCastString(int index);
	void SetCurrentCastString(CCastString* pString);
	void SetCurrentCastString(int casterNum, int index);
	void SetCurrentCastString(int casterNum, CCastString* pString);


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CScenMru();
	virtual ~CScenMru();


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void PromoteProdnScen(CProdnScen*);
	void PromoteCasterScen(CCasterScen*);


	int MaybeCasterScenChangeOk(bool cancelOk);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:

	void OnUpdate(IMvcSubject* pSubject, 
				  IMvcMessage* pMsg);

	void CreateCasterScenEntries(int casterNum);
	void CreateProdnScenEntries();

	void RemoveProdnScen(CProdnScen* pScen);
	CCasterScenInfo RemoveCasterScen(CCasterScen* pScen);


		// required to provide implementation of IQueryGuid::QueryGuid (stingray)
	BEGIN_GUID_MAP(CScenMru)
	END_GUID_MAP

};

#endif // !defined(AFX_SCENMRU_H__E262DF55_4C20_11D1_804C_006097B38214__INCLUDED_)
