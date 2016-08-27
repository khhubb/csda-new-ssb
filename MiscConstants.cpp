// MiscConstants.cpp: implementation of the CMiscConstants class.
//
//  Maint:
//     
//    K. Hubbard   11-16-04  Increased max slab length (for 3 combi) from 384 to 
//                           399" for planning and sale of slabs to Dofasco customer.
//                           per P. Fronczek temporary run       
//
//    K. Hubbard   03-24-06  Increased max slab length (for 2 BOF) from 399" to 
//                           425" for planning and slab sales customers.
//                           per P. Fronczek        
//
//    K. Hubbard   11-30-06  Increased max slab length (for 2 BOF) from 425" to 
//                           436" for planning and slab sales customers.
//                           per P. Fronczek        
//
//    K. Hubbard   02-04-10  Set global length values here in MiscConstants.cpp allowed at 2BOF
//                           432" for 2SC and 412" at 3Combi Caster. Also, develop an audit
//                           for 2BOF steelshop facility length limitations in CastStringValidator.cpp.
//
//    K. Hubbard   03-07-13  Set global length values here in MiscConstants.cpp allowed at 2BOF
//                           431" for 3Combi Caster. Also, develop an audit
//                           for 2BOF steelshop facility length limitations in CastStringValidator.cpp.
//
//    K. Hubbard   03-31-16  Set global length values here in MiscConstants.cpp allowed at 1SC and 2SC
//                           452" for Calvert customer order support. Also, develop an audit
//                           for BOF steelshop facility length limitations in CastStringValidator.cpp.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "MiscConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "DBConn.h"
#include "PerUserDefaultValuesSet.h"


////////////////////////////////////////////////////////////////
//	
//	Data
//	
////////////////////////////////////////////////////////////////

//  static
bool CMiscConstants::m_oneExists = false;

//static 
double CMiscConstants::m_steelDensity = 0.2833;

//  Exceeding these limits below will write 'Fatal' in CastStringValidator,cpp k. hubbard 02-04-10
//static 
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 384, 384 };
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 399, 399 };  // k. hubbard 11-08-04 maint.
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 425, 425 };  // k. hubbard 03-24-06 maint.
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 436, 436 };  // k. hubbard 11-30-06 maint.

//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 432, 412 };  // Changed 2SC max to 432" and 3Combi to 412" max plan length, exceeding these limits writes 'Fatal' in CastStringValidator,cpp k. hubbard 02-04-10 maint.
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 432, 431 };  // Changed 3Combi to 431" max plan length, exceeding these limits writes 'Fatal' in CastStringValidator,cpp k. hubbard 02-04-10 maint.
//Length CMiscConstants::m_casterSlabLengthMaxes[4] = { 0, 440, 440, 431 };  // Changed 2BOF to 440" max plan length, exceeding these limits writes 'Fatal' in CastStringValidator,cpp k. hubbard 01-22-16 maint.
// CASTER TODO: Determine casterSlabLengthMaxes for casters 4,5
Length CMiscConstants::m_casterSlabLengthMaxes[Caster::CasterArrayLen] = { 0, 452, 452, 431, 450, 450 };  // Changed 1SC and 2SC to 452" max plan length, exceeding these limits writes 'Fatal' in CastStringValidator,cpp k. hubbard 03-31-16 maint.


//static 
Length CMiscConstants::m_max80HsmRunoutLength = 440;

//static 
Length CMiscConstants::m_minCastableSlabLength = 218;


// static
int CMiscConstants::m_archiveVersionRetentionCount = 4;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////





CMiscConstants::CMiscConstants()
{
	//  Don't allow more than one to be created.
	assert ( ! m_oneExists );
	m_oneExists = true;

	//  Mark as unmodified and uninitialized.
	m_initialized = false;
	m_modified = false;

	//  Zero out any values.
	for ( int i=Caster::FirstCaster; i<=Caster::LastCaster; ++i ) {
		m_minHeatSizes[i] = 0;
		m_aimHeatSizes[i] = 0;
		m_maxHeatSizes[i] = 0;
	}

	m_turnaroundTimes[0] = CTimeSpan(0,0,0,0);
	m_turnaroundTimes[Caster::C1] = CTimeSpan(0,0,0,0);
	m_turnaroundTimes[Caster::C2] = CTimeSpan(0, 0, 75, 0);
	m_turnaroundTimes[Caster::C3] = CTimeSpan(0, 0, 90, 0);
	// CASTER TODO: determine correct values for turnaround times.
	m_turnaroundTimes[Caster::C4] = CTimeSpan(0, 0, 150, 0);
	m_turnaroundTimes[Caster::C5] = CTimeSpan(0, 0, 90, 0);

}



CMiscConstants::~CMiscConstants()
{
	// nothing to do
}



////////////////////////////////////////////////////////////////
//	
//	Accessors/modifiers
//	
////////////////////////////////////////////////////////////////


int CMiscConstants::MinHeatSize(int caster) const
{
	//  don't allow access to value if not initialized
	assert( m_initialized );

	ValidateCaster(caster);

	return m_minHeatSizes[caster];
}

int CMiscConstants::AimHeatSize(int caster) const
{
	//  don't allow access to value if not initialized
	assert( m_initialized );

	ValidateCaster(caster);

	return m_aimHeatSizes[caster];
}
int CMiscConstants::MaxHeatSize(int caster) const
{
	//  don't allow access to value if not initialized
	assert( m_initialized );

	ValidateCaster(caster);

	return m_maxHeatSizes[caster];
}



void CMiscConstants::SetMinHeatSize(int caster, int size)
{
	ValidateCaster(caster);

	assert( size > 0 );

	// Mark as modified if value changes
	m_modified = ( m_minHeatSizes[caster] != size );

	m_minHeatSizes[caster] = size;
}
	


void CMiscConstants::SetAimHeatSize(int caster, int size)
{
	ValidateCaster(caster);

	assert( size > 0 );

	// Mark as modified if value changes
	m_modified = ( m_aimHeatSizes[caster] != size );

	m_aimHeatSizes[caster] = size;
}
	
void CMiscConstants::SetMaxHeatSize(int caster, int size)
{
	ValidateCaster(caster);

	assert( size > 0 );

	// Mark as modified if value changes
	m_modified = ( m_maxHeatSizes[caster] != size );

	m_maxHeatSizes[caster] = size;
}
	

CTimeSpan CMiscConstants::TurnaroundTime(int caster) const
{	
	ValidateCaster(caster);

	return m_turnaroundTimes[caster];
}


////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////


void CMiscConstants::Init(CString loginName_)
{

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();

	CPerUserDefaultValuesSet rs(dbc.GetDB());

	try 
	{

		rs.m_loginNameParm = loginName_;
		rs.Open(CRecordset::forwardOnly,0,CRecordset::readOnly);

		m_minHeatSizes[Caster::C1] = rs.m_Caster1HeatSizeMin;
		m_aimHeatSizes[Caster::C1] = rs.m_Caster1HeatSizeAim;
		m_maxHeatSizes[Caster::C1] = rs.m_Caster1HeatSizeMax;
		m_minHeatSizes[Caster::C2] = rs.m_Caster2HeatSizeMin;
		m_aimHeatSizes[Caster::C2] = rs.m_Caster2HeatSizeAim;
		m_maxHeatSizes[Caster::C2] = rs.m_Caster2HeatSizeMax;
		m_minHeatSizes[Caster::C3] = rs.m_Caster3HeatSizeMin;
		m_aimHeatSizes[Caster::C3] = rs.m_Caster3HeatSizeAim;
		m_maxHeatSizes[Caster::C3] = rs.m_Caster3HeatSizeMax;
		m_minHeatSizes[Caster::C4] = rs.m_Caster4HeatSizeMin;
		m_aimHeatSizes[Caster::C4] = rs.m_Caster4HeatSizeAim;
		m_maxHeatSizes[Caster::C4] = rs.m_Caster4HeatSizeMax;
		m_minHeatSizes[Caster::C5] = rs.m_Caster5HeatSizeMin;
		m_aimHeatSizes[Caster::C5] = rs.m_Caster5HeatSizeAim;
		m_maxHeatSizes[Caster::C5] = rs.m_Caster5HeatSizeMax;

		m_loginName = loginName_;

		m_initialized	= true;
		m_modified	= false;		
		
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


void CMiscConstants::Save()
{
	// don't save if not initialized

	assert( m_initialized );

	//  no point saving if not modified.

	if ( ! m_modified ) 
		return;

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();

	CPerUserDefaultValuesSet rs(dbc.GetDB());

	try 
	{
		rs.m_loginNameParm = m_loginName;
		rs.Open();
		
		rs.Edit();

		rs.m_Caster1HeatSizeMin = m_minHeatSizes[Caster::C1];
		rs.m_Caster1HeatSizeAim = m_aimHeatSizes[Caster::C1];
		rs.m_Caster1HeatSizeMax = m_maxHeatSizes[Caster::C1];
		rs.m_Caster2HeatSizeMin = m_minHeatSizes[Caster::C2];
		rs.m_Caster2HeatSizeAim = m_aimHeatSizes[Caster::C2];
		rs.m_Caster2HeatSizeMax = m_maxHeatSizes[Caster::C2];
		rs.m_Caster3HeatSizeMin = m_minHeatSizes[Caster::C3];
		rs.m_Caster3HeatSizeAim = m_aimHeatSizes[Caster::C3];
		rs.m_Caster3HeatSizeMax = m_maxHeatSizes[Caster::C3];
		rs.m_Caster4HeatSizeMin = m_minHeatSizes[Caster::C4];
		rs.m_Caster4HeatSizeAim = m_aimHeatSizes[Caster::C4];
		rs.m_Caster4HeatSizeMax = m_maxHeatSizes[Caster::C4];
		rs.m_Caster5HeatSizeMin = m_minHeatSizes[Caster::C5];
		rs.m_Caster5HeatSizeAim = m_aimHeatSizes[Caster::C5];
		rs.m_Caster5HeatSizeMax = m_maxHeatSizes[Caster::C5];

		rs.Update();

		m_modified = false;	
	
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




//  We need exactly one of these.
//  It will be initialized by the main routine.

CMiscConstants GlobalConstants;
