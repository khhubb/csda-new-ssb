
#include "StdAfx.h"


#include "SpecMgr.h"
#include "Spec.h"
#include "SpecDPACHemsSet.h"
#include "SpecDPASet.h"
#include "NOutputWnd.h"


////////////////////////////////////////////////////////////////
//
//  CSpecMgr
//
//
//	We have a set of specs.
//  Calculations of cast-times, etc. are actually handled
//    by a separate COM object with interface IBOFSpeedCalc.
//  We hold a pointer to one of these.
//	We also keep a map from spec name to CSpec instances.
//
//
////////////////////////////////////////////////////////////////


//
//  Bookkeeping.
//  When we create a spec, record it under its name.

void CSpecMgr::RecordSpec( CSpec* pSpec )
{
	RecordSpec(pSpec->Name(),pSpec);
}



void CSpecMgr::RecordSpec( const CString& keyName, CSpec* pSpec )
{
	// Make a copy of any spec being registered under a different name
	// This takes up more space, but makes deletion easier.
	// (Not that we do any deletions in this system.
	//  this came from the 2BOF code.)
	
	if ( keyName != pSpec->Name() ) {
		pSpec = new CSpec(*pSpec);
		pSpec->m_name = keyName;
	}

	T_SpecMap::iterator is = specMap.find(keyName);

	if ( is == specMap.end() )
		specMap.insert(T_SpecMap::value_type(keyName,pSpec));
}



//
// This is direct name lookup in the map.
//

CSpec* CSpecMgr::FindSpecAux(CString& specName)
{
	T_SpecMap::iterator is = specMap.find(specName);

	if ( is == specMap.end() )
		// not found
		return 0;
	else
		// found
		return (*is).second;
}



//
//  This is somewhat fancier lookup.
//  The original tables don't contain all specs.
//  For example 05 specs and 40 specs don't have entries,
//    but rely on the 00 spec entry.
//  When we look up one of these specs, we create its own entry
//    for future lookups.
//

CSpec* CSpecMgr::FindSpec(CString& specName)
{
	CSpec* pSpec = FindSpecAux(specName);

	// Found by direct lookup
	if ( pSpec != 0 )
		return pSpec;

	// try alternative names

	// Make sure we weren't passed a bogus string
	if ( specName.GetLength() != 7 )
		return 0;

	CString copy(specName);


	// XXX-XX-X5 ==> XXX-XX-X0

	if ( copy[6] == '5' ) {

		copy.SetAt(6,'0');
		pSpec = FindSpecAux(copy);

		if ( pSpec != 0 ) {

			RecordSpec(specName,pSpec);
			return pSpec;
		}
	}


	copy = specName;

	// XX-XX-0X  ==> XXX-XX-4X

	if ( copy[5] == '0' ) {

		copy.SetAt(5,'4');
		pSpec = FindSpecAux(copy);

		if ( pSpec != 0 ) { 

			RecordSpec(specName,pSpec);
			return pSpec;
		}
	}

	return 0;

}



CSpec* CSpecMgr::FindSpecMaybe(CString& specName, int caster)
{
	CSpec* pSpec = FindSpec(specName);

	if ( pSpec != 0 )
		return pSpec;


	CString copy(specName);

	//  XXX-0X-XX ==>  XXX-*X-XX  where X = 2,4,7

	if ( copy[3] == '0' ) {

		if ( caster == 2 || caster == 3 ) {

			copy.SetAt(3,'2');
			pSpec = FindSpec(copy);
			if ( pSpec != 0 ) 
				return pSpec;

		}

		// CASTER TODO: is this for just caster 1 or also 4,5?
		else { // caster == 1
			
			// TODO -- when we have the list of RHOB specs, we can choose 4 vs 7.

			copy.SetAt(3,'4');
			pSpec = FindSpec(copy);
			if ( pSpec != 0 )
				return pSpec;

			copy.SetAt(3,'7');
			pSpec = FindSpec(copy);
			if ( pSpec != 0 )
				return pSpec;
		}
	}

	return 0;
}



void CSpecMgr::Cleanup()
{
	for ( T_SpecMap::iterator is = specMap.begin();
		  is != specMap.end();
		  ++is )

		delete (*is).second;

	specMap.erase(specMap.begin(),specMap.end());
}


void CSpecMgr::CleanupDispatch()
{
	m_pBOFSpecMgr->ReleaseDispatch();
	delete m_pBOFSpecMgr;
	m_pBOFSpecMgr = 0;
}



//
//  Read in the specs from the database tables
//

bool CSpecMgr::Initialize( CDatabase* pDBDPA  )
{
	bool retval;

	CSpecDPASet specSet(pDBDPA);
	CSpecDPAChemsSet chemsSet(pDBDPA);

	try 
	{
		specSet.Open(CRecordset::forwardOnly,
					 0,
					 CRecordset::readOnly);
		InitializeSpecs(specSet);
		
		chemsSet.Open(CRecordset::forwardOnly,
					  0,
					  CRecordset::readOnly);
		InitializeDPAChems(chemsSet);

		retval = true;
	}
   catch ( CDBException* e )
   {
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );

	   e->Delete();

   	   retval = false;
   }

   if ( specSet.IsOpen() )
	   specSet.Close();

   if ( chemsSet.IsOpen() )
	   chemsSet.Close();

   if ( ! InitializeBOFSpecs() )
	   retval = false;

   return retval;
}


//
//  Initialize from the set of specs from the DPA system
//



void CSpecMgr::InitializeSpecs(CSpecDPASet& specSet)
{
	NOutputWnd::PostLoadMsg("Loading specs: ");

	Cleanup();

	long count = 0;
	while ( ! specSet.IsEOF() ) {

		CSpec* pSpec = new CSpec(specSet);

		RecordSpec(pSpec);

		specSet.MoveNext();

		++count;
		NOutputWnd::WriteProgress(count);
	}

	{
		ostrstream ostr;
		ostr << "Loaded " << count << " specs\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}

}




typedef	map<long,vector<CChemRange> > TChemMap;


void CSpecMgr::InitializeDPAChems(CSpecDPAChemsSet& chemSet)
{
	NOutputWnd::PostLoadMsg("Loading spec chems: ");

	long count = 0;
	while ( ! chemSet.IsEOF() ) {
	
		// construct the actual spec to use
		CString spec;

		spec.Format("%05ld%s%1ld",
					chemSet.m_STEEL_SPECIFICATION,
					LPCTSTR(chemSet.m_SIXTH_DIGIT_OF_SPEC),
					chemSet.m_SEVENTH_DIGIT_OF_SPEC);

		// CASTER TODO: What is correct here with the new casters?
		//at the moment, having only caster == 2 or not has the desired effect.
		int caster;
		if ( chemSet.m_CASTER_INDICATOR.Find('1') != -1 )
			caster = 1;
		else 
			caster = 2;

		CSpec* pSpec = FindSpecMaybe(spec,caster);

		if ( pSpec == 0 ) {
#if _DEBUG
			afxDump << "DPACHEMS: Could not find spec " << spec << "\n";
#endif
		}
		else {
		
			CChem::Element elt;
		
			if ( CChem::ToElement(chemSet.m_ELEMENT_SYMBOL,elt) ) 
				pSpec->m_dpaChems.push_back(CChemRange(elt,
													 chemSet.m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT,
													 chemSet.m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT,
													 chemSet.m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT,
													 (chemSet.m_SM_SPEC_OPEN_RANGE_IND == "*")));
		}
		chemSet.MoveNext();

		++count;
		NOutputWnd::WriteProgress(count);

	}

	{
		ostrstream ostr;
		ostr << "Loaded " << count << " specs\n" << ends;
		NOutputWnd::PostLoadMsg(ostr);
	}
}



bool CSpecMgr::InitializeBOFSpecs()
{

	static bool isInitialized = false;

	if ( isInitialized ) 
		return true;

	isInitialized = true;

	m_pBOFSpecMgr = new IBOFSpeedCalc;

	if (  ! m_pBOFSpecMgr->CreateDispatch("BOFSpecMgr.BOFSpeedCalc.1") ) {

		AfxMessageBox("Error creating dispatch: You will not be able to compute\n   cast times properly.");
		return false;
	}

	try
	{
		NOutputWnd::PostLoadMsg("Loading spec chems: ");
		m_pBOFSpecMgr->Initialize("DSN=CSDATest");
		NOutputWnd::PostLoadMsg(" done.\n");

		return true;
	}
	catch ( COleDispatchException* e )
	{
		AfxMessageBox( e->m_strDescription 
						+ "\nError initializing BOFSpecMgr"
						  "\nYou will not be able to compute cast times properly.",   
					MB_ICONEXCLAMATION );
		e->Delete();
		return false;
	}

	return true;
}




//
//  Compute a cast time based on slab info
//

double CSpecMgr::SlabCastTime(const CString& specName,
							  int caster,
							  int strand,
							  int position,
							  Weight weight,
							  Width width,
							  Thick thickness,
							  CChem::Bound carbon,
							  CChem::Bound manganese,
							  int heatSize,
							  bool* isValid)
{
	if ( m_pBOFSpecMgr == 0 )
		// compute a bogus duration, as a fraction of the heat times a 45-minute heat time.
		return weight/(heatSize*2000)*45*60;
	
	double time;

	try 
	{
		long found;


		time = m_pBOFSpecMgr->SlabCastTime(specName,
										 caster,
										 strand,
										 position,
										 weight,
										 width,
										 thickness,
										 carbon,
										 manganese,
										 heatSize,
										 &found);
		*isValid = (found != 0);

	}
	catch ( COleDispatchException* e )
	{
		AfxMessageBox( e->m_strDescription, 
					
					MB_ICONEXCLAMATION );
		e->Delete();
		*isValid = false;
	}	

	if ( ! *isValid )
		return 45*60;
	else
		return time;
}


