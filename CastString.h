// CastString.h: interface for the CCastString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTSTRING_H__F60F11C3_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CASTSTRING_H__F60F11C3_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "modelTypes.h"
#include "orderSelection.h"

#include "CastStringId.h"
#include "CastStringMiscProps.h"
#include "CastStringSummaryRecord.h"
#include "CastStringHeat.h"
#include "CastStringHeatValidnError.h"
#include "RulesCheckLineupSet.h"

class CDalyPlanSet;
class CSSBOutput;
class CSuperScen;

class CCSOrder;
class COrder;
class CCasterScen;
class CSnapshot;

class CUploadMgr;


#include "CSOrder.h"


class CCastString
{
	////////////////////////////////////////////////////////////////
	//	
	//	A CCastString represents a sequence of orders to cast.
	//  Actually, in the case of multiple strands on a caster,
	//     multiple sequences of orders are involved.
	//  The orders are CCSOrder object.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef vector<       CCSOrder*> T_CSOrderVec;
	typedef vector<         COrder*> T_OrderVec;
	typedef vector< CCastStringHeat> T_HeatVec;

	// Each string has a status, based on the statuses of its heats.
	enum E_StringStatus { STRING_STATUS_910,
						  STRING_STATUS_909,
						  STRING_STATUS_NEW };

	//  The algorithm to allocate orders to heats automatically
	//		can be configured to ignore some of the initial heats
	//		or all of them.

	enum E_HeatLockStatus { HEATS_NONE_LOCKED,
						    HEATS_SOME_LOCKED,
						    HEATS_ALL_LOCKED };


	// some utility classes for filtering and sorting on basis of start time.

	class CFilterInvalidStartTimes : public unary_function<const CCastString*,bool>
	{
	public:
		bool operator()(const CCastString* x) 
		{
			return ! x->IsStartTimeValid();
		}
	};

	class CStartTimeCmp: public binary_function<const CCastString*, const CCastString*, bool>
	{
	public:

		bool operator()(const CCastString* x1, const CCastString* x2 )
		{
			return x1->CastStartTime() < x2->CastStartTime();
		}
	};


	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:
	
	CCasterScen* m_pCScen;
		// The scenario this string belongs to

	CCastStringId m_id;
		// Each string has a unique id.

	CCastStringMiscProps m_props;
		// Other defining properties of the string

	CCastString* m_pOrigString;
		// the original string from the snapshot that this was copied from
		// will be 0 if this is a new string
		// We keep track of this to determine if we need to archive the string
		//    in detail or (in case it has not changed) we can just say to use
		//    the snapshot version

	T_CSOrderVec m_strand[3];	
		// the sequence of orders on each strand
		// 1-based usage:  strand[0] not used, strand[1] and strand[2] are

	CCastStringSummaryRecord m_strandSummary[3];
		// summary information, per strand

	CCastStringSummaryRecord m_stringSummary;
		// summary information for the string as a whole

	vector<CCastStringSummaryRecord> m_heatSummaries[3];
		// summary information per heat, both per strand and total.
		// [1] for strand1, [2] for strand2, [0] for both

	T_HeatVec m_heats;
		// the sequence of heats 
		// the heats themselves have pointers into m_strand to indicate
		//    which orders are on which heats

	COrderSelection m_orderSelection;
		// the filter attached to this string

	E_HeatLockStatus m_heatLockStatus;
		// Heats that are locked do not participate in the reallocation
		//
	int m_numLockedHeats;
		// when m_heatLockStatus = HEATS_SOME_LOCKED, then this indicates
		//   the initial segment of heats that are locked.
	
	bool m_canEdit;
		//  Strings coming from the download are locked out from editing
		//    to prevent accidental changes.
		//  The user must explicitly unlock the string to enable editing.


	E_StringStatus m_status;
		//  910, 909, New

	vector<CCastStringHeatValidnError> m_validationErrors;
		//  The errors computed the last time validation was performed

	bool m_bIsValid;
		// no fatal errors last time validation was performed


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CCastString* OrigString()
	{
		return m_pOrigString;
	}

	void OrigString( CCastString* newVal)
	{
		m_pOrigString = newVal;
	}


	E_StringStatus Status() const
	{	return m_status;	}


	void Status(E_StringStatus newVal)
	{	m_status = newVal;	}

	void SetStatus();


	CCasterScen* CasterScen() const
	{	return m_pCScen;	}

	const CCastStringId& Id() const
	{	return m_id;		}

	const CCastStringMiscProps& Props() const
	{	return m_props;	}

	T_CSOrderVec& Strand(int index)
	{	
		assert( index == 1 || index == 2 );
		return m_strand[index];
	}

	T_CSOrderVec::iterator StrandBegin(int index)
	{	return Strand(index).begin();	}

	T_CSOrderVec::iterator StrandEnd(int index)
	{	return Strand(index).end();	}


	T_HeatVec& Heats()
	{	return m_heats;	}

	const T_HeatVec& Heats() const
	{	return m_heats;	}

	const CCastStringSummaryRecord& StrandSummary(int index) const
	{
		assert( index == 1 || index == 2 );
		return m_strandSummary[index];
	}

	const CCastStringSummaryRecord& StringSummary() const
	{	return m_stringSummary; }

	const vector<CCastStringSummaryRecord>& HeatSummaries() const
	{	return m_heatSummaries[0]; }

	const vector <CCastStringSummaryRecord>& HeatSummaries(int index) const
	{
		assert( index == 1 || index == 2 || index == 0 );
		return m_heatSummaries[index];
	}


	Weight SchedTons() const;
	Weight SchedTons(int strandNum) const;

	long TotalDuration() const;
	long OverallRelFinishTime() const;

	Weight MinHeatSize() const;
	Weight AimHeatSize() const;
	Weight MaxHeatSize() const;


	//  The only time these should be used is by an editor
	//   that will be changing them.

	CCastStringId& GetId()
	{	return m_id; }

	CCastStringMiscProps& GetProps()
	{	return m_props;	}


	COrderSelection& OrderSelection()
	{	return m_orderSelection; }

	const COrderSelection& OrderSelection() const
	{	return m_orderSelection; }


	bool CanEdit() const
	{	return m_canEdit;			}

	void CanEdit(bool newVal)
	{	m_canEdit = newVal;		}

	E_HeatLockStatus HeatStatus() const
	{	return m_heatLockStatus;	}

	void HeatStatus(E_HeatLockStatus newVal)
	{	m_heatLockStatus = newVal;	}


	int NumLockedHeats() const
	{	return m_numLockedHeats;	}

	void NumLockedHeats(int newVal) 
	{	m_numLockedHeats = newVal;	}


	CTime CastStartTime() const
	{
		return m_props.StartTime();
	}

	void SetCastStartTime(CTime t)
	{
		m_props.SetStartTime(t);
	}

	bool IsStartTimeValid() const
	{	return m_props.IsStartTimeValid();	}

	void SetIsStartTimeValid(bool newVal)
	{	m_props.SetIsStartTimeValid(newVal);	}

	CTimeSpan PourToOpenTime() const;
	CTimeSpan InterStringTime() const;
	Weight HotMetalTonsPerHeat() const;


	vector<CCastStringHeatValidnError>& ValidnErrors()
	{	return m_validationErrors;	}

	bool IsValid() const
	{	return m_bIsValid;	}

	void SetIsValid(bool newVal)
	{	m_bIsValid = newVal;	}



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastString(CCasterScen* pCScen_, 
				const CCastStringId& rId,
				const CCastStringMiscProps& rProp); 
	
	CCastString(CCasterScen* pCScen_, 
		        const CCastString& x, 
				CCastString* pOrigString_);

	CCastString(CCasterScen* pCScen, 
				const CCastStringId& rId,
				const CCastStringMiscProps& rProp, 
		        const CCastString& x);


	CCastString(CCasterScen* pCScen_, CDalyPlanSet&);

	CCastString(CCasterScen* pCScen_, CSSBOutput&);


	~CCastString();

private:
	// don't implement
	CCastString(const CCastString&);
	
	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CCastString& x) const
	{	return m_id < x.m_id;	}


private:
	
	// do not implement
	CCastString& operator=(const CCastString&);
	// do not implement
	

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void DeleteOrders(const vector<int>& selectedItemIndexes, 
					  int strandNum,
					  vector<COrder*>& deletedOrders);

	void InsertOrders(const vector<COrder*>& selectedItems, 
					  CSuperScen* pSS,
					  int strandNum, 
					  int heatIndex,
					  int orderIndex,
					  bool query = false);	

	void MoveOrders(const vector<int>& selectedItemIndexes, 
					int sourceStrand,
					int destnStrand,
					int heatIndex,
					int orderIndex);

	CCSOrder* InsertStockOrder(CString& spec,
							   Width slabWidth,
							   Length slabLength,
							   int condnCode,
							   int numPieces,
							   CSuperScen* pSS,
							   int strandNum,
							   int heatIndex,
							   int orderIndex,
							   CString InclusionModified,  // added material direction k. hubbard 5-20-08
							   CString stockReason,  // added reason 5-5-09 k. hubbard declaration error
							   CString stockCommercialReason);  // added reason 5-5-09 k. hubbard declaration error

	CCSOrder* InsertStockOrder(CSuperScen* pSS,
							   int strandNum,
							   int heatIndex,
							   int orderIndex);
							

	
	void AddCSOrder(CCSOrder*);

	void CalculateHeats();
	void CalculateSummary();

	void ModifyId(const CCastStringId& newId);
	void ModifyMiscProps(const CCastStringMiscProps& newProps);


	bool ModifyCSOrderParms(CSuperScen* pSuperScen,
							int strandNum,
							int index);

	bool EditMultipleOrders(int heatNum,
							int strandNum,
							T_CSOrderVec::iterator ioBegin,
							T_CSOrderVec::iterator ioEnd,
							CSuperScen* pSS);

	bool IsSameString( const CCastString* pString) const;

	bool Upload(CUploadMgr& uMgr, int newStatus);
	bool UploadDeleteRecords(CUploadMgr& uMgr);
//	bool UploadForRuleCheck();


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////
	
private:


	void Init(CDalyPlanSet&);
	void Init(CSSBOutput&);

	void RegisterCSOrder(CCSOrder* pCSOrder);
	void UnregisterCSOrder(CCSOrder* pCSOrder);

	void CalculateStrandSummary(int strandNum);
	void CalculateTransitions();
	void CalculateTransitions(int strandNum);

	CTimeSpan ComputeStdDuration(int heatNum, Weight tons);

	void FixHeatSequencing();
	void FixHeatSequencing(int strandNum);
	void ClearSlabHeatAssigns();
	void ClearSlabHeatAssigns(int strandNum);
	int ComputeFreezeEndpoints(vector<CCSOrder*>::iterator& rFreezeEnd1,
							   vector<CCSOrder*>::iterator& rFreezeEnd2);
	int MaxHeatSeqNum(int strandNum);
	void ComputeFreezeEndpoint(int strandNum, 
							   vector<CCSOrder*>::iterator& rFreezeEnd);
	void CalculateHeatsFrozen(int lastFrozenHeatIndex,
							  vector<CCSOrder*>::iterator end1,
							  vector<CCSOrder*>::iterator end2);
	void CalculateFrozenDurations(int strandNum,
								  vector<CCSOrder*>::iterator strandEnd,
								  vector<long>& durs,
								  vector<Weight>& weights);
	void CalculateLotEndpoints(int strandNum,
							   vector<CCSOrder*>::iterator strandEnd);
	int CalculateHeatsUnfrozen(int firstHeatIndex,
							    vector<CCSOrder*>::iterator begin1,
							    vector<CCSOrder*>::iterator begin2);
	int CalculateUnfrozenDurations(int firstHeatIndex,
									vector<CCSOrder*>::iterator begin1,
									vector<CCSOrder*>::iterator begin2);
	void ComputeDummyHeats(int heatCount);
	int PickStrand(int caster,
				   int heatCount,
				   Weight heatTons,
				   long heatStartTime,
				   vector<CCSOrder*>::iterator ic1,
				   int slabCount1,
				   long finishTime1,
				   vector<CCSOrder*>::iterator ic2,
				   int slabCount2,
				   long finishTime2);
	void ProgressHeat(int caster,
					  int& heatCount,
					  Weight& heatTons,
					  long& heatStartTime,
					  vector<CCSOrder*>::iterator& ic1,
					  int& slabCount1,
					  long& startTime1,
					  long& finishTime1,
					  vector<CCSOrder*>::iterator& ic2,
					  int& slabCount2,
					  long& startTime2,
					  long& finishTime2);
	void ProgressSlab(int caster,
					  int strandNum,
					  int& heatCount,
					  Weight& heatTons,
					  long& heatStartTime,
					  vector<CCSOrder*>::iterator& ic,
					  int& slabCount,
					  long& startTime,
					  long& finishTime,
					  long& otherStartTime,
					  long& otherFinishTime);
	void ProgressNextOrder(int strandNum,
						   int heatCount,
						   vector<CCSOrder*>::iterator& ic,
						   int& slabCount,
						   long& startTime,
						   long& finishTime);
	void AbsorbAdjacentLots(vector<CCSOrder*>::iterator begin1,
							vector<CCSOrder*>::iterator begin2);
	void AbsorbAdjacentLots(int strandNum,vector<CCSOrder*>::iterator begin);
	void SplitHeatBoundaryLots();
	void SplitHeatBoundaryLots(int strandNum);
	void InitializeHeatOrderPointers();



	void ComputeHeatSeqNumAndInsertIndex(int destnStrand,
										 int heatIndex,
										 int orderIndex,
										 int& heatSeqNum,
										 vector<CCSOrder*>::iterator& insertIter);


//	void UploadCSOrderForRuleCheck(CRulesCheckLineupSet& rset,
//								   CString& statusToUse,
//								   int heatIndex,
//								   CCastStringHeat& rHeat,
//								   int strandNum,
//								   int lotIndex,
//								   CCSOrder* pOrder);

	void UploadHeat( CUploadMgr& uMgr,
				     int newStatus,
					 int heatIndex,
					 CCastStringHeat& rHeat);

	void UploadHeatChems( CUploadMgr& uMgr,
						  CString& statusToUse,
						  int heatIndex,
						  CCastStringHeat& rHeat);


	void UploadCSOrder( CUploadMgr& uMgr,
						CString& statusToUse,
						int heatIndex,
						CCastStringHeat& rHeat,
						int strandNum,
						int lotIndex,
						CCSOrder* pOrder);

};




#endif // !defined(AFX_CASTSTRING_H__F60F11C3_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
