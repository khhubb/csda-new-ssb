//### validation functions called with a caster-specific value: for now, left as an int

//## caster 1 explicitly, the others only implicitly
bool CCastStringValidator::IsNumberOfPiecesOk(vector<CCSOrder*>::const_iterator io, int caster) {
  bool isOk = true;
  int numPieces = (*io)->NumPieces();

  if (caster == 1) { //### caster-specific
    if (numPieces == 0 || (numPieces > 32 && numPieces != 99)) {
      ostr << "#PC must be < 33" << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      isOk = false;
    }
  }
  else if (numPieces == 0) {
    ostr << "#PC invalid" << ends;
    ADD_ERR(CCastStringHeatValidnError::FATAL);
    isOk = false;
  }
  return isOk;
}

//## Checks caster 3 only, except for trivial check for empty width on the others.
bool CCastStringValidator::IsSteelWidthOk(vector<CCSOrder*>::const_iterator io, int caster) {
  bool isOk = true;
  Width width = (*io)->SlabWidth();

  if (caster == 3) { //### caster-specific
    if ( width < 24 || width > 60) {
      ostr << "steel width (" << setw(2) << width << ") not in range [24,60]" << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      isOk = false;
    }
  }
  else if (width == 0) {
    ostr << "steel width (" << setw(2) << width << ") is invalid" << ends;
    ADD_ERR(CCastStringHeatValidnError::FATAL);
    isOk = false;
  }
  return isOk;
}

bool CCastStringValidator::IsSteelLengthOk(vector<CCSOrder*>::const_iterator io, int caster) {
  bool isOk = true;

  if ((*io)->PlanSteelLengthMin() == 0 || (*io)->PlanSteelLengthMax() == 0) {
    if ((*io)->FpOrderNum().Left(7) == "9999999") {
      CString spec3 = (*io)->LotSpec().Left(3);
      // Shouldn't this be a general check for CMS (condn = 5XXX or 8XXX)???
      if ((*io)->SlabCondnCode() == 5782 &&
	  // Added for Nancy Hake Honda trials 6-15-09 k. hubbard 
	  (spec3 == "520" || spec3 == "524" || spec3 == "726" || 
	   spec3 == "811" || spec3 == "820" || spec3 == "822" || 
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
      if ( (*io)->Order() != 0) {
	isOk = AuditSlabLengthData((*io));  //### in original isLenOk, but doesn't seem to be used anywhere

	Length minL; //### passed by reference to ComputeSlabLength
	Length maxL; //### passed by reference to ComputeSlabLength	
	ComputeSlabLength(caster, (*io), isOk, minL, maxL); //### caster-specific
	(*io)->PlanSteelLengthMin(minL);
	(*io)->PlanSteelLengthMax(maxL);
      }
    }
  }
  return isOk;
}

//## caster 3 only
bool CCastStringValidator::IsHeatPositionOfCombi3Ok(vector<CCSOrder*>::const_iterator io, int caster) {
  //  check heat position for 3 combi startup slabs
  bool isOk = true;
  if ( caster == 3 ) { //### caster-specific
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
    if ( prevWidth == 0  ) { // 1st lot in strand
      int condn = (*io)->SlabCondnCode();
      //	  if ( condn == 4682 || condn == 4782 ) {  comm. out low quality exception check 7-1-04 k. hubbard   
      //		  // no op
      //	  } 
      //	  else if ( (*io)->SlabLength() > 315 ) {  changed per cast rules 6-30-04 k.hubbard
      if ( (*io)->SlabLength() > 362 ) {
	ostr << "Slab length " << setw(3) << (*io)->SlabLength() 
	     << " > 362 in 1st slab."    // changed from 315 per cast rules 6-30-04 k.hubbard
	     << ends;
	//							ADD_ERR(CCastStringHeatValidnError::FATAL);
	ADD_ERR(CCastStringHeatValidnError::WARNING);  // changed per J. Sarb-Beer 9-29-08 k. hubbard
	isOk = false;
      }   // 362" length check
    }     // 1st slab startup check 
          //				}           comm out 7-01-04 k. hubbard // spec check      
  }               // 3 combi caster check
  return isOk;
} 

//## Checks casters 1, 2, and 3
bool CCastStringValidator::IsSteelLengthComplianceOk(vector<CCSOrder*>::const_iterator io, int caster) {         
  bool isOk = true;
  Length len = (*io)->SlabLength();

  // Added 1 Slab Caster length check 2-4-10 k. hubbard  
  if (caster == 1) { //### caster-specific
    if (len > GlobalConstants.CasterSlabLengthMax(caster)) { //### caster-specific
      ostr << "Slab length " << setw(3) << len 
	   << " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
	   << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      return false;
    }
  }
  
  // Added 2 Slab Caster length check 2-4-10 k. hubbard  
  if (caster == 2) { //### caster-specific
    // if ( len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171 ) { //### caster-specific
    if (len > GlobalConstants.CasterSlabLengthMax(caster)) { //### caster-specific
      ostr << "Slab length " << setw(3) << len 
	   << " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
	   << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      return false;
    }
  }
  else {
    if (caster == 3) { //### caster-specific
      if ( len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171 ) { //### caster-specific
	ostr << "Slab length " << setw(3) << len 
	     << " not between steelshop length boundaries 171 and " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
	     << ends;
	ADD_ERR(CCastStringHeatValidnError::FATAL);
	return false;
      }
    }
  }
  //			else {
  if ((*io)->SlabLength() == 0) {
    ostr << "Slab length 0 is invalid." << ends;
    ADD_ERR(CCastStringHeatValidnError::FATAL);
    isOk = false;
  }
  //			}
  return isOk;
}

bool CCastStringValidator::IsExposureCodeOk(vector<CCSOrder*>::const_iterator io, int caster) {
  bool isOk = true;
  char exp = (*io)->ExposureCode();
  int condn = (*io)->SlabCondnCode();

  if (caster == 1) { //### caster-specific
    if (condn > 0 && ( exp == 'E' || exp == 'U')) {
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
  else  {
    if (condn > 0
	&&
	(exp == ' ' || exp == 'E' || exp == 'U' || exp == '1' || exp == '2')) {
      // no-op
    }
    else {
      ostr << "Exposure code '" << exp << "' is invalid" << ends;
      ADD_ERR(CCastStringHeatValidnError::WARNING);
      isOk = false;
    }
  }
  return isOk;
}

bool CCastStringValidator::Is2BOFInwardWidthChangeOk(vector<CCSOrder*>::const_iterator io, int caster) {
  bool isOk = true;

  if ((caster == 2 || caster == 3) && prevWidth > 0) {
    if ((*io)->SlabWidth() > prevWidth)  {
      //				&&
      //				code == ' ' )  //	Consider slit type i.e. Radicals ok. 
      ostr << "Width jump from " << setw(3) << prevWidth
	   << " to " << setw(3) << (*io)->SlabWidth()
	   << " is invalid 2BOF outward jump"
	   << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      //  Had to reverse, not looking at slit type i.e. Radicals ok.	
      // ADD_ERR(CCastStringHeatValidnError::WARNING); Changing transition width violations to Fatal 12-11-03 k. hubbard
      isOk = false;
    }
  }
  return isOk;
}

//### validation functions without a caster-specific argument
bool CCastStringValidator::IsSlitTypeCodeOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  char code = (*io)->SlitTypeCode();

  if (code != 'D' && // Added noslit type code here on xxx4 condition codes 6-27-05 k. hubbard  
      code != 'E' &&
      code != 'F' &&
      code != 'S' &&
      code != 'R' &&
      code != ' ') {
    ostr << "invalid slit type code = " << code << ends;
    ADD_ERR(CCastStringHeatValidnError::WARNING);
    isOk = false;
  }
  return isOk;
}

bool CCastStringValidator::IsWidthTransitionOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  Width width = (*io)->SlabWidth();
  char code   = (*io)->SlitTypeCode();
  bool isTrans = (*io)->IsTransition();
  bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");
  
  if ((*io)->Order() != 0  && !isStock) {
    Width provWidthMax = (*io)->Order()->ProvSlabWidthMax();
    Width provWidthMin = (*io)->Order()->ProvSlabWidthMin();

    //### in original, two occurrences of '=' where '==' is meant
    //### code simplified to eliminate nested ifs
    if ((code == ' ') && !isTrans && (width > provWidthMax)) {
      ostr << "steel width (" << width << ") must not" << "be > Prov Max" << provWidthMax << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);
      isOk = false;
    }
  }
  return isOk;
}

bool CCastStringValidator::IsStrandNumOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
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
  return isOk;
}

bool CCastStringValidator::IsPlannedLengthOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;

  if ((*io)->FpOrderNum().Left(7) != "9999999") {
    if ((*io)->Order() != 0 && ((*io)->Order()->CustCoilReq() == 8 || (*io)->Order()->CustCoilReq() == 9)) {
      // no-op
    }
    else if (long ((*io)->SlabLength()+ 0.5) < long ((*io)->PlanSteelLengthMin()+ 0.5) ||
	     long ((*io)->SlabLength()+ 0.5) > long ((*io)->PlanSteelLengthMax()+ 0.5)) {
      ostr << "slab length " << long((*io)->SlabLength()+ 0.5) << " should be between " << long((*io)->PlanSteelLengthMin()+0.5)
	   << " and " << long((*io)->PlanSteelLengthMax()+ 0.5)<< ends;
      ADD_ERR(CCastStringHeatValidnError::WARNING);  // temp switched from fatal to warning for Dofasco 
      // order campaign per P. Fronczek. maint k. hubbard 11-11-04. 					                              
      // ADD_ERR(CCastStringHeatValidnError::FATAL);
      isOk = false;
    }
  }
  return isOk;
}

bool CCastStringValidator::IsTekKotePositionalOrderOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  int commodcode = (*io)->CICode();
  bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );
  int heatNumber = (*io)->HeatSeqNum();
  int lotNumber = (*io)->LotNum();

  if (prevWidth == 0 && heatNumber == 0)
    //				&&
    //					lotNumber == 1 )  // Verification  check for beginning of a Cast!!!
    // important to put this initial update before the check above
    //   for CheckStringHeatPosition
    if (! isStock) { 
      // Added commocode (11 [stringers], 45-50) to Tek/Kote material check 3-17-06 k. hubbard
      if (commodcode == 11 ||
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
	  commodcode == 73)
	{
	  ostr << "Tek and Kote commodity code " << setw(3) << commodcode
	       << " can not be planned at start of a cast" 
	       << ends;
	  //  ADD_ERR(CCastStringHeatValidnError::FATAL);   // Changed to warning per new 1st cast piece cropping plans. 02/20/07 k. hubbard  
	  ADD_ERR(CCastStringHeatValidnError::WARNING);
	  isOk = false;
	}
    }
  return isOk;
}

bool CCastStringValidator::IsWidthChangeOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  char code = (*io)->SlitTypeCode();

  if (prevWidth == 0)
    // this is the first we've seen, use it for initial width
    // important to put this initial update before the check above
    // for CheckStringHeatPosition
    prevWidth = (*io)->SlabWidth();

  Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
  if (widthDiff > TRANS_WIDTH && code == ' ')  { //	Consider slit type i.e. Radicals ok. 11-29-05 k. hubbard
    ostr << "Width jump from " << setw(3) << prevWidth
	 << " to " << setw(3) << (*io)->SlabWidth()
	 << " is invalid"
	 << ends;
    ADD_ERR(CCastStringHeatValidnError::FATAL);
    //	Had to reverse, not looking at slit type i.e. Radicals ok.	ADD_ERR(CCastStringHeatValidnError::WARNING); Changing transition width violations to Fatal 12-11-03 k. hubbard
    isOk = false;
  }
  return isOk;
}

bool CCastStringValidator::Is80InSpreadSqueezeOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  if ((*io)->Order() != 0) {
    Width width = (*io)->SlabWidth();
    // Width ProvWidthMax = (*io)->ProvSlabWidthMax();

    float ProvWidthMin;   // Added 4 lines here for min check 3-17-06 k. hubbard
    CString strProvMin;
    //	strProvMin.Format("%2.1f",(*io)->ProvSlabWidthMin());   Changed line (see next line) to throw reference more reliable Order.h field here for min value 4-5-07 k. hubbard
    strProvMin.Format("%2.1f",(*io)->Order()->ProvSlabWidthMin());
    ProvWidthMin = atof(strProvMin);  

    float ProvWidthMax;  
    CString strProvMax;
    //			strProvMax.Format("%2.1f",(*io)->ProvSlabWidthMax());   Changed line (see next line) to throw reference more reliable Order.h field here for max value 4-5-07 k. hubbard
    strProvMax.Format("%2.1f",(*io)->Order()->ProvSlabWidthMax()); 
    ProvWidthMax = atof(strProvMax);    
			
    char code   = (*io)->SlitTypeCode();
    bool isTrans = (*io)->IsTransition();
    Width transSpread80 = 1;  //Restored per P. Fronczek maint. 4-18-07 k. hubbard

    //					strProvMax.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
    //		m_provCastSlabWidthMax = atof(strProvMax);
    //			Width transProvWidthMax = (ProvWidthMax - transSpread80);
    bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");
    if  (prevWidth != 0 && !isStock && (*io)->Order() != 0) { // Added line to make sure order exist for more reliable Order.h field here for min/max values 4-5-07 k. hubbard
      //  int transProvWidthMax = (ProvWidthMax - transSpread80);   comm. out 4-5-07 k. hubbard
      Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
      Width NarrowestWidthPoint = min((*io)->SlabWidth(), prevWidth);
      Width transProvWidthMax = (ProvWidthMax - transSpread80);   //Restored per P. Fronczek maint. 4-18-07 k. hubbard

      // Added or condition here for min check 3-17-06 k. hubbard
      if  (widthDiff == 1 && (code == ' ') && (isTrans) &&           
	   (NarrowestWidthPoint > transProvWidthMax || NarrowestWidthPoint < ProvWidthMin))   //Switched per P. Fronczek maint. 4-18-07 k. hubbard
	// (NarrowestWidthPoint > ProvWidthMax || NarrowestWidthPoint < ProvWidthMin))  //Comm out switched per P. Fronczek maint. 4-18-07 k. hubbard
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
  return isOk;
}

bool CCastStringValidator::IsSideSlittingOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  char code = (*io)->SlitTypeCode();
  bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

  //  Width widthDiff = fabs((*io)->SlabWidth() - prevWidth); 
  //  Modified width difference comparison to check the current lot width to the ordered aim width 02-10-09 k. hubbard per P. Velasco
  //  Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 

  if (!isStock) {
    Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 
    if (widthDiff > 0)
      { 
	if (widthDiff < 2 && code == 'S')  //	Check slit type. 
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
  return isOk;
}

bool CCastStringValidator::IsWidthJumpingOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  int commodcode = (*io)->CICode();
  char code = (*io)->SlitTypeCode();
  Width transSpread80 = 0.3;
  Width transSqueeze80 = 1.6;
  bool isStock = ((*io)->FpOrderNum().Left(7) == "9999999");

  // if ( prevWidth == 0 )
  // this is the first we've seen, use it for initial width
  // important to put this initial update before the check above
  // for CheckStringHeatPosition
  // prevWidth = (*io)->SlabWidth(); 

  Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
  Width MinTransHBWidth = fabs(prevWidth - transSqueeze80);
  Width MaxTransHBWidth = fabs(prevWidth + transSpread80);
  //  this program uses to reference the current order's hotband value. 11-29-05 k. hubbard 
  if (!isStock)
    { 
      Width hbwidth = (*io)->OrderHotBandWidth();   // Built and used a pointer to COrder.h inside (see field) CSOrder.h which 
			
      if (widthDiff == 1 && (commodcode == 65 || commodcode == 66) &&
	  (hbwidth < MinTransHBWidth  ||  hbwidth > MaxTransHBWidth) && code == ' ')  // Consider slit type i.e. Radicals ok. 
	{
	  ostr << "Slab Width Jump from " << setw(3) << prevWidth
	       << " to " << setw(3) << (*io)->SlabWidth()
	       << " is invalid"
	       << ends;
	  ADD_ERR(CCastStringHeatValidnError::FATAL);
	  isOk = false;
	}
    }
  return isOk;
}

bool CCastStringValidator::IsStockReasonOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  CString stockFaclReason = (*io)->StockReason();
  CString stockCommReason = (*io)->StockCommercialReason();
  // FP Change
  //if ( (*io)->OrderNum() == 9999999 ) {
  if ((*io)->FpOrderNum().Left(7) == "9999999") {
      if (stockFaclReason == "" ||
	  stockFaclReason == "0" ||
	  stockFaclReason == "<blank>" ||
	  stockCommReason == ""  ||
	  stockCommReason == "0" ||
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
  return isOk;
}

bool CCastStringValidator::IsSlitReasonOk(vector<CCSOrder*>::const_iterator io) {
  bool isOk = true;
  char code = (*io)->SlitTypeCode();
  CString slitFaclReason = (*io)->SlitReason();

  if (code == 'E' || code == 'S' || code == 'R') {
    if (slitFaclReason == "" || slitFaclReason == "0" || slitFaclReason == "<blank>") {
      ostr << "slit facility reason '" << setw(20) << LPCTSTR(slitFaclReason)
	   << " ' must not be blank " 
	   << ends;
      ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning 
      isOk = false;
    }
  }
  return isOk;
}

bool CCastStringValidator::Validate340080(int strandNum) 
{
  bool isOk = true;
  ostrstream ostr;

  int caster = m_pCastString->Id().Caster(); //### caster-specific

  const vector<CCSOrder*>& strand = m_pCastString->Strand(strandNum);

  if (strand.size() == 0)
    // nothing to check
    return true;

  if (strandNum == 2 && Caster::NumStrandsOnCaster(caster) == 1) { //### caster-specific
    ostr << "Caster " << caster << ": There are orders on strand #2!" << ends;
    //AddValidationString(ostr);
    AddValidnError(-1,0,-1,ostr,CCastStringHeatValidnError::FATAL);
    return false;
  }

  Width prevWidth = 0;
  for (vector<CCSOrder*>::const_iterator io = strand.begin(); io != strand.end(); ++io) {
    CCastStringHeat& rHeat = (*m_pHeats)[ (*io)->HeatSeqNum() ];
    if (!rHeat.IsMarked())
      continue;

    // audit lot spec -- we should already have this ok'd
    //  We need to have some fun with certain digits of the spec.
    FixHeatSpec( rHeat, caster ); //### caster-specific

    //### predicates to replace code blocks below
    isOk = IsNumberOfPiecesOk(io, caster) && 
      IsSlitTypeCodeOk(io) && 
      IsSteelWidthOk(io, caster);  //### and so on
    
    // audit numPieces
    /** ###
	{
			int numPieces = (*io)->NumPieces();
			
			if ( caster == 1 ) { //### caster-specific

				if ( numPieces == 0 
					 || 
					 (numPieces > 32 && numPieces != 99) ) {
					ostr << "#PC must be < 33"
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else if ( numPieces == 0 ) {
				ostr << "#PC invalid"
					 << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}
		### **/

		// audit slit type code
		/** ###
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
				code != ' ' ) {
				
				ostr << "invalid slit type code = "
					 << code 
					 << ends;
				ADD_ERR(CCastStringHeatValidnError::WARNING);
				isOk = false;
			}
		}
		### **/

		// audit steel width
		/** ###
		{
			Width width = (*io)->SlabWidth();

			if ( caster == 3 ) { //### caster-specific

				if ( width < 24 || width > 60 ) {
					ostr << "steel width ("
						 << setw(2) << width
						 << ") not in range [24,60]"
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else if ( width == 0 ) {
				ostr << "steel width ("
					 << setw(2) << width
					 << ") is invalid"
					 << ends;
				ADD_ERR(CCastStringHeatValidnError::FATAL);
				isOk = false;
			}
		}
		## **/

// Note: K. Hubbard 12-11-03; Add New Fatal here by checking width (*io)->ProvSlabWidthMin & Max

		// audit non Transition Provided Steel widths
		/** ###
		{
			Width width = (*io)->SlabWidth();
			char code   = (*io)->SlitTypeCode();
			bool isTrans = (*io)->IsTransition();
//			Width provWidthMax = (*io)->ProvSlabWidthMax();
			bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );

			if ( (*io)->Order() != 0 
			 	&&
			 ! isStock )
				{
					Width provWidthMax = (*io)->Order()->ProvSlabWidthMax();
					Width provWidthMin = (*io)->Order()->ProvSlabWidthMin();

				if  (code = ' ') {
			//	&&
					if	(isTrans = false) {
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
			### **/

// End Note: K. Hubbard 12-11-03; End maint. of add new fatal here for checking width (*io)->ProvSlabWidthMin & Max


		// audit min/max steel length
		/** ###
		{
			if ((*io)->PlanSteelLengthMin() == 0
				||
				(*io)->PlanSteelLengthMax() == 0 ) {

				// FP Change
				//if ( (*io)->OrderNum() == 9999999 ) {
				if ( (*io)->FpOrderNum().Left(7) == "9999999" ) {

					CString spec3 = (*io)->LotSpec().Left(3);
					
					// Shouldn't this be a general check for CMS (condn = 5XXX or 8XXX)???
					
					if ( (*io)->SlabCondnCode() == 5782
						 &&
						 ( spec3 == "520"
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
						   spec3 == "826" ) ) {

						// avoid impractical 5% CMS increase if plan steel length >= 365
						// assumption: 384 is max length
						// 384 - 5% = 365

						if ( (*io)->SlabLength() >= 365 ) {

							(*io)->PlanSteelLengthMin(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
							(*io)->PlanSteelLengthMax(GlobalConstants.CasterSlabLengthMax(caster)); //### caster-specific
						}

						// shouldn't we have an else here?

					}
					else {
						
						(*io)->PlanSteelLengthMin( (*io)->SlabLength() );
						(*io)->PlanSteelLengthMax( (*io)->SlabLength() );
					}
				}
				else {

					if ( (*io)->Order() != 0 ) {

						bool isLenOk = AuditSlabLengthData((*io));
	
						Length minL;
						Length maxL;
	
						ComputeSlabLength( caster, (*io), isLenOk, minL, maxL ); //### caster-specific
						(*io)->PlanSteelLengthMin( minL );
						(*io)->PlanSteelLengthMax( maxL );
					}
				}
			}
		}
		### **/

		// audit planned length

		// {
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
		  /** ##
			if ( (*io)->FpOrderNum().Left(7) != "9999999" ) {
 
				if ( (*io)->Order() != 0
					 &&
					 ((*io)->Order()->CustCoilReq() == 8
					  ||
					  (*io)->Order()->CustCoilReq() == 9 ) ){

					// no-op
				}
				else if ( long((*io)->SlabLength()+0.5) < long((*io)->PlanSteelLengthMin()+0.5)
					      ||
						  long((*io)->SlabLength()+0.5) > long((*io)->PlanSteelLengthMax()+0.5) ) {

					ostr << "slab length " << long((*io)->SlabLength()+0.5)
						 << " should be between " << long((*io)->PlanSteelLengthMin()+0.5)
						 << " and " << long((*io)->PlanSteelLengthMax()+0.5)
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::WARNING);  // temp switched from fatal to warning for Dofasco 
//                          order campaign per P. Fronczek. maint k. hubbard 11-11-04. 					                              
//					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			}
			## **/
		  /**###
		//  check heat position for 3 combi startup slabs
		{
			if ( caster == 3 ) { //### caster-specific

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

					 if ( prevWidth == 0  ) { // 1st lot in strand

						int condn = (*io)->SlabCondnCode();

//						if ( condn == 4682 || condn == 4782 ) {  comm. out low quality exception check 7-1-04 k. hubbard   
//							// no op
//						} 

//						else if ( (*io)->SlabLength() > 315 ) {  changed per cast rules 6-30-04 k.hubbard
						if ( (*io)->SlabLength() > 362 ) {

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
		## **/

    /** ##
		// audit steel length compliance with steelshop limitations
		{
			Length len = (*io)->SlabLength();

//            Added 1 Slab Caster length check 2-4-10 k. hubbard  
			if ( caster == 1 ) { //### caster-specific
				if ( len > GlobalConstants.CasterSlabLengthMax(caster) ) { //### caster-specific
					ostr << "Slab length " << setw(3) << len 
						 << " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
//            Added 2 Slab Caster length check 2-4-10 k. hubbard  
			if ( caster == 2 ) { //### caster-specific
//  				if ( len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171 ) { //### caster-specific
				if ( len > GlobalConstants.CasterSlabLengthMax(caster) ) { //### caster-specific
					ostr << "Slab length " << setw(3) << len 
						 << " not within steelshop length boundary " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
				}
			}
			else {

				if ( caster == 3 ) { //### caster-specific

					if ( len > GlobalConstants.CasterSlabLengthMax(caster) || len < 171 ) { //### caster-specific
						ostr << "Slab length " << setw(3) << len 
						 << " not between steelshop length boundaries 171 and " << GlobalConstants.CasterSlabLengthMax(caster) //### caster-specific
						 << ends;
						ADD_ERR(CCastStringHeatValidnError::FATAL);
						isOk = false;
					}
				}
			}
//			else {

			if ( (*io)->SlabLength() == 0 ) {

					ostr << "Slab length 0 is invalid." 
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);
					isOk = false;
			}
//			}
		}
		## **/


		// audit strand num
		// no need - we can't get this wrong

		// Reinstalled 8/8/03 K. Hubbard 
		// New: CastStringValidator.cpp for special condition code assignments; Maint. added by K. Hubbard 8-5-03  
		// audit ordered condition code for special non-slit instructions. - per Larry Thomas, Quality Design Engineer (QDE). 
    /** ##
		{

			char slitCode = (*io)->SlitTypeCode();

			int condnCode = (*io)->SlabCondnCode();

			int dig3 = condnCode/10%10; // get D of xxDx, third digit of ordered condition code  
			int dig4 = condnCode/1%10;  // get D of xxxD, fourth digit of ordered condition code

			if ( dig3 == 3 && dig4 == 4 ) {
				
				if ( slitCode == ' ' || slitCode == 'D' ) {
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
		## **/


		// audit exposure code
    /** ##
		{
			char exp = (*io)->ExposureCode();
			int condn = (*io)->SlabCondnCode();

			if ( caster == 1 ) { //### caster-specific

				if ( condn > 0 && ( exp == 'E' || exp == 'U' ) ) {
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
			else  {

				if ( condn > 0
					 &&
					 ( exp == ' ' || exp == 'E' || exp == 'U' || exp == '1' || exp == '2' ) ) {
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
		## **/

		// Audit Tek and Kote positional planned orders at beginning of cast violations.  Added 03-06-06 k. hubbard per P. Velasco
    /** ##
		{

			int commodcode = (*io)->CICode();
			bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );
			int heatNumber = (*io)->HeatSeqNum();
			int lotNumber = (*io)->LotNum();

			if ( prevWidth == 0 
				&&
				 heatNumber == 0 )
//				&&
//					lotNumber == 1 )  // Verification  check for beginning of a Cast!!!

				// important to put this initial update before the check above
				//   for CheckStringHeatPosition
				
			if (! isStock) { 
			
				if	(commodcode == 11   // Added commocode (11 [stringers], 45-50) to Tek/Kote material check 3-17-06 k. hubbard
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
					commodcode == 73 )

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
		## **/

    /** ##
		// audit width change.  Radicals are ok here 12-11-03 k. hubbard
		{

			char code = (*io)->SlitTypeCode();

			if ( prevWidth == 0 )
				// this is the first we've seen, use it for initial width
				// important to put this initial update before the check above
				//   for CheckStringHeatPosition

				prevWidth = (*io)->SlabWidth();

			Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);

			if ( widthDiff > TRANS_WIDTH 
				&&
				code == ' ' )  //	Consider slit type i.e. Radicals ok. 11-29-05 k. hubbard
			
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
    **/

////			prevWidth = (*io)->SlabWidth();  //Important !!!! repositioned below width jump audit to work inside this For loop and share between width audits 11-22-05 k. hubbard

		// Maint. Note: K. Hubbard 3-15-06; Add New Fatal here by checking width (*io)->ProvSlabWidthMin & Max

    /** ##
		// Audit Transition 80" Slab Spread Squeeze violations against Provided Cast Widths
		if ( (*io)->Order() != 0 )
		{
			Width width = (*io)->SlabWidth();
//			Width ProvWidthMax = (*io)->ProvSlabWidthMax();

			float ProvWidthMin;   // Added 4 lines here for min check 3-17-06 k. hubbard
			CString strProvMin;
//			strProvMin.Format("%2.1f",(*io)->ProvSlabWidthMin());   Changed line (see next line) to throw reference more reliable Order.h field here for min value 4-5-07 k. hubbard
			strProvMin.Format("%2.1f",(*io)->Order()->ProvSlabWidthMin());
	       	ProvWidthMin = atof(strProvMin);  

			float ProvWidthMax;  
			CString strProvMax;
//			strProvMax.Format("%2.1f",(*io)->ProvSlabWidthMax());   Changed line (see next line) to throw reference more reliable Order.h field here for max value 4-5-07 k. hubbard
			strProvMax.Format("%2.1f",(*io)->Order()->ProvSlabWidthMax()); 
			ProvWidthMax = atof(strProvMax);    
			
			char code   = (*io)->SlitTypeCode();
			bool isTrans = (*io)->IsTransition();
			Width transSpread80 = 1;  //Restored per P. Fronczek maint. 4-18-07 k. hubbard

			//					strProvMax.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
//		m_provCastSlabWidthMax = atof(strProvMax);
//			Width transProvWidthMax = (ProvWidthMax - transSpread80);
			bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );
			
			if  ( prevWidth != 0 
				&&
				 ! isStock 
				&&
				 (*io)->Order() != 0 )  // Added line to make sure order exist for more reliable Order.h field here for min/max values 4-5-07 k. hubbard
			{

//				int transProvWidthMax = (ProvWidthMax - transSpread80);   comm. out 4-5-07 k. hubbard
				Width widthDiff = fabs((*io)->SlabWidth() - prevWidth);
				Width NarrowestWidthPoint = min((*io)->SlabWidth(), prevWidth);

				Width transProvWidthMax = (ProvWidthMax - transSpread80);   //Restored per P. Fronczek maint. 4-18-07 k. hubbard


				if  (widthDiff == 1 
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

		**/
		
		// Audit Side Slits Minimum Allowance change.  Added 5-24-06 k. hubbard per P. Velasco
    /** ##
		{
		  char code = (*io)->SlitTypeCode();
		  bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );

		  //			Width widthDiff = fabs((*io)->SlabWidth() - prevWidth); 
		  //           Modified width difference comparison to check the current lot width to the ordered aim width 02-10-09 k. hubbard per P. Velasco
		  //			Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 
		  
		  //try {		          Tried this for missing orders on CSOrder.h and assoc hotband info (i.e. ? marks instead of number values ) from CSOrder.h 
		  if (! isStock )
		    {				
		      Width widthDiff = fabs((*io)->SlabWidth() - (*io)->Order()->SlabWidth());  // from P-STEEL-MAKE-PLAN file k. hubbard 
		      if (widthDiff > 0)
			{ 
			  if ( widthDiff < 2
			       &&
			       code == 'S' )  //	Check slit type. 
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
    **/
		// End Maint. audit side slit minimum allowance change end 5-24-06 k. hubbard
		
		// Audit Width Jumps change.  Added 11-29-05 k. hubbard per P. Velasco
    /** ##
		{

			int commodcode = (*io)->CICode();
			char code = (*io)->SlitTypeCode();
			Width transSpread80 = 0.3;
			Width transSqueeze80 = 1.6;
			bool isStock = ( (*io)->FpOrderNum().Left(7) == "9999999" );

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
			if (! isStock )
				{ 
				Width hbwidth = (*io)->OrderHotBandWidth();   // Built and used a pointer to COrder.h inside (see field) CSOrder.h which 
			
				if ( widthDiff == 1
					&&
					(commodcode == 65 || commodcode == 66 )
					&&
					( hbwidth < MinTransHBWidth  ||  hbwidth > MaxTransHBWidth )
					&&
					code == ' ' )  //	Consider slit type i.e. Radicals ok. 

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
		**/
		// Audit Stock Reason codes   added audit for stock reason 5-5-09 k. hubbard
    /** ##
		{
			CString stockFaclReason = (*io)->StockReason();
			CString stockCommReason = (*io)->StockCommercialReason();
			// FP Change
			//if ( (*io)->OrderNum() == 9999999 ) {
			if ( (*io)->FpOrderNum().Left(7) == "9999999" )
			{
 
				if ( stockFaclReason == ""
					  ||
  					  stockFaclReason == "0"
					  || 	
					  stockFaclReason == "<blank>"
                      || 
					  stockCommReason == "" 
					  ||
					  stockCommReason == "0" 
					  ||
					  stockCommReason == "<blank>" )
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
    **/
		// Audit Slit Reason Code  added audit for slit reason 2-26-10 k. hubbard
    /**
		{
			char code = (*io)->SlitTypeCode();
			CString slitFaclReason = (*io)->SlitReason();

			if (code == 'E'   
				||
				code == 'S'
				||
				code == 'R')
			{
				
				if ( slitFaclReason == ""
					  ||
  					  slitFaclReason == "0"
					  || 	
					  slitFaclReason == "<blank>" )
                {
					ostr << "slit facility reason '" << setw(20) << LPCTSTR(slitFaclReason)
						 << " ' must not be blank " 
						 << ends;
					ADD_ERR(CCastStringHeatValidnError::FATAL);  // temp switched from fatal to warning 
					isOk = false;
				}
			}
		}
    **/		
		// Audit 2BOF inward width change designs.  1-28-10 k. hubbard
    /**
    {
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
    **/
    
//     **** !!!!!! Caution the variable below is shared between 2 width audits above and must get populated once after both or else one audit above will fail!!!! 11-29-05 k. hubbard
    prevWidth = (*io)->SlabWidth();  // Important CAUTION!!!! repositioned here to work inside this For loop and share between width change and width jump audits 11-29-05 k. hubbard
    // audit change indicator
    {
      if ( (*io)->ChangeMarked() ) {
	ostr << "Change indicator has not been removed." 
	     << ends;
	ADD_ERR(CCastStringHeatValidnError::WARNING);
	isOk = false;
      }
    }
  }
  return isOk;
}
