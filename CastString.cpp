// CastString.cpp: implementation of the CCastString class.
//
// Description: This prog controls the updating of a heat or lot that 
//              is a record or PE element on Stlmake_Daily_Plan file and
//              will create CSDA_DAILY_PLAN load used for lineup handoff to IBM.
//
// Maintenance:  
//    Name          Date       Description
// -------------  --------   -----------------------------------------
//
// K. Hubbard     05-20-08:  Added these 2 fields via MFC Wizard for material direction limits request m_HEAT_SPEC and m_INCLUSION_MODIFIED 
//	                         Used MFC Class Wizard's, Update Columns and Bind All buttons linked to 
//                           OrdersForDailyPlanOnly fields in newinl via ODBC path. per J. Sarb-Beer
//
// K. Hubbard     05-5-09    Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                           modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                           controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "CastString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "DalyPlanSet.h"
#include "SSBOutput.h"


#include "Order.h"
#include "CSOrder.h"
#include "MiscConstants.h"
#include "Snapshot.h"
#include "CasterScen.h"
#include "CSOrderParmDlg.h"
#include "MultipleOrderInsertDlg.h"
#include "DailyPlanUploadSet.h"
#include "DailyPlanLineupsUploadSet.h"
#include "UploadMgr.h"
#include "RulesCheckLineupSet.h"
#include "DBConn.h"


#include <math.h>	// fabs

 

/////////////////////////////////////////////////////////////
//
// CCastString
//
/////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//	
//	Accessors
//	
////////////////////////////////////////////////////////////////


Weight CCastString::MinHeatSize() const
{
	return m_props.MinHeatSize();
}

Weight CCastString::AimHeatSize() const
{
	return m_props.AimHeatSize();
}

Weight CCastString::MaxHeatSize() const
{
	return m_props.MaxHeatSize();
}



							
Weight CCastString::SchedTons() const
{
	return SchedTons(1)+SchedTons(2);
}


Weight CCastString::SchedTons(int strandNum) const
{
	Weight weight = 0.0;

	for ( T_CSOrderVec::const_iterator ic = m_strand[strandNum].begin();
		  ic != m_strand[strandNum].end();
		  ++ic ) 

		weight += (*ic)->Tons();
		  

	return weight;
}



long CCastString::TotalDuration() const
{
	if ( m_heats.size() == 0 ) 
		return 0;
	else
		return m_heats.back().m_relFinishTime - m_heats.front().m_relStartTime;
}


long CCastString::OverallRelFinishTime() const
{
	if ( m_heats.size() == 0 )
		return 0;
	else
		return m_heats.back().m_relFinishTime;
}



////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////


CCastString::CCastString(CCasterScen* pCScen,
						 const CCastStringId& rId,
					 	 const CCastStringMiscProps& rProps)

 :	m_pCScen				( pCScen					),
	m_pOrigString			( 0							),
	m_id					( rId						),
	m_props					( rProps					),
	m_orderSelection		( pCScen->Caster()			),
	m_canEdit				( true						),
	m_heatLockStatus		( HEATS_NONE_LOCKED			),
	m_numLockedHeats		( 0							),
	m_status				( STRING_STATUS_NEW			),
	m_bIsValid				( true						)
{ 
}



CCastString::CCastString(CCasterScen* pCScen,
						 const CCastString& x,
						 CCastString* pOrigString)

 :	m_pCScen				( pCScen					),
	m_pOrigString			( pOrigString				),
	m_id					( x.m_id					),
	m_props					( x.m_props					),
	m_orderSelection		( x.m_orderSelection		),
	m_canEdit				( x.m_canEdit				),
	m_heatLockStatus		( x.m_heatLockStatus		),
	m_numLockedHeats		( x.m_numLockedHeats		),
	m_status				( x.m_status				),
	m_bIsValid				( true						)

{
	m_orderSelection.SetCaster(pCScen->Caster());
	
	// The only time we are likely to use this is when copying a scenario,
	//    in which case copying the id is okay.

	for ( int strandNum = 1; strandNum <= 2; ++strandNum ) {

		for ( T_CSOrderVec::const_iterator io = x.m_strand[strandNum].begin();
			  io != x.m_strand[strandNum].end();
			  ++io ) {

			CCSOrder* pOrder = new CCSOrder((**io));

			AddCSOrder(pOrder);
		}
	}

	// copy the heats, in case this is frozen.

	m_heats = x.m_heats; // copy

	CalculateHeats();
	CalculateSummary();
}


CCastString::CCastString(CCasterScen* pCScen, 
			const CCastStringId& rId,
			const CCastStringMiscProps& rProp, 
	        const CCastString& x)

 :	m_pCScen				( pCScen					),
	m_pOrigString			( 0							),
	m_id					( rId						),
	m_props					( rProp						),
	m_orderSelection		( x.m_orderSelection		),
	m_canEdit				( x.m_canEdit				),
	m_heatLockStatus		( x.m_heatLockStatus		),
	m_numLockedHeats		( x.m_numLockedHeats		),
	m_status				( x.m_status				),
	m_bIsValid				( true						)

{
	m_orderSelection.SetCaster(pCScen->Caster());
	
	// The only time we are likely to use this is when copying a scenario,
	//    in which case copying the id is okay.

	for ( int strandNum = 1; strandNum <= 2; ++strandNum ) {

		for ( T_CSOrderVec::const_iterator io = x.m_strand[strandNum].begin();
			  io != x.m_strand[strandNum].end();
			  ++io ) {

			CCSOrder* pOrder = new CCSOrder((**io));

			AddCSOrder(pOrder);
		}
	}

	// copy the heats, in case this is frozen.

	m_heats = x.m_heats; // copy

	CalculateHeats();
	CalculateSummary();
}



CCastString::~CCastString()
{
	for ( int i = 0; i < 3; ++i ) {

		vector<CCSOrder*>& orders = m_strand[i];
		for ( vector<CCSOrder*>::iterator io = orders.begin();
		      io != orders.end();
			  ++io)
			UnregisterCSOrder((*io));

		release(m_strand[i].begin(),
				m_strand[i].end());
	}
}




CCastString::CCastString(CCasterScen* pCScen,
						 CDalyPlanSet& planSet)
 : m_pCScen				( pCScen					),
   m_pOrigString		( 0							),
   m_orderSelection		( pCScen->Caster()			),
   m_props				( 1,pCScen->Caster()		),
   m_canEdit			( true						),
   m_heatLockStatus		( HEATS_NONE_LOCKED			),
   m_numLockedHeats		( 0							),
   m_status				( STRING_STATUS_909			),
	m_bIsValid				( true						)

{
	Init(planSet);
}



CCastString::CCastString(CCasterScen* pCScen,
						 CSSBOutput& sbSet)
 : m_pCScen				( pCScen					),
   m_pOrigString		( 0							),
   m_orderSelection		( pCScen->Caster()			),
   m_props				( 1,pCScen->Caster()		),
   m_canEdit			( true						),
   m_heatLockStatus		( HEATS_ALL_LOCKED			),
   m_numLockedHeats		( 0							),
   m_status				( STRING_STATUS_909			),
	m_bIsValid				( true						)

{
	Init(sbSet);
} 


void CCastString::Init(CDalyPlanSet& planSet)
{
	{
		CCastStringId firstId(planSet);

		m_id = firstId;	// copy
	}


	//if ( planSet.m_PLAN_HEAT_STATUS_CODE <= "4" ) {
	//	freezeHeats = false;
	//	status = STATUS_909;
	//}

	//props.SetComment(planSet.m_PLAN_LINEUP_COMMENTS);

	int lastHeatNum = -2;

	while ( ! planSet.IsEOF() ) {

		//  Retrieve cast string id of current record,
		//		determine if we need to move to the next string.

		CCastStringId nextId(planSet);

		if ( nextId != m_id )
			break;
		
		if ( planSet.m_PLAN_HEAT_SEQUENCE_NO - 1 != lastHeatNum ) {

			// record information on the next heat.
			lastHeatNum = planSet.m_PLAN_HEAT_SEQUENCE_NO - 1;
			m_heats.resize(max(m_heats.size(),lastHeatNum+1));

			m_heats[lastHeatNum].Init(m_id.m_caster,
								      planSet);
		}

		//  Create an order for this record.
		CCSOrder* pCSOrder = new CCSOrder(planSet,m_id.m_caster);
		AddCSOrder(pCSOrder);

		// adjust #pieces
		if (  pCSOrder->Order() != 0 
			  &&
			  m_heats[pCSOrder->HeatSeqNum()].Status() 
			         != CCastStringHeat::HEAT_STATUS_909) {

			int currSlabsDue = pCSOrder->Order()->NumSlabsDue();
			int increment = pCSOrder->NumPieces();
			if ( pCSOrder->SlitTypeCode() == 'E' )
				// even slit
				increment *= 2;

			pCSOrder->Order()->IncrementNumSlabsDue( increment );
		}
	
		planSet.MoveNext();
	}


	// Adjust status depending on status of heats
	SetStatus();

	// Additionally, make sure canEdit is initialized properly 
	//   (SetStatus only affects the status and maybe the freezeHeats mode).

	if ( m_status == STRING_STATUS_910 )
		m_canEdit = false;
}



void CCastString::Init(CSSBOutput& sbSet)
{
	{
		CCastStringId firstId(sbSet);

		m_id = firstId;	// copy
	}

	int lastHeatNum = -2;

	while ( ! sbSet.IsEOF() ) {

		//  Retrieve cast string id of current record,
		//		determine if we need to move to the next string.
		//  Of course, all records should be from the same string,
		//		this code is duplicated from above.

		CCastStringId nextId(sbSet);

		if ( nextId != m_id )
			break;
		
		if ( sbSet.m_PLAN_HEAT_SEQUENCE_NO - 1 != lastHeatNum ) {

			// record information on the next heat.
			lastHeatNum = sbSet.m_PLAN_HEAT_SEQUENCE_NO - 1;
			m_heats.resize(max(m_heats.size(),lastHeatNum+1));

			m_heats[lastHeatNum].Init(m_id.m_caster,
								      sbSet);
		}

		//  Create an order for this record.
		CCSOrder* pCSOrder = new CCSOrder(sbSet,m_id.m_caster);
		AddCSOrder(pCSOrder);

		// adjust #pieces
		// We shouldn't need this adjustment
//		if (  pCSOrder->Order() != 0 
//			  &&
//			  m_heats[pCSOrder->HeatSeqNum()].Status() 
//			         != CCastStringHeat::HEAT_STATUS_909) {
//
//			int currSlabsDue = pCSOrder->Order()->NumSlabsDue();
//			int increment = pCSOrder->NumPieces();
//			if ( pCSOrder->SlitTypeCode() == 'E' )
//				// even slit
//				increment *= 2;
//
//			pCSOrder->Order()->IncrementNumSlabsDue( increment );
//		}
	
		sbSet.MoveNext();
	}


	// Adjust status depending on status of heats
	// this should do nothing
	SetStatus();

	// Additionally, make sure canEdit is initialized properly 
	//   (SetStatus only affects the status and maybe the freezeHeats mode).

	if ( m_status == STRING_STATUS_910 )
		m_canEdit = false;
}


////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////



//  Update the string status based on the status of heats
//    if all heats are new then STRING_STATUS_NEW
//    if some 909, no 910, then STRING_STATUS_909
//    if some 910          then STRING_STATUS_910
//
//  The rather peculiar control strategy is in case I ever add 
//     more status codes here.


void CCastString::SetStatus()
{
	m_status = STRING_STATUS_NEW;

	for ( T_HeatVec::iterator ih = m_heats.begin();
		  ih != m_heats.end();
		  ++ih ) {

		if ( (*ih).Status() == CCastStringHeat::HEAT_STATUS_909
			 &&
			 m_status != STRING_STATUS_910 )
			m_status = STRING_STATUS_909;
		else if ( (*ih).Status() == CCastStringHeat::HEAT_STATUS_910 )
			m_status = STRING_STATUS_910;
	}

	if ( m_status != STRING_STATUS_NEW ) 
		m_heatLockStatus = HEATS_ALL_LOCKED;
	
}




// Adds a CSOrder to the end.
// The strand number had better be set already.


void CCastString::AddCSOrder(CCSOrder* pCSOrder)
{
	assert( m_pCScen != 0 );

	if ( pCSOrder->Order() == 0 ) 
		// FP change
		pCSOrder->m_pOrder = m_pCScen->FindOrder(pCSOrder->FpOrderNum());

	RegisterCSOrder(pCSOrder);
		
	if (pCSOrder->StrandNum() != 1
		&&
		pCSOrder->StrandNum() != 2)
	{
		ostrstream ostr;
		ostr.fill('0');
		// FP change
		ostr << "Cast string order # "  
			<< setw(15) << pCSOrder->FpOrderNum() 
			<< ": Error in strand#, found '"
			<< pCSOrder->StrandNum()
			<< "', setting to 1."
			<< ends;
		AfxMessageBox(ostr.str());
		ostr.freeze(false);
		
		pCSOrder->m_strandNum = 1;
	}
												
	m_strand[pCSOrder->StrandNum()].push_back(pCSOrder);
	pCSOrder->m_pCastString = this;
}


//  we need to determine the insertion point in the strand
//  and the heat number to assign.
//  we have four cases
//
//                  orderIndex
//                  -1 | >=0
//               /-----------\
//               |     |     |
//            -1 |  A  |  B  |
//               |     |     |
// heatIndex     +-----+-----+
//               |     |     |
//           >=0 |  C  |  D  |
//               |     |     |
//               \-----------/
//
//	Case A:  Insert at end.
//
//               heatSeqNum = heatSeqNum of last order
//                            if no orders, then 0
//
//               orderIter = strand end
//
//	Case B:  Insert before order, in same heat
//
//				 heatSeqNum = heatSeqNum of indicated order
//               orderIter = strandbegin + orderIndex;
//
//  Case C:  Insert at end of heat heatIndex
//
//                heatSeqNum = heatIndex
//                orderIter  = indicated heat's strandEnd
//
//	Case D:  Can't happen
//


void CCastString::ComputeHeatSeqNumAndInsertIndex(int destnStrand,
												  int heatIndex,
												  int orderIndex,
												  int& heatSeqNum,
												  vector<CCSOrder*>::iterator& insertIter)
{
	// Case D can't happen.
	assert( heatIndex == -1 || orderIndex == -1 );

	vector<CCSOrder*>&   toStrand = m_strand[destnStrand];

	if ( heatIndex == -1 && orderIndex == -1 ) {

		// Case A
		heatSeqNum = (toStrand.size() == 0 
					  ? 0 
					  : toStrand.back()->HeatSeqNum() );
		insertIter = toStrand.end();
	}
	else if ( heatIndex == -1 ) {
		
		// Case B
		insertIter = toStrand.begin() + orderIndex;
		heatSeqNum = (*insertIter)->HeatSeqNum();
	}
	else {

		// Case C (case D excluded already)
		heatSeqNum = heatIndex;
		insertIter = m_heats[heatIndex].StrandEnd(destnStrand);
	}
}



void CCastString::MoveOrders(const vector<int>& selectedItemIndexes, 
							   int sourceStrand,
							   int destnStrand,
							   int heatIndex,
							   int orderIndex)
{
	if ( selectedItemIndexes.size() == 0 )
		return;

	vector<CCSOrder*>&   toStrand = m_strand[destnStrand];
	vector<CCSOrder*>& fromStrand = m_strand[sourceStrand];
	
	int heatSeqNum;
	vector<CCSOrder*>::iterator insertIter;

	ComputeHeatSeqNumAndInsertIndex(destnStrand,
									heatIndex,
									orderIndex,
									heatSeqNum,
									insertIter);

	for ( vector<int>::const_iterator ii = selectedItemIndexes.begin();
		  ii != selectedItemIndexes.end();
		  ++ii ) {

		fromStrand[(*ii)]->m_strandNum = destnStrand;
		fromStrand[(*ii)]->m_heatSeqNum = heatSeqNum;
	}



	TransferVectorItems(toStrand,
						insertIter,
						fromStrand,
						selectedItemIndexes);

	CalculateHeats();
	CalculateSummary();
}





void CCastString::InsertOrders(const vector<COrder*>& selectedItems, 
							   CSuperScen* pSS,
							   int strandNum, 
							   int heatIndex,
							   int orderIndex,
							   bool query /* = false */ )	
{
	if ( selectedItems.size() == 0 )
		return;

	vector<CCSOrder*> csOrders;

	for ( vector<COrder*>::const_iterator item = selectedItems.begin();
		  item != selectedItems.end();
		  ++item ) {

		CCSOrder* pCSOrder = new CCSOrder((*item),
										  strandNum,
										  m_id.m_caster);
		pCSOrder->NumPieces(pSS->NumSlabsDue((*item)));

		pCSOrder->m_pCastString = this;
		csOrders.push_back(pCSOrder);
	}


	if ( query ) {

		CMultipleOrderInsertDlg dlg;
		dlg.m_strandNum = strandNum;
		dlg.m_casterNum = Id().Caster();
		dlg.m_pSuperScen = pSS;
		dlg.m_orders = csOrders;	// copy

		if ( dlg.DoModal() != IDOK )
			return;
			
		csOrders = dlg.m_orders;	// copy
		dlg.m_orders.clear();	// don't want dlg deleting these orders for me.
	}


	int heatSeqNum;
	vector<CCSOrder*>::iterator insertIter;

	ComputeHeatSeqNumAndInsertIndex(strandNum,
									heatIndex,
									orderIndex,
									heatSeqNum,
									insertIter);
		  
	for ( vector<CCSOrder*>::iterator io = csOrders.begin();
	      io != csOrders.end();
		  ++io) {
		(*io)->m_pCastString = this;
		RegisterCSOrder((*io));
		(*io)->m_heatSeqNum = heatSeqNum;
	}

	vector<CCSOrder*>& insStrand = m_strand[strandNum];

	insStrand.insert( insertIter,
					  csOrders.begin(),
					  csOrders.end() );

	CalculateHeats();
	CalculateSummary();
}


void CCastString::DeleteOrders(const vector<int>& selectedItemIndexes, 
							   int strandNum,
							   vector<COrder*>& deletedOrders)
{
	if ( selectedItemIndexes.size() == 0 )
		return;

	vector<CCSOrder*>& delStrand = m_strand[strandNum];

	for ( vector<int>::const_iterator ii = selectedItemIndexes.begin();
		  ii != selectedItemIndexes.end();
		  ++ii ) {

		CCSOrder* pDelOrder = delStrand[(*ii)];

		if ( pDelOrder->Order() != 0 )
			deletedOrders.push_back(pDelOrder->Order());

		UnregisterCSOrder(pDelOrder);
		delete pDelOrder;
	}

	DeletePositionsFromVector(delStrand,selectedItemIndexes);

	CalculateHeats();
	CalculateSummary();
}





CCSOrder* CCastString::InsertStockOrder(CSuperScen* pSuperScen,
										int strandNum,
										int heatIndex,
										int orderIndex)
{
	// Get data from the user.

	//  We are about to add a stock order.
	//  Find the order just after the insertion point,
	//    or the last order if we are inserting at the end
	//    (or possibly there are no orders in the strand).

	const vector<CCSOrder*>& orders = Strand(strandNum);


	CCSOrder* pCSOrder = 0;

	if ( orders.size() > 0 ) {

		if ( heatIndex != -1 ) {
			// insert at end of heat
			// if this is the last heat, use the last order
			// else use the first order on the next heat.

			if ( heatIndex == Heats().size() - 1 ) {
				vector<CCSOrder*>::const_iterator io 
					= Heats()[heatIndex].StrandEnd(strandNum);
				if ( io != orders.begin() )
					pCSOrder = (*(io-1)); 
			}
			else {
				vector<CCSOrder*>::const_iterator io 
					= Heats()[heatIndex+1].StrandBegin(strandNum);
				if ( io != orders.end() )
					pCSOrder = (*io); 
				else
					pCSOrder = orders.back();	// empty heats
			}
		}
		else if ( orderIndex == -1)
			// insert at end
			pCSOrder = orders.back();
		else if ( orderIndex == 0 )
			// insert at beginning
			pCSOrder = orders.front();
		else 
			// insert somewhere
			pCSOrder = orders[orderIndex];
	}


	CCSOrder* pStockOrder = 0;

	CCSOrderParmDlg dlg;
	dlg.m_pCSOrder			= pCSOrder;
	dlg.m_pOrder			= 0;
	dlg.m_pSuperScen		= pSuperScen;
	dlg.m_casterNum			= Id().Caster();
	dlg.m_bCreate			= true;
	dlg.m_bStock			= true;
	dlg.m_precedingWidth	= ( pCSOrder == 0 ? 0 : pCSOrder->SlabWidth() );

	if ( dlg.DoModal() == IDOK ) {

		pStockOrder = InsertStockOrder(dlg.m_spec,
									   dlg.m_castWidth,
									   dlg.m_castLength,
									   dlg.m_condnCode,
									   dlg.m_numPieces,
									   pSuperScen,
									   strandNum,
									   heatIndex,
									   orderIndex,
									   dlg.m_pOrder->InclusionModified(), // added material direction function (no reference & call) call k. hubbard 5-20-08
									   dlg.m_stockReason,   // added reasons 5-5-09 k. hubbard
									   dlg.m_stockCommercialReason);  // added reasons 5-5-09 k. hubbard

	}

	return pStockOrder;
}




CCSOrder* CCastString::InsertStockOrder(CString& spec,
					 				    Width slabWidth,
									    Length slabLength,
									    int condnCode,
									    int numPieces,
									    CSuperScen* pSS,
									    int strandNum,
									    int heatIndex,
									    int orderIndex,
										CString InclusionModified,  // added material direction see caststring.h def k. hubbard 5-20-08
										CString stockReason,  // added reasons 5-5-09 k. hubbard
									    CString stockCommercialReason)  // added reasons 5-5-09 k. hubbard
									

{
	CCSOrder* pCSOrder = CCSOrder::CreateStockOrder(spec,
												    slabWidth,
													slabLength,
													condnCode,
													numPieces,
													strandNum,
													m_id.Caster(),
													InclusionModified,   // added material direction see caststring.h def k. hubbard 5-20-08
													stockReason,         // added 5-5-09 k. hubbard 
									                stockCommercialReason);  // added reasons 5-5-09 k. hubbard

	pCSOrder->m_pCastString = this;

	int heatSeqNum;
	vector<CCSOrder*>::iterator insertIter;

	ComputeHeatSeqNumAndInsertIndex(strandNum,
									heatIndex,
									orderIndex,
									heatSeqNum,
									insertIter);


	RegisterCSOrder(pCSOrder);
	pCSOrder->m_heatSeqNum = heatSeqNum;

		  
	vector<CCSOrder*>& insStrand = m_strand[strandNum];

	insStrand.insert( insertIter,
					  pCSOrder);


	OrderSelection().UpdateGroupsFromOrderChange(pSS);
	CalculateHeats();
	CalculateSummary();

	return pCSOrder;
}



void CCastString::ModifyId(const CCastStringId& newId)
{
	m_id = newId;	// copy

}


void CCastString::ModifyMiscProps(const CCastStringMiscProps& newProps)
{
	m_props = newProps;	// copy

	CalculateHeats();
	CalculateSummary();
}



bool CCastString::ModifyCSOrderParms(CSuperScen* pSuperScen,
									 int strandNum,
								     int index)
{
	CCSOrder* pCSOrder = Strand(strandNum)[index];
// get this far
	CCSOrderParmDlg dlg;

	dlg.m_pSuperScen		= pSuperScen;
	dlg.m_pCSOrder			= pCSOrder;
	dlg.m_casterNum			= Id().Caster();
	dlg.m_bCreate			= false;
	dlg.m_precedingWidth	= (index == 0 ? 0 : Strand(strandNum)[index-1]->SlabWidth());

	if ( dlg.DoModal() == IDOK ) {

		bool reregister = ( dlg.m_numPieces != pCSOrder->NumPieces() 
							||
							dlg.m_slitTypeCode != pCSOrder->SlitTypeCode() );


		if ( reregister )
			CasterScen()->UnregisterCSOrder(pCSOrder);

		bool change = dlg.XferToCSOrder(pCSOrder);

		if ( reregister )
			CasterScen()->RegisterCSOrder(pCSOrder);

		// (2) the group entry for this order may have changed
		// make sure this item will appear, if it can
		OrderSelection().AddSpecialSelectedOrder(pCSOrder->Order());
		OrderSelection().UpdateGroupsFromOrderChange(pSuperScen);


		if ( change ) {
			CasterScen()->SetModified(true);

			// Warning:: This operation can delete pCSOrder
			// That's why we moved the AddSpecial... call above this point
			CalculateHeats();
			CalculateSummary();
			return true;
		}
	}

	return false;
}



bool CCastString::EditMultipleOrders(int heatNum,
									 int strandNum,
									 T_CSOrderVec::iterator ioBegin,
									 T_CSOrderVec::iterator ioEnd,
									 CSuperScen* pSS)
{

	CMultipleOrderInsertDlg dlg;

	dlg.m_strandNum = strandNum;
	dlg.m_casterNum = Id().Caster();
	dlg.m_pSuperScen = pSS;

	// Make a copy of the orders

	{
		for ( T_CSOrderVec::iterator io = ioBegin;
			  io != ioEnd;
			  ++io ) 
			dlg.m_orders.push_back( new CCSOrder( *(*io) ) );
	}


	if ( dlg.DoModal() != IDOK ) {
		release( dlg.m_orders.begin(), dlg.m_orders.end() );
		return false;
	}
		
	T_CSOrderVec newOrders = dlg.m_orders;	// copy
	dlg.m_orders.clear();	// don't want dlg deleting these orders for me.


	{
		for ( T_CSOrderVec::iterator io = ioBegin;
			  io != ioEnd;
			  ++io ) 

		  UnregisterCSOrder( (*io) );
	}

	release(ioBegin,ioEnd);
	
	T_CSOrderVec& orders = Strand(strandNum);

	bool atBegin = ioBegin == orders.begin();

	T_CSOrderVec::iterator iInsert;
	if ( ! atBegin ) iInsert = ioBegin - 1;

	orders.erase(ioBegin,ioEnd);

	if ( atBegin )
		iInsert = orders.begin();
	else
		++iInsert;

		  
	for ( vector<CCSOrder*>::iterator io = newOrders.begin();
	      io != newOrders.end();
		  ++io) {
		(*io)->m_pCastString = this;
		RegisterCSOrder((*io));
		(*io)->m_heatSeqNum = heatNum;
	}

	orders.insert( iInsert,
					  newOrders.begin(),
					  newOrders.end() );

	CalculateHeats();
	CalculateSummary();

	return true;

}



////////////////////////////////////////////////////////////////
//	
//	Registering CSOrders
//	
////////////////////////////////////////////////////////////////

void CCastString::RegisterCSOrder(CCSOrder* pCSOrder)
{
	m_pCScen->RegisterCSOrder(pCSOrder);
}

void CCastString::UnregisterCSOrder(CCSOrder* pCSOrder)
{
	m_pCScen->UnregisterCSOrder(pCSOrder);
}


////////////////////////////////////////////////////////////////
//	
//	Calculating heats
//	
////////////////////////////////////////////////////////////////



CTimeSpan CCastString::ComputeStdDuration(int heatNum, Weight heatSize)
{
	static CString zeros("0000000");

	CSpec* pSpec = Props().InitSpecPtr();
	if ( pSpec == 0 )
		return 3600;

	Width startWidth = Props().StartWidth(1) + Props().StartWidth(2);
	Width endWidth   = Props().EndWidth(1) + Props().EndWidth(2);
	
	int numHeats = max(1,max(heatNum+1,Props().NumHeats()));

	Width width = startWidth + (endWidth-startWidth)*((heatNum+0.5)/numHeats);

	CChem::Bound aimC  =  ( Props().MinCarbon() + Props().MaxCarbon() )  / 2;
	CChem::Bound aimMn =  ( Props().MinMn() + Props().MaxMn() ) / 2;

	bool isValid;
	double dur = TheSnapshot.SpecMgr().SlabCastTime(pSpec->Name(),
													Id().Caster(),
													1,
													heatNum,
													heatSize*2000,
													width,
													9.25,
													aimC,
													aimMn,
													heatSize,
													&isValid);
	return dur;
}




//  For 909 and 910, we inherit whatever heat assignments came in.
//  These are frozen and do not change, unless the sequencing of orders
//		and the sequencing of heats does not match. 
//  Different calculations are performed depending on whether heats assigns are
//		frozen or not.
//  If frozen, we just accept the assignments and calculate durations of the lots.
//  If not frozen, we must determine where the heat boundaries lie.
//  
//  We have added the capability to freeze the first few heats in the string
//     and allow calculated heat boundaries after that point.
//  A flag and an index determine where the cut-off point lies:
//		m_heatLockStatus == HEATS_ALL_LOCKED ==> all heats are frozen
//		m_heatLockStatus == HEATS_NONE_LOCKED ==> no heats are frozen
//		m_heatLockStatus == HEATS_SOME_LOCKED ==> m_numLockedHeats indicates the cut-off
//								heats [0,m_numLockedHeats) are locked
//								heats [m_numLockedHeats,numHeats) are unlocked



void CCastString::CalculateHeats()
{

#if 0
	afxDump << "CalculateHeats: " 
			<< m_id.Year() << " "
			<< m_id.Week() << " "
			<< m_id.Caster() << " "
			<< m_id.StringId() << "\n";
	afxDump << "Initial heats size: " << m_heats.size() << "\n";
#endif

	
	// We occasionally get some bogus heat sequence numbers on orders coming from DPA.
	// This will fix them, as best we can.
	FixHeatSequencing();

	//  Get rid of some old bookkeeping
	ClearSlabHeatAssigns();

	//  Set iterators to indicate the end of freezing on each strand
	//  (these would also be the beginning of unfrozen orders).
	//  Also determine the index of the last frozen heat.

	vector<CCSOrder*>::iterator freezeEnd1;
	vector<CCSOrder*>::iterator freezeEnd2;

	// Note: numFrozenHeats == min(numHeats,m_numLockedHeats) in case HEATS_SOME_LOCKED
	//                      == 0 if HEATS_NONE_LOCKED
	//						== numHeats if HEATS_ALL_LOCKED

	int numFrozenHeats = ComputeFreezeEndpoints(freezeEnd1,freezeEnd2);


	// Now do the calculations of the frozen heats.
	// freezeEnd1&2 indicate the end of iteration on the two strands

	CalculateHeatsFrozen(numFrozenHeats,freezeEnd1,freezeEnd2);

	
	// Now do the calculations of the frozen heats
	// FreezeEnd1&2 indicate the beginning of iteration on the two strands

	int numUnfrozenHeats = CalculateHeatsUnfrozen(numFrozenHeats,freezeEnd1,freezeEnd2);


	// This really only applies to the unfrozen heats,
	// but it no-ops on the frozen heats, so we apply it to all lots
	//  in order to keep the code simple.
	// In the unfrozen section, orders may be split across heat boundaries.
	// We have to create duplicate orders.

	//SplitHeatBoundaryLots();
	
	ComputeDummyHeats(numUnfrozenHeats);


#if 0
	{
		afxDump << "Heats: \n" ;
		for ( HeatVec::iterator ih = m_heats.begin();
			  ih != m_heats.end();
			  ++ih )
			afxDump  << "# "			<< ih-m_heats.begin()
					 << " : dur = "		<< (*ih).m_duration
					 << ", start = "	<< (*ih).m_relStartTime
					 << ", finish = "	<< (*ih).m_relFinishTime
					 << ", tons = "		<< (*ih).m_heatTons
					 << ", tph = "		<< ((*ih).m_duration.GetTotalSeconds() == 0 
											? 0
											: (*ih).m_heatTons*3600/(*ih).m_duration.GetTotalSeconds())
					 << "\n";
	}
#endif

	// Finish up with some bookkeeping.
	InitializeHeatOrderPointers();
}



//  Make sure we have consistent heatSeqNums across all the orders
//    in a strand.
//  In particular, the heatSeqNums should be monotonically non-decreasing.

void CCastString::FixHeatSequencing()
{
	FixHeatSequencing(1);
	FixHeatSequencing(2);
}


void CCastString::FixHeatSequencing(int strandNum)
{
	if ( m_strand[strandNum].empty() )
		return;

	int lastHeatNum = m_strand[strandNum][0]->m_heatSeqNum;

	for ( T_CSOrderVec::iterator io = m_strand[strandNum].begin();
		  io != m_strand[strandNum].end();
		  ++io ) {

		if ( (*io)->m_heatSeqNum < lastHeatNum )
			(*io)->m_heatSeqNum = lastHeatNum;

		lastHeatNum = (*io)->m_heatSeqNum;
	}
}



//  We get rid of some bookkeeping on the lots.

void CCastString::ClearSlabHeatAssigns()
{
	ClearSlabHeatAssigns(1);
	ClearSlabHeatAssigns(2);
}

void CCastString::ClearSlabHeatAssigns(int strandNum)
{
	for ( T_CSOrderVec::iterator ic = m_strand[strandNum].begin();
		  ic != m_strand[strandNum].end();
		  ++ic )
		(*ic)->ClearSlabHeatAssigns();
}




//  We need to compute the endpoint of iteration on each strand,
//  before which heats are frozen.
//  We also compute the index of the last frozen heat.

int CCastString::ComputeFreezeEndpoints(vector<CCSOrder*>::iterator& rFreezeEnd1,
										 vector<CCSOrder*>::iterator& rFreezeEnd2)
{

	if ( m_heatLockStatus == HEATS_ALL_LOCKED ) {
		//  All heats are frozen.
		//  So all orders are frozen.
		//  The number of frozen heats is the last heatSeqNum mentioned plus 1.
		rFreezeEnd1 = StrandEnd(1);
		rFreezeEnd2 = StrandEnd(2);
		int index = max(MaxHeatSeqNum(1),MaxHeatSeqNum(2));
#if 0
		afxDump << "ComputeFreezeEndpoints: m_freezeHeats=true, last index=" 
				<< index
				<< "\n";
#endif
		return index+1;
	}
	else {
		//  Freeze endpoints are computed on basis of m_numLockedHeats
		//  that is also the value we return
		ComputeFreezeEndpoint(1,rFreezeEnd1);
		ComputeFreezeEndpoint(2,rFreezeEnd2);
#if 0
		afxDump << "ComputeFreezeEndpoints: m_freezeHeats=false, numLockedHeats=" 
				<< m_numLockedHeats
				<< ", strand indices="
				<< rFreezeEnd1 - StrandBegin(1)
				<< ","
				<< rFreezeEnd2 - StrandBegin(2)
				<< "\n";
#endif
		return m_numLockedHeats;
	}
}


// if there are not lots on this strand, then there are no frozen heats
//   from this strand's viewpoint == return -1
// else, look at the last lot.

int CCastString::MaxHeatSeqNum(int strandNum)
{
	if ( Strand(strandNum).size() == 0 )
		// no orders on the strand
		return -1;
	else
		return Strand(strandNum).back()->m_heatSeqNum;
}



//  The first order with a heatSeqNum >= m_numLockedHeats is the
//    first unfrozen order, hence the end of frozen iteration.
//  If there is no such order, then the end is the end of the strand.

void CCastString::ComputeFreezeEndpoint(int strandNum, 
										vector<CCSOrder*>::iterator& rFreezeEnd)
{
	rFreezeEnd = StrandBegin(strandNum);

	for ( rFreezeEnd = StrandBegin(strandNum);
		  rFreezeEnd != StrandEnd(strandNum);
		  ++rFreezeEnd ) {

		if ( (*rFreezeEnd)->HeatSeqNum() >= m_numLockedHeats )
			break;
	}
}



//  Compute durations of lots on each strand on each frozen heat.

void CCastString::CalculateHeatsFrozen(int numFrozenHeats,
									  vector<CCSOrder*>::iterator end1,
									  vector<CCSOrder*>::iterator end2)
{
	// Compute durations and weights on each strand per heat.

	vector<long> heatDurs1;
	vector<long> heatDurs2;
	vector<Weight> heatWgts1;
	vector<Weight> heatWgts2;

	CalculateFrozenDurations(1,end1,heatDurs1,heatWgts1);
	CalculateFrozenDurations(2,end2,heatDurs2,heatWgts2);
	
	// TODO: watch out here.

	if ( m_heats.size() < numFrozenHeats )
		m_heats.resize(max(numFrozenHeats,
						   Props().NumHeats()));


	double lastTime = 0;

	T_HeatVec::iterator ih;
	vector<long>::iterator il1;
	vector<long>::iterator il2;
	vector<Weight>::iterator iw1;
	vector<Weight>::iterator iw2;


	for ( ih = m_heats.begin(),
		  il1 = heatDurs1.begin(),
		  il2 = heatDurs2.begin(),
		  iw1 = heatWgts1.begin(),
		  iw2 = heatWgts2.begin();
	      ih != m_heats.begin()+numFrozenHeats;
		  ++ih ) {

		if ( il1 == heatDurs1.end() 
			 &&
			 il2 == heatDurs2.end() ) {

			CTimeSpan dur = ComputeStdDuration( ih-m_heats.begin(), AimHeatSize() );

			(*ih).m_duration		= dur;
			(*ih).m_relStartTime	= lastTime;
			(*ih).m_relFinishTime	= lastTime + dur.GetTotalSeconds();
			(*ih).m_heatTons		= AimHeatSize();
			lastTime				+= dur.GetTotalSeconds();

		}
		else {

			long duration = 0;
			Weight tons = 0;
			if ( il1 != heatDurs1.end() ) {
				duration = (*il1);
				tons = (*iw1);
				++il1;
				++iw1;
			}
			
			if ( il2 != heatDurs2.end() ) {
				duration = max(duration,(*il2));
				tons += (*iw2);
				++il2;
				++iw2;
			}

			(*ih).m_duration		= duration;
			(*ih).m_relStartTime	= lastTime;
			(*ih).m_relFinishTime	= lastTime + duration;
			(*ih).m_heatTons		= tons;
			lastTime				+= duration;
		}
	}


	// Now that we know the beginning of each heat
	// we can compute the endpoints of each lot.

	CalculateLotEndpoints(1,end1);
	CalculateLotEndpoints(2,end2);
}


void CCastString::CalculateFrozenDurations(int strandNum,
										   vector<CCSOrder*>::iterator strandEnd,
										   vector<long>& durs,
										   vector<Weight>& weights)
{

	int maxHeatNum = 0;

	{
		for ( T_CSOrderVec::iterator io = StrandBegin(strandNum);
			  io != strandEnd;
			  ++io ) 
			maxHeatNum = max(maxHeatNum,(*io)->m_heatSeqNum);
	}

	durs.clear();
	durs.resize(maxHeatNum+1,0);
	weights.clear();
	weights.resize(maxHeatNum+1,0);

#if 0
	afxDump << "CalculateFrozenDurations, strand = " << strandNum
			<< ", resizing to " << durs.size() << "\n";
#endif


	for ( T_CSOrderVec::iterator io = StrandBegin(strandNum);
		  io != strandEnd;
		  ++io ) {

		CTimeSpan slabDur = (*io)->CalculateSlabDuration(m_id.m_caster,
														 (*io)->m_heatSeqNum,
														 AimHeatSize());
		(*io)->m_duration = (*io)->NumPieces()*slabDur.GetTotalSeconds();
		(*io)->NoteSlabHeatAssigns( (*io)->m_heatSeqNum, (*io)->NumPieces() );

		durs[(*io)->m_heatSeqNum] += (*io)->m_duration.GetTotalSeconds();
		weights[(*io)->m_heatSeqNum] += (*io)->Tons();

#if 0
		afxDump << "Order " << (*io)->OrderNum()
				<< ", dur = " << (*io)->Duration()
				<< ", heat# = " << (*io)->HeatSeqNum()
				<< ", heat dur = " << durs[(*io)->HeatSeqNum()]
				<< "\n";
#endif
	}
}


void CCastString::CalculateLotEndpoints(int strandNum,
										vector<CCSOrder*>::iterator strandEnd)
{
	int lastTime = 0;
	int lastHeatNum = 0;

	for ( T_CSOrderVec::iterator io = StrandBegin(strandNum);
		  io != strandEnd;
		  ++io ) {
		
		if ( (*io)->m_heatSeqNum != lastHeatNum ) {
			lastHeatNum = (*io)->m_heatSeqNum;
			lastTime = m_heats[lastHeatNum].m_relStartTime;
		}

		(*io)->m_relStartTime = lastTime;
		(*io)->m_relFinishTime = lastTime + (*io)->m_duration.GetTotalSeconds();
		lastTime = (*io)->m_relFinishTime;
	}
}
	


int CCastString::CalculateHeatsUnfrozen(int numFrozenHeats,
										 vector<CCSOrder*>::iterator begin1,
										 vector<CCSOrder*>::iterator begin2)
{

		
	
	//  Iterate through the orders on the string,
	//    and compute where each order completes on each strand

	//	Automatically splits lots in response to lots crossing
	//     heat boundaries.
	//	Therefore, it needs to un-split adjacent lots of identical characteristics.

	AbsorbAdjacentLots(begin1,begin2);

	return CalculateUnfrozenDurations(numFrozenHeats,begin1,begin2);
}





//  we have a three way iteration, 
//  balancing movement along strand1 slabs
//                           strand2 slabs
//                       and heats
//
//  we progress to a new heat when we have used up the metal in the current heat
//  we process the next slab from either strand1 or strand2, depending on which
//     is further back in time, based on durations of slabs already processed.

int CCastString::CalculateUnfrozenDurations(int numFrozenHeats,
											 vector<CCSOrder*>::iterator begin1,
											 vector<CCSOrder*>::iterator begin2)
{
	//  Get ready to do a nasty iteration

	// Here's a problem:  we may add some orders, 
	//  which will invalidate begin1 and begin2.
	// So we record the indexes for later.
	
	int oIndex1 = begin1 - StrandBegin(1);
	int oIndex2 = begin2 - StrandBegin(2);
	
	T_CSOrderVec::iterator ic1 = begin1;
	T_CSOrderVec::iterator ic2 = begin2;


	int slabCount1 = 0;
	int slabCount2 = 0;


	Weight heatTons		= 0;
	int heatCount		= numFrozenHeats;
	long heatStartTime	= (numFrozenHeats == 0
						  ? 0
						  : m_heats[numFrozenHeats-1].m_relFinishTime);

	long startTime1		= heatStartTime;
	long startTime2		= heatStartTime;
	long finishTime1	= heatStartTime;
	long finishTime2	= heatStartTime;

	while ( ic1 != StrandEnd(1) || ic2 != StrandEnd(2) ) {

#if 0
		afxDump << "Loop: 1: " << ic1 - m_strand[1].begin() 
				<< " of " << int(m_strand[1].size()) 
				<< (ic1 == m_strand[1].end() ? " END " : " OK  ")
				<< ", 2: " << ic2 - m_strand[2].begin()
				<< " of " << int(m_strand[2].size())
				<< (ic2 == m_strand[2].end() ? " END " : " OK  ")
				<< "\n";
#endif
		int pick = PickStrand(m_id.m_caster,
							  heatCount,
							  heatTons,
							  heatStartTime,
							  ic1,
							  slabCount1,
							  finishTime1,
							  ic2,
							  slabCount2,
							  finishTime2);
		switch ( pick ) {
		case 0:
			ProgressHeat(m_id.m_caster,
						 heatCount,
						 heatTons,
						 heatStartTime,
						 ic1,
						 slabCount1,
						 startTime1,
						 finishTime1,
						 ic2,
						 slabCount2,
						 startTime2,
						 finishTime2);
			break;
		case 1:
			// advance on strand1
			ProgressSlab(m_id.m_caster,
						 1,
						 heatCount,
						 heatTons,
						 heatStartTime,
						 ic1,
						 slabCount1,
						 startTime1,
						 finishTime1,
						 startTime2,
						 finishTime2);
			break;
		case 2:
			// advance on strand2
			ProgressSlab(m_id.m_caster,
						 2,
						 heatCount,
						 heatTons,
						 heatStartTime,
						 ic2,
						 slabCount2,
						 startTime2,
						 finishTime2,
						 startTime1,
						 finishTime1);
			break;
		}
	}


	//  of necessity, there is a heat which has not had its tonnage recorded
	//  provided that there are any slabs at all.

	if ( StrandBegin(1)+oIndex1 != StrandEnd(1) 
		 ||
		 StrandBegin(2)+oIndex2 != StrandEnd(2) )
		ProgressHeat(m_id.m_caster,
					 heatCount,
					 heatTons,
					 heatStartTime,
					 ic1,
					 slabCount1,
					 startTime1,
					 finishTime1,
					 ic2,
					 slabCount2,
					 startTime2,
					 finishTime2);

	return heatCount;
}


void CCastString::ComputeDummyHeats(int heatCount)
{
	// Adjust the number of heats 
	m_heats.resize(max(heatCount,m_props.NumHeats()));

	if ( heatCount < m_heats.size() ) {

#if 0
		afxDump << "Computing dummy heats " << heatCount 
				<< " through " << m_heats.size()-1
				<< "\n";
#endif
	}

	//  we may have some dummy heats that need filling out yet!
	int lastTime = 0;
	if ( heatCount > 0 ) 
		lastTime = m_heats[heatCount-1].m_relFinishTime;

	for ( int i=heatCount; i<m_heats.size(); ++i ) {

		CTimeSpan dur = ComputeStdDuration(i,AimHeatSize());
		m_heats[i].m_duration		= dur;
		m_heats[i].m_relStartTime	= lastTime;
		m_heats[i].m_relFinishTime	= lastTime + dur.GetTotalSeconds();
		m_heats[i].m_heatTons		= AimHeatSize();
		lastTime					+= dur.GetTotalSeconds();
	}
}



// Returns: 0 ==> move to next heat.
//          1 ==> advance one slab on strand1
//			2 ==> advance one slab on strand2

int CCastString::PickStrand(int caster,
							int heatCount,
							Weight heatTons,
							long heatStartTime,
							vector<CCSOrder*>::iterator ic1,
							int slabCount1,
							long finishTime1,
							vector<CCSOrder*>::iterator ic2,
							int slabCount2,
							long finishTime2)
{
#if 0
		afxDump << "PickStrand: " << caster << " "
				<< heatCount << " "
				<< heatTons << " "
				<< heatStartTime << " "
				<< ic1 - StrandBegin(1) << " "
				<< slabCount1 << " "
				<< finishTime1 << " "
				<< ic2 - StrandBegin(2) << " "
				<< slabCount2 << " "
				<< finishTime2 << ": ";
#endif

	Weight minSize = MinHeatSize();
	Weight aimSize = AimHeatSize();
	Weight maxSize = MaxHeatSize();

	if ( heatTons >= minSize ) {		// WAS aimSize
		// we have surpassed the aim size on this heat
		// indicate that we should move the next heat.
#if 0
		afxDump << "size exceeded, move to next heat.\n";
#endif
		return 0;
	}

	else if ( ic1 == StrandEnd(1) ) {

		Weight incrTons2 = (*ic2)->Tons()/(*ic2)->NumPieces();	// one piece

		if ( heatTons + incrTons2   > maxSize ) {
			// strand 1 is out, strand 2 puts us over.
#if 0
			afxDump << "strand 1 out, strand 2 to " << heatTons + incrTons2 
					<< " ==> move to next heat.\n";
#endif
			return 0;
		}
		else {
			// strand 1 is out, strand 2 is okay
#if 0
			afxDump << "strand 1 is out, process strand 2.\n";
#endif
			return 2;
		}
	}
	else if ( ic2 == StrandEnd(2) ) {

		Weight incrTons1 = (*ic1)->Tons()/(*ic1)->NumPieces();	// one piece

		if ( heatTons + incrTons1   > maxSize ) {
			// strand 2 is out, strand 1 puts us over.
#if 0
			afxDump << "strand 2 out, strand 1 to " << heatTons + incrTons1 
					<< " ==> move to next heat.\n";
#endif
			return 0;
		}
		else  {
			// strand 2 is out, strand 1 is okay
#if 0
			afxDump << "strand 2 is out, process strand 1.\n";
#endif
			return 1;
		}
	}
	else {
			

		Weight incrTons1 = (*ic1)->Tons()/(*ic1)->NumPieces();	// one piece
		Weight incrTons2 = (*ic2)->Tons()/(*ic2)->NumPieces();	// one piece

		//  Time to run the next slab on strand x, assuming on this heat.

		CTimeSpan slabDur1 = (*ic1)->CalculateSlabDuration(caster,
														   heatCount,
														   minSize);		// WAS aimSize
		CTimeSpan slabDur2 = (*ic2)->CalculateSlabDuration(caster,
														   heatCount,
														   minSize);		// WAS aimSize

		long   nextFinishTime1 = finishTime1 + slabDur1.GetTotalSeconds();
		long   nextFinishTime2 = finishTime2 + slabDur2.GetTotalSeconds();

		Weight nextFinishTons1 = heatTons + incrTons1;
		Weight nextFinishTons2 = heatTons + incrTons2;
	
#if 0
		afxDump << " values " << nextFinishTime1 << " "
				<< nextFinishTime2 << " "
				<< nextFinishTons1 << " "
				<< nextFinishTons2 << " - ";
#endif

		// we can decide to 
		//   (a) go to the next heat because either strand puts us over
		//   (b) pick one of the strands because the other puts us over
		//   (c) pick the strand with smallest finish time

		if ( nextFinishTons1 > maxSize && nextFinishTons2 > maxSize ) {
			// both put us over the limit
#if 0
			afxDump << "both over, go to next heat\n";
#endif
			return 0;
		}
		else if ( nextFinishTons1 > maxSize ) {
			// 1 puts us over, 2 does not
#if 0
			afxDump << "1 over, use 2.\n";
#endif
			return 2;
		}
		else if ( nextFinishTons2 > maxSize ) {
			// 2 puts us over, 1 does not
#if 0
			afxDump << "2 over, use 1.\n";
#endif
			return 1;
		}
		else if ( nextFinishTime1 < nextFinishTime2 ) {
			// both okay, but 1 finishes first
#if 0
			afxDump << "both okay, 1 finishes first.\n";
#endif
			return 1;
		}
		else  {
			// both okay but 2 finishes first
#if 0
			afxDump << "both okay, 2 finishes first.\n";
#endif
			return 2;
		}
	}
}



void CCastString::ProgressHeat(int caster,
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
							   long& finishTime2)
{
#if 0
	afxDump << "Finishing heat " << heatCount
			<< ": " << heatTons << " "
			<< heatStartTime << "\n";
#endif

	if ( m_heats.size() <= heatCount )
		m_heats.resize(max(heatCount+1,
						   m_props.NumHeats()));

	m_heats[heatCount].m_relStartTime	= heatStartTime;
	m_heats[heatCount].m_relFinishTime	= max(finishTime1,finishTime2);
	m_heats[heatCount].m_duration		
		= m_heats[heatCount].m_relFinishTime - m_heats[heatCount].m_relStartTime;
	m_heats[heatCount].m_heatTons		= heatTons;

	// let's make sure we update the orders before we advance the heat.


	if ( ic1 != StrandEnd(1) ) {
		if ( slabCount1 == 0 ) {
		}
		else {
#if 0
			afxDump << "Splitting on strand 1, order = " 
					<< (*ic1)->OrderNum()
					<< ", slabCount = " 
					<< "#pieces = " 
					<< (*ic1)->NumPieces()
					<< "\n";
#endif
			int newCount= (*ic1)->NumPieces() - slabCount1;
			(*ic1)->NumPieces(slabCount1);
			CCSOrder* pNewOrder = (*ic1)->SplitLot(heatCount,newCount);
			int index = ic1 - StrandBegin(1);
			Strand(1).insert(ic1+1,pNewOrder);
			ic1 = StrandBegin(1) + index;
			ProgressNextOrder(1,
							  heatCount,
							  ic1,
							  slabCount1,
							  startTime1,
							  finishTime1);
		}
	}

	if ( ic2 != StrandEnd(2) ) {
		if ( slabCount2 == 0 ) {
		}
		else {
#if 0
			afxDump << "Splitting on strand 2, order = " 
					<< (*ic2)->OrderNum()
					<< ", slabCount = " 
					<< "#pieces = " 
					<< (*ic2)->NumPieces()
					<< "\n";
#endif
			int newCount= (*ic2)->NumPieces() - slabCount2;
			(*ic2)->NumPieces(slabCount2);
			CCSOrder* pNewOrder = (*ic2)->SplitLot(heatCount,newCount);
			int index = ic2 - StrandBegin(2);
			Strand(2).insert(ic2+1,pNewOrder);
			ic2 = StrandBegin(2) + index;
			ProgressNextOrder(2,
							  heatCount,
							  ic2,
							  slabCount2,
							  startTime2,
							  finishTime2);
		}
	}

	++heatCount;
	heatTons = 0;
	heatStartTime = max(finishTime1,finishTime2);
	startTime1 = heatStartTime;
	startTime2 = heatStartTime;
	finishTime1 = heatStartTime;
	finishTime2 = heatStartTime;
}



void CCastString::ProgressSlab(int caster,
							   int strandNum,
							   int& heatCount,
							   Weight& heatTons,
							   long& heatStartTime,
							   vector<CCSOrder*>::iterator& ic,
							   int& slabCount,
							   long& startTime,
							   long& finishTime,
							   long& otherStartTime,
							   long& otherFinishTime)
{
	Weight incrTons = (*ic)->Tons()/(*ic)->NumPieces();	// one piece

	CTimeSpan slabDur = (*ic)->CalculateSlabDuration(caster,
													 heatCount,
													 AimHeatSize());

	finishTime += slabDur.GetTotalSeconds();
	heatTons += incrTons;
	(*ic)->NoteSlabHeatAssign(heatCount);
	++slabCount;

#if 0
	afxDump << "Using slab on strand " << strandNum
			<< " for heat " << heatCount
			<< "\n";
#endif

	if ( slabCount >= (*ic)->NumPieces() ) 
		ProgressNextOrder(strandNum,
						  heatCount,
						  ic,
						  slabCount,
						  startTime,
						  finishTime);

}
							   


void CCastString::ProgressNextOrder(int strandNum,
									int heatCount,
									vector<CCSOrder*>::iterator& ic,
									int& slabCount,
									long& startTime,
									long& finishTime)
{
#if 0
	afxDump << "Moving to next order on strand "
			<< strandNum << ".\n";
#endif

	(*ic)->m_relStartTime	= startTime;
	(*ic)->m_relFinishTime	= finishTime;
	(*ic)->m_duration	    = (*ic)->RelFinishTime() - (*ic)->RelStartTime();
	(*ic)->m_heatSeqNum		= heatCount;
	++ic;
	slabCount = 0;
	startTime = finishTime;

#if 0
	{
		for ( vector<CCSOrder*>::iterator ii = ic;
			  ii != StrandEnd(strandNum);
			  ++ii )
			afxDump << (*ii)->OrderNum() << " ";

		afxDump << "\n";
	}
#endif

}







void CCastString::AbsorbAdjacentLots(vector<CCSOrder*>::iterator begin1,
									 vector<CCSOrder*>::iterator begin2)
{
	//  We have a notion of when two lots are equivalent.
	//  If two adjacent lots are equivalent, we rejoin them.
	//  Done on a per-strand basis.

	AbsorbAdjacentLots(1,begin1);
	AbsorbAdjacentLots(2,begin2);
}



void CCastString::AbsorbAdjacentLots(int strandNum,vector<CCSOrder*>::iterator begin)
{
	T_CSOrderVec& orders = m_strand[strandNum];

	// nothing to do unless there are at least 2 orders in the strand.

	if ( orders.size() <= 1 ) 
		return;

	//  We iterate from first to last-but-one,
	//     since we compare to the following order.
	//  Note that the iteration is advanced conditionally:
	//     if we absorb the following order, we want to stay
	//        on this order because the order that moves up
	//        might also be absorbed.
	//  Note that the end might move up to meet us, so we 
	//		have to stop at either end or end-1.

	for ( T_CSOrderVec::iterator io = begin;
		  io != orders.end() - 1 && io != orders.end();
		  ) {
		
		if ( (*io)->IsEquivalentLot( *(io+1), true ) ) {
			(*io)->AbsorbLot( *(io+1) );
			orders.erase( io+1 );
		}
		else
			++io;

	}
}


void CCastString::SplitHeatBoundaryLots()
{
	SplitHeatBoundaryLots(1);
	SplitHeatBoundaryLots(2);
}


void CCastString::SplitHeatBoundaryLots(int strandNum)
{
	T_CSOrderVec& orders = m_strand[strandNum];
	T_CSOrderVec newSeq;

	newSeq.reserve(orders.size());

	for ( T_CSOrderVec::iterator io = orders.begin();
		  io != orders.end();
		  ++io ) {

		vector< pair< int, int > >& assigns = (*io)->SlabHeatAssigns();

		assert( assigns.size()  > 0 );

		(*io)->m_heatSeqNum = assigns.front().first;
		(*io)->NumPieces( assigns.front().second );

		newSeq.push_back( (*io) );

		if ( assigns.size() > 1 ) {

			// this lot is on multiple heats.

			for ( vector< pair< int, int > >::iterator ip = (*io)->SlabHeatAssigns().begin()+1;
				  ip != (*io)->SlabHeatAssigns().end();
				  ++ip ) 

				newSeq.push_back( (*io)->SplitLot( (*ip).first, (*ip).second ) );
				
		}
	}
			
	m_strand[strandNum] = newSeq;		// copy vector
}




void CCastString::InitializeHeatOrderPointers()
{
	vector<CCSOrder*>& strand1 = Strand(1);
	vector<CCSOrder*>& strand2 = Strand(2);


	{
		for ( vector<CCastStringHeat>::iterator ih = m_heats.begin();
			  ih != m_heats.end();
			  ++ih ) {

			(*ih).SetStrandBegin ( 1, strand1.end() );
			(*ih).SetStrandEnd   ( 1, strand1.end() );
			(*ih).SetStrandBegin ( 2, strand2.end() );
			(*ih).SetStrandEnd   ( 2, strand2.end() );
		}
	}



	{
		for ( vector<CCSOrder*>::iterator io = strand1.begin();
			  io != strand1.end();
			  ++io ) {
			
			CCastStringHeat& rHeat = m_heats[ (*io)->HeatSeqNum() ];
			if ( rHeat.StrandBegin(1) == strand1.end() )
				rHeat.SetStrandBegin(1,io);
			rHeat.SetStrandEnd(1,io+1);

		}
	}

	{
		for ( vector<CCSOrder*>::iterator io = strand2.begin();
			  io != strand2.end();
			  ++io ) {
			
			CCastStringHeat& rHeat = m_heats[ (*io)->HeatSeqNum() ];
			if ( rHeat.StrandBegin(2) == strand2.end() )
				rHeat.SetStrandBegin(2,io);
			rHeat.SetStrandEnd(2,io+1);

		}
	}


		// Some heats might have no orders.
		// Rather than leave the StrandBegin and StrandEnd of such a heat
		//   pointing at the end of strand, we make these be the same
		//   as the StrandBegin and StrandEnd of the preceding heat.
		// If this is the first heat, then it must point to the beginning.

	{
		for ( vector<CCastStringHeat>::iterator ih = m_heats.begin();
			  ih != m_heats.end();
			  ++ih ) {
				  
				
			if ( (*ih).StrandBegin(1) ==  strand1.end() ) {

				if ( ih == m_heats.begin() ) {
					(*ih).SetStrandBegin( 1, strand1.begin() );
					(*ih).SetStrandEnd  ( 1, strand1.begin() );
				}
				else {
					(*ih).SetStrandBegin( 1, (*(ih-1)).StrandEnd(1));
					(*ih).SetStrandEnd  ( 1, (*(ih-1)).StrandEnd(1));
				}
			}

			if ( (*ih).StrandBegin(2) ==  strand2.end() ) {

				if ( ih == m_heats.begin() ) {
					(*ih).SetStrandBegin( 2, strand2.begin() );
					(*ih).SetStrandEnd  ( 2, strand2.begin() );
				}
				else {
					(*ih).SetStrandBegin( 2, (*(ih-1)).StrandEnd(2));
					(*ih).SetStrandEnd  ( 2, (*(ih-1)).StrandEnd(2));
				}
			}
		}
	}
}






////////////////////////////////////////////////////////////////
//	
//	Miscellaneous
//	
////////////////////////////////////////////////////////////////

// TODO -- verify that RHOB on #1SC is with spec XXX-4X-XX
// TODO -- verify that per string is okay here, not per heat.

CTimeSpan CCastString::PourToOpenTime() const
{
	int hours;
	int minutes;

	if ( Id().Caster() != 1 ) {
		hours = 2;
		minutes = 0;
	}
	// Caster TODO:  What is correct caln of PourToOpenTime for casters 4,5 
	else if ( Heats().size() > 0
		      &&
			  Heats()[0].Spec().GetLength() >= 4
			  &&
			  Heats()[0].Spec().GetAt(3) == 4 ) {
		// RHOB
		hours = 2;
		minutes = 30;
	}
	else {
		// non-RHOB
		hours = 2;
		minutes = 30;
	}

	return CTimeSpan(0,hours,minutes,0);
}



CTimeSpan CCastString::InterStringTime() const
{
	return m_props.TurnaroundTime();
}


//  Should this be per-heat?
//  We need to deal with high-metal vs low-metal practice at #1SC

Weight CCastString::HotMetalTonsPerHeat() const
{
	Weight w;

	if ( Id().Caster() == 1 )
		w = 225;
	// CASTER TODO: What is correct HotMetalTonsPerHeat for casters 4,5
	else
		w = 195;

	return w;
}



////////////////////////////////////////////////////////////////
//	
//	Computing summaries
//	
////////////////////////////////////////////////////////////////

void CCastString::CalculateSummary()
{
	CalculateTransitions();

	m_heatSummaries[0].clear();
	m_heatSummaries[1].clear();
	m_heatSummaries[2].clear();


	CCastStringSummaryRecord zeroRecord;

	m_heatSummaries[0].insert(m_heatSummaries[0].begin(),
							  m_heats.size(),
							  zeroRecord);
	m_heatSummaries[1].insert(m_heatSummaries[1].begin(),
							  m_heats.size(),
							  zeroRecord);
	m_heatSummaries[2].insert(m_heatSummaries[2].begin(),
							  m_heats.size(),
							  zeroRecord);

	CalculateStrandSummary(1);
	CalculateStrandSummary(2);
	
	m_stringSummary.Init();
	m_stringSummary.Sum(m_strandSummary[1]);
	m_stringSummary.Sum(m_strandSummary[2]);
	m_stringSummary.m_duration = max(m_strandSummary[1].m_duration,m_strandSummary[2].m_duration);
	m_stringSummary.m_avgWidth = m_strandSummary[1].m_avgWidth + m_strandSummary[2].m_avgWidth;

	// fix up heat durations
	int count = 0;
	for ( T_HeatVec::iterator ih = m_heats.begin();
		  ih != m_heats.end();
		  ++ih, ++count ) {

		m_heatSummaries[0][count].m_duration = (*ih).Duration();
		m_heatSummaries[1][count].m_duration = (*ih).Duration();
		m_heatSummaries[2][count].m_duration = (*ih).Duration();
		  }


}


void CCastString::CalculateStrandSummary(int strandNum)
{
	m_strandSummary[strandNum].Init();

	T_CSOrderVec& orders = m_strand[strandNum];

	for ( T_CSOrderVec::iterator io = orders.begin();
		  io != orders.end();
		  ++io ) {

		m_strandSummary[strandNum].AddCSOrder((*io));

		for ( vector<pair<int,int> >::iterator ip = (*io)->m_slabHeatAssigns.begin();
			  ip != (*io)->m_slabHeatAssigns.end();
			  ++ip ) {

			m_heatSummaries[    0    ][ (*ip).first ].AddCSOrder( (*io), (*ip).second );
			m_heatSummaries[strandNum][ (*ip).first ].AddCSOrder( (*io), (*ip).second );
		}
	}
}


void CCastString::CalculateTransitions()
{
	CalculateTransitions(1);
	CalculateTransitions(2);
}

void CCastString::CalculateTransitions(int strandNum)
{
	T_CSOrderVec& orders = m_strand[strandNum];

	if ( orders.size() == 0 )
		return;

	orders[0]->IsTransition(false);
	orders[0]->IsRadical(false);


	for ( T_CSOrderVec::iterator io = orders.begin()+1;
		  io != orders.end();
		  ++io ) {

		T_CSOrderVec::iterator iprev = io-1;

		Width diff = fabs( (*io)->SlabWidth() - (*iprev)->SlabWidth() );
		(*io)->IsTransition( diff > 0.0 );
		(*io)->IsRadical( diff >= 2.0 ); 
	}
}





////////////////////////////////////////////////////////////////
//	
//	Uploading to the outside world
//	
////////////////////////////////////////////////////////////////


bool CCastString::IsSameString( const CCastString* pString) const
{

	if ( pString == 0 )
		return false;

	if ( Id() != pString->Id() )
		return false;

	if ( Heats().size() != pString->Heats().size() ) 
		return false;

	vector<CCastStringHeat>::const_iterator ih1;
	vector<CCastStringHeat>::const_iterator ih2;

	for ( ih1 = Heats().begin(),
		  ih2 = pString->Heats().begin();
	      ih1 != Heats().end();
		  ++ih1, ++ih2 )

		if ( ! (*ih1).IsSameHeat((*ih2)) )
			return false;


	return true;
}



bool CCastString::Upload(CUploadMgr& uMgr, int newStatus)
{
	try 
	{
		int heatIndex = 0;
		for ( T_HeatVec::iterator ih = m_heats.begin();
			  ih != m_heats.end();
			  ++ih, ++heatIndex ) 

			UploadHeat(uMgr,newStatus,heatIndex,(*ih));
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




void CCastString::UploadHeat( CUploadMgr& uMgr, 
							  int newStatus,
							  int heatIndex,
							  CCastStringHeat& rHeat)
{

	CString statusToUse;

	switch ( newStatus ) {
	case 1:
		statusToUse = "1";
		break;

	case 3:
		statusToUse = ( rHeat.IsMarked() ? "3" : "1" );
		break;

	case 5:
		statusToUse = ( rHeat.Status() == CCastStringHeat::HEAT_STATUS_910
						? "5"
						: "3" );
		break;

	default:
		assert( 0 );
		break;
	}


	UploadHeatChems(uMgr,statusToUse,heatIndex,rHeat);

	int lotIndex = 0;
	{
		
		for ( T_CSOrderVec::iterator io = rHeat.StrandBegin(1);
			  io != rHeat.StrandEnd(1);
			  ++io, ++lotIndex )
			UploadCSOrder(uMgr,statusToUse,heatIndex,rHeat,1,lotIndex,(*io));
	}

	{
	
		for ( T_CSOrderVec::iterator io = rHeat.StrandBegin(2);
			  io != rHeat.StrandEnd(2);
			  ++io, ++lotIndex )
			UploadCSOrder(uMgr,statusToUse,heatIndex,rHeat,2,lotIndex,(*io));
	}
}
		     




void CCastString::UploadHeatChems( CUploadMgr& uMgr,
								   CString& statusToUse,
								   int heatIndex,
								   CCastStringHeat& rHeat)
{

	CDailyPlanLineupsUploadSet& luSet = uMgr.LUSet();

	// For each heat, we write several records to the DPA_DAILY_PLAN_LINEUPS file,
	//   one record for each chemistry.
	
	char buf[30];

	for ( int elt = 0; elt < CChem::NumElements; ++elt ) {

		if ( rHeat.MinChem(CChem::Element(elt)) == 0.0
			 &&
			 rHeat.MaxChem(CChem::Element(elt)) == 100.0 )
			continue;

		// Let's go
		luSet.AddNew();

		//  this is just the caster unit
		luSet.m_LU_RECORD_TYPE
			= _itoa(Id().Caster(),buf,10);

		//  910 -> 5
		//  marked for handoff -> 3
		//  909 -> 1
		luSet.m_PLAN_HEAT_STATUS_CODE = statusToUse;

		//  ???  We are not doing deletions yet.
		luSet.m_RECORD_DESCRIPTOR = "LU";

		// Plan week encoded as YYYYWW
		luSet.m_PLAN_WEEK
			= Id().Year()*100 + Id().Week();

		// ??? I don't know how these are assigned
		luSet.m_PLAN_CAST_SEQUENCE_NO	= 0;

		luSet.m_PLAN_STRING_ID
			= Id().StringId();

		// Numbering is 1-based in the database, but we are 0-based internally.
		luSet.m_PLAN_HEAT_SEQUENCE_NO
			= heatIndex + 1;

		// ??? I suppose I can compute this, but don't yet do so.
		luSet.m_PLAN_HEAT_GROUP		= 1;

		luSet.m_PLAN_CAST_UNIT_CODE
			= _itoa(Id().Caster(),buf,10);

		luSet.m_PLAN_HEAT_TAB_NUMBER
			= rHeat.TabNum();
 
		luSet.m_PLAN_HEAT_SPEC_WITH_DASHES
			= rHeat.Spec().Left(3)
			  + "-"
			  + rHeat.Spec().Mid(3,2)
			  + "-"
			  + rHeat.Spec().Right(2);

		// per heat comments are handled in the other file.
		luSet.m_PLAN_LINEUP_COMMENTS = "";

		// Repeating group
		// luSet.m_SM_SPEC_INFO

		luSet.m_ELEMENT_SYMBOL
			= CChem::ToString(CChem::Element(elt));

		// CString::Format was reallocating the buffers, 
		//   causing CRecordset to puke,
		//   so had to resort to a workaround.

		// ??? These are A8 fields, how to format?
//		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT.Format("%8.4f",
//													   rHeat.MinChem(CChem::Element(elt)));
//
//		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT.Format("%8.4f",
//													  rHeat.MaxChem(CChem::Element(elt)));
//
//		// ??? I don't know what the aim is, so I just average
//		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT.Format("%8.4f",
//													  rHeat.AimChem(CChem::Element(elt)));

	
		CString temp;
		temp.Format("%8.4f",rHeat.MinChem(CChem::Element(elt)));

		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT = temp;

		temp.Format("%8.4f",rHeat.MaxChem(CChem::Element(elt)));

		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT = temp;

		// ??? I don't know what the aim is, so I just average
		temp.Format("%8.4f",rHeat.AimChem(CChem::Element(elt)));
		luSet.m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT = temp;



		// ??? No idea.
		luSet.m_PLAN_HEAT_BACKUP_MOLD_CODE	= 0;

		// ??? Should be okay to hardwire this.
		luSet.m_PLAN_HOT_MILL	 = 8;

		// ??? Computed outside
		luSet.SetFieldNull(&luSet.m_LIQUIDOUS_TEMP_C);
		luSet.SetFieldNull(&luSet.m_SOLIDOUS_TEMP_C);

		// ??? I don't know the distinctions here,
		//     and this won't be used 'til I2 comes online.

		luSet.m_PLAN_STRING_EARLY_START_DATE	= 0;
		luSet.m_PLAN_STRING_EARLY_START_TIME	= 0;
		luSet.m_PLAN_STRING_LATEST_END_DATE		= 0;
		luSet.m_PLAN_STRING_LATEST_END_TIME		= 0;
		luSet.m_PLAN_HEAT_EARLY_START_DATE		= 0;
		luSet.m_PLAN_HEAT_EARLY_START_TIME		= 0;
		//luSet.m_PLAN_HEAT_LATEST_END_DATE 		= ;
		//luSet.m_PLAN_HEAT_LATEST_END_TIME		= ;
		rHeat.ComputePlannedLatestStart(luSet.m_PLAN_HEAT_LATEST_END_DATE,
										luSet.m_PLAN_HEAT_LATEST_END_TIME);
		luSet.m_PLAN_HEAT_LATEST_END_TIME /= 100;
	luSet.m_PLAN_HEAT_LATEST_END_TIME = max(0,min(luSet.m_PLAN_HEAT_LATEST_END_TIME,9999));
	luSet.m_PLAN_HEAT_LATEST_END_DATE = max(0,min(luSet.m_PLAN_HEAT_LATEST_END_DATE,99999999));



		//  The remaining fields are per-lot, so I'm going to NULL them.

		luSet.SetFieldNull(&luSet.m_PLAN_LOT_NUMBER);
		luSet.SetFieldNull(&luSet.m_PLAN_CASTER_STRAND_NO);
		luSet.SetFieldNull(&luSet.m_PLAN_NUMBER_OF_PIECES);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_SPEC_WITH_DASHES);
		luSet.SetFieldNull(&luSet.m_PLAN_SLAB_CONDITION_CD);
		luSet.SetFieldNull(&luSet.m_PLAN_EXPOSURE_CODE);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_DISP_CODE);
		luSet.SetFieldNull(&luSet.m_PLAN_STEEL_WIDTH_MM);
		luSet.SetFieldNull(&luSet.m_PLAN_STEEL_THICKNESS_MM);
		luSet.SetFieldNull(&luSet.m_PLAN_STEEL_LENGTH_MM);
		luSet.SetFieldNull(&luSet.m_PLAN_MIN_SLAB_LENGTH_MM);
		luSet.SetFieldNull(&luSet.m_PLAN_MAX_SLAB_LENGTH_MM);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_STL_EARLY_START_DATE);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_STL_EARLY_START_TIME);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_STL_LATEST_END_DATE);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_STL_LATEST_END_TIME);
		luSet.SetFieldNull(&luSet.m_MILL_ORDER_NO);
		// FP CHANGE
		luSet.SetFieldNull(&luSet.m_FP_M_ORDER_NO);
		luSet.SetFieldNull(&luSet.m_PLAN_LOT_TYPE_SLIT_CODE);

		//  ???  Not a clue
		luSet.SetFieldNull(&luSet.m_PLAN_DELETE_DATE);
		luSet.SetFieldNull(&luSet.m_PLAN_DELETE_TIME);
		luSet.SetFieldNull(&luSet.m_DESC_UNIT_STATUS_TAB_KEY);
		luSet.SetFieldNull(&luSet.m_DESC_UNIT_TAB_STATUS_KEY);

		luSet.Update();
	}
}

void CCastString::UploadCSOrder( CUploadMgr& uMgr,
								 CString& statusToUse,
								 int heatIndex,
								 CCastStringHeat& rHeat,
								 int strandNum,
								 int lotIndex,
								 CCSOrder* pOrder)
{

	CDailyPlanUploadSet& dpSet = uMgr.DPSet();

	char buf[30];

	dpSet.AddNew();

	// Plan week encoded as YYYYWW
	dpSet.m_PLAN_WEEK
		= Id().Year()*100 + Id().Week();

	dpSet.m_PLAN_CAST_UNIT_CODE
		= _itoa(Id().Caster(),buf,10);
	
	// ??? I don't know how these are assigned
	dpSet.m_PLAN_CAST_SEQUENCE_NO = 0;

	dpSet.m_PLAN_STRING_ID
		= Id().StringId();

	// ??? I suppose I can compute this, but don't yet do so.
	dpSet.m_PLAN_HEAT_GROUP = 1;

	// Numbering is 1-based in the database, but we are 0-based internally.
	dpSet.m_PLAN_HEAT_SEQUENCE_NO
		= heatIndex + 1;

	dpSet.m_PLAN_HEAT_TAB_NUMBER
		= rHeat.TabNum();

	//  first five digits
	dpSet.m_PLAN_HEAT_SPEC
		= atol(rHeat.Spec().Left(5));

	//  What it says
	dpSet.m_PLAN_HEAT_SIXTH_DIGIT_SPEC
		= rHeat.Spec().Mid(5,1);

	//  What it says
	dpSet.m_PLAN_HEAT_SEVENTH_DIGIT_SPEC
		= atol(rHeat.Spec().Right(1));

	dpSet.m_PLAN_STEEL_EARLY_START_DATE = 0;
	dpSet.m_PLAN_STEEL_EARLY_START_TIME = 0;

	rHeat.ComputePlannedLatestStart(dpSet.m_PLAN_STEEL_LATEST_END_DATE,
									dpSet.m_PLAN_STEEL_LATEST_END_TIME);
	dpSet.m_PLAN_STEEL_LATEST_END_TIME /= 100;
	dpSet.m_PLAN_STEEL_LATEST_END_TIME = max(0,min(dpSet.m_PLAN_STEEL_LATEST_END_TIME,9999));
	dpSet.m_PLAN_STEEL_LATEST_END_DATE = max(0,min(dpSet.m_PLAN_STEEL_LATEST_END_DATE,99999999));



	//dpSet.m_PLAN_STEEL_LATEST_END_DATE  = 0;
	//dpSet.m_PLAN_STEEL_LATEST_END_TIME  = 0;

	//  It was suggested that we use my figure.
	dpSet.m_PLAN_HEAT_PROD_TIME_MINUTES = rHeat.Duration().GetTotalMinutes();

	dpSet.m_PLAN_HEAT_DISPOSITION_CODE = rHeat.DispCode();

	// ??? No idea.
	dpSet.m_PLAN_HEAT_BACKUP_MOLD_CODE = 0;

	dpSet.m_PLAN_HEAT_CARBON_MIN
		= rHeat.MinChem(CChem::ELT_C);

	dpSet.m_PLAN_HEAT_CARBON_MAX
		= rHeat.MaxChem(CChem::ELT_C);

	dpSet.m_PLAN_HEAT_MANGANESE_MIN
		= rHeat.MinChem(CChem::ELT_MN);

	dpSet.m_PLAN_HEAT_MANGANESE_MAX
		= rHeat.MaxChem(CChem::ELT_MN);

	//  910 -> 5
	//  marked for handoff -> 3
	//  909 -> 1
	dpSet.m_PLAN_HEAT_STATUS_CODE = statusToUse;

	//  ???  Who assigns this number?
	dpSet.m_PLAN_LINEUP_NUMBER = 0;

	//  ???  Who assigns this number?
	//       How is it different from PLAN_HEAT_SEQUENCE_NO?
	dpSet.m_PLAN_LINEUP_ITEM_SEQ_NO = 0;

	// I have per heat comments
	dpSet.m_PLAN_LINEUP_COMMENTS
		= rHeat.Comment().GetLength() > 80 
		  ? rHeat.Comment().Left(80)
		  : rHeat.Comment();
	
	// ??? I have no idea what this should be
	dpSet.m_PLAN_910_REPORT_IND = " ";

	//  ??? where does this come from?
	dpSet.m_PLAN_SHOP_SCHED_DATE = 0;
	dpSet.m_PLAN_SHOP_SCHED_TIME = 0;

	//  This is a PE Element
	//dpSet.m_PLAN_LOT_INFORMATION

	// Here are the lot level fields

	//  This field is 1-based in the db, 0-based internally.
	dpSet.m_PLAN_LOT_NUMBER
		= lotIndex + 1;

	// FP change
	dpSet.m_MILL_ORDER_NO
		= atol(pOrder->FpOrderNum().Left(7));

	dpSet.m_FP_M_ORDER_NO
		= pOrder->FpOrderNum();

//	Added population of the 2 Factory Planner originated fields below for 
//   909 Reschuffle Handoff to IBM Request 5-03-04 K. Hubbard

	dpSet.m_FP_ISHRO_PRTY 
		= ( pOrder->Order() == 0
			? 0
			: pOrder->Order()->FpIshroPrty()); 

	dpSet.m_FP_PLAN_MELT_PST_DATE 
		= ( pOrder->Order() == 0
			? 0
			: pOrder->Order()->FpMeltPstDate());  // YYYYMMDD format       
//			: NTime::PSDateDelta(pOrder->Order()->PlannedLatestStartDate()));  // 0 - 9999 integer format

	dpSet.m_PLAN_STEEL_WIDTH
		= pOrder->SlabWidth();

	dpSet.m_PLAN_STEEL_THICKNESS
		= pOrder->SlabThickness();

	dpSet.m_PLAN_STEEL_LENGTH
		= pOrder->SlabLength();

	dpSet.m_PLAN_STEEL_WGT_TONS
		= pOrder->Tons();

	dpSet.m_PLAN_NUMBER_OF_PIECES
		= pOrder->NumPieces();

	dpSet.m_PLAN_LOT_SPEC
		= pOrder->LotSpec();

	dpSet.m_PLAN_LOT_DISP_CODE
		= pOrder->OutputDispCode();

	dpSet.m_PLAN_LOT_TYPE_SLIT_CODE
		= pOrder->SlitTypeCode();

	//  ??? Where does this come from?
	dpSet.m_PLAN_LOT_TYPE_SLAB = "";

	dpSet.m_PLAN_SLAB_CONDITION_CD
		= pOrder->OutputCondnCode();

	//  ??? wher does this come from?
	dpSet.m_PLAN_EXPOSURE_CODE 
		= pOrder->ExposureCode();

	//  ??? I think it is okay to hardwire this.
	dpSet.m_PLAN_HOT_MILL_CD
		= 8;

	dpSet.m_PLAN_CASTER_STRAND_NO
		= _itoa(strandNum,buf,10);

	// ???  For now, just blank
	dpSet.m_PLAN_CHANGE_INDICATOR = " ";

	// ???  If I'm not really tracking this ...
	dpSet.m_PLAN_ORIGINAL_PLAN_ID = "";

	// from smp and mord
	dpSet.m_PLAN_EXPECTED_APPLIED_SPEC 
		= (pOrder->Order() == 0 
			? pOrder->LotSpec() 
			: pOrder->Order()->CastSpec());
	dpSet.m_PLAN_EXPECTED_APPLIED_COND_CD 
		= (pOrder->Order() == 0
			? pOrder->SlabCondnCode()
			: pOrder->Order()->SlabCondnCode());

	// ??? what should this be?
	dpSet.m_PLAN_909_LOCK_IND	= "";

	// ??? Where do these come from? the order?
	dpSet.m_PLAN_STEEL_LENGTH_MIN = pOrder->PlanSteelLengthMin();
	dpSet.m_PLAN_STEEL_LENGTH_MAX = pOrder->PlanSteelLengthMax();

	dpSet.m_STOCK_REASON            // added reason field 5-5-09 k. hubbard 
		= pOrder->StockReason();

	dpSet.m_STOCK_COMMERCIAL_REASON   // added reason field 5-5-09 k. hubbard 
		= pOrder->StockCommercialReason();

	dpSet.m_SLIT_REASON             // added reason field 2-26-10 k. hubbard 
		= pOrder->SlitReason();


	// ??? I have not a clue on these;
	dpSet.m_PLAN_CAST_UNIT_ORDER_KEY	= "";
	dpSet.m_PLAN_UNIT_HT_STATUS_TAB_KEY	= "";
	dpSet.m_PLAN_CAST_UNIT_HEAT_SEQ_KEY	= "";
	dpSet.m_PLAN_CAST_TAB_STATUS_KEY	= "";
	dpSet.m_PLAN_ADJUSTMENT_KEY			= "";
	dpSet.m_PLAN_MILL_ORDER_KEY			= "";
	dpSet.m_PLAN_SHOP_SCHED_KEY			= "";

	dpSet.Update();
}



bool CCastString::UploadDeleteRecords(CUploadMgr& uMgr)
{
	CDailyPlanLineupsUploadSet& luSet = uMgr.LUSet();
	char buf[30];

	try 
	{
		int heatIndex = 0;
		for ( T_HeatVec::iterator ih = m_heats.begin();
			  ih != m_heats.end();
			  ++ih, ++heatIndex ) {

			if ( (*ih).TabNum() == 0 )
				continue;

			// Let's go
			luSet.AddNew();
	
			//  this is just the caster unit
			luSet.m_LU_RECORD_TYPE
				= _itoa(Id().Caster(),buf,10);	

			//  Doing a deletion.
			luSet.m_RECORD_DESCRIPTOR = "DT";


			luSet.m_PLAN_CAST_UNIT_CODE
				= _itoa(Id().Caster(),buf,10);

			luSet.m_PLAN_HEAT_TAB_NUMBER
				= (*ih).TabNum();

			luSet.Update();
		}

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

#ifdef DEPRECATED
// get rid of this, then get rid of CRuleCheckLineupSet.
bool CCastString::UploadForRuleCheck()
{
	CDBConn dbc(CDBConn::DB_RULES);
	dbc.Connect();

	CRulesCheckLineupSet rset(dbc.GetDB());
	rset.Open(CRecordset::dynaset,0,CRecordset::appendOnly);

	static CString strStatus = "3";

	try { 

		dbc.GetDB()->BeginTrans();
		dbc.GetDB()->ExecuteSQL("DELETE FROM CSDAInput");

		int heatIndex = 0;
		for ( vector<CCastStringHeat>::iterator ih = Heats().begin();
			  ih != Heats().end();
			  ++ih, ++heatIndex ) {


			int lotIndex = 0;
			{
				
				for ( T_CSOrderVec::iterator io = (*ih).StrandBegin(1);
					  io != (*ih).StrandEnd(1);
					  ++io, ++lotIndex )
					UploadCSOrderForRuleCheck(rset,strStatus,heatIndex,(*ih),1,lotIndex,(*io));
			}

			{
				
				for ( T_CSOrderVec::iterator io = (*ih).StrandBegin(2);
					  io != (*ih).StrandEnd(2);
					  ++io, ++lotIndex )
					UploadCSOrderForRuleCheck(rset,strStatus,heatIndex,(*ih),2,lotIndex,(*io));
			}
		}
	}
	catch ( CDBException* e )
	{

	   AfxMessageBox( "Unable to upload for rules check due to database error: \n" + e->m_strError,   
                      MB_ICONEXCLAMATION );
	   e->Delete();

	   dbc.GetDB()->Rollback();

	   return false;
	}

	if ( rset.IsOpen() )
		rset.Close();

	return true;
}
#endif

// get rid of this, then get rid of CRuleCheckLineupSet.

// does this look familiar
#ifdef DEPRECATED
void CCastString::UploadCSOrderForRuleCheck(CRulesCheckLineupSet& rSet,
											CString& statusToUse,
											int heatIndex,
											CCastStringHeat& rHeat,
											int strandNum,
											int lotIndex,
											CCSOrder* pOrder)
{
	char buf[30];

	rSet.AddNew();

	// Plan week encoded as YYYYWW
	rSet.m_PLAN_WEEK
		= Id().Year()*100 + Id().Week();

	rSet.m_PLAN_CAST_UNIT_CODE
		= _itoa(Id().Caster(),buf,10);
	
	// ??? I don't know how these are assigned
	rSet.m_PLAN_CAST_SEQUENCE_NO = 0;

	rSet.m_PLAN_STRING_ID
		= Id().StringId();

	// ??? I suppose I can compute this, but don't yet do so.
	rSet.m_PLAN_HEAT_GROUP = 1;

	// Numbering is 1-based in the database, but we are 0-based internally.
	rSet.m_PLAN_HEAT_SEQUENCE_NO
		= heatIndex + 1;

	rSet.m_PLAN_HEAT_TAB_NUMBER
		= rHeat.TabNum();

	//  first five digits
	rSet.m_PLAN_HEAT_SPEC
		= atol(rHeat.Spec().Left(5));

	//  What it says
	rSet.m_PLAN_HEAT_SIXTH_DIGIT_SPEC
		= rHeat.Spec().Mid(5,1);

	//  What it says
	rSet.m_PLAN_HEAT_SEVENTH_DIGIT_SPEC
		= atol(rHeat.Spec().Right(1));

	rSet.m_PLAN_STEEL_EARLY_START_DATE = 0;
	rSet.m_PLAN_STEEL_EARLY_START_TIME = 0;

	rHeat.ComputePlannedLatestStart(rSet.m_PLAN_STEEL_LATEST_END_DATE,
									rSet.m_PLAN_STEEL_LATEST_END_TIME);
	rSet.m_PLAN_STEEL_LATEST_END_TIME /= 100;

	rSet.m_PLAN_STEEL_LATEST_END_TIME = max(0,min(rSet.m_PLAN_STEEL_LATEST_END_TIME,9999));
	rSet.m_PLAN_STEEL_LATEST_END_DATE = max(0,min(rSet.m_PLAN_STEEL_LATEST_END_DATE,99999999));


	//rSet.m_PLAN_STEEL_LATEST_END_DATE  = 0;
	//rSet.m_PLAN_STEEL_LATEST_END_TIME  = 0;

	//  It was suggested that we use my figure.
	rSet.m_PLAN_HEAT_PROD_TIME_MINUTES = rHeat.Duration().GetTotalMinutes();

	rSet.m_PLAN_HEAT_DISPOSITION_CODE = rHeat.DispCode();

	// ??? No idea.
	rSet.m_PLAN_HEAT_BACKUP_MOLD_CODE = 0;

	rSet.m_PLAN_HEAT_CARBON_MIN
		= rHeat.MinChem(CChem::ELT_C);

	rSet.m_PLAN_HEAT_CARBON_MAX
		= rHeat.MaxChem(CChem::ELT_C);

	rSet.m_PLAN_HEAT_MANGANESE_MIN
		= rHeat.MinChem(CChem::ELT_MN);

	rSet.m_PLAN_HEAT_MANGANESE_MAX
		= rHeat.MaxChem(CChem::ELT_MN);

	//  910 -> 5
	//  marked for handoff -> 3
	//  909 -> 1
	rSet.m_PLAN_HEAT_STATUS_CODE = statusToUse;

	//  ???  Who assigns this number?
	rSet.m_PLAN_LINEUP_NUMBER = 0;

	//  ???  Who assigns this number?
	//       How is it different from PLAN_HEAT_SEQUENCE_NO?
	rSet.m_PLAN_LINEUP_ITEM_SEQ_NO = 0;

	// I have per heat comments
	rSet.m_PLAN_LINEUP_COMMENTS
		= rHeat.Comment().GetLength() > 80 
		  ? rHeat.Comment().Left(80)
		  : rHeat.Comment();
	
	// ??? I have no idea what this should be
	rSet.m_PLAN_910_REPORT_IND = " ";

	//  This is a PE Element
	//rSet.m_PLAN_LOT_INFORMATION

	// Here are the lot level fields

	//  This field is 1-based in the db, 0-based internally.
	rSet.m_PLAN_LOT_NUMBER
		= lotIndex + 1;

	rSet.m_MILL_ORDER_NO
		= pOrder->OrderNum();

	rSet.m_PLAN_STEEL_WIDTH
		= pOrder->SlabWidth();

	rSet.m_PLAN_STEEL_THICKNESS
		= pOrder->SlabThickness();

	rSet.m_PLAN_STEEL_LENGTH
		= pOrder->SlabLength();

	rSet.m_PLAN_STEEL_WGT_TONS
		= pOrder->Tons();

	rSet.m_PLAN_NUMBER_OF_PIECES
		= pOrder->NumPieces();

	rSet.m_PLAN_LOT_SPEC
		= pOrder->LotSpec();

	rSet.m_PLAN_LOT_DISP_CODE
		= pOrder->OutputDispCode();

	rSet.m_PLAN_LOT_TYPE_SLIT_CODE
		= pOrder->SlitTypeCode();

	//  ??? Where does this come from?
	rSet.m_PLAN_LOT_TYPE_SLAB = "";

	rSet.m_PLAN_SLAB_CONDITION_CD
		= pOrder->OutputCondnCode();

	//  ??? wher does this come from?
	rSet.m_PLAN_EXPOSURE_CODE 
		= pOrder->ExposureCode();

	//  ??? I think it is okay to hardwire this.
	rSet.m_PLAN_HOT_MILL_CD
		= 8;

	rSet.m_PLAN_CASTER_STRAND_NO
		= _itoa(strandNum,buf,10);

	// ???  For now, just blank
	rSet.m_PLAN_CHANGE_INDICATOR = " ";

	// ???  If I'm not really tracking this ...
	rSet.m_PLAN_ORIGINAL_PLAN_ID = "";

	// from smp and mord
	rSet.m_PLAN_EXPECTED_APPLIED_SPEC 
		= (pOrder->Order() == 0 
			? pOrder->LotSpec() 
			: pOrder->Order()->CastSpec());
	rSet.m_PLAN_EXPECTED_APPLIED_COND_CD 
		= (pOrder->Order() == 0
			? pOrder->SlabCondnCode()
			: pOrder->Order()->SlabCondnCode());

	// ??? what should this be?
	rSet.m_PLAN_909_LOCK_IND	= "";

	// ??? Where do these come from? the order?
	rSet.m_PLAN_STEEL_LENGTH_MIN = pOrder->PlanSteelLengthMin();
	rSet.m_PLAN_STEEL_LENGTH_MAX = pOrder->PlanSteelLengthMax();


	rSet.Update();
}
#endif
