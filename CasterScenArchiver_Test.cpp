// CasterScenArchiver.cpp: implementation of the CCasterScenArchiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "CasterScenArchiver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DBConn.h"
#include "CasterScenariosSet.h"
#include "CasterStringsSet.h"
#include "CasterStringOrdersSet.h"
#include "CasterStringHeatsSet.h"
#include "CasterStringHeatChemsSet.h"
#include "CasterStringTemplatesSet.h"
#include "CasterStringTemplateChemsSet.h"
#include "CasterStringTemplateSortsSet.h"
#include "CasterStringTemplateSpecsSet.h"
#include "CsdaEventLogSet2.h"
#include "CasterScen.h"
#include "CSOrder.h"
#include "CastString.h"
#include "UserInfo.h"
#include "ScenMgr.h"
#include "Snapshot.h"
#include "MiscConstants.h"
#include "MsgDlg.h"
#include "CasterScenariosMaxIdSet.h"


//	There are two classes interwined here.
//
//  CCasterScenArchiver -- a static-only class that provides the top-level
//		functions for archiving caster scenarios and recreating them
//		from the archive.
//
//	CCasterScenArchiverAux -- an auxiliary class used by CCasterScenArchiver
///		It encapsulates certain information needed across multiple method 
//		calls during archiving.  CCasterScenArchiver creates and initializes
//		one of these things in order to do an archive write or read of an
//		entire scenario, then passes the work off to one its methods.



////////////////////////////////////////////////////////////////
//	
//	CCasterScenArchiver
//	
////////////////////////////////////////////////////////////////

//
//  When creating a new caster scenario, 
//		we need to have an id assigned.
//

void CCasterScenArchiver::AssignCasterScenId(CCasterScen* pScen)
{
	pScen->SetId(GetNewCasterScenId(pScen->Caster()));
}



//
//  We generate a new id by creating a new record in the CasterScenarios table
//		and determining the id of that record.
//

long CCasterScenArchiver::GetNewCasterScenId(int caster)
{
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA	= dbc.GetDB();
	CCasterScenariosSet rs(pDBCSDA);
	CCasterScenariosMaxIdSet rs1(pDBCSDA);

	try
	{
		rs.m_pDatabase->BeginTrans();

		rs.Open();

		rs.AddNew();

		rs.m_Caster		= caster;
		rs.m_Owner		= CUserInfo::TheUser.LoginName();
		rs.m_CreateTime	= CTime::GetCurrentTime();
		rs.m_Version	= 1;
		rs.m_IsPublic	= false;

		rs.Update();

		// Let's try to find out that record number
		
		rs1.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
		long id = ( rs1.IsEOF() ? 0 : rs1.m_MaxOfId );
		rs1.Close();

		rs.m_pDatabase->CommitTrans();

		rs.Close();

		return id;
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

	   if ( rs1.IsOpen() )
		   rs1.Close();

	   if ( rs.IsOpen() )
		   rs.Close();

	   //  If we fail, generate some large random number

	   return (long(rand())<<16)+rand();
	}
}



//
//  Some basic information about a caster scenario has been changed,
//		we need to update the database.
//

void CCasterScenArchiver::UpdateCasterScenInfo(CCasterScen* pScen)
{
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA	= dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);

	try
	{

		if ( ! LocateCasterScenRecord(rs,
			                          CRecordset::dynaset,
									  CRecordset::none,
									  pScen) ) {
			if ( rs.IsOpen() ) 
				rs.Close();
			return;
		}

		rs.Edit();
		rs.m_Owner = pScen->Owner();
		rs.m_Comment = pScen->Comment();
		rs.Update();

		rs.Close();
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

	   if ( rs.IsOpen() )
		   rs.Close();
	}
}



//
//  Each user has archived scenarios marked as public.
//  (Not currently used, but perhaps someday.)
//
//  Make sure these exist.
//


//static 
bool CCasterScenArchiver::MaybeCreatePublicScensForUser(const CString& user)
{
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA	= dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);
	rs.m_strFilter.Format("[IsPublic] = Yes and [Owner] = '%s'",
						  LPCTSTR(user));

	bool retval = true;

	try {

		rs.Open(CRecordset::dynaset,0,CRecordset::none);

		bool publicSeen[4] = { false, false, false, false };

		while ( ! rs.IsEOF() ) {

			if ( 1 <= rs.m_Caster  && rs.m_Caster <= 3 )
				publicSeen[rs.m_Caster] = true;

			rs.MoveNext();
		}
	
		for ( int i=1; i<=3; ++i ) {
			if ( ! publicSeen[i] ) {

				rs.AddNew();

				rs.m_Caster		= i;
				rs.m_Owner		= user;
				rs.m_CreateTime	= CTime::GetCurrentTime();
				rs.m_Version	= 1;
				rs.m_IsPublic	= true;
				rs.m_Comment	= "Fit for public consumption";
	
				rs.Update();
			}
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   retval = false;
	}

	if ( rs.IsOpen() )
		rs.Close();


	return retval;
}




//
//  Archive a scenario.
//  Delegate the work to an Aux.
//


void CCasterScenArchiver::Archive(CCasterScen* pScen, bool notify)
{
	CWaitCursor wc;

	CCasterScenArchiverAux aux;
	
	aux.Archive(pScen,notify);

	pScen->SetModified(false);
}
	

//
//  When a string is 910'd, we need to save it in the master scenario.
//
//  Delegate the work to an Aux.
//

void CCasterScenArchiver::ArchiveStringToMasterScen(CCasterScen* pScen,
													CCastString* pString)
{
	CWaitCursor wc;
	CCasterScenArchiverAux aux;

	aux.ArchiveStringToMasterScen(pScen,pString);
}



//
//  Someday...
//

//static
void CCasterScenArchiver::DeleteCasterScen(long id)
{
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	pDBCSDA->BeginTrans();
	
	CString sql;

	sql.Format("DELETE FROM CasterStrings "
			   " WHERE [ScenId] = %d ",
			   id);
//	pDBCSDA->SQLExecDirect(hstmt,sql,SQL_NTS);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeats "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeatChems "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringOrders "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplates "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateChems "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSorts "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSpecs "
			   " WHERE [ScenId] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterScenarios "
			   " WHERE [Id] = %d ",
			   id);

	pDBCSDA->ExecuteSQL(sql);



	pDBCSDA->CommitTrans();

	CDatabase Cancel();
}



//
//  Load in an archived scenario
//

//static
long CCasterScenArchiver::LoadCasterScen(long id)
{
	CWaitCursor wc;
	
	CCasterScenArchiverAux aux;

	return aux.LoadCasterScen(id);
}



//
//  Load a string archived into the master scenario
//

// static 
bool CCasterScenArchiver::LoadStringFromMasterScen(CCasterScen* pScen,
												   const CCastStringId& id)
{
	return LoadStringFromMasterScen(pScen,id.Year(),id.Week(),id.Caster(),id.StringId());
}


// static 
bool CCasterScenArchiver::LoadStringFromMasterScen(CCasterScen* pScen,
												   int year, 
												   int week, 
												   int caster, 
												   int stringId)
{
	CWaitCursor wc;

	CCasterScenArchiverAux aux;

	return aux.LoadStringFromMasterScen(pScen,year,week,caster,stringId);
}



//
//  A utility function whose sole purpose in life
//		is to find the record associated with a given caster scenario.
//

//static
bool CCasterScenArchiver::LocateCasterScenRecord(CCasterScenariosSet& rs, 
												 int openType,
												 int openOptions,
												 CCasterScen* pScen)
{
	return LocateCasterScenRecord(rs,openType,openOptions,pScen->Id());
}



//static
bool CCasterScenArchiver::LocateCasterScenRecord(CCasterScenariosSet& rs, 
												 int openType,
												 int openOptions,
												 long id)
{
	try
	{
		rs.m_strFilter.Format("id = %ld",id);
		rs.Open(openType,
				0,	//query,
				openOptions);

		if ( rs.IsEOF() && rs.IsBOF() ) {
			CString msg;
			msg.Format("Error: No entry in database for CasterScen # %d\n"
				       "Contact system maintainer.",
						id);
			AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
			return false;
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   return false;
	}

	return true;
}






////////////////////////////////////////////////////////////////
//	
//	CCasterScenArchiverAux
//	
////////////////////////////////////////////////////////////////


//
//  initialize to nominal values
//

CCasterScenArchiverAux::CCasterScenArchiverAux()
{
	m_pScen			=  0;
	m_version		= -1;

	m_pDBCSDA		=  0;

	m_pStringsSet		=  0;
	m_pOrdersSet		=  0;
	m_pHeatsSet			=  0;
	m_pHeatChemsSet		=  0;
	m_pTemplatesSet		=  0;
	m_pTemplateSpecsSet	=  0;
	m_pTemplateChemsSet	=  0;
	m_pTemplateSortsSet	=  0;
	m_pEventLogSet		=  0;

}




CCasterScenArchiverAux::~CCasterScenArchiverAux()
{
	// do nothing
}




void CCasterScenArchiverAux::DeleteRecordset(CRecordset* prs)
{
	if ( prs != 0  ) {
		
		if ( prs->IsOpen() )
			prs->Close();

		delete prs;
	}
}




////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////


//
//  This is the master Archive method.
//
//  Whenever a caster scenario is archived,
//		it receives a new version number.
//  The current version is retrieved from the database,
//		incremented there, and used as a key field in
//		all data stored.
//
//  I got tired of passing 14 variables to every method involved
//		in this archive process, so certain variables are cached
//		in the object:.
//
//	pScen		  -- the caster scenario being archived
//  version		  -- the version to use during this save
//  pStringsSet	  -- a recordset to write to table CasterStrings
//  pOrdersSet	  -- a recordset to write to table CasterStringOrders
//	pHeatsSet	  -- a recordset to write to table CasterStringHeats
//	pHeatChemsSet -- a recordset to write to table CasterStringHeatChems
//   etc.
//
//	These need to be passed to almost every method.
//  The recordsets are the biggest problem, since in the future more may be needed.
//  They persist over several calls to subordinate functions, so caching seems
//  to be the correct way to go.
//

void CCasterScenArchiverAux::Archive(CCasterScen* pScen_,bool notify)
{
	m_pScen = pScen_;

	IncrementVersion();

	if (m_version == -1 ) 
		return;

	CMsgDlg* pMsgDlg = new CMsgDlg;

	if ( notify ) {
		pMsgDlg->m_title = "Save in progress ... ";
		pMsgDlg->m_message = "\nBeginning save ... \n\n";
		pMsgDlg->Create(0);
	}

	try 
	{
		pMsgDlg->AddText("Connecting to database.\n");
		
		{
			CDBConn dbc(CDBConn::DB_CSDA);
			dbc.Connect();
			CDatabase* pDBCSDA = dbc.GetDB();
		
			CreateRecordsets(pDBCSDA);
			pDBCSDA->BeginTrans();
			OpenRecordsetsForWriting();
	
			ArchiveStrings(pMsgDlg);
	
			pMsgDlg->AddText("Closing tables.\n");
			CloseRecordsets();
		
			pMsgDlg->AddText("Committing transaction.\n");
			pDBCSDA->CommitTrans();
		}

		pMsgDlg->AddText("Deleting prior versions ... ");
		DeletePriorVersions();

		pMsgDlg->AddText(" done.\n");

	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	}

	DestroyRecordsets();

	if ( pMsgDlg->m_hWnd != 0 )
		pMsgDlg->DestroyWindow();

}



void CCasterScenArchiverAux::ArchiveStringToMasterScen(CCasterScen* pScen,
													   CCastString* pString)
{
	CMsgDlg msgDlg;
	msgDlg.m_title = "Archive string to master -- in progress ... ";
	msgDlg.m_message = "\nBeginning save ... \n\n";
	msgDlg.Create(0);
	try 
	{
		msgDlg.AddText("Connecting to database.\n");
		
		{
			CDBConn dbc(CDBConn::DB_CSDA);
			dbc.Connect();
			CDatabase* pDBCSDA = dbc.GetDB();
		
			CreateRecordsets(pDBCSDA);
			pDBCSDA->BeginTrans();
			OpenRecordsetsForWriting();
	
			m_version = 1;
			m_pScen = CScenMgr::CreateMasterCasterScen(pScen->Caster());
			DeleteStringFromMasterScen(pDBCSDA,pString,&msgDlg);
			ArchiveString(pString,&msgDlg);
			Log910Event(pString,&msgDlg);
			CScenMgr::DeleteMasterCasterScen(m_pScen);
	
			msgDlg.AddText("Closing tables.\n");
			CloseRecordsets();
	
			msgDlg.AddText("Committing transaction.\n");
			pDBCSDA->CommitTrans();
		}
		msgDlg.AddText(" done.\n");
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	}

	DestroyRecordsets();

	msgDlg.DestroyWindow();
}



void CCasterScenArchiverAux::CreateRecordsets(CDatabase* pDBCSDA)
{
	m_pDBCSDA			= pDBCSDA;

	m_pStringsSet		= new CCasterStringsSet(pDBCSDA);
	m_pOrdersSet		= new CCasterStringOrdersSet(pDBCSDA);
	m_pHeatsSet			= new CCasterStringHeatsSet(pDBCSDA);
	m_pHeatChemsSet		= new CCasterStringHeatChemsSet(pDBCSDA);
	m_pTemplatesSet		= new CCasterStringTemplatesSet(pDBCSDA);
	m_pTemplateSpecsSet	= new CCasterStringTemplateSpecsSet(pDBCSDA);
	m_pTemplateChemsSet	= new CCasterStringTemplateChemsSet(pDBCSDA);
	m_pTemplateSortsSet	= new CCasterStringTemplateSortsSet(pDBCSDA);
	m_pEventLogSet		= new CCsdaEventLogSet2(pDBCSDA);

	m_pStringsSet		->m_nParams = 0;
	m_pOrdersSet		->m_nParams = 0;
	m_pHeatsSet			->m_nParams = 0;
	m_pHeatChemsSet		->m_nParams = 0;
	m_pTemplatesSet		->m_nParams = 0;
	m_pTemplateSpecsSet	->m_nParams = 0;
	m_pTemplateChemsSet	->m_nParams = 0;
	m_pTemplateSortsSet	->m_nParams = 0;
	m_pEventLogSet		->m_nParams = 0;
}

void CCasterScenArchiverAux::OpenRecordsetsForWriting()
{

	m_pStringsSet		->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pOrdersSet		->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pHeatsSet			->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pHeatChemsSet		->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pTemplatesSet		->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pTemplateSpecsSet	->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pTemplateChemsSet	->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pTemplateSortsSet	->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
	m_pEventLogSet		->Open(CRecordset::dynaset,0,CRecordset::appendOnly);
}



#ifdef SET_RS_PARMS2
#undef SET_RS_PARMS2
#endif

#define SET_RS_PARMS2(rs,filter,sort,id,version) \
	rs->m_nParams = 2; \
	rs->m_strFilter = (filter); \
	rs->m_strSort = (sort); \
	rs->m_paramScenId = (id); \
	rs->m_paramVersion = (version)

#ifdef SET_RS_PARMS6
#undef SET_RS_PARMS6
#endif

#define SET_RS_PARMS6(rs,filter,sort,id,version,year,week,caster,stringId) \
	SET_RS_PARMS2(rs,filter,sort,id,version); \
	rs->m_nParams = 6; \
	rs->m_paramYear = (year); \
	rs->m_paramWeek = (week); \
	rs->m_paramCaster = (caster); \
	rs->m_paramStringId = (stringId)




void CCasterScenArchiverAux::OpenRecordsetsForReadingScenario(long id, long version)
{
	CString filter = "[ScenId] = ? AND [Version] = ? ";
	CString sort1  = "[Caster], [Year], [Week],  [StringId] ";
	CString sort2  = "[StringCaster], [StringYear], [StringWeek],  [StringId] ";

	SET_RS_PARMS2( m_pStringsSet,		filter, sort1,						id, m_version );
	SET_RS_PARMS2( m_pOrdersSet,		filter, sort2 + ", [SeqNum]",		id, m_version );
	SET_RS_PARMS2( m_pHeatsSet,			filter, sort2 + ", [SeqNum]",		id, m_version );
	SET_RS_PARMS2( m_pHeatChemsSet,		filter, sort1 + ", [HeatSeqNum]",	id, m_version );
	SET_RS_PARMS2( m_pTemplatesSet,		filter, sort1,						id, m_version );
	SET_RS_PARMS2( m_pTemplateChemsSet,	filter, sort1 + ", [Element]",		id, m_version );
	SET_RS_PARMS2( m_pTemplateSpecsSet, filter, sort1 + ", [SeqNum]",		id, m_version );
	SET_RS_PARMS2( m_pTemplateSortsSet, filter, sort1 + ", [SeqNum]",		id, m_version );
	m_pStringsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pOrdersSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pHeatsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pHeatChemsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pTemplatesSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pTemplateChemsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pTemplateSpecsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pTemplateSortsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
}




void CCasterScenArchiverAux::OpenRecordsetsForReadingStringFromMasterScen(int year, 
																		  int week,
																		  int caster,
																		  int stringId)
{
	CString filter1 = "[ScenId] = ? AND [Version] = ? AND [Year] = ? AND [Week] = ? AND [Caster] = ? AND [StringId] = ?";
	CString filter2 = "[ScenId] = ? AND [Version] = ? AND [StringYear] = ? AND [StringWeek] = ? AND [StringCaster] = ? AND [StringId] = ?";
	CString sort1  = "[Caster], [Year], [Week],  [StringId] ";
	CString sort2  = "[StringCaster], [StringYear], [StringWeek],  [StringId] ";
	int id = caster;

	SET_RS_PARMS6( m_pStringsSet,		filter1, sort1,						id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pOrdersSet,		filter2, sort2 + ", [SeqNum]",		id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pHeatsSet,			filter2, sort2 + ", [SeqNum]",		id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pHeatChemsSet,		filter1, sort1 + ", [HeatSeqNum]",	id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pTemplatesSet,		filter1, sort1,						id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pTemplateChemsSet,	filter1, sort1 + ", [Element]",		id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pTemplateSpecsSet, filter1, sort1 + ", [SeqNum]",		id, m_version, year, week, caster, stringId );
	SET_RS_PARMS6( m_pTemplateSortsSet, filter1, sort1 + ", [SeqNum]",		id, m_version, year, week, caster, stringId );
	m_pStringsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
	m_pOrdersSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
CloseRecordsets();
	m_pHeatsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly); 
CloseRecordsets();

	m_pHeatChemsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
CloseRecordsets();

	m_pTemplatesSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
CloseRecordsets();

	m_pTemplateChemsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
CloseRecordsets();

	m_pTemplateSpecsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
CloseRecordsets();

	m_pTemplateSortsSet->Open(CRecordset::forwardOnly,0,CRecordset::readOnly);
}


#define MAYBE_CLOSE(x) if ( (x) != 0 && (x)->IsOpen() ) (x)->Close()
#define MAYBE_DESTROY(x) if ( (x) !=  0) { delete (x); (x) = 0; }

void CCasterScenArchiverAux::CloseRecordsets()
{
	MAYBE_CLOSE( m_pStringsSet			);
	MAYBE_CLOSE( m_pOrdersSet			);
	MAYBE_CLOSE( m_pHeatsSet			);
	MAYBE_CLOSE( m_pHeatChemsSet		);
	MAYBE_CLOSE( m_pTemplatesSet		);
	MAYBE_CLOSE( m_pTemplateSpecsSet	);
	MAYBE_CLOSE( m_pTemplateChemsSet	);
	MAYBE_CLOSE( m_pTemplateSortsSet	);
	MAYBE_CLOSE( m_pEventLogSet			);
}



void CCasterScenArchiverAux::DestroyRecordsets()
{
	CloseRecordsets();

	MAYBE_DESTROY( m_pStringsSet		);
	MAYBE_DESTROY( m_pOrdersSet			);
	MAYBE_DESTROY( m_pHeatsSet			);
	MAYBE_DESTROY( m_pHeatChemsSet		);
	MAYBE_DESTROY( m_pTemplatesSet		);
	MAYBE_DESTROY( m_pTemplateSpecsSet	);
	MAYBE_DESTROY( m_pTemplateChemsSet	);
	MAYBE_DESTROY( m_pTemplateSortsSet	);
	MAYBE_DESTROY( m_pEventLogSet		);
}


void CCasterScenArchiverAux::IncrementVersion()
{
	m_version = -1;

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);

	try 
	{
		if ( ! CCasterScenArchiver::LocateCasterScenRecord(rs,
														   CRecordset::dynaset,
														   CRecordset::none,
														   m_pScen) ) {
			if ( rs.IsOpen() ) 
				rs.Close();

			return;
		}
		
		rs.Edit();

		rs.m_LastSaveTime = CTime::GetCurrentTime();

		++rs.m_Version;

		m_version = rs.m_Version;

		rs.Update();

	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   m_version = -1;
	}
	
	if ( rs.IsOpen() )
		rs.Close();

}


// Cascading deletes in the database used to do the work for us.


void CCasterScenArchiverAux::DeletePriorVersions()
{
	if ( m_version == -1 )
		return;

	//CDatabase* pDBCSDA	= CDBMgr::GetDB(CDBMgr::DB_CSDA);
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	pDBCSDA->BeginTrans();
	
	CString sql;

	int id = m_pScen->Id();
	int version = m_version - CMiscConstants::ArchiveVersionRetentionCount();


	sql.Format("DELETE FROM CasterStrings "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeats "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeatChems "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringOrders "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplates "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateChems "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSorts "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSpecs "
			   " WHERE [ScenId] = %d "
			   "   AND [Version] <= %d ",
			   id,version);

	pDBCSDA->ExecuteSQL(sql);

	pDBCSDA->CommitTrans();
}



void CCasterScenArchiverAux::DeleteStringFromMasterScen(CDatabase* pDBCSDA,
														CCastString* pString,
														CMsgDlg* pDlg)

{
	if ( m_version == -1 )
		return;

	pDlg->AddText("Deleting prior string entry from master scenario ... ");

	CString sql;
	int id = m_pScen->Id();
	int year		= pString->Id().Year();
	int week		= pString->Id().Week();
	int caster		= pString->Id().Caster();
	int stringId	= pString->Id().StringId();
	CString where1;
	CString where2;
	where1.Format(" WHERE [ScenId] = %d "
				 "   AND [Year] = %d "
				 "   AND [Week] = %d "
				 "   AND [Caster] = %d "
				 "   AND [StringId] = %d ",
				 id,year,week,caster,stringId);

	// Sigh
	where2.Format(" WHERE [ScenId] = %d "
				 "   AND [StringYear] = %d "
				 "   AND [StringWeek] = %d "
				 "   AND [StringCaster] = %d "
				 "   AND [StringId] = %d ",
				 id,year,week,caster,stringId);

	sql.Format("DELETE FROM CasterStrings %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeats %s",LPCTSTR(where2));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringHeatChems %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringOrders %s",LPCTSTR(where2));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplates %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateChems %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSorts %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	sql.Format("DELETE FROM CasterStringTemplateSpecs %s",LPCTSTR(where1));
	pDBCSDA->ExecuteSQL(sql);

	pDlg->AddText(" done.\n");
}



void CCasterScenArchiverAux::Log910Event(CCastString* pString,
										 CMsgDlg* pDlg)
{
	pDlg->AddText("Writing event log entry ... ");

	m_pEventLogSet->AddNew();
	m_pEventLogSet->m_EventType = "910";

	CString temp;
	temp.Format("%d %d %d %d",
				pString->Id().Caster(),
				pString->Id().Year(),
				pString->Id().Week(),									
				pString->Id().StringId());
	m_pEventLogSet->m_Comment = temp;

	m_pEventLogSet->m_EventTimestamp = CTime::GetCurrentTime();
	m_pEventLogSet->SetFieldDirty(0);	// all fields
	m_pEventLogSet->Update();

	pDlg->AddText("done.\n");
}


void CCasterScenArchiverAux::ArchiveStrings(CMsgDlg* pDlg)
{	
	vector<CCastString*>& strings = m_pScen->CastStrings();

	ostrstream ostr;

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {

		ostr << "Saving string " 
			 << (*is)->Id().Year() 
			 << "-"
			 << (*is)->Id().Week()
			 << "-"
			 << (*is)->Id().StringId()
			 << " ... "
			 << ends;

		pDlg->AddText( ostr.str() );
		ostr.freeze(false);
		ostr.seekp(0);

		ArchiveString((*is),pDlg);

		pDlg->AddText(" done.\n");
	}
}



void CCasterScenArchiverAux::ArchiveString(CCastString* pString,
										   CMsgDlg* pDlg)
{
	bool useOrig = pString->OrigString() != 0
				   &&
				   pString->IsSameString( pString->OrigString() );



	if ( ! useOrig ) {

		pDlg->AddText("+");
		WriteStringEntry(pString,useOrig);
		pDlg->AddText("H");
		ArchiveHeats(pString);
		pDlg->AddText("1");
		ArchiveStrand(1,pString);
		pDlg->AddText("2");
		ArchiveStrand(2,pString);
		pDlg->AddText("T");
		ArchiveTemplate(pString);

	}
}


void CCasterScenArchiverAux::ArchiveHeats(CCastString* pString)
{
	vector<CCastStringHeat>& heats = pString->Heats();

	int count = 0;

	for ( vector<CCastStringHeat>::iterator ih = heats.begin();
		  ih != heats.end();
		  ++ih, ++count ) 
		ArchiveHeat((*ih),count,pString);
}


void CCasterScenArchiverAux::ArchiveHeat(CCastStringHeat& rHeat,
										 int seqNum,
										 CCastString* pString)
{
	m_pHeatsSet->AddNew();

	m_pHeatsSet->m_ScenId		= m_pScen->Id();
	m_pHeatsSet->m_StringYear	= pString->Id().Year();
	m_pHeatsSet->m_StringWeek	= pString->Id().Week();
	m_pHeatsSet->m_StringCaster	= pString->Id().Caster();
	m_pHeatsSet->m_StringId		= pString->Id().StringId();
	m_pHeatsSet->m_Version		= m_version;
	m_pHeatsSet->m_SeqNum		= seqNum;
	m_pHeatsSet->m_TabNum		= rHeat.TabNum();
	m_pHeatsSet->m_Spec			= rHeat.Spec();
	m_pHeatsSet->m_CondnCode	= rHeat.CondnCode();
	m_pHeatsSet->m_DispCode		= rHeat.DispCode();
	m_pHeatsSet->m_IsMarked		= rHeat.IsMarked();
	m_pHeatsSet->m_Spec			= rHeat.Spec();

	CString strStatus;
	switch ( rHeat.Status() ) {
	case CCastStringHeat::HEAT_STATUS_910:
		strStatus = "910";
		break;

	case CCastStringHeat::HEAT_STATUS_909:
		strStatus = "909";
		break;

	case CCastStringHeat::HEAT_STATUS_NEW:
		strStatus = "NEW";
		break;
	}

	m_pHeatsSet->m_Status		= strStatus;


	m_pHeatsSet->m_Comment		= (rHeat.Comment().GetLength() > 80 
								   ? rHeat.Comment().Left(80)
								   : rHeat.Comment());

	m_pHeatsSet->Update();



	for ( int elt = 0; elt < CChem::NumElements; ++ elt ) {

		if ( rHeat.MinChem(CChem::Element(elt)) == 0 
			 && 
			 rHeat.MaxChem(CChem::Element(elt)) == 100.0 )
			continue;

		m_pHeatChemsSet->AddNew();

		m_pHeatChemsSet->m_ScenId		= m_pScen->Id();
		m_pHeatChemsSet->m_Version		= m_version;
		m_pHeatChemsSet->m_Year			= pString->Id().Year();
		m_pHeatChemsSet->m_Week			= pString->Id().Week();
		m_pHeatChemsSet->m_Caster		= pString->Id().Caster();
		m_pHeatChemsSet->m_StringId		= pString->Id().StringId();
		m_pHeatChemsSet->m_HeatSeqNum	= seqNum;
		m_pHeatChemsSet->m_Element		= CChem::ToString(CChem::Element(elt));
		m_pHeatChemsSet->m_MinValue		= rHeat.MinChem(CChem::Element(elt));
		m_pHeatChemsSet->m_MaxValue		= rHeat.MaxChem(CChem::Element(elt));
		m_pHeatChemsSet->m_AimValue		= rHeat.AimChem(CChem::Element(elt));

		m_pHeatChemsSet->Update();
	}
}




void CCasterScenArchiverAux::ArchiveStrand(int strandNum, 
										   CCastString* pString)
{
	const vector<CCSOrder*>& csOrders = pString->Strand(strandNum);

	int count = 0;

	for ( vector<CCSOrder*>::const_iterator io = csOrders.begin();
		  io != csOrders.end();
		  ++io, ++count ) 
		ArchiveCSOrder((*io),count,strandNum,pString);
}


void CCasterScenArchiverAux::ArchiveCSOrder(CCSOrder* pOrder,
											int seqNum,
											int strandNum, 
											CCastString* pString)
{
	m_pOrdersSet->AddNew();

	m_pOrdersSet->m_ScenId			= m_pScen->Id();
	m_pOrdersSet->m_StringYear		= pString->Id().Year();
	m_pOrdersSet->m_StringWeek		= pString->Id().Week();
	m_pOrdersSet->m_StringCaster	= pString->Id().Caster();
	m_pOrdersSet->m_StringId		= pString->Id().StringId();
	m_pOrdersSet->m_Version			= m_version;
	m_pOrdersSet->m_StrandNum		= strandNum;
	m_pOrdersSet->m_SeqNum			= seqNum;
	// FP change
	m_pOrdersSet->m_OrderNum		= atol(pOrder->FpOrderNum().Left(7));
	m_pOrdersSet->m_FpOrderNum		= pOrder->FpOrderNum();
	m_pOrdersSet->m_SlabWidth		= pOrder->SlabWidth();
	m_pOrdersSet->m_SlabLength		= pOrder->SlabLength();
	m_pOrdersSet->m_CondnCode		= pOrder->SlabCondnCode();
	m_pOrdersSet->m_NumPieces		= pOrder->NumPieces();
	m_pOrdersSet->m_LotSpec			= pOrder->LotSpec();
	m_pOrdersSet->m_ExposureCode	= pOrder->ExposureCode();
	m_pOrdersSet->m_ChangeFlag		= pOrder->ChangeFlag();
	m_pOrdersSet->m_DispCode		= pOrder->DispCode();
	m_pOrdersSet->m_SlitTypeCode	= pOrder->SlitTypeCode();
	m_pOrdersSet->m_PieceWgt		= pOrder->PieceWgt();
	m_pOrdersSet->m_SlabThickness	= pOrder->SlabThickness();
	m_pOrdersSet->m_HeatSeqNum		= pOrder->HeatSeqNum();
	m_pOrdersSet->m_PlanSteelLengthMin	= pOrder->PlanSteelLengthMin();
	m_pOrdersSet->m_PlanSteelLengthMax	= pOrder->PlanSteelLengthMax();

	m_pOrdersSet->Update();
}


void CCasterScenArchiverAux::WriteStringEntry(CCastString* pString, bool useOrig)
{
	m_pStringsSet->AddNew();

	m_pStringsSet->m_ScenId			= m_pScen->Id();
	m_pStringsSet->m_Version		= m_version;
	m_pStringsSet->m_Year			= pString->Id().Year();
	m_pStringsSet->m_Week			= pString->Id().Week();
	m_pStringsSet->m_Caster			= pString->Id().Caster();
	m_pStringsSet->m_StringId		= pString->Id().StringId();
	m_pStringsSet->m_NumHeats		= pString->Props().NumHeats();
	m_pStringsSet->m_AimHeatSize	= pString->AimHeatSize();
	m_pStringsSet->m_MinHeatSize	= pString->MinHeatSize();
	m_pStringsSet->m_MaxHeatSize	= pString->MaxHeatSize();
	m_pStringsSet->m_FreezeHeats	= true;
	m_pStringsSet->m_CanEdit		= pString->CanEdit();
	m_pStringsSet->m_UseOriginal	= useOrig;
	m_pStringsSet->m_HeatLockStatus = pString->HeatStatus();
	m_pStringsSet->m_NumHeatsLocked = pString->NumLockedHeats();
	m_pStringsSet->m_StartWidth1 	= pString->Props().StartWidth(1);
	m_pStringsSet->m_StartWidth2	= pString->Props().StartWidth(2);
	m_pStringsSet->m_EndWidth1		= pString->Props().EndWidth(1);
	m_pStringsSet->m_EndWidth2		= pString->Props().EndWidth(2);
	m_pStringsSet->m_Spec			= pString->Props().InitSpec();
	m_pStringsSet->m_MinCarbon		= pString->Props().MinCarbon();
	m_pStringsSet->m_MaxCarbon		= pString->Props().MaxCarbon();
	m_pStringsSet->m_MinMn			= pString->Props().MinMn();
	m_pStringsSet->m_MaxMn			= pString->Props().MaxMn();
	m_pStringsSet->m_StartTime		= pString->Props().StartTime();
	m_pStringsSet->m_IsValidStartTime	= pString->Props().IsStartTimeValid();
	m_pStringsSet->m_TurnaroundTime	= pString->Props().TurnaroundTime().GetTotalMinutes();


	CString strStatus;
	switch ( pString->Status() ) {
	case CCastString::STRING_STATUS_910:
		strStatus = "910";
		break;

	case CCastString::STRING_STATUS_909:
		strStatus = "909";
		break;

	case CCastString::STRING_STATUS_NEW:
		strStatus = "NEW";
		break;
	}

	m_pStringsSet->m_Status	= strStatus;

	if ( pString->OrigString() == 0 ) {
		m_pStringsSet->m_OrigStringYear		= 0;
		m_pStringsSet->m_OrigStringWeek		= 0;
		m_pStringsSet->m_OrigStringStringId	= 0;
	}
	else {
		m_pStringsSet->m_OrigStringYear		= pString->OrigString()->Id().Year();
		m_pStringsSet->m_OrigStringWeek		= pString->OrigString()->Id().Week();
		m_pStringsSet->m_OrigStringStringId	= pString->OrigString()->Id().StringId();
	}

	m_pStringsSet->Update();
}



void CCasterScenArchiverAux::ArchiveTemplate(CCastString* pString)
{

	COrderSelection& os = pString->OrderSelection();

	m_pTemplatesSet->AddNew();

	m_pTemplatesSet->m_ScenId				= m_pScen->Id();
	m_pTemplatesSet->m_Version				= m_version;
	m_pTemplatesSet->m_Year					= pString->Id().Year();
	m_pTemplatesSet->m_Week					= pString->Id().Week();
	m_pTemplatesSet->m_Caster				= pString->Id().Caster();
	m_pTemplatesSet->m_StringId				= pString->Id().StringId();
	m_pTemplatesSet->m_UseAllCICodes		= os.UseAllCICodes();
	m_pTemplatesSet->m_UseAllSpecs			= os.UseAllSpecs();
	m_pTemplatesSet->m_UseCrossApp			= os.UseCrossApp();
	m_pTemplatesSet->m_UseWidthRange		= os.UseWidthRange();
	m_pTemplatesSet->m_MinWidth				= os.MinWidth();
	m_pTemplatesSet->m_MaxWidth				= os.MaxWidth();
	m_pTemplatesSet->m_IncludeOrders		= os.IncludeOrders();
	m_pTemplatesSet->m_UseHrWkCurrentBack	= os.UseHrwk(COrderSelection::HR_WK_CURRENT_BACK);
	m_pTemplatesSet->m_UseHrWkPlus1			= os.UseHrwk(COrderSelection::HR_WK_PLUS_1);
	m_pTemplatesSet->m_UseHrWkPlus2			= os.UseHrwk(COrderSelection::HR_WK_PLUS_2);
	m_pTemplatesSet->m_UseHrWkPlus3			= os.UseHrwk(COrderSelection::HR_WK_PLUS_3);
	m_pTemplatesSet->m_UseHrWkPlus4			= os.UseHrwk(COrderSelection::HR_WK_PLUS_4);
	m_pTemplatesSet->m_UseHrWkFuture		= os.UseHrwk(COrderSelection::HR_WK_FUTURE);
	m_pTemplatesSet->m_CurrentHrYear		= os.CurrentHrYrWk().Year();	
	m_pTemplatesSet->m_CurrentHrWeek		= os.CurrentHrYrWk().Week();
//	m_pTemplatesSet->m_UsePstPrio			= os.UsePstRange(COrderSelection::PST_PRIO);
	m_pTemplatesSet->m_UsePstLt8			= os.UsePstRange(COrderSelection::PST_LT_8);
	m_pTemplatesSet->m_UsePst8to14			= os.UsePstRange(COrderSelection::PST_8_14);
	m_pTemplatesSet->m_UsePst15to21			= os.UsePstRange(COrderSelection::PST_15_21);
	m_pTemplatesSet->m_UsePst22to28			= os.UsePstRange(COrderSelection::PST_22_28);
	m_pTemplatesSet->m_UsePst29to35			= os.UsePstRange(COrderSelection::PST_29_35);
	m_pTemplatesSet->m_UsePstGt35			= os.UsePstRange(COrderSelection::PST_GT_35);

	m_pTemplatesSet->m_Use80HsmLuType		= os.Use80HsmLuType();
	m_pTemplatesSet->m_MegaLuType			= os.MegaLuType	();
	m_pTemplatesSet->m_UsePriorityCode		= os.UsePriorityCode();
	m_pTemplatesSet->m_MaxCIPriority		= os.MaxCIPriority();
	m_pTemplatesSet->m_MinCIPriority		= os.MinCIPriority();
	m_pTemplatesSet->m_IncludeNewOrders		= os.IncludeNewOrders();

	m_pTemplatesSet->Update();

	const vector<CString>& specs = os.SelectedSpecs();
	
	int iscount = 0;
	for ( vector<CString>::const_iterator is = specs.begin();
		  is != specs.end();
		  ++is, ++iscount ) {

		m_pTemplateSpecsSet->AddNew();

		m_pTemplateSpecsSet->m_ScenId	= m_pScen->Id();
		m_pTemplateSpecsSet->m_Version	= m_version;
		m_pTemplateSpecsSet->m_Year		= pString->Id().Year();
		m_pTemplateSpecsSet->m_Week		= pString->Id().Week();
		m_pTemplateSpecsSet->m_Caster	= pString->Id().Caster();
		m_pTemplateSpecsSet->m_StringId	= pString->Id().StringId();
		m_pTemplateSpecsSet->m_SeqNum	= iscount;
		m_pTemplateSpecsSet->m_Spec		= (*is);

		m_pTemplateSpecsSet->Update();
	}

	for ( int elt = 0; elt < CChem::NumElements; ++ elt ) {

		if ( os.MinChem(elt) == 0 
			 && 
			 os.MinChem(elt) == 100.0 )
			continue;


		m_pTemplateChemsSet->AddNew();

		m_pTemplateChemsSet->m_ScenId	= m_pScen->Id();
		m_pTemplateChemsSet->m_Version	= m_version;
		m_pTemplateChemsSet->m_Year		= pString->Id().Year();
		m_pTemplateChemsSet->m_Week		= pString->Id().Week();
		m_pTemplateChemsSet->m_Caster	= pString->Id().Caster();
		m_pTemplateChemsSet->m_StringId	= pString->Id().StringId();
		m_pTemplateChemsSet->m_Element	= CChem::ToString(CChem::Element(elt));
		m_pTemplateChemsSet->m_MinValue	= os.MinChem(elt);
		m_pTemplateChemsSet->m_MaxValue	= os.MaxChem(elt);

		m_pTemplateChemsSet->Update();
	}


	const COrderSelection::T_SortVec& groups = os.GroupFields();
	const COrderSelection::T_SortVec& sorts  = os.SortFields();

	int fcount = 0;
	for ( COrderSelection::T_SortVec::const_iterator ig = groups.begin();
		  ig != groups.end();
		  ++ig, ++fcount)

		WriteTemplateSortFieldEntry(os,
									pString,
									COrderSelection::E_SortField((*ig)),
									fcount,
									"G");

	for ( COrderSelection::T_SortVec::const_iterator ir = sorts.begin();
		  ir != sorts.end();
		  ++ir, ++fcount)

		WriteTemplateSortFieldEntry(os,
									pString,
									COrderSelection::E_SortField((*ir)),
									fcount,
									"S");

}


void CCasterScenArchiverAux::WriteTemplateSortFieldEntry(COrderSelection& os,
														 CCastString* pString,
														 COrderSelection::E_SortField field,
														 int seqNum,
														 CString type)
{
	m_pTemplateSortsSet->AddNew();

	m_pTemplateSortsSet->m_ScenId		= m_pScen->Id();
	m_pTemplateSortsSet->m_Version		= m_version;
	m_pTemplateSortsSet->m_Year			= pString->Id().Year();
	m_pTemplateSortsSet->m_Week			= pString->Id().Week();
	m_pTemplateSortsSet->m_Caster		= pString->Id().Caster();
	m_pTemplateSortsSet->m_StringId		= pString->Id().StringId();
	m_pTemplateSortsSet->m_SeqNum		= seqNum;
	m_pTemplateSortsSet->m_SortField	= COrderSelection::SortFieldString(field);
	m_pTemplateSortsSet->m_SortType		= type;

	m_pTemplateSortsSet->Update();
}




////////////////////////////////////////////////////////////////
//	
//	Available scenarios
//	
////////////////////////////////////////////////////////////////



void CCasterScenArchiver::GetAvailScens(vector<CAvailCasterScenId>& scens,
										bool withSaves)
{
	scens.clear();

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);

	try
	{
		
		rs.m_strSort = "[Caster], [Owner], [Id]";

		for ( rs.Open(); ! rs.IsEOF(); rs.MoveNext() ) {

			if ( ! withSaves 
				 ||
				 ! rs.IsFieldNull(&rs.m_LastSaveTime) ) {

				CAvailCasterScenId scen;
				scen.Id()			= rs.m_Id;
				scen.Caster()		= rs.m_Caster;
				scen.Owner()		= rs.m_Owner;
				scen.CreateTime()	= rs.m_CreateTime;
				scen.LastSaveTime()	= rs.m_LastSaveTime;
				scen.Version()		= rs.m_Version;
				scen.Comment()		= rs.m_Comment;
				scen.IsPublic()		= rs.m_IsPublic;
				scen.IsLoaded()		= (CScenMgr::CasterScenById(scen.Caster(),scen.Id()) != 0 );
				
				scens.push_back(scen);
			}
		}
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	}

	if ( rs.IsOpen() )
		rs.Close();
}




////////////////////////////////////////////////////////////////
//	
//	Loading caster scenario
//	
////////////////////////////////////////////////////////////////



long CCasterScenArchiverAux::LoadCasterScen(long id)
{

	//  This will set up m_pScen and m_version
	CreateBlankCasterScen(id);

	if ( m_pScen == 0 ) 
		//  There was a problem -- get outa here.
		return 0;

	bool isOk = true;
	
	try 
	{
		CDBConn dbc(CDBConn::DB_CSDA);
		dbc.Connect();
		CDatabase* pDBCSDA = dbc.GetDB();


		CreateRecordsets(pDBCSDA);
		OpenRecordsetsForReadingScenario(id,m_version);
		LoadStrings();
		LoadTemplates();
	
		CloseRecordsets();
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   isOk = false;
	}

	DestroyRecordsets();

	if ( isOk )
		return m_pScen->Id();
	else
		return 0;
}






bool CCasterScenArchiverAux::LoadStringFromMasterScen(CCasterScen* pScen,
													  int year,
													  int week,
													  int caster,
													  int stringId)
{
	m_pScen = pScen;
	long origId = m_pScen->Id();
	m_pScen->SetIdBoldly(caster);

	m_version = 1;

	bool isOk = true;

	try 
	{
		CDBConn dbc(CDBConn::DB_CSDA);
//		dbc.Close();    // khh 10-2-02
		dbc.Connect();
		CDatabase* pDBCSDA = dbc.GetDB();


		CreateRecordsets(pDBCSDA);
		OpenRecordsetsForReadingStringFromMasterScen(year,week,caster,stringId);
		LoadStringFromMasterScen();
	
// khh 12-6-02		CloseRecordsets();
		//virtual CDBConn::close();
//		CDBConn close();
		//		dbc.close();
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   isOk = false;
	}

	DestroyRecordsets();

	m_pScen->SetIdBoldly(origId);

	if ( isOk )
		return m_pScen->Id();
	else
		return 0;
}


void CCasterScenArchiverAux::CreateBlankCasterScen(long id)
{
	m_version	= -1;
	m_pScen		= 0;

	//CDatabase* pDBCSDA	= CDBMgr::GetDB(CDBMgr::DB_CSDA);
	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);

	try 
	{

		if ( ! CCasterScenArchiver::LocateCasterScenRecord(rs,
														   CRecordset::forwardOnly,
														   CRecordset::readOnly,
														   id) ) {
			if ( rs.IsOpen() )
				rs.Close();
			return;
		}
		
		m_version			= rs.m_Version;
		int caster			= rs.m_Caster;
		CString origComment	= rs.m_Comment;
		CString origOwner	= rs.m_Owner;

		rs.Close();

		// Are we creating this under the original id or under a new one?

		if ( CScenMgr::CasterScenById(caster,id) != 0
			 ||
			 origOwner != CUserInfo::TheUser.LoginName() ) {
			m_pScen = CScenMgr::CreateCasterScen(caster,true);
			m_pScen->SetOwner(CUserInfo::TheUser.LoginName());
			CString comment;
			comment.Format("%s + Loaded from scenario #%d",LPCTSTR(origComment),id);
			m_pScen->SetComment(comment);
			CCasterScenArchiver::UpdateCasterScenInfo(m_pScen);
		}
		else
			m_pScen = CScenMgr::CreateCasterScen(caster,id);
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
	   m_version = -1;
	   m_pScen = 0;
	}

	if ( rs.IsOpen() )
		rs.Close();

}



void CCasterScenArchiverAux::LoadStrings()
{
	// m_pStringsSet and m_pOrdersSet have been created and are ready to go.
	//
	// Iterate through strings and correspondingly through orders on those strings.

	while ( ! m_pStringsSet->IsEOF() ) {

		LoadString();
		m_pStringsSet->MoveNext();

	}


	set<CCastString*> origStrings;

	{
		for ( vector<CCastString*>::iterator is = m_pScen->CastStrings().begin();
			  is != m_pScen->CastStrings().end();
			  ++is ) {
	
			if ( (*is)->OrigString() != 0 )
				origStrings.insert( (*is)->OrigString() );
		}
	}

	{
		CCasterScen* pSnapScen = TheSnapshot.SuperScen()->CasterScen(m_pScen->Caster());

		for ( vector<CCastString*>::iterator is = pSnapScen->CastStrings().begin();
			  is != pSnapScen->CastStrings().end();
			  ++is )

			if ( origStrings.find( (*is) ) == origStrings.end() )
				m_pScen->CastStrings().push_back( new CCastString( m_pScen, (**is), (*is) ) ); 
	}

	// TODO -- check for duplicate ids!!!!!
}




void CCasterScenArchiverAux::LoadString()
{
	CCasterScen* pSnapScen = TheSnapshot.SuperScen()->CasterScen(m_pStringsSet->m_Caster);

	CCastStringId origId(m_pStringsSet->m_OrigStringYear,
						 m_pStringsSet->m_OrigStringWeek,
						 m_pStringsSet->m_Caster,
						 m_pStringsSet->m_StringId);

	CCastString* pOrigString = 0;

	{
		for ( vector<CCastString*>::iterator is = pSnapScen->CastStrings().begin();
			  is != pSnapScen->CastStrings().end();
			  ++is ) {

			if ( (*is)->Id() == origId ) {
				pOrigString = (*is);
				break;
			}
		}
	}


	if ( pOrigString != 0 && m_pStringsSet->m_UseOriginal ) {

		CCastString* pCastString = new CCastString(m_pScen, *pOrigString, pOrigString);
		m_pScen->CastStrings().push_back(pCastString);
		return;
	}

	if ( m_pStringsSet->m_UseOriginal )
		return;


	CCastStringId id(m_pStringsSet->m_Year,
					 m_pStringsSet->m_Week,
					 m_pStringsSet->m_Caster,
					 m_pStringsSet->m_StringId);
		
	CCastStringMiscProps props(m_pStringsSet->m_NumHeats,
							   m_pStringsSet->m_MinHeatSize,
							   m_pStringsSet->m_AimHeatSize,
							   m_pStringsSet->m_MaxHeatSize,
							   CTimeSpan(0,0,m_pStringsSet->m_TurnaroundTime,0));

	props.SetStartWidth	( 1,	m_pStringsSet->m_StartWidth1	);
	props.SetStartWidth	( 2,	m_pStringsSet->m_StartWidth2	);
	props.SetEndWidth	( 1,	m_pStringsSet->m_EndWidth1	);
	props.SetEndWidth	( 2,	m_pStringsSet->m_EndWidth2	);
	props.SetInitSpec	(		m_pStringsSet->m_Spec, m_pStringsSet->m_Caster	);
	props.SetMinCarbon	(		m_pStringsSet->m_MinCarbon	);
	props.SetMaxCarbon	(		m_pStringsSet->m_MaxCarbon	);
	props.SetMinMn		(		m_pStringsSet->m_MinMn		);
	props.SetMaxMn		(		m_pStringsSet->m_MaxMn		);
	props.SetStartTime	(		m_pStringsSet->m_StartTime	);
	props.SetIsStartTimeValid(	m_pStringsSet->m_IsValidStartTime	);
	props.SetTurnaroundTime( CTimeSpan(0,0,m_pStringsSet->m_TurnaroundTime,0) );

	CCastString* pCastString = new CCastString(m_pScen,id,props);
	pCastString->OrigString( pOrigString);

//	pCastString->FreezeHeats(m_pStringsSet->m_FreezeHeats);
	
	int status = m_pStringsSet->m_HeatLockStatus;
	if ( status == CCastString::HEATS_ALL_LOCKED
		 ||
		 status == CCastString::HEATS_SOME_LOCKED
		 ||
		 status == CCastString::HEATS_NONE_LOCKED )
		 pCastString->HeatStatus( CCastString::E_HeatLockStatus(status) );
	else
		pCastString->HeatStatus(CCastString::HEATS_ALL_LOCKED);
	pCastString->NumLockedHeats(m_pStringsSet->m_NumHeatsLocked);
	pCastString->CanEdit(m_pStringsSet->m_CanEdit);
	pCastString->Status(m_pStringsSet->m_Status == "910"
						? CCastString::STRING_STATUS_910
						: m_pStringsSet->m_Status == "909"
						? CCastString::STRING_STATUS_909
						: CCastString::STRING_STATUS_NEW);

	if ( m_pStringsSet->m_HeatLockStatus < 0 || m_pStringsSet->m_HeatLockStatus > 2 )
		pCastString->HeatStatus(CCastString::HEATS_ALL_LOCKED);
	else
		pCastString->HeatStatus( CCastString::E_HeatLockStatus(m_pStringsSet->m_HeatLockStatus) );

	pCastString->NumLockedHeats( min(99,max(0,m_pStringsSet->m_NumHeatsLocked)) );


	m_pScen->CastStrings().push_back(pCastString);





	int count = 0;

	while ( ! m_pHeatsSet->IsEOF() ) {

		if ( m_pHeatsSet->m_StringYear != id.Year()
			 ||
			 m_pHeatsSet->m_StringWeek != id.Week()
			 ||
			 m_pHeatsSet->m_StringCaster != id.Caster()
			 ||
			 m_pHeatsSet->m_StringId != id.StringId() )
			 
			 break;

		CCastStringHeat heat(*m_pHeatsSet,
							 pCastString);

		while ( ! m_pHeatChemsSet->IsEOF() 
			    &&
				m_pHeatChemsSet->m_Year == id.Year()
				&&
				m_pHeatChemsSet->m_Week == id.Week()
				&&
				m_pHeatChemsSet->m_Caster == id.Caster()
				&&
				m_pHeatChemsSet->m_StringId == id.StringId()
				&&
				m_pHeatChemsSet->m_HeatSeqNum == count ) {
			
			CChem::Element elt;
			if ( CChem::ToElement(m_pHeatChemsSet->m_Element,elt) ) {
				heat.MinChem( elt ,m_pHeatChemsSet->m_MinValue);
				heat.MaxChem( elt ,m_pHeatChemsSet->m_MaxValue);
				heat.AimChem( elt, m_pHeatChemsSet->m_AimValue);
			}
			m_pHeatChemsSet->MoveNext();

		}

		pCastString->Heats().resize(m_pHeatsSet->m_SeqNum+1);
		pCastString->Heats()[m_pHeatsSet->m_SeqNum] = heat;			
		++count;

		m_pHeatsSet->MoveNext();
	}

	while ( ! m_pOrdersSet->IsEOF() ) {

		if ( m_pOrdersSet->m_StringYear != id.Year()
			 ||
			 m_pOrdersSet->m_StringWeek != id.Week()
			 ||
			 m_pOrdersSet->m_StringCaster != id.Caster()
			 ||
			 m_pOrdersSet->m_StringId != id.StringId() )
			 
			 break;


		CCSOrder* pCSOrder = new CCSOrder(*m_pOrdersSet);

		pCastString->AddCSOrder(pCSOrder);

		m_pOrdersSet->MoveNext();
	}

	pCastString->CalculateHeats();
	pCastString->CalculateSummary();
}





bool CCasterScenArchiverAux::LoadStringFromMasterScen()
{
	if ( m_pStringsSet->IsEOF() )
		return false;

	CCastStringId id(m_pStringsSet->m_Year,
					 m_pStringsSet->m_Week,
					 m_pStringsSet->m_Caster,
					 m_pStringsSet->m_StringId);
		
	CCastStringMiscProps props(m_pStringsSet->m_NumHeats,
							   m_pStringsSet->m_MinHeatSize,
							   m_pStringsSet->m_AimHeatSize,
							   m_pStringsSet->m_MaxHeatSize,
							   CTimeSpan(0,0,m_pStringsSet->m_TurnaroundTime,0));

	props.SetStartWidth	( 1,	m_pStringsSet->m_StartWidth1	);
	props.SetStartWidth	( 2,	m_pStringsSet->m_StartWidth2	);
	props.SetEndWidth	( 1,	m_pStringsSet->m_EndWidth1	);
	props.SetEndWidth	( 2,	m_pStringsSet->m_EndWidth2	);
	props.SetInitSpec	(		m_pStringsSet->m_Spec, m_pStringsSet->m_Caster	);
	props.SetMinCarbon	(		m_pStringsSet->m_MinCarbon	);
	props.SetMaxCarbon	(		m_pStringsSet->m_MaxCarbon	);
	props.SetMinMn		(		m_pStringsSet->m_MinMn		);
	props.SetMaxMn		(		m_pStringsSet->m_MaxMn		);
	props.SetStartTime	(		m_pStringsSet->m_StartTime	);
	props.SetIsStartTimeValid(	m_pStringsSet->m_IsValidStartTime	);
	props.SetTurnaroundTime( CTimeSpan(0,0,m_pStringsSet->m_TurnaroundTime,0) );


	CCastString* pCastString = new CCastString(m_pScen,id,props);
	pCastString->OrigString( 0 );

	//pCastString->FreezeHeats(m_pStringsSet->m_FreezeHeats);
	int status = m_pStringsSet->m_HeatLockStatus;
	if ( status == CCastString::HEATS_ALL_LOCKED
		 ||
		 status == CCastString::HEATS_SOME_LOCKED
		 ||
		 status == CCastString::HEATS_NONE_LOCKED )
		 pCastString->HeatStatus( CCastString::E_HeatLockStatus(status) );
	else
		pCastString->HeatStatus(CCastString::HEATS_ALL_LOCKED);
	pCastString->NumLockedHeats(m_pStringsSet->m_NumHeatsLocked);

	pCastString->CanEdit(m_pStringsSet->m_CanEdit);
	pCastString->Status(m_pStringsSet->m_Status == "910"
						? CCastString::STRING_STATUS_910
						: m_pStringsSet->m_Status == "909"
						? CCastString::STRING_STATUS_909
						: CCastString::STRING_STATUS_NEW);

	if ( m_pStringsSet->m_HeatLockStatus < 0 || m_pStringsSet->m_HeatLockStatus > 2 )
		pCastString->HeatStatus(CCastString::HEATS_ALL_LOCKED);
	else
		pCastString->HeatStatus( CCastString::E_HeatLockStatus(m_pStringsSet->m_HeatLockStatus) );

	pCastString->NumLockedHeats( min(99,max(0,m_pStringsSet->m_NumHeatsLocked)) );

	m_pScen->CastStrings().push_back(pCastString);

	int count = 0;

	while ( ! m_pHeatsSet->IsEOF() ) {

		if ( m_pHeatsSet->m_StringYear != id.Year()
			 ||
			 m_pHeatsSet->m_StringWeek != id.Week()
			 ||
			 m_pHeatsSet->m_StringCaster != id.Caster()
			 ||
			 m_pHeatsSet->m_StringId != id.StringId() )
			 
			 break;

		CCastStringHeat heat(*m_pHeatsSet,
							 pCastString);

		while ( ! m_pHeatChemsSet->IsEOF() 
			    &&
				m_pHeatChemsSet->m_Year == id.Year()
				&&
				m_pHeatChemsSet->m_Week == id.Week()
				&&
				m_pHeatChemsSet->m_Caster == id.Caster()
				&&
				m_pHeatChemsSet->m_StringId == id.StringId()
				&&
				m_pHeatChemsSet->m_HeatSeqNum == count ) {
			
			CChem::Element elt;
			if ( CChem::ToElement(m_pHeatChemsSet->m_Element,elt) ) {
				heat.MinChem( elt ,m_pHeatChemsSet->m_MinValue);
				heat.MaxChem( elt ,m_pHeatChemsSet->m_MaxValue);
				heat.AimChem( elt, m_pHeatChemsSet->m_AimValue);
			}
			m_pHeatChemsSet->MoveNext();

		}

		pCastString->Heats().resize(m_pHeatsSet->m_SeqNum+1);
		pCastString->Heats()[m_pHeatsSet->m_SeqNum] = heat;			
		++count;

		m_pHeatsSet->MoveNext();
	}

	while ( ! m_pOrdersSet->IsEOF() ) {

		if ( m_pOrdersSet->m_StringYear != id.Year()
			 ||
			 m_pOrdersSet->m_StringWeek != id.Week()
			 ||
			 m_pOrdersSet->m_StringCaster != id.Caster()
			 ||
			 m_pOrdersSet->m_StringId != id.StringId() )
			 
			 break;


		CCSOrder* pCSOrder = new CCSOrder(*m_pOrdersSet);

		pCastString->AddCSOrder(pCSOrder);

		m_pOrdersSet->MoveNext();
	}

	if ( ! m_pTemplatesSet->IsEOF() )
		LoadTemplate(pCastString);

	pCastString->CalculateHeats();
	pCastString->CalculateSummary();

	return true;
}




void CCasterScenArchiverAux::LoadTemplates()
{	

	for ( ; ! m_pTemplatesSet->IsEOF(); m_pTemplatesSet->MoveNext() ) {

		CCastStringId tid(m_pTemplatesSet->m_Year,
						  m_pTemplatesSet->m_Week,
						  m_pTemplatesSet->m_Caster,
						  m_pTemplatesSet->m_StringId);
		
		CCastString* pString = m_pScen->FindCastString(tid);

		if ( pString == 0 )
			continue;

		LoadTemplate(pString);
	}
}


void CCasterScenArchiverAux::LoadTemplate(CCastString* pString)
{
	
	COrderSelection& os = pString->OrderSelection();


	os.SetUseAllCICodes		( m_pTemplatesSet->m_UseAllCICodes	);
	os.SetUseAllSpecs		( m_pTemplatesSet->m_UseAllSpecs		);
	os.SetUseCrossApp		( m_pTemplatesSet->m_UseCrossApp		);
	os.SetUseWidthRange		( m_pTemplatesSet->m_UseWidthRange	);
	os.SetMinWidth			( m_pTemplatesSet->m_MinWidth			);
	os.SetMaxWidth			( m_pTemplatesSet->m_MaxWidth			);
	os.SetIncludeOrders		( COrderSelection::E_IncludeOrderType(m_pTemplatesSet->m_IncludeOrders)			);
	os.SetUseHrwk			( COrderSelection::HR_WK_CURRENT_BACK,m_pTemplatesSet->m_UseHrWkCurrentBack	);
	os.SetUseHrwk			( COrderSelection::HR_WK_PLUS_1,m_pTemplatesSet->m_UseHrWkPlus1	);
	os.SetUseHrwk			( COrderSelection::HR_WK_PLUS_2,m_pTemplatesSet->m_UseHrWkPlus2	);
	os.SetUseHrwk			( COrderSelection::HR_WK_PLUS_3,m_pTemplatesSet->m_UseHrWkPlus3	);
	os.SetUseHrwk			( COrderSelection::HR_WK_PLUS_4,m_pTemplatesSet->m_UseHrWkPlus4	);
	os.SetUseHrwk			( COrderSelection::HR_WK_FUTURE,m_pTemplatesSet->m_UseHrWkFuture	);
	os.SetCurrentHrYrWk		( CWeek(m_pTemplatesSet->m_CurrentHrYear,m_pTemplatesSet->m_CurrentHrWeek)		);	
//	os.SetUsePstRange		( COrderSelection::PST_PRIO,  m_pTemplatesSet->m_UsePstPrio   );
	os.SetUsePstRange		( COrderSelection::PST_LT_8,  m_pTemplatesSet->m_UsePstLt8    );
	os.SetUsePstRange		( COrderSelection::PST_8_14,  m_pTemplatesSet->m_UsePst8to14  );
	os.SetUsePstRange		( COrderSelection::PST_15_21, m_pTemplatesSet->m_UsePst15to21 );
	os.SetUsePstRange		( COrderSelection::PST_22_28, m_pTemplatesSet->m_UsePst22to28 );
	os.SetUsePstRange		( COrderSelection::PST_29_35, m_pTemplatesSet->m_UsePst29to35 );
	os.SetUsePstRange		( COrderSelection::PST_GT_35, m_pTemplatesSet->m_UsePstGt35   );

	os.SetUse80HsmLuType	( m_pTemplatesSet->m_Use80HsmLuType	);
	os.SetMegaLuType		( m_pTemplatesSet->m_MegaLuType		);
	os.SetUsePriorityCode	( m_pTemplatesSet->m_UsePriorityCode	);
	os.SetMaxCIPriority		( m_pTemplatesSet->m_MaxCIPriority	);
	os.SetMinCIPriority		( m_pTemplatesSet->m_MinCIPriority	);
	os.SetIncludeNewOrders	( m_pTemplatesSet->m_IncludeNewOrders	);

	vector<CString> specs;
	while ( ! m_pTemplateSpecsSet->IsEOF() 
			&&
			m_pTemplateSpecsSet->m_Year == m_pTemplatesSet->m_Year
			&&
			m_pTemplateSpecsSet->m_Week == m_pTemplatesSet->m_Week
			&&
			m_pTemplateSpecsSet->m_Caster == m_pTemplatesSet->m_Caster
			&&
			m_pTemplateSpecsSet->m_StringId == m_pTemplatesSet->m_StringId ) {

		specs.push_back(m_pTemplateSpecsSet->m_Spec);

		m_pTemplateSpecsSet->MoveNext();
	}

	os.SetSelectedSpecs(specs);


	while ( ! m_pTemplateChemsSet->IsEOF() 
			&&
			m_pTemplateChemsSet->m_Year == m_pTemplatesSet->m_Year
			&&
			m_pTemplateChemsSet->m_Week == m_pTemplatesSet->m_Week
			&&
			m_pTemplateChemsSet->m_Caster == m_pTemplatesSet->m_Caster
			&&
			m_pTemplateChemsSet->m_StringId == m_pTemplatesSet->m_StringId ) {
		
		CChem::Element elt;
		if ( CChem::ToElement(m_pTemplateChemsSet->m_Element,elt) )  {
			os.SetMinChem( elt ,m_pTemplateChemsSet->m_MinValue);
			os.SetMaxChem( elt ,m_pTemplateChemsSet->m_MaxValue);
		}
		m_pTemplateChemsSet->MoveNext();
	}

	vector<int> groupFields;
	vector<int> sortFields;

	while ( ! m_pTemplateSortsSet->IsEOF() 
			&&
			m_pTemplateSortsSet->m_Year == m_pTemplatesSet->m_Year
			&&
			m_pTemplateSortsSet->m_Week == m_pTemplatesSet->m_Week
			&&
			m_pTemplateSortsSet->m_Caster == m_pTemplatesSet->m_Caster
			&&
			m_pTemplateSortsSet->m_StringId == m_pTemplatesSet->m_StringId ) {

		COrderSelection::E_SortField field = COrderSelection::StringToSortField(m_pTemplateSortsSet->m_SortField);

		if ( m_pTemplateSortsSet->m_SortType == "G" )
			groupFields.push_back(field);
		else
			sortFields.push_back(field);
		
		m_pTemplateSortsSet->MoveNext();
	}

	os.InitializeGroupSortFields(groupFields,sortFields);

}

