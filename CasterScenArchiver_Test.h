// CasterScenArchiver.h: interface for the CCasterScenArchiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTERSCENARCHIVER_H__C6D2F272_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSCENARCHIVER_H__C6D2F272_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCasterScen;
class CCastString;
class CCastStringId;
class CCastStringHeat;
class CCSOrder;
class CCasterScenariosSet;
class CCasterStringsSet;
class CCasterStringOrdersSet;
class CCasterStringTemplateSortsSet;
class CCasterStringHeatsSet;
class CCasterStringHeatChemsSet;
class CCasterStringHeatChemsSet;
class CCasterStringTemplatesSet;
class CCasterStringTemplateSpecsSet;
class CCasterStringTemplateChemsSet;
class CCasterStringTemplateSortsSet;
class CCsdaEventLogSet2;
class CMsgDlg;

#include "OrderSelection.h"
#include "AvailCasterScenId.h"

class CCasterScenArchiverAux
{
	////////////////////////////////////////////////////////////////
	//	
	//	This is an auxiliary class of CCasterScenArchiver.
	//  This class manages the open recordsets for the db access
	//    and does the actual archive reads/writes.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	friend class CCasterScenArchiver;
		//  everything private, so only CCasterScenArchiver can use.


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	bool m_isSaving;
		// flag to determine if writing(saving) or reading.

	CCasterScen* m_pScen;
		// the caster scen being written or created

	long m_version;
		// the version number of the save

	CDatabase* m_pDBCSDA;
		// the database object being used

	CCasterStringsSet*				m_pStringsSet;
	CCasterStringOrdersSet*			m_pOrdersSet;
	CCasterStringHeatsSet*			m_pHeatsSet;
	CCasterStringHeatChemsSet*		m_pHeatChemsSet;
	CCasterStringTemplatesSet*		m_pTemplatesSet;
	CCasterStringTemplateSpecsSet*	m_pTemplateSpecsSet;
	CCasterStringTemplateChemsSet*	m_pTemplateChemsSet;
	CCasterStringTemplateSortsSet*	m_pTemplateSortsSet;
	CCsdaEventLogSet2*				m_pEventLogSet;
		// the recordsets for the various tables being read/written.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////


private:

	CCasterScenArchiverAux();
	~CCasterScenArchiverAux();

	
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

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void DeleteRecordset(CRecordset* prs);
	
	void Archive(CCasterScen* pScen_, bool notify);
	void ArchiveStringToMasterScen(CCasterScen* pScen,
								   CCastString* pString);
	void DeleteStringFromMasterScen(CDatabase* pDBCSDA,
									CCastString* pString,
									CMsgDlg* dlg);

	void CreateRecordsets(CDatabase* pDBCSDA);
	void OpenRecordsetsForWriting();
	void OpenRecordsetsForReadingScenario(long id, long version);
	void OpenRecordsetsForReadingStringFromMasterScen(int year,
													  int week,
													  int caster,
													  int stringId);
	void CloseRecordsets();
	void DestroyRecordsets();

	void IncrementVersion();
	void Log910Event(CCastString* pString,CMsgDlg*);

	void ArchiveStrings(CMsgDlg*);

	void ArchiveString(CCastString* pString,CMsgDlg*);

	void ArchiveHeats(CCastString* pString);

	void ArchiveHeat(CCastStringHeat& rHeat,
					 int seqNum,
					 CCastString* pString);

	void ArchiveStrand(int strandNum, 
					   CCastString* pString);

	void ArchiveCSOrder(CCSOrder* pOrder,
						int seqNum,
						int strandNum, 
						CCastString* pString);

	void WriteStringEntry(CCastString* pString, bool useOrig);

	void ArchiveTemplate(CCastString* pString);

	void WriteTemplateSortFieldEntry(COrderSelection& os,
									 CCastString* pString,
									 COrderSelection::E_SortField field,
									 int seqNum,
									 CString type);

	long LoadCasterScen(long id);
	void CreateBlankCasterScen(long id);
	void LoadStrings();
	void LoadString();
	void LoadTemplates();
	void LoadTemplate(CCastString* pString);
	bool LoadStringFromMasterScen(CCasterScen* pScen,
								  int year,
								  int week,
								  int caster,
								  int stringId);
	bool LoadStringFromMasterScen();

	void DeletePriorVersions();

};



class CCasterScenArchiver  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Responsible for reading/writing scenarios to the database
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	friend class CCasterScenArchiverAux;
		// this class tightly coupled


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

private:

	// do not implement
	// this is a static only class

	CCasterScenArchiver();
	virtual ~CCasterScenArchiver();
	CCasterScenArchiver(const CCasterScenArchiver&);


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

	static void AssignCasterScenId(CCasterScen* pScen);
	static void UpdateCasterScenInfo(CCasterScen* pScen);
	static void Archive(CCasterScen* pScen,bool notify);
	static void ArchiveStringToMasterScen(CCasterScen* pScen,
										  CCastString* pString);
	static bool LocateCasterScenRecord(CCasterScenariosSet& rs,
									   int openType,
									   int openOptions,
									   CCasterScen* pScen);
	static bool LocateCasterScenRecord(CCasterScenariosSet& rs, 
									   int openType,
									   int openOptions,
									   long id);
	static void GetAvailScens(vector<CAvailCasterScenId>& scens,
							  bool withSaves);

	static void DeleteCasterScen(long id);
	static long LoadCasterScen(long id);
	static bool LoadStringFromMasterScen(CCasterScen* pScen,
								  const CCastStringId& id);
	static bool LoadStringFromMasterScen(CCasterScen* pScen,
										 int year, 
										 int week, 
										 int caster, 
										 int stringId);

	static bool MaybeCreatePublicScensForUser(const CString& user);


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	static long GetNewCasterScenId(int caster);
};



#endif // !defined(AFX_CASTERSCENARCHIVER_H__C6D2F272_46B9_11D1_804A_006097B38214__INCLUDED_)
