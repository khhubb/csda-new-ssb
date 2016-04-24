// Order.h: interface for the COrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__F60F11C0_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_ORDER_H__F60F11C0_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef _modelTypes_included_
#include "modelTypes.h"
#endif

#include "Week.h"
#include "Chem.h"

class CUnappOrderSet;
class CDalyPlanOrdersOnlySet;
class CSSBInput;
class COrder;
class CSuperScen;


// Very convenient

typedef vector< COrder*> T_OrderVec;


class COrder 
{

	////////////////////////////////////////////////////////////////
	//	
	//	Represents a mill order.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:


	// the following fields are directly from 
	//	the P_MILL_ORDER and P_STEEL_MAKE_PLAN tables, and OrdersForDailyPlanOnly query  

	long	m_millOrderNum;
	CString	m_custName;
	long	m_custCode;
	int		m_pdtCode;
	long	m_automotiveCode;
	CWeek	m_hrYrWk;
	CString	m_orderedSpec;

	// at some point, we may be able to get rid of these
	CChem::Bound	m_minCarbon;
	CChem::Bound	m_maxCarbon;
	CChem::Bound	m_minMn;
	CChem::Bound	m_maxMn;
	CChem::Bound	m_minSignifEl;
	CChem::Bound	m_maxSignifEl;

	CString	m_signifElSym;
	int		m_ciCode;
	long	m_pgmExposure;
	Width	m_hbWidth;
	Gauge	m_hbGauge;
	Gauge	m_orderedGauge;
	Width	m_cbWidth;
	int		m_commodityPriority;
	int		m_orderPriority;
	int		m_slabCondnCode;
	Width	m_provSlabWidth;
	Thick	m_provSlabThickness;
	Length	m_provSlabLength;
	int		m_provSlabsDue;
	Weight	m_provSlabWgtDue;
	Weight	m_provSlabIndivSlabWgt;
	Weight	m_provMinSlabWgt;
	Weight	m_provMaxSlabWgt;
	Length	m_provMinSlabLength;
	Length	m_provMaxSlabLength;
	CString m_smPlanSteelSpec;
	CString m_smPlanSlabUnitCd;
	CString m_smPlanFaclPref1Cd;
	CString m_smPlanFaclPref2Cd;
	CString m_smPlanFaclPref3Cd;
	CString m_hmPlanMegaLuCd;
	CString m_hmPlanHotChargeInd;
	CString m_smPlanCastSpec;
	char	m_slApplyNewOrderCode;
	long	m_custCoilReq;
	Weight	m_provCastSlPcWgt;
	Length	m_provCastSlLnth;
	Width	m_orderedWidth;
	float	m_provHbYldFctr;
	float	m_cmsYieldLossFactor;
	Weight	m_custPieceWgt;
	Weight	m_provPiwWgt;
	long	m_plan910NoSlOrd;
	char	m_smPlanSlabsToPlanInd;
	long	m_smPlanSlabsOn909;
	char	m_smPlanOn909Ind;
	char	m_smPlanOn910Ind;
	char	m_smPlanDemandDueInd;
	Weight	m_slApply910PlanHbWgt;
	Weight	m_slApply910PlanSlWgt;
	Weight	m_slApply909PlanHbWgt;
	long	m_plannedLatestStartDate;
	long	m_plannedLatestStartTime;
	long	m_provHbProvTol;
	Width	m_provCastSlabWidthMax;   // added these 2 fields 11-11-03 k. hubbard
	Width	m_provCastSlabWidthMin;


	CChem::Bound m_chemMins[CChem::NumElements];
	CChem::Bound m_chemMaxs[CChem::NumElements];
	char m_chemReasons[CChem::NumElements];
		// Computed by chemistry code

	Weight	m_transitionMinWidth;
	Weight	m_transitionMaxWidth;
	Weight	m_nonTransMinWidth;
	Weight	m_nonTransMaxWidth;
		// Computed by ComputeSpreadSqueeze().


	CString m_fpOrderNum;
	long    m_fpMeltPstDate;
	long    m_fpLpstDate;
	int     m_fpIshroPrty;
	char    m_fpMtoFlowOrdInd;
	Weight	m_fpPlanSlabWgt;
	int		m_fpPlanNumPieces;	
		// this data comes from the factory planner order segments

	// cache the original m_provSlabsDue
	int m_origProvSlabsDue;

	// added for critical customer identification from p-mill-order 12-1-05 k.hubbard
	char	m_mordCriticalCustInd; 
	CString m_mordHotrollUnitName;   // added this field 1-24-07 k. hubbard
//	char    m_mordHotrollUnitName;   // added this field 1-24-07 k. hubbard

	CString m_crheatSpec;      		// added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	CString m_crInclusionMod;    // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////


public:


//	long OrderNum() const
//	{ return m_millOrderNum; }

	const CString& CustName() const
	{ return m_custName; }

	long CustCode() const
	{ return m_custCode; }

	int PdtCode() const
	{ return m_pdtCode; }

	long AutomotiveCode() const
	{ return m_automotiveCode;	}

	const CWeek& HrYrWk() const
	{ return m_hrYrWk; }

	const CString& OrderedSpec() const
	{ return m_orderedSpec; }

	const CString& CastSpec() const
	{ return m_smPlanCastSpec; }

	const CString& SteelSpec() const
	{ return m_smPlanSteelSpec; }

	CChem::Bound MinCarbon() const
	{ return m_minCarbon; }

	CChem::Bound MaxCarbon() const
	{ return m_maxCarbon; }

	CChem::Bound MinMn() const
	{ return m_minMn; }

	CChem::Bound MaxMn() const
	{ return m_maxMn; }
	
	CChem::Bound MinElement(CChem::Element elt) const
	{	return m_chemMins[elt];		}

	CChem::Bound MaxElement(CChem::Element elt) const
	{	return m_chemMaxs[elt];		}

	char ElementReason(CChem::Element elt) const
	{	return m_chemReasons[elt];	}

	void SetEltRange(CChem::Element elt, 
					 CChem::Bound minVal, 
					 CChem::Bound maxVal,
					 char reason)
	{	
		m_chemMins[elt] = minVal;
		m_chemMaxs[elt] = maxVal;
		m_chemReasons[elt] = reason;
	}

	CChem::Bound MinSignifEl() const
	{ return m_minSignifEl; }
	
	CChem::Bound MaxSignifEl() const
	{ return m_maxSignifEl; }

	const CString& SignifElSym() const
	{ return m_signifElSym; }

	int	CICode() const
	{ return m_ciCode; }

	long PgmExposure() const 
	{ return m_pgmExposure; }

	Width HbWidth() const
	{ return m_hbWidth; }

	Gauge HbGauge() const
	{ return m_hbGauge; }

	Gauge OrderedGauge() const
	{	return m_orderedGauge;	}

	Width CbWidth() const
	{ return m_cbWidth; }

	int CommodityPriority() const
	{ return m_commodityPriority; }

	int OrderPriority() const
	{ return m_orderPriority; }

	int SlabCondnCode() const
	{ return m_slabCondnCode; }

	Width SlabWidth() const
	{ return m_provSlabWidth; }   // Note: from P-STEEL-MAKE-PLAN 2-10-09 k. hubbard

	Thick SlabThickness() const
	{ return m_provSlabThickness; }

	Length SlabLength() const
	{ return m_provSlabLength; }

	Length MinSlabLength() const
	{ return m_provMinSlabLength;	}

	Length MaxSlabLength() const
	{ return m_provMaxSlabLength;	}

	int NumSlabsDue() const
	{ return m_provSlabsDue; }

	Weight SlabWgtDue() const
	{ return m_provSlabWgtDue; }

	Weight IndivSlabWgt() const
	{ return m_provSlabIndivSlabWgt; }

	Weight MinSlabWgt() const 
	{ return m_provMinSlabWgt; }

	Weight MaxSlabWgt() const
	{ return m_provMaxSlabWgt; }

	const CString& SlabUnitCd() const
	{ return m_smPlanSlabUnitCd; }

	const CString& FaclPrefCd(int i) const;

	const CString& MegaLuCd() const 
	{ return m_hmPlanMegaLuCd; }

	//  Later -- return type will be CTime,
	//           value returned will be from a field-to-be-named-later
	//  Part of the I2 inclusion

	long PlannedLatestStartDate() const
	{ return m_plannedLatestStartDate;	} 

	long PlannedLatestStartTime() const
	{	return m_plannedLatestStartTime; }

	Width MinAllowedSlabWidth(bool isTransition)
	{ return ( isTransition ? m_transitionMinWidth : m_nonTransMinWidth );	}

	Width MaxAllowedSlabWidth(bool isTransition)
	{ return ( isTransition ? m_transitionMaxWidth : m_nonTransMaxWidth );	}

	char NewOrderCode() const
	{	return m_slApplyNewOrderCode;	}


	long CustCoilReq() const
	{	return m_custCoilReq;		}

	Weight ProvCastSlPcWgt() const
	{	return m_provCastSlPcWgt;	}

	Length ProvCastSlLnth() const
	{	return m_provCastSlLnth;	}

	Width OrderedWidth() const
	{	return m_orderedWidth;	}  // Note: from P-MILL-ORDER (used for slab sales) 2-10-09 k. hubbard

	float ProvHbYldFctr() const
	{	return m_provHbYldFctr;	}

	float CmsYieldLossFactor() const
	{	return m_cmsYieldLossFactor;	}

	Weight CustPieceWgt() const
	{	return m_custPieceWgt;	}

	Weight ProvPiwWgt() const
	{	return m_provPiwWgt;		}

	const CString& HmPlanHotChargeInd() const
	{	return m_hmPlanHotChargeInd;	}

	long SmPlanSlabsOn909() const
	{	return	m_smPlanSlabsOn909;	}

	long& SmPlanSlabsOn909() 
	{	return	m_smPlanSlabsOn909;	}

	Weight	SlApply909PlanHbWgt() const
	{	return m_slApply909PlanHbWgt;	}

	Weight&	SlApply909PlanHbWgt() 
	{	return m_slApply909PlanHbWgt;	}


	Width  ProvSlabWidthMax() const          // added these 2 fields 11-11-03 k. hubbard  
	{   return m_provCastSlabWidthMax;  }	

	Width  ProvSlabWidthMin() const          // added these 2 fields 11-11-03 k. hubbard
	{   return m_provCastSlabWidthMin;  }	


	CString FpOrderNum() const 
	{	return m_fpOrderNum;	}

	long	FpMeltPstDate() const
	{	return m_fpMeltPstDate;	}

	long    FpLpstDate() const 
	{	return m_fpLpstDate;	}

	int     FpIshroPrty() const 
	{	return m_fpIshroPrty;	}

	char    FpMtoFlowOrdInd() const
	{	return m_fpMtoFlowOrdInd;	}

	Weight	FpPlanSlabWgt() const
	{	return m_fpPlanSlabWgt;	}

	int		FpPlanNumPieces() const
	{	return m_fpPlanNumPieces;	}


	int ParentNumSlabsDue() const 
	{	return m_origProvSlabsDue;	}

	// added for critical customer identification from p-mill-order 11-1-05 k.hubbard
	char	CriticalCustInd() const
	{	return m_mordCriticalCustInd;	}

	// added this field for identification of Slab Sales Customer orders value = 'OPLV' or 'CUST' not equal to '80HR' 1-24-07 k. hubbard
	const CString& HotrollUnitName() const   
	{   return m_mordHotrollUnitName;  }

//	// added this field for identification of Slab Sales Customer orders value = 'OPLV' 1-24-07 k. hubbard
//	char    HotrollUnitName() const   
//	{   return m_mordHotrollUnitName;  }

	const CString& HeatSpec() const  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	{	return m_crheatSpec;	}      		
	const CString& InclusionModified() const  // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	{	return m_crInclusionMod;	}      		


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	// compiler-defined default c-tor, copy c-tor, operator=, d-tor all okay.

	


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void Init(const CUnappOrderSet&);
	void Init(const CDalyPlanOrdersOnlySet&);

	Weight OrderedTons() const;
	CString GetBoxAnnType() const;

	// FP change
	//static COrder* FindOrder(int,T_OrderVec&);
	static COrder* FindOrder(CString&,T_OrderVec&);
	static COrder* FindCSDOrder(long millOrderNum,
							 T_OrderVec& orders);
	static void FindOrdersFromMillOrderNum(long millOrderNum,
										T_OrderVec& orders,
										T_OrderVec& matchingOrders);
	static void FindOrdersFromMillOrderNum(CString& orderNum,
										T_OrderVec& orders,
										T_OrderVec& matchingOrders);

	void IncrementNumSlabsDue(int incr) { m_provSlabsDue += incr; }
	void IncrementParentNumSlabsDue(int incr);

	void ZeroChems();
	void ComputeSpreadSqueeze();

	bool IsCMS() const;

	static bool IsCMS(int condn, const CString& spec);
	static bool IsCMSSpec(const CString& spec);


	void WriteNewStringBuilderRecord(CSSBInput& rs,CSuperScen* pScen, int userId) const;


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	// FP change

//	bool operator<(const COrder& x) const
//	{
//		return m_millOrderNum < x.m_millOrderNum;
//	}
	
	bool operator<(const COrder& x) const
	{
		return m_fpOrderNum < x.m_fpOrderNum;
	}

};




#endif // !defined(AFX_ORDER_H__F60F11C0_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
