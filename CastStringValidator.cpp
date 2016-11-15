// CastStringValidator.cpp: implementation of the CCastStringValidator class.
//
// Program: R0341070 (DPA combination) Mark / Validate Tab for Handoff 
//
// Description: This prog validate tabs that are marked for handoff to caster, 
//              and establishes some validation warnings. 
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
// K. Hubbard     01/28/03   Added logic to send slabs to HH or LL Yard
//                           to support 7Blast Furnace reline. This is done 
//                           by assigning dispcode = 2 for non CMS material 
//                           if CI code is 43 or 44.   per T. Fronczek
//                           pgms involved: CSOrder.h and CastStringValidator.cpp         
//
// K. Hubbard     08/05/03   Added logic to prevent ordered condition code orders xx34 
//                           from being planned for slitting. per Larry Thomas 
//
// K. Hubbard     12/11/03   Add New Fatal block to check plan width against (*io)->ProvSlabWidthMax
//                           for non slit, non Transition see (audit non Transition Provided Steel widths).
//                           Also changed warning to fatal on (audit steel width) for exceeding 
//                           1" allowance transition when compared to previous slab. per P. Fronczek and P. Velasco. 
//
// K. Hubbard     06/30/04   Changed #3Combi startup length from 315 to 362 per new cast rules and P. Fronczek. 
//
// K. Hubbard     07/01/04   Commented out low quality 4682 & 4782 condition code exception checks and heat spec checks
//                           for 3 Combi Caster startup length 362" verification. per P. Fronczek.   
//
// K. Hubbard     11/11/04   Temp switched length boundary validation from Fatal to Warning for Dofasco
//                           order campaign per P. Fronczek. 					                              
//
// K. Hubbard     05/4/05    Changed flip disp code 2 to represent Slab Customer Sales
//                           logic. per P. Velasco
//
// K. Hubbard     05/4/05    Added exception of disposition code 4 to avoid 1212 and plan condition code assignment logic 
//                           in UpdateCondnCode per P. Fronczek.
//
// K. Hubbard     06/29/05   Added noslit adjustment on xxx4 condition codes to receive a slit type of 'D'
//                           and disp code = 4 logic. per P. Fronczek
//
// K. Hubbard     09/08/05   Commented out shop specification for Flips which now occur at 2&4 BOF on 9xxx condition codes 
//                           and receive a disposition code of '1'
//
// K. Hubbard     11/21/05   Commented out assignment of 9282 planned condition code in order to integrate with P.A. (K.Wert) flip 
//                           logic that sends 931 Slab Status codes to Q.A. system. per P. Fronczek & T. Lara  
//
// K. Hubbard     11/29/05   Modified and added width jump logic to conform with 80" Spread / Squeeze facility limits. These limits  
//                           are examined on transitions of 1" slab differences, on hot roll products (ci codes 65 or 66). To establish a 
//                           minimum (squeeze) hotband limit, we subtract 1.6 from previous slab width. To establish a max (spread) hotband 
//                           limit, we add 0.3 to the previous slab width. Next we determine if the current 1" narrower slab's ordered  
//                           hotband width is within these min / max limits, if "yes" we allow the slab width jump, if "no" we assign a FATAL error 
//                           message and do not permit lineup transmission. See changes dated 11-22-05 in Width Jump Audit area. per P. Velasco.     
//
// K. Hubbard     01/31/06   Added width jump audit logic to conform with 80" Spread / Squeeze facility limits. The max limit is examined  
//                           on transitions of 1" slab differences, on all ci code products. To establish a max 80" (spread) slab limit,
//                           we subtract 1" from (80" system / rules use 0.3) to the current slab's provided cast width. Next we determine if the 
//                           current slab's ordered width is within this min / max limits, if "yes" we allow the slab width jump, if "no" we assign
//                           a FATAL error 80" Spread Squeeze violation message and do not permit lineup transmission. See changes dated 1-31-06 in 
//                           80" Slab Spread Squeeze Width Jump Audit area. per P. Fronczek.     
//
// K. Hubbard     03/06/06   Added Tek and Kote (CI Codes 12,17,18,19,20,21,72,73) audit to prevent via "Fatal" message, that prohibits positional
//                           planning of these orders at the beginning of a cast. per P. Velasco.   
//
// K. Hubbard     03/08/06   Temporarily changed 01/31/06 to a Warning per P. Fronczek. 
//
// K. Hubbard     03/15/06   Modified width jump audit logic to conform with 80" Spread / Squeeze facility limits. The max limit is examined  
//                           on transitions of 1" slab differences, on all ci code products. To establish a max 80" (spread) slab limit,
//                           we subtract 1" from (80" system / rules use 0.3) to the current slab's provided cast width. Next we take the minimum slab 
//                           width of 2 consecutive lots and determine if the minimum width between the 2 lots is within this transitional min / max limits,                            a FATAL error 80" Spread Squeeze violation message and do not permit lineup transmission. See changes dated 3-15-06 in 
//                           if "yes" we allow the slab width jump, if "no" we assign a WARNING error 80" Slab Spread Squeeze Width Jump Audit area. 
//                           per P. Fronczek.     
//
// K. Hubbard     03/17/06   Added Tek and Kote (CI Codes 11,45-50) audit to prevent via "Fatal" message, that prohibits positional
//                           planning of these orders at the beginning of a cast. per P. Fronczek.   
//
// K. Hubbard     03/21/06   Implementation of minimum slab lengths as constrained by the physical limits of the steel shops (currently cold lengths are
//                           2BOF = 172" 4BOF = 220"). per T. Fronczek.   
//
// K. Hubbard     05/24/06   Added minimum slit allowance audit to prevent positional planning of width transitions below the cutting ability of the  
//                           burning machines. per P. Velasco.   
//
// K. Hubbard     05/30/06   Added 52442xx spec to the exception list of fourth digit modifications in the FixHeatSpec subroutine
//                           See comments (5/30/06 k. hubbard) per P. Fronczek.   
//
// K. Hubbard     01-24-07   Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers and release of 
//	                         CI code = 86 identification. New Slab Sale Cust identification method is MORD_HOTROLL_UNIT_NAME not equal to "80HR". 
//                           Used MFC Class Wizard's, Member Variables, Update Columns button linked to OrdersForDailyPlanOnly VIEW fields in newinl via 
//                           ODBC path. per T. Fronczek
//
// K. Hubbard     03/21/07   Added sixth digit validation of calcium range specs where the sixth digit of lot order spec in a single heat is 8 or 9, the 
//				             heat spec sixth digit must also be a 8 or 9 for compatibility 3-21-07 k. hubbard  
//
// K. Hubbard     06/29/07   Modified Tek and Kote (CI Codes 12,17,18,19,20,21,72,73) audit to warning per new 1st cast piece cropping business 
//                           quality plans for allowance of positional planning of these orders at the beginning of a cast. per P. Velasco.   
//
// K. Hubbard     09/28/07   Implementation of minimum slab lengths as 80% of max length for steelshop optimization modeling. Also commented out code of
//                           minimum lengths set by physical limits of the steel shops: 2BOF = 172" 4BOF = 220". per J. Sarb-Beer.   
//
// K. Hubbard     02/10/09   Modified minimum slit allowance audit preventing positional planning of width transitions below the cutting ability of the  
//                           burning machines, by comparing current lot width to the ordered aim width. per P. Velasco.   
//
// K. Hubbard     05-5-09    Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                           modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                           controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//
// K. Hubbard     01-28-10   Create audit for 2BOF inward width change designs, outward width changes permitted for 4BOF, but not allowed at 2BOF.
//
// K. Hubbard     02-04-10   Develop an audit for 2BOF steelshop facility length limitations. Also, set global length values in MiscConstants.cpp allowed at 2BOF
//                           432" for 2SC and 412" at 3Combi Caster.
//
// K. Hubbard     02-26-10   Added slit facility reason fields that are assigned during creation or modification of planned slits
//                           within the order parameter window. Also this includes addition of 1 new Combobox control.       
//
// K. Hubbard     01-22-16   Develop an audit for 2BOF steelshop facility length limitations. Also, set global length values in MiscConstants.cpp allowed at 2BOF
//                           440" for 2SC and 3Combi Caster.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "CastStringValidator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "CastString.h"
#include "CasterScen.h"
#include "SuperScen.h"
#include "Spec.h"
#include "SpecCmp.h"
#include "SpecMgr.h"
#include "MiscConstants.h"



//////////////////////////////////////////////////////////////////////
//
// Construction/Destruction
//
//////////////////////////////////////////////////////////////////////

CCastStringValidator::CCastStringValidator()
{
	m_pHeats = 0;
	m_pCastString = 0;
	m_pCasterScen = 0;
	m_pSuperScen = 0;
}


////////////////////////////////////////////////////////////////
//	
//	Adding errors
//	
////////////////////////////////////////////////////////////////


void CCastStringValidator::AddValidnError(int heatIndex,
	int strandNum,
	int lotIndex,
	const char* str,
	CCastStringHeatValidnError::E_Severity severity)
{
	CCastStringHeat* pHeat = (heatIndex == -1
		? 0
		: &(*m_pHeats)[heatIndex]);
	CString cstr(str);

	CCSOrder* pCSOrder = (pHeat != 0
		&&
		(strandNum == 1 || strandNum == 2)
		&&
		lotIndex != -1)
		? *(pHeat->StrandBegin(strandNum) + lotIndex)
		: 0;

	CCastStringHeatValidnError err(heatIndex,
		pHeat,
		strandNum,
		lotIndex,
		pCSOrder,
		str,
		severity);

	m_pCastString->ValidnErrors().push_back(err);
}



void CCastStringValidator::AddValidnError(int heatIndex,
	int strandNum,
	int lotIndex,
	ostrstream& ostr,
	CCastStringHeatValidnError::E_Severity severity)
{
	AddValidnError(heatIndex, strandNum, lotIndex, ostr.str(), severity);
	ostr.freeze(false);
	ostr.seekp(0);
}



const vector<CCastStringHeatValidnError>& CCastStringValidator::ValidnErrors() const
{
	return m_pCastString->ValidnErrors();
}



////////////////////////////////////////////////////////////////
//	
//	Main routine -- Validate()
//	
////////////////////////////////////////////////////////////////

bool CCastStringValidator::Validate(vector<CCastStringHeat>& heats,
	CCastString* pCastString,
	CCasterScen* pCasterScen,
	CSuperScen* pSuperScen)
{
	// we use instance variables to store these values
	// so we don't have to pass them everywhere.

	m_pHeats = &heats;
	m_pCastString = pCastString;
	m_pCasterScen = pCasterScen;
	m_pSuperScen = pSuperScen;

	// clear out the vector of errors, from previous validations
	m_pCastString->ValidnErrors().clear();

	// Assume valid
	bool isValid = true;

	UpdateLotSlabThicknesses();	//  Originally in UpdateCondAndDispCodes


	///////////////////////////////////////////////////////////////////////////
	// DPA Program R0341070
	// Make modifications to condition and disposition codes 
	{

		UpdateCondAndDispCodes();

		if (!Validate340080())
			isValid = false;

		if (!CheckCondAndDispCodeRelationships())
			isValid = false;

	}
	// end R0341070
	///////////////////////////////////////////////////////////////////////////


	// Check for duplicate tab nums
	if (!ValidateTabNums())
		isValid = false;

	// check for weight under/over
	if (!ValidateWeights())
		isValid = false;

	// check demand due
	if (!ValidateDemandDue())
		isValid = false;

	// check bad chems
	if (!ValidateChems())
		isValid = false;

	m_pCastString->SetIsValid(isValid);




	return isValid;
}




////////////////////////////////////////////////////////////////
//	
//	Updating condition and disposition codes
//
//	From DPA Program R0341070
//	
////////////////////////////////////////////////////////////////
//  Unlike the original DPA code, 
//    we have to run separate iterations for each strand.
//  We also do all marked heats at once, 
//	  so we need to iterate across the marked heats


//  All slabs must have a thickness of 9.25
//  Originally in UpdateCondAndDispCodes
void CCastStringValidator::UpdateLotSlabThicknesses()
{
	UpdateLotSlabThicknesses(1);
	UpdateLotSlabThicknesses(2);
}

void CCastStringValidator::UpdateLotSlabThicknesses(int strandNum)
{
	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);
	for (vector<CCSOrder*>::const_iterator io = strand.begin();
		io != strand.end();
		++io) {

		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];

		if (!rHeat.IsMarked())
			continue;

		if ((*io)->SlabThickness() != 9.25)
			(*io)->SlabThickness(9.25);
	}
}





void CCastStringValidator::UpdateCondAndDispCodes()
{
	UpdateCondAndDispCodes(1);
	UpdateCondAndDispCodes(2);

	for (vector<CCastStringHeat>::iterator ih = m_pHeats->begin();
		ih != m_pHeats->end();
		++ih) {

		if (!(*ih).IsMarked())
			continue;

		UpdateHeatDispCode((*ih));
	}

}

void CCastStringValidator::UpdateCondAndDispCodes(int strandNum)
{
	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);
	for (vector<CCSOrder*>::const_iterator io = strand.begin();
		io != strand.end();
		++io) {

		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];

		if (!rHeat.IsMarked())
			continue;

		UpdateLotDispCode((*io), rHeat);
	}


}




void CCastStringValidator::UpdateLotDispCode(CCSOrder* pCSOrder, CCastStringHeat& rHeat)
{
	// FP Change
//	bool isStock = ( pCSOrder->OrderNum() == 9999999 );
	bool isStock = (pCSOrder->FpOrderNum().Left(7) == "9999999");
	COrder* pOrder = pCSOrder->Order();

	// Make output condn and disp codes match the order.

	int condn = pCSOrder->SlabCondnCode();
	int disp = pCSOrder->DispCode();

	int outCondn = condn;
	int outDisp = disp;

	//  k. hubbard 1-28-03: comes through and declared from CSOrder.h which gets database link from Order.h
	//  Always start in database link from Order.h to see if mill order, smp, or dpa field name exist for usage. 

	int commodcode = pCSOrder->CICode();
	//	bool isSlabSaleCust = ( pCSOrder->HRUnitName().Left(4) != "80HR" );  // 1-24-07 k.hubbard  Slab Customer Sales new identification method in CSD
	//	bool isSlabSaleCust = ( pOrder->HotrollUnitName() != "80HR"	&&  pOrder->HotrollUnitName() != " " );  // 1-24-07 k.hubbard  Slab Customer Sales new identification method in CSD
	bool isSlabSaleCust = false;
	if (!isStock
		&&
		pOrder != 0) {
		if (pOrder->HotrollUnitName() != "80HR" &&  pOrder->HotrollUnitName() != " ") {  // 2-15-07 k.hubbard  Slab Customer Sales new identification method in CSD	
			isSlabSaleCust = true;
		}
	}
	//  end 1-28-03


		//	logic for plan condn code assignment for planned slits
		//	for disposition codes 3,8, 
		//	make the following replacements/modifications
		//		to the condition code:
		//
		//	4303 --> 1300
		//  4373 --> 1570
		//	4505 --> 1570
		//	4575 --> 1570
		//	4585 --> 1570
		//
		//  else, 1st digit --> 1
		//        4th digit --> 0

	if ((disp == 3
		||
		disp == 8)
		&&
		!isStock
		&&
		pOrder != 0) {

		if (condn == 4303)

			outCondn = 1300;

		else if (condn == 4373
			||
			condn == 4505
			||
			condn == 4575
			||
			condn == 4585)

			outCondn = 1570;

		else {
			outCondn = 1000 + condn % 1000;		// change 1st digit to 1
			outCondn = outCondn / 10 * 10;			// change 4th digit to 0
		}
	}


	//	logic for disp code assignment 
	//	for planned slits on mandatory cms and cms/flip

	if ((disp == 3
		||
		disp == 8)
		&&
		!isStock
		&&
		pOrder != 0) {

		if (pOrder->IsCMS()) {

			outDisp = 8;
			outCondn = condn / 10 * 10 + 5;	// change 4th digit to 5
		}
	}


	// K. Hubbard 1-28-03: 7Blast Furnace Reline support in marshalling slabs to HH Yard.:
	// K. Hubbard 5-4-05:  Slab Customer Sales support in marshalling slabs to 1 & 2 Yard idenntified by ci code 86.:

	// audit Slab Sale Customers code

	if (!isStock) {

		if
			(pOrder != 0

				//			&&
				//				m_pCastString->Id().Caster() == 1     9-8-05 k.hubbard  Slab Customer Sales now occur at 2&4 BOF 

				&&

				condn / 1000 == 4    // 1st digit is of condition code 4xxx only non-CMS added 3-3-2003 k. hubbard 

//			&&
//				( disp == 1 || disp == 4 )  

&&
//				(commodcode == 43 || commodcode == 44 )) 
//				(commodcode == 86 ))     // 5-4-05 k.hubbard  Slab Customer Sales
(isSlabSaleCust))   // 1-24-07 k.hubbard  Slab Customer Sales new identification method in CSD

outDisp = 2;             // dispCode that gets downloaded to Level 6

	}


	//		if ( pOrder->HmPlanHotChargeInd() == "M" )
	//			outDisp = 2;
	// end k. hubbard here 1-28-03



	if (disp == 5)

		outCondn = 4782;   // restored to original K. Hubbard 1-15-03 per P. Fronczek

		//outCondn = 8782;   // comment out 1-15-03, changed K. Hubbard 9-11-02 per P. Fronczek



	if (disp == 2
		||
		disp == 3
		||
		disp == 4          // added to avoid 1212 and plan condition code assignment logic in UpdateCondnCode 
		||                 //  routine below per P. Fronczek  5/4/05 k. hubbard
		disp == 5
		||
		disp == 7
		||
		disp == 8) {

		// do nothing
	}
	else {

		if (!isStock) {

			if (pOrder != 0
				&&
				condn / 1000 == 9 // 1st digit is 9
//				 &&
//				 m_pCastString->Id().Caster() == 2    9-8-05 k.hubbard  Flips now occur at 2&4 BOF 
&&
disp == 1) {
				// do nothing   To integrate with P.A. (K.Wert) flip logic that sends 931 Slab Status codes to Q.A. system. K. Hubbard 11-21-05  per P. Fronczek & T. Lara  
			}
			//	 disp == 9 ) comment out K. Hubbard 1-15-03

			//	 outCondn = 9282; comment out to integrate with P.A. (K.Wert) flip logic K. Hubbard 11-21-05  per P. Fronczek & T. Lara  

			//	 m_pCastString->Id().Caster() == 1   comment out 4 lines K. Hubbard 9-11-02 per P. Fronczek
			//	 &&
			//	 disp == 1 )

			//	 outCondn = 9212;

			else
				UpdateCondnCode(pCSOrder, rHeat, outCondn, outDisp);
		}
	}


	pCSOrder->OutputCondnCode(outCondn);
	pCSOrder->OutputDispCode(outDisp);

}




//  UpdateCondnCode is essentially just a big switch statement.
//  Selection is made on the basis of the first few digits of the spec
//   and sometimes on the carbon range.
//  We update the lot's condition code.


#define defp(spec,code) \
	specs.push_back(spec); \
	codes.push_back(code); \
	limits.push_back(0); \
	lowCCodes.push_back(0)

#define defpc(spec,carb,lowcode,highcode) \
	specs.push_back(spec); \
	limits.push_back(carb); \
	lowCCodes.push_back(lowcode); \
	codes.push_back(highcode)


void CCastStringValidator::UpdateCondnCode(CCSOrder* pOrder,
	CCastStringHeat& rHeat,
	int& outCondn,
	int& outDisp)
{
	//CChem::Bound meanC = (pOrder->Order()->MinCarbon() + pOrder->Order()->MaxCarbon())/2.0;

	CChem::Bound meanC = rHeat.AimChem(CChem::ELT_C);

	if (meanC == 0.0)
		meanC = (rHeat.MinChem(CChem::ELT_C) + rHeat.MaxChem(CChem::ELT_C)) / 2.0;


	// look for matches, X = wildcard
	CSpecCmp cmp(pOrder->LotSpec());

	// Most cases are straightforward.
	// Be sure to put longer matches before any prefixes, 
	//    such as 52163xx before 521xxxx
	// A few cases are dependent on carbon.
	// If not dependent on carbon, we push a 0 limit.

	// TODO: we could put this in database table.
	static vector<CString> specs;
	static vector<int> codes;
	static vector<CChem::Bound> limits;
	static vector<int> lowCCodes;
	static bool isInitialized = false;

	if (!isInitialized) {

		isInitialized = true;

		defp("51XXXXX", 4303);
		defp("520XXXX", 4212);
		defp("52163XX", 4505);
		defp("521XXXX", 4212);
		defp("523XXXX", 4212);
		defp("524XXXX", 4212);
		defp("525XXXX", 4212);
		defp("526XXXX", 4212);
		defp("528X9XX", 4303);
		defpc("528XXXX", 0.09, 4212, 1212);
		defp("53XXXXX", 4303);
		defp("551XXXX", 1303);
		defp("55520XX", 1303);
		defp("55527XX", 1303);
		defpc("555XXXX", 0.12, 1303, 4303);
		defp("570259X", 1303);
		defpc("570XXXX", 0.12, 1303, 4303);
		defp("571X5XX", 1303);
		defp("571XXXX", 1212);
		defp("5736XXX", 1303);
		defp("573XXXX", 1212);
		defp("574XXXX", 4212);
		defp("584XXXX", 4303);
		defp("720XXXX", 1212);
		defp("723XXXX", 4212);
		defp("823XXXX", 4212);
		defp("8262XXX", 4303);
		defp("8264XXX", 4212);
		defp("828XXXX", 1303);
		defp("829XXXX", 4303);
		defpc("855XXXX", 0.12, 1303, 4303);
		defp("870XXXX", 1212);
		defp("928XXXX", 1303);
		defp("954XXXX", 1303);
		defp("97022XX", 4303);
		defp("97062XX", 1303);
		defp("971XXXX", 4343);
		defp("XXXXXXX", 1212);
	}

	vector<CString>::iterator is = find_if(specs.begin(),
		specs.end(),
		cmp);

	if (is == specs.end())
		// shouldn't happen, with the XXXXXXX pattern at the end
		outCondn = 1212;
	else {
		int index = is - specs.begin();

		if (meanC < limits[index])
			outCondn = lowCCodes[index];
		else
			outCondn = codes[index];
	}

	// special case
	if ((cmp)("551XXXX"))
		outDisp = 5;
}




void CCastStringValidator::UpdateHeatDispCode(CCastStringHeat& rHeat)
{
	bool seen2 = false;
	bool seen5 = false;
	bool seen9 = false;
	bool seen38 = false;

	// Added 9 (flip) as valid dispcode assignment value for strand 1&2. KHH 9-11-02 
	// Removed 9 (flip) as valid dispcode assignment value for strand 1&2. KHH 1-15-03 

	{
		for (vector<CCSOrder*>::iterator io = rHeat.StrandBegin(1);
			io != rHeat.StrandEnd(1);
			++io) {

			int disp = (*io)->DispCode();

			if (disp == 2)
				seen2 = true;
			else if (disp == 5)
				seen5 = true;
			//		else if ( disp == 9 )  removed 1-15-03 KHH
			//			seen9 = true;
			else if (disp == 3 || disp == 8)
				seen38 = true;
		}
	}


	{
		for (vector<CCSOrder*>::iterator io = rHeat.StrandBegin(2);
			io != rHeat.StrandEnd(2);
			++io) {

			int disp = (*io)->DispCode();

			if (disp == 2)
				seen2 = true;
			else if (disp == 5)
				seen5 = true;
			//		else if ( disp == 9 )  removed 1-15-03 KHH
			//			seen9 = true;
			else if (disp == 3 || disp == 8)
				seen38 = true;
		}
	}

	if (seen2)
		rHeat.DispCode(2);
	else if (seen5)
		rHeat.DispCode(5);
	//else if ( seen9 )      // Added 9 (flip) as valid dispcode assignment value. KHH 9-11-02 
	//	rHeat.DispCode(9);   removed 1-15-03 KHH
	else if (seen38)
		rHeat.DispCode(3);

}



bool CCastStringValidator::CheckCondAndDispCodeRelationships()
{
	bool result1 = CheckCondAndDispCodeRelationships(1);
	bool result2 = CheckCondAndDispCodeRelationships(2);

	return result1 && result2;
}


bool CCastStringValidator::CheckCondAndDispCodeRelationships(int strandNum)
{
	bool result = true;

	ostrstream ostr;

	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);
	for (vector<CCSOrder*>::const_iterator io = strand.begin();
		io != strand.end();
		++io) {

		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];

		if (!rHeat.IsMarked())
			continue;

		int disp = (*io)->OutputDispCode();

		switch (disp) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 7:
		case 8:
			//		case 9:                       // Added 9 (flip) maint:KHH 9-11-02  //removed 1-15-03 KHH 
						// no-op
			break;
			// Added 9 (flip) as valid dispcode assignment value. KHH 9-11-02 
			//removed 1-15-03 KHH
		default:

			ostr << "Lot disposition = " << disp
				<< ", must be 1,2,3,4,5,7,8"
				<< ends;
			AddValidnError((*io)->HeatSeqNum(),
				strandNum,
				io - rHeat.StrandBegin(strandNum),
				ostr,
				CCastStringHeatValidnError::WARNING);
			result = false;
			break;
		}

		int condn = (*io)->OutputCondnCode();

		if (condn == 0) {

			ostr << "Flat condn code = " << condn
				<< " is invalid."
				<< ends;
			AddValidnError((*io)->HeatSeqNum(),
				strandNum,
				io - rHeat.StrandBegin(strandNum),
				ostr,
				CCastStringHeatValidnError::WARNING);
			result = false;
		}

	}

	return result;
}





bool CCastStringValidator::ValidateTabNums()
{
	ostrstream ostr;

	bool bad = false;

	set<long> tabNums;

	{
		for (vector<CCastStringHeat>::iterator ih = m_pHeats->begin();
			ih != m_pHeats->end();
			++ih) {

			if (!(*ih).IsMarked())
				continue;

			long tabNum = (*ih).TabNum();
			if (tabNum == 0) {
				ostr << "Heat # " << setw(2) << ih - m_pHeats->begin() + 1
					<< " has no tab # assigned. "
					<< ends;
				AddValidnError(ih - m_pHeats->begin(),
					0,
					-1,
					ostr,
					CCastStringHeatValidnError::FATAL);
				bad = true;
			}
			else if (tabNums.find(tabNum) != tabNums.end()) {
				ostr << "Duplicate tab# within this string." << ends;
				AddValidnError(ih - m_pHeats->begin(),
					0,
					-1,
					ostr,
					CCastStringHeatValidnError::FATAL);

				bad = true;
			}
			else
				tabNums.insert(tabNum);
		}
	}

	vector<CCastString*>& strings = m_pCasterScen->CastStrings();

	for (vector<CCastString*>::iterator is = strings.begin();
		is != strings.end();
		++is) {

		if ((*is) == m_pCastString)  // don't check against self
			continue;

		//if ( (*is)->Status() != CCastString::STATUS_910 )	// only check against 910s
		//	continue;

		for (vector<CCastStringHeat>::iterator ih = (*is)->Heats().begin();
			ih != (*is)->Heats().end();
			++ih) {

			long tabNum = (*ih).TabNum();

			if (tabNums.find(tabNum) != tabNums.end()) {
				ostr << "Tab # duplicated on string "
					<< setw(4) << (*is)->Id().Year()
					<< "-"
					<< setw(2) << (*is)->Id().Week()
					<< "-"
					<< setw(2) << (*is)->Id().StringId()
					<< ", heat # "
					<< setw(2) << ih - (*is)->Heats().begin() + 1
					<< ends;
				AddValidnError(ih - m_pHeats->begin(),
					0,
					-1,
					ostr,
					CCastStringHeatValidnError::FATAL);
				bad = true;
			}
		}
	}

	return !bad;
}



bool CCastStringValidator::ValidateWeights()
{
	bool isOk = true;

	ostrstream ostr;

	Weight minSize = m_pCastString->MinHeatSize();
	Weight maxSize = m_pCastString->MaxHeatSize();

	for (vector<CCastStringHeat>::iterator ih = m_pHeats->begin();
		ih != m_pHeats->end();
		++ih) {

		if (!(*ih).IsMarked())
			continue;

		if ((*ih).HeatTons() < minSize) {
			ostr << "Under weight: "
				<< setw(3) << (*ih).HeatTons()
				<< " tons < minimum of "
				<< setw(3) << minSize
				<< ends;
			AddValidnError(ih - m_pHeats->begin(),
				0,
				-1,
				ostr,
				CCastStringHeatValidnError::WARNING);

			isOk = false;
		}
		else if ((*ih).HeatTons() > maxSize) {
			ostr << "Over weight: "
				<< setw(3) << (*ih).HeatTons()
				<< " tons > maximum of "
				<< setw(3) << maxSize
				<< ends;
			AddValidnError(ih - m_pHeats->begin(),
				0,
				-1,
				ostr,
				CCastStringHeatValidnError::WARNING);
			isOk = false;
		}
	}

	return isOk;
}




bool CCastStringValidator::ValidateDemandDue()
{
	bool result1 = ValidateDemandDue(1);
	bool result2 = ValidateDemandDue(2);
	return  result1 && result2;
}


bool CCastStringValidator::ValidateDemandDue(int strandNum)
{
	bool isOk = true;
	ostrstream ostr;

	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);
	for (vector<CCSOrder*>::const_iterator io = strand.begin();
		io != strand.end();
		++io) {

		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];

		if (!rHeat.IsMarked())
			continue;

		if ((*io)->Order() != 0
			&&
			m_pSuperScen->NumSlabsDue((*io)->Order()) < 0) {

			ostr << "Order will be over scheduled by "
				<< -(m_pSuperScen->NumSlabsDue((*io)->Order()))
				<< " pieces."
				<< ends;
			AddValidnError((*io)->HeatSeqNum(),
				strandNum,
				io - rHeat.StrandBegin(strandNum),
				ostr,
				CCastStringHeatValidnError::WARNING);
			isOk = false;
		}
	}

	return isOk;
}



bool CCastStringValidator::ValidateChems()
{
	bool isOk = true;

	{
		for (vector<CCastStringHeat>::iterator ih = m_pHeats->begin();
			ih != m_pHeats->end();
			++ih) {

			if (!(*ih).IsMarked())
				continue;

			if (!ValidateHeatChems(ih - m_pHeats->begin(), (*ih)))
				isOk = false;


			if (!ValidateLotChems(ih - m_pHeats->begin(), (*ih), 1))
				isOk = false;

			if (!ValidateLotChems(ih - m_pHeats->begin(), (*ih), 2))
				isOk = false;
		}
	}

	return isOk;
}



bool CCastStringValidator::ValidateHeatChems(int heatIndex, CCastStringHeat& rHeat)
{
	// If there is no heat spec then we can't continue here.
	if (rHeat.SpecPtr() == 0)
		return true;



	// The heat chem must match exactly the spec chem on non-appSpecific elements.
	// The heat chem must be within the range of the spec chem on appSpecific elements

	ostrstream ostr;
	ostr.setf(ios::fixed, ios::floatfield);

	bool isOk = true;

	for (int elt = 0; elt < CChem::NumElements; ++elt) {

		if ((rHeat.MinChem(CChem::Element(elt)) != 0.0
			||
			(rHeat.MaxChem(CChem::Element(elt)) != 0.0
				&&
				rHeat.MaxChem(CChem::Element(elt)) != 100.0)
			||
			rHeat.AimChem(CChem::Element(elt)) != 0.0)

			&&

			(rHeat.MinChem(CChem::Element(elt)) > rHeat.AimChem(CChem::Element(elt))
				||
				rHeat.AimChem(CChem::Element(elt)) > rHeat.MaxChem(CChem::Element(elt))
				||
				rHeat.MinChem(CChem::Element(elt)) > rHeat.MaxChem(CChem::Element(elt)))) {

			ostr << LPCTSTR(CChem::ToString(CChem::Element(elt)))
				<< ": Heat range is invalid:  "
				<< setw(6) << setprecision(4) << rHeat.MinChem(CChem::Element(elt))
				<< "/"
				<< setw(6) << setprecision(4) << rHeat.AimChem(CChem::Element(elt))
				<< "/"
				<< setw(6) << setprecision(4) << rHeat.MaxChem(CChem::Element(elt))
				<< ends;
			AddValidnError(heatIndex,
				0,
				-1,
				ostr,
				CCastStringHeatValidnError::WARNING);
			isOk = false;
			continue;
		}

		const CChemRange* pRange = rHeat.SpecPtr()->GetChemRange(CChem::Element(elt));

		if (pRange == 0) {

			if (rHeat.MinChem(CChem::Element(elt)) != 0.0
				||
				(rHeat.MaxChem(CChem::Element(elt)) != 0.0
					&&
					rHeat.MaxChem(CChem::Element(elt)) != 100.0)
				||
				rHeat.AimChem(CChem::Element(elt)) != 0.0) {

				ostr << LPCTSTR(CChem::ToString(CChem::Element(elt)))
					<< ": Spec has no range, heat has "
					<< setw(6) << setprecision(4) << rHeat.MinChem(CChem::Element(elt))
					<< "/"
					<< setw(6) << setprecision(4) << rHeat.AimChem(CChem::Element(elt))
					<< "/"
					<< setw(6) << setprecision(4) << rHeat.MaxChem(CChem::Element(elt))
					<< ends;
				AddValidnError(heatIndex,
					0,
					-1,
					ostr,
					CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
		}
		else { // pRange != 0 

			if (pRange->IsAppSpecific()) {

				if (rHeat.MinChem(CChem::Element(elt)) < pRange->MinVal()
					||
					rHeat.MaxChem(CChem::Element(elt)) > pRange->MaxVal()) {

					ostr << LPCTSTR(CChem::ToString(CChem::Element(elt)))
						<< ": (open) Heat range:  "
						<< setw(6) << setprecision(4) << rHeat.MinChem(CChem::Element(elt))
						<< "/"
						<< setw(6) << setprecision(4) << rHeat.MaxChem(CChem::Element(elt))
						<< " vs spec "
						<< setw(6) << setprecision(4) << pRange->MinVal()
						<< "/"
						<< setw(6) << setprecision(4) << pRange->MaxVal()
						<< ends;
					AddValidnError(heatIndex,
						0,
						-1,
						ostr,
						CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
			else { // not app-specific

				if (rHeat.MinChem(CChem::Element(elt)) != pRange->MinVal()
					||
					rHeat.MaxChem(CChem::Element(elt)) != pRange->MaxVal()) {

					ostr << LPCTSTR(CChem::ToString(CChem::Element(elt)))
						<< ": Heat range:  "
						<< setw(6) << setprecision(4) << rHeat.MinChem(CChem::Element(elt))
						<< "/"
						<< setw(6) << setprecision(4) << rHeat.MaxChem(CChem::Element(elt))
						<< " vs spec "
						<< setw(6) << setprecision(4) << pRange->MinVal()
						<< "/"
						<< setw(6) << setprecision(4) << pRange->MaxVal()
						<< ends;
					AddValidnError(heatIndex,
						0,
						-1,
						ostr,
						CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}
	}

	return isOk;
}



bool CCastStringValidator::ValidateLotChems(int heatIndex, CCastStringHeat& rHeat, int strandNum)
{
	// If there is no heat spec then we can't continue here.
	if (rHeat.SpecPtr() == 0)
		return true;

	ostrstream ostr;

	bool isOk = true;

	for (vector<CCSOrder*>::iterator io = rHeat.StrandBegin(strandNum);
		io != rHeat.StrandEnd(strandNum);
		++io) {

		// If there is no lot spec, then we can't do our test
		// (the error is recorded elsewhere)

		if ((*io)->PtrLotSpec() == 0)
			continue;

		//  The order and spec chems must contain the heat chem range for each element

		for (int elt = 0; elt < CChem::NumElements; ++elt) {

			CChem::Bound heatMin = rHeat.MinChem(CChem::Element(elt));
			CChem::Bound heatMax = rHeat.MaxChem(CChem::Element(elt));

			CChem::Bound ordMin;
			CChem::Bound ordMax;
			bool hasReasonCode;
			bool nonTrivial = (*io)->GetChemRange(CChem::Element(elt), ordMin, ordMax, hasReasonCode);

			if (nonTrivial) {

				if (ordMin > heatMin
					||
					ordMax < heatMax) {
					ostr << "Elt " << LPCTSTR(CChem::ToString(CChem::Element(elt)))
						<< ": lot tighter than heat: "
						<< setw(6) << setprecision(4) << ordMin
						<< "/"
						<< setw(6) << setprecision(4) << ordMax
						<< " vs"
						<< setw(6) << setprecision(4) << heatMin
						<< "/"
						<< setw(6) << setprecision(4) << heatMax
						<< "/"
						<< ends;
					AddValidnError(heatIndex,
						strandNum,
						io - rHeat.StrandBegin(strandNum),
						ostr,
						CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}
	}

	return isOk;
}




bool CCastStringValidator::Validate340080()
{
	bool isOk = true;

	// Check for valid specs
	if (!ValidateSpecs())
		isOk = false;


	// original program audits PLAN_STEEL_EARLY_START_TIME
	//						   PLANT_STEEL_LATEST_END_TIME
	// Question to Carey:  do we care
	// Answer: not right now

	// DPA code iterates over lots in a heat.
	// We iterate over all lots in all marked heats,
	// But we have to do one strand and then the other.

	if (!Validate340080(1))
		isOk = false;

	if (!Validate340080(2))
		isOk = false;

	if (!Validate340080Weights())
		isOk = false;

	return isOk;
}



// We need only check that the spec has been assigned.
// and that lot specs are compatible

bool CCastStringValidator::ValidateSpecs()
{
	bool ok = true;

	ostrstream ostr;

	for (vector<CCastStringHeat>::iterator ih = m_pHeats->begin();
		ih != m_pHeats->end();
		++ih) {

		if (!(*ih).IsMarked())
			continue;

		if ((*ih).SpecPtr() == 0) {

			ostr << "Bad heat spec: '" << LPCTSTR((*ih).Spec()) << "'." << ends;

			AddValidnError(ih - m_pHeats->begin(),
				0,
				-1,
				ostr,
				CCastStringHeatValidnError::WARNING);
			ok = false;
		}

		if (!ValidateLotSpecs((*ih), 1))
			ok = false;

		if (!ValidateLotSpecs((*ih), 2))
			ok = false;

	}

	return ok;
}



bool CCastStringValidator::ValidateLotSpecs(CCastStringHeat& rHeat, int strandNum)
{
	// Make sure each lot spec has been assigned
	//  and is compatible with the heat spec

	ostrstream ostr;

	bool isOk = true;

	int caster = m_pCastString->Id().Caster(); //### caster-specific

	for (vector<CCSOrder*>::iterator io = rHeat.StrandBegin(strandNum);
		io != rHeat.StrandEnd(strandNum);
		++io) {

		if ((*io)->PtrLotSpec() == 0) {

			ostr << "Missing lot spec" << ends;
			AddValidnError((*io)->HeatSeqNum(),
				strandNum,
				io - rHeat.StrandBegin(strandNum),
				ostr,
				CCastStringHeatValidnError::WARNING);
			isOk = false;
		}
		else if (rHeat.SpecPtr() != 0) {
			// check for compatibility between heat spec and lot spec
			if (!TheSnapshot.HeatSpecCrossApp().IsCompatible((*io)->LotSpec(), rHeat.Spec(), caster)) { //### caster-specific
				ostr << "Lot spec "
					<< LPCTSTR((*io)->LotSpec())
					<< " incompatible with heat spec "
					<< LPCTSTR(rHeat.Spec())
					<< ends;
				AddValidnError((*io)->HeatSeqNum(),
					strandNum,
					io - rHeat.StrandBegin(strandNum),
					ostr,
					CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
			else if (rHeat.SpecPtr()->Name()[6] == '0' && (*io)->PtrLotSpec()->Name()[6] == '5') {
				ostr << "Lot spec "
					<< LPCTSTR((*io)->LotSpec())
					<< " is 05, not compatible with heat spec "
					<< LPCTSTR(rHeat.Spec())
					<< ends;
				AddValidnError((*io)->HeatSeqNum(),
					strandNum,
					io - rHeat.StrandBegin(strandNum),
					ostr,
					CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
			//            Added for sixth digit validation of calcium range specs where the sixth digit is 8 or 9 and heat
			//            must also be a 8 or 9 for compatibility 3-21-07 k. hubbard  
			//            if ( rHeat.SpecPtr()->Name()[5] != '8' && (*io)->PtrLotSpec()->Name()[5] == '8' ) {
			if ((*io)->Order() != 0) {
				if (rHeat.SpecPtr()->Name()[5] != '8' && (*io)->Order()->OrderedSpec()[5] == '8') {
					ostr << "Lot Order Spec "
						<< LPCTSTR((*io)->Order()->OrderedSpec())
						<< " is 8x, not compatible with heat spec "
						<< LPCTSTR(rHeat.Spec())
						<< ends;
					AddValidnError((*io)->HeatSeqNum(),
						strandNum,
						io - rHeat.StrandBegin(strandNum),
						ostr,
						CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
				else if (rHeat.SpecPtr()->Name()[5] != '9' && (*io)->Order()->OrderedSpec()[5] == '9') {
					ostr << "Lot Order Spec "
						<< LPCTSTR((*io)->Order()->OrderedSpec())
						<< " is 9x, not compatible with heat spec "
						<< LPCTSTR(rHeat.Spec())
						<< ends;
					AddValidnError((*io)->HeatSeqNum(),
						strandNum,
						io - rHeat.StrandBegin(strandNum),
						ostr,
						CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
		}
	}

	return isOk;
}



#ifdef ADD_ERR
#undef ADD_ERR
#endif

#define ADD_ERR(severity) \
			AddValidnError( (*io)->HeatSeqNum(),					\
							strandNum,								\
							io - rHeat.StrandBegin(strandNum),		\
							ostr, severity )


int TRANS_WIDTH = 1;

bool CCastStringValidator::Validate3SP(int caster) {
	bool isOk = true;
	ostrstream ostr;

	// Check the strand size, returning true if the size is zero.
	int strandNum = 1;  // only one strand per caster
	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);
	if (strand.size() == 0) return true;  // trivially ok, as there's nothing to check.

	Width prevWidth = 0; // we start at the beginning
	int slabNum = 1;

	// Iterate over the orders, validating as we go.
	for (vector<CCSOrder*>::const_iterator io = strand.begin(); io != strand.end(); ++io) {
		// Consider only marked heats.
		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];
		if (!rHeat.IsMarked())
			continue;

		// Audit the lot spec, looking at specific digits.
		FixHeatSpec3SP(rHeat, caster); //### caster-specific

		// Audit the number of pieces. Specific numbers for 4 and 5 other than not zero? See old version for details on caster 1.
		int numPieces = (*io)->NumPieces();
		if (0 == numPieces) {
			ostr << "#PC invalid" << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			isOk = false;
		}

		//### Get COrder from CSOrder to access condition code and specification code
		COrder* order = (*io)->Order();
		CString conditionCode = order->CondWest();
		CString specificationCode = order->WestSpec();
		CString lastChar = conditionCode.Mid(conditionCode.GetLength() - 1, 1);

		//### new rules
		//#### New rule 1.1.1 Degrades for 1st slab in heat after startup
		if ((*io)->HeatSeqNum() == 0 &&   //### heat numbers start at 0: 1st heat?
			slabNum == 1 &&               //### 1st slab in 1st heat?
			order != 0)                   //### ensure non-null order
		{
        	//## line pipe (rule 1.1.7) defined as follows: MORD_SMK_SPEC_WEST = “J26” and  MORD_SMK_COND_WEST code last character = “J”
			//### PROBLEM: check with KH -- how can last char be "J" if code must be "8H"?
			if ((specificationCode.Compare("J26") == 0) && ((lastChar.Compare("J") == 0) || (lastChar.Compare("H") == 0)))
			{
				if (conditionCode.Compare("8H") != 0)
				{
					ostr << "Invalid condition code for 1st slab in 1st heat in line pipe schedule = " << conditionCode << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else if ((conditionCode.Compare("7D") != 0) && (conditionCode.Compare("8H") != 0))
			{	
				ostr << "Invalid condition code for 1st slab in 1st heat = " << conditionCode << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		//#### Also new (1.1.2): check 2nd slab in heat
		if ((*io)->HeatSeqNum() == 0 &&   //### heat numbers seem to start at 0
			slabNum == 2 &&               //### 2nd slab in 1st heat?
			order != 0)                   //### ensure non-null order
		{
			CString conditionCode = order->CondWest();
			CString specificationCode = order->WestSpec();

			if (conditionCode.GetLength() < 2)
			{
				ostr << "Can't check bad condition code for 2nd slab in 1st heat = " << conditionCode << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
			else if (specificationCode.GetLength() < 5)
			{
				ostr << "Can't check bad specification code for 2nd slab in 1st heat = " << specificationCode << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
			//### line pipe stuff goes here

			// For spec K201G, the conditionCondition cannot be TD, 2D, or 3D
			else if (specificationCode.Compare("K201G") == 0) //### rule 1.1.3 (b)
			{
				if ((conditionCode.Compare("TD") == 0) ||
					(conditionCode.Compare("2D") == 0) ||
					(conditionCode.Compare("3D") == 0))
				{
					ostr << "Invalid condition code for 2nd slab in 1st heat = " << conditionCode << 
						" with specificationCode " << specificationCode << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			// otherwise, no TD or 2D condition code (but 3D is ok)
			else
			{
				if ((conditionCode.Compare("TD") == 0) ||
					(conditionCode.Compare("2D") == 0))
				{
					ostr << "Invalid condition code for 2nd slab in 1st heat = " << conditionCode << 
						" with specification code " << specificationCode << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
		}

		/*
		if (slabNum == 2)
			if ((code != 'D')) {
				ostr << "invalid slit type code for 1st slab in heat = " << code << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
			*/
		// Audit slit-code type
		char code = (*io)->SlitTypeCode(); //### inherited code, not a new one

		// Added noslit type code here on xxx4 condition codes 6-27-05 k. hubbard  
		if (code != 'D' &&
			code != 'E' &&
			code != 'F' &&
			code != 'S' &&
			code != 'R' &&
			code != ' ') {
			ostr << "invalid slit type code = " << code << ends;
			ADD_ERR(CCastStringHeatValidnError::WARNING);
			isOk = false;
		}

		//### New condition code 'H' per KH: the length must not exceed 460
		if (code == 'H' && (*io)->SlabLength() > 460)
		{
			ostr << "For slit-code H, the slab length of  " << (*io)->SlabLength() << " exceeds 460" << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			isOk = false;
		}

		// Audit steel width
		//### For casters 4 and 5, some other check than non-zero? Original checks caster 3 only.
		Width width = (*io)->SlabWidth();
		if (0 == width) {
			ostr << "steel width (" << setw(2) << width << ") is invalid" << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			isOk = false;
		}
		//### New -- assume this is the intended logic for 4 and 5.
		else {
			Width min = (*io)->MinAllowedSlabWidth();
			Width max = (*io)->MaxAllowedSlabWidth();
			if (width < min || width > max) {
				ostr << "steel width (" << setw(2) << width << ") not in range [" << min << "," << max << "	]" << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		// Audit non-transition provided steel widths
		width = (*io)->SlabWidth();
		code = (*io)->SlitTypeCode();
		bool isTrans = (*io)->IsTransition();
		bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

		if ((*io)->Order() != 0 && !isStock) {
			Width provWidthMax = (*io)->Order()->ProvSlabWidthMax();

			if (code == ' ' && !isTrans && width > provWidthMax) {  // original has: code = ' ', which seems an error
				ostr << "steel width (" << width << ") must not" << "be > Prov Max" << provWidthMax << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}
		// End Note: K. Hubbard 12-11-03; End maint. of add new fatal here for checking width (*io)->ProvSlabWidthMin & Max

		// Audit min/max steel length
		if ((*io)->PlanSteelLengthMin() == 0 || (*io)->PlanSteelLengthMax() == 0) {
			if ((*io)->FpOrderNum().Left(7) == "9999999") {
				CString spec3 = (*io)->LotSpec().Left(3);
				// Shouldn't this be a general check for CMS (condn = 5XXX or 8XXX)???
				if ((*io)->SlabCondnCode() == 5782
					&& (spec3 == "520" ||
						spec3 == "524" ||
						spec3 == "726" ||
						spec3 == "811" ||
						spec3 == "820" ||
						spec3 == "822" ||  // Added for Nancy Hake Honda trials 6-15-09 k. hubbard  
						spec3 == "826")) {
					//### Does this hold for 3SP? Yes, confirmed by KH (8-29-2016)
					// avoid impractical 5% CMS increase if plan steel length >= 365
					// assumption: 384 is max length 384 - 5% = 365
					if ((*io)->SlabLength() >= 365) {
						(*io)->PlanSteelLengthMin(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
						(*io)->PlanSteelLengthMax(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
					}
				}
				else {
					(*io)->PlanSteelLengthMin((*io)->SlabLength());
					(*io)->PlanSteelLengthMax((*io)->SlabLength());
				}
			}
			else {
				if ((*io)->Order() != 0) {
					bool isLenOk = AuditSlabLengthData((*io));
					Length minL;
					Length maxL;
					ComputeSlabLength(caster, (*io), isLenOk, minL, maxL); //### caster-specific
					(*io)->PlanSteelLengthMin(minL);
					(*io)->PlanSteelLengthMax(maxL);
				}
			}
		}

		// Audit planned length.
		if ((*io)->FpOrderNum().Left(7) != "9999999") {
			if ((*io)->Order() != 0 && ((*io)->Order()->CustCoilReq() == 8 || (*io)->Order()->CustCoilReq() == 9)) {
				// no-op
			}
			else if (long((*io)->SlabLength() + 0.5) < long((*io)->PlanSteelLengthMin() + 0.5) ||
				long((*io)->SlabLength() + 0.5) > long((*io)->PlanSteelLengthMax() + 0.5)) {
				ostr << "slab length " << long((*io)->SlabLength() + 0.5) << " should be between " << long((*io)->PlanSteelLengthMin() + 0.5)
					<< " and " << long((*io)->PlanSteelLengthMax() + 0.5) << ends;
				ADD_ERR(CCastStringHeatValidnError::WARNING);  // temp switched from fatal to warning for Dofasco 
				// order campaign per P. Fronczek. maint k. hubbard 11-11-04. 					                              
				isOk = false;
			}
		}

		if ((*io)->FpOrderNum().Left(7) != "9999999") {
			if ((*io)->Order() != 0 && ((*io)->Order()->CustCoilReq() == 8 || (*io)->Order()->CustCoilReq() == 9)) {
				// no-op
			}
			else if (long((*io)->SlabLength() + 0.5) < long((*io)->PlanSteelLengthMin() + 0.5) ||
				long((*io)->SlabLength() + 0.5) > long((*io)->PlanSteelLengthMax() + 0.5)) {
				ostr << "slab length " << long((*io)->SlabLength() + 0.5) << " should be between " << long((*io)->PlanSteelLengthMin() + 0.5)
					<< " and " << long((*io)->PlanSteelLengthMax() + 0.5) << ends;
				ADD_ERR(CCastStringHeatValidnError::WARNING);  // temp switched from fatal to warning for Dofasco 
															   // order campaign per P. Fronczek. maint k. hubbard 11-11-04. 					                              
				isOk = false;
			}
		}

		// Audit steel length compliance with steelshop limitations
		//## Original has if-clauses for casters 1, 2, and 3, which are omitted here. Instead, slab length for 4 and 5 is checked.
		Length len = (*io)->SlabLength();
		Length minLen = 200; // per TF's email of 8-6-2016
		if (0 == len) {
			ostr << "Slab length 0 is invalid." << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			isOk = false;
		}
		else if (len < minLen || len > GlobalConstants.CasterSlabLengthMax(caster)) { //### caster-specific
			ostr << "Slab length " << setw(3) << len << " not between steelshop length boundaries" << GlobalConstants.CasterSlabLengthMax(caster) << "," << minLen << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			isOk = false;
		}

		// Audit special condition codes
		// Reinstalled 8/8/03 K. Hubbard 
		// New: CastStringValidator.cpp for special condition code assignments; Maint. added by K. Hubbard 8-5-03  
		// audit ordered condition code for special non-slit instructions. - per Larry Thomas, Quality Design Engineer (QDE). 
		//##### How do the scheduling rules in 1.1 Degrades section pertain here?
		char slitCode = (*io)->SlitTypeCode();
		int condnCode = (*io)->SlabCondnCode();
		int dig3 = condnCode / 10 % 10; // get D of xxDx, third digit of ordered condition code  
		int dig4 = condnCode / 1 % 10;  // get D of xxxD, fourth digit of ordered condition code
		if (dig3 == 3 && dig4 == 4) {
			if (slitCode == ' ' || slitCode == 'D') {
				// do nothing operation
			}
			else {
				ostr << "Condition code " << condnCode << " orders CAN NOT be slit per QDE Larry Thomas" << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		// Audit exposure code
		//######### How do rules pertain here for casters 4 and 5?
		//### In original, the if-clause is for caster 1, the else for the rest. Does that still hold?
		char exp = (*io)->ExposureCode();
		int condn = (*io)->SlabCondnCode();
		if (condn > 0 &&
			(exp == ' ' || exp == 'E' || exp == 'U' || exp == '1' || exp == '2')) {
			// no-op
		}
		else {
			ostr << "Exposure code '" << exp << "' is invalid" << ends;
			ADD_ERR(CCastStringHeatValidnError::WARNING);
			isOk = false;
		}


		// Audit Tek and Kote positional planned orders at beginning of cast violations.  Added 03-06-06 k. hubbard per P. Velasco
		int commodcode = (*io)->CICode();
		isStock = ((*io)->FpOrderNum().Left(7) == "9999999");
		int heatNumber = (*io)->HeatSeqNum();
		int lotNumber = (*io)->LotNum();

		if (prevWidth == 0 && heatNumber == 0) {
			if (!isStock) {
				if (commodcode == 11 ||  // Added commocode (11 [stringers], 45-50) to Tek/Kote material check 3-17-06 k. hubbard
					commodcode == 12 ||
					commodcode == 17 ||
					commodcode == 18 ||
					commodcode == 19 ||
					commodcode == 20 ||
					commodcode == 21 ||
					commodcode == 45 ||
					commodcode == 46 ||
					commodcode == 47 ||
					commodcode == 48 ||
					commodcode == 49 ||
					commodcode == 50 ||
					commodcode == 72 ||
					commodcode == 73) {
					ostr << "Tek and Kote commodity code " << setw(3) << commodcode << " can not be planned at start of a cast" << ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}

		// Audit width change.  Radicals are ok here 12-11-03 k. hubbard
		code = (*io)->SlitTypeCode();

		// This is the first we've seen, use it for initial width: important to put this initial update before the check above
		// for CheckStringHeatPosition
		if (0 == prevWidth)
			prevWidth = (*io)->SlabWidth();

		Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
		if (widthDiff > TRANS_WIDTH && code == ' ') {  //	Consider slit type i.e. Radicals ok. 11-29-05 k. hubbard
			ostr << "Width jump from " << setw(3) << prevWidth << " to " << setw(3) << (*io)->SlabWidth() << " is invalid" << ends;
			ADD_ERR(CCastStringHeatValidnError::FATAL);
			//	Changing transition width violations to Fatal 12-11-03 k. hubbard
			isOk = false;
		}

		// Audit Transition 80" Slab Spread Squeeze violations against Provided Cast Widths
		if ((*io)->Order() != 0) {
			Width width = (*io)->SlabWidth();
			float ProvWidthMin;   // Added 4 lines here for min check 3-17-06 k. hubbard
			CString strProvMin;
			// Changed line (see next line) to throw reference more reliable Order.h field here for min value 4-5-07 k. hubbard
			strProvMin.Format("%2.1f", (*io)->Order()->ProvSlabWidthMin());
			ProvWidthMin = atof(strProvMin);

			float ProvWidthMax;
			CString strProvMax;
			// Changed line (see next line) to throw reference more reliable Order.h field here for max value 4-5-07 k. hubbard
			strProvMax.Format("%2.1f", (*io)->Order()->ProvSlabWidthMax());
			ProvWidthMax = atof(strProvMax);

			char code = (*io)->SlitTypeCode();
			bool isTrans = (*io)->IsTransition();
			Width transSpread80 = 1;  //Restored per P. Fronczek maint. 4-18-07 k. hubbard
									  // strProvMax.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
									  // m_provCastSlabWidthMax = atof(strProvMax);
									  // Width transProvWidthMax = (ProvWidthMax - transSpread80);
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

			if (prevWidth != 0 && !isStock && (*io)->Order() != 0) { // Added line to make sure order exist for more reliable Order.h field here for min/max values 4-5-07 k. hubbard
				//	int transProvWidthMax = (ProvWidthMax - transSpread80);   comm. out 4-5-07 k. hubbard
				Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
				Width NarrowestWidthPoint = min((*io)->SlabWidth(), prevWidth);
				Width transProvWidthMax = (ProvWidthMax - transSpread80);   //Restored per P. Fronczek maint. 4-18-07 k. hubbard
																			// Added or condition here for min check 3-17-06 k. hubbard
				if (widthDiff == 1 && (code == ' ') && (isTrans) &&
					(NarrowestWidthPoint > transProvWidthMax || NarrowestWidthPoint < ProvWidthMin)) {   //Switched per P. Fronczek maint. 4-18-07 k. hubbard
					// Comm out switched per P. Fronczek maint. 4-18-07 k. hubbard
					ostr << "Transition steel width (" << NarrowestWidthPoint << ") is a violation of"
						<< "80 HSM Spread / Squeeze Prov Min (" << ProvWidthMin << ") or Prov Max (" << ProvWidthMax
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}
		// End Maint. Note: K. Hubbard 3-15-06; End maint. of add new fatal here for checking width (*io)->ProvSlabWidthMin & Max

		// Audit Side Slits Minimum Allowance change.  Added 5-24-06 k. hubbard per P. Velasco
		code = (*io)->SlitTypeCode();
		isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

		if (!isStock) {
			Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 
			if (widthDiff > 0) {
				if (widthDiff < 2 && code == 'S') { //	Check slit type. 
					ostr << "Planned Side Slits from " << setw(3) << (*io)->SlabWidth()
						<< " to " << setw(3) << (*io)->Order()->SlabWidth()  // Throw reference to reliable Order.h (P-STEEL-MAKE-PLAN) field here for aim value 02-10-09 k. hubbard
						<< " under 2 inches is a dock violation" << ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}

		// Audit Width Jumps change.  Added 11-29-05 k. hubbard per P. Velasco
		commodcode = (*io)->CICode();
		code = (*io)->SlitTypeCode();
		Width transSpread80 = 0.3;
		Width transSqueeze80 = 1.6;
		isStock = ((*io)->FpOrderNum().Left(7) == "9999999");
		widthDiff = fabs((*io)->SlabWidth() - prevWidth);
		Width MinTransHBWidth = fabs(prevWidth - transSqueeze80);
		Width MaxTransHBWidth = fabs(prevWidth + transSpread80);

		if (!isStock) {
			Width hbwidth = (*io)->OrderHotBandWidth();
			if (widthDiff == 1 && (commodcode == 65 || commodcode == 66) &&
				(hbwidth < MinTransHBWidth || hbwidth > MaxTransHBWidth) && code == ' ') {  //	Consider slit type i.e. Radicals ok. 
				ostr << "Slab Width Jump from " << setw(3) << prevWidth << " to " << setw(3) << (*io)->SlabWidth()
					<< " is invalid" << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		// Audit Stock Reason codes   added audit for stock reason 5-5-09 k. hubbard
		CString stockFaclReason = (*io)->StockReason();
		CString stockCommReason = (*io)->StockCommercialReason();
		// FP Change
		if ((*io)->FpOrderNum().Left(7) == "9999999") {
			if (stockFaclReason == "" || stockFaclReason == "0" || stockFaclReason == "<blank>" ||
				stockCommReason == "" || stockCommReason == "0" || stockCommReason == "<blank>") {
				ostr << "stock facility reason '" << setw(20) << LPCTSTR(stockFaclReason)
					<< " ' or stock commercial reason '" << setw(20) << LPCTSTR(stockCommReason)
					<< " ' must not be blank " << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning for Dofasco 
				isOk = false;
			}
		}

		// Audit Slit Reason Code  added audit for slit reason 2-26-10 k. hubbard
		code = (*io)->SlitTypeCode();
		CString slitFaclReason = (*io)->SlitReason();

		if (code == 'E' || code == 'S' || code == 'R') {
			if (slitFaclReason == "" || slitFaclReason == "0" || slitFaclReason == "<blank>") {
				ostr << "slit facility reason '" << setw(20) << LPCTSTR(slitFaclReason)
					<< " ' must not be blank " << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning 
				isOk = false;
			}
		}

		// !!!!!! Caution the variable below is shared between 2 width audits above and must get populated once after both or else one audit above will fail!!!! 11-29-05 k. hubbard
		prevWidth = (*io)->SlabWidth();  // Important CAUTION!!!! repositioned here to work inside this For loop and share between width change and width jump audits 11-29-05 k. hubbard

		// Audit change indicator
		if ((*io)->ChangeMarked()) {
			ostr << "Change indicator has not been removed." << ends;
			ADD_ERR(CCastStringHeatValidnError::WARNING);
			isOk = false;
		}
		slabNum++; //##### new: tracking which slab
	} // for loop
	return isOk;
} // method body

bool CCastStringValidator::Validate340080(int strandNum)
{
	bool isOk = true;
	ostrstream ostr;

	int caster = m_pCastString->Id().Caster(); //### caster-specific

	//### new code
	if (caster == 4 || caster == 5)
		return Validate3SP(caster);

	const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);

	if (strand.size() == 0)
		// nothing to check
		return true;

	//#### Presumably casters 4 and 5 should be included here: each has 1 strand
	if (strandNum == 2 && Caster::NumStrandsOnCaster(caster) == 1) { //### caster-specific
		ostr << "Caster " << caster << ": There are orders on strand #2!" << ends;
		//AddValidationString(ostr);
		AddValidnError(-1, 0, -1, ostr, CCastStringHeatValidnError::FATAL);
		return false;
	}

	Width prevWidth = 0;

	for (vector<CCSOrder*>::const_iterator io = strand.begin();
		io != strand.end();
		++io) {

		CCastStringHeat& rHeat = (*m_pHeats)[(*io)->HeatSeqNum()];

		if (!rHeat.IsMarked())
			continue;


		// audit lot spec -- we should already have this ok'd

		//  We need to have some fun with certain digits of the spec.

		FixHeatSpec(rHeat, caster); //### caster-specific

		// audit numPieces
		{
			int numPieces = (*io)->NumPieces();

			if (caster == 1) { //### caster-specific

				if (numPieces == 0
					||
					(numPieces > 32 && numPieces != 99)) {
					ostr << "#PC must be < 33"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else if (numPieces == 0) {
				ostr << "#PC invalid"
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		// audit slit type code
		{
			char code = (*io)->SlitTypeCode();

			if (code != 'D'  // Added noslit type code here on xxx4 condition codes 6-27-05 k. hubbard  
				&&
				code != 'E'
				&&
				code != 'F'
				&&
				code != 'S'
				&&
				code != 'R'
				&&
				code != ' ') {

				ostr << "invalid slit type code = "
					<< code
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
		}


		// audit steel width
		//#### Not clear whether and how width  changes play into this logic.
		{
			Width width = (*io)->SlabWidth();

			if (caster == 3) { //### caster-specific

				if (width < 24 || width > 60) {
					ostr << "steel width ("
						<< setw(2) << width
						<< ") not in range [24,60]"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else if (width == 0) {
				ostr << "steel width ("
					<< setw(2) << width
					<< ") is invalid"
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}

		// Note: K. Hubbard 12-11-03; Add New Fatal here by checking width (*io)->ProvSlabWidthMin & Max

				// audit non Transition Provided Steel widths
		{
			Width width = (*io)->SlabWidth();
			char code = (*io)->SlitTypeCode();
			bool isTrans = (*io)->IsTransition();
			//			Width provWidthMax = (*io)->ProvSlabWidthMax();
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

			if ((*io)->Order() != 0
				&&
				!isStock)
			{
				Width provWidthMax = (*io)->Order()->ProvSlabWidthMax();
				Width provWidthMin = (*io)->Order()->ProvSlabWidthMin();

				if (code = ' ') {
					//	&&
					if (isTrans = false) {
						//	&&
						if (width > provWidthMax) {

							ostr << "steel width ("
								<< width
								<< ") must not"
								<< "be > Prov Max" << provWidthMax
								<< ends;
							ADD_ERR(CCastStringHeatValidnError::FATAL);
							isOk = false;
						}
					}
				}
			}
		}

		// End Note: K. Hubbard 12-11-03; End maint. of add new fatal here for checking width (*io)->ProvSlabWidthMin & Max


				// audit min/max steel length

		{
			if ((*io)->PlanSteelLengthMin() == 0
				||
				(*io)->PlanSteelLengthMax() == 0) {

				// FP Change
				//if ( (*io)->OrderNum() == 9999999 ) {
				if ((*io)->FpOrderNum().Left(7) == "9999999") {

					CString spec3 = (*io)->LotSpec().Left(3);

					// Shouldn't this be a general check for CMS (condn = 5XXX or 8XXX)???

					if ((*io)->SlabCondnCode() == 5782
						&&
						(spec3 == "520"
							||
							spec3 == "524"
							||
							spec3 == "726"
							||
							spec3 == "811"
							||
							spec3 == "820"
							||
							spec3 == "822"  // Added for Nancy Hake Honda trials 6-15-09 k. hubbard  
							||
							spec3 == "826")) {

						// avoid impractical 5% CMS increase if plan steel length >= 365
						// assumption: 384 is max length
						// 384 - 5% = 365

						if ((*io)->SlabLength() >= 365) {

							(*io)->PlanSteelLengthMin(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
							(*io)->PlanSteelLengthMax(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
						}

						// shouldn't we have an else here?

					}
					else {

						(*io)->PlanSteelLengthMin((*io)->SlabLength());
						(*io)->PlanSteelLengthMax((*io)->SlabLength());
					}
				}
				else {

					if ((*io)->Order() != 0) {

						bool isLenOk = AuditSlabLengthData((*io));

						Length minL;
						Length maxL;

						ComputeSlabLength(caster, (*io), isLenOk, minL, maxL); //### caster-specific
						(*io)->PlanSteelLengthMin(minL);
						(*io)->PlanSteelLengthMax(maxL);
					}
				}
			}
		}

		// audit planned length

		{
			//			{ 
			//				ostrstream msg;
			//				msg << "Slab length: " << (*io)->SlabLength()
			//					<< "\nMax: " << (*io)->PlanSteelLengthMax()
			//					<< ends;
			//				AfxMessageBox(msg.str());
			//				msg.freeze(false);
			//			}

						// FP Change
						//if ( (*io)->OrderNum() != 9999999 ) {
			if ((*io)->FpOrderNum().Left(7) != "9999999") {

				if ((*io)->Order() != 0
					&&
					((*io)->Order()->CustCoilReq() == 8
						||
						(*io)->Order()->CustCoilReq() == 9)) {

					// no-op
				}
				else if (long((*io)->SlabLength() + 0.5) < long((*io)->PlanSteelLengthMin() + 0.5)
					||
					long((*io)->SlabLength() + 0.5) > long((*io)->PlanSteelLengthMax() + 0.5)) {

					ostr << "slab length " << long((*io)->SlabLength() + 0.5)
						<< " should be between " << long((*io)->PlanSteelLengthMin() + 0.5)
						<< " and " << long((*io)->PlanSteelLengthMax() + 0.5)
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);  // temp switched from fatal to warning for Dofasco 
//                          order campaign per P. Fronczek. maint k. hubbard 11-11-04. 					                              
//					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
		}

		//  check heat position for 3 combi startup slabs

		{
			//#### Need rules here for 4 and 5 on slab length. For both, MIN is 200" and MAX is 460".
			if (caster == 3) { //### caster-specific

				CString heatSpec3 = rHeat.Spec().Left(3);
				CString heatSpec5 = rHeat.Spec().Left(5);

				//                comm out spec check 7-01-04 k. hubbard
				//				if ( heatSpec3 == "521"
				//					 ||
				//					 heatSpec3 == "523"
				//					 || 
				//					 heatSpec3 == "524"
				//					 ||
				//					 heatSpec5 == "52025"
				//					 ||
				//					 heatSpec5 == "52822"
				//					 ||
				//					 heatSpec5 == "52826" ) {

				if (prevWidth == 0) { // 1st lot in strand

					int condn = (*io)->SlabCondnCode();

					//						if ( condn == 4682 || condn == 4782 ) {  comm. out low quality exception check 7-1-04 k. hubbard   
					//							// no op
					//						} 

					//						else if ( (*io)->SlabLength() > 315 ) {  changed per cast rules 6-30-04 k.hubbard
					if ((*io)->SlabLength() > 362) {

						ostr << "Slab length " << setw(3) << (*io)->SlabLength()
							<< " > 362 in 1st slab."    // changed from 315 per cast rules 6-30-04 k.hubbard
							<< ends;
						//							ADD_ERR(CCastStringHeatValidnError::FATAL);
						ADD_ERR(CCastStringHeatValidnError::WARNING);  // changed per J. Sarb-Beer 9-29-08 k. hubbard
						isOk = false;
					}   // 362" length check
				}      // 1st slab startup check 
//				}           comm out 7-01-04 k. hubbard // spec check      
			}               // 3 combi caster check
		}                   // audit heat position check

		// audit steel length compliance with steelshop limitations

		{
			Length len = (*io)->SlabLength();

			//            Added 1 Slab Caster length check 2-4-10 k. hubbard  
			if (caster == 1) { //### caster-specific
				if (len > GlobalConstants.CasterSlabLengthMax(caster)) { //### caster-specific
					ostr << "Slab length " << setw(3) << len
						<< " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			//            Added 2 Slab Caster length check 2-4-10 k. hubbard  
			if (caster == 2) { //### caster-specific
//  				if ( len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171 ) { //### caster-specific
				if (len > GlobalConstants.CasterSlabLengthMax(caster)) { //### caster-specific
					ostr << "Slab length " << setw(3) << len
						<< " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else {

				if (caster == 3) { //### caster-specific

					if (len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171) { //### caster-specific
						ostr << "Slab length " << setw(3) << len
							<< " not between steelshop length boundaries 171 and " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
							<< ends;
						ADD_ERR(CCastStringHeatValidnError::FATAL);
						isOk = false;
					}
				}
			}
			//			else {

			if ((*io)->SlabLength() == 0) {

				ostr << "Slab length 0 is invalid."
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
			//			}
		}


		// audit strand num
		// no need - we can't get this wrong

		// Reinstalled 8/8/03 K. Hubbard 
		// New: CastStringValidator.cpp for special condition code assignments; Maint. added by K. Hubbard 8-5-03  
		// audit ordered condition code for special non-slit instructions. - per Larry Thomas, Quality Design Engineer (QDE). 

		//##### How do the scheduling rules in 1.1 Degrades section pertain here?
		{

			char slitCode = (*io)->SlitTypeCode();

			int condnCode = (*io)->SlabCondnCode();

			int dig3 = condnCode / 10 % 10; // get D of xxDx, third digit of ordered condition code  
			int dig4 = condnCode / 1 % 10;  // get D of xxxD, fourth digit of ordered condition code

			if (dig3 == 3 && dig4 == 4) {

				if (slitCode == ' ' || slitCode == 'D') {
					// do nothing operation
				}
				else {
					ostr << "Condition code " << condnCode
						<< " orders CAN NOT be slit per QDE Larry Thomas"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}


			}


		}



		// audit exposure code
		//######### How do 1.1 Degrade rules pertain here for casters 4 and 5?
		//#########  According to KH answer to questions.txt:  None today Ignore for casters 4 and 5
		//#########  So restricting the else clause to casters 2,3
		{
			char exp = (*io)->ExposureCode();
			int condn = (*io)->SlabCondnCode();

			if (caster == 1) { //### caster-specific

				if (condn > 0 && (exp == 'E' || exp == 'U')) {
					// no-op
				}
				else {
					ostr << "Exposure code " << exp
						<< " must be 'E' or 'U'"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
			else if ( caster == 2 || caster == 3 )  {

				if (condn > 0
					&&
					(exp == ' ' || exp == 'E' || exp == 'U' || exp == '1' || exp == '2')) {
					// no-op
				}
				else {
					ostr << "Exposure code '" << exp << "' is invalid"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}

		// Audit Tek and Kote positional planned orders at beginning of cast violations.  Added 03-06-06 k. hubbard per P. Velasco

		{

			int commodcode = (*io)->CICode();
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");
			int heatNumber = (*io)->HeatSeqNum();
			int lotNumber = (*io)->LotNum();

			if (prevWidth == 0
				&&
				heatNumber == 0)
				//				&&
				//					lotNumber == 1 )  // Verification  check for beginning of a Cast!!!

								// important to put this initial update before the check above
								//   for CheckStringHeatPosition

				if (!isStock) {

					if (commodcode == 11   // Added commocode (11 [stringers], 45-50) to Tek/Kote material check 3-17-06 k. hubbard
						||
						commodcode == 12
						||
						commodcode == 17
						||
						commodcode == 18
						||
						commodcode == 19
						||
						commodcode == 20
						||
						commodcode == 21
						||
						commodcode == 45
						||
						commodcode == 46
						||
						commodcode == 47
						||
						commodcode == 48
						||
						commodcode == 49
						||
						commodcode == 50
						||
						commodcode == 72
						||
						commodcode == 73)

					{

						ostr << "Tek and Kote commodity code " << setw(3) << commodcode
							<< " can not be planned at start of a cast"
							<< ends;
						//				ADD_ERR(CCastStringHeatValidnError::FATAL);   // Changed to warning per new 1st cast piece cropping plans. 02/20/07 k. hubbard  
						ADD_ERR(CCastStringHeatValidnError::WARNING);
						isOk = false;
					}
				}
		}


		// audit width change.  Radicals are ok here 12-11-03 k. hubbard

		{

			char code = (*io)->SlitTypeCode();

			if (prevWidth == 0)
				// this is the first we've seen, use it for initial width
				// important to put this initial update before the check above
				//   for CheckStringHeatPosition

				prevWidth = (*io)->SlabWidth();

			Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);

			if (widthDiff > TRANS_WIDTH
				&&
				code == ' ')  //	Consider slit type i.e. Radicals ok. 11-29-05 k. hubbard

			{

				ostr << "Width jump from " << setw(3) << prevWidth
					<< " to " << setw(3) << (*io)->SlabWidth()
					<< " is invalid"
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				//	Had to reverse, not looking at slit type i.e. Radicals ok.	ADD_ERR(CCastStringHeatValidnError::WARNING); Changing transition width violations to Fatal 12-11-03 k. hubbard
				isOk = false;
			}
		}

		////			prevWidth = (*io)->SlabWidth();  //Important !!!! repositioned below width jump audit to work inside this For loop and share between width audits 11-22-05 k. hubbard

				// Maint. Note: K. Hubbard 3-15-06; Add New Fatal here by checking width (*io)->ProvSlabWidthMin & Max

				// Audit Transition 80" Slab Spread Squeeze violations against Provided Cast Widths
		if ((*io)->Order() != 0)
		{
			Width width = (*io)->SlabWidth();
			//			Width ProvWidthMax = (*io)->ProvSlabWidthMax();

			float ProvWidthMin;   // Added 4 lines here for min check 3-17-06 k. hubbard
			CString strProvMin;
			//			strProvMin.Format("%2.1f",(*io)->ProvSlabWidthMin());   Changed line (see next line) to throw reference more reliable Order.h field here for min value 4-5-07 k. hubbard
			strProvMin.Format("%2.1f", (*io)->Order()->ProvSlabWidthMin());
			ProvWidthMin = atof(strProvMin);

			float ProvWidthMax;
			CString strProvMax;
			//			strProvMax.Format("%2.1f",(*io)->ProvSlabWidthMax());   Changed line (see next line) to throw reference more reliable Order.h field here for max value 4-5-07 k. hubbard
			strProvMax.Format("%2.1f", (*io)->Order()->ProvSlabWidthMax());
			ProvWidthMax = atof(strProvMax);

			char code = (*io)->SlitTypeCode();
			bool isTrans = (*io)->IsTransition();
			Width transSpread80 = 1;  //Restored per P. Fronczek maint. 4-18-07 k. hubbard

			//					strProvMax.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
//		m_provCastSlabWidthMax = atof(strProvMax);
//			Width transProvWidthMax = (ProvWidthMax - transSpread80);
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

			if (prevWidth != 0
				&&
				!isStock
				&&
				(*io)->Order() != 0)  // Added line to make sure order exist for more reliable Order.h field here for min/max values 4-5-07 k. hubbard
			{

				//				int transProvWidthMax = (ProvWidthMax - transSpread80);   comm. out 4-5-07 k. hubbard
				Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
				Width NarrowestWidthPoint = min((*io)->SlabWidth(), prevWidth);

				Width transProvWidthMax = (ProvWidthMax - transSpread80);   //Restored per P. Fronczek maint. 4-18-07 k. hubbard


				if (widthDiff == 1
					&&
					(code == ' ')
					&&
					(isTrans)
					&&           // Added or condition here for min check 3-17-06 k. hubbard
					(NarrowestWidthPoint > transProvWidthMax || NarrowestWidthPoint < ProvWidthMin))   //Switched per P. Fronczek maint. 4-18-07 k. hubbard

//					(NarrowestWidthPoint > ProvWidthMax || NarrowestWidthPoint < ProvWidthMin))  //Comm out switched per P. Fronczek maint. 4-18-07 k. hubbard

				{
					ostr << "Transition steel width ("
						<< NarrowestWidthPoint
						<< ") is a violation of"
						<< "80 HSM Spread / Squeeze Prov Min (" << ProvWidthMin
						<< ") or Prov Max (" << ProvWidthMax
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);
					isOk = false;
				}
			}
		}

		// End Maint. Note: K. Hubbard 3-15-06; End maint. of add new fatal here for checking width (*io)->ProvSlabWidthMin & Max



		// Audit Side Slits Minimum Allowance change.  Added 5-24-06 k. hubbard per P. Velasco

		{
			char code = (*io)->SlitTypeCode();
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

			//			Width widthDiff = fabs((*io)->SlabWidth() - prevWidth); 
			//           Modified width difference comparison to check the current lot width to the ordered aim width 02-10-09 k. hubbard per P. Velasco
			//			Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 

					//try {		          Tried this for missing orders on CSOrder.h and assoc hotband info (i.e. ? marks instead of number values ) from CSOrder.h 
			if (!isStock)
			{
				Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 
				if (widthDiff > 0)
				{
					if (widthDiff < 2
						&&
						code == 'S')  //	Check slit type. 
					{
						//						ostr << "Planned Side Slits from " << setw(3) << prevWidth
						ostr << "Planned Side Slits from " << setw(3) << (*io)->SlabWidth()
							<< " to " << setw(3) << (*io)->Order()->SlabWidth()  // Throw reference to reliable Order.h (P-STEEL-MAKE-PLAN) field here for aim value 02-10-09 k. hubbard
							<< " under 2 inches is a dock violation"
							<< ends;
						ADD_ERR(CCastStringHeatValidnError::WARNING);
						isOk = false;
					}
				}
			}
			//}
			 //	catch ( CDBException* e )
			 //		{
			 //		   AfxMessageBox( e->m_strError,   
			 //          MB_ICONEXCLAMATION );
			 //		   e->Delete();
			 //		}
		}

		// End Maint. audit side slit minimum allowance change end 5-24-06 k. hubbard



		// Audit Width Jumps change.  Added 11-29-05 k. hubbard per P. Velasco

		{

			int commodcode = (*io)->CICode();
			char code = (*io)->SlitTypeCode();
			Width transSpread80 = 0.3;
			Width transSqueeze80 = 1.6;
			bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

			//			if ( prevWidth == 0 )
							// this is the first we've seen, use it for initial width
							// important to put this initial update before the check above
							//   for CheckStringHeatPosition

			//				prevWidth = (*io)->SlabWidth(); 

			Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
			Width MinTransHBWidth = fabs(prevWidth - transSqueeze80);
			Width MaxTransHBWidth = fabs(prevWidth + transSpread80);
			//try {		          Tried this for missing orders on CSOrder.h and assoc hotband info (i.e. ? marks instead of number values ) from CSOrder.h 3-13-06 k.hubbard

					//  this program uses to reference the current order's hotband value. 11-29-05 k. hubbard 
			if (!isStock)
			{
				Width hbwidth = (*io)->OrderHotBandWidth();   // Built and used a pointer to COrder.h inside (see field) CSOrder.h which 

				if (widthDiff == 1
					&&
					(commodcode == 65 || commodcode == 66)
					&&
					(hbwidth < MinTransHBWidth || hbwidth > MaxTransHBWidth)
					&&
					code == ' ')  //	Consider slit type i.e. Radicals ok. 

				{

					ostr << "Slab Width Jump from " << setw(3) << prevWidth
						<< " to " << setw(3) << (*io)->SlabWidth()
						<< " is invalid"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			//}
			//	catch ( CDBException* e )
			//		{
			//		   AfxMessageBox( e->m_strError,   
			//          MB_ICONEXCLAMATION );
			//		   e->Delete();
			//		}
		}

		// Audit Width Jumps change end 11-29-05 k. hubbard

		// Audit Stock Reason codes   added audit for stock reason 5-5-09 k. hubbard

		{
			CString stockFaclReason = (*io)->StockReason();
			CString stockCommReason = (*io)->StockCommercialReason();
			// FP Change
			//if ( (*io)->OrderNum() == 9999999 ) {
			if ((*io)->FpOrderNum().Left(7) == "9999999")
			{

				if (stockFaclReason == ""
					||
					stockFaclReason == "0"
					||
					stockFaclReason == "<blank>"
					||
					stockCommReason == ""
					||
					stockCommReason == "0"
					||
					stockCommReason == "<blank>")
				{

					ostr << "stock facility reason '" << setw(20) << LPCTSTR(stockFaclReason)
						<< " ' or stock commercial reason '" << setw(20) << LPCTSTR(stockCommReason)
						<< " ' must not be blank "
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning for Dofasco 
					isOk = false;
				}

			}
		}

		// Audit Slit Reason Code  added audit for slit reason 2-26-10 k. hubbard

		{
			char code = (*io)->SlitTypeCode();
			CString slitFaclReason = (*io)->SlitReason();

			if (code == 'E'
				||
				code == 'S'
				||
				code == 'R')
			{

				if (slitFaclReason == ""
					||
					slitFaclReason == "0"
					||
					slitFaclReason == "<blank>")
				{
					ostr << "slit facility reason '" << setw(20) << LPCTSTR(slitFaclReason)
						<< " ' must not be blank "
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning 
					isOk = false;
				}
			}
		}

		// Audit 2BOF inward width change designs.  1-28-10 k. hubbard

		{
		//######### Not clear on width change rules for 4 and 5 (1.6 in documentation)
		//######### KH Answer: Ignore for casters 4 and 5 today. In the future some grades will be inward only, but not today.
		if (( caster == 2 || caster == 3) //### caster-specific
			&&
			 prevWidth > 0)
			{

				if ((*io)->SlabWidth() > prevWidth)
					//				&&
					//				code == ' ' )  //	Consider slit type i.e. Radicals ok. 

				{
					ostr << "Width jump from " << setw(3) << prevWidth
						<< " to " << setw(3) << (*io)->SlabWidth()
						<< " is invalid 2BOF outward jump"
						<< ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					//	Had to reverse, not looking at slit type i.e. Radicals ok.	ADD_ERR(CCastStringHeatValidnError::WARNING); Changing transition width violations to Fatal 12-11-03 k. hubbard
					isOk = false;
				}
			}
		}


		//     **** !!!!!! Caution the variable below is shared between 2 width audits above and must get populated once after both or else one audit above will fail!!!! 11-29-05 k. hubbard
		prevWidth = (*io)->SlabWidth();  // Important CAUTION!!!! repositioned here to work inside this For loop and share between width change and width jump audits 11-29-05 k. hubbard


		// audit change indicator

		{
			if ((*io)->ChangeMarked()) {
				ostr << "Change indicator has not been removed."
					<< ends;
				ADD_ERR(CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
		}
	}

	return isOk;
}

bool CCastStringValidator::FixHeatSpec3SP(CCastStringHeat& rHeat, int caster) { //### caster-specific
	int strandNum = 1; // only one strand

	// At the end?
	if (rHeat.StrandBegin(strandNum) == rHeat.StrandEnd(strandNum))
		return true;

	// Set the order.
	CCSOrder* pOrder = NULL;
	if (rHeat.StrandBegin(strandNum) != rHeat.StrandEnd(strandNum))
		pOrder = *rHeat.StrandBegin(strandNum);

	const CString& orderedSpec = pOrder->LotSpec();
	CString heatSpec = rHeat.Spec();

	// Extend the fixing of the HeatSpec.
	bool result = FixHeatSpec3SP(heatSpec, orderedSpec, caster); //### caster-specific
	if (!result) {
		ostrstream ostr;
		ostr << "Unable to find heat spec:"
			<< "\n  Original heat spec: " << LPCTSTR(rHeat.Spec())
			<< "\n  First ordered spec: " << LPCTSTR(orderedSpec)
			<< "\n  Converted heat spec: " << LPCTSTR(heatSpec)
			<< ends;
		AfxMessageBox(ostr.str());
		ostr.freeze(false);
		return false;
	}

	rHeat.SetSpec(heatSpec, caster); //### caster-specific
	return true;
}

//### For now, entire body is copy of old version for casters 1, 2, and 3
bool CCastStringValidator::FixHeatSpec3SP(CString& heatSpec, const CString& orderedSpec, int caster) { //### caster-specific
	if (orderedSpec.GetLength() != 7)
		return false;
	if (heatSpec.GetLength() != 7)
		return false;

	char ord4 = orderedSpec[3];  // compare fourth digit of individual lot specs within the heat spec. k. hubbard
	char ord6 = orderedSpec[5];  // compare sixth digit of individual lot specs within the heat spec. k. hubbard

	//#### What should these digits be?
	if (caster == 2 || caster == 3) { //### caster-specific

		if (ord4 == '2' || ord4 == '6')
			heatSpec.SetAt(3, ord4);
		else
			heatSpec.SetAt(3, '2');

		if (ord6 == '4')
			heatSpec.SetAt(5, '0');
	}
	else {

		CString front = heatSpec.Left(5);
		if (front == "52440" || front == "52442" || front == "52449") {
			// do nothing.     Added 52442xx spec above, to the exception list of fourth digit modifications. maint. 5/30/06 k. hubbard
		}
		else {
			if (ord4 == '7' || ord4 == '4' || ord4 == '6')
				heatSpec.SetAt(3, ord4);
			else
				heatSpec.SetAt(3, '7');

			if (ord4 == '4')
				heatSpec.SetAt(5, '4');
		}
	}

	CSpec* pSpec = TheSnapshot.SpecMgr().FindSpecMaybe(heatSpec, caster); //### caster-specific
	return pSpec != 0;
}

bool CCastStringValidator::FixHeatSpec(CCastStringHeat& rHeat, int caster) //### caster-specific
{
	if (rHeat.StrandBegin(1) == rHeat.StrandEnd(1)
		&&
		rHeat.StrandBegin(2) == rHeat.StrandEnd(2))

		return true;

	CCSOrder* pOrder;

	if (rHeat.StrandBegin(1) != rHeat.StrandEnd(1))
		pOrder = *rHeat.StrandBegin(1);
	else
		pOrder = *rHeat.StrandBegin(2);

	const CString& orderedSpec = pOrder->LotSpec();
	CString heatSpec = rHeat.Spec();

	bool result = FixHeatSpec(heatSpec, orderedSpec, caster); //### caster-specific

	if (!result) {
		ostrstream ostr;
		ostr << "Unable to find heat spec:"
			<< "\n  Original heat spec: " << LPCTSTR(rHeat.Spec())
			<< "\n  First ordered spec: " << LPCTSTR(orderedSpec)
			<< "\n  Converted heat spec: " << LPCTSTR(heatSpec)
			<< ends;
		AfxMessageBox(ostr.str());
		ostr.freeze(false);
		return false;
	}

	rHeat.SetSpec(heatSpec, caster); //### caster-specific
	return true;
}


// static 
//############## Heat spec rules for 4 and 5 are not clear.
bool CCastStringValidator::FixHeatSpec(CString& heatSpec,
	const CString& orderedSpec,
	int caster) //### caster-specific
{
	if (orderedSpec.GetLength() != 7)
		return false;

	if (heatSpec.GetLength() != 7)
		return false;

	char ord4 = orderedSpec[3];  // compare fourth digit of individual lot specs within the heat spec. k. hubbard
	char ord6 = orderedSpec[5];  // compare sixth digit of individual lot specs within the heat spec. k. hubbard

	if (caster == 2 || caster == 3) { //### caster-specific

		if (ord4 == '2' || ord4 == '6')
			heatSpec.SetAt(3, ord4);
		else
			heatSpec.SetAt(3, '2');

		if (ord6 == '4')
			heatSpec.SetAt(5, '0');
	}
	else {

		CString front = heatSpec.Left(5);
		if (front == "52440" || front == "52442" || front == "52449") {
			// do nothing.     Added 52442xx spec above, to the exception list of fourth digit modifications. maint. 5/30/06 k. hubbard
		}
		else {
			if (ord4 == '7' || ord4 == '4' || ord4 == '6')
				heatSpec.SetAt(3, ord4);
			else
				heatSpec.SetAt(3, '7');

			if (ord4 == '4')
				heatSpec.SetAt(5, '4');
		}
	}

	CSpec* pSpec = TheSnapshot.SpecMgr().FindSpecMaybe(heatSpec, caster); //### caster-specific

	return pSpec != 0;
}



bool CCastStringValidator::Validate340080Weights()
{
	// this validated heat weight in range.
	// we have done this elsewhere

	return true;
}





bool CCastStringValidator::AuditSlabLengthData(CCSOrder* pOrder)
{

	if (pOrder->Order() == 0)
		return false;

	{
		CString code = pOrder->Order()->SlabUnitCd();

		if (code != "1"
			&&
			code != "2"
			&&
			code != "3")
			return false;
	}

	// test on PROV_HOT_MILL_CD == 8
	// we already filter for that.

	// test on SM_PLAN_MOLD_TYPE_CD
	// we already filter for that

	{
		if (pOrder->Order()->CustCoilReq() < 4)
			return false;
	}


	{
		Weight wgt = pOrder->Order()->ProvCastSlPcWgt();

		if (wgt < 6000 || wgt > 79999)
			return false;
	}

	{
		Width hbw = pOrder->Order()->HbWidth();

		if (hbw < 18 || hbw > 79.99)
			return false;
	}

	{
		Width ow = pOrder->Order()->OrderedWidth();

		if (ow > 79.99)
			return false;
	}

	{
		float yld = pOrder->Order()->ProvHbYldFctr();

		if (yld < 1 || yld > 1.75)
			return false;
	}

	{
		Weight cw = pOrder->Order()->CustPieceWgt();

		if (cw < 5000 || cw > 79999)
			return false;
	}

	{
		Weight piw = pOrder->Order()->ProvPiwWgt();

		if (piw < 20000 || piw > 79000)
			return false;
	}

	{
		if (pOrder->DispCode() == 3)
			return false;
	}

	return true;
}





void CCastStringValidator::ComputeSlabLength(int caster,	//### caster-specific
	CCSOrder* pOrder,
	bool isLenOk,
	Length& minL,
	Length& maxL)
{
	if (isLenOk) {
		CCSOrder::CalculateMinMaxLengths(pOrder->Order(),
			caster,
			pOrder->SlabWidth(),
			minL,
			maxL);

	}
	else {        // Assign min max slab length when the order min/max is zero. 3-21-06 k. hubbard 

//		minL = pOrder->SlabLength();
		maxL = pOrder->SlabLength();
		minL = maxL * 0.80;      // Testing comm. out if statement below and replaced with 80% 9-28-07 k. hubbard  
//		if (caster == 1)         // 4BOF physical constraint min slab length limit
//			minL = 220;
//		else
//			minL = 172;	         // 2BOF physical constraint min slab length limit	
	}




#if 0
	// old DPA version, being rewritten
	//  delete, eventually
	if (isLenOk) {

		CCSOrder::U0340050Parms parms;

		// if we are here, we know pOrder->Order() != 0;

		COrder* po = pOrder->Order();

		assert(po != 0);

		//parms.autoCode				= po->AutomotiveCode();
		//parms.provCastSlPcWgt		= po->ProvCastSlPcWgt();
		//parms.hbWidth				= po->HbWidth();
		//parms.custPieceWgt			= po->CustPieceWgt();
		//parms.provCastSlLength		= pOrder->SlabLength();
		parms.m_provCastSlWidth = pOrder->SlabWidth();
		parms.m_provCastSlThick = pOrder->SlabThickness();
		//parms.provHbYldFctr			= po->ProvHbYldFctr();
		//parms.provSlabLength		= po->SlabLength();

		if (po->IsCMS()) {

			if (po->ProvCastSlLnth() >= 365) {

				pOrder->SlabLength(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
				minL = GlobalConstants.CasterSlabLengthMax(caster);                //### caster-specific
				maxL = GlobalConstants.CasterSlabLengthMax(caster);                //### caster-specific
				return;
			}
			else {
				parms.m_provCastMinSlabWgt = po->MinSlabWgt() * (1 + po->CmsYieldLossFactor());
				parms.m_provCastMaxSlabWgt = po->MaxSlabWgt() * (1 + po->CmsYieldLossFactor());
			}
		}
		else {
			parms.m_provCastMinSlabWgt = po->MinSlabWgt();
			parms.m_provCastMaxSlabWgt = po->MaxSlabWgt();
		}

		CCSOrder::U0340050(parms, minL, maxL);
	}
	else {

		minL = pOrder->SlabLength();
		maxL = pOrder->SlabLength();
	}

#endif // old, delete

}

