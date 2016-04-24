////////////////////////////////////////////////////////////////
//	
//	CCastStringSummaryRecord
//	
////////////////////////////////////////////////////////////////


#include "StdAfx.h"

#include "CastStringSummaryRecord.h"

#include "CSOrder.h"
#include "Order.h"
#include "CastString.h"
#include "OrderSelection.h"




CCastStringSummaryRecord::CCastStringSummaryRecord()
{
	Init();
}


void CCastStringSummaryRecord::Init()
{
	m_tons				= 0;
	m_numPieces			= 0;

	m_numRadicals			= 0;

	m_numCMS				= 0;
	m_numCMSFlips			= 0;
	m_numFlips			= 0;

	m_numSlits			= 0;
	m_numSplits			= 0;

	m_numStockPieces		= 0;

	m_numCurrentHRPieces	= 0;
	m_numBackHRPieces		= 0;
	m_numFutureHRPieces	= 0;

	m_avgWidth			= 0;

	m_duration			= 0;

	// FP Change
	m_tonsManual   = 0.0;
	m_tonsComputer = 0.0;
	m_tonsNonprio  = 0.0;

	m_luTypeTonsMap.insert(HsmLuTypeToTonsMap::value_type("",0));
	m_luTypeTonsMap.clear();

	m_ciCodeTonsMap.insert(CICodeToTonsMap::value_type(0,0));
	m_ciCodeTonsMap.clear();
}


void CCastStringSummaryRecord::AddLuTypeTons(const CString& luType,
											 Weight tons)
{
	HsmLuTypeToTonsMap::iterator im = m_luTypeTonsMap.find(luType);

	Weight curVal = 0.0;
	
	if ( im != m_luTypeTonsMap.end() ) {
		curVal = (*im).second;
		m_luTypeTonsMap.erase(im);
	}
	
	int newVal = curVal + tons;

	m_luTypeTonsMap.insert(HsmLuTypeToTonsMap::value_type(luType,newVal));
}


void CCastStringSummaryRecord::AddCICodeTons(int ciCode,
											 Weight tons)
{
	CICodeToTonsMap::iterator im = m_ciCodeTonsMap.find(ciCode);

	Weight curVal = 0.0;
	
	if ( im != m_ciCodeTonsMap.end() ) {
		curVal = (*im).second;
		m_ciCodeTonsMap.erase(im);
	}
	
	int newVal = curVal + tons;

	m_ciCodeTonsMap.insert(CICodeToTonsMap::value_type(ciCode,newVal));
}



void CCastStringSummaryRecord::Sum(const CCastStringSummaryRecord& x)
{
	// easier to do avgWidth calcn first

	if ( m_numPieces + x.m_numPieces  > 0 )
		m_avgWidth = (m_avgWidth*m_numPieces + x.m_avgWidth*x.m_numPieces)
					  /
					  (m_numPieces + x.m_numPieces);

	m_tons					+= x.m_tons;
	m_numPieces				+= x.m_numPieces;

	m_numRadicals			+= x.m_numRadicals;

	m_numCMS				+= x.m_numCMS;
	m_numCMSFlips			+= x.m_numCMSFlips;
	m_numFlips				+= x.m_numFlips;

	m_numSlits				+= x.m_numSlits;
	m_numSplits				+= x.m_numSplits;

	m_numStockPieces		+= x.m_numStockPieces;

	m_numCurrentHRPieces	+= x.m_numCurrentHRPieces;
	m_numBackHRPieces		+= x.m_numBackHRPieces;
	m_numFutureHRPieces		+= x.m_numFutureHRPieces;

	m_duration				+= x.m_duration;

	// FP CHange
	m_tonsManual			+= x.m_tonsManual;
	m_tonsComputer			+= x.m_tonsComputer;
	m_tonsNonprio			+= x.m_tonsNonprio;

	{
		for ( HsmLuTypeToTonsMap::const_iterator im = x.m_luTypeTonsMap.begin();
			  im != x.m_luTypeTonsMap.end();
			  ++im ) 
		
				AddLuTypeTons((*im).first,(*im).second);
	}

	{
		for ( CICodeToTonsMap::const_iterator im = x.m_ciCodeTonsMap.begin();
			  im != x.m_ciCodeTonsMap.end();
			  ++im ) 
			
				AddCICodeTons((*im).first,(*im).second);
	}

}



void  CCastStringSummaryRecord::AddCSOrder(CCSOrder* pCSOrder)
{
	AddCSOrder(pCSOrder,pCSOrder->NumPieces());
}


void CCastStringSummaryRecord::AddCSOrder(CCSOrder* pCSOrder, int numPcs)
{
	// generic calculations

	Weight tonsToUse = (pCSOrder->IsSplit() ? 2 : 1 )*numPcs*pCSOrder->PieceWgt()/2000;

	m_tons		+= tonsToUse;
	m_numPieces	+= numPcs;
	m_duration	+= pCSOrder->Duration();
	
	// Radical width jump

	if ( pCSOrder->IsRadical() )
		m_numRadicals += 1;

	// CMS/Flip calculation

	int dig1 = (pCSOrder->SlabCondnCode() / 1000) % 10;

	switch ( dig1 ) {
	case 5:
		m_numCMS		+= numPcs;
		break;

	case 8:
		m_numCMSFlips	+= numPcs;
		break;

	case 9:
		m_numFlips	+= numPcs;
		break;
	}



	//	Stock orders
	//	Slits/splits


	// FP change
//	if ( pCSOrder->OrderNum() == 9999999 )
	if ( pCSOrder->FpOrderNum().Left(7) == "9999999" )
		m_numStockPieces += numPcs;
	else if ( pCSOrder->IsSplit() )
		m_numSplits += numPcs;
	else if ( pCSOrder->IsSlit() )
		m_numSlits += numPcs;

	//	Current/Back/Future
	
	if ( pCSOrder->Order() != 0 ) {

		//CWeek currWk = pCSOrder->CastString()->OrderSelection().CurrentHrYrWk();
		//CWeek orderWk = pCSOrder->Order()->HrYrWk();

		//if ( orderWk == currWk )
		//	m_numCurrentHRPieces	+= numPcs;
		//else if ( orderWk < currWk )
		//	m_numBackHRPieces		+= numPcs;
		//else
		//	m_numFutureHRPieces	+= numPcs;

		AddLuTypeTons(pCSOrder->Order()->MegaLuCd(),tonsToUse);
		AddCICodeTons(pCSOrder->Order()->CICode(),tonsToUse);
	}


	// average width

	if ( m_numPieces > 0 ) {

		m_avgWidth = ( m_avgWidth * ( m_numPieces - numPcs ) // total width so far
						+ pCSOrder->SlabWidth()*numPcs ) // additional width
					/ m_numPieces;					     // average it
	}


	if ( pCSOrder->Order() != 0 ) {
		long tons = long(pCSOrder->Order()->OrderedTons());
		switch ( pCSOrder->Order()->FpIshroPrty() ) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			m_tonsManual += tons;
			break;
		case 10:
			m_tonsComputer += tons;
			break;
		default:
			m_tonsNonprio += tons;
			break;
		}
	}
}
