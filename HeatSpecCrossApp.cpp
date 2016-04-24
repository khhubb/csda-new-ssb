// HeatSpecCrossApp.cpp: implementation of the CHeatSpecCrossApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"



#include "csda.h"
#include "HeatSpecCrossApp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "HeatSpecCrossAppSet.h"
#include "NOutputWnd.h"


////////////////////////////////////////////////////////////////
//	
//	CHeatSpecCrossApp
//
//	Holds a multimap from specs to cross-applicable specs
//	
////////////////////////////////////////////////////////////////


void CHeatSpecCrossApp::Clear()
{
	m_xappMap.clear();
}



bool CHeatSpecCrossApp::Init(CDatabase* pDB)
{
	bool retval = true;

	NOutputWnd::PostLoadMsg("Loading cross apps: ");

	Clear();
	
	CHeatSpecCrossAppSet xset(pDB);

	long count = 0;

	try 
	{
		xset.Open(CRecordset::forwardOnly,
				  0,
				  CRecordset::readOnly);

		while ( ! xset.IsEOF() ) {

			m_xappMap.insert( THeatSpecCrossAppMap::value_type(xset.m_SPECARD_SPEC,
															 xset.m_SPECARD_CROSS_APPL_SPEC_CD ) );
			xset.MoveNext();

			++count;
			NOutputWnd::WriteProgress(count);
		}
   }
   catch ( CDBException* e )
   {
	   AfxMessageBox( e->m_strError,   
                      MB_ICONEXCLAMATION );

	   e->Delete();

   	   retval = false;
   };

   {
	   ostrstream ostr;
	   ostr << "Read " << count << "records.\n" << ends;
	   NOutputWnd::PostLoadMsg(ostr);
   }


#if 1
   PrintMap();
#endif

   if ( xset.IsOpen() )
	   xset.Close();

   return retval;
}



void CHeatSpecCrossApp::PrintMap()
{
#ifdef _DEBUG
	afxDump << "\n\nHeatSpecCrossAppMap\n"
			<< "=====================================\n";

	CString lastSpec = "";


	for ( THeatSpecCrossAppMap::const_iterator im = m_xappMap.begin();
		  im != m_xappMap.end();
		  ++im ) {

		CString spec = (*im).first;
		CString xapp = (*im).second;

		if ( lastSpec != spec )
			afxDump << "\n" << spec << " : ";

		afxDump << xapp << " ";

		lastSpec = spec;
	}

	afxDump << "\n=====================================\n\n";
#endif
}


//
//  For a given spec and caster, 
//		find the set of specs cross-applicable with the spec.
//  We play some games with the last two digits (zeroing them out)
//		to try to get a match.
//

THeatSpecCrossAppSet CHeatSpecCrossApp::GetCrossAppSpecs(const CString& spec, int caster) const
{

	pair<THeatSpecCrossAppMap::const_iterator,
	 	 THeatSpecCrossAppMap::const_iterator> range 
		
		 = m_xappMap.equal_range(spec);

	if ( range.first == range.second ) {
		// failed to find an entry, try zeroing the 90/40 part.
		CString spec7 = spec;
		spec7.SetAt(6,'0');

#if _DEBUG
		afxDump << "Failed cross app " << spec << ", trying " << spec7 << "\n";
#endif
		range = m_xappMap.equal_range(spec7);

		if ( range.first == range.second ) {

			spec7.SetAt(5,'0');
#if _DEBUG
		afxDump << "Failed cross app again, trying " << spec7 << "\n";
#endif

			range = m_xappMap.equal_range(spec7);
		}
	}


	THeatSpecCrossAppSet x;

	// the following should work, but VC++ 5.0 does not support it yet
	// x.insert(range.first,range.second);

	for ( THeatSpecCrossAppMap::const_iterator im = range.first;
		  im != range.second;
		  ++ im ) {

		char dig4 = (*im).second[3];

		// filter for specs suitable for this caster
			  
		if ( dig4 == '0'
			 ||
			 ( caster == 1 && ( dig4 == '4' || dig4 == '7' ) )
			 ||
			 ( caster != 1 && ( dig4 == '2' ) ) )			
			
			x.insert( (*im).second );
	}
		  

	return x;
}
	

void my_set_intersection(THeatSpecCrossAppSet& x,
						 THeatSpecCrossAppSet& y,
						 vector<CString>& v)
{
	for ( THeatSpecCrossAppSet::iterator ix = x.begin();
		  ix != x.end();
		  ++ix ) {

		if ( y.find((*ix)) != y.end() )
			v.push_back( (*ix) );
	}
}

	

//
//  For a set of specs, find the intersection of the their sets of cross-applicable specs
//

THeatSpecCrossAppSet CHeatSpecCrossApp::GetCrossAppSpecs(THeatSpecCrossAppSet& specs, int caster) const
{
	if ( specs.size() == 0 )
		return THeatSpecCrossAppSet();

	if ( specs.size() == 1 )
		return GetCrossAppSpecs( *(specs.begin()), caster );

	THeatSpecCrossAppSet x = GetCrossAppSpecs( *(specs.begin()), caster );
	vector<CString> v;

	for ( THeatSpecCrossAppSet::iterator is = specs.begin();
		  is != specs.end();
		  ++is ) {

		THeatSpecCrossAppSet z = GetCrossAppSpecs( (*is), caster );

		//set_intersection(x.begin(),x.end(),
		//				    z.begin(),z.end(),
		//				    v);

		my_set_intersection(x,z,v);

#if _DEBUG
		{
			afxDump << "GetCrossAppSpecs: intersecting from " << (*is) << ", { ";
			for ( THeatSpecCrossAppSet::iterator ix = x.begin();
				  ix != x.end();
				  ++ix ) 
				afxDump << (*ix) << " ";
			afxDump << "} ^ { ";
			for ( THeatSpecCrossAppSet::iterator iz = z.begin();
				  iz != z.end();
				  ++iz ) 
				afxDump << (*iz) << " ";
			afxDump << "} = { ";
			for ( vector<CString>::iterator iv = v.begin();
				  iv != v.end();
				  ++iv ) 
				afxDump << (*iv) << " ";
			afxDump << "}\n";
		}
#endif



		x.clear();
		x.insert(v.begin(),v.end()); // copy
		v.clear();

		if ( x.size() == 0 )
			break;
	}

	return x;
}

	

//  
//  IsCompatible(spec1,spec2) if spec2 appears in spec1's list of cross-app specs.
//

bool CHeatSpecCrossApp::IsCompatible(const CString& spec1, const CString& spec2, int caster) const
{
	if ( spec1 == spec2 )
		return true;

	THeatSpecCrossAppSet xset = GetCrossAppSpecs(spec1,caster);

	return xset.find( spec2 ) != xset.end();
}
