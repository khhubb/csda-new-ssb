// Order.cpp: implementation of the COrder class.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function   :  This program reads in orders data resulting from Unapplied Orders Set and Daily Plan Orders Only Set and makes 
//                makes order info available to other class modules throughout via Order.h connects via m_pOrder->field x
//                including the Single String Builder. Spread Squeeze is computed here too.      
//	Maintenance:
//
//  k. hubbard  11-11-03: Added these 2 fields for reprovide limits request m_MORD_PROV_CAST_SL_WTH_MAX 
//	                      m_MORD_PROV_CAST_SL_WTH_MIN. per P. Fronczek
//
//  k. hubbard  11-13-03: Began loading Transition min max fields with m_provCastSlabWidthMin Max fields which come from new prov fields 
//                        from comment above per P. Fronczek.  
//
//  k. hubbard  12-11-03: Subtracting .7 from m_provCastSlabWidthMax fields on transitions. 
//
//  k. hubbard  11-17-04: Added if condition below to capture finished slab customer orders currently only
//                        Dofasco. Customer in this ci code group require a decimal precision reflected in CSD 
//                        and for shop use via english to metric length conversions. See k. hubbard 11-17-04 maint.	
//
//  k. hubbard  12-1-05:  Added critical customer field population and identification from p-mill-order. see comments 12-1-05 k.hubbard
//
//  k. hubbard  1-18-06:  Subtraction .7 from m_provCastSlabWidthMax fields on transitions widths increased to 1". per P. Fronczek. 
//                        See k. hubbard 1-18-06 maint. 
//
//  k. hubbard  01-24-07: Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers and release of 
//	                      CI code = 86 identification. Used MFC Class Wizard's, Member Variables, Update Columns button linked to 
//                        OrdersForDailyPlanOnly VIEW fields in newinl via ODBC path. per T. Fronczek
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

#include "stdafx.h"


#include "csda.h"
#include "Order.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



#include "UnappOrderSet.h"
#include "DalyPlanOrdersOnlySet.h"
#include "MiscConstants.h"
#include "SSBInput.h"
#include "SuperScen.h"
#include "Snapshot.h"




////////////////////////////////////////////////////////////////
//	
//	Accessors
//	
////////////////////////////////////////////////////////////////

// most are inline

//
// There are up to three facility preference codes.
//

const CString& COrder::FaclPrefCd(int i) const
{
	switch ( i ) {
	case 1:
		return m_smPlanFaclPref1Cd;

	case 2:
		return m_smPlanFaclPref2Cd;

	case 3:
		return m_smPlanFaclPref3Cd;

	default:
		assert( false );
		return m_smPlanFaclPref1Cd;
	}
}





////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////


//
//  Set the chemistries array to full range for each chem.
//

void COrder::ZeroChems()
{
	for ( int elt = 0;  elt< CChem::NumElements; ++elt ) {
		m_chemMins[elt] = 0.0;
		m_chemMaxs[elt] = 100.0;
		m_chemReasons[elt] = ' ';
	}
}




//
//  Initialize from a database record (for unapplied orders)
//

void COrder::Init(const CUnappOrderSet& x)
{

  	m_millOrderNum			= x.m_MILL_ORDER_NO;
	m_custName				= x.m_MORD_CUSTOMER_NAME;
	m_custCode				= x.m_MORD_CUSTOMER_CODE;
	m_pdtCode				= x.m_MORD_PRODUCT_CODE;
	m_automotiveCode		= x.m_MORD_AUTOMOTIVE_CODE;

//	m_hrYrWk.Set(x.m_MORD_HOT_ROLL_YEAR,
//			     x.m_MORD_HOT_ROLL_WEEK);

	m_hrYrWk.Set(x.m_MORD_HOT_ROLL_YEAR,x.m_FP_CAST_DEMAND_YR_WK); // Added 2-08-05 maint. k. hubbard

	m_orderedSpec			= x.m_MORD_ORDERED_SPEC;
	m_minCarbon				= x.m_MORD_CARBON_MIN;
	m_maxCarbon				= x.m_MORD_CARBON_MAX;
	m_minMn					= x.m_MORD_MANGANESE_MIN;
	m_maxMn					= x.m_MORD_MANGANESE_MAX;
	m_minSignifEl			= x.m_MORD_SIGNIF_EL_MIN;
	m_maxSignifEl			= x.m_MORD_SIGNIF_EL_MAX;
	m_signifElSym			= x.m_MORD_SIGNIF_EL_SYM;
	m_ciCode				= x.m_MORD_PRODUCT_CMDTY_CD;
	m_pgmExposure			= x.m_MORD_PGM_EXPOSURE;
	m_hbWidth				= x.m_MORD_HOT_BAND_WIDTH;
	m_hbGauge				= x.m_MORD_HOT_BAND_GAUGE;
	m_orderedGauge			= x.m_MORD_ORDERED_GAUGE;
	m_cbWidth				= x.m_MORD_COLD_BAND_WIDTH;
	m_commodityPriority		= x.m_MORD_PLAN_PRTY_WK_DAY/10%100;  // left 2 digits, of 3
	m_orderPriority			= x.m_MORD_PLAN_PRTY_WK_DAY%10;      // rightmost digit
	m_slabCondnCode			= x.m_MORD_SLAB_CONDITION_CD;
	m_provSlabWidth			= x.m_PROV_SLAB_WIDTH;
	//m_provSlabThickness	= x.m_PROV_SLAB_THICKNESS;
	m_provSlabThickness		= 9.25;
//
	m_mordHotrollUnitName	= x.m_MORD_HOTROLL_UNIT_NAME;  // Added for SLAB SALES Customer identification from p-mill-order 1-24-07 k.hubbard

//	m_mordHotrollUnitName	= x.m_MORD_HOTROLL_UNIT_NAME.GetLength() == 0  	// Char data types init this way. Added for critical customer identification from p-mill-order 12-1-05 k.hubbard
//                              ? ' '
//							  : x.m_MORD_HOTROLL_UNIT_NAME[0];
    m_crheatSpec			= x.m_HEAT_SPEC;      		// added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
    m_crInclusionMod		= x.m_INCLUSION_MODIFIED;   // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08

//     Added if condition below to capture finished slab customer orders currently only
//     Dofasco. Customer in this ci code group require a decimal precision reflected in CSD 
//     and for shop use via english to metric length conversions. k. hubbard 11-17-04 maint.	
//	if ( m_ciCode == 86 )
	if ( m_mordHotrollUnitName.Left(4) == "80HR" )  // added this field for new identification of Slab Sales customer orders 1-24-07 k. hubbard
	   m_provSlabLength		= x.m_PROV_SLAB_LENGTH;
	else
		m_provSlabLength	= x.m_MORD_ORDERED_LENGTH;  // Slab Sale Customer


//	m_provSlabLength		= x.m_PROV_SLAB_LENGTH;
	m_provSlabsDue			= x.m_PROV_SLABS_DUE;
	m_provSlabWgtDue		= x.m_PROV_SLAB_WGT_DUE;
	m_provSlabIndivSlabWgt	= x.m_PROV_INDV_SLAB_WGT;
	m_provMinSlabWgt		= x.m_MORD_PROV_MIN_SLAB_WGT;
	m_provMaxSlabWgt		= x.m_MORD_PROV_MAX_SLAB_WGT;
	m_provMinSlabLength		= x.m_MORD_MINIMUM_SLAB_LENGTH;
	m_provMaxSlabLength		= x.m_MORD_MAXIMUM_SLAB_LENGTH;
	m_smPlanSteelSpec		= x.m_SM_PLAN_STEEL_SPEC;
	m_smPlanSlabUnitCd		= x.m_SM_PLAN_SLAB_UNIT_CD;
	m_smPlanFaclPref1Cd		= x.m_SM_PLAN_FACL_PREF_1_CD;
	m_smPlanFaclPref2Cd		= x.m_SM_PLAN_FACL_PREF_2_CD;
	m_smPlanFaclPref3Cd		= x.m_SM_PLAN_FACL_PREF_3_CD;
	m_hmPlanMegaLuCd		= x.m_HM_PLAN_MEGA_LINEUP_CD;
	m_hmPlanHotChargeInd	= x.m_HM_PLAN_HOT_CHARGE_IND;
	m_smPlanCastSpec		= x.m_SM_PLAN_CAST_SPEC;

	// new caster 4,5 fields
	m_mordAxiomOrder		= x.m_MORD_AXIOM_ORDER;
	m_mordSmkCondWest		 = x.m_MORD_SMK_COND_WEST;
	m_mordSmkSpecWest		= x.m_MORD_SMK_SPEC_WEST;
	m_mordIhCastUnitName	= x.m_MORD_IH_CAST_UNIT_NAME;
	m_mordAxiomOrder.Trim();
	m_mordSmkCondWest.Trim();
	m_mordSmkSpecWest.Trim();
	m_mordIhCastUnitName.Trim();

	m_slApplyNewOrderCode	= x.m_SL_APPLY_NEW_ORDER_CODE.GetLength() < 1 
							  ? ' '
							  : x.m_SL_APPLY_NEW_ORDER_CODE[0];
	m_custCoilReq			= x.m_MORD_CUST_COIL_REQ;
	m_provCastSlPcWgt		= x.m_MORD_PROV_CAST_SL_PC_WGT;

//     Added if condition below to capture finished slab customer orders currently only
//     Dofasco. Customer in this ci code group require a decimal precision reflected in CSD 
//     and for shop use via english to metric length conversions. k. hubbard 11-17-04 maint.	
//	if ( m_ciCode == 86 )
	if ( m_mordHotrollUnitName == "80HR" )  // added this field for new identification of Slab Sales customer orders 1-24-07 k. hubbard
	   m_provCastSlLnth		= x.m_MORD_PROV_CAST_SL_LNTH;
	else
	   m_provCastSlLnth		= x.m_MORD_ORDERED_LENGTH;  // Slab Sale Customer


//	m_provCastSlLnth		= x.m_MORD_PROV_CAST_SL_LNTH;
	m_orderedWidth			= x.m_MORD_ORDERED_WIDTH;
	m_provHbYldFctr			= x.m_PROV_HB_YLD_FCTR;
	m_cmsYieldLossFactor	= x.m_MORD_CMS_YIELD_LOSS_FACTOR;
	m_custPieceWgt			= x.m_MORD_CUST_PIECE_WGT;
	m_provPiwWgt			= x.m_MORD_PROV_PIW_WGT;
	m_plan910NoSlOrd		= x.m_PLAN_910_NO_SL_ORD;
	m_smPlanSlabsToPlanInd  = x.m_SM_PLAN_SLABS_TO_PLAN_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_SLABS_TO_PLAN_IND[0];
	m_smPlanOn909Ind		= x.m_SM_PLAN_ON_909_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_ON_909_IND[0];
	m_smPlanOn910Ind		= x.m_SM_PLAN_ON_910_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_ON_910_IND[0];
	m_smPlanDemandDueInd	= x.m_SM_PLAN_DEMAND_DUE_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_DEMAND_DUE_IND[0];
	m_smPlanSlabsOn909		= x.m_SM_PLAN_SLABS_ON_909;
	m_slApply910PlanHbWgt	= x.m_SL_APPLY_910_PLAN_HB_WGT;
	m_slApply910PlanSlWgt	= x.m_SL_APPLY_910_PLAN_SL_WGT;
	m_slApply909PlanHbWgt	= x.m_SL_APPLY_909_PLAN_HB_WGT;
	m_plannedLatestStartDate	= x.m_SM_PLAN_FP_PST_DATE2;
	m_plannedLatestStartTime	= x.m_SM_PLAN_FP_PST_TIME2;
	m_provHbProvTol			= x.m_PROV_HB_PROV_TOL;

	m_provCastSlabWidthMax  = x.m_MORD_PROV_CAST_SL_WTH_MAX;   // added these 2 fields 11-11-03 k. hubbard
    m_provCastSlabWidthMin  = x.m_MORD_PROV_CAST_SL_WTH_MIN;   // conclude 2 fields here 11-11-03 k. hubbard

	m_mordCriticalCustInd   = x.m_MORD_CRITICAL_CUST_IND.GetLength() == 0  	// Char data types init this way. Added for critical customer identification from p-mill-order 12-1-05 k.hubbard
                              ? ' '
							  : x.m_MORD_CRITICAL_CUST_IND[0];

	// new for FP order segment
	m_fpOrderNum	= x.m_FP_M_ORDER_NO;
	m_fpMeltPstDate	= x.m_FP_MELT_PST_DATE;
	m_fpLpstDate	= x.m_FP_LPST_DATE;
	m_fpIshroPrty	= x.m_FP_ISHRO_PRTY;
	m_fpMtoFlowOrdInd = x.m_FP_MTO_FLOW_ORD_IND.GetLength() == 0
						? ' '
						: x.m_FP_MTO_FLOW_ORD_IND[0];
	m_fpPlanSlabWgt	  = 2000.0 * x.m_FP_PLAN_SLAB_WGT;
	m_fpPlanNumPieces = x.m_FP_PLAN_NO_PCS;
	// overwrite data in mill_order that is supplied by FP.
	m_origProvSlabsDue = m_provSlabsDue;
	m_provSlabsDue	  = m_fpPlanNumPieces;
	m_plannedLatestStartDate = m_fpMeltPstDate;
	m_commodityPriority = m_fpIshroPrty;

	ZeroChems();
	ComputeSpreadSqueeze();

	////////// DM CHANGE: 2002-09-14: missing chemistries
	// Don't really need this here, since loading order chems will catch there.
	//  Really need it for the CDalyPlanOrdersOnlySet version

	SetEltRange(CChem::ELT_C,
				m_minCarbon,
				m_maxCarbon,
				' ');
	SetEltRange(CChem::ELT_MN,
				m_minMn,
				m_maxMn,
				' ');

	CChem::Element elt;
	if ( CChem::ToElement(m_signifElSym,elt) )
		SetEltRange(elt,
					m_minSignifEl,
					m_maxSignifEl,
					' ');
}



//
//  Initialize from a database record (for an order on lineup, not applied).
//

// need a counter to init order segment part of order num
static int orderNumCounter = 0;

void COrder::Init(const CDalyPlanOrdersOnlySet& x)
{
  	m_millOrderNum			= x.m_MILL_ORDER_NO;
	m_custName				= x.m_MORD_CUSTOMER_NAME;
	m_custCode				= x.m_MORD_CUSTOMER_CODE;
	m_pdtCode				= x.m_MORD_PRODUCT_CODE;
	m_automotiveCode		= x.m_MORD_AUTOMOTIVE_CODE;
	m_hrYrWk.Set(x.m_MORD_HOT_ROLL_YEAR,
			   x.m_MORD_HOT_ROLL_WEEK);
	m_orderedSpec			= x.m_MORD_ORDERED_SPEC;
	m_minCarbon				= x.m_MORD_CARBON_MIN;
	m_maxCarbon				= x.m_MORD_CARBON_MAX;
	m_minMn					= x.m_MORD_MANGANESE_MIN;
	m_maxMn					= x.m_MORD_MANGANESE_MAX;
	m_minSignifEl			= x.m_MORD_SIGNIF_EL_MIN;
	m_maxSignifEl			= x.m_MORD_SIGNIF_EL_MAX;
	m_signifElSym			= x.m_MORD_SIGNIF_EL_SYM;
	m_ciCode				= x.m_MORD_PRODUCT_CMDTY_CD;
	m_pgmExposure			= x.m_MORD_PGM_EXPOSURE;
	m_hbWidth				= x.m_MORD_HOT_BAND_WIDTH;
	m_hbGauge				= x.m_MORD_HOT_BAND_GAUGE;
	m_orderedGauge			= x.m_MORD_ORDERED_GAUGE;
	m_cbWidth				= x.m_MORD_COLD_BAND_WIDTH;
	m_commodityPriority		= x.m_MORD_PLAN_PRTY_WK_DAY/10%100;  // left 2 digits, of 3
	m_orderPriority			= x.m_MORD_PLAN_PRTY_WK_DAY%10;      // rightmost digit
	m_slabCondnCode			= x.m_MORD_SLAB_CONDITION_CD;
	m_provSlabWidth			= x.m_PROV_SLAB_WIDTH;
	//m_provSlabThickness	= x.m_PROV_SLAB_THICKNESS;
	m_provSlabThickness		= 9.25;
//
	m_mordHotrollUnitName	= x.m_MORD_HOTROLL_UNIT_NAME;  	// added this field for new identification of Slab Sales customer orders 1-24-07 k. hubbard

    m_crheatSpec			= x.m_HEAT_SPEC;      		// added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
    m_crInclusionMod		= x.m_INCLUSION_MODIFIED;   // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	
//     Added if condition below to capture finished slab customer orders currently only
//     Dofasco. Customer in this ci code group require a decimal precision reflected in CSD 
//     and for shop use via english to metric length conversions. k. hubbard 11-17-04 maint.	
//	if ( m_ciCode == 86 )
	if ( m_mordHotrollUnitName == "80HR" )  // added this field for new identification of Slab Sales customer orders 1-24-07 k. hubbard
	   m_provSlabLength		= x.m_PROV_SLAB_LENGTH;
	else
	   m_provSlabLength		= x.m_MORD_ORDERED_LENGTH;  // Slab Sale Customer
	

//	m_provSlabLength		= x.m_PROV_SLAB_LENGTH;
	m_provSlabsDue			= x.m_PROV_SLABS_DUE;
	m_provSlabWgtDue		= x.m_PROV_SLAB_WGT_DUE;
	m_provSlabIndivSlabWgt	= x.m_PROV_INDV_SLAB_WGT;
	m_provMinSlabWgt		= x.m_MORD_PROV_MIN_SLAB_WGT;
	m_provMaxSlabWgt		= x.m_MORD_PROV_MAX_SLAB_WGT;
	m_provMinSlabLength		= x.m_MORD_MINIMUM_SLAB_LENGTH;
	m_provMaxSlabLength		= x.m_MORD_MAXIMUM_SLAB_LENGTH;
	m_smPlanSteelSpec		= x.m_SM_PLAN_STEEL_SPEC;
	m_smPlanSlabUnitCd		= x.m_SM_PLAN_SLAB_UNIT_CD;
	m_smPlanFaclPref1Cd		= x.m_SM_PLAN_FACL_PREF_1_CD;
	m_smPlanFaclPref2Cd		= x.m_SM_PLAN_FACL_PREF_2_CD;
	m_smPlanFaclPref3Cd		= x.m_SM_PLAN_FACL_PREF_3_CD;
	m_hmPlanMegaLuCd		= x.m_HM_PLAN_MEGA_LINEUP_CD;
	m_hmPlanHotChargeInd	= x.m_HM_PLAN_HOT_CHARGE_IND;
	m_smPlanCastSpec		= x.m_SM_PLAN_CAST_SPEC;
	m_slApplyNewOrderCode	= x.m_SL_APPLY_NEW_ORDER_CODE.GetLength() < 1 
							  ? ' '
							  : x.m_SL_APPLY_NEW_ORDER_CODE[0];
	m_custCoilReq			= x.m_MORD_CUST_COIL_REQ;
	m_provCastSlPcWgt		= x.m_MORD_PROV_CAST_SL_PC_WGT;

//     Added if condition below to capture finished slab customer orders currently only
//     Dofasco. Customer in this ci code group require a decimal precision reflected in CSD 
//     and for shop use via english to metric length conversions. k. hubbard 11-17-04 maint.	
//	if ( m_ciCode == 86 )
	if ( m_mordHotrollUnitName == "80HR" )  // added this field for new identification of Slab Sales customer orders 1-24-07 k. hubbard
	   m_provCastSlLnth		= x.m_MORD_PROV_CAST_SL_LNTH;
	else
	   m_provCastSlLnth		= x.m_MORD_ORDERED_LENGTH;  // Slab Sale Customer

//	m_provCastSlLnth		= x.m_MORD_PROV_CAST_SL_LNTH;
	m_orderedWidth			= x.m_MORD_ORDERED_WIDTH;
	m_provHbYldFctr			= x.m_PROV_HB_YLD_FCTR;
	m_cmsYieldLossFactor	= x.m_MORD_CMS_YIELD_LOSS_FACTOR;
	m_custPieceWgt			= x.m_MORD_CUST_PIECE_WGT;
	m_provPiwWgt			= x.m_MORD_PROV_PIW_WGT;
	m_plan910NoSlOrd		= x.m_PLAN_910_NO_SL_ORD;
	m_smPlanSlabsToPlanInd  = x.m_SM_PLAN_SLABS_TO_PLAN_IND.GetLength() == 0
							? ' '
							  : x.m_SM_PLAN_SLABS_TO_PLAN_IND[0];
	m_smPlanOn909Ind		= x.m_SM_PLAN_ON_909_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_ON_909_IND[0];
	m_smPlanOn910Ind		= x.m_SM_PLAN_ON_910_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_ON_910_IND[0];
	m_smPlanDemandDueInd	= x.m_SM_PLAN_DEMAND_DUE_IND.GetLength() == 0
							  ? ' '
							  : x.m_SM_PLAN_DEMAND_DUE_IND[0];
	m_smPlanSlabsOn909		= x.m_SM_PLAN_SLABS_ON_909;
	m_slApply910PlanHbWgt	= x.m_SL_APPLY_910_PLAN_HB_WGT;
	m_slApply910PlanSlWgt	= x.m_SL_APPLY_910_PLAN_SL_WGT;
	m_slApply909PlanHbWgt	= x.m_SL_APPLY_909_PLAN_HB_WGT;
	m_plannedLatestStartDate	= x.m_SM_PLAN_FP_PST_DATE2;
	m_plannedLatestStartTime	= x.m_SM_PLAN_FP_PST_TIME2;
	m_provHbProvTol			= x.m_PROV_HB_PROV_TOL;

	m_provCastSlabWidthMax  = x.m_MORD_PROV_CAST_SL_WTH_MAX;   // added these 2 fields 11-11-03 k. hubbard
    m_provCastSlabWidthMin  = x.m_MORD_PROV_CAST_SL_WTH_MIN;   // conclude 2 fields here 11-11-03 k. hubbard

	m_mordCriticalCustInd   = x.m_MORD_CRITICAL_CUST_IND.GetLength() == 0  	// Char data types init this way. Added for critical customer identification from p-mill-order 12-1-05 k.hubbard
                              ? ' '
							  : x.m_MORD_CRITICAL_CUST_IND[0];

		// new for FP order segment
	m_fpOrderNum.Format("%07d-CSD%03d",x.m_MILL_ORDER_NO,++orderNumCounter);
	m_fpMeltPstDate = m_plannedLatestStartDate;
	m_fpLpstDate	= 0;
	m_fpIshroPrty	= m_commodityPriority;
	m_fpMtoFlowOrdInd = ' ';
	m_fpPlanSlabWgt	  = m_provSlabIndivSlabWgt;
	m_fpPlanNumPieces = m_provSlabsDue;
	m_provSlabsDue	  = m_fpPlanNumPieces;
	m_origProvSlabsDue = m_provSlabsDue;


	ZeroChems();
	ComputeSpreadSqueeze();

////////// DM CHANGE: 2002-09-14: missing chemistries
	// Because these no longer get chemistries, 
	//    we need to set what we have in the array.

	SetEltRange(CChem::ELT_C,
				m_minCarbon,
				m_maxCarbon,
				' ');
	SetEltRange(CChem::ELT_MN,
				m_minMn,
				m_maxMn,
				' ');

	CChem::Element elt;
	if ( CChem::ToElement(m_signifElSym,elt) )
		SetEltRange(elt,
					m_minSignifEl,
					m_maxSignifEl,
					' ');
}




//
//  Compute the ordered tons for the order
//  We need to compute slab weight, rounded to tens.
//  Then multiply by the #slabs due, then convert to tons.
//

Weight COrder::OrderedTons()	const
{
	// The plus 5 is to get rounding in the tens place.
	Weight indivSlabWgt = m_provSlabWidth 
						  * m_provSlabLength 
						  * 9.25 
						  * CMiscConstants::SteelDensity() + 5;

	// complete rounding to tens place
	indivSlabWgt = long(indivSlabWgt * .1)*10;

	// compute total weight and convert to tons.

	Weight orderedSlabWgt = indivSlabWgt * m_provSlabsDue;

	return Weight(long(orderedSlabWgt / 2000));
}



//
//	Not used anymore.
//
//  Compute the box-anneal type from the product code and ci code
//  From the original DPA system.
//

CString COrder::GetBoxAnnType() const
{
	if (( 641 <= m_pdtCode && m_pdtCode <= 648 )
		||
		m_pdtCode == 690
		||
		m_pdtCode == 695 )
	
		return CString(" EG");

	else if (671 <= m_pdtCode && m_pdtCode <= 678 )

		return CString("ZNC");

	else if ( m_ciCode == 52 || m_ciCode == 56 )
		
		return CString("BOX");

	else

		return CString("   ");
}




//
//  Look up an order by order# in a sorted set of COrders.
//


// FP Change

//COrder* COrder::FindOrder(int orderNum,
//	 					  T_OrderVec& orders)
//{
//	COrder order;
//	order.m_millOrderNum = orderNum;
//
//	T_OrderVec::iterator io = lower_bound(orders.begin(),
//										orders.end(),
//										&order,
//										less_p<COrder*>());
//
//	// Standard kludge to ame sure target matches exactly.
//
//	if ( io != orders.end()
//		 &&
//		 ( ! ( order < **io ) ) )
//		return *io;
//	else 
//		return 0;
//}

//static
COrder* COrder::FindOrder(CString& fpOrderNum,
	 					  T_OrderVec& orders)
{
	COrder order;
	order.m_fpOrderNum = fpOrderNum;

	T_OrderVec::iterator io = lower_bound(orders.begin(),
										orders.end(),
										&order,
										less_p<COrder*>());

	// Standard kludge to make sure target matches exactly.

	if ( io != orders.end()
		 &&
		 ( ! ( order < **io ) ) )
		return *io;
	else 
		return 0;
}

// static
COrder* COrder::FindCSDOrder(long millOrderNum,
							 T_OrderVec& orders)
{
	COrder order;
	order.m_fpOrderNum.Format("%7d",millOrderNum);

	T_OrderVec::iterator io = lower_bound(orders.begin(),
										orders.end(),
										&order,
										less_p<COrder*>());

	COrder* pOrder = 0;

	for ( ; io != orders.end(); ++io ) {
		if ( (*io)->m_millOrderNum > millOrderNum )
			break;
		if ( (*io)->m_fpOrderNum.Mid(8,3) == "CSD" ) {
			pOrder = (*io);
			break;
		}
		if ( pOrder == 0 )
			pOrder = (*io);	// this will probably mess up scheduled quantities.
	}

#ifdef _DEBUG
	if ( pOrder == 0 )
		afxDump << "Can't find CSD order for " << millOrderNum << "\n";
#endif

	return pOrder;
}

// static
void COrder::FindOrdersFromMillOrderNum(long millOrderNum,
										T_OrderVec& orders,
										T_OrderVec& matchingOrders)
{
	CString orderNum;
	orderNum.Format("%07d",millOrderNum);

	FindOrdersFromMillOrderNum(orderNum,orders,matchingOrders);
}


//static
void COrder::FindOrdersFromMillOrderNum(CString& orderNum,
										T_OrderVec& orders,
										T_OrderVec& matchingOrders)
{
	COrder order;
	order.m_fpOrderNum = orderNum;

	T_OrderVec::iterator io = lower_bound(orders.begin(),
										orders.end(),
										&order,
										less_p<COrder*>());

	matchingOrders.clear();

	for ( ; io != orders.end(); ++io ) {
		int numChars = min(7,orderNum.GetLength());
		CString leftPart = (*io)->m_fpOrderNum.Left(numChars);
		if ( leftPart > orderNum )
			break;
		if ( leftPart == orderNum ) 
			matchingOrders.push_back( (*io) );
	}
}


// static
void COrder::IncrementParentNumSlabsDue(int incr)
{
	T_OrderVec matchingOrders;
	FindOrdersFromMillOrderNum(m_millOrderNum,TheSnapshot.Orders(),matchingOrders);

	for ( T_OrderVec::iterator io = matchingOrders.begin();
	      io != matchingOrders.end();
		  ++io ) 
		(*io)->m_origProvSlabsDue += incr;
}


//
// TODO:  Table-ize.
//

void COrder::ComputeSpreadSqueeze()
{
	Width downstreamSqueeze;

	if ( 12 <= m_ciCode && m_ciCode <= 22 ) 
		// INTek
		downstreamSqueeze = 1.4;
	else if ( m_ciCode == 65 || m_ciCode == 66 )
		downstreamSqueeze = 0;
	else {

		if ( m_hbGauge < 0.090 )
			downstreamSqueeze = 2.0;
		else if ( m_hbGauge < 0.140 )
			downstreamSqueeze = 3.0;
		else if ( m_hbGauge < 0.160 )
			downstreamSqueeze = 2.5;
		else
			downstreamSqueeze = 2.0;
	}

	Width transSpread80 = 0.0;

	Width transSqueeze80;
	Width nonTransSqueeze80;
	Width nonTransSpread80;

	// CICodes 65,66 have m_cbWidth == 0, so we use m_hbWidth for those instead.
	Width width = ( m_cbWidth == 0 ? m_hbWidth : m_cbWidth );

	Width adjustedCBWidth = width + downstreamSqueeze + 1.6;


	if ( m_provSlabLength < 212 ) {
		// short slab
		nonTransSqueeze80 = 0.5;
		nonTransSpread80  = 0.3;
		transSqueeze80	  = 0.5;
	}
	else if ( adjustedCBWidth >= 64.4 ) {
		// wide slab
		nonTransSqueeze80 = 0.6;
		nonTransSpread80  = 0.3;
		transSqueeze80	  = 0.6;
	}
	else {
		nonTransSqueeze80 = 1.6;
		nonTransSpread80  = 0.3;
		transSqueeze80	  = 0.9;
	}

//	m_transitionMinWidth	= m_hbWidth - transSpread80;    // ok per P. Fronczek 
	m_transitionMinWidth	= m_provCastSlabWidthMin; // - 0.7; // replaced with new providing min/max fields per P. Velasco, P. Fronczek. maint: 11-13-03 K. Hubbard


//	m_transitionMaxWidth	= width     + transSqueeze80 + downstreamSqueeze;  comment out and replaced with new providing min/max fields per P. Velasco, P. Fronczek 11-13-03 K. Hubbard
//	m_transitionMaxWidth	= m_provCastSlabWidthMax - 0.7; //  replaced with new providing min/max fields per P. Velasco, P. Fronczek. maint: 11-13-03 K. Hubbard
	m_transitionMaxWidth	= m_provCastSlabWidthMax - 1; //  replaced with new providing min/max fields per P. Velasco, P. Fronczek. maint: 1-18-06 K. Hubbard


//	m_nonTransMinWidth		= m_hbWidth - nonTransSpread80; comment out and replaced with new providing min/max fields per P. Velasco, P. Fronczek 11-13-03 K. Hubbard 
	m_nonTransMinWidth		= m_provCastSlabWidthMin; // replaced with new providing min/max fields per P. Velasco, P. Fronczek. maint: 11-13-03 K. Hubbard


//	m_nonTransMaxWidth		= width     + nonTransSqueeze80 + downstreamSqueeze; comment out and replaced with new providing min/max fields per P. Velasco, P. Fronczek 11-13-03 K. Hubbard
	m_nonTransMaxWidth		= m_provCastSlabWidthMax; // replaced with new providing min/max fields per P. Velasco, P. Fronczek. maint: 11-13-03 K. Hubbard


	m_transitionMinWidth	= max(m_transitionMinWidth,m_nonTransMinWidth); // Pass the greater of these 2. 
	m_transitionMaxWidth	= min(m_transitionMaxWidth,m_nonTransMaxWidth); // Pass the lesser of these 2. 

#if 0
	afxDump << "Order# " << m_millOrderNum 
			<< " T:[" << m_transitionMinWidth 
			<< ","    << m_transitionMaxWidth
			<< "], NT:[" << m_nonTransMinWidth
			<< ","	     << m_nonTransMaxWidth
			<< "]\n";
#endif
}




//
//  CMS = Cold machine scarf
//
//  CMS orders are detected by condnCode = 5XXX or 8XXX
//   and spec one of 726-XX
//					 520
//					 524
//					 811
//					 820
//					 826
//					 


bool COrder::IsCMS() const
{
	return IsCMS(SlabCondnCode(),CastSpec());
}


//static
bool COrder::IsCMS(int condn, const CString& spec)
{
	int condnDigit1 = condn/1000;	// 1st digit of condn

	return ( (condnDigit1 == 5 || condnDigit1 == 8) 
		     &&
			 IsCMSSpec(spec) );
}


bool COrder::IsCMSSpec(const CString& spec)
{
	CString spec3 = spec.Left(3);

	return  (spec3 == "726"
			 ||
			 spec3 == "520" 
			 ||
			 spec3 == "524" 
			 ||
			 spec3 == "811"
			 ||
			 spec3 == "820" 
			 ||
			 spec3 == "822"  // Added for Nancy Hake Honda trials 6-15-09 k. hubbard  
			 ||
			 spec3 == "826") ;
}




///////////////////////////////////////////////////////////////////
//
//  Output to table for string builder
//
///////////////////////////////////////////////////////////////////

//
//  When we start up the single string builder,
//    we pass it a table of selected orders.
//  This is used to write a record to that table for the given order.
//  The superScen is needed in order to compute slabs due
//

void COrder::WriteNewStringBuilderRecord(CSSBInput& rs,
									  CSuperScen* pSS,
									  int userId) const
{
	rs.AddNew();
	
	// FP CHANGE
	rs.m_FP_M_ORDER_NO				= m_fpOrderNum;
	
	rs.m_MILL_ORDER_NO				= m_millOrderNum;
	rs.m_MORD_HOT_ROLL_YEAR			= m_hrYrWk.Year();
	rs.m_MORD_HOT_ROLL_WEEK			= m_hrYrWk.Week();
	rs.m_MORD_PRODUCT_CODE			= m_pdtCode;
	rs.m_MORD_SLAB_CONDITION_CD		= m_slabCondnCode;
	rs.m_MORD_ORDERED_GAUGE			= m_orderedGauge;
	rs.m_MORD_ORDERED_WIDTH			= m_orderedWidth;
	//rs.m_MORD_ORDERED_LENGTH		= ;
	//rs.m_MORD_ORDERED_WEIGHT		= ;
	rs.m_MORD_ORDERED_SPEC			= m_orderedSpec;
	rs.m_MORD_SIXTH_DIGIT_OF_SPEC	= m_orderedSpec[5];
	rs.m_MORD_CARBON_MIN			= m_minCarbon;
	rs.m_MORD_CARBON_MAX			= m_maxCarbon;
	rs.m_MORD_MANGANESE_MIN			= m_minMn;
	rs.m_MORD_MANGANESE_MAX			= m_maxMn;
	rs.m_MORD_SIGNIF_EL_MIN			= m_minSignifEl;
	rs.m_MORD_SIGNIF_EL_MAX			= m_maxSignifEl;
	rs.m_MORD_SIGNIF_EL_SYM			= m_signifElSym;
	rs.m_MORD_PRODUCT_CMDTY_CD		= m_ciCode;
	rs.m_MORD_HOT_BAND_WIDTH		= m_hbWidth;
	//rs.m_MORD_HOT_BAND_WIDTH_MIN	= ;
	//rs.m_MORD_HOT_BAND_WIDTH_MAX	= ;
	rs.m_MORD_HOT_BAND_GAUGE		= m_hbGauge;
	//rs.m_MORD_HOT_BAND_GAUGE_MIN	= ;
	//rs.m_MORD_HOT_BAND_GAUGE_MAX	= ;
	rs.m_MORD_COLD_BAND_WIDTH		= m_cbWidth;
	rs.m_MORD_MINIMUM_SLAB_LENGTH	= m_provMinSlabLength;
	rs.m_MORD_MAXIMUM_SLAB_LENGTH	= m_provMaxSlabLength;
	rs.m_MORD_CMS_YIELD_LOSS_FACTOR	= m_cmsYieldLossFactor;
	rs.m_PROV_SLABS_DUE				= pSS->NumSlabsDue(this);
	rs.m_MORD_PLAN_PRTY_WK_DAY		= m_commodityPriority*10+m_orderPriority;
	//rs.m_REPROV_SLABS_DUE			= ;
	rs.m_PROV_SLAB_WIDTH			= m_provSlabWidth;
	rs.m_PROV_SLAB_THICKNESS		= m_provSlabThickness;
	rs.m_PROV_SLAB_LENGTH			= m_provSlabLength;
	rs.m_PROV_SLAB_WGT_DUE			= m_provSlabWgtDue;
	rs.m_PROV_INDV_SLAB_WGT			= m_provSlabIndivSlabWgt;
	rs.m_PROV_HB_PROV_TOL			= m_provHbProvTol;
	rs.m_SM_PLAN_STEEL_SPEC			= m_smPlanSteelSpec;
	rs.m_SM_PLAN_SLAB_UNIT_CD		= m_smPlanSlabUnitCd;
	rs.m_SM_PLAN_FACL_PREF_1_CD		= m_smPlanFaclPref1Cd;
	rs.m_SM_PLAN_FACL_PREF_2_CD		= m_smPlanFaclPref2Cd;
	rs.m_SM_PLAN_FACL_PREF_3_CD		= m_smPlanFaclPref3Cd;
	//rs.m_SM_PLAN_FACL_PREF_4_CD	= ;
	rs.m_SM_PLAN_CAST_SPEC			= m_smPlanCastSpec;
	rs.m_SM_PLAN_FP_PST_DATE		= m_plannedLatestStartDate;
	rs.m_SM_PLAN_FP_PST_TIME		= m_plannedLatestStartTime;
	rs.m_USER_ID = userId;

	rs.Update();
}

