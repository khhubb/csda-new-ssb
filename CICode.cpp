// CICode.cpp: implementation of the CCICodeGroup class.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//  Function   :  This dialog (GUI) program displays CI codes and their "Bogie" HSM rollng targets associated with the
//                CI Code..       
//	Maintenance:
//
//  k. hubbard  05-4-10: Added SQL Dataloads of Gross tons, commodity xref, commodity tons, and Bogie tables. 
//	                     This will update CI codes with sorting ability.  per T. Fronczek
//
//  k. hubbard  05-26-10: Created full viewing of Bogie Plans and their fields. 

////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"



#include "csda.h"
#include "CICode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//#include "CICodeSet.h"
#include "CIBogieSet.h"

#include "CICodeGroupDefSet.h"
#include "CICodeGroupXrefSet.h"
#include "NOutputWnd.h"



///////////////////////////////////////////////////////////////////////
//
//  class CCICodeGroup
//
//////////////////////////////////////////////////////////////////////


//  CI code = Commodity code
//  Major categorization for orders
//  CSDer organizes work around bogies per CI code
//
//  Or used to. Now, PSTs take precedence.  
//
//  The CI code itself just a positive integer, 2-digit.
//  In certain instances, several CI Codes are treated as a group.
//  For example, CI codes 37 and 38 together represent #4ALUM.
//  Also, on the reports we have, the CI codes get sorted strangely.
//
//  A CCICodeGroup represents a group of CI codes that appear together.
//  They have the same name.  The group is assigned a sort number 
//  indicating its position in the sort of all CI code groups.
//
//
//  Fields:
//
//    m_groupId - an integer identifier for the group
//    m_codes -- a vector of the codes that are part of the group
//    m_name  -- an English description of the group
//    m_sortOrder -- an integer indicating sort position.
//
//	Originally, we did not have an m_groupId. The group was identified
//     by one of the codes belonging to it.
//  The current arrangement of tables has one table defining the groups.
//  Each group has in integer identifier (m_groupId) which also doubles
//    to define the sort order.
//  Thus the m_sortOrder field is now the same as the m_groupId field.
//	
//
//  operator< is defined (inline) to sort on the basis of m_sortOrder.
//
//  The c-tor is private -- creation of groups is to be via friend
//  class CICodeGroupMgr.
//



CCICodeGroup::CCICodeGroup(int groupId,
						 CString name,
						 int sortOrder, // added next 10 bogie fields 5-4-10 k. hubbard
						 long myBogie, 
						 long myRolled,
						 long myOnLineup,
						 long mySlabbedAvail,
						 long myCms,
						 long myShaves,
						 long myOtherUnavail,
						 long myTons910,
						 long myTons909,
						 long myTonsRemPlan)  
: m_groupId(groupId),
  m_name(name), 
  m_sortOrder(sortOrder),	// added next 10 bogie fields 5-4-10 k. hubbard					
  m_Bogie(myBogie),
  m_Rolled(myRolled),  
  m_OnLineup(myOnLineup),
  m_SlabbedAvail(mySlabbedAvail),
  m_Cms(myCms),
  m_Shaves(myShaves),
  m_OtherUnavail(myOtherUnavail),
  m_Tons910(myTons910),
  m_Tons909(myTons909),
  m_TonsRemPlan(myTonsRemPlan)

{
}




/////////////////////////////////////////////////////////////////////
//
//  class CCICodeGroupException
//
/////////////////////////////////////////////////////////////////////

//  Exceptions can be thrown during the operations of the CICodeGroupMgr
//  A CCICodeGroupException records an error type and an index that is
//     incorrect.
//
//  The types of errors are:
//
//		MISSING_GROUP
//			a lookup was made for a group containing a particular CI code 
//			-- it failed
//
//      BAD_INDEX
//			an attempt was made to access via an out-of-bounds index
//
//		ALREADY_ASSIGNED
//			We thought we were creating a new group, but it already exists
//
//  Everything is inline for this.




//////////////////////////////////////////////////////////////////////
//
//  class CCICodeGroupMgr
//
//////////////////////////////////////////////////////////////////////

//  This class manages a set of CCICodeGroups.
//  
//	The c-tor has a size argument which defaults to 100.
//  This should always serve present purposes, where CICode is 2 digits.
//
//  Two vectors and one map are maintained:
//
//   m_ciCodeToGroup maps a CICode (an int) to its CCICodeGroup.
//   m_ciCodeToGroup[i] ==>  pointer to CCICodeGroup containing i, if it exists
//                      ==>  0 otherwise
//	 if we ever have significantly more than 100 possible values, we would
//     need to reimplement as a map.
//
//   m_groups	contains the CCICodeGroups which have been created by this Mgr.
//   this vector will be sored by the sortNum,  except transiently when
//      we allow bulk additions of CICodeGroups and only sort when done.
//
//   m_groupMap is a map from int (groupId) to CCICodeGroup.
//
//	Operations:
//
//		//void CreateGroups(CCICodeSet& rSet)
//		//	read initialization info from a database
//		//	to create a set of groups
//			
//		CCICodeGroup& GetGroup(int ciCode)
//          returns reference to the group that ciCode belongs to.
//          throws a CCICodeGroupException(type = BAD_INDEX ) 
//              if ciCode is not in the range [0,m_size)
//          throws a CCICodeGroupException(type=MISSING_GROUP)
//              if ciCode does not belong to a group.
//
//     CCICodeGroup* GetGroupMaybe(int ciCode)
//          returns a pointer to the group that ciCode belongs to.
//          returns 0 if ciCode is out of range or not in a group.
//
//		void CreateGroup(int groupId, 
//						CString& name, 
//						int sortOrder, 
//						bool doSort = TRUE)
//			creates a group, with given name and sortOrder
//          throws CICodeGroupException(type=BAD_INDEX) if groupId is already 
//              defined as a group.
//
//		void AddCodeToGroup(int newCICode, int groupId)
//          adds newCICode to the group with id groupId.
//          throws a CCICodeGroupException(type = BAD_INDEX ) 
//              if newCICode is not in the range [0,m_size)
//          throws a CCICodeGroupException(type = ALREADY_ASSIGNED)
//              if newCICode already belongs to a group.
//          throws a CCICodeGroupException(type=MISSING_GROUP)
//              if groupId is not a defined group.
//
//		void SortGroups()
//			sorts the groups into proper order.  
//			Useful after doing one or more
//          CreateGroup operations with doSort=FALSE.
//
//		GroupVec::const_iterator GroupBegin()
//		GroupVec::const_iterator GroupEnd()
//			provides iterators for outsiders to iterate through all groups
//
//		bool IsCICodeInRange(int ciCode)
//          Returns TRUE if ciCode is in range [0,m_size)
//
//		bool IsCICodeDefined(int ciCode)
//          Returns TRUE if ciCode is in range and belongs to a group
//
//		void VerifyCICodeInRange(int ciCode)
//			utility function -- verifies ciCode in [0,m_size)
//          if not, throws BAD_INDEX exception.
//
//      void VerifyCICodeDefined(int ciCode)
//			utility function -- verifies that ciCode belongs to a group
//          if not, throws a MISSING_GROUP exception.
//
//      void VerifyCICodeUndefined(int ciCode)
//          utility function -- verifies that ciCode does not belong to a group
//			if it does, throws an ALREADY_ASSIGNED exception.


//
//	C-tor records the size and sets aside the appropriate number of spaces
//		in the map vector (ciCodeToGroup)
//

CCICodeGroupMgr::CCICodeGroupMgr(int size)
{
	m_size = size;
	m_ciCodeToGroup.resize(m_size,0);
}


//
//	d-tor must deallocate all the new'd CCICodeGroups
//

CCICodeGroupMgr::~CCICodeGroupMgr()
{
	Cleanup();
}


void CCICodeGroupMgr::Cleanup()
{
	for ( T_GroupVec::iterator ig = m_ciCodeToGroup.begin();
		  ig != m_ciCodeToGroup.end();
		  ++ig ) 
		(*ig) = 0;

	release(m_groups.begin(),m_groups.end());
	m_groups.erase(m_groups.begin(),m_groups.end());
}



//
// Given a database, open the table with CI_Codes and initialize from there.
//
// #if 0                        // test comm out 5-3-10 k. hubbard
bool CCICodeGroupMgr::CreateGroups(CDatabase* pDB)
{
	bool retval = true;

	NOutputWnd::PostLoadMsg("Creating CI Code groups ... ");

	CCICodeGroupDefSet defSet(pDB);
	CCICodeGroupXrefSet xrefSet(pDB);
    CCIBogieSet  bogieSet(pDB);       /// added k. hubbard 5-4-10

	try 
	{

//		defSet.Open(CRecordset::forwardOnly,
		defSet.Open(CRecordset::dynaset,
			0,
			CRecordset::readOnly);
		CreateGroups(defSet);

//		xrefSet.Open(CRecordset::forwardOnly,
		xrefSet.Open(CRecordset::dynaset,
			0,
			CRecordset::readOnly);
		PopulateGroups(xrefSet);

		bogieSet.Open(CRecordset::dynaset,   // added k. hubbard 5-4-10
			0,
			CRecordset::readOnly);
        Initialize80HSMBogies(bogieSet);       /// added k. hubbard 5-4-10
	}

	catch ( CDBException* e )
	{
		AfxMessageBox( e->m_strError,   
					   MB_ICONEXCLAMATION );

		e->Delete();

		retval = false;;
	}
	catch ( CCICodeGroupException& e )
	{
		ostrstream ostr;
		ostr << "Failure reading CI Codes: " 
			<< ( e.Type() == CCICodeGroupException::MISSING_GROUP
			? "MISSING_GROUP"
			: ( e.Type() == CCICodeGroupException::BAD_INDEX
			? "BAD_INDEX"
			: "ALREADY_DEFINED" ) )
			<< ", index = "
			<< e.Index()
			<< ends;

		AfxMessageBox( ostr.str(), 
				  	   MB_ICONEXCLAMATION );
		ostr.freeze(FALSE);

		retval =  false;
	}

	if ( defSet.IsOpen() )
		defSet.Close();

	if ( xrefSet.IsOpen() )
		xrefSet.Close();

	if ( bogieSet.IsOpen() )  // added k. hubbard 5-4-10
		bogieSet.Close();

	NOutputWnd::PostLoadMsg("done.\n");

	return retval;
}

//  #endif   // test comm out 5-3-10 k. hubbard

//
//  It is assumed that the CCICodeSet is open and initialized
//     to the initial position.
//

void CCICodeGroupMgr::CreateGroups(CCICodeGroupDefSet& rSet)
{
	// Wipe out any existing data
	Cleanup();


	// load records

	while ( ! rSet.IsEOF() ) {

		CreateGroup(rSet.m_CMDTY_XREF_CMDTY_GRP_CD,
					rSet.m_CMDTY_GRP_NAME,
					rSet.m_CMDTY_XREF_CMDTY_GRP_CD,
					rSet.m_BOGIE,    // added 10 bogie member fields below from CCICodeGroupDefSet 5-4-10 k. hubbard
					rSet.m_ROLLED,   
					rSet.m_ON_LINEUP,
					rSet.m_SLABBED_AVAIL,
					rSet.m_CMS,
					rSet.m_SHAVES,
					rSet.m_OTHER_UNAVAIL,
					rSet.m_TONS_910,
					rSet.m_TONS_909,
					rSet.m_TONS_REM_PLAN);

		rSet.MoveNext();
	}

	//  Sort 'em

	SortGroups();
}
	


//
//  Iterate through the ci code records and assign each to a group.
//

void CCICodeGroupMgr::PopulateGroups(CCICodeGroupXrefSet& rSet)
{
	while ( ! rSet.IsEOF() ) {

		AddCodeToGroup(atol(LPCTSTR(rSet.m_CMDTY_XREF_CMDTY_CD)),
					   rSet.m_CMDTY_XREF_CMDTY_GRP_CD);

		rSet.MoveNext();
	}
}

void CCICodeGroupMgr::Initialize80HSMBogies(CCIBogieSet& rSet)       /// added k. hubbard 5-4-10
{

//		Initialize(rSet);		
			while ( ! rSet.IsEOF() ) {
//				rset1.push_back
				rSet.MoveNext();
			}
#if 0
	}
	catch ( CDBException* e )
	{

	   AfxMessageBox( "Unable to upload for 80HSM Bogies due to database error: \n" + e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

///	   dbc.GetDB()->Rollback();

///	   return false;
	}

	if ( rSet.IsOpen() )
		rSet.Close();
#endif
}


//  Utility functions validating indexes for various operations

void CCICodeGroupMgr::VerifyCICodeInRange(int ciCode)
{
	if ( ! IsCICodeInRange(ciCode) )
		throw CCICodeGroupException(CCICodeGroupException::BAD_INDEX,ciCode);
}


void CCICodeGroupMgr::VerifyCICodeDefined(int ciCode)
{
	if ( ! IsCICodeDefined(ciCode) )
		throw CCICodeGroupException(CCICodeGroupException::MISSING_GROUP,ciCode);
}


void CCICodeGroupMgr::VerifyCICodeUndefined(int ciCode)
{
	if ( IsCICodeDefined(ciCode) )
		throw CCICodeGroupException(CCICodeGroupException::ALREADY_ASSIGNED,ciCode);
}


																		 
CCICodeGroup& CCICodeGroupMgr::GetGroup(int ciCode)
{
	VerifyCICodeDefined(ciCode);

	return *m_ciCodeToGroup[ciCode];
}


CCICodeGroup* CCICodeGroupMgr::GetGroupMaybe(int ciCode)
{
	return ( IsCICodeDefined(ciCode) ? m_ciCodeToGroup[ciCode] : 0 );
}


CCICodeGroup& CCICodeGroupMgr::GetGroupById(int groupId)
{
	T_GroupMap::iterator im = m_groupMap.find(groupId);

	if ( im == m_groupMap.end() )
		throw CCICodeGroupException(CCICodeGroupException::MISSING_GROUP,groupId);
	
	return *((*im).second);
}


CCICodeGroup* CCICodeGroupMgr::GetGroupByIdMaybe(int groupId)
{
	T_GroupMap::iterator im = m_groupMap.find(groupId);

	if ( im == m_groupMap.end() )
		return 0;

	return (*im).second;
}



void CCICodeGroupMgr::AddCodeToGroup(int newCICode, int groupId)
{
 	VerifyCICodeInRange(newCICode);
	VerifyCICodeUndefined(newCICode);

  	CCICodeGroup& existingGroup = GetGroupById(groupId);

	existingGroup.m_codes.push_back(newCICode);
	m_ciCodeToGroup[newCICode] = &existingGroup;
 }



void CCICodeGroupMgr::CreateGroup(int groupId, 
								  CString& name,
								  int sortOrder,   // added next 10 bogie fields below 5-4-10 k. hubbard
								  long myBogie,    
								  long myRolled,
								  long myOnLineup,
								  long mySlabbedAvail,
								  long myCms,
								  long myShaves,
								  long myOtherUnavail,
								  long myTons910,
								  long myTons909,
							      long myTonsRemPlan,
								  bool doSort)     
{
	if ( GetGroupByIdMaybe(groupId) )
		throw CCICodeGroupException(CCICodeGroupException::ALREADY_ASSIGNED,groupId);


	CCICodeGroup* pGroup = new CCICodeGroup(groupId,name,sortOrder, myBogie,
								myRolled, myOnLineup, mySlabbedAvail, myCms,
								myShaves, myOtherUnavail, myTons910, 
								myTons909, myTonsRemPlan);  // added previous 10 bogie fields 5-4-10 k. hubbard
	m_groups.push_back(pGroup);
	m_groupMap.insert( T_GroupMap::value_type(groupId,pGroup) );

	if ( doSort )
		SortGroups();
}


void CCICodeGroupMgr::SortGroups()
{
	sort(m_groups.begin(),
		 m_groups.end(),
		 less_p<CCICodeGroup*>());
}



//////////////////////////////////////////////////////////////
//	class CCICodeBogie
//////////////////////////////////////////////////////////////

//  This class represents a bogie record for a CCICodeGroup.
//  Objects of this class are managed by an instance of CCICodeBogieMgr.
//
//  There will typically be a 1-1 correspondence between CCICodeGroups
//		and CCICodeBogies withing a CCICodeBogieMgr.  
//  The CCICodeBogies within a CCICodeBogieMgr will all refer 
//		to CCICodeGroups from the same CCICodeGroupMgr.
//  Their is a direct analogy of
//
//    CCICodeGroup:CCICodeGroupMgr ::  CCiCodeBogie:CCICodeBogieMgr
//
//  The only reason we go to this much trouble is that is possible
//		to have multiple sets of bogies.  The spreadsheet from which
//      this is taken has two sets of figures with a week's difference,
//		e.g., the 3-day vs 10-day bogies.
//
//  A CCICodeBogieMgr will be hooked to a CCICodeGroupMgr.
//  The CCICodeBogies within the CCICodeBogieMgr will all map
//		to CCICodeGroups from withing that CCICodeGroupMgr.
//
//
//  The data fields within a CCICodeBogie map to columns in a spreadsheet.
//  Thus, a CCICodeBogie maps to a row from this spreadsheet.
//
//  Some of the fields are independent data, some are dependent.
//
//  The CCICodeBogie maintains const references to the CCICodeBogieMgr
//		which manages it, to the CCICodeGroup to which it is linked, and
//		to the CCICodeGroupMgr the CCICodeGroup is managed by (this is
//		redundant, but so what).
//
//  Operations on CCICodeBogie are:
//
//		void Initialize();
//			Set to zeros.
//
//		void Initialize(CCIBogieSet& rSet);
//			Initialize from a database record (corresponding to a row
//			in the originating spreadsheet).
//
//		void ComputeDependentVars();
//			What it says.  These vars are set to 0 if the value 
//			would otherwise be negative.


//  void CCICodeBogie::Initialize(CCIBogieSet& rSet)

void CCICodeBogie::Initialize()
{                                      // added population of 10 bogie fields here 5-4-10 k. hubbard
	m_bogie			= 0;
	m_rolled		= 0;
	m_onLu			= 0;
	m_slabbed		= 0;
	m_cms			= 0;
	m_shaves		= 0;
	m_otherUnavail	= 0;
	m_on910			= 0;
	m_on909			= 0;

//	m_bogie			= m_rCI.BogieData.m_Bogie; works for "struct" test of data structure 5-4-10 k. hubbard
//    if 	(m_rCI.m_Bogie != ' ')
//	{
//		m_bogie = m_rCI.m_Bogie;
//	}
//	else{
//		  m_bogie = m_rCI.m_Bogie;
//		}

//	m_bogie			= m_rCI.m_Bogie;
//	m_rolled		= m_rCI.m_Rolled;
//	m_rolled		= m_bogies.m_ROLLED;
//	m_rolled		= m_pCIBogieSet->m_ROLLED;
//    m_rolled		= m_rCI.GroupId()->ROLLED();
//	m_rolled		= m_pCIBogieSet->m_ROLLED();
//	m_onLu			= m_rCI.m_OnLineup;
//	m_slabbed		= m_rCI.m_SlabbedAvail;
//	m_cms			= m_rCI.m_Cms;
//	m_shaves		= m_rCI.m_Shaves;
//	m_otherUnavail	= m_rCI.m_OtherUnavail;
//	m_on910			= m_rCI.m_Tons910; 
//	m_on909			= m_rCI.m_Tons909;

///////////////////////////
///////////////////////////
	ComputeDependentVars();
}
	


void CCICodeBogie::ComputeDependentVars()
{
//   Added 9 member fields below 5-4-10 k. hubbard
	m_bogie			= m_rCI.m_Bogie;
	m_rolled		= m_rCI.m_Rolled;
	m_onLu			= m_rCI.m_OnLineup;
	m_slabbed		= m_rCI.m_SlabbedAvail;
	m_cms			= m_rCI.m_Cms;
	m_shaves		= m_rCI.m_Shaves;
	m_otherUnavail	= m_rCI.m_OtherUnavail;
	m_on910			= m_rCI.m_Tons910; 
	m_on909			= m_rCI.m_Tons909;

	int done1 = m_rolled + m_onLu;
	int done2 = done1 + m_slabbed + m_cms + m_shaves + m_otherUnavail + m_on910;
	int done3 = done2 + m_on909;

	long rem1 = m_bogie - done1;

	if ( rem1 < 0 ) {
		m_remainingToSchedule	= 0;
		m_amountOverOnLu		= -rem1;
	}
	else {
		m_remainingToSchedule	= rem1;
		m_amountOverOnLu		= 0;
	}

	m_remainingAfter910	= max(0,m_bogie-done2);
	m_remainingAfter909	= max(0,m_bogie-done3);
	m_amountOverBogie	= (m_bogie-done3 < 0 ? done3-m_bogie : 0);
}




///////////////////////////////////////////////////////////////
//
//  class CCICodeBogieMgr
//
///////////////////////////////////////////////////////////////

//  Manages a set of CCICodeBogies.
//  Rationale given above.
//
//  C-tor records the associated CCICodeGroupMgr.
//	D-tor cleans up allocated CCICodeBogies.
//
//  Maintains a (sorted) vector of allocated CCICodeBogies.
//
//		void InitializeBogies(CCIBogieSet& rSet)
//			read in bogie values from a database
//



CCICodeBogieMgr::CCICodeBogieMgr(CCICodeGroupMgr& rGrpMgr_)
: m_rGrpMgr(rGrpMgr_)
{
}


CCICodeBogieMgr::~CCICodeBogieMgr()
{
	Cleanup();
}



void CCICodeBogieMgr::Cleanup()
{
	release(m_bogies.begin(),m_bogies.end());
	m_bogies.erase(m_bogies.begin(),m_bogies.end());
}

#if 0
// 
bool CCICodeBogieMgr::Initialize80HSMBogies(CDatabase* pDB)       /// added k. hubbard 5-4-10

{
//	PSnapLongOpn->SetCurrentStep(STEP_CAST_RULES);
	
//	NOutputWnd::PostLoadMsg("Loading rules: ");

	bool retval = true;

    CCIBogieSet rSet(pDB);
///	CCheckStockApplicationRules rset1(pDB);
///	static CString strStatus = "3";
	long count = 0;

	try 
	{ 
			//rset.m_strSort = "[MILL_ORDER_NO]";
		{
			rSet.Open(CRecordset::forwardOnly,
						  0,
						  CRecordset::readOnly);
				
			while ( ! rSet.IsEOF() ) {
			
///				COrder* pOrder = new COrder;
///				Init(rset1);
///				pOrder->Init(rset1);
///				pOrder->Init(orderSet1);
///				m_orders.push_back(pOrder);
//	            rset1.AddNew();
//				rset1.push_back
				rSet.MoveNext();

				++count;
				NOutputWnd::WriteProgress(count);
			}
		
		}

	}
	catch ( CDBException* e )
	{

	   AfxMessageBox( "Unable to upload for 80HSM Bogies due to database error: \n" + e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

///	   dbc.GetDB()->Rollback();

	   return false;
	}

	if ( rSet.IsOpen() )
		rSet.Close();

	return true;
}
#endif
//void CCICodeBogieMgr::Initialize(CCIBogieSet& rSet)
//bool CCICodeBogieMgr::InitializeBogies(CDatabase* pDBPDA)
//bool CCICodeBogieMgr::InitializeBogies(CDatabase* /* pDB */)   commented out original 5-4-10 k. hubbard

bool CCICodeBogieMgr::InitializeBogies(CDatabase* pDB)

{

	NOutputWnd::PostLoadMsg("Loading CI code bogies ... ");

	Cleanup();

	// For now, just create one bogie record for each ci code group

	for ( CCICodeGroupMgr::T_GroupVec::const_iterator ig = m_rGrpMgr.GroupBegin();
		  ig != m_rGrpMgr.GroupEnd();
		  ++ig ) {

		CCICodeBogie* pBogie = new CCICodeBogie(*this,
												**ig,
												m_rGrpMgr);
// CCIBogieSet  bogieSet(pDB);       /// added k. hubbard 5-4-10
// pBogie->m_bogie = pBogie->CCIBogieSet->CI_Code(bogieSet);
		m_bogies.push_back(pBogie);
		pBogie->Initialize();
	}

	SortBogies();

	NOutputWnd::PostLoadMsg(" done.\n");

   return true;
}


void CCICodeBogieMgr::SortBogies()
{
	sort(m_bogies.begin(),
		m_bogies.end(),
		less_p<CCICodeBogie*>());
}


CCICodeBogie* CCICodeBogieMgr::BogiesAt(int index)
{
	assert( 0 <= index && index < m_bogies.size() );
	
	return m_bogies[index];
}
