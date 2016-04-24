// ScenMgr.h: interface for the CScenMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENMGR_H__4545D593_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SCENMGR_H__4545D593_A6F5_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
 

class CProdnScen;
class CCasterScen;
class CSuperScen;
class CScenario;

#include "CasterScen.h"
#include "ProdnScen.h"
#include "SuperScen.h"
#include "ArchiveQueue.h"
#include "MessageBus.h"

//  These seemed useful


typedef vector< CCasterScen* > TCasterScenVec;
typedef vector<  CProdnScen* > TProdnScenVec;
typedef vector<  CSuperScen* > TSuperScenVec;


class CScenMgr  
{
	
	////////////////////////////////////////////////////////////////
	//	
	//	This class serves as a class factory and general manager
	//		for all types of scenarios and all superscenarios
	//	Because c-tors and d-tors for those classes are private,
	//		only this class can create/delete these kinds of objects.
	//  This class has only static members.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:


	//  The IMvcMessage classes for notifications to observers.

	class CCreateCasterScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_CREATE_CASTER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CCreateCasterScenMsg(CCasterScen* pCasterScen)
			: m_pCasterScen(pCasterScen)
		{}

		CCasterScen* CasterScen() { return m_pCasterScen;	}

	protected:
		CCasterScen* m_pCasterScen;
	};

	class CCreateProdnScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_CREATE_PRODN_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CCreateProdnScenMsg(CProdnScen* pProdnScen)
			: m_pProdnScen(pProdnScen)
		{}

		CProdnScen* ProdnScen() { return m_pProdnScen;	}

	protected:
		CProdnScen* m_pProdnScen;
	};


	class CDeleteCasterScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_DELETE_CASTER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CDeleteCasterScenMsg(CCasterScen* pCasterScen)
			: m_pCasterScen(pCasterScen)
		{}

		CCasterScen* CasterScen() { return m_pCasterScen;	}

	protected:
		CCasterScen* m_pCasterScen;
	};

	class CDeleteProdnScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_DELETE_PRODN_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CDeleteProdnScenMsg(CProdnScen* pProdnScen)
			: m_pProdnScen(pProdnScen)
		{}

		CProdnScen* ProdnScen() { return m_pProdnScen;	}

	protected:
		CProdnScen* m_pProdnScen;
	};


	class CCreateSuperScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_CREATE_SUPER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CCreateSuperScenMsg(CSuperScen* pSS)
			: m_pSuperScen(pSS)
		{}

		CSuperScen* SuperScen() { return m_pSuperScen;	}

	protected:
		CSuperScen* m_pSuperScen;
	};

	class CDeleteSuperScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_DELETE_SUPER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CDeleteSuperScenMsg(CSuperScen* pSS)
			: m_pSuperScen(pSS)
		{}

		CSuperScen* SuperScen() { return m_pSuperScen;	}

	protected:
		CSuperScen* m_pSuperScen;
	};

	class CSaveCasterScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_SAVE_CASTER_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CSaveCasterScenMsg(CCasterScen* pCasterScen)
			: m_pCasterScen(pCasterScen)
		{}

		CCasterScen* CasterScen() { return m_pCasterScen;	}

	protected:
		CCasterScen* m_pCasterScen;
	};

	class CSaveProdnScenMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_SAVE_PRODN_SCEN>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CSaveProdnScenMsg(CProdnScen* pProdnScen)
			: m_pProdnScen(pProdnScen)
		{}

		CProdnScen* ProdnScen() { return m_pProdnScen;	}

	protected:
		CProdnScen* m_pProdnScen;
	};



	class CCasterScenInfoChangedMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CCasterScenInfoChangedMsg(CCasterScen* pCasterScen)
			: m_pCasterScen(pCasterScen)
		{}

		CCasterScen* CasterScen() { return m_pCasterScen;	}

	protected:

		CCasterScen* m_pCasterScen;
	};


	class CProdnScenInfoChangedMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_PRODN_SCEN_INFO_CHANGED>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CProdnScenInfoChangedMsg(CProdnScen* pProdnScen)
			: m_pProdnScen(pProdnScen)
		{}

		CProdnScen* ProdnScen() { return m_pProdnScen;	}

	protected:

		CProdnScen* m_pProdnScen;
	};

	class CSuperScenInfoChangedMsg : public CCsdaMsg_Impl<MSG_ID_SMGR_SUPER_SCEN_INFO_CHANGED>
	{
	public:

		virtual void Sprint(CString& strCmd);

		CSuperScenInfoChangedMsg(CSuperScen* pSS)
			: m_pSuperScen(pSS)
		{}

		CSuperScen* SuperScen() { return m_pSuperScen;	}

	protected:

		CSuperScen* m_pSuperScen;
	};


	////////////////////////////////////////////////////////////////
	//	
	//	Data members (static)
	//	
	////////////////////////////////////////////////////////////////

private:

	static TCasterScenVec	m_casterScens[4]; // index = 1,2,3
		// vectors of caster scens for each caster

	static TProdnScenVec		m_prodnScens;
		// vector of production scenarios which have been created

	static TSuperScenVec		m_superScens;
		// vector of super scens which have been created

	static CArchiveQueue m_archiveQueue;
		// The archive queue manages archiving (checkpointing) scenarios to the database
		// we need one of these.
	

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	static TCasterScenVec::iterator CasterScensBegin(int casterNum)
	{	return m_casterScens[casterNum].begin();	}

	static TCasterScenVec::iterator CasterScensEnd(int casterNum)
	{	return m_casterScens[casterNum].end();	}

	//### into to size_t
	static size_t CasterScensCount(int casterNum)
	{	return m_casterScens[casterNum].size();	}

	static TProdnScenVec::iterator ProdnScensBegin()
	{	return m_prodnScens.begin();	}

	static TProdnScenVec::iterator ProdnScensEnd()
	{	return m_prodnScens.end();	}

	//## int to size_t
	static size_t ProdnScensCount()
	{	return m_prodnScens.size();	}

	static TSuperScenVec::iterator SuperScensBegin()
	{	return m_superScens.begin();	}

	static TSuperScenVec::iterator SuperScensEnd()
	{	return m_superScens.end();	}

	//## int to size_t
	static size_t SuperScensCount()
	{	return m_superScens.size();	}

	static CCasterScen* CasterScen(int casterNum, int index);

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////


private:

	// static-only class, keep c-tor private, unimplemented.
	CScenMgr();
	~CScenMgr();
	CScenMgr(const CScenMgr&);
	CScenMgr& operator=(const CScenMgr&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////



public:

	static CCasterScen* CreateCasterScen(int casterNum,
										 bool canEdit);

	static CCasterScen* CreateMasterCasterScen(int casterNum);

	static CCasterScen* CreateCasterScen(int casterNum,
										 long id);


	static CCasterScen* CopyCasterScen(CCasterScen*,bool canEdit,bool isSnapScen);
	static CCasterScen* CopyCasterScen(CCasterScen*,bool canEdit,
									   vector<int>& indexes909);
	static CCasterScen* CopyCasterScenForArchival(CCasterScen*);

	static void BackupCasterScen(CCasterScen* pBase);
	static CCasterScen* RestoreCasterScenBackup(CCasterScen* pBase);
	static void DeleteCasterScenBackup(CCasterScen* pBase);
	static void DeleteArchivalCasterScen(CCasterScen* pScen);
	
	
	static CProdnScen* CreateProdnScen(bool canEdit);

	static CSuperScen* CreateSuperScen(CCasterScen* pCScen1,
									   CCasterScen* pCScen2,
									   CCasterScen* pCScen3,
									   CProdnScen*  pProdnScen,
									   bool forSnap = false);

	static CSuperScen* CreateSuperScen();

	static CSuperScen* FindSuperScen(CCasterScen* pCScen1,
								    CCasterScen* pCScen2,
								    CCasterScen* pCScen3,
								    CProdnScen*  pProdnScen);

	static CSuperScen* SuperScen(CCasterScen* pCScen1,
						 	     CCasterScen* pCScen2,
								 CCasterScen* pCScen3,
								 CProdnScen*  pProdnScen);

	static bool SuperScenExists(CCasterScen* pCScen1,
								CCasterScen* pCScen2,
								CCasterScen* pCScen3,
								CProdnScen*  pProdnScen);


	static void DeleteAllProdnScens();
	static void DeleteAllCasterScens();

	static void DeleteCasterScen(CCasterScen*);
	static void DeleteMasterCasterScen(CCasterScen*);
	static void DeleteProdnScen(CProdnScen*);
	static void DeleteSuperScen(CSuperScen*);


	static int CasterScenIndex(int casterNum, CCasterScen* pScen);
	static int ProdnScenIndex(CProdnScen* pScen);

	static CCasterScen* CasterScenById(int casterNum, long id);


	static void ArchiveAllScens();
	//static void ArchiveAllCasterScens(int casterNum);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////


private:
	static void DeleteContainingSuperScens(CScenario* pScen);
	static void FindModifiedCasterScens(int caster,
								 vector<CCasterScen*>& copyScens,
								 vector<CCasterScen*>& origScens);

};




#endif // !defined(AFX_SCENMGR_H__4545D593_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
