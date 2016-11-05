// OrderSelection.cpp: implementation of the COrderSelection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "OrderSelection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



#include "DBConn.h"
#include "CICode.h"
#include "UnappOrderSet.h"
#include "SSBInput.h"
#include "StringBuilderDlg.h"
#include "Snapshot.h"
#include "SuperScen.h"
#include "SpecCmp.h"
#include "NTime.h"
 

////////////////////////////////////////////////////////////////
//	
//	Sort constants
//	
////////////////////////////////////////////////////////////////

//  We use constants (integer) of enum E_SortField to designate the different fields
//     by which we can sort and group.
//  We need maps from  E_SortField -> string
//                     string      -> E_SortField
//                     E_SortField -> comparison function


bool COrderSelection::sm_staticsInitialized = false;

COrderSelection::T_SortFieldStringMap	COrderSelection::sm_sortFieldStrings;
COrderSelection::T_SortFieldCmpFcnMap	COrderSelection::sm_sortFieldCmpFcns;
COrderSelection::T_SortFieldStringMapInverse COrderSelection::sm_stringsToSortFields;



void COrderSelection::InitializeStatics()
{
	sm_staticsInitialized = true;

	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_CI,			"CI Code"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_SPEC,			"Spec"			)); 
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_WIDTH,			"Width"			));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_80HSM_LU_TYPE,	"80\" LU Type"	));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_PRIORITY_CODE,  "Prio code"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_CONDN_CODE2,	"Condn code"	));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_HR_WEEK,		"HR Week"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_PST_DELTA_WEEK,"PSTWk"			));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_PLANNED_START,	"Planned start"	));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_SLAB_LENGTH,	"Length"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_CARBON,		"Carbon"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_MANGANESE,		"Manganese"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_SIGNIF_EL,		"SignifEl"		));
	sm_sortFieldStrings.insert(T_SortFieldStringMap::value_type(SORT_CUSTOMER,		"Customer"		));

	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_CI,			CmpByCICode			));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_SPEC,			CmpBySpec			));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_WIDTH,			CmpByWidth			));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_80HSM_LU_TYPE,	CmpBy80HSMLuType	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_PRIORITY_CODE,	CmpByPrioCode		));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_CONDN_CODE2,	CmpByCondnCode2		));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_HR_WEEK,		CmpByHRWeek			));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_PST_DELTA_WEEK,CmpByPSTDeltaWeek	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_PLANNED_START,	CmpByPlannedStart	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_SLAB_LENGTH,	CmpBySlabLength		));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_CARBON,		CmpByCarbonRange	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_MANGANESE,		CmpByManganeseRange	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_SIGNIF_EL,		CmpBySignifElRange	));
	sm_sortFieldCmpFcns.insert(T_SortFieldCmpFcnMap::value_type(SORT_CUSTOMER,		CmpByCustomer		));


	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("CI Code",		SORT_CI				));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Spec",			SORT_SPEC			)); 
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Width",			SORT_WIDTH			));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("80\" LU Type",	SORT_80HSM_LU_TYPE	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Prio code",		SORT_PRIORITY_CODE	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Condn code",		SORT_CONDN_CODE2	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("HR Week",		SORT_HR_WEEK		));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("PSTWk",			SORT_PST_DELTA_WEEK	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Planned start",	SORT_PLANNED_START	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Length",			SORT_SLAB_LENGTH	));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Carbon",			SORT_CARBON			));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Manganese",		SORT_MANGANESE		));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("SignifEl",		SORT_SIGNIF_EL		));
	sm_stringsToSortFields.insert(T_SortFieldStringMapInverse::value_type("Customer",		SORT_CUSTOMER		));
}


//
//  Find the string for a given sort field
//

const CString& COrderSelection::SortFieldString(COrderSelection::E_SortField field)
{
	T_SortFieldStringMap::const_iterator im = sm_sortFieldStrings.find(field);

	assert( im != sm_sortFieldStrings.end() );

	return (*im).second;
}



//
//  Find the sort field for a given string
//

COrderSelection::E_SortField COrderSelection::StringToSortField(const CString& string)
{

	T_SortFieldStringMapInverse::const_iterator im
		= sm_stringsToSortFields.find(string);


	assert( im != sm_stringsToSortFields.end() );

	return (*im).second;
}



////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////



COrderSelection::COrderSelection(int caster_)
{

	if ( ! sm_staticsInitialized )
		InitializeStatics();

	m_caster = caster_;

	m_useAllCICodes = true;

	m_useAllSpecs = true;
	m_useCrossApp	= true;

	{ 
		for ( int i=0; i<CChem::NumElements; ++i ) {
			m_minChems[i] = 0;
			m_maxChems[i] = 100.0;
		}
	}

	m_useWidthRange = false;
	m_minWidth =  0.0;
	m_maxWidth = 99.9;

	m_includeOrders	= INCLUDE_ALL;

	{
		for ( int i=0; i<NumHrWkDesignators; ++i )
			m_useHrwk[i] = true;
	}


	// HR Week = current calendar week + 1
	m_currentHrYrWk = CWeek(CTime::GetCurrentTime()+CTimeSpan(7,0,0,0));  // add a week.

	{
		for ( int i=0; i<NumPstRangeDesignators; ++i ) {
			m_usePstRange[i] = true;
			m_pstRangeTons[i] = 0;
		}
	}


	m_use80HsmLuType		= false;
	//	megaLuType (CString)

	m_usePriorityCode		= false;
	m_maxCIPriority		= 99;
	m_minCIPriority		=  0;

	m_includeNewOrders	= true;

	m_groupFields.push_back(SORT_SPEC);
	m_groupFields.push_back(SORT_WIDTH); 
	m_groupFields.push_back(SORT_PRIORITY_CODE);
	m_groupFields.push_back(SORT_PST_DELTA_WEEK);
	m_availGroupFields.push_back(SORT_PLANNED_START);
	m_availGroupFields.push_back(SORT_HR_WEEK);
	m_availGroupFields.push_back(SORT_CI);
	m_availGroupFields.push_back(SORT_80HSM_LU_TYPE); 
	m_availGroupFields.push_back(SORT_CONDN_CODE2);
	m_availGroupFields.push_back(SORT_SLAB_LENGTH);
	m_availGroupFields.push_back(SORT_CARBON);
	m_availGroupFields.push_back(SORT_MANGANESE);
	m_availGroupFields.push_back(SORT_SIGNIF_EL);
	m_availGroupFields.push_back(SORT_CUSTOMER);

	m_sortFields.push_back(SORT_SPEC);
	m_sortFields.push_back(SORT_WIDTH); 
	m_sortFields.push_back(SORT_PLANNED_START);
	m_sortFields.push_back(SORT_SLAB_LENGTH);
	m_availSortFields.push_back(SORT_CI);
	m_availSortFields.push_back(SORT_SLAB_LENGTH);
	m_availSortFields.push_back(SORT_80HSM_LU_TYPE); 
	m_availSortFields.push_back(SORT_PRIORITY_CODE);
	m_availSortFields.push_back(SORT_HR_WEEK);
	m_availSortFields.push_back(SORT_PST_DELTA_WEEK);
	m_availSortFields.push_back(SORT_CONDN_CODE2);
	m_availSortFields.push_back(SORT_CARBON);
	m_availSortFields.push_back(SORT_MANGANESE);
	m_availSortFields.push_back(SORT_SIGNIF_EL);
	m_availSortFields.push_back(SORT_CUSTOMER);

	m_normalSelectedOrderCount = 0;

	m_isModified = true;
}




COrderSelection::COrderSelection(const COrderSelection& x)
{
	Copy(x);
}

COrderSelection& COrderSelection::operator=(const COrderSelection& x)
{
	if ( this != &x ) {
		Cleanup();
		Copy(x);
	}
	return *this;
}


void COrderSelection::Copy(const COrderSelection& x)
{
	if ( ! sm_staticsInitialized )
		InitializeStatics();

	m_caster			= x.m_caster;

	m_useAllCICodes		= x.m_useAllCICodes;
	m_selectedBogies	= x.m_selectedBogies; // copy

	m_useAllSpecs		= x.m_useAllSpecs;
	m_useCrossApp		= x.m_useCrossApp;
	m_selectedSpecs		= x.m_selectedSpecs;	// copy

	{
		for ( int i=0; i<CChem::NumElements; ++i ) {
			m_minChems[i] = x.m_minChems[i];
			m_maxChems[i] = x.m_maxChems[i];
		}
	}

	m_useWidthRange	= x.m_useWidthRange;
	m_minWidth		= x.m_minWidth;
	m_maxWidth		= x.m_maxWidth;

	m_includeOrders	= x.m_includeOrders;

	{
		for ( int i=0; i<NumHrWkDesignators; ++i ) 
			m_useHrwk[i] = x.m_useHrwk[i];
	}

	m_currentHrYrWk		= x.m_currentHrYrWk;

	{
		for ( int i=0; i<NumPstRangeDesignators; ++i ) {
			m_usePstRange[i] = x.m_usePstRange[i];
			m_pstRangeTons[i] = x.m_pstRangeTons[i];
		}
	}

	m_use80HsmLuType	= x.m_use80HsmLuType;
	m_megaLuType		= x.m_megaLuType;

	m_usePriorityCode	= x.m_usePriorityCode;
	m_maxCIPriority		= x.m_maxCIPriority;
	m_minCIPriority		= x.m_minCIPriority;

	m_includeNewOrders	= x.m_includeNewOrders;

	m_groupFields		= x.m_groupFields;		// copy
	m_sortFields		= x.m_sortFields;		// copy
	m_availSortFields	= x.m_availSortFields;	// copy
	m_availGroupFields	= x.m_availGroupFields;	// copy

	m_orders					= x.m_orders;							 // copy
	m_specialSelectedOrders		= x.m_specialSelectedOrders; // copy
	m_selectedOrders				= x.m_selectedOrders;				 // copy
	m_normalSelectedOrderCount	= x.m_normalSelectedOrderCount;

	for ( T_GroupVec::const_iterator ig = x.m_groups.begin();
		  ig != x.m_groups.end();
		  ++ig ) {


		// compute offsets of group endpoints
		int beginIndex = (*ig)->GroupBegin() - x.m_orders.begin();
		int endIndex   = (*ig)->GroupEnd()   - x.m_orders.begin();

		COrderSelectionGroup* pGroup 
			= new COrderSelectionGroup((**ig),
									   m_orders.begin()+beginIndex,
									   m_orders.begin()+endIndex,
									   this);

		m_groups.push_back(pGroup);

		if ( find(x.m_selectedGroups.begin(),
			      x.m_selectedGroups.end(),
				  (*ig)) != x.m_selectedGroups.end() )
			m_selectedGroups.push_back(pGroup);
	}

	m_isModified = true;
}




COrderSelection::~COrderSelection()
{
	Cleanup();
}





//  Cleanup all allocated goodies.

void COrderSelection::Cleanup(bool refilter /* = true */ )
{
	release( m_groups.begin(), m_groups.end());

	m_groups.clear();;
	m_selectedGroups.clear();
	m_selectedOrders.clear();
	m_normalSelectedOrderCount = 0;

	if ( refilter ) {
		m_orders.clear();
		m_specialSelectedOrders.clear();
	}
}



////////////////////////////////////////////////////////////////
//	
//	Modifier functions
//	
////////////////////////////////////////////////////////////////

// These are all responsible for changing some filter value
// and updating the modified field, if the value has changed.


//  Most of these modifiers have almost identical bodies
//  we make a little macro which does the trick
//  (1) Compare old value to new to detect if there is a change
//  (2) set the data member to the new value.
//  (3) if there has been a change, mark this as modified
//  (4) return a bool indicating if this value changed.

#define SETWMOD(member) \
	bool change = ( newVal != (member) ); \
	member = newVal; \
	if ( change ) SetModified(true); \
	return change


bool COrderSelection::SetUseAllCICodes(bool newVal)
{
	SETWMOD(m_useAllCICodes);
}

bool COrderSelection::SetUseAllSpecs(bool newVal)
{
	SETWMOD(m_useAllSpecs);
}

bool COrderSelection::SetUseCrossApp(bool newVal)
{	
	SETWMOD(m_useCrossApp);
}

bool COrderSelection::SetSelectedSpecs(const vector<CString>& newVal)
{	
	SETWMOD(m_selectedSpecs);
}

bool COrderSelection::SetUseWidthRange(bool newVal)
{
	SETWMOD(m_useWidthRange);
}

bool COrderSelection::SetMinWidth(Width newVal)
{
	SETWMOD(m_minWidth);
}

bool COrderSelection::SetMaxWidth(Width newVal)
{	
	SETWMOD(m_maxWidth);
}


bool COrderSelection::SetIncludeOrders(COrderSelection::E_IncludeOrderType newVal)
{
	assert( newVal == INCLUDE_THIS_CASTER_ONLY
			||
			newVal == INCLUDE_SWING_TONS
			||
			newVal == INCLUDE_ALL );

	SETWMOD(m_includeOrders);
}

bool COrderSelection::SetUseHrwk(int week, bool newVal)
{
	assert( 0 <= week && week < NumHrWkDesignators );
	SETWMOD(m_useHrwk[week]);
}

bool COrderSelection::SetCurrentHrYrWk(const CWeek& newVal)
{
	SETWMOD(m_currentHrYrWk);
}

bool COrderSelection::SetUsePstRange(int index, bool newVal)
{
	assert( 0 <= index && index < NumPstRangeDesignators );
	SETWMOD(m_usePstRange[index]);
}

bool COrderSelection::SetUse80HsmLuType(bool newVal)
{
	SETWMOD(m_use80HsmLuType);
}

bool COrderSelection::SetMegaLuType(const CString& newVal)
{
	SETWMOD(m_megaLuType);
}

bool COrderSelection::SetUsePriorityCode(bool newVal)
{
	SETWMOD(m_usePriorityCode);
}

bool COrderSelection::SetMaxCIPriority(int newVal)
{
	SETWMOD(m_maxCIPriority);
}

bool COrderSelection::SetMinCIPriority(int newVal)
{
	SETWMOD(m_minCIPriority);
}

bool COrderSelection::SetIncludeNewOrders(bool newVal)
{
	SETWMOD(m_includeNewOrders);
}


bool COrderSelection::SetMinChem(int i, CChem::Bound newVal)
{
	assert(0 <= i && i < CChem::NumElements);
	SETWMOD(m_minChems[i]);
}

bool COrderSelection::SetMaxChem(int i, CChem::Bound newVal)
{
	assert(0 <= i && i < CChem::NumElements);
	SETWMOD(m_maxChems[i]);
}




//
//  Set the selected bogies from indexes into the bogies vector
//	   maintained by the bogieMgr 
//

void COrderSelection::SetSelectedBogies(const vector<int>& indexes)
{
	//  Save the current values so later we can check for a change 

	CCICodeBogieMgr::T_BogiesVec oldSel;

	oldSel.insert(oldSel.end(),
				  m_selectedBogies.begin(),
				  m_selectedBogies.end());


	// Set the new bogies

	m_selectedBogies.clear();

	for ( vector<int>::const_iterator ii = indexes.begin();
		  ii != indexes.end();
		  ++ii ) 
			  
		m_selectedBogies.push_back(TheSnapshot.BogieMgr().BogiesAt((*ii)));


	// check for change

	if (oldSel.size() != m_selectedBogies.size() 
		||
		! equal(oldSel.begin(),oldSel.end(),m_selectedBogies.begin()) )
		
		SetModified(true);
}





////////////////////////////////////////////////////////////////
//	
//	Comparison functions
//	
////////////////////////////////////////////////////////////////

//  The functions below compare certain fields of orders.
//  They are the basis for grouping and sorting.

bool COrderSelection::CmpByCICode(const COrder* pOrder1, 
								  const COrder* pOrder2 )
{
	return pOrder1->CICode() < pOrder2->CICode();
}

bool COrderSelection::CmpBySpec(const COrder* pOrder1, 
								const COrder* pOrder2 )
{
	return pOrder1->CastSpec() < pOrder2->CastSpec(); 
}

bool COrderSelection::CmpByWidth(const COrder* pOrder1, 
								 const COrder* pOrder2 )
{
	// descending
	return long(pOrder1->SlabWidth()) > long(pOrder2->SlabWidth());
}


bool COrderSelection::CmpBySlabLength(const COrder* pOrder1,
									  const COrder* pOrder2)
{
	return long(pOrder1->SlabLength()) < long(pOrder2->SlabLength());
}


bool COrderSelection::CmpBy80HSMLuType(const COrder* pOrder1, 
									   const COrder* pOrder2 )
{
	return pOrder1->MegaLuCd() < pOrder2->MegaLuCd();
}

bool COrderSelection::CmpByPrioCode(const COrder* pOrder1, 
									const COrder* pOrder2 )
{
	return pOrder1->CommodityPriority() < pOrder2->CommodityPriority()
		   ||
		   (pOrder1->CommodityPriority() == pOrder2->CommodityPriority()
		    &&
			pOrder1->OrderPriority() < pOrder2->OrderPriority() );
}

bool COrderSelection::CmpByCondnCode2(const COrder* pOrder1, 
									  const COrder* pOrder2 )
{
	return pOrder1->SlabCondnCode()/100 < pOrder2->SlabCondnCode()/100;
}

bool COrderSelection::CmpByHRWeek(const COrder* pOrder1, 
								  const COrder* pOrder2 )
{
	return pOrder1->HrYrWk() < pOrder2->HrYrWk();
}

bool COrderSelection::CmpByPSTDeltaWeek(const COrder* pOrder1, 
										const COrder* pOrder2 )
{
	return NTime::PSDateDeltaWeekTruncated(pOrder1->PlannedLatestStartDate())
			<
		   NTime::PSDateDeltaWeekTruncated(pOrder2->PlannedLatestStartDate());
}

bool COrderSelection::CmpByPlannedStart(const COrder* pOrder1, 
										const COrder* pOrder2 )
{
	return pOrder1->PlannedLatestStartDate() < pOrder2->PlannedLatestStartDate();
}


bool COrderSelection::CmpByCarbonRange(const COrder* pOrder1,
									   const COrder* pOrder2 )
{
	return CmpByElementRange(pOrder1,pOrder2,CChem::ELT_C);
}


bool COrderSelection::CmpByManganeseRange(const COrder* pOrder1,
										  const COrder* pOrder2 )
{
	return CmpByElementRange(pOrder1,pOrder2,CChem::ELT_MN);
}


bool COrderSelection::CmpBySignifElRange(const COrder* pOrder1,
										 const COrder* pOrder2 )
{
	if ( pOrder1->SignifElSym() < pOrder2->SignifElSym() )
		return true;
	else if ( pOrder1->SignifElSym() > pOrder2->SignifElSym() )
		return false;
	else {
		CChem::Element elt;
		if ( ! CChem::ToElement(pOrder1->SignifElSym(),elt) )
			return true;
		else 
			return CmpByElementRange(pOrder1,pOrder2,elt);
	}
}


bool COrderSelection::CmpByElementRange(const COrder* pOrder1,
										const COrder* pOrder2,
										CChem::Element elt)
{
	return  pOrder1->MinElement(elt) < pOrder2->MinElement(elt)
		    ||
			( pOrder1->MinElement(elt) == pOrder2->MinElement(elt)
			  &&
			  pOrder1->MaxElement(elt) < pOrder2->MaxElement(elt) );
}


bool COrderSelection::CmpByCustomer(const COrder* pOrder1, 
									const COrder* pOrder2 )
{
	return pOrder1->CustName() < pOrder2->CustName();
}





//  A variable comparitor applies a set of comparitors.

bool COrderSelection::VarCmp::operator()(const COrder* p1, const COrder* p2) const
{
	for ( T_CmpVec::const_iterator ic = m_cmps.begin(); 
		  ic != m_cmps.end();
		  ++ic ) {

		if ( (*ic)(p1,p2) )
			return true;
		else if ( (*ic)(p2,p1) )
			return false;

	}

	return false;
}


//  We need the notion of equality in order to group
//  We are equal if each is not less than the other.

bool COrderSelection::VarCmp::Equal(const COrder* p1, const COrder* p2) const
{
	return ( ! (*this)(p1,p2) && ! (*this)(p2,p1) );
}



//  We need to build a VarCmp by adding in the desired comparitor functions.
//  We are typically given a vector of SortFields.
//  We must convert from SortFields to comparitor functions.

void COrderSelection::AddToCmpVec(T_CmpVec& cmpVec, T_SortVec& sortVec )
{	
	for ( T_SortVec::iterator is = sortVec.begin();
		  is != sortVec.end();
		  ++is ) {
	
		T_SortFieldCmpFcnMap::iterator im = sm_sortFieldCmpFcns.find(E_SortField((*is)));

		assert( im != sm_sortFieldCmpFcns.end() );

		cmpVec.push_back( (*im).second );
		
	}
		
}




///////////////////////////////////////////////////////////////
//	
//	Sorting and grouping
//	
////////////////////////////////////////////////////////////////

//
//  Sorting uses both the group fields and the sort fields.
//  Build the vector of comparitors, then sort. 
//

void COrderSelection::SortOrders()
{
	T_CmpVec vec;
	
	AddToCmpVec(vec,m_groupFields);

	stable_sort(m_orders.begin(),
				m_orders.end(),
				VarCmp(vec));
}


//
//  We sort the selected orders by the sort fields.
//  However, we always leave special selected orders at the end.
//  Thus, this sort only covers the beginning of the vector.
//

void COrderSelection::SortSelectedOrders()
{
	T_CmpVec vec;
	
	AddToCmpVec(vec,m_sortFields);

	stable_sort(m_selectedOrders.begin(),
				m_selectedOrders.begin()+m_normalSelectedOrderCount,
				VarCmp(vec));
}


//
// Assume the orders are sorted appropriately.
// A group is a sequence of contiguous orders that are equal
//   with respect to the groupFields.
//

void COrderSelection::GroupOrders(CSuperScen* pSS)
{
	//  if no orders, nothing to do
	if ( m_orders.size() == 0 )
		return;


	// build a VarCmp from the group fields

	T_CmpVec vec;
	AddToCmpVec(vec,m_groupFields);
	VarCmp varCmp(vec);


	// Create the first group, which begins with the first order

	COrderSelectionGroup* pGroup = new COrderSelectionGroup(m_orders.begin(),this);
	m_groups.push_back(pGroup);
	

	// inspect orders in sequence.
	// if this order does not belong to the current group 
	//    (on the basis of being Equal to the first order in the current group)
	//    then begin a new group starting with this order)
	// add the order to the current group.
	//
	// we play strange games withe end iEnd of the group.
	// since it always points past the last order in the group, a la iterators,
	//    -- well, read it.

	for ( T_OrderVec::iterator iv = m_orders.begin();
		  iv != m_orders.end();
		  ++iv ) {
			  
	
		if ( ! varCmp.Equal(*(pGroup->m_iBegin),(*iv)) ) {
			pGroup->m_iEnd = iv;
			pGroup = new COrderSelectionGroup(iv,this);
			m_groups.push_back(pGroup);
		}
		pGroup->AddOrder((*iv),pSS);
	}
		
	pGroup->m_iEnd = m_orders.end();	
}




///////////////////////////////////////////////////////////////
//	
//	COrderSelectionGroup
//	
////////////////////////////////////////////////////////////////



//
//  Set to default values
//

COrderSelectionGroup::COrderSelectionGroup(T_OrderVec::iterator iv, COrderSelection* pOwner)
{
	m_numPieces			= 0;
	m_numPriorityOrders	= 0;
	m_totalTons			= 0.0;
	m_currentBackTons	= 0.0;
	m_pOwner			= pOwner;
	m_iBegin			= iv;
}



//  
// Creating a copy group.
// This is done when copying a COrderSelection.
// The new group will ahve a new owner and will have new iterators
//   (because they point into the m_orders of the owner).
// However, if used properly, the calculated fields will be the same.
//

COrderSelectionGroup::COrderSelectionGroup(const COrderSelectionGroup& x,
										   T_OrderVec::iterator ib,
										   T_OrderVec::iterator ie,
										   COrderSelection* pOwner)
{
	m_numPieces			= x.m_numPieces;
	m_totalTons			= x.m_totalTons;
	m_numPriorityOrders	= x.m_numPriorityOrders;
	m_currentBackTons	= x.m_currentBackTons;

	m_iBegin	= ib;
	m_iEnd		= ie;

	m_pOwner	= pOwner;
}



//
//  Used during computation of groups.
//  One more order is being added to the group.
// 

void COrderSelectionGroup::AddOrder(COrder* pOrder,CSuperScen* pSS)
{
	m_numPieces += pSS->NumSlabsDue(pOrder);
	m_totalTons += pOrder->OrderedTons();

	if ( pOrder->CommodityPriority() <= 20 )
		++m_numPriorityOrders;

	//////////DM CHANGE: 2002-09-14: modified currentBackTons calculation

//	if ( pOrder->HrYrWk() <= m_pOwner->CurrentHrYrWk() )
//		m_currentBackTons += pOrder->OrderedTons();


	COrderSelection::E_PstRangeDesignator pstRange = COrderSelection::PstRange(pOrder);

	if ( pstRange == COrderSelection::PST_PRIO || pstRange == COrderSelection::PST_LT_8  )
		m_currentBackTons += pOrder->OrderedTons();

}



///////////////////////////////////////////////////////////////
//	
//	Utilities
//	
////////////////////////////////////////////////////////////////



void COrderSelection::Dump() const
{
#ifdef _DEBUG
	ostrstream ostr;
	Print(ostr);
	ostr << ends;

	afxDump << ostr.str();
	ostr.freeze(0);
#endif
}

void COrderSelection::Print(ostream& ostr) const
{
	ostr << "Order selection -- modified = " << (m_isModified ? "Yes" : "No") << "\n"
		<< "CI:      " << (m_useAllCICodes ? "Yes" : "No " );

	for ( CCICodeBogieMgr::T_BogiesVec::const_iterator ib = m_selectedBogies.begin();
	      ib != m_selectedBogies.end();
		  ++ib ) {

		const CCICodeGroup& rCI = (*ib)->GetCICodeGroup();

		for ( vector<int>::const_iterator ii = rCI.Codes().begin();
		      ii != rCI.Codes().end();
			  ++ ii )
			ostr << " " << setw(2) << (*ii);
	}

	ostr << "\n";

	ostr << "Spec:     " << (m_useAllSpecs ? "Yes" : "No " );

	for ( vector<CString>::const_iterator is = m_selectedSpecs.begin();
		  is != m_selectedSpecs.end();
		  ++is )
		ostr << " " << (LPCTSTR) (*is);

	ostr << "\n";

	ostr << "Width:    " << (m_useWidthRange ? "Yes" : "No " ) << m_minWidth << " " << m_maxWidth << "\n"
		 << "Include:  " << ( m_includeOrders == INCLUDE_THIS_CASTER_ONLY
							  ? "THIS_CASTER_ONLY"
							  : ( m_includeOrders == INCLUDE_SWING_TONS
								  ? "SWING_TONS"
								  : "ALL" ) ) << "\n"
		 << "HR Week:  " << m_currentHrYrWk.Week() << " " << m_currentHrYrWk.Year()
		 << ", C+B: " << (m_useHrwk[0] ? "Y" : "N")
		 << ",  +1: " << (m_useHrwk[1] ? "Y" : "N")
		 << ",  +2: " << (m_useHrwk[2] ? "Y" : "N")
		 << ",  +3: " << (m_useHrwk[3] ? "Y" : "N")
		 << ",  +4: " << (m_useHrwk[4] ? "Y" : "N")
		 << ",   F: " << (m_useHrwk[5] ? "Y" : "N")
		 << "\n"
		 << "PST Range: " 
		 << "Prio: "	<< (m_usePstRange[PST_PRIO]  ? "Y" : "N" )
		 << "< 8: "		<< (m_usePstRange[PST_LT_8]  ? "Y" : "N" )
		 << ", 8-14: "	<< (m_usePstRange[PST_8_14]  ? "Y" : "N" )
		 << ", 15-21"	<< (m_usePstRange[PST_15_21] ? "Y" : "N" )
		 << ", 22-28"	<< (m_usePstRange[PST_22_28] ? "Y" : "N" )
		 << ", 29-35: "	<< (m_usePstRange[PST_29_35] ? "Y" : "N" )
		 << ", > 35: "	<< (m_usePstRange[PST_GT_35] ? "Y" : "N" )
		 << "\n"
		 << "LU Type:  " << (m_use80HsmLuType ? "Yes" : "No " ) << (LPCTSTR) m_megaLuType << "\n"
		 << "Prio:     " << (m_usePriorityCode ? "Yes" : "No " ) << m_maxCIPriority << " " << m_minCIPriority << "\n";

	ostr << "MinChem:  ";

	{
		for ( int i=0; i<CChem::NumElements; ++i )
			ostr << setw(4) << (LPCTSTR) CChem::ToString(CChem::Element(i)) << " " 
			     << setw(6) << setprecision(3) << m_minChems[i];
	}

	ostr << "\n";

	ostr << "MaxChem:  ";

	{
		for ( int i=0; i<CChem::NumElements; ++i )
			ostr << setw(4) << (LPCTSTR) CChem::ToString(CChem::Element(i)) << " " 
			     << setw(6) << setprecision(3) << m_maxChems[i];
	}

	ostr << "\n";

	ostr << "Group by: ";
	
	{
		for ( T_SortVec::const_iterator is = m_groupFields.begin();
			  is != m_groupFields.end();
			  ++is ) {

			T_SortFieldStringMap::const_iterator im = sm_sortFieldStrings.find(COrderSelection::E_SortField((*is)));
			if ( im == sm_sortFieldStrings.end() )
				ostr << " ??";
			else 
				ostr << " " << (LPCTSTR) (*im).second;
		}
	}

	ostr << "\n";

	ostr << "Sort by: ";

	{
		for ( T_SortVec::const_iterator is = m_sortFields.begin();
			  is != m_sortFields.end();
			  ++is ) {

			T_SortFieldStringMap::const_iterator im = sm_sortFieldStrings.find(COrderSelection::E_SortField((*is)));
			if ( im == sm_sortFieldStrings.end() )
				ostr << " ??";
			else 
				ostr << " " << (LPCTSTR) (*im).second;
		}
	}
	
	ostr << "\n";
 
	ostr << "#orders:       " << setw(4) << m_orders.size() << "\n"
		 << "#groups:       " << setw(4) << m_groups.size() << "\n"
		 << "#sel grps:     " << setw(4) << m_selectedGroups.size() << "\n"
		 << "#spec sel ord: " << setw(4) << m_specialSelectedOrders.size() << "\n"
		 << "=========================================================\n";
}



///////////////////////////////////////////////////////////////
//	
//	Filtering
//	
////////////////////////////////////////////////////////////////

// 
//  Recompute the set of filtered orders
//  Note: this does not sort or compute groups
//

void COrderSelection::FilterOrders(CSuperScen* pSS)
{

	// prepare to compute tonnages for each pst range
	Weight tons[NumPstRangeDesignators];

	{
		for ( int i=0; i<NumPstRangeDesignators; ++i ) 
			tons[i] = 0.0;
	}


	// Iterate through all orders in the snapshot

	set<int> selectedCICodes;

	for ( CCICodeBogieMgr::T_BogiesVec::iterator ib = m_selectedBogies.begin();
	      ib != m_selectedBogies.end();
		  ++ib ) {

		const CCICodeGroup& rCI = (*ib)->GetCICodeGroup();

		selectedCICodes.insert(rCI.Codes().begin(),rCI.Codes().end());
			
	}


	CString casterStr;
	casterStr.Format("%d",m_caster);

	CSpecCmp cmp("");


	vector <CString> specsToUse;

	if ( ! m_useAllSpecs ) {

		// we are using spec filtering
		// Put the desired specs in specsToUse
		// if we are using cross-apps, compute the cross-app specs of the first selected spec
		// otherwise, just use the selected specs.
		if ( ReallyUseCrossApp() ) {
			const set<CString>& crossAppSpecs 
				= TheSnapshot.FindSteelSpecCrossApp( (m_selectedSpecs.size() > 0 
													 ? m_selectedSpecs[0] 
													 : "0000000") );
			for	( set<CString>::const_iterator is = crossAppSpecs.begin();
				  is != crossAppSpecs.end();
				  ++is ) {
				specsToUse.push_back( (*is) );
			}
		}
		else
			specsToUse.insert(specsToUse.end(),
							 m_selectedSpecs.begin(),
							 m_selectedSpecs.end());

//		if ( specsToUse.size() == 0 )
//			AfxMessageBox("No specs!");
	}

	//  
	//  Iterate through all orders and put on m_orders the ones that meet the criteria

	for ( T_OrderVec::iterator io = TheSnapshot.Orders().begin();
		  io != TheSnapshot.Orders().end();
		  ++io ) {


		int numPieces = pSS->NumSlabsDue((*io));
		if ( numPieces <= 0 )
			continue;


		int weeksDelta = (*io)->HrYrWk() - m_currentHrYrWk;

		bool hrWkOk = ( m_useHrwk[0] && weeksDelta <= 0 
			            ||
						m_useHrwk[1] && weeksDelta == 1
						||
						m_useHrwk[2] && weeksDelta == 2
						||
						m_useHrwk[3] && weeksDelta == 3
						||
						m_useHrwk[4] && weeksDelta == 4
						||
						m_useHrwk[5] && weeksDelta > 4 );
		if ( ! hrWkOk )
			continue;

		// Casters 1,2,3: 
		//   we compare against steelSpec unless we are using cross-apps,
		//   in which case we use orderedSpec
		// Casters 4,5: 
		//  We compare against westSpec if we are THIS_CASTER_ONLY,
		//   otherwise we compare against steelSpec (no cross-apps?)

		bool specOk = false;

		switch (m_caster) {
		case Caster::C1:
		case Caster::C2:
		case Caster::C3:
			if (m_useAllSpecs || !m_useCrossApp)
				cmp.SetString((*io)->SteelSpec());
			else
				cmp.SetString((*io)->OrderedSpec());

			specOk = (m_useAllSpecs
				||
				find_if(specsToUse.begin(),
				specsToUse.end(),
				cmp) != specsToUse.end());
			break;

		case Caster::C4:
		case Caster::C5:
		{
			CString spec;
			if (m_includeOrders == INCLUDE_THIS_CASTER_ONLY)
				spec = (*io)->WestSpec();
			else
				spec = (*io)->SteelSpec();

			specOk = (m_useAllSpecs
				||
				find(specsToUse.begin(),
				specsToUse.end(),
				spec) != specsToUse.end());
		}
			break;
		}

		if ( ! specOk )
			continue;

		bool casterOk = false;

		switch (m_includeOrders)
		{
		case INCLUDE_ALL:
			casterOk = true;
			break;

		case INCLUDE_SWING_TONS:
			casterOk = 
				((*io)->FaclPrefCd(1) == casterStr
				 ||
				 (*io)->FaclPrefCd(2) == casterStr
				 ||
				 (*io)->FaclPrefCd(3) == casterStr);
			break;

		case INCLUDE_THIS_CASTER_ONLY:

			switch (m_caster)
			{
			case Caster::C1:
			case Caster::C2:
			case Caster::C3:
				casterOk = (*io)->SlabUnitCd() == casterStr;
				break;
			case Caster::C4:
				casterOk = (*io)->IhCastUnitName() == "1CCM";
				break;
			case Caster::C5:
				casterOk = (*io)->IhCastUnitName() == "2CCM";
				break;
			}
			break;
		}
		
		if ( ! casterOk )
			continue;


		bool ciCodeOk = m_useAllCICodes
			            ||
						selectedCICodes.find((*io)->CICode()) != selectedCICodes.end();

		if ( ! ciCodeOk )
			continue;

		bool widthOk = ! m_useWidthRange
			           ||
			           (m_minWidth <= (*io)->SlabWidth() && (*io)->SlabWidth() <= m_maxWidth);

		if ( ! widthOk )
			continue;

		bool luTypeOk = ! m_use80HsmLuType
			            ||
						(*io)->MegaLuCd() == m_megaLuType;

		if ( ! luTypeOk )
			continue;
	
			
		bool prioOk = ! m_usePriorityCode
			          ||
					  ((*io)->CommodityPriority() <= m_maxCIPriority
					   &&
					   m_minCIPriority <= (*io)->CommodityPriority());
		
		if ( ! prioOk )
			continue;

		bool chemOk = true;
		for ( int i=0; i<CChem::NumElements; ++i ) {
			if ((*io)->MinElement(CChem::Element(i)) > MaxChem(i)
				||
				(*io)->MaxElement(CChem::Element(i)) < MinChem(i) ) {
				chemOk = false;
				break;
			}
		}

		if ( ! chemOk )
			continue;

		if (! m_includeNewOrders 
			&&
			(*io)->NewOrderCode() != ' ' )
			continue;

		// this test is last because they still want to compute the tons for unchecked ranges.

		E_PstRangeDesignator pstRange = PstRange((*io));

		tons[pstRange] += (*io)->OrderedTons();


		bool pstOk = m_usePstRange[pstRange];

		if ( ! pstOk )
			continue;


		// we passed all the tests, use the order
		m_orders.push_back((*io));

	}

	// ComputePstRangeTons();
	for ( int i=0; i<NumPstRangeDesignators; ++i ) 
		m_pstRangeTons[i] = long( tons[i] + 0.5 );

}





// static
COrderSelection::E_PstRangeDesignator COrderSelection::PstRange(COrder* pOrder)
{
	if ( IsPriorityOrder( pOrder ) )
		return PST_PRIO;

	long pstDelta = NTime::PSDateDelta(pOrder->PlannedLatestStartDate());

	if ( pstDelta < 8 )
		return PST_LT_8;
	else if ( pstDelta <= 14 )
		return PST_8_14;
	else if ( pstDelta <= 21 )
		return PST_15_21;
	else if ( pstDelta <= 28 )
		return PST_22_28;
	else if ( pstDelta <= 35 )
		return PST_29_35;
	else
		return PST_GT_35;
}


// static
bool COrderSelection::IsPriorityOrder(COrder* pOrder) 
{
	int comm = pOrder->CommodityPriority();
	int ord  = pOrder->OrderPriority();

	return comm < 10 || comm == 10 && ord < 4;
}


//  This code has been dispersed into FilterOrders

//void COrderSelection::ComputePstRangeTons() 
//{
//	Weight tons[NumPstRangeDesignators];
//
//	{
//		for ( int i=0; i<NumPstRangeDesignators; ++i ) 
//			tons[i] = 0.0;
//	}
//
//
//	for ( vector<COrder*>::iterator io = m_orders.begin();
//		  io != m_orders.end();
//		  ++io ) {
//		E_PstRangeDesignator pstRange = PstRange((*io));
//		tons[pstRange] += (*io)->OrderedTons();
//	}
//
//	for ( int i=0; i<NumPstRangeDesignators; ++i ) 
//		m_pstRangeTons[i] = long( tons[i] + 0.5 );
//
//#if 0
//	{
//		for ( int i=0; i<NumPstRangeDesignators; ++i ) 
//			afxDump << i << " " << m_pstRangeTons[i] << "\n";
//	}
//#endif
//
//
//}




///////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////


//
//  Called when the set of selected groups needs to be changed
//    (perhaps because the user selected some groups in a list box).
//  Any of the special selected orders which are in one of the selected groups
//    can be removed from that list.
//

void COrderSelection::SetSelectedGroups(const vector<int>& indexes, CSuperScen* pSS)
{

	m_selectedGroups.clear();
	

	for ( vector<int>::const_iterator ii = indexes.begin();
		  ii != indexes.end();
		  ++ii )
	
		m_selectedGroups.push_back(m_groups[(*ii)]);


	m_selectedOrders.clear();
	m_normalSelectedOrderCount = 0;

	for ( T_GroupVec::const_iterator ig = m_selectedGroups.begin();
	      ig != m_selectedGroups.end();
		  ++ig ) 

		m_selectedOrders.insert(m_selectedOrders.end(),(*ig)->m_iBegin,(*ig)->m_iEnd);

	// ## add cast to int
	m_normalSelectedOrderCount = (int)m_selectedOrders.size();


	AbsorbSpecialSelectedOrders(pSS);

	SortSelectedOrders();
}


//
//  Any special selected orders that are in the selected groups' orders  can be removed.
// 

void COrderSelection::AbsorbSpecialSelectedOrders(CSuperScen* pSS)
{
	set<COrder*> groupOrders;
	vector<int> indexes;
	vector<COrder*> newSpecials;

	for ( T_GroupVec::const_iterator ig = m_selectedGroups.begin();
	      ig != m_selectedGroups.end();
		  ++ig ) 

		groupOrders.insert((*ig)->m_iBegin,(*ig)->m_iEnd);

	for ( set<COrder*>::const_iterator io = m_specialSelectedOrders.begin();
		  io != m_specialSelectedOrders.end();
		  ++io)
		
		if (groupOrders.find((*io)) == groupOrders.end() 
			&& 
			(*io) != 0
			&&
			pSS->NumSlabsDue((*io)) > 0 )
			newSpecials.push_back( (*io) );

	m_specialSelectedOrders.clear();
	m_specialSelectedOrders.insert(newSpecials.begin(),
								   newSpecials.end());


	m_selectedOrders.insert(m_selectedOrders.end(),newSpecials.begin(),newSpecials.end());
}


//
// When the user selects orders from a lineup,
//   we want to select the groups which match the selected orders
//   in terms of spec and width.
//  Add the desired orders to the specialSelectedOrders,
//   so that an order will still show up even if it is not in a group.
//

void COrderSelection::SelectMatchingGroups(vector<COrder*>& orders,
										   CSuperScen* pSS)
{
	if ( orders.size() == 0 )
		return;
	
	// compute a set of specs and a width range from the given orders

	set<CString> specs;

	Width minWidth = orders.front()->SlabWidth();
	Width maxWidth = orders.front()->SlabWidth();
		
	for ( T_OrderVec::const_iterator io = orders.begin();
	      io != orders.end();
		  ++io ) {

		specs.insert( (*io)->SteelSpec() );

		minWidth = min(minWidth,(*io)->SlabWidth());
		maxWidth = max(maxWidth,(*io)->SlabWidth());
	}


	vector<int> indexes;

	for ( T_GroupVec::const_iterator ig = m_groups.begin();
	      ig != m_groups.end();
		  ++ig ) {
		
		for ( T_OrderVec::const_iterator igo = (*ig)->m_iBegin;
			  igo != (*ig)->m_iEnd;
			  ++igo )

			if ( specs.find((*igo)->SteelSpec()) != specs.end()
				 &&
				 minWidth <= (*igo)->SlabWidth()
				 &&
				 (*igo)->SlabWidth() <= maxWidth ) {

				// found a match -- record the index and skip to the next group
				indexes.push_back( ig - m_groups.begin() );
				break;
			}
	}

    m_specialSelectedOrders.insert(orders.begin(),orders.end());

	SetSelectedGroups(indexes,pSS);
}


//  The user has performed some operation which might 
//  cause orders to enter/leave the set of filtered orders.
//  The operation would be numPieces.
//  We want to maintain the list of selected groups,
//      plus also select any groups that just became part of the
//      set of filtered orders.
//  There are probably more efficient ways to do this,
//		but I despair coding them.

void COrderSelection::UpdateGroupsFromOrderChange(CSuperScen* pSS)
{
	// create the set of currently selected orders
	set<COrder*> selOrders;

	{
		for ( T_GroupVec::const_iterator ig = m_selectedGroups.begin();
			  ig != m_selectedGroups.end();
			  ++ig ) 
	
			selOrders.insert((*ig)->m_iBegin,(*ig)->m_iEnd);
	}

	// save a copy of the current specialSelectedOrders
	set<COrder*> saveSpecialOrders;

	// want to do the following, but compile won't let me
	//saveSpecialOrders.insert(specialSelectedOrders.begin(),
	// 						   specialSelectedOrders.end());

	InsertSet(saveSpecialOrders,m_specialSelectedOrders);


	// make a set of all (filtered) orders
	set<COrder*> oldOrders(m_orders.begin(),m_orders.end());


	// refilter and regroup
	Cleanup();
	FilterOrders(pSS);
	SortOrders();
	GroupOrders(pSS);


	// make a set of all (filtered) orders now
	set<COrder*> newOrders(m_orders.begin(),m_orders.end());



	//  Determine which orders have been added to the filtered list

	set<COrder*>::const_iterator iOld = oldOrders.begin();
	set<COrder*>::const_iterator iNew = newOrders.begin();


	while ( iNew != newOrders.end() ) {

		if ( iOld == oldOrders.end() 
			 ||
			 (*iNew) < (*iOld) ) {
			// we are off the end of the old orders,
			// or iOld has advance beyond iNew
			// so this new order is not matched, 
			// so this really is a newly added order
			// we add it to selOrders so we will select the group containing it
  
			selOrders.insert( (*iNew) );
			++iNew;
		}
		else if ( (*iOld) < (*iNew) ) {
			// we have an order which has disappeared from the new world
			// we add it to saveSpecialOrders so it will still be listed
			saveSpecialOrders.insert( (*iOld) );
			iOld++;
		}
		else { // match
			iOld++;
			iNew++;
		}
	}

	//  put back the saved special selected orders (wiped out by Cleanup)
	//   plus any additions

	// want to do the following, but compile won't let me
	//specialSelectedOrders.insert(saveSpecialOrders.begin(),
	//							   saveSpecialOrders.end());

	InsertSet(m_specialSelectedOrders,saveSpecialOrders);

	// now restore the selected groups	


	vector<int> indexes;

	for ( T_GroupVec::const_iterator ig = m_groups.begin();
	      ig != m_groups.end();
		  ++ig ) {
		
		for ( T_OrderVec::const_iterator igo = (*ig)->m_iBegin;
			  igo != (*ig)->m_iEnd;
			  ++igo )

			if (  selOrders.find((*igo)) != selOrders.end() ) {
			
				// found a match -- record the index and skip to the next group
				indexes.push_back( ig - m_groups.begin() );
				break;
			}
	}


	SetSelectedGroups(indexes,pSS);
}




// Similar to UpdateGroupsFromOrderChange above,
//   but some of the orders we are interested in may have been
//   filtered out already and so they won't appear unless we
//   first add them to the specialSelectedOrders.


void COrderSelection::UpdateGroupsFromOrderChange(const vector<COrder*>& specials,
												   CSuperScen* pSS)
{
	AddSpecialSelectedOrders(specials);

	UpdateGroupsFromOrderChange(pSS);
}


void COrderSelection::AddSpecialSelectedOrders(const vector<COrder*>& specials)
{
	for ( vector<COrder*>::const_iterator io = specials.begin();
		  io != specials.end();
		  ++io ) {

		AddSpecialSelectedOrder( (*io) );

	}
}



//  Called to change the current grouping and sorting.



//  Called to change the current grouping and sorting.

void COrderSelection::SetGroupFields(COrderSelection::T_SortVec& newGroups,
									 CSuperScen* pSS)
{
	SetGroupOrSortFields(newGroups,
						 m_groupFields,
						 m_availGroupFields);

	Cleanup(false);  // don't cleanup the orders, no need to refilter
	SortOrders();
	GroupOrders(pSS);

	//Dump();
}


void COrderSelection::SetSortFields(COrderSelection::T_SortVec& newSorts,
									 CSuperScen* pSS)
{
	SetGroupOrSortFields(newSorts,
						 m_sortFields,
						 m_availSortFields);

	SortSelectedOrders();

	//Dump();
}


//  This one does not involve updating anything,
//  just setting the fields for grouping and sorting.
//  Primary use:  restoring an archived template

void COrderSelection::InitializeGroupSortFields(COrderSelection::T_SortVec& newGroups,
											    COrderSelection::T_SortVec& newSorts)
{
	SetGroupOrSortFields(newGroups,
						 m_groupFields,
						 m_availGroupFields);
	SetGroupOrSortFields(newSorts,
						 m_sortFields,
						 m_availSortFields);
}




void COrderSelection::SetGroupOrSortFields(COrderSelection::T_SortVec& newFields,
										   COrderSelection::T_SortVec& selectedFields,
										   COrderSelection::T_SortVec& availFields)

{
	// check for values in range

	for ( T_SortVec::iterator ig = newFields.begin();
		  ig != newFields.end();
		  ++ig )
		assert( 0 <= (*ig) && (*ig) < NumSortCodes );
	
	// Probably we should check for no duplicates, but I didn't care enough.


	// copy the group and sort field vectors

	selectedFields = newFields;	// copy

	availFields.clear();

	//  Set the avail fields to any values not in the group and sort lists
	for ( int i = 0; i<NumSortCodes; ++i ) {

		if ( find(selectedFields.begin(),
			      selectedFields.end(),
				  E_SortField(i)) == selectedFields.end() )
			
			availFields.push_back(E_SortField(i));
	}
}






//  The big bomb
//  Get rid of all computed information
//
void COrderSelection::Init(CSuperScen* pSS)
{
	Cleanup();
	FilterOrders(pSS);
	SortOrders();
	GroupOrders(pSS);

	m_isModified = false;

	//Dump();
}



/////////////////////////////////////////////////////////////////////
//
//  Auto string builder input
//
/////////////////////////////////////////////////////////////////////

bool COrderSelection::WriteOrdersForNewStringBuilder(CSuperScen* pSS, int userId)
{
	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CStringBuilderDlg::ClearNewInputTable(dbc.GetDB(),userId);

	CSSBInput rs(dbc.GetDB());

	bool retval = true;

	try {
		
		dbc.GetDB()->BeginTrans();
		// rs.Open(CRecordset::dynaset,LPCTSTR(CStringBuilderDlg::InputTableName()),CRecordset::appendOnly); k. hubbard 3-7-03

		rs.Open(CRecordset::dynamic,LPCTSTR(CStringBuilderDlg::NewInputTableName()),CRecordset::none);

		for ( vector<COrder*>::const_iterator io = Orders().begin();
			  io != Orders().end();
			  ++io ) 
			(*io)->WriteNewStringBuilderRecord(rs,pSS,userId);

		dbc.GetDB()->CommitTrans();
	}
	catch ( CDBException* e )
	{
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();
		retval = false;
		dbc.GetDB()->Rollback();
	}

	if ( rs.IsOpen() )
		rs.Close();

	return retval;
}


//bool COrderSelection::WriteOrdersForStringBuilder(CSuperScen* pSS)
//{
//	CDBConn dbc(CDBConn::DB_DPA);
//	dbc.Connect();
//
//	CStringBuilderDlg::CreateInputTable(dbc.GetDB());
//	CStringBuilderDlg::ClearInputTable(dbc.GetDB());
//
//	CStringBuilderInputSet rs(dbc.GetDB());
//
//	bool retval = true;
//
//	try {
//		
//		dbc.GetDB()->BeginTrans();
//		// rs.Open(CRecordset::dynaset,LPCTSTR(CStringBuilderDlg::InputTableName()),CRecordset::appendOnly); k. hubbard 3-7-03
//
//		rs.Open(CRecordset::dynamic,LPCTSTR(CStringBuilderDlg::InputTableName()),CRecordset::none);
//
//		for ( vector<COrder*>::const_iterator io = Orders().begin();
//			  io != Orders().end();
//			  ++io ) 
//			(*io)->WriteStringBuilderRecord(rs,pSS);
//
//		dbc.GetDB()->CommitTrans();
//	}
//	catch ( CDBException* e )
//	{
//	   AfxMessageBox( e->m_strError,   
//                      MB_ICONEXCLAMATION );
//	   e->Delete();
//		retval = false;
//		dbc.GetDB()->Rollback();
//	}
//
//	if ( rs.IsOpen() )
//		rs.Close();
//
//	return retval;
//}








