// A variety of useful things for window manipulation

#include "stdafx.h"
#include "WinUtils.h"

  

//  The name is pretty descriptive.
//
//  initialize a CListBox from a vector of strings.
//  select the ones listed in the set selStrings.

void WinUtils::InitializeListBoxFromStrings(CListBox& lbox,
											vector<CString>& strings,
											set<CString>& selStrings)
{
	lbox.ResetContent();

	{
		for ( vector<CString>::iterator is = strings.begin();
			  is != strings.end();
			  ++is ) 
			lbox.AddString( LPCTSTR( (*is) ) );
	}


	{ 
		for ( set<CString>::iterator ig = selStrings.begin();
			  ig != selStrings.end();
			  ++ig ) {

			vector<CString>::iterator is = find(strings.begin(),
												strings.end(),
												(*ig));

			if ( is != strings.end() ) 
				lbox.SetSel( is - strings.begin(), TRUE );
		}
	}
}




//  Given a list box, record the selected strings in selStrings.
//  It is assumed that the vector strings contains the same
//  strings as is displayed in the list box, in the same order.
//  It is these strings that are placed in selStrings.

void WinUtils::SetSelStringsFromListBox(CListBox& lbox,
										vector<CString>& strings,
										set<CString>& selStrings)
{
	int count = lbox.GetSelCount();
	assert( count >= 0 );
	int* indexes = new int[count];
	lbox.GetSelItems(count,indexes);

	selStrings.clear();

	for ( int i = 0; i < count; ++i ) 
		selStrings.insert( strings[ indexes[i] ] );

	delete[] indexes;
}




//  I can't believe they didn't provide this.
//  Deletes all header items from a CListCtrl.

void WinUtils::DeleteHeaderItems(CListCtrl* pCtrl)
{
	int count = pCtrl->GetHeaderCtrl()->GetItemCount();

	for ( int i=count-1; i>=0; --i ) 
		pCtrl->DeleteColumn(i);
}




//  Compute the indexes of all selected items in a CListCtrl
//  Put in vector posns.
//  Versions for CListCtrl* and CListCtrl&

void WinUtils::GetSelectedPosns(CListCtrl* pCtrl,vector<int>& posns)
{
	GetSelectedPosns(*pCtrl,posns);
}


void WinUtils::GetSelectedPosns(CListCtrl& rCtrl,vector<int>& posns)
{
	posns.clear();

	for ( int i = rCtrl.GetNextItem(-1,LVNI_SELECTED);
		  i != -1;
	      i = rCtrl.GetNextItem(i,LVNI_SELECTED) )

		posns.push_back( i );

}



//  Get a count of the number of selected items in a CListCtrl.
//  Versions for CListCtrl* and CListCtrl&.

int WinUtils::GetSelectedCount(CListCtrl* pCtrl)
{
	return GetSelectedCount(*pCtrl);
}

int WinUtils::GetSelectedCount(CListCtrl& rCtrl)
{
	int count = 0;

	for ( int i = rCtrl.GetNextItem(-1,LVNI_SELECTED);
		  i != -1;
	      i = rCtrl.GetNextItem(i,LVNI_SELECTED) )

		++count;

	return count;
}



//  The following functions are useful for generating HTML reports
//  based on the contents of a CListCtrl or SECTreeCtrl.
//  Because these are so similar, we can templatize.
//
//  the entry points are:
//
// ExtractContentsForReport(ostream& ostr, CListCtrl& rCtrl,bool selectedOnly)
// ExtractContentsForReport(ostream& ostr, SECTreeCtrl& rCtrl,bool selectedOnly)
// ExtractContentsForReport(ostream& ostr, CListCtrl* pCtrl,bool selectedOnly)
// ExtractContentsForReport(ostream& ostr, SECTreeCtrl* pCtrl,bool selectedOnly)
//
// Each writes an HTML table to the output stream ostr.
// A header row comes from the headers on the CListCtrl or CTreeCtrl.


//  internal function
//  given a vector of strings representing the column headers,
//  output a row of an HTML table containing those headers wrapped
//  in <th>...</th>.

static void WriteHeaderForReport(ostream& ostr, vector<CString>& headers)
{
	ostr << "<tr>\n";
	
	for ( vector<CString>::iterator is = headers.begin();
		  is != headers.end();
		  ++is ) {

		ostr << "  <th>" << LPCTSTR((*is)) << "</th>\n";

	}

	ostr << "</tr>\n";
}


// internal function
// works for both SECTreeCtrl and CListCtrl
// vector subItems has the column indexes to iterate through.
// iterate through the indicated columns in the given row and
//  print one table cell per.

template<class TCtrl, class TIndex>
static void WriteRowForReport(ostream& ostr, TCtrl& rCtrl,TIndex row,vector<int>& subItems)
{
	ostr << "<tr>\n";
	
	for ( vector<int>::iterator ii = subItems.begin();
		  ii != subItems.end();
		  ++ii ) {

		ostr << "  <td>" << LPCTSTR(rCtrl.GetItemText(row,(*ii))) << "</td>\n";
	}

	ostr << "</tr>\n";
}



// internal function
// retrieve the column header strings and indexes of the columns in a CListCtrl or SECTreeCtrl

template<class TCtrl>
static void GetHeaderDetails(TCtrl& rCtrl, int colCount, vector<CString>& headers, vector<int>& subItems)
{
	{
		char buffer[200];
		LVCOLUMN col;
		col.mask = LVCF_SUBITEM | LVCF_TEXT;
		col.pszText = buffer;
		col.cchTextMax = 200;

		for (int i=0; i<colCount; ++i ) {
			rCtrl.GetColumn(i,&col);
			if ( (col.mask & LVCF_SUBITEM )
				 &&
				 (col.iSubItem >= 0 ) )
				 subItems.push_back(col.iSubItem);
				 headers.push_back( CString(col.pszText) );
		}
	}
}


// internal function
// interfaces to templatized GetHeaderDetails

static void GetHeaderDetails(SECTreeCtrl& rCtrl, vector<CString>& headers, vector<int>& subItems)
{
	GetHeaderDetails(rCtrl,rCtrl.GetColumnCount(),headers,subItems);
}



// internal function
// interfaces to templatized GetHeaderDetails

static void GetHeaderDetails(CListCtrl& rCtrl, vector<CString>& headers, vector<int>& subItems)
{
	int colCount = rCtrl.GetHeaderCtrl()->GetItemCount();
	GetHeaderDetails(rCtrl,colCount,headers,subItems);
}


// internal function
// iterate through selected rows only and write them to the report.

static void WriteSelectedRows(ostream& ostr, CListCtrl& rCtrl, vector<int>& subItems)
{
	for ( int item = rCtrl.GetNextItem(-1,LVNI_SELECTED);
		  item != -1;
		  item = rCtrl.GetNextItem(item,LVNI_SELECTED) ) 
		WriteRowForReport(ostr,rCtrl,item,subItems);
}



// internal function
// iterate through selected rows only and write them to the report.

static void WriteSelectedRows(ostream& ostr, SECTreeCtrl& rCtrl, vector<int>& subItems)
{

	for ( HTREEITEM hItem = rCtrl.GetFirstSelectedItem();
		  hItem != 0;
		  hItem = rCtrl.GetNextSelectedItem(hItem) ) 
		WriteRowForReport(ostr,rCtrl,hItem,subItems);
}



// internal function
// iterate through all rows and write them to the report.

static void WriteAllRows(ostream& ostr, CListCtrl& rCtrl, vector<int>& subItems)
{
	int rowCount = rCtrl.GetItemCount();
	for ( int item = 0; item<rowCount; ++item )
		WriteRowForReport(ostr,rCtrl,item,subItems);
}



// internal function
// iterate through all rows and write them to the report.

static void WriteAllRows(ostream& ostr, SECTreeCtrl& rCtrl, vector<int>& subItems)
{
	for ( HTREEITEM hItem = rCtrl.GetRootItem();
		  hItem != 0;
		  hItem = rCtrl.GetNextVisibleItem(hItem) )

		WriteRowForReport(ostr,rCtrl,hItem,subItems);
}




// internal function
// Get the header details and write the header row to the report,
// then write either all or only selected rows to the report.

template<class TCtrl>
static void ExtractContentsForReportAux(ostream& ostr, TCtrl& rCtrl,bool selectedOnly)
{
	vector<int> subItems;
	vector<CString> headers;

	GetHeaderDetails(rCtrl,headers,subItems);

	WriteHeaderForReport(ostr,headers);

	if ( selectedOnly ) 
		WriteSelectedRows(ostr,rCtrl,subItems);
	else
		WriteAllRows(ostr,rCtrl,subItems);


}



// The public interface functions.
// 'Twould be nice to have only one, templated, but the compiler choked on that.

void WinUtils::ExtractContentsForReport(ostream& ostr, CListCtrl& rCtrl,bool selectedOnly)
{
	ExtractContentsForReportAux(ostr,rCtrl,selectedOnly);
}

void WinUtils::ExtractContentsForReport(ostream& ostr, SECTreeCtrl& rCtrl,bool selectedOnly)
{
	ExtractContentsForReportAux(ostr,rCtrl,selectedOnly);
}

void WinUtils::ExtractContentsForReport(ostream& ostr, CListCtrl* pCtrl,bool selectedOnly)
{
	ExtractContentsForReport(ostr,*pCtrl,selectedOnly);
}


void WinUtils::ExtractContentsForReport(ostream& ostr, SECTreeCtrl* pCtrl,bool selectedOnly)
{
	ExtractContentsForReport(ostr,*pCtrl,selectedOnly);
}