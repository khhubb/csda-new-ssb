// CastStringHeatValidnError.cpp: implementation of the CCastStringHeatValidnError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "CastStringHeatValidnError.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CastStringHeat.h"
#include "CSOrder.h"


const CString& CCastStringHeatValidnError::SeverityAsString() const
{
	static CString f("Fatal");
	static CString w("Warng");

	switch ( m_severity ) {
	case FATAL:
		return f;
		
	case WARNING:
		return w;

	default:
		assert(0);
	}

	return f;
}



// We need to display a list of errors in a CListCtrl
// This happens in several places, so we decided to 
// do it once and put the code here.


// static
void CCastStringHeatValidnError::SetHeader(CListCtrl& clist)
{
	static vector<CString> validStrings;
	static vector<CString> validFormats;
	static vector<int>	   validJustifs;

	if ( validStrings.empty() ) {

		validStrings.push_back("Severity");
		validFormats.push_back("Severity");
		validJustifs.push_back(LVCFMT_LEFT);

		validStrings.push_back("Heat#");
		validFormats.push_back("99");
		validJustifs.push_back(LVCFMT_RIGHT);

		validStrings.push_back("Tab#");
		validFormats.push_back("99999");
		validJustifs.push_back(LVCFMT_RIGHT);

		validStrings.push_back("Strand#");
		validFormats.push_back("9");
		validJustifs.push_back(LVCFMT_RIGHT);

		validStrings.push_back("Lot#");
		validFormats.push_back("99");
		validJustifs.push_back(LVCFMT_RIGHT);

		validStrings.push_back("Order#");
		validFormats.push_back("9999999-MMMMM");
		validJustifs.push_back(LVCFMT_RIGHT);

		validStrings.push_back("Comment");
		validFormats.push_back("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
		validJustifs.push_back(LVCFMT_LEFT);
	}

	vector<int>     validWidths;
	
	{
		int num=0;
		for ( vector<CString>::iterator is = validStrings.begin();
		is != validStrings.end();
		++is, ++num ) {
			
			int width = 15+max(clist.GetStringWidth((*is)),
				clist.GetStringWidth( validFormats[num] ));
			
			validWidths.push_back( width );
		}
	}


	int num = 0;
	for ( vector<CString>::iterator is = validStrings.begin();
		  is != validStrings.end();
		  ++is, ++num )

		clist.InsertColumn(num,(*is),validJustifs[num],validWidths[num],num);

}


// static
void CCastStringHeatValidnError::UpdateList(CListCtrl& clist, const vector<CCastStringHeatValidnError>& errors)
{
	clist.DeleteAllItems();


	if ( errors.size() == 0 )
		clist.InsertItem(0,"No errors.");
	else {
		char buf[100];
		int itemNum = 0;
		for ( vector<CCastStringHeatValidnError>::const_iterator ie = errors.begin();
			  ie != errors.end();
			  ++ie, ++itemNum ) {

			clist.InsertItem( itemNum, LPCTSTR((*ie).SeverityAsString()) );
			
			
			const CCastStringHeat* pHeat = (*ie).Heat();
			if ( pHeat == 0 ) {
				clist.SetItemText(itemNum, 1,  "--" );
				clist.SetItemText(itemNum, 2, "--" );
				clist.SetItemText(itemNum, 3, "-"  );
			}
			else {
				clist.SetItemText(itemNum, 1,  _itoa( (*ie).HeatIndex()+1, buf, 10) );
				clist.SetItemText(itemNum, 2, _itoa( pHeat->TabNum(),        buf, 10) );
				clist.SetItemText(itemNum, 3, ((*ie).StrandNum() == 0  
														  ? "-"
														  : _itoa( (*ie).StrandNum(), buf, 10)) );
			}

			const CCSOrder* pCSOrder = (*ie).CSOrder();

			if ( pCSOrder == 0 ) {
				clist.SetItemText(itemNum, 4, "-");
				clist.SetItemText(itemNum, 5, "-");
			}
			else {
				clist.SetItemText(itemNum, 4, _itoa( (*ie).LotIndex()+1, buf, 10) );
				// FP change
				//clist.SetItemText(itemNum, 5, _ltoa( pCSOrder->OrderNum(),  buf, 10) );
				clist.SetItemText(itemNum, 5, LPCTSTR(pCSOrder->FpOrderNum()) );
			}

			clist.SetItemText(itemNum, 6, LPCTSTR((*ie).Msg()) );
		}
	}
}
