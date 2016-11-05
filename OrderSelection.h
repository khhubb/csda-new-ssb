// OrderSelection.h: interface for the COrderSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERSELECTION_H__4545D59D_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_ORDERSELECTION_H__4545D59D_A6F5_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSnapshot;
class CSuperScen;

#include "CICode.h"
#include "modelTypes.h"
#include "Order.h"

#include "spec.h"




class COrderSelection;	// forward decl


class COrderSelectionGroup
{

	////////////////////////////////////////////////////////////////
	//	
	//  An instance of this class summarizes some set of orders.
	//  It is assumed that there is a vector of orders which are
	//    sorted so that grouped orders are contiguous and we can
	//    represent the range of records by iterators into the vector.
	//
	//  I had originally made this a nested class inside COrderSelection,
	//    but VC++ 5.0 puked on a vector of a nested class.  COMPILER BUG.
	//	
	////////////////////////////////////////////////////////////////

	friend class COrderSelection;


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:
	
	long m_numPieces;
		// sum of numPieces for each order in the group

	long m_numPriorityOrders;
		// count of orders in the group with priority 

	Weight m_totalTons;
		// Sum of tons for each order in the group.

	Weight m_currentBackTons;
		// Sum of back-order tons in the group

	T_OrderVec::const_iterator m_iBegin;
	T_OrderVec::const_iterator m_iEnd;
		// delimits a range in the vector of orders 
		//    (in the COrderSelection that owns this group)

	COrderSelection* m_pOwner;
		// backlink to the COrderSelection that owns this


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	long NumPieces() const
	{	return m_numPieces;			}

	long NumPriorityOrders() const
	{	return m_numPriorityOrders;	}

	Weight Tons() const
	{	return m_totalTons;				}

	Weight CurrentBackTons() const
	{	return m_currentBackTons;		}

	Weight FutureTons() const
	{	return Tons() - CurrentBackTons();	}


	T_OrderVec::const_iterator GroupBegin() const
	{	return m_iBegin;	}

	T_OrderVec::const_iterator GroupEnd() const
	{	return m_iEnd;		}

	long NumOrders() const
	{
		return m_iEnd - m_iBegin;
	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

private:
	
	// C-tor private because only needs to be created by COrderSelection

	COrderSelectionGroup(T_OrderVec::iterator, COrderSelection* pOwner_);
	COrderSelectionGroup(const COrderSelectionGroup& x,
						 T_OrderVec::iterator ib,
						 T_OrderVec::iterator ie,
						 COrderSelection* pOwner_);


	// default d-tor okay

private:

	// do not implement
	COrderSelectionGroup(const COrderSelectionGroup&);
	COrderSelectionGroup& operator=(const COrderSelectionGroup&);


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void AddOrder(COrder*,CSuperScen* pSS);
};





class COrderSelection  
{
	////////////////////////////////////////////////////////////////
	//	
	//	A template indicating which orders to select and how to group them
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_IncludeOrderType { INCLUDE_THIS_CASTER_ONLY,
						    INCLUDE_SWING_TONS,
							INCLUDE_ALL	 };

	enum E_SortField { SORT_CI, 
					 SORT_SPEC, 
					 SORT_WIDTH, 
					 SORT_SLAB_LENGTH,
					 SORT_80HSM_LU_TYPE, 
					 SORT_PRIORITY_CODE, 
					 SORT_HR_WEEK,
					 SORT_PST_DELTA_WEEK,
					 SORT_CONDN_CODE2,
					 SORT_PLANNED_START,
					 SORT_CARBON,
	                 SORT_MANGANESE,
					 SORT_SIGNIF_EL,
					 SORT_CUSTOMER };

	enum { NumSortCodes = SORT_CUSTOMER+1 };

	enum E_HrWkDesignator { HR_WK_CURRENT_BACK,
						  HR_WK_PLUS_1,
						  HR_WK_PLUS_2,
						  HR_WK_PLUS_3,
						  HR_WK_PLUS_4,
						  HR_WK_FUTURE };

	enum { NumHrWkDesignators = HR_WK_FUTURE+1 };


	enum E_PstRangeDesignator { PST_PRIO,
							    PST_LT_8,
		                        PST_8_14,
							    PST_15_21,
							    PST_22_28,
							    PST_29_35,
							    PST_GT_35 };
	enum { NumPstRangeDesignators = PST_GT_35+1 };


	typedef bool(T_CmpFcnDecl)(const COrder*, const COrder*);

	typedef bool(*T_CmpFcn)(const COrder*, const COrder*);
	typedef vector< T_CmpFcn > T_CmpVec;


	//  This is what I'd like, but the compiler complains
	//typedef vector< SortField > SortVec;

	//   This is what I use instead.
	typedef vector< int > T_SortVec;

	typedef map<E_SortField,CString> T_SortFieldStringMap;
	typedef map<E_SortField,T_CmpFcn>  T_SortFieldCmpFcnMap;
	typedef map<CString,E_SortField> T_SortFieldStringMapInverse;

	typedef vector< COrderSelectionGroup* > T_GroupVec;

	////////////////////////////////////////////////////////////////
	//	
	//	Static comparator methods
	//	
	////////////////////////////////////////////////////////////////

private:

	
	static T_CmpFcnDecl CmpByCICode			;
	static T_CmpFcnDecl CmpBySpec			;
	static T_CmpFcnDecl CmpByWidth			;
	static T_CmpFcnDecl CmpBy80HSMLuType	;
	static T_CmpFcnDecl CmpByPrioCode		;
	static T_CmpFcnDecl CmpByHRWeek			;
	static T_CmpFcnDecl	CmpByPSTDeltaWeek	;
	static T_CmpFcnDecl CmpByPlannedStart	;
	static T_CmpFcnDecl CmpByCondnCode2		;
	static T_CmpFcnDecl CmpBySlabLength		;
	static T_CmpFcnDecl CmpByCarbonRange	;
	static T_CmpFcnDecl CmpByManganeseRange	;
	static T_CmpFcnDecl CmpBySignifElRange	;
	static T_CmpFcnDecl CmpByCustomer		;

	static bool CmpByElementRange(const COrder*, const COrder*, CChem::Element);

	
	////////////////////////////////////////////////////////////////
	//	
	//	Nested class VarCmp -- variable comparitor
	//	
	////////////////////////////////////////////////////////////////


	class VarCmp : binary_function<const COrder*, const COrder*, bool>
	{
	public:
		
		VarCmp( T_CmpVec& initVec) { m_cmps = initVec; /* copy */ }
		void Add( T_SortVec& sortVec);
		bool operator()(const COrder*, const COrder*) const;
		bool Equal(const COrder*, const COrder*) const;

	private:

		T_CmpVec m_cmps;
	};


	////////////////////////////////////////////////////////////////
	//	
	//	Data members (static)
	//	
	////////////////////////////////////////////////////////////////

private:

	static bool sm_staticsInitialized;
		// Once-only initialization of static fields

	static void InitializeStatics();


	static T_SortFieldStringMap	sm_sortFieldStrings;
		// maps from the enum to the strings to use to represent the values

	static T_SortFieldCmpFcnMap	sm_sortFieldCmpFcns;
		// maps from the enum to the functions used to compute the sort

	static T_SortFieldStringMapInverse sm_stringsToSortFields;
		// inverse of sm_sortFieldStrings
	

	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////
	
private:
	
	bool m_isModified;	
		// Whenever a change is made, we mark this.
		// Helps to determine when we should archive
	
	int m_caster;
		// What caster this is for.

	CCICodeBogieMgr::T_BogiesVec m_selectedBogies;
		// Filter for orders whose CI codes are in this list.

	bool m_useAllCICodes;
		// Flag to determine if we should use m_selectedBogies in the filter.

	bool m_useAllSpecs;
	bool m_useCrossApp;
	vector<CString> m_selectedSpecs;
		// m_useAllSpecs, if true, means no filtering on spec
		// if false, then use the specs in m_selectedSpecs.
		// if only one spec in m_selectedSpecs, then we may include
		// all cross-applicable specs (if m_useCrossApp is true)

	bool m_useWidthRange;
	Width m_minWidth;
	Width m_maxWidth;
		// the flag determines if we filter by width.
		// the other two are the ranges of widths to filter

	E_IncludeOrderType m_includeOrders;
		// do we include orders for just caster, swing tons, or all orders.


	bool m_useHrwk[NumHrWkDesignators];  // 0 = current+back, +1, +2, +3, +4, +5&future 
		// For each category of HRWeek, do we include orders from that HRWeek or not 
	CWeek m_currentHrYrWk;
		// What to use as the HRWeek in computing the categories (which are relative)

	bool m_usePstRange[NumPstRangeDesignators];
		// for each category of PST week, do we include orders from that PSTWeek or not

	long m_pstRangeTons[NumPstRangeDesignators];
		// computed fields showing tons in the indicated ranges.

	bool m_use80HsmLuType;
	CString m_megaLuType;
		// flag determines if we use the filter for value of megaLuType on COrder
		// filter is a simple equality test

	bool m_usePriorityCode;
	int m_maxCIPriority;
	int m_minCIPriority;
		// flag determines if we use priority in filtering.
		// other variables determine the range to pass.

	bool m_includeNewOrders;
		// if true, do not pass orders unless the newordercode is blank.

	T_SortVec m_availSortFields;
	T_SortVec m_availGroupFields;
	T_SortVec m_groupFields;
	T_SortVec m_sortFields;
		// together m_availSortFields/m_sortFields exhaust the values in E_SortField
		// ditto for m_availGroupFields/m_groupFields.
		// the values in m_sortFields determine the sort order for the COrders.
		// the values for m_groupFields determine the grouping

	CChem::Bound m_minChems[CChem::NumElements];
	CChem::Bound m_maxChems[CChem::NumElements];
		// chemistry ranges to filter by

	T_OrderVec m_orders;
		// the orders, filtered, sorted by m_sortFields.

	T_GroupVec m_groups;
		// the groups, which delimit segments of orders that match on the basis
		//    of m_groupFields

	T_GroupVec m_selectedGroups;
		// Part of the GUI, really.  Groups that have been selected by the user.
		// Kept here because it is needed in multiple places in the interface.

	set<COrder*> m_specialSelectedOrders;
		// Also part of the GUI.  These are orders that for some reason we also
		//   want to view as selected, even though their group is not selected.

	T_OrderVec m_selectedOrders;
		// All the orders from the m_selectedGroups + m_specialSelectedOrders.
		// The specials come at the end.

	int m_normalSelectedOrderCount;
		// The number of orders in m_selectedOrder that come from m_selectedGroups
		// Because the specials come at the end, this delimits the non-specials.

	
	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	static const T_SortFieldStringMap& SortFieldStrings()
	{	return	sm_sortFieldStrings;	}

	static const T_SortFieldCmpFcnMap& SortFieldCmpFcns()
	{	return	sm_sortFieldCmpFcns;	}

	static const T_SortFieldStringMapInverse& StringsToSortFields()
	{	return sm_stringsToSortFields;	}


	static const CString& SortFieldString(E_SortField field);
	static E_SortField StringToSortField(const CString& string);


	int Caster() const
	{	return m_caster;			}

	void SetCaster(int newVal) // only used when making a copy of a string
	{  m_caster = newVal; }

	void SetModified(bool newVal) 
	{	m_isModified = newVal;	}

	bool IsModified() const 
	{	return m_isModified;		}


	const CCICodeBogieMgr::T_BogiesVec& SelectedBogies()
	{	return m_selectedBogies;	}

	bool UseAllCICodes() const
	{	return m_useAllCICodes;	}

	bool UseAllSpecs() const
	{	return m_useAllSpecs;		}

	bool UseCrossApp() const
	{	return m_useCrossApp;		}

	// We don't look at m_useCrossApp on casters 4,5
	bool ReallyUseCrossApp() const
	{	return m_useCrossApp && m_caster != Caster::C4 && m_caster != Caster::C5; }

	const CString& SelectedSpec(int i) const
	{	
		static const CString empty;

		assert ( 0 <= i);

		if ( i < m_selectedSpecs.size() )
			return m_selectedSpecs[i];
		else
			return empty;
	}

	const vector<CString>& SelectedSpecs() const
	{	return m_selectedSpecs;	}
	
	bool UseWidthRange() const
	{	return m_useWidthRange;	}

	Width MinWidth() const
	{	return m_minWidth;	}

	Width MaxWidth() const
	{	return m_maxWidth;	}

	E_IncludeOrderType IncludeOrders() const
	{	return m_includeOrders;	}

	bool UseHrwk(int week) const
	{	assert( 0 <= week && week < NumHrWkDesignators );
		return m_useHrwk[week];	}

	CWeek CurrentHrYrWk() const
	{	return m_currentHrYrWk;	}

	bool UsePstRange(int index) const
	{	assert( 0 <= index && index < NumPstRangeDesignators );
		return m_usePstRange[index];	}

	long PstRangeTons(int index) const
	{	assert( 0 <= index && index < NumPstRangeDesignators );
		return m_pstRangeTons[index];	}

	bool Use80HsmLuType() const
	{	return m_use80HsmLuType;	}

	const CString& MegaLuType() const
	{	return m_megaLuType;		}

	bool UsePriorityCode() const
	{	return m_usePriorityCode;	}

	int MaxCIPriority () const
	{	return m_maxCIPriority;	}

	int MinCIPriority () const
	{	return m_minCIPriority;	}

	CChem::Bound MinChem(int i)
	{	assert(0 <= i && i < CChem::NumElements);
		return m_minChems[i];	}

	CChem::Bound MaxChem(int i)
	{	assert(0 <= i && i < CChem::NumElements);
		return m_maxChems[i];	}

	bool IncludeNewOrders() const
	{	return m_includeNewOrders;	}


	bool SetUseAllCICodes(bool newVal);
	bool SetUseAllSpecs(bool newVal);
	bool SetUseCrossApp(bool newVal);
	bool SetSelectedSpecs(const vector<CString>& newSpecs);
	bool SetUseWidthRange(bool newVal);
	bool SetMinWidth(Width newVal);
	bool SetMaxWidth(Width newVal);
	bool SetIncludeOrders(E_IncludeOrderType newVal);
	bool SetUseHrwk(int week, bool newVal);
	bool SetCurrentHrYrWk(const CWeek& newVal);
	bool SetUsePstRange(int index,bool newVal);
	bool SetUse80HsmLuType(bool newVal);
	bool SetMegaLuType(const CString& newVal);
	bool SetUsePriorityCode(bool newVal);
	bool SetMaxCIPriority(int newVal);
	bool SetMinCIPriority(int newVal);
	bool SetMinChem(int i, CChem::Bound newVal);
	bool SetMaxChem(int i, CChem::Bound newVal);
	bool SetIncludeNewOrders(bool newVal);


	const T_SortVec& AvailGroupFields() const
	{	return m_availGroupFields;	}

	const T_SortVec& GroupFields() const
	{	return m_groupFields; }

	const T_SortVec& AvailSortFields() const
	{	return m_availSortFields;	}

	const T_SortVec& SortFields() const
	{	return m_sortFields;	}



	const T_OrderVec& Orders() const
	{	return m_orders;			}

	const T_GroupVec& Groups() const
	{	return m_groups;			}

	const T_GroupVec& SelectedGroups() const
	{	return m_selectedGroups;	}

	const set<COrder*>& SpecialSelectedOrders() const
	{	return m_specialSelectedOrders;	}

	const T_OrderVec& SelectedOrders() const
	{	return m_selectedOrders; }

	bool SelectedOrderIsSpecial(int index) const
	{
		return index >= m_normalSelectedOrderCount;
	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:


	COrderSelection(int caster_);
	COrderSelection(const COrderSelection&);
	COrderSelection& operator=(const COrderSelection&);
	virtual ~COrderSelection();

private:


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////
	
public:

	void InitializeGroupSortFields(T_SortVec& newGroups,
								   T_SortVec& newSorts);

	void SetGroupFields(T_SortVec& newGroups,
						CSuperScen* pSS);
	void SetSortFields(T_SortVec& newSorts,
					   CSuperScen* pSS);

	void SetSelectedGroups(const vector<int>& indexes,CSuperScen* pSS);

	void SetSelectedBogies(const vector<int>& indexes);

	void SelectMatchingGroups(vector<COrder*>& orders,
							  CSuperScen* pSS);

    void UpdateGroupsFromOrderChange(CSuperScen* pSS);
    void UpdateGroupsFromOrderChange(const vector<COrder*>& specials, 
	                                CSuperScen* pSS);

	void Init(CSuperScen* pSS);

	void AddSpecialSelectedOrders(const vector<COrder*>& specials) ;

	void AddSpecialSelectedOrder(COrder* pOrder) 
	{	m_specialSelectedOrders.insert(pOrder);	}

	//bool WriteOrdersForStringBuilder(CSuperScen* pSS);
	bool WriteOrdersForNewStringBuilder(CSuperScen* pSS, int userId);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:


	void Copy(const COrderSelection& x);

	void Cleanup(bool refilter = true);
	void FilterOrders(CSuperScen* pSS);
	void SortOrders();
	void SortSelectedOrders();
	void GroupOrders(CSuperScen* pSS);
	void SetGroupOrSortFields(COrderSelection::T_SortVec& newSorts,
							  COrderSelection::T_SortVec& selectedFields,
							  COrderSelection::T_SortVec& availFields);

	void AbsorbSpecialSelectedOrders(CSuperScen* pSS);

	//void ComputePstRangeTons();

	void AddToCmpVec(T_CmpVec&,T_SortVec&); 

	void Dump() const;
	void Print(ostream&) const;
	
	static bool IsPriorityOrder(COrder* pOrder);

//////////DM CHANGE: 2002-09-14: made this public
public:

	static E_PstRangeDesignator PstRange(COrder* pOrder);



};

#endif // !defined(AFX_ORDERSELECTION_H__4545D59D_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
