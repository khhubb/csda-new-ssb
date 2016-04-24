// DBConn.cpp: implementation of the CDBConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "DBConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//  Class CDBConn manages a connection to one of the databases.
//  Typical usage:
//
//  {
//      CDBConn dbc(CDBConn::DB_DPA);	// or DB_CSDA or DB_HM
//		dbc.Connect();
//		
//          ... dbc.GetDB() ...
//         RS rs(dbc.GetDB());
//         // where RS is a class derived from CRecordset
//
//	}	// d-tor automatically closes the database connection
//
//


// static
CString CDBConn::DSNs[NumDBTypes];

// static
bool CDBConn::DSNsInitialized = false;


void CDBConn::InitializeDSNs()
{
	// ensure once-only initialization

	if ( DSNsInitialized )
		return;

	DSNsInitialized = true;

	char* defaultDSNs[NumDBTypes];
	char* iniItems[NumDBTypes];

	defaultDSNs[DB_DPA]		= "DSN=DPATest";
	defaultDSNs[DB_CSDA]	= "DSN=CSDATest";
	defaultDSNs[DB_HM]		= "DSN=hotmetal";
	defaultDSNs[DB_RULES]	= "DSN=rules";

	iniItems[DB_DPA]	= "DPA";
	iniItems[DB_CSDA]	= "CSDA";
	iniItems[DB_HM]		= "HM";
	iniItems[DB_RULES]	= "RULES";

	// an ini file can be used to hold the DSNs.
	// we haven't done that yet.

	for ( int i=0; i<NumDBTypes; ++i ) 

		DSNs[i] = AfxGetApp()->GetProfileString("Database",
												iniItems[i],
												defaultDSNs[i]);
}



//
//  C-tor must specify which database
//

CDBConn::CDBConn(CDBConn::DBType which)
: m_which(which)
{
	assert( 0 <= which && which < NumDBTypes );
}



//
//  d-tor will attempt to close if open
//

CDBConn::~CDBConn()
{
	try
	{
		if ( m_db.IsOpen() )
			m_db.Close();
	}
	catch ( CDBException* e )
	{
		AfxMessageBox( e->m_strError,   
                       MB_ICONEXCLAMATION );

		e->Delete();
	}
}


//
//  Connect to appropriate database.
//

bool CDBConn::Connect()
{
	InitializeDSNs();

	CString DSN = DSNs[m_which];

   try 
   {
		m_db.OpenEx(DSN);
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


//
//  Check if opened ok
//

bool CDBConn::IsOpen()
{
	return m_db.IsOpen();
}


//
//  Return a CDatabase object to use.
//

CDatabase* CDBConn::GetDB()
{
	assert( m_db.IsOpen() );

	return &m_db;
}

