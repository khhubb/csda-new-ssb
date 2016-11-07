// CSOrder.cpp: implementation of the CCSOrder class.
//
// Program: R0341060 (DPA) Update Periodic Elements (lots) in File
//
// Description: This prog controls the updating of a heat or lot that 
//              is a record or PE element on Stlmake_Daily_Plan file.
//
// Maintenance:  
//    Name          Date       Description
// -------------  --------   -----------------------------------------
// K. Hubbard     09/11/02   Added flip cond code 9xxx and disp code 9
//                           logic and cms/flip cond code 8xxx with disp
//                           code 5.   per P. Fronczek       
//
// K. Hubbard     01/15/03   Reversed flip disp code to 1 and restored 
//                           logic for cms/flip cond code 8xxx to disp
//                           code 7.   per P. Fronczek       
//
// K. Hubbard     11-11-03:  Added these 2 fields for reprovide limits 
//                           request m_MORD_PROV_CAST_SL_WTH_MAX 
//	                         m_MORD_PROV_CAST_SL_WTH_MIN. per P. Fronczek
//
// K. Hubbard     06/11/04   Changed flip cond code 9xxx and new disp code 2
//                           logic. per P. Fronczek
//
// K. Hubbard     09/20/04   Subtract 2 inches from Provided and Reprovide plan 
//                           (aim) length when less than 1.7 or equal to max length.
//                            per T. Fronczek
//
// K. Hubbard     10/11/04   Verify adequate (> 2") min/max length inch gap from Provided 
//                           and Reprovide plan(aim) length calculations before subtracting 2". 
//                            per P. Fronczek
//
// K. Hubbard     05/04/05   Changed flip disp code 2 to represent Slab Customer Sales
//                           logic. per P. Velasco
//
// K. Hubbard     06/29/05   Added noslit adjustment on xxx4 condition codes to receive a slit type of 'D'
//                           and disp code = 4 logic. per P. Fronczek
//
// K. Hubbard     09/08/05   Changed flip disposition code value from '2' to '1' on condition codes 9xxx.
//                           9-8-05 per P. Fronczek
//
// K. Hubbard     02/03/06	 Added millOrderNum parameter to ComputeDispCode in this pgm and Public definition 
//                           ComputeDispCode in CSOrder.h for stock order disp code assignment of '2' to route 
//                           slab material to #2Yard. per T. Fronczek  
//
// K. Hubbard     01-24-07   Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers and release of 
//	                         CI code = 86 identification. Used MFC Class Wizard's, Member Variables, Update Columns button linked to 
//                           OrdersForDailyPlanOnly VIEW fields in newinl via ODBC path. per T. Fronczek
//
// K. Hubbard     01/24/08   Removed (comm out) stock disp code 2 assignment for millOrderNum, caster scheduler can manually set stock to dispcode=2. 
//                           Stock default disp code returned to 4 around dock. per E. Jackson (Slab Logistics project) 
//
// K. Hubbard     05-20-08   Added these 2 fields via MFC Wizard for material direction limits request m_HEAT_SPEC and m_INCLUSION_MODIFIED 
//	                         Used MFC Class Wizard's, Update Columns and Bind All buttons linked to 
//                           OrdersForDailyPlanOnly fields in newinl via ODBC path. per J. Sarb-Beer
//
// K. Hubbard     05-5-09    Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                           modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                           controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//
//                  Note: Acceptable database input values must be part of combo box data list items....   
//
//
// K. Hubbard     08-27-14   Added 6 for new Mandatory Non-CMS condition code (6xxx) with aroung the dock '4' lot dispcode assignment per M. Keilman.  
//
//
//
//    ** Important Scenario Note ** : In order for 910 transmitted or saved data to be available immediately 
//                                    following validation or saves, field must be also present in appropriate
//                                    location in CasterScenArchiver.cpp !!! Also this will update the desired 
//                                    table in CsdaTest database.   
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "CSOrder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "DalyPlanSet.h"
#include "CasterStringOrdersSet.h"
#include "order.h"
#include "Snapshot.h"
#include "Spec.h"
#include "SpecMgr.h"
#include "MiscConstants.h"
#include "CastStringValidator.h"
#include "SSBOutput.h"
#include "CastString.h"
#include "CasterScen.h"

#include "CheckStockApplicationRules.h"   // added k. hubbard 4-4-08


////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////


// Each c-tor defers to a corresponding initialization routine.
// I have no idea why I did it this way.


CCSOrder::CCSOrder()
{
	Init();
}


CCSOrder::CCSOrder(const CDalyPlanSet& planSet,
				   int caster) //### caster-specific
{
	Init(planSet,caster); //### caster-specific
}



CCSOrder::CCSOrder(COrder* pOrder, 
				   int strandNum,
				   int caster) //### caster-specific
{
	Init(pOrder,strandNum,caster); //### caster-specific
}


CCSOrder::CCSOrder(const CCSOrder& x)
{
	Init(x);
}


CCSOrder::CCSOrder(const CCasterStringOrdersSet& ordersSet)
{
	Init(ordersSet);
}


CCSOrder::CCSOrder(const CCheckStockApplicationRules& x)  // added 4-4-08 k. hubbard
{
    Init(x);
}

CCSOrder::CCSOrder(const CSSBOutput& sbSet,
				   int caster) //### caster-specific
{
	Init(sbSet,caster); //### caster-specific
}




void CCSOrder::Init()
{
 	m_heatSeqNum	= 0;
	m_lotNum		= 0;

	m_strandNum		= 0;
	m_pCastString	= 0;

	m_millOrderNum	= 0;
	m_fpOrderNum    = "";
	m_pOrder		= 0;

	m_duration		= 0;
	m_relStartTime	= 0;
	m_relFinishTime	= 0;
	
	m_slabWidth		= 0.0;
	m_slabThickness	= 0.0;
	m_slabLength	= 0.0;
	m_pieceWgt		= 0.0;
	m_tons			= 0.0;
	m_numPieces		= 0.0;

	m_planSteelLengthMax	= 0.0;
	m_planSteelLengthMin	= 0.0;

	m_ciCode = 0;  // 1-28-03 k. hubbard
	
	m_slabCondnCode	= 0;
	m_exposureCode	= ' ';

	m_pLotSpec		= 0;

	m_changeFlag	= ' ';

	m_dispCode		   = 1;
	m_expandedDispCode = 0;
	m_slitTypeCode	   = ' ';

	m_isTransition	= false;
	m_isRadical		= false;

	m_outputCondnCode = m_slabCondnCode;
	m_outputDispCode  = m_dispCode;

	m_stockReason  = 'a';  // added reasons 5-5-09 k. hubbard
	m_stockCommercialReason  = 'a';  // added reasons 5-5-09 k. hubbard
	m_slitReason  = ' ';   // added reasons 2-26-10 k. hubbard

}


// Initializing from DPA file exisiting 910 fields during model startups k. hubbard 

void CCSOrder::Init(const CDalyPlanSet& planSet,    // DPA field data initialization only.
					int caster)  //### caster-specific
{
	//  database heatSeq# are 1-based, we are 0-based
 	m_heatSeqNum	= planSet.m_PLAN_HEAT_SEQUENCE_NO - 1;
	m_lotNum		= planSet.m_PLAN_LOT_NUMBER;

	m_strandNum		= (planSet.m_PLAN_CASTER_STRAND_NO == "1"
					   ? 1
					   : (planSet.m_PLAN_CASTER_STRAND_NO == "2"
					   ? 2
					   : 0));
	m_pCastString	= 0;


	m_millOrderNum	= planSet.m_MILL_ORDER_NO;
	m_pOrder		= 0;

	m_slabWidth		= planSet.m_PLAN_STEEL_WIDTH;
	m_slabThickness	= planSet.m_PLAN_STEEL_THICKNESS;
	m_slabLength	= planSet.m_PLAN_STEEL_LENGTH;
	m_slabCondnCode	= planSet.m_PLAN_SLAB_CONDITION_CD;
	m_tons			= planSet.m_PLAN_STEEL_WGT_TONS;
	m_numPieces		= planSet.m_PLAN_NUMBER_OF_PIECES;
	m_pieceWgt		= m_tons*2000.0/m_numPieces+0.5;

	m_planSteelLengthMax = planSet.m_PLAN_STEEL_LENGTH_MAX;
	m_planSteelLengthMin = planSet.m_PLAN_STEEL_LENGTH_MIN;

	
	m_lotSpec		= planSet.m_PLAN_LOT_SPEC;
	m_exposureCode	= (planSet.m_PLAN_EXPOSURE_CODE.IsEmpty()
						? ' '
						: planSet.m_PLAN_EXPOSURE_CODE[0]);
	
	m_changeFlag	= (planSet.m_PLAN_CHANGE_INDICATOR.IsEmpty()
					   ? ' '
					   : planSet.m_PLAN_CHANGE_INDICATOR[0]);

	m_dispCode		= planSet.m_PLAN_LOT_DISP_CODE;
	m_expandedDispCode = planSet.m_PLAN_EXPANDED_DISP_CODE;
	m_slitTypeCode	= (planSet.m_PLAN_LOT_TYPE_SLIT_CODE.IsEmpty()
						? ' '
						: planSet.m_PLAN_LOT_TYPE_SLIT_CODE[0]);

	//  Here is why we should rename m_pieceWgt to be m_slabWgt!!!!
	if ( m_slitTypeCode == 'E' )
		m_pieceWgt /= 2;

	m_isTransition = false;
	m_isRadical	 = false;

	m_outputCondnCode = m_slabCondnCode;
	m_outputDispCode  = m_dispCode;

//	m_slitReason  = (planSet.m_SLIT_REASON.IsEmpty()   // added reasons 5-5-09 k. hubbard
//						? ' '
//						: planSet.m_SLIT_REASON[0]);
	m_stockReason  = planSet.m_STOCK_REASON;        // added reasons 5-5-09 k. hubbard
	m_stockCommercialReason  = planSet.m_STOCK_COMMERCIAL_REASON;        // added reasons 5-5-09 k. hubbard
	m_slitReason  = planSet.m_SLIT_REASON;        // added reasons 2-26-10 k. hubbard

//	m_stockReason  = (planSet.m_STOCK_REASON.IsEmpty() // added reasons 5-5-09 k. hubbard
//						? ' '
//						: planSet.m_STOCK_REASON[0]);


	// we need to find the fpOrderNum.
	// This requires a search in the orders.
	// We use the millOrderNum to index into the orders, 
	//   looking for an applied order record.
	// This should be of the form  XXXXXXX-CSDXXXX
	// wish me luck.
	
	m_pOrder = COrder::FindCSDOrder(m_millOrderNum,TheSnapshot.Orders());
	if ( m_pOrder != 0 )
		m_fpOrderNum = m_pOrder->FpOrderNum();
	else if ( m_millOrderNum == 9999999 )
		m_fpOrderNum = "9999999";
	else
		m_fpOrderNum = "???????";


	SetLotSpecPtr(caster); //### caster-specific
}

// Initializing DPA 909/910 fields during click and drag of orders during
//   manual string building, with order data where available k. hubbard 

void CCSOrder::Init(COrder* pOrder, 
					int strandNum,
					int caster) //### caster-specific
{
	m_heatSeqNum	= 0;
	m_lotNum		= 0;

	m_strandNum		= strandNum;

	m_pCastString	= 0;

	m_pOrder		= pOrder;

	//FP change
	//m_millOrderNum	= m_pOrder->OrderNum();
	m_millOrderNum  = atol(m_pOrder->FpOrderNum().Left(7));
	m_fpOrderNum	= m_pOrder->FpOrderNum();

	m_absStartTime	= 0;
	m_absFinishTime	= 0;
	m_relStartTime	= 0;
	m_relFinishTime	= 0;
   	m_duration		= 0;

	m_slabWidth		= m_pOrder->SlabWidth();
	m_slabThickness	= m_pOrder->SlabThickness();
	m_slabLength	= m_pOrder->SlabLength();
	
	m_pieceWgt		= m_pOrder->IndivSlabWgt();
	m_numPieces		= m_pOrder->NumSlabsDue();
	m_tons			= m_pieceWgt*m_numPieces/2000;

	m_planSteelLengthMax = 0.0;
	m_planSteelLengthMin = 0.0;
	
	m_lotSpec		= m_pOrder->CastSpec();

	m_ciCode = m_pOrder->CICode();  // 1-28-03 k. hubbard

	// added these 2 fields 11-11-03 k. hubbard
	m_provCastSlabWidthMin = m_pOrder->ProvSlabWidthMin(); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
	m_provCastSlabWidthMax = m_pOrder->ProvSlabWidthMax(); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard

	m_hotrollUnitName = m_pOrder->HotrollUnitName(); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 01-24-07 k. hubbard

	m_heatSpec			= m_pOrder->HeatSpec();          // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
	m_inclusionModified	= m_pOrder->InclusionModified(); // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08

	m_slabCondnCode	= m_pOrder->SlabCondnCode();
	m_changeFlag	= ' ';
	m_slitTypeCode	= ' ';

	m_slabLength = m_pOrder->ProvCastSlLnth();

	if ( m_slabLength == 0 || m_pOrder->IsCMS() ) 
		CalculateLengths(caster, //### caster-specific
					   m_pOrder,
					   SlabWidth(),
					   m_planSteelLengthMin,
					   m_slabLength,
					   m_planSteelLengthMax);
	else {
		m_planSteelLengthMin = m_pOrder->MinSlabLength();
		m_planSteelLengthMax = m_pOrder->MaxSlabLength();
	}

	// Subtracting 2 inches from Provided (aim) lengths during string building. 10/11/04 k. hubbard   
	if	( m_planSteelLengthMax - m_planSteelLengthMin > 2 ) {

		if ( m_slabLength >= m_planSteelLengthMax  
			||
			m_planSteelLengthMax - m_slabLength <= 1.7 )
			// return Length(long(length - 2));
			{
				m_slabLength = (m_slabLength - 2);
			}
	}

#if 0
	// get rid of this
	if ( m_pOrder->IsCMS() )
		PreCMSPercentageAdjustment(caster); //### caster-specific
	else {
		m_planSteelLengthMin = m_pOrder->MinSlabLength();
		m_planSteelLengthMax = m_pOrder->MaxSlabLength();

		if ( m_pOrder->ProvCastSlLnth() == 0 )

			// Per request, truncate slab length
			
			// This original code is from DPA.
			// I have no idea why the +1 is there.
			
			m_slabLength = m_pOrder->IndivSlabWgt() 
							/ ( SlabWidth() 
								* SlabThickness() 
								* CMiscConstants::SteelDensity());

			//m_slabLength = m_pOrder->IndivSlabWgt() 
			//				/ ( SlabWidth() 
			//					 * SlabThickness() 
			//					 * CMiscConstants::SteelDensity() ) 
			//				+ 1;
		
		else
			m_slabLength = m_pOrder->ProvCastSlLnth();
	}
#endif
		
	m_exposureCode = ( m_pOrder->PgmExposure() == 1
						 ? 'E'
						 : 'U' );
 
//   Added new Slab Sale Customer identification substituting CI Code with Hotroll Unit name below 1-24-07 k. hubbard 
	m_dispCode = ComputeDispCode(m_slitTypeCode,m_slabCondnCode,m_ciCode,m_millOrderNum,m_hotrollUnitName,m_inclusionModified);  //Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
//	m_dispCode = ComputeDispCode(m_slitTypeCode,m_slabCondnCode,m_mordHotrollUnitName,m_millOrderNum);  //Added millOrderNum for stock disp code assignment k. hubbard 2-3-06

	m_expandedDispCode = 0;

	m_isTransition	= false;
	m_isRadical	 = false;

	m_outputCondnCode = m_slabCondnCode;
	m_outputDispCode  = m_dispCode;

	m_stockReason  = 'b';  // added reasons 5-5-09 k. hubbard
	m_stockCommercialReason  = 'b';  // added reasons 5-5-09 k. hubbard
	m_slitReason  = ' ';   // added reasons 2-26-10 k. hubbard

	CString tempSpec = m_lotSpec;
	if ( CCastStringValidator::FixHeatSpec(tempSpec,m_lotSpec,caster) ) //### caster-specific
		m_lotSpec = tempSpec;
	SetLotSpecPtr(caster); //### caster-specific
}



void CCSOrder::Init(const CCSOrder& x)
{
	m_heatSeqNum	= x.m_heatSeqNum;
	m_lotNum		= x.m_lotNum;

	m_strandNum		= x.m_strandNum;

	m_pCastString	= 0;

	m_millOrderNum	= x.m_millOrderNum;

	// FP change
	m_fpOrderNum    = x.m_fpOrderNum;

	m_pOrder		= x.m_pOrder;

	m_absStartTime	= x.m_absStartTime;
	m_absFinishTime	= x.m_absFinishTime;
	m_relStartTime	= x.m_relStartTime;
	m_relFinishTime	= x.m_relFinishTime;
	m_duration		= x.m_duration;

	m_slabWidth		= x.m_slabWidth;
	m_slabThickness	= x.m_slabThickness;
	m_slabLength	= x.m_slabLength;
	m_tons			= x.m_tons;
	m_numPieces		= x.m_numPieces;
	m_pieceWgt		= x.m_pieceWgt;

	m_planSteelLengthMin = x.m_planSteelLengthMin;
	m_planSteelLengthMax = x.m_planSteelLengthMax;

	m_lotSpec		= x.m_lotSpec;

	m_ciCode        = x.m_ciCode;

	m_provCastSlabWidthMin = x.m_provCastSlabWidthMin; // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
	m_provCastSlabWidthMax = x.m_provCastSlabWidthMax; // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard

	m_slabCondnCode	= x.m_slabCondnCode;
	m_exposureCode	= x.m_exposureCode;

	m_pLotSpec		= x.m_pLotSpec;

	m_changeFlag	= x.m_changeFlag;

	m_dispCode		   = x.m_dispCode;
	m_expandedDispCode = x.m_expandedDispCode;
	m_slitTypeCode	   = x.m_slitTypeCode;

	m_isTransition	= x.m_isTransition;
	m_isRadical		= x.m_isRadical;

	m_outputCondnCode = m_slabCondnCode;
	m_outputDispCode  = m_dispCode;

	m_inclusionModified  = x.m_inclusionModified; // added k. hubbard 5-20-08

	m_slitReason   = x.m_slitReason;   // added reasons 2-26-10 k. hubbard
	m_stockReason  = x.m_stockReason;  // added reasons 5-5-09 k. hubbard
	m_stockCommercialReason  = x.m_stockCommercialReason;  // added reasons 5-5-09 k. hubbard

}




void CCSOrder::Init(const CSSBOutput& sbSet,
					int caster) //### caster-specific
{
	
	// See if there is an order for the this millOrderNum
	// Create an order for this record and add to the string.

	// FP Change
	//COrder* pOrder = COrder::FindOrder(sbSet.m_MILL_ORDER_NO,
	//								   TheSnapshot.Orders());
	// FP Change
	CString ordNum = sbSet.m_FP_M_ORDER_NO;
	COrder* pOrder = COrder::FindOrder(ordNum,
									   TheSnapshot.Orders());
	m_strandNum		= (sbSet.m_PLAN_CASTER_STRAND_NO == "1"
					   ? 1
					   : (sbSet.m_PLAN_CASTER_STRAND_NO == "2"
					   ? 2
					   : 0));

	if ( pOrder == 0 ) {
		Init();
		m_millOrderNum	= sbSet.m_MILL_ORDER_NO;
		m_fpOrderNum	= sbSet.m_FP_M_ORDER_NO;
	}
	else{

		Init(pOrder,
			 m_strandNum,
			 caster); //### caster-specific
	}

	m_heatSeqNum	= sbSet.m_PLAN_HEAT_SEQUENCE_NO - 1;
	m_lotNum		= sbSet.m_PLAN_LOT_NUMBER;

	m_strandNum		= (sbSet.m_PLAN_CASTER_STRAND_NO == "1"
					   ? 1
					   : (sbSet.m_PLAN_CASTER_STRAND_NO == "2"
					   ? 2
					   : 0));
	m_pCastString	= 0;


	m_slabWidth		= sbSet.m_PLAN_STEEL_WIDTH;
	m_slabThickness	= sbSet.m_PLAN_STEEL_THICKNESS;
	m_slabLength	= sbSet.m_PLAN_STEEL_LENGTH;
	m_slabCondnCode	= sbSet.m_PLAN_EXPECTED_APPLIED_COND_CD;
	m_numPieces		= sbSet.m_PLAN_NUMBER_OF_PIECES;
	
	// we need to take the lot spec from order,
	// unless there is no order, in which case we recreate the heat spec
	if ( pOrder == 0 ) {	
		m_lotSpec.Format("%05d%c%d",
		            sbSet.m_PLAN_HEAT_SPEC,
					sbSet.m_PLAN_HEAT_SIXTH_DIGIT_SPEC[0],
					sbSet.m_PLAN_HEAT_SEVENTH_DIGIT_SPEC);
	}

	m_slitTypeCode	= (sbSet.m_PLAN_LOT_TYPE_SLIT_CODE.IsEmpty()
						? ' '
						: sbSet.m_PLAN_LOT_TYPE_SLIT_CODE[0]);

//   Added new Slab Sale Customer identification substituting CI Code with Hotroll Unit name below 1-24-07 k. hubbard 
	m_dispCode = ComputeDispCode(m_slitTypeCode,m_slabCondnCode,m_ciCode,m_millOrderNum,m_hotrollUnitName,m_inclusionModified);  //Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
	m_expandedDispCode = 0;

	// piece weight set by order.
	if ( m_slitTypeCode == 'E' )
		m_pieceWgt /= 2;

	m_tons			= m_pieceWgt*m_numPieces/2000;

	m_isTransition = false;
	m_isRadical	 = m_slitTypeCode == 'R';

	m_outputCondnCode = m_slabCondnCode;
	m_outputDispCode  = m_dispCode;

	SetLotSpecPtr(caster); //### caster-specific
}




void CCSOrder::Init(const CCasterStringOrdersSet& ordersSet)
{

	// See if there is an order for the this millOrderNum
	// Create an order for this record and add to the string.

	// FP change
	//COrder* pOrder = COrder::FindOrder(ordersSet.m_OrderNum,
	//		TheSnapshot.Orders());
	//COrder* pOrder = COrder::FindOrder(ordersSet.m_OrderNum,
	//								   TheSnapshot.Orders());
	CString ordNum = ordersSet.m_FpOrderNum;
	COrder* pOrder = COrder::FindOrder(ordNum,
									   TheSnapshot.Orders());

	if ( pOrder == 0 ) {
		Init();
		m_millOrderNum	= ordersSet.m_OrderNum;
		m_fpOrderNum    = ordersSet.m_FpOrderNum;
		m_strandNum		= ordersSet.m_StrandNum;
	}
	else{

		Init(pOrder,
			 ordersSet.m_StrandNum,
			 ordersSet.m_StringCaster);
	}

	HeatSeqNum		( ordersSet.m_HeatSeqNum	);

	SlabWidth		( ordersSet.m_SlabWidth		);
	SlabThickness	( ordersSet.m_SlabThickness	);
	PieceWgt		( ordersSet.m_PieceWgt		);

	NumPieces		( ordersSet.m_NumPieces		);

	SlabLength			( ordersSet.m_SlabLength			);
	PlanSteelLengthMin	( ordersSet.m_PlanSteelLengthMin	);
	PlanSteelLengthMax	( ordersSet.m_PlanSteelLengthMax	);

	DispCode		( ordersSet.m_DispCode		);
	SlabCondnCode	( ordersSet.m_CondnCode		);

	ExpandedDispCode(ordersSet.m_ExpandedDispCode);
	
	ChangeFlag		( ordersSet.m_ChangeFlag.GetLength() == 0 
				      ? ' ' 
					  : ordersSet.m_ChangeFlag[0]	);

	ExposureCode	( ordersSet.m_ExposureCode.GetLength() == 0
					  ? ' '
					  : ordersSet.m_ExposureCode[0]	);

	SlitTypeCode	( ordersSet.m_SlitTypeCode.GetLength() == 0
					  ? ' '
					  :	ordersSet.m_SlitTypeCode[0] );


	LotSpec			( ordersSet.m_LotSpec,
					  ordersSet.m_StringCaster	);

	
	SlitReason     (ordersSet.m_SlitReason.GetLength() == 0      // added reasons 5-5-09 k. hubbard
						? ' '
						: ordersSet.m_SlitReason[0]);

	StockReason    ( ordersSet.m_StockReason,        // added reasons 5-5-09 k. hubbard
					 ordersSet.m_StringCaster	);

	StockCommercialReason    ( ordersSet.m_StockCommercialReason,        // added reasons 5-5-09 k. hubbard
					 ordersSet.m_StringCaster	);

//	StockReason    ( ordersSet.m_StockReason   );     // added reasons 5-5-09 k. hubbard

//	StockReason    (ordersSet.m_StockReason.GetLength() == 0     // added reasons 5-5-09 k. hubbard
//						? 'hello'
//						: ordersSet.m_StockReason[0]);

}




// static
CCSOrder* CCSOrder::CreateStockOrder(const CString& lotSpec,
		                             Width slabWidth,
									 Length slabLength,
									 int condnCode,
									 int numPieces,
									 int strandNum,
									 int casterNum,  //### caster-specific
     								 CString InclusionModified,  // added material direction see caststring.h def k. hubbard 5-20-08
                                     CString stockReason,             // added 5-5-09 k. hubbard 
							         CString stockCommercialReason)   // added reason 5-5-09 k. hubbard declaration error

{
	CCSOrder* pOrder = new CCSOrder;


	pOrder->m_heatSeqNum	= 0;
	pOrder->m_lotNum		= 0;

	pOrder->m_strandNum		= strandNum;

	pOrder->m_pCastString	= 0;


	pOrder->m_millOrderNum	= 9999999;
	pOrder->m_fpOrderNum   = "9999999";


	pOrder->m_absStartTime	= 0;
	pOrder->m_absFinishTime	= 0;
	pOrder->m_relStartTime	= 0;
	pOrder->m_relFinishTime	= 0;
   	pOrder->m_duration		= 0;


	pOrder->m_slabWidth		= slabWidth;
	pOrder->m_slabThickness	= 9.25;
	pOrder->m_slabLength	= slabLength;

	if  (pOrder->m_hotrollUnitName = "80HR")  // added min/max length condition to change the 2 fields below 12-08-08 k. hubbard
		{
		  pOrder->m_planSteelLengthMax = min(384,slabLength + 2); // compensate for 80" HSM limits
		}
	else	{
			  pOrder->m_planSteelLengthMax = slabLength + 2;
		}
	pOrder->m_planSteelLengthMin = slabLength * .8;  // end min/max condition change 12-08-08 here k. hubbard

	pOrder->m_pieceWgt = pOrder->m_slabWidth
						 * pOrder->m_slabThickness
						 * pOrder->m_slabLength
						 * CMiscConstants::SteelDensity() ;
	
	pOrder->m_numPieces	= numPieces;
	pOrder->m_tons		= numPieces*pOrder->m_pieceWgt/2000.0;

	pOrder->LotSpec(lotSpec,casterNum); //### caster-specific

	pOrder->m_slabCondnCode	= condnCode;

	pOrder->m_changeFlag		= ' ';


	pOrder->m_slitTypeCode	= ' ';

	pOrder->m_exposureCode	= 'U';

    pOrder->m_inclusionModified = InclusionModified;  // Added for material direction of stock (planned unapplied) slabs k. hubbard 5-20-08 

	pOrder->m_dispCode = ComputeDispCode(pOrder->m_slitTypeCode,
									     pOrder->m_slabCondnCode,
                                         pOrder->m_ciCode,  
									     pOrder->m_millOrderNum,    // added for stock disp code req. k. hubbard 2-3-06 
										 pOrder->m_hotrollUnitName, // Added new Slab Sale Customer identification substituting CI Code with Hotroll Unit name below 1-24-07 k. hubbard
                                         pOrder->m_inclusionModified); // Added for material direction of stock (planned unapplied) slabs k. hubbard 5-20-08 

	pOrder->m_expandedDispCode = 0;

	pOrder->m_isTransition	= false;
	pOrder->m_isRadical	 = false;

	pOrder->m_outputCondnCode = pOrder->m_slabCondnCode;
	pOrder->m_outputDispCode  = pOrder->m_dispCode;

    pOrder->m_stockReason  =  stockReason;             // added 5-5-09 k. hubbard 
    pOrder->m_stockCommercialReason  =  stockCommercialReason;             // added 5-5-09 k. hubbard 

	return pOrder;
}



void CCSOrder::SetLotSpecPtr(int caster) //### caster-specific
{
	//m_pLotSpec	= TheSnapshot.SpecMgr().FindSpec(m_lotSpec,caster); //### caster-specific
	m_pLotSpec	= TheSnapshot.SpecMgr().FindSpecMaybe(m_lotSpec,caster); //### caster-specific

	if ( m_pLotSpec == 0 ) {
#ifdef _DEBUG
		afxDump << "Can't find spec " << m_lotSpec << "\n";
#endif
	}
}


void CCSOrder::LotSpec(const CString& newVal,int caster)  //### caster-specific
{	
	CString copySpec = m_lotSpec;
	if ( CCastStringValidator::FixHeatSpec(copySpec,newVal,caster) )  //### caster-specific
		m_lotSpec = copySpec;
	else
		m_lotSpec = newVal;

//	m_lotSpec = newVal;
	SetLotSpecPtr(caster); //### caster-specific
}




//////////////////////////////////////////////////////////////
//
//  Weight-related calculations
//
//
//////////////////////////////////////////////////////////////



bool CCSOrder::IsSplit() const
{
	return ( Order() != 0 
		&&                 // condition line added to exclude Slab Sales Customers from incorrect split flag assignment  
			m_pOrder->HotrollUnitName() == "80HR"  // on summary screens due to absence of hot band widths (HBW exist for 80" only) 7-24-07 k. hubbard     
		&& 
			SlabWidth() - Order()->HbWidth() > 20 );
}

bool CCSOrder::IsSlit()
{
	return ( Order() != 0 
			 && 
			 SlabWidth() > MaxAllowedSlabWidth() );
}


//static
bool CCSOrder::IsSplit(Width hbWidth, Width slabWidth)
{
//	if (HRUnitName == "80HR")

	return hbWidth <= slabWidth/2;   // similar condition line added above to exclude Slab Sales Customers from incorrect split flag assignment  
	//  original comm. out for 9-13-07 k. hubbard testing	return hbWidth <= slabWidth/2;   // similar condition line added above to exclude Slab Sales Customers from incorrect split flag assignment  
                           // may need to be added here for 'E' even slit calcs?? not sure yet, will leave alone so far no user complaints 7-24-07 k. hubbard    
}

// static
bool CCSOrder::IsSlit(Width slabWidth, Width maxSlabWidth)
{
	return slabWidth > maxSlabWidth;
}



Length CCSOrder::ComputeSlabLengthFromWidth(Width newWidth) const
{
	return ComputeSlabLengthFromWidth(Order(),m_pCastString->CasterScen()->Caster(), newWidth);
}


// static
Length CCSOrder::ComputeSlabLengthFromWidth(COrder* pOrder, int caster, Width newWidth)
{
	// This code modified from DPA Program #U0340050

	assert( pOrder != 0 );

	Length length = pOrder->IndivSlabWgt() 
					/ ( CMiscConstants::SteelDensity() 
					    * newWidth
						* pOrder->SlabThickness() );

 
    length 
		= min(max(length,CMiscConstants::MinCastableSlabLength(caster)),
		      CMiscConstants::Max80HsmRunoutLength());

	// return Length(long(length+0.5));
	// Per request, truncate slab length
	return Length( long(length) );

}


//static
Weight CCSOrder::ComputeSlabWeight(Width width, Length length)
{
	// I had 9.2 here for years, and no one said anything.
 	return Weight(long(length*width*9.25*CMiscConstants::SteelDensity()+0.5));
}



//
//  compute min/max lengths from basic parameters, after any adjustment for CMS
//
//  This is a basic length from width/wgt/thickness calculation, however,
//    we must make sure that we stay within the extremes of minimum castable
//    length and maximum size imposed by 80"HSM.
//


void CCSOrder::CalculateMinMaxLengthsAux(int caster,
										 Width width,
										 Thick thickness,
										 Weight adjustedMinSlabWgt,
										 Weight adjustedMaxSlabWgt,
										 Length& rMinL,
										 Length& rMaxL)
{
 	if ( width == 0 ) 
		return;

	rMinL = adjustedMinSlabWgt 
			/ CMiscConstants::SteelDensity()
			/ width 
			/ thickness;

	rMaxL = adjustedMaxSlabWgt 
			/ CMiscConstants::SteelDensity()
			/ width 
			/ thickness;

	if ( rMinL > rMaxL )
		rMinL = rMaxL * 0.90;

	rMinL = min(max(rMinL,
				   CMiscConstants::MinCastableSlabLength(caster)),
			   CMiscConstants::Max80HsmRunoutLength());

	rMaxL = min(max(rMaxL,
		CMiscConstants::MinCastableSlabLength(caster)),
			   CMiscConstants::Max80HsmRunoutLength());
}




// static
void CCSOrder::CalculateMinMaxLengths(COrder* pOrder,
									  int caster,
									  Width width,
									  Length& rMinLength,
									  Length& rMaxLength)
{
	assert( pOrder != 0 );

	Weight adjMinWgt;
	Weight adjMaxWgt;

	if ( pOrder->IsCMS() ) {
		adjMinWgt = pOrder->MinSlabWgt() * (1 + pOrder->CmsYieldLossFactor());
		adjMaxWgt = pOrder->MaxSlabWgt() * (1 + pOrder->CmsYieldLossFactor());
	}
	else {
		adjMinWgt = pOrder->MinSlabWgt();
		adjMaxWgt = pOrder->MaxSlabWgt();
	}

	CalculateMinMaxLengthsAux(caster,
							  width,
						      pOrder->SlabThickness(),
						      adjMinWgt,
						      adjMaxWgt,
						      rMinLength,
						      rMaxLength);
}



// static
void CCSOrder::CalculateLengths(int caster,  //### caster-specific
								COrder* pOrder,
								Width width,
								Length& rMinLength,
								Length& rLength,
								Length& rMaxLength)
{
	rLength = ComputeSlabLengthFromWidth(pOrder,caster,width);

	if ( pOrder->IsCMS() ) {

		// make the cms adjustment for length.
		rLength *= ( 1 + pOrder->CmsYieldLossFactor() );

		// if this adjustment makes the slab too long, use default values for all
		if (  rLength >= CMiscConstants::Max80HsmRunoutLength() ) {
                        //### caster-specific
			rLength    = CMiscConstants::CasterSlabLengthMax(caster);
			rMinLength = CMiscConstants::CasterSlabLengthMax(caster);
			rMaxLength = CMiscConstants::CasterSlabLengthMax(caster);
			return;
		}
	}

	CalculateMinMaxLengths(pOrder,caster,width,rMinLength,rMaxLength);

	// Subtracting 2 inches from reprovide plan (aim) lengths sent to casters. 10/11/04 k. hubbard   
	if	( rMaxLength - rMinLength > 2 ) {
		if ( rLength >= rMaxLength  
			||
			rMaxLength - rLength <= 1.7 )
			// return Length(long(length - 2));
			{
				rLength = (rLength - 2);
			}
	}

}





Width CCSOrder::MinAllowedSlabWidth()
{
	return ( m_pOrder == 0 
			 ? 20
			 : m_pOrder->MinAllowedSlabWidth(m_isTransition) );

}

Width CCSOrder::MaxAllowedSlabWidth()
{
	return ( m_pOrder == 0 
			 ? 80
			 : m_pOrder->MaxAllowedSlabWidth(m_isTransition) );
}


//  There are a number of parameters of a lot that are linked to the width.
//  If we just needed to compute these for a lot (CCSOrder) directly,
//    it wouldn't be so hard,
//  but sometimes we need to compute them semi-independently, 
//    such as when we are editing the parameters of a CCSOrder
//    but are not yet ready to make the change.  
//  Perhaps it would be easiest to create a copy of the CCSOrder,
//    but I'm concerned about its linkages to the rest of the world.
//
//  The following parms can change when the width changes:
//
//		dispCode
//		slitTypeCode
//		numPieces
//		minLength
//		aimLength
//		maxLength
//
//  In order to compute these values we need some other values,
//      most of which come from the order.
//
//	

// static
void CCSOrder::ComputeSlabWidthRelatedParms(int caster,  //### caster-specific
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
											Length& rMaxLength)

{
	int dig3 = condnCode/10%10; // get D of xxDx, third digit of ordered condition code  
	int dig4 = condnCode/1%10;  // get D of xxxD, fourth digit of ordered condition code
	long OrderNum = atol(pOrder->FpOrderNum().Left(7)); // Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
	CString MatlDirect = pOrder->InclusionModified();   // Added Material Direction for planned stock disp code assignment k. hubbard 5-20-08


//{ original bracket position before noslit check added k. hubbard 6-27=05

	if ( pOrder == 0 ) {
		rDispoCode		= dispoCode;
		rSlitTypeCode	= slitTypeCode;
		rNumPieces		= numPieces;
		rMinLength		= minLength;
		rAimLength		= aimLength;
		rMaxLength		= maxLength;
		return;
	}


	Width useWidth;

	if ( CCSOrder::IsSplit(pOrder->HbWidth(),width)
		&&
		( pOrder->HbWidth() >= 1.0)) {     // 9-14-07 k. hubbard testing condition that will block Slab Sale customers

		rSlitTypeCode = 'E';
		useWidth	  = width / 2;

		//useWidth	  = pOrder->SlabWidth();
	}
	else if ( CCSOrder::IsSlit(width,pOrder->MaxAllowedSlabWidth(isTransition)) ) {
		rSlitTypeCode = (slitTypeCode == 'R' ? 'R' : 'S');
		useWidth	  = pOrder->SlabWidth();
		
		// Added noslit adjustment here on xxx4 condition codes 6-27-05 k. hubbard  
		if ( dig4 == 4 && rSlitTypeCode == 'S' ) {
				
				rSlitTypeCode = 'D'; 
					// change to noslit type code of 'D'  6-27-05 k. hubbard
				}
	
	}
	else {
		rSlitTypeCode = (slitTypeCode == 'R' ? 'R' : ' ');
		useWidth	  = (slitTypeCode == 'R' ? pOrder->SlabWidth() : width);
	}

	// ** Reversed Added new Slab Sale Customer identification substituting CI Code with Hotroll Unit name below 1-24-07 k. hubbard 
	rDispoCode = ComputeDispCode(rSlitTypeCode,condnCode,CICode,OrderNum,HRUnitName,MatlDirect); //Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
//	rDispoCode = ComputeDispCode(rSlitTypeCode,condnCode,HRUnitName,OrderNum); //Added millOrderNum for stock disp code assignment k. hubbard 2-3-06

	if ( slitTypeCode == 'E' && rSlitTypeCode != 'E' )
		rNumPieces = numPieces * 2;
	else if ( slitTypeCode != 'E' && rSlitTypeCode == 'E' )
		rNumPieces = max(1,numPieces / 2);
	else
		rNumPieces = numPieces;

	CalculateLengths(caster,  //### caster-specific
					 pOrder,
					 useWidth,
					 rMinLength,
					 rAimLength,
					 rMaxLength);


#if 0
	// get rid of this
	if ( pOrder->IsCMS() )
		CalculateLengthsForCMS(caster,  //### caster-specific
							   pOrder,
							   useWidth,
							   rMinLength,
							   rAimLength,
							   rMaxLength);
	else
		CalculateLengthsForNonCMS(caster,  //### caster-specific
							      pOrder,
								  useWidth,
								  rMinLength,
								  rAimLength,
								  rMaxLength);
#endif // delete
}




CTimeSpan CCSOrder::CalculateSlabDuration(int caster,  //### caster-specific
										  int heatIndex,
										  int heatSize)
{
	if ( m_pLotSpec == 0 || m_numPieces == 0 )
		return CTimeSpan(0,0,10,0);

	//assert( m_pLotSpec != 0 );
	//assert( m_numPieces > 0 );

	bool isValid;

	return TheSnapshot.SpecMgr().SlabCastTime(m_pLotSpec->Name(),
											  caster,  //### caster-specific
											  m_strandNum,
											  heatIndex,
											  m_tons*2000/m_numPieces,
											  m_slabWidth,
											  m_slabThickness,
											  (MinCarbon()+MaxCarbon())/2,
											  (MinMn()+MaxMn())/2,
											  heatSize,
											  &isValid);

	// TODO: what is aimCarbon?
	// TODO: need to set backlink to cast string
}



void CCSOrder::ClearSlabHeatAssigns()
{
	m_slabHeatAssigns.erase(m_slabHeatAssigns.begin(),
						  m_slabHeatAssigns.end());
}


void CCSOrder::NoteSlabHeatAssign(int heatIndex)
{
	NoteSlabHeatAssigns(heatIndex,1);
}	



void CCSOrder::NoteSlabHeatAssigns(int heatIndex, int numPcs)
{
	if ( m_slabHeatAssigns.size() != 0 
		 &&
		 m_slabHeatAssigns.back().first == heatIndex ) 
		
		 m_slabHeatAssigns.back().second += numPcs;
	
	else {
	 
		pair<int,int> p(heatIndex,numPcs);

		m_slabHeatAssigns.push_back( p );
	}
}



bool CCSOrder::IsEquivalentLot( const CCSOrder* px, bool checkRadical ) const
{	
	// An equivalent lot must be on the same order
	//    have the same orderNum (Better check, just in case)
	// has the same:
	//	  slab width
	//    slab length
	//    condn code
	//	  spec
	
	return m_pOrder == px->m_pOrder
		   &&
//FP		   OrderNum() == px->OrderNum()
		   FpOrderNum() == px->FpOrderNum()
		   &&
		   SlabWidth() == px->SlabWidth()
		   &&
		   SlabLength() == px->SlabLength()
		   &&
		   SlabCondnCode() == px->SlabCondnCode()
		   &&
		   m_pLotSpec == px->m_pLotSpec
		   &&
		   m_lotSpec == px->m_lotSpec
		   &&
		   (! checkRadical || SlitTypeCode() != 'R')
		   && 
		   (! checkRadical || px->SlitTypeCode() != 'R');
}



void CCSOrder::AbsorbLot( CCSOrder* px )
{
	NumPieces( NumPieces() + px->NumPieces() );
	px->NumPieces(0);
	delete px;
}


CCSOrder* CCSOrder::SplitLot(int heatNum, int numPieces)
{
	// create a copy
	CCSOrder* pNew = new CCSOrder(*this);

	// indicate that we are on the same string,
	// and set the number of pieces and assigned heat.

	pNew->m_pCastString = m_pCastString;
	pNew->NumPieces( numPieces );
	pNew->m_heatSeqNum = heatNum;

	return pNew;
}



bool CCSOrder::GetChemRange(CChem::Element elt, 
							CChem::Bound& rMin,
							CChem::Bound& rMax,
							bool& rHasReasonCode) const
{
	rMin =   0.0;
	rMax = 100.0;

	if ( Order() == 0 ) {
		
		if ( PtrLotSpec() == 0 )
			return false;

		const CChemRange* range = PtrLotSpec()->GetChemRange(elt);

		if ( range == 0 )
			return false;

		rMin = range->MinVal();
		rMax = range->MaxVal();
		rHasReasonCode = false; // range->IsAppSpecific();

	}
	else {
		rMin = Order()->MinElement(elt);
		rMax = Order()->MaxElement(elt);
		rHasReasonCode = Order()->ElementReason(elt) != ' ';
	}

	return rMin != 0 || rMax != 100.0;
}
		



bool CCSOrder::IsSameLot( const CCSOrder* pOrder ) const
{
	return IsEquivalentLot( pOrder, false )
		   &&
		   NumPieces() == pOrder->NumPieces()
		   &&
		   SlitTypeCode() == pOrder->SlitTypeCode()
		   &&
		   DispCode() == pOrder->DispCode();

}

// static  
//  Added new Slab Sale Customer identification substituting CI Code with Hotroll Unit name below 1-24-07 k. hubbard 
int CCSOrder::ComputeDispCode(char slitTypeCode, int condnCode, int m_ciCode, long m_millOrderNum, CString m_mordHotrollUnitName, CString m_inclusionModified) // Assign Lot Level DispCodes. Added millOrderNum for stock disp code assignment k. hubbard 2-3-06

// Note: See CastStringValidator.cpp for heat level dispcode assignments  
{
	int dispCode;

	int dig1 = condnCode/1000%10;  // get D of Dxxx

	switch ( slitTypeCode ) {
		
	case 'E':
	case 'R': 
	case 'S':

		dispCode = 3;
		break;          // terminates inner most switch, do, for, or while statement

	// Added noslit adjustment here on xxx4 condition codes 6-27-05 k. hubbard  
	case 'D':

		dispCode = 4;
		break;          // terminates inner most switch, do, for, or while statement


	default:


		switch ( dig1 ) {

		case 4:
//			if (m_ciCode == 43 || m_ciCode ==44)   // 2-12-03 k.hubbard  7BF reline
//			Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
//			if (m_ciCode == 86 || m_millOrderNum == 9999999) // 5-4-05 k.hubbard  Slab Customer Sales and Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
///			if  ((m_mordHotrollUnitName != "80HR" && m_mordHotrollUnitName != " ")
///				|| 
///			 	 m_millOrderNum == 9999999) // 1-24-07 k.hubbard  Slab Sales Customer and Added millOrderNum for stock disp code assignment k. hubbard 2-3-06
////			if  ((m_mordHotrollUnitName != "80HR" && m_mordHotrollUnitName != " ")
////				 && 
////	    		   m_millOrderNum != 9999999) // Stock default disp code returned to 4 around dock. per E. Jackson (Slab Logistics project) maint. k. hubbard 1-24-08
			if  (m_mordHotrollUnitName != "80HR" && m_mordHotrollUnitName != " " && m_millOrderNum != 9999999)

				dispCode = 2;                // 5-4-05 k.hubbard  Slab Customer Sales
			else if //	 ||  
				(m_millOrderNum == 9999999 && m_inclusionModified == "x") // Added for material direction of stock (planned unapplied) slabs k. hubbard 5-20-08
			   
				dispCode = 2;                // 5-4-05 k.hubbard  Slab Customer Sales 
			else                                  
				dispCode = 4;
			break;

		case 5:

			dispCode = 7;
			break;

		case 6:

			dispCode = 4; // added 6 for Mandatory Non-CMS with aroung the dock 4 lot dispcode assignment K. Hubbard 8-27-14 per M. Keilman 
			break;

		case 8:

			dispCode = 7; // changed to 7 for cms / flip lot dispcode assignment K. Hubbard 1-15-03 per P. Fronzcek 
			break;

			//			dispCode = 5; // added 2 lines for cms / flip lot dispcode assignment K. Hubbard 9-10-02 per P. Fronzcek 

 
//    Restored 3 lines for flip lot dispcode assignment K. Hubbard 4-27-05 per P. Fronczek for use with 9XXX Condition Codes. 
  		case 9:          // added 3 lines for flip lot dispcode assignment K. Hubbard 6-11-04 per P. Fronzcek 

			dispCode = 1;   // changed flip disp code value from '2' to '1'. K. Hubbard 9-8-05 per P. Fronczek
			break;

		default:
	
			dispCode = 1;
			break;
		}
		break;
	}

	return dispCode;
}



#if 0
// this was modeled directly on the old DPA code.
// I am rewriting these calculations so I can understand them.
// Eventually, get rid of this code.



//////////////////////////////////////////////////////////////
//
//  Weight-related calculations
//
//
//////////////////////////////////////////////////////////////

//	Called when creating a CCSOrder from a COrder.
//	Called only when the order is CMS.
//  Obviously, pOrder is not 0 at this point.
//
//  The equivalent to the original from DPA is this:
//
//
//	if ( pOrder->ProvCastSlLnth() >= 365 ) {
//		SlabLength        ( GLOBAL_CASTER_SLAB_LENGTH_MAX );
//		PlanSteelLengthMin( GLOBAL_CASTER_SLAB_LENGTH_MAX );
//		PlanSteelLengthMax( GLOBAL_CASTER_SLAB_LENGTH_MAX );
//	}
//	else if ( pOrder->ProvCastSlLnth() > 0 ) {
//		SlabLength( pOrder->ProvCastSlLnth() * ( 1 + pOrder->CmsYieldLossFactor() ) );
//		CMSRecalcNewMinMaxSteelLength();
//	}
//	else {
//		Length len = pOrder->IndivSlabWgt() / (pOrder->SlabWidth() * pOrder->SlabThickness() * STEEL_DENSITY ) + 1;
//		len *= ( 1 + pOrder->CmsYieldLossFactor() );
//		SlabLength( len );
//	}
//
//
//  However, the 365 hardcoded here is 95% * 384 
//	   where 384 is the max run-out length at the 80".  
//	This calculation is based on a fixed CMS percentage increase.
//  However, there is no longer such a fixed percentage.
//  So, I have re-arranged the code.
//  I'm also puzzled as to why, when the order has ProvCastSlLngth of 0
//    (does this ever happen?), we do not reset the min/max lengths.
//  It does happen during validation (I think), but I need it now.
//  I also need to parameterize on the caster in order to cmpute the caster max.
// 
//  It is difficult to roll this method and CalculateLengthsForCMS into one:
//     For the PreCMSPercentageAdjustment, we take the given length (from the order)
//        unless it is zero.
//  In the other, we want to force a new length calculation.

void CCSOrder::PreCMSPercentageAdjustment(int caster)  //### caster-specific
{
	assert( m_pOrder != 0 );

	Length len = m_pOrder->ProvCastSlLnth();

	if ( len == 0 )
		len = m_pOrder->IndivSlabWgt() 
		      / 
			  (m_pOrder->SlabWidth() 
			   * m_pOrder->SlabThickness() 
			   * CMiscConstants::SteelDensity() ) 
			  + 1;

	Length aimL;
	Length minL;
	Length maxL;

	Length adjustedLen = len * ( 1 + m_pOrder->CmsYieldLossFactor() );

	CalculateLengthsForCMS(caster,m_pOrder,m_pOrder->SlabWidth(),len,minL,aimL,maxL);  //### caster-specific

	// Per request, truncate slab length

	SlabLength        ( long(aimL) );
	PlanSteelLengthMin( minL );
	PlanSteelLengthMax( maxL );
}



// static
void CCSOrder::CalculateLengthsForCMS(int caster,  //### caster-specific
									  COrder* pOrder, 
									  Width width,
									  Length length,
									  Length& rMinLength,
									  Length& rLength,
									  Length& rMaxLength)
{
	assert( pOrder != 0 );

	Length adjustedLength = length * ( 1 + pOrder->CmsYieldLossFactor() );

	
	if (  adjustedLength >= CMiscConstants::Max80HsmRunoutLength() ) {
                //### caster-specific
		rLength    = CMiscConstants::CasterSlabLengthMax(caster);
		rMinLength = CMiscConstants::CasterSlabLengthMax(caster);
		rMaxLength = CMiscConstants::CasterSlabLengthMax(caster);

	}
	else {

		rLength = adjustedLength;
		CalculateMinMaxLengthsForCMS(pOrder,width,rMinLength,rMaxLength);

	}
}


//  static
void CCSOrder::CalculateLengthsForCMS(int caster, //### caster-specific
									  COrder* pOrder,
									  Width width,
									  Length& rMinLength,
									  Length& rLength,
									  Length& rMaxLength)
{
	Length length = ComputeSlabLengthFromWidth(pOrder,width);
	CalculateLengthsForCMS(caster,pOrder,width,length,rMinLength,rLength,rMaxLength); //### caster-specific
}





//  static
void CCSOrder::CalculateLengthsForNonCMS(int caster,   //### caster-specific
									     COrder* pOrder,
										 Width width,
										 Length& rMinLength,
										 Length& rLength,
										 Length& rMaxLength)
{
	rLength = ComputeSlabLengthFromWidth(pOrder,width);
	CalculateMinMaxLengthsForNonCMS(pOrder,width,rMinLength,rMaxLength);
}


// static
void CCSOrder::CalculateMinMaxLengthsForCMS(COrder* pOrder,
											Width width,
											Length& rMinLength,
											Length& rMaxLength)
{
	assert( pOrder != 0 );

	U0340050Parms parms;

	parms.m_provCastSlWidth		= width;
	parms.m_provCastSlThick		= pOrder->SlabThickness();
	parms.m_provCastMinSlabWgt	= pOrder->MinSlabWgt() * (1 + pOrder->CmsYieldLossFactor());
	parms.m_provCastMaxSlabWgt	= pOrder->MaxSlabWgt() * (1 + pOrder->CmsYieldLossFactor());

	U0340050(parms,rMinLength,rMaxLength);
}

// static
void CCSOrder::CalculateMinMaxLengthsForNonCMS(COrder* pOrder,
											Width width,
											Length& rMinLength,
											Length& rMaxLength)
{
	assert( pOrder != 0 );

	U0340050Parms parms;

	parms.m_provCastSlWidth		= width;
	parms.m_provCastSlThick		= pOrder->SlabThickness();
	parms.m_provCastMinSlabWgt	= pOrder->MinSlabWgt();
	parms.m_provCastMaxSlabWgt	= pOrder->MaxSlabWgt();

	U0340050(parms,rMinLength,rMaxLength);
}




// static 
void CCSOrder::U0340050( CCSOrder::U0340050Parms& parms, 
						Length& minL, 
						Length& maxL )
{

}






#endif // dpa length calcs -- old


void CCSOrder::Init(const CCheckStockApplicationRules& x)
{
	m_If_Spec = x.m_HEAT_SPEC;
}

