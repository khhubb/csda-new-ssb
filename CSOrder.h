// CSOrder.h: interface for the CCSOrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSORDER_H__F60F11C2_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_CSORDER_H__F60F11C2_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef _modelTypes_included_
#include "modelTypes.h"
#endif

#include "Order.h"

class CDalyPlanSet;
class COrder;
class CCastString;
class CSpec;
class CCasterStringOrdersSet;
class CSSBOutput;

class CCheckStockApplicationRules;   // added k. hubbard 4-4-08


class CCSOrder
{
	////////////////////////////////////////////////////////////////
	//	
	//	Represents an order scheduled in a cast string
	//	
	////////////////////////////////////////////////////////////////


	//  CCastStrings own CCSOrders, so they occasionally need access
	//  that no one else should have

	friend class CCastString;


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	////////////////////////////////////////////////////////////////
	//	
	//	Data members
	//	
	////////////////////////////////////////////////////////////////

private:

	int m_heatSeqNum;
		// the index of the heat this order is on.
		// in this system, the heat indexes are 0-based.
		// in DPA, the heat indexes were 1-based.

	int m_lotNum;
		// from DPA.  Used to order.  not really used in this system

	int m_strandNum;
		// which strand the order is on, 1 or 2.

	CCastString* m_pCastString;
		//  backpointer to the cast string this order is assigned to.

	long m_millOrderNum;
		//  millOrderNum of the order that's been scheduled.

	COrder* m_pOrder;
		//  pointer to the COrder that's been scheduled.
		//  m_pOrder, if not null, will match m_millOrderNum.
		//  it is possible for m_pOrder to be null if the order is missing
		//    or if this is a stock order.

	CTime m_absStartTime;
	CTime m_absFinishTime;
		//  The start/finish times calculated for this order by the hot metal editor

	long m_relStartTime;
	long m_relFinishTime;
		//  The start/finish times for this order relative to the start of 
		//    the scenario.

	CTimeSpan m_duration;
		// The duration of the order, the difference of the start and finish times

	Width m_slabWidth;
		// The width to cast at.

	Thick m_slabThickness;
		// the thickness.  always 9.25.

	Length m_slabLength;
		// the length of the slab to cast.
		// depends on the width

	int m_ciCode;
		// order commodity data k. hubbard 1-28-03

	CString m_hotrollUnitName;
		// order slab sale customer data k. hubbard 1-24-07

	CString m_heatSpec;
		// cast rule slab sale material direction spec rule data k. hubbard 5-20-08

	CString m_inclusionModified;
		// cast rule slab sale material direction spec rule data k. hubbard 5-20-08


		// added these 2 fields 11-11-03 k. hubbard
    Width m_provCastSlabWidthMin; // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
	Width m_provCastSlabWidthMax; // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard


	int m_slabCondnCode;
		// order data

	Length m_planSteelLengthMax;
	Length m_planSteelLengthMin;
		// from order data, but can be computed.

	Weight m_pieceWgt;
		// weight of one slab -- should be called m_slabWeight

	Weight m_tons;
		// total weight on the order
		// this is NumSlabs()*m_pieceWgt.

	int m_numPieces;
		// this is the number of pieces to be made.
		// not the same as the number of slabs
		// if this order is even slit, then  #slabs = 2*numPieces.
		// see the accessor NumSlabs() below.

	CString m_lotSpec;
	CSpec* m_pLotSpec;
		// Basically, same as order's cast spec, but with some modifications
		//   involving the 4th digit.

	char m_exposureCode;
		// from the order

	char m_changeFlag;
		// Marked coming from download as an order to which changes have been made.

	int m_dispCode;
	char m_slitTypeCode;
		// computed from the values on the order, the slabCondnCode, whether we
		//   are slitting, even slitting, etc.

//	bool m_isTransitionChecked;  // did not use, may need later 1-31-06 k. hubbard
		// this verifies check box is checked on a transition slab (different width than preceding slab)

	bool m_isTransition;
		// this is a transition slab (different width than preceding slab)

	bool m_isRadical;
		// this is a radical transition slab

	int m_outputCondnCode;
	int m_outputDispCode;
		//  the codes output to the mainframe are different than the ones
		//  we look at internally.

	vector< pair<int,int> > m_slabHeatAssigns;
		// used by the algorithm to assign orders to heats.

	CString m_slitReason;   // added slit slab reason data k. hubbard 2-26-10
	CString m_stockReason;  // added stock slab reason data k. hubbard 5-5-09
	CString m_stockCommercialReason;  // added stock slab reason data k. hubbard 5-5-09
 

	// added for FP order segment
	CString m_fpOrderNum;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////


public:

	int HeatSeqNum() const
	{	return m_heatSeqNum;		}

	void HeatSeqNum(int newVal ) 
	{	m_heatSeqNum = newVal;	}

	int LotNum() const
	{	return m_lotNum;			}


	int StrandNum() const
	{	return m_strandNum;		}

	CCastString* CastString() const 
	{	return m_pCastString;		}

//	long OrderNum() const 
//	{	return m_millOrderNum;	}

	COrder* Order() const
	{	return m_pOrder;			}

	CTime AbsStartTime() const
	{	return m_absStartTime;	}

	CTime AbsFinishTime() const
	{	return m_absFinishTime;	}

	long RelStartTime() const
	{	return m_relStartTime;	}

	long RelFinishTime() const 
	{	return m_relFinishTime;	}

	CTimeSpan Duration() const
	{	return m_duration;		}

	Weight Tons() const
	{	return m_tons;			}

	const CString& LotSpec() const
	{	return m_lotSpec;			}

	void LotSpec(const CString& newVal,int caster);

	char ExposureCode() const
	{	return m_exposureCode;	}

	void ExposureCode(char newVal)
	{	m_exposureCode = newVal;	}

	CSpec* PtrLotSpec() const	
	{	return m_pLotSpec;		}

	double MinCarbon() const
	{	return ( m_pOrder == 0 ?   0.0 :  m_pOrder->MinCarbon() );	}

	double MaxCarbon() const
	{	return ( m_pOrder == 0 ? 100.0 : m_pOrder->MaxCarbon() );	}

	double MinMn() const	
	{	return ( m_pOrder == 0 ?   0.0 : m_pOrder->MinMn() );		}

	double MaxMn() const
	{	return ( m_pOrder == 0 ? 100.0 : m_pOrder->MaxMn() );		}

	Thick SlabThickness() const
	{	return m_slabThickness;	}

	void SlabThickness(Thick newVal) 
	{	m_slabThickness = newVal;	}

	// This parm is used for flagging width jump violations in CastStringValidator added 11-22-05 k. hubbard

	Width OrderHotBandWidth() const
	{	return m_pOrder->HbWidth();		}

	// These parms are settable by the user

	Width SlabWidth() const
	{	return m_slabWidth;		}

	void SlabWidth(Width newVal)
	{	m_slabWidth = newVal;		}

	static void ComputeSlabWidthRelatedParms(Width newVal);

	Length SlabLength() const
	{	return m_slabLength;		}

	void SlabLength(Length newVal)
	{	m_slabLength = newVal;	}

	int NumPieces() const
	{	return m_numPieces;		}

	int NumSlabs() const
	{	return ( SlitTypeCode() == 'E' ? 2*m_numPieces : m_numPieces );	}

	void NumPieces(int newVal)
	{	
		m_numPieces = newVal;	
		ComputeTons();
	}

	Weight PieceWgt() const
	{	return m_pieceWgt;		}

	void PieceWgt(Weight newVal)
	{	
		m_pieceWgt = newVal;
		ComputeTons();
	}

	void ComputeTons()
	{	m_tons = NumSlabs()*m_pieceWgt/2000; }

// K. Hubbard added 1-28-03 testing 
	int	CICode() const              // from order.h data links to database via cdalyplanordersonlyset class   
	{ return m_ciCode; }
	
	void CICode(int newVal)
	{
		m_ciCode = newVal;          // get field names from order.h
	}
// end 1-28-03

// K. Hubbard added 1-24-07 maint testing 
	const CString& HRUnitName() const  	 // from order.h data links to database via cdalyplanordersonlyset class   
	{ return m_hotrollUnitName; }

   void HRUnitName(const CString& newVal);
///	{ 
///		m_hotrollUnitName = newVal;
///	}
// end 1-24-07 maint.

	const CString& CRHeatSpec() const  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	{ return m_heatSpec; }      		

	void CRHeatSpec(const CString& newVal);  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08

	const CString& CRInclModified() const  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	{ return m_inclusionModified; }      		
	
	void CRInclModified(const CString& newVal);  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08


// K. Hubbard 11-11-03 maint. begin here: 
	Width  ProvSlabWidthMax() const          // added these 2 fields 11-11-03 k. hubbard  
	{   return m_provCastSlabWidthMax;  }	
	
	void ProvSlabWidthMax(Width newVal)
	{
		m_provCastSlabWidthMax = newVal;
	}

	Width  ProvSlabWidthMin() const          // added these 2 fields 11-11-03 k. hubbard
	{   return m_provCastSlabWidthMin;  }	
	
	void ProvSlabWidthMin(Width newVal)
	{
		m_provCastSlabWidthMin = newVal;
	}
// K. Hubbard 11-11-03 maint. ends here.

	CString StockReason() const		// added these 2 reason fields 5-5-09 k. hubbard
	{   return m_stockReason;             }	

	void StockReason(CString newVal,int caster)
//	void StockReason(CString newVal)
	{
		m_stockReason = newVal;
	}


	CString StockCommercialReason() const		// added these 2 reason fields 5-5-09 k. hubbard
	{   return m_stockCommercialReason;             }	

	void StockCommercialReason(CString newVal,int caster)
	{
		m_stockCommercialReason = newVal;
	}


	CString SlitReason() const		// added these 2 reason fields 5-5-09 k. hubbard
	{   return m_slitReason; }	

//	void SlitReason(CString newVal,int caster)
	void SlitReason(CString newVal)
	{
		m_slitReason = newVal;
	}

	int SlabCondnCode() const
	{	return m_slabCondnCode;	}

	void SlabCondnCode(int newVal)
	{
		m_slabCondnCode = newVal;
		m_outputCondnCode = newVal;	
	}

	bool ChangeMarked() const
	{	return m_changeFlag != ' ';	}

	char ChangeFlag() const
	{	return m_changeFlag;	}

	void ChangeFlag(char x)
	{	m_changeFlag = x;		}

	int DispCode() const
	{	return m_dispCode;	}

	void DispCode(int newVal)
	{
		m_dispCode = newVal;
		m_outputDispCode = newVal;
	}

	char SlitTypeCode() const
	{	return m_slitTypeCode;	}

	void SlitTypeCode(char newVal)
	{	
		m_slitTypeCode = newVal;
		ComputeTons();
	}

//	bool IsTransitionChecked() const          // added these 2 fields 1-31-06 k. hubbard
//	{	return m_isTransitionChecked;	}     // did not use, may need later 1-31-06 k. hubbard

	bool IsTransition() const
	{	return m_isTransition;	}

	void IsTransition(bool newVal)
	{	m_isTransition = newVal;	}

	bool IsRadical() const
	{	return m_isRadical;	}

	void IsRadical(bool newVal)
	{	m_isRadical = newVal;	}

	bool IsSplit() const;
	bool IsSlit();
	static bool IsSplit(Width hbWidth, Width slabWidth);
	static bool IsSlit(Width slabWidth, Width maxSlabWidth);


	vector< pair< int, int > >& SlabHeatAssigns()
	{	return m_slabHeatAssigns;	}

	Length PlanSteelLengthMax() const
	{	return m_planSteelLengthMax; }

	void PlanSteelLengthMax(Length newVal)
	{	m_planSteelLengthMax = newVal;	}

	Length PlanSteelLengthMin() const
	{	return m_planSteelLengthMin; }

	void PlanSteelLengthMin(Length newVal)
	{	m_planSteelLengthMin = newVal;	}

	int OutputCondnCode() const
	{	return m_outputCondnCode;	}

	int OutputDispCode() const
	{	return m_outputDispCode;	}

	void OutputCondnCode(int newVal)
	{	m_outputCondnCode = newVal;	}

	void OutputDispCode(int newVal)
	{	m_outputDispCode = newVal;	}

	
	// added for FP order segment 

	CString FpOrderNum() const 
	{	return m_fpOrderNum;	}


	////////////////////////////////////////////////////////////////
	//	
	//	Ctors, dtor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCSOrder();
	CCSOrder(const CDalyPlanSet&,
			 int caster);
	CCSOrder(COrder*, 
			 int strandNum,
			 int caster);
	CCSOrder(const CCSOrder&);

	CCSOrder(const CCasterStringOrdersSet& ordersSet);

	CCSOrder(const CSSBOutput& sbSet,
			int caster);

	CCSOrder(const CCheckStockApplicationRules& x);  // added 4-4-08 k. hubbard

	// default d-tor okay

	// factory
	static CCSOrder* CreateStockOrder(const CString& spec,
		                              Width slabWidth,
									  Length slabLength,
									  int condnCode,
									  int numPieces,
									  int strandNum,
									  int casterNum,
									  CString InclusionModified,
									  CString stockReason,  // added reasons 5-5-09 k. hubbard
									  CString stockCommercialReason);  // added reasons 5-5-09 k. hubbard


private:

	// do not implement
	CCSOrder& operator=(const CCSOrder&);
	// do not implement



	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	CTimeSpan CalculateSlabDuration(int caster,int heatIndex,int heatSize);

	void ClearSlabHeatAssigns();
	void NoteSlabHeatAssign(int heatIndex);
	void NoteSlabHeatAssigns(int heatIndex, int numPcs);

	Length ComputeSlabLengthFromWidth(Width newWidth) const;
	static Length ComputeSlabLengthFromWidth(COrder* pOrder, Width newWidth);

	static Weight ComputeSlabWeight(Width width, Length length);

//   New Slab Sale Identification below, old CI code id line above. 1-24-07 k. hubbard 
	static int CCSOrder::ComputeDispCode(char slitTypeCode, int condnCode, int m_ciCode, long m_millOrderNum, CString m_mordHotrollUnitName, CString m_inclusionModified); // Assign Lot Level DispCodes. Added m_inclusionModified for cast rule influenced disp code assignment k. hubbard 5-20-08  


	Width MinAllowedSlabWidth();
	Width MaxAllowedSlabWidth();

	bool IsEquivalentLot( const CCSOrder* px, bool checkRadical ) const;
	void AbsorbLot( CCSOrder* px);
	CCSOrder* SplitLot(int heatNum, int numPieces);


	bool GetChemRange(CChem::Element elt, 
					  CChem::Bound& rMin,
					  CChem::Bound& rMax,
					  bool& rHasReasonCode) const;


	static void ComputeSlabWidthRelatedParms(int caster,
											COrder* pOrder,
											bool isTransition,
											Width width,
											int condnCode,
											int CICode,  // 1-28-03 k. hubbard
											const CString& HRUnitName,  // 1-24-07 k. hubbard
											int dispoCode,
											char slitTypeCode,
											int numPieces,
											Length minLength,
											Length aimLength,
											Length maxLength,
											int& rDispoCode,
											char& rSlitTypeCode,
											int& rNumPieces,
											Length& rMinLength,
											Length& rAimLength,
											Length& rMaxLength);

	bool IsSameLot( const CCSOrder* pOrder ) const;


	static void CalculateMinMaxLengths(COrder* pOrder,
									   Width width,
									   Length& rMinLength,
									   Length& rMaxLength);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////
	
private:

	void Init();
	void Init(const CDalyPlanSet&,
			  int caster);

	void Init(COrder*, 
			  int strandNum,
			  int caster);
	void Init(const CCSOrder&);
	void Init(const CCasterStringOrdersSet&);
	void Init(const CSSBOutput&,
			  int caster);

	void Init(const CCheckStockApplicationRules& x);  // added 4-4-08 k. hubbard


	static void CalculateMinMaxLengthsAux(Width width,
										 Thick thickness,									   
										 Weight adjustedMinSlabWgt,
										 Weight adjustedMaxSlabWgt,
										 Length& rMinL,
										 Length& rMaxL);
	
	static void CalculateLengths(int caster,
								 COrder* pOrder,
								 Width width,
								 Length& rMinLength,
								 Length& rLength,
								 Length& rMaxLength);


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////


public:            // added 4-4-08 k. hubbard
	
	CString m_If_Spec;   	 // from data links to database via cCheckSlabApplicationRules class   

//	const CString& m_Lmf_Spec() const  	 // from data links to database via cCheckSlabApplicationRules class   
//	{ return SpecName; }

//	void HeatSeqNum(int newVal ) 
//	{	m_heatSeqNum = newVal;	}

//	int LotNum() const
//	{	return m_lotNum;			}


//	int StrandNum() const
//	{	return m_strandNum;		}

//	CCastString* CastString() const 
//	{	return m_pCastString;		}

//	long OrderNum() const 
//	{	return m_millOrderNum;	}

//	COrder* Order() const
//	{	return m_pOrder;			}

#if 0
	// this was modeled on the DPA code.
	// I have reworked it to make it more understandable (perhaps)

	void PreCMSPercentageAdjustment(int caster);

	static void CalculateLengthsForCMS(int caster,
									  COrder* pOrder, 
									  Width width,
									  Length length,
									  Length& rMinLength,
									  Length& rLength,
									  Length& rMaxLength);
	static void CalculateLengthsForCMS(int caster,
									  COrder* pOrder,
									  Width width,
									  Length& rMinLength,
									  Length& rLength,
									  Length& rMaxLength);
	static void CalculateMinMaxLengthsForCMS(COrder* pOrder,
											Width width,
											Length& rMinLength,
											Length& rMaxLength);

	static void CalculateLengthsForNonCMS(int caster,
									     COrder* pOrder,
										 Width width,
										 Length& rMinLength,
										 Length& rLength,
										 Length& rMaxLength);

	static void CalculateMinMaxLengthsForNonCMS(COrder* pOrder,
											Width width,
											Length& rMinLength,
											Length& rMaxLength);


	//  this is taken from the old DPA code
	//  the commented out data elements were not being used in the code

	class U0340050Parms
	{
	public:

		//long	autoCode;
		//Weight	provCastSlPcWgt;
		//Width	hbWidth;
		//Weight	custPieceWgt;
		//Length	provCastSlLength;
		Width	m_provCastSlWidth;
		Thick	m_provCastSlThick;

		//float	provHbYldFctr;
		//Length	provSlabLength;
		Weight	m_provCastMinSlabWgt;
		Weight	m_provCastMaxSlabWgt;
	};

	static void U0340050( U0340050Parms& parms, 
						  Length& minL, 
						  Length& maxL );

#endif

	void SetLotSpecPtr(int caster);
};


#endif // !defined(AFX_CSORDER_H__F60F11C2_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
