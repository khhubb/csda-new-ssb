// UploadMgr.cpp: implementation of the CUploadMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "UploadMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
//
//	CUploadMgr
//
//  This class opens and closes two recordsets related to uploading.
//  I just decided it was easier to move this functionality off to another
//    class to make sure that things got closed properly, due to some
//    complexity of the control structure in the upload process.
//
//	Usage:
//
//		CUploadMgr uMgr(dbc.GetDB());
//
//		uMgr.Initialize();
//	
//		// perform operations using the open recordsets
//
//		uMgr.Finalize();	// you could let the d-tor do this.
//
//
////////////////////////////////////////////////////////////////////////


CUploadMgr::CUploadMgr(CDatabase* pDB)
:	m_pDB(pDB),
	m_isInitialized(false),
	m_isFinalized(false),
	m_dpSet(pDB),
	m_luSet(pDB)
{
	
}

CUploadMgr::~CUploadMgr()
{
	if ( m_isInitialized && ! m_isFinalized )
		Finalize();
}


bool CUploadMgr::Initialize()
{
	assert( ! m_isInitialized );

	m_isInitialized = true;

	try
	{	
		
		m_pDB->BeginTrans();
		m_dpSet.Open(CRecordset::dynamic,0,CRecordset::none);
		m_luSet.Open(CRecordset::dynamic,0,CRecordset::none);

//		m_dpSet.Open(CRecordset::dynaset,0,CRecordset::appendOnly);  SQL conversion 3-13-03 k. hubbard
//		m_luSet.Open(CRecordset::dynaset,0,CRecordset::appendOnly);  SQL conversion 3-13-03 k. hubbard


	}
	catch ( CDBException* e )
	{
	   m_pDB->Rollback();

	   if ( m_dpSet.IsOpen() ) m_dpSet.Close();
	   if ( m_luSet.IsOpen() ) m_luSet.Close();

	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
   	   return false;
	}

	return true;
}



bool CUploadMgr::Finalize()
{
	assert( ! m_isFinalized );

	m_isFinalized = true;


	bool retVal = true;

	try
	{	
		if ( m_dpSet.IsOpen() ) m_dpSet.Close();
		if ( m_luSet.IsOpen() ) m_luSet.Close();
		m_pDB->CommitTrans();
	}
	catch ( CDBException* e )
	{
	   m_pDB->Rollback();


	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
   	   retVal = false;
	}


	return retVal;
}