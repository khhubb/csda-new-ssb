// CasterScen.cpp: implementation of the CCasterScen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "CasterScen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "DalyPlanSet.h"
#include "SSBOutput.h"
#include "Order.h"
#include "CSOrder.h"
#include "CastString.h"
#include "Snapshot.h"
#include "UserInfo.h"

#include "CasterScenArchiver.h"


///////////////////////////////////////////////////////////////
//
//
//	
//	class CCasterScen
//	
//	Represents a scenario for a caster.
//	
//
///////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tors
//	
////////////////////////////////////////////////////////////////


//  No default c-tor

//  In order to create a CCasterScen, we must know 
//		(1) which caster it is for


CCasterScen::CCasterScen(int caster_)
: m_caster(caster_),
  m_pBackup(0),
  m_isPublic(false)
{
	SetOwner( CUserInfo::TheUser.LoginName() );
}



//	Copying involves copying all the strings of the source CCasterScen.


CCasterScen::CCasterScen(const CCasterScen& base, bool isSnapScen)
: m_caster(base.Caster()),
  m_pBackup(0),
  m_isPublic(false)
{
	SetOwner( base.Owner() );

	for ( TCastStringVec::const_iterator is = base.m_castStrings.begin();
		  is != base.m_castStrings.end();
		  ++is ) {

		CCastString* pString 
			= new CCastString(this,
							  (**is), 
							  (isSnapScen 
							   ? (*is) 
							   : (*is)->OrigString() ));
		
		m_castStrings.push_back(pString);
	}
}





//	This modified copy copies all strings which are 910 
//     and all strings at the given indexes.

CCasterScen::CCasterScen(const CCasterScen& base,
						 vector<int>& index909s)
: m_caster(base.Caster()),
  m_pBackup(0),
  m_isPublic(false)
{
	SetOwner( base.Owner() );

	for ( TCastStringVec::const_iterator is = base.m_castStrings.begin();
		  is != base.m_castStrings.end();
		  ++is ) {
	
		if ( (*is)->Status() == CCastString::STRING_STATUS_910
			||
			find(index909s.begin(),
			     index909s.end(),
				 (is - base.m_castStrings.begin())) != index909s.end() ) {

			CCastString* pString = new CCastString( this, (**is), (*is) );

			m_castStrings.push_back( pString );
		}
	}
}



//  The cleanup required is to delete the cast strings.
//  This action can happen in other places, so we encapsulate
//     this action elsewhere.

CCasterScen::~CCasterScen()
{
	Cleanup();
}




//  Get rid of all the cast strings.

void CCasterScen::Cleanup()
{

	if ( m_pBackup != 0 )
		delete m_pBackup;

	m_orderSlabsMap.clear();


	release(m_castStrings.begin(),
			m_castStrings.end());
	m_castStrings.clear();
}




//////////////////////////////////////////////////////////////
//	
//	Initializing, manipulating strings
//
//////////////////////////////////////////////////////////////


//  A primary method of initialization  -- read the existing lineup
//		from DPA.
//
//  Returns true/false depending on success of database operations.

bool CCasterScen::LoadFromDalyPlan(CDalyPlanSet& planSet)
{
	//  Get rid of any existing records
	Cleanup();
	
	//  Do retrieve on DalyPlan table, parameterized by caster

	if ( ! OpenDalyPlanSet(planSet) ) 
		return false;
	

	//  The records are sorted in string order 
	//		and by lot# within a string.
	//  Create strings and the orders within each string.
	//  The c-tor for CCastString will read records to fill in its lots
	//     and leave the recordset either at the beginning of the next
	//     string or at EOF.


	//  Query set iteration

	while ( ! planSet.IsEOF() ) 

		m_castStrings.push_back( new CCastString(this,planSet) );
		
	
	//  Calculate heats for all strings.

	for ( TCastStringVec::iterator is = m_castStrings.begin();
		  is != m_castStrings.end();
		  ++is ) {
		(*is)->CalculateHeats();
		(*is)->CalculateSummary();
	}

	return true;
}



//  private
//
//  Support function for LoadFromDalyPlan
// 
//  Set the caster parm for the query,
//  then either open or requery the recordset.

bool CCasterScen::OpenDalyPlanSet(CDalyPlanSet& planSet)
{
	//  Set to retrieve only records for this caster

	//  Potential problem for Format on CRecordset CString parm Format
	//planSet.castUnitParam.Empty();
	//planSet.castUnitParam.Format("%d",m_caster);

	CString temp;
	temp.Format("%d",m_caster);
	planSet.castUnitParam = temp;
	
	try
	{
		if ( planSet.IsOpen() )
			planSet.Requery();
		else
			planSet.Open(CRecordset::forwardOnly,
				         0,
						 CRecordset::readOnly);
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

bool CCasterScen::LoadFromNewStringBuilderOutput(CSSBOutput& sbSet, vector<CCastString*>& newStrings)
{
	// This will always add new records
	// Do NOT Cleanup();

	newStrings.clear();

	//  The records are sorted in string order 
	//		and by lot# within a string.
	//  Create strings and the orders within each string.
	//  The c-tor for CCastString will read records to fill in its lots
	//     and leave the recordset either at the beginning of the next
	//     string or at EOF.


	//  Query set iteration

	while (!sbSet.IsEOF())	{

		CCastString* pString = new CCastString(this, sbSet);
		m_castStrings.push_back(pString);
		newStrings.push_back(pString);
	}


	//  Set exposure codes for stock orders, to either preceding or following

	{
		for (TCastStringVec::iterator is = m_castStrings.begin();
			is != m_castStrings.end();
			++is) {
			for (vector<CCSOrder*>::iterator io = (*is)->StrandBegin(1);
				io != (*is)->StrandEnd(1);
				++io) {
				// FP change
				if ((*io)->FpOrderNum().Left(7) == "9999999") {
					// stock order
					if (io != (*is)->StrandBegin(1))
						(*io)->ExposureCode((*(io - 1))->ExposureCode());
					else if (io != (*is)->StrandEnd(1))
						//			(*io)->ExposureCode( (*(io+1))->ExposureCode() );
						//		else 
						(*io)->ExposureCode('E');
				}
			}

			for (vector<CCSOrder*>::iterator io = (*is)->StrandBegin(2);
				io != (*is)->StrandEnd(2);
				++io) {
				// FP change
				if ((*io)->FpOrderNum().Left(7) == "9999999") {
					// stock order
					if (io != (*is)->StrandBegin(2))
						(*io)->ExposureCode((*(io - 1))->ExposureCode());
					else if (io != (*is)->StrandEnd(2))
						//		(*io)->ExposureCode( (*(io+1))->ExposureCode() );
						//	else 
						(*io)->ExposureCode('E');
				}
			}


		}
	}


	//  Calculate heats for all strings.

	for (TCastStringVec::iterator is = m_castStrings.begin();
		is != m_castStrings.end();
		++is) {
		(*is)->CalculateHeats();
		(*is)->CalculateSummary();
	}

	return true;
}

//bool CCasterScen::LoadFromStringBuilderOutput(CStringBuilderOutputSet& sbSet,vector<CCastString*>& newStrings)
//{
//	// This will always add new records
//	// Do NOT Cleanup();
//
//	newStrings.clear();
//
//	//  The records are sorted in string order 
//	//		and by lot# within a string.
//	//  Create strings and the orders within each string.
//	//  The c-tor for CCastString will read records to fill in its lots
//	//     and leave the recordset either at the beginning of the next
//	//     string or at EOF.
//
//
//	//  Query set iteration
//
//	while ( ! sbSet.IsEOF() )	{
//
//		CCastString* pString = new CCastString(this,sbSet);
//		m_castStrings.push_back( pString );
//		newStrings.push_back(pString);
//	}
//
//		
//	//  Set exposure codes for stock orders, to either preceding or following
//
//	{
//		for ( TCastStringVec::iterator is = m_castStrings.begin();
//			  is != m_castStrings.end();
//			  ++is ) {
//			for ( vector<CCSOrder*>::iterator io = (*is)->StrandBegin(1);
//				  io != (*is)->StrandEnd(1);
//				  ++io ) {
//				// FP change
//				if ( (*io)->FpOrderNum().Left(7) == "9999999" ) {
//					// stock order
//					if ( io != (*is)->StrandBegin(1) ) 
//						(*io)->ExposureCode( (*(io-1))->ExposureCode() );
//					else if ( io != (*is)->StrandEnd(1) )
//			//			(*io)->ExposureCode( (*(io+1))->ExposureCode() );
//			//		else 
//						(*io)->ExposureCode( 'E' );
//				}
//			}
//				
//			for ( vector<CCSOrder*>::iterator io = (*is)->StrandBegin(2);
//				  io != (*is)->StrandEnd(2);
//				  ++io ) {
//				// FP change
//				if ( (*io)->FpOrderNum().Left(7) == "9999999" ) {
//					// stock order
//					if ( io != (*is)->StrandBegin(2) ) 
//						(*io)->ExposureCode( (*(io-1))->ExposureCode() );
//					else if ( io != (*is)->StrandEnd(2) )
//				//		(*io)->ExposureCode( (*(io+1))->ExposureCode() );
//				//	else 
//						(*io)->ExposureCode( 'E' );
//				}
//			}
//
//				
//		}
//	}
//
//
//	//  Calculate heats for all strings.
//
//	for ( TCastStringVec::iterator is = m_castStrings.begin();
//		  is != m_castStrings.end();
//		  ++is ) {
//		(*is)->CalculateHeats();
//		(*is)->CalculateSummary();
//	}
//
//	return true;
//}




//  Strings are added to the scenario in two ways:
//		(1)	when the scenario is initialized from DPA (LoadFromDalyPlan)
//		(2)	at user request, during editing (CreateNewString)
//
//	In order to create a string, we need an id for it and we need
//		to know other miscellaneous definitional criteria.
//	
//	In (1), the id comes from the DALYPLAN records.  
//	At the moment, we don't cache any of the definitional criteria.
//  
//  TODO:  Archive the definitional criteria.
//
//  Also, in (1), we create the orders in the string.
//
//	In (2), the user has already defined the id and other info,
//		so that is just passed along to us.
//

CCastString* CCasterScen::CreateNewString(CCastStringId& id,
										  CCastStringMiscProps& props)
{
	// Make sure we only add a string destined for the same caster
	//  that this scenario is for.
	
	assert( id.Caster() == m_caster);

	//  Make sure we don't duplicate an id.

	for ( vector<CCastString*>::const_iterator is = m_castStrings.begin();
		  is != m_castStrings.end();
		  ++is )

		assert ( ! ((*is)->Id() == id) );


	//  Create the string and add to the list.

	CCastString* pString = new CCastString(this,id,props);
	m_castStrings.push_back(pString);

	// Sort the new string into correct position.

	sort(m_castStrings.begin(),
		 m_castStrings.end(),
		 less_p<CCastString*>());

	//  Calculate heats for the string.
	//  Note that we don't have to worry about changing the heat calcs
	//    for the other strings.
	
	pString->CalculateHeats();
	pString->CalculateSummary();

	return pString;
}


//
//  Another way to create a string is to move a string from one scenario
//    to another.
//  We pass the string to copy, the id for the new string (guaranteed to
//    be unique when inserted in this scenario), and misc props (the user
//    might make a change from the existing string).

CCastString* CCasterScen::CreateCopyString(CCastStringId& id,
   										  CCastStringMiscProps& props,
										   CCastString* pOrigString)
{
	// Make sure we only add a string destined for the same caster
	//  that this scenario is for.
	
	assert( id.Caster() == m_caster);

	//  Make sure we don't duplicate an id.

	for ( vector<CCastString*>::const_iterator is = m_castStrings.begin();
		  is != m_castStrings.end();
		  ++is )

		assert ( ! ((*is)->Id() == id) );


	//  Create the string and add to the list.

	// create copy of string
	CCastString* pCopyString = new CCastString(this,id,props,*pOrigString);
	m_castStrings.push_back(pCopyString);


	// Sort the new string into correct position.

	sort(m_castStrings.begin(),
		 m_castStrings.end(),
		 less_p<CCastString*>());

	//  Calculate heats for the string.
	//  Note that we don't have to worry about changing the heat calcs
	//    for the other strings.
	
	pCopyString->CalculateHeats();
	pCopyString->CalculateSummary();

	return pCopyString;
}





//  Deleting a string is usually performed by the user from the
//     caster scenario editor
//  Locate in vector and remove from vector, then destroy.

void CCasterScen::DeleteString(CCastString* pString)
{
	
	//  Locate
	vector<CCastString*>::iterator is = find(m_castStrings.begin(),
											 m_castStrings.end(),
											 pString);

	//  Make sure it exists.
	assert ( is != m_castStrings.end() );
	
	//  Remove from vector
	m_castStrings.erase(is);

	//	Destroy
	delete pString;
}




//	Called when user wishes to change the cast string id.
//  It is assumed of course that the caster has not changed!
//  However, either the week/year or the string# may change.
//  This will force the strings to be re-sorted.

void CCasterScen::ModifyCastStringId(CCastString* pString,
									 const CCastStringId& newId)
{
	assert( newId.Caster() == m_caster );

	pString->ModifyId(newId);

	sort(m_castStrings.begin(),
		 m_castStrings.end(),
		 less_p<CCastString*>());
}



//  Determine if there is a cast string with the given id.

CCastString* CCasterScen::FindCastString(const CCastStringId& id) const
{
	for ( vector<CCastString*>::const_iterator is = m_castStrings.begin();
		  is != m_castStrings.end();
		  ++is )

		if ( (*is)->Id() == id )
			return (*is);

	return 0;
}




////////////////////////////////////////////////////////////////
//	
//	Managing lots
//	
////////////////////////////////////////////////////////////////


//  Whenever a CCSOrder is really added/removed to/from a CCastString,
//	  then RegisterCSOrder/UnregisterCSOrder must be called.
//	This serves primarily to update a cached value of the number of pieces
//    scheduled for the underlying COrder in this scenario.
//	This value is retrieved by OrderNumPiecesScheduled.
//	Current due for an order is then dependent on the super scenario:
//		current due = total pieces ordered - sum(caster=1,2,3) OrderNumPiecesScheduled 
//                                        

void CCasterScen::RegisterCSOrder(CCSOrder* pCSOrder)
{
	if ( pCSOrder->Order() == 0 ) 
		return;

	TOrderIntMap::iterator im
		= m_orderSlabsMap.find(pCSOrder->Order());

	if ( im == m_orderSlabsMap.end() ) 
		// No existing value
		m_orderSlabsMap.insert( TOrderIntMap::value_type(pCSOrder->Order(),
													    pCSOrder->NumSlabs()));
	else  {

		int value = (*im).second;
		// existing value -- increment it
		//(*im).second += pCSOrder->NumPieces();
		//(*im).second += pCSOrder->NumSlabs();
		// Wow: This blew up if I didn't erase first!!!!!!
		m_orderSlabsMap.erase(im);
		m_orderSlabsMap.insert( TOrderIntMap::value_type(pCSOrder->Order(),
													    value + pCSOrder->NumSlabs()));
	}
}


void CCasterScen::UnregisterCSOrder(CCSOrder* pCSOrder)
{
	if ( pCSOrder->Order() == 0 ) 
		return;

	TOrderIntMap::iterator im
		= m_orderSlabsMap.find(pCSOrder->Order());

	if ( im != m_orderSlabsMap.end() ) {
		// existing value -- decrement it
		//(*im).second -= pCSOrder->NumSlabs();
		int curVal = (*im).second;
		curVal -= pCSOrder->NumSlabs();
		m_orderSlabsMap.erase(im);
		if ( curVal != 0 )
			m_orderSlabsMap.insert( TOrderIntMap::value_type(pCSOrder->Order(),
														    curVal));
	}
}


int CCasterScen::NumSlabsScheduled(const COrder* pOrder) const
{
	// I don't know why find is const!
	TOrderIntMap::const_iterator im = m_orderSlabsMap.find(const_cast<COrder*>(pOrder));

	if ( im == m_orderSlabsMap.end() )
		// Not scheduled
		return 0;
	else
		// Scheduled
		return (*im).second;
}



//  A little utility function.
//  Delegates finding an order from an order num
//		to a lookup function provided by COrder, 
//		operating on the vector of all orders from the snapshot

// FP change
//COrder* CCasterScen::FindOrder(int orderNum) const
//{
//	return COrder::FindOrder(orderNum,TheSnapshot.Orders());
//}

COrder* CCasterScen::FindOrder(CString& fpOrderNum) const
{
	return COrder::FindOrder(fpOrderNum,TheSnapshot.Orders());
}


//	For purposes of allowing the user to pick a new CCastStringId,
//		either (1) for a new cast string or (2) to renumber an existing one,
//		this returns the currently used strings.
//	pIdToIgnore is used in case 2, since it is permissible to renumber 
//		a castString to its current id (i.e., make no change) -- 
//		to accomplish this, we do not add its id to the list of ids to avoid.

void CCasterScen::GetLineupIds(vector<CCastStringId>& ids,
							   const CCastStringId* pIdToIgnore)
{

	for ( vector<CCastString*>::iterator is = CastStrings().begin();
		  is != CastStrings().end();
		  ++is )

		if ( pIdToIgnore == 0 
			 ||
			 (*is)->Id() != *pIdToIgnore )
			 ids.push_back((*is)->Id());  // pushing copy

}


typedef vector<pair<__int64, __int64> > PII_;

// ## Changed int to __int64
void CCasterScen::FindMarkedHeats( vector< pair<__int64,__int64> >& markedHeats, 
								   CCastString* pIgnoreString )
{

	for ( vector<CCastString*>::iterator is = CastStrings().begin();
	      is != CastStrings().end();
		  ++is ) {

		if ( (*is) == pIgnoreString )
			continue;

		for ( vector<CCastStringHeat>::iterator ih = (*is)->Heats().begin();
			  ih != (*is)->Heats().end();
			  ++ih ) {

			if ( (*ih).IsMarked() )

				markedHeats.push_back( PII_::value_type( is - CastStrings().begin(),
				                                         ih - (*is)->Heats().begin() ));
		}
	}
}



