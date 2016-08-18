// CastStringHeat.cpp: implementation of the CCastStringHeat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "CastStringHeat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "DalyPlanSet.h"
#include "SSBOutput.h"
#include "CasterStringHeatsSet.h"
#include "Spec.h"
#include "Snapshot.h"
#include "SpecMgr.h"


/*

Each heat carries a status code.
Possible values are 

	HEAT_STATUS_NEW
	HEAT_STATUS_909
	HEAT_STATUS_910

These values correspond to designations in the original DPA system.

HEAT_STATUS_NEW		-> not yet published to the outside world
HEAT_STATUS_909		-> thinking about these orders for the future
HEAT_STATUS_910		-> locked and loaded (downloaded to the casters)

When a heat is first created, its status is _NEW.
Heats can marked for handoff, typically in the HeatDetailsDlg.
They can be unmarked again here.
Once the heat is 910, it _can_ be returned to 909 status.

We have an additional bool status called isMarked which indicates
whether the heat is marked or not.  Marked heats are ones that
will go through a status change.

Strings have a status which is determined by the status
of the heats in the string. Values are

		STRING_STATUS_910
        STRING_STATUS_909
		STRING_STATUS_NEW

The string will have STRING_STATUS_NEW if all heats on 
the string have status HEAT_STATUS_NEW and none are marked.
If any heat has status HEAT_STATUS_910, then the string 
will have status STRING_STATUS_910.
Otherwise it will have STRING_STATUS_909.

When a string has status STRING_STATUS_910 or _909, 
  then it must have heats locked.

*/



////////////////////////////////////////////////////////////////
//	
//	C-tors, d-tor
//	
////////////////////////////////////////////////////////////////


CCastStringHeat::CCastStringHeat()
: m_duration		( 0						),
  m_relStartTime	( 0						),
  m_relFinishTime	( 0						),
  m_heatTons		( 0						),
  m_tabNum			( 0						),
  m_origTabNum		( 0						),
  m_condnCode		( 0						),
  m_dispCode		( 1						),
  m_pSpec			( 0						),
  m_caster			( 0						), //### caster-specific, init to 0 (not a good index)
  m_isMarked		( false					),
  m_signifEl		( CChem::Element(-1)	),
  m_status			( HEAT_STATUS_NEW		)
{
	// note: cannot initialize strandBegins/strandEnds at this time

	ResetChems();
}


CCastStringHeat::CCastStringHeat(CCasterStringHeatsSet& rHeatsSet,
					CCastString* pCastString)
: m_duration		( 0						),
  m_relStartTime	( 0						),
  m_relFinishTime	( 0						),
  m_heatTons		( 0						),
  m_tabNum			( 0						),
  m_origTabNum		( 0						),
  m_condnCode		( 0						),
  m_dispCode		( 1						),
  m_pSpec			( 0						),
  m_caster			( 0						), //### caster-specific (see above)
  m_isMarked		( false					),
  m_signifEl		( CChem::Element(-1)	),
  m_status			( HEAT_STATUS_NEW		)
{
	ResetChems();
	Init(rHeatsSet,pCastString);
}



////////////////////////////////////////////////////////////////
//	
//	Operations
//	
////////////////////////////////////////////////////////////////


//
//  Initialize from P_STLMAKE_DALY_PLAN
//

void CCastStringHeat::Init(int caster, //### caster-specific
						   CDalyPlanSet& planSet)
{
	m_caster = caster; //### caster-specific

	m_spec.Format("%05d%c%d",
		            planSet.m_PLAN_HEAT_SPEC,
					planSet.m_PLAN_HEAT_SIXTH_DIGIT_SPEC[0],
					planSet.m_PLAN_HEAT_SEVENTH_DIGIT_SPEC);

	SetSpecPtr(m_caster); //### caster-specific

	m_minChems[CChem::ELT_C ] = planSet.m_PLAN_HEAT_CARBON_MIN;
	m_maxChems[CChem::ELT_C ] = planSet.m_PLAN_HEAT_CARBON_MAX;
	m_minChems[CChem::ELT_MN] = planSet.m_PLAN_HEAT_MANGANESE_MIN;
	m_maxChems[CChem::ELT_MN] = planSet.m_PLAN_HEAT_MANGANESE_MAX;
	m_aimChems[CChem::ELT_C ] = ( m_minChems[CChem::ELT_C ]  > 0.0
		                          && 
								  m_maxChems[CChem::ELT_C ] < 100.0
								  &&
								  m_maxChems[CChem::ELT_C ] > 0.0 
								  ? (m_minChems[CChem::ELT_C ] + m_maxChems[CChem::ELT_C ])/2
								  : 0.0);
	m_aimChems[CChem::ELT_MN] = ( m_minChems[CChem::ELT_MN] > 0.0
		                          && 
								  m_maxChems[CChem::ELT_MN] < 100.0
								  &&
								  m_maxChems[CChem::ELT_MN] > 0.0 
								  ? (m_minChems[CChem::ELT_MN] + m_maxChems[CChem::ELT_MN])/2
								  : 0.0);


	m_comment		= planSet.m_PLAN_LINEUP_COMMENTS;

	m_dpaDuration = CTimeSpan(0,0,planSet.m_PLAN_HEAT_PROD_TIME_MINUTES,0);

	m_tabNum		= planSet.m_PLAN_HEAT_TAB_NUMBER;
	m_origTabNum	= m_tabNum;

	m_status = ( planSet.m_PLAN_HEAT_STATUS_CODE < "4" 
	  	         ? HEAT_STATUS_909
			     : HEAT_STATUS_910 );

	m_dispCode = planSet.m_PLAN_HEAT_DISPOSITION_CODE; 
}




//
//  Initialize from the results of the single string builder
//

void CCastStringHeat::Init(int caster, //### caster-specific
						   CSSBOutput& sbSet)
{
	m_caster = caster; //### caster-specific

	// Specs and chems should be set by the user.


	//m_spec.Format("%05d%c%d",
	//	            sbSet.m_PLAN_HEAT_SPEC,
	//				sbSet.m_PLAN_HEAT_SIXTH_DIGIT_SPEC[0],
	//				sbSet.m_PLAN_HEAT_SEVENTH_DIGIT_SPEC);

	//SetSpecPtr(m_caster);


	m_tabNum		= sbSet.m_PLAN_HEAT_TAB_NUMBER;
	m_origTabNum	= m_tabNum;

	m_status = HEAT_STATUS_909;

	//ostrstream ostr;
	//SetChemsFromHeatSpec(ostr,sbSet.m_PLAN_HEAT_SEQUENCE_NO-1,caster);
}



//
//  Initialize from archive
//

void CCastStringHeat::Init(CCasterStringHeatsSet& rHeatsSet,
						   CCastString* pCastString)
{
	TabNum(rHeatsSet.m_TabNum);

	CondnCode(rHeatsSet.m_CondnCode);
	DispCode(rHeatsSet.m_DispCode);
	IsMarked(rHeatsSet.m_IsMarked);

	SetSpec(rHeatsSet.m_Spec,
		    rHeatsSet.m_StringCaster);

	Status( rHeatsSet.m_Status == "910"
		    ? HEAT_STATUS_910
			: rHeatsSet.m_Status == "909"
			? HEAT_STATUS_909
			: HEAT_STATUS_NEW);

	Comment(rHeatsSet.m_Comment);
}


//
//  When we set the spec name,
//    look up the CSpec object and save a pointer to it.
//

void CCastStringHeat::SetSpec(CString newVal,
							  int caster) //### caster-specific
{
	m_spec = newVal; 
	if ( m_spec == "" ) 
		NullifySpecPtr();
	else
		SetSpecPtr(caster); //### caster-specific
}


//
//  Find the CSpec object corresponding to the current value
//    of the spec name.
//  Set the signifEl to match.
//

void CCastStringHeat::SetSpecPtr(int caster) //### caster-specific
{
	//m_pSpec = TheSnapshot.SpecMgr().FindSpec(m_spec,caster);
	m_pSpec = TheSnapshot.SpecMgr().FindSpecMaybe(m_spec,caster); //### caster-specific

	if ( m_pSpec == 0 )
		m_signifEl = CChem::Element(-1);
	else
		m_signifEl = m_pSpec->FindSignifEl();
}



//
//  Open up to no restriction
//

void CCastStringHeat::ResetChems()
{	
	for ( int elt = 0; elt < CChem::NumElements; ++elt ) {
		m_minChems[elt] = 0.0;
		m_aimChems[elt] = 0.0;
		m_maxChems[elt] = 100.0;
	}
}




//
//  A little snippet of code to take care of rounding
//    the chemistries into the 4th decimal digit.
//
#define FIX_CHEM(c) (long(10000*(c+0.00005))/10000.0)


//
//  Set the chemistry ranges on a heat from the heat spec.
//  Messages regarding the status of this operation are printed to the stream.
//  The spec pointer should be set, else a message is printed.
//  Otherwise, set the range for an element to what is given on the spec
//    and fix aim chems to match.

bool CCastStringHeat::SetChemsFromHeatSpec(ostrstream& ostr, int heatNum, int caster) //### caster-specific
{
	if ( m_pSpec == 0 ) {
		ostr << "No spec set for heat # " << heatNum << "\n";
		return false;
	}

	for ( int elt=0; elt<CChem::NumElements; ++elt ) {

		const CChemRange* pRange = m_pSpec->GetChemRange(CChem::Element(elt));

		if ( pRange != 0 ) {
				
			CChem::Bound specMin = pRange->MinVal();
			CChem::Bound specMax = pRange->MaxVal();
			CChem::Bound specAim = pRange->AimVal();
		
			m_minChems[elt] = specMin;
			m_maxChems[elt] = specMax;
			m_aimChems[elt] = specAim;

			FixAimChem(pRange,CChem::Element(elt),caster); //### caster-specific

#ifdef _DEBUG
			afxDump << "Setting heat chems: "
					<< CChem::ToString( CChem::Element(elt) )
					<< " -- "
					<< FIX_CHEM(m_minChems[elt])
					<< " "
					<< FIX_CHEM(m_aimChems[elt])
					<< " "
					<< FIX_CHEM(m_maxChems[elt])
					<< "\n";
#endif
		}
	}
	
	return true;
}


//
//  Fix aim chem for all elements
//

bool CCastStringHeat::FixAimChems(ostrstream& ostr, 
								  int heatNum, 
								  int caster) //### caster-specific
{
	if ( m_pSpec == 0 ) {
		// already we would have added the error message.
		//ostr << "No spec set for heat # " << heatNum << "\n";
		return false;
	}

	for ( int elt=0; elt<CChem::NumElements; ++elt ) 
		FixAimChem( CChem::Element(elt), caster ); //### caster-specific
	
	return true;	
}



//
//  Fix aim chem for a given element.
//  Find the range and hand off.
//

bool CCastStringHeat::FixAimChem(CChem::Element elt, 
								 int caster) //### caster-specific
{
	if ( m_pSpec == 0 )
		return false;

	const CChemRange* pRange = m_pSpec->GetChemRange(elt);


	return FixAimChem(pRange,elt,caster); //### caster-specific

}


//
//  Fix aim chem for a given element 
//    given the range object from the spec for that element.
//
//  The aim chem will have already been set from the range element.
//  That value may need adjusting.  Specifically, 
//     case 1) if on casters 2 or 3, 
//             and the element is Sulfur.
//             and min is 0 and max is non-trivial (not 0, not 100)
//			   and aim is 0
//     case 2) not ( on casters 2,3, element Sulfur)
//             and min > 0 and max is non-trivial
//			   and aim is 0
//### logic needs to be expanded for casters 4 and 5
//
//  in case 1), aim = max - 0.001
//  in case 2), aim = average(min,max)
//
    
// CASTER TODO: WHat is correct for casters 4,5
// CASTER TODO: when we find out, firx caster!=1 to be caster==2 or 3, and whatever for 4,5
bool CCastStringHeat::FixAimChem(const CChemRange* pRange,
								 CChem::Element elt, 
								 int caster) //### caster-specific
{
	if ( pRange != 0 ) {
			
		CChem::Bound specMin = pRange->MinVal();
		CChem::Bound specMax = pRange->MaxVal();
		CChem::Bound specAim = pRange->AimVal();
	

		bool computeAim = (caster != 1 && elt == CChem::ELT_S  //### caster-specific
				           ? (m_minChems[elt] == 0
								&&
								m_maxChems[elt] > 0 
								&& 
								m_maxChems[elt] < 100.0
								&&
								specAim == 0.0)
							:(m_minChems[elt] > 0
								&&
								m_maxChems[elt] > 0 
								&& 
								m_maxChems[elt] < 100.0
								&&
								specAim == 0.0)	);

		if ( computeAim ) {

			if ( caster != 1 && elt == CChem::ELT_S ) //### caster-specific
				m_aimChems[elt] = m_maxChems[elt] - 0.001;
			else
				m_aimChems[elt] = (m_minChems[elt] + m_maxChems[elt])/2.0;
		}
	}

	return true;
}



//
// Relationship of order chemistries to heat chemistries
// From a memo from Carl Neis dated June 24, 1999.
//
//
//  A)	Straight Heat Chemistry (No '05's and no open chemistry ranges
//		All of the elemental chemistries are to be drawn from the "Green Book Table".
//		
//	B)	Heat Chemistry with an '05' and no open chemistry range
//		This is to be handled the same as Scenario A.
//		The QDE's change the chemistries in the "Green Book Table" to reflect
//			the required modified chemistry.
//		The '05' chmistry should only be pulled in if the heat chemistry is defined
//			as an '05'.
//
//	C)	Heat Chemistry with an '05' and Open Chemistry Range(s)
//		The base chmistry is to be pulled in from the "Green Book Table".
//		The open range chemistry (asterisked *) minimum/maximums are the only 
//			chemistries for the defined heat that are to be pulled from the orders
//			and defined to a specific heat being evaluated.
//		In addition, the chemistries (calculated/defined) for the open range chemistries
//			are only to be defined within the min./max. parameters defined by the
//			"Green Book Table".
//		If the chemistry range is < or > the "Green Book Table" then the order must be '05'd.
//
//	D)	Heat Chemistry Open Chemistry Range(s) and No '05'
//		Same as C)
//

bool CCastStringHeat::IntersectChems( const CCSOrder* pOrder, vector<bool>& badChems )
{
	if ( m_pSpec == 0 ) {
		// already we would have added the error message.
		//ostr << "No spec set for heat # " << heatNum << "\n";
		return false;
	}

	bool is05Spec = m_pSpec->Name().GetLength() >= 7
					&&
					m_pSpec->Name()[6] == '5';

	bool isOk = true;

	badChems.clear();
	badChems.resize(CChem::NumElements,false);

	for ( int elt=0; elt<CChem::NumElements; ++elt ) {
		
//		afxDump << "Intersect: "
//				<< (void*)this
//				<< " "
//				<< pOrder->OrderNum()
//				<< " MO = "
//				<< FIX_CHEM(m_minChems[CChem::ELT_MO])
//				<< "/"
//				<< FIX_CHEM(m_maxChems[CChem::ELT_MO])
//				<< "\n";
//		afxDump.Flush();

		const CChemRange* pRange = m_pSpec->GetChemRange(CChem::Element(elt));
		bool isAppSpecific = pRange != 0 && pRange->IsAppSpecific();

		CChem::Bound orderMin;
		CChem::Bound orderMax;
		bool hasReasonCode;
		
		bool nonTrivial = pOrder->GetChemRange(CChem::Element(elt),orderMin,orderMax,hasReasonCode);
		
		
		if ( nonTrivial ) {
			
			if ( isAppSpecific ) {
				// for appSpecific, need intersection of ranges
				CChem::Bound newMin = max(m_minChems[elt], orderMin);
				CChem::Bound newMax = min(m_maxChems[elt], orderMax);
				
				if ( newMax < newMin ) {
					isOk = false;
					badChems[elt] = true;
#ifdef _DEBUG
					afxDump << "Intersect: "
							// FP change
							//<< pOrder->OrderNum()
							<< LPCTSTR(pOrder->FpOrderNum())
							<< " "
							<< CChem::ToString( CChem::Element(elt) )
							<< " (open) - current:"
							<< FIX_CHEM(m_minChems[elt])
							<< "/"
							<< FIX_CHEM(m_maxChems[elt])
							<< ", order: "
							<< FIX_CHEM(orderMin)
							<< "/"
							<< FIX_CHEM(orderMax)
							<< " -- bad range!\n";
					afxDump.Flush();
#endif
				}
				else {
#ifdef _DEBUG
					afxDump << "Intersect: "
							// FP change
							//<< pOrder->OrderNum()
							<< LPCTSTR(pOrder->FpOrderNum())
							<< " "
							<< CChem::ToString( CChem::Element(elt) )
							<< " (open) - current:"
							<< FIX_CHEM(m_minChems[elt])
							<< "/"
							<< FIX_CHEM(m_maxChems[elt])
							<< ", order: "
							<< FIX_CHEM(orderMin)
							<< "/"
							<< FIX_CHEM(orderMax)
							<< ", new: "
							<< FIX_CHEM(newMin)
							<< "/"
							<< FIX_CHEM(newMax)
							<< "\n";
					afxDump.Flush();
#endif

					m_minChems[elt] = newMin;
					m_maxChems[elt] = newMax;
				}
			}
			else {	// not app specific
				// for not app specific, order range must contain spec range
				if ( m_minChems[elt] < orderMin
					||
					m_maxChems[elt] > orderMax ) {
					isOk = false;
					badChems[elt] = true;
#ifdef _DEBUG
					afxDump << "Intersect: "
							// FP change
							//<< pOrder->OrderNum()
							<< LPCTSTR(pOrder->FpOrderNum())
							<< " "
							<< CChem::ToString( CChem::Element(elt) )
							<< " (closed) - current:"
							<< FIX_CHEM(m_minChems[elt])
							<< "/"
							<< FIX_CHEM(m_maxChems[elt])
							<< ", order: "
							<< FIX_CHEM(orderMin)
							<< "/"
							<< FIX_CHEM(orderMax)
							<< " -- bad range!\n";
					afxDump.Flush();
#endif
				}
			}
		} // nonTrivial
		

#if 0
		// TODO: Need a definitive answer as to whether we need to consider this

		// spec chems (LotSpec)
		if ( pOrder->PtrLotSpec() != 0 ) {

			const CChemRange* pRange = pOrder->PtrLotSpec()->GetChemRange(CChem::Element(elt));

			if ( pRange != 0 ) {

				CChem::Bound specMin = pRange->MinVal();
				CChem::Bound specMax = pRange->MaxVal();

				bool nonTrivial = (specMin != 0.0 || specMax != 100.0);
				if ( nonTrivial ) {
					
					if ( pRange->IsAppSpecific() ) {

						// for appSpecific, need intersection of ranges

						CChem::Bound newMin = max(m_minChems[elt], specMin);
						CChem::Bound newMax = min(m_maxChems[elt], specMax);

						if ( newMax < newMin ) {
							isOk = false;
							badChems[elt] = true;
						}
						else {
							m_minChems[elt] = newMin;
							m_maxChems[elt] = newMax;
						}
					}
					else {
						// for not app specific, order range must contain spec range
						if ( m_minChems[elt] < specMin
							||
							m_maxChems[elt] > specMax ) {
							isOk = false;
							badChems[elt] = true;
						}
					}
				}
			}
		}
#endif // lotSpec
	}

	return isOk;
}


//
//  Used to determine if two heats match.
//  Used to compare backup copy with current copy to see if changes have occurred,
//     requiring archiving.
//

bool CCastStringHeat::IsSameHeat( const CCastStringHeat& rHeat ) const
{
	if ( m_condnCode != rHeat.m_condnCode
		 ||
		 m_dispCode  != rHeat.m_dispCode
		 ||
		 m_tabNum    != rHeat.m_tabNum
		 ||
		 m_spec      != rHeat.m_spec 
		 ||
		 m_strandEnds[1] - m_strandBegins[1] != rHeat.m_strandEnds[1] - rHeat.m_strandBegins[1]
		 ||
		 m_strandEnds[2] - m_strandBegins[2] != rHeat.m_strandEnds[2] - rHeat.m_strandBegins[2] )
		 
		 return false;

	for ( int elt=0; elt<CChem::NumElements; ++elt ) {

		if ( m_minChems[elt] != rHeat.m_minChems[elt]
			 ||
			 m_maxChems[elt] != rHeat.m_maxChems[elt] )
	
			 return false;
	}


	vector<CCSOrder*>::const_iterator io1;
	vector<CCSOrder*>::const_iterator io2;

	for ( io1 = m_strandBegins[1], io2 = rHeat.m_strandBegins[1];
	      io1 != m_strandEnds[1];
		  ++io1, ++io2 ) {

		if ( ! (*io1)->IsSameLot( (*io2) ) )

			return false;
	}

	for ( io1 = m_strandBegins[2], io2 = rHeat.m_strandBegins[2];
	      io1 != m_strandEnds[2];
		  ++io1, ++io2 ) {

		if ( ! (*io1)->IsSameLot( (*io2) ) )

			return false;
	}


	return true;
}


//
//  Compute the number of tons of the heat that would run on each strand,
//    given the initial widths on each strand.
//  If we don't have a width on one of the strands, set that strand's tons to 0.

void CCastStringHeat::ComputeApproximateStrandBalance(int aimHeatSize, 
													  int& rStrand1Tons, 
													  int& rStrand2Tons) const
{
	rStrand1Tons = 0;
	rStrand2Tons = 0;

	double width1 = 0;
	double width2 = 0;

	if ( m_strandEnds[1] != m_strandBegins[1] ) 
		width1 = (*(m_strandBegins[1]))->SlabWidth();

	if ( m_strandEnds[2] != m_strandBegins[2] ) 
		width2 = (*(m_strandBegins[2]))->SlabWidth();

	if ( width1 == 0 && width2 == 0 ) {
		// do nothing
	}
	else if ( width2 == 0 )
		rStrand1Tons = aimHeatSize;
	else if ( width1 == 0 )
		rStrand2Tons = aimHeatSize;
	else {
		rStrand1Tons = long( double(aimHeatSize * width1) / ( width1 + width2 ) + 0.5 );
		rStrand2Tons = aimHeatSize - rStrand1Tons;
	}
}



//
//  Compute the planned latest start date and time for the heat.
//  This is the minimum of the planned latest start for the orders on the heat.
// 

void CCastStringHeat::ComputePlannedLatestStart(long& startDate, long& startTime)
{
	startDate = 0;
	startTime = 0;
	bool foundOne = false;

	{
		for ( vector<CCSOrder*>::iterator io1 = m_strandBegins[1];
			  io1 != m_strandEnds[1];
			  ++io1 ) {

			if ( (*io1)->Order() != 0 ) {

				if ( ! foundOne && (*io1)->Order()->PlannedLatestStartDate() != 0 ) {
					startDate = (*io1)->Order()->PlannedLatestStartDate();
					startTime = (*io1)->Order()->PlannedLatestStartTime();
					foundOne = true;
				}
				else if ( (*io1)->Order()->PlannedLatestStartDate() != 0  ) {

					if ( (*io1)->Order()->PlannedLatestStartDate() < startDate
						 ||
						 ((*io1)->Order()->PlannedLatestStartDate() == startDate
						  &&
						  (*io1)->Order()->PlannedLatestStartTime() < startTime ) ) {
					   startDate = (*io1)->Order()->PlannedLatestStartDate();
					   startTime = (*io1)->Order()->PlannedLatestStartTime();
					}
				}
			}
		}
	}

	{
		for ( vector<CCSOrder*>::iterator io2= m_strandBegins[2];
			  io2 != m_strandEnds[2];
			  ++io2 ) {

			if ( (*io2)->Order() != 0 ) {

				if ( ! foundOne && (*io2)->Order()->PlannedLatestStartDate() != 0 ) {
					startDate = (*io2)->Order()->PlannedLatestStartDate();
					startTime = (*io2)->Order()->PlannedLatestStartTime();
					foundOne = true;
				}
				else if ( (*io2)->Order()->PlannedLatestStartDate() != 0 ){

					if ( (*io2)->Order()->PlannedLatestStartDate() < startDate
						 ||
						 ((*io2)->Order()->PlannedLatestStartDate() == startDate
						  &&
						  (*io2)->Order()->PlannedLatestStartTime() < startTime ) ) {
					   startDate = (*io2)->Order()->PlannedLatestStartDate();
					   startTime = (*io2)->Order()->PlannedLatestStartTime();
					}
				}
			}
		}
	}
}

