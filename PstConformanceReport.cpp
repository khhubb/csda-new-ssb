// PstConformanceReport.cpp: implementation of the CPstConformanceReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "PstConformanceReport.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "CasterScen.h"
#include "CastString.h"
#include "CSOrder.h"
#include "NReport.h"
#include "ReportDlg.h"
#include "MultiSelectDlg.h"
#include "NTime.h"


//
//  The CPstConformanceReport is given a CSuperScen.
//  It walks through the three casters,
//     for each caster through the strings
//         for each string through the orders
//  computing summary information on tonnages
//     by ciCode, prio vs non-prio, and PST week (relative to today)
//
//  CPstConformanceReport creates one instance of CScenHolder for each scenario.
//  CScenHolder creates one instance of CStringHolder for each of its strings.
//  CStringHolder creates one instance of CItem for each ciCode among its orders.
//  The d-tors of these classes must do the cleanup (all are dynamically allocated).


CPstConformanceReport::~CPstConformanceReport()
{
	for ( vector<CScenHolder*>::iterator ih = m_scenHolders.begin();
		  ih != m_scenHolders.end();
		  ++ih ) 
		delete (*ih);
	m_scenHolders.clear();
}


CPstConformanceReport::CScenHolder::~CScenHolder()
{
	release(m_stringHolders.begin(),m_stringHolders.end());
	m_stringHolders.clear();
}

CPstConformanceReport::CStringHolder::~CStringHolder()
{
	for ( T_MapIntItem::iterator ii = m_items.begin();
		  ii != m_items.end();
		  ++ii ) 
		delete (*ii).second;

	m_items.clear();
}


//
//
//  Usage:
//
//		CSuperScen* pSS = ... ;
//	
//		CPstConformanceReport report(pSS);
//		report.Create();
//
//


//
//  The entry point for creating the report.
//

void CPstConformanceReport::Create()
{
	assert( m_pSS != 0 );

	// Determine which strings to report on
	CMultiSelectDlg dlg;
	vector<CCastString*> castStrings;

	{ 
		ostrstream ostr;
		for ( int caster=1; caster <= 3; ++ caster ) {
			CCasterScen* pScen = m_pSS->CasterScen(caster);
			if ( pScen != 0 ) {
				for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
					  is != pScen->CastStrings().end();
					  ++is ) {
					ostr << "Caster " << caster
						 << ", "
						 << (*is)->Id().Year()
						 << "-"
						 << (*is)->Id().Week()
						 << "-"
						 << (*is)->Id().StringId()
						 << ends;
					dlg.m_strings.push_back(CString(ostr.str()));
					ostr.freeze(false);
					ostr.seekp(0);
					castStrings.push_back((*is));
				}
			}
		}
	}

	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.m_posns.size() == 0 )
		return;

	// dlg.m_posns contains the indexes of the selected strings.
	// First, select out those strings, per caster
	vector<CCastString*> selCastStrings[4];
	{
		for ( vector<int>::iterator ii = dlg.m_posns.begin();
			  ii != dlg.m_posns.end();
			  ++ii ) {
			CCastString* pString = castStrings[(*ii)];
			selCastStrings[pString->CasterScen()->Caster()].push_back(pString);
		}
	}

	
	// iterate through the casters, creating a CScenHolder for each
	//   and having each compute its summary information.
	// Add the summary information for each caster to the total information.

	for ( int caster=1; caster <= 3; ++caster ) {
		CCasterScen* pScen = m_pSS->CasterScen(caster);
		if ( pScen != 0 && selCastStrings[caster].size() != 0 ) {
			CScenHolder* pHolder = new CScenHolder(pScen);
			m_scenHolders.push_back(pHolder);
			pHolder->Compute(selCastStrings[caster]);
			m_totItem.AddItem(pHolder->TotItem());
		}
	}

	//  Write the summary information to a file and make available for printing.
	CreateReport();
}


//
//  Write the summary information to a file and make available for printing.
//

void CPstConformanceReport::CreateReport()
{
	CString filename;;

	WriteReport(filename);
	DisplayReport(filename);
	DeleteFile(filename);
}



void CPstConformanceReport::WriteReport(CString& filename)
{
	// open a temporary file

	ofstream ofs;
	
	if ( ! NReport::OpenStdHtmlFile(ofs,
									"PSTConformanceReport",
									"PST Conformance Report",
									true, // need landscape
									filename) )
		return;



	ofs.setf(ios_base:: fixed, ios_base:: floatfield);

	// Write the report for each scenario.

	for ( vector<CScenHolder*>::iterator ih = m_scenHolders.begin();
		  ih != m_scenHolders.end();
		  ++ih ) {
		(*ih)->WriteReport(ofs);

		// write table end and a form feed except for last one
		ofs << "</table>\n";
		NReport::WriteHtmlNewPage(ofs);
	}


	//  Write a total report for all the casters.
	CItem::WriteHeader(ofs);
	m_totItem.WriteTotReport(ofs,
							 "Total tons for all 3 casters",
							 "Combined");

	// close table on this last page
	ofs << "</table>\n" ;

	// done
	NReport::CloseStdHtmlFile(ofs);
}


// 
//  Make the report available for printing.
//

void CPstConformanceReport::DisplayReport(CString& filename)
{

	CReportDlg dlg;
	dlg.m_filename = filename;
	dlg.DoModal();



	// originally, we had a straight text file and used notepad 
	// for viewing and printing
//	ostrstream ostr;
//	ostr << "notepad " << filename << ends;
//	system(ostr.str());
//	ostr.freeze(false);
}


// 
//  get rid of temporary file
//

void CPstConformanceReport::DeleteFile(CString& filename)
{
	unlink(LPCTSTR(filename));
}



/////////////////////////////////////////////////////////////////////
//
//  class CPstConformanceReport::CScenHolder
//
/////////////////////////////////////////////////////////////////////


//
//  Compute summary statistics for each string and for the scenario as a whole
//

void CPstConformanceReport::CScenHolder::Compute()
{
	Compute(m_pScen->CastStrings());
}

void CPstConformanceReport::CScenHolder::Compute(vector<CCastString*>& castStrings)
{
	for ( vector<CCastString*>::iterator is = castStrings.begin();
		  is != castStrings.end();
		  ++is ) 
		AddString((*is));

	ComputeTotals();
}



//
//  Compute summary statistics for one string
//

void CPstConformanceReport::CScenHolder::AddString(CCastString* pString)
{
	CStringHolder* pHolder = new CStringHolder(pString);
	m_stringHolders.push_back(pHolder);
	pHolder->Compute();
}


//
//  Compute summary statistics for the scenario
//

void CPstConformanceReport::CScenHolder::ComputeTotals()
{
	for ( vector<CStringHolder*>::iterator ih = m_stringHolders.begin();
		  ih != m_stringHolders.end();
		  ++ih ) 
		m_totItem.AddItem((*ih)->TotItem());
}


//
//  Write summary information for each string and for the scenario
//

void CPstConformanceReport::CScenHolder::WriteReport(ostream& ostr)
{

	for ( vector<CStringHolder*>::iterator ih = m_stringHolders.begin();
		  ih != m_stringHolders.end();
		  ++ih ) {
		(*ih)->WriteReport(ostr);
		// write table end and form feed except for last one
		if ( (ih+1) != m_stringHolders.end() ) {
			ostr << "</table>\n";
			NReport::WriteHtmlNewPage(ostr);
		}
	}

	// We have to make sure there is a table open in the html
	//  in order for the code that follows to operate properly.
	// We will not have a table open if there are no strings on this caster.
	if ( m_stringHolders.size()  == 0 )
		CItem::WriteHeader(ostr);


	ostrstream strstr;
	strstr << "Total tons for #" << m_pScen->Caster() << "caster" << ends;

	m_totItem.WriteTotReport(ostr,
							 strstr.str(),
							 "Caster");
	strstr.freeze(false);
}




//////////////////////////////////////////////////////////////////////////////
//
//  CPstConformanceReport::CStringHolder
//
//////////////////////////////////////////////////////////////////////////////


//
//  Add summary information for each order in the string.
//  Summary information is computed per CI code.
//
//  Then sum over all ci code summaries to compute the string summary
//

void CPstConformanceReport::CStringHolder::Compute()
{
	{
		for ( vector<CCSOrder*>::iterator io = m_pString->StrandBegin(1);
			  io != m_pString->StrandEnd(1);
			  ++io ) 
			AddCSOrder((*io));
	}

	{
		for ( vector<CCSOrder*>::iterator io = m_pString->StrandBegin(2);
			  io != m_pString->StrandEnd(2);
			  ++io ) 
			AddCSOrder((*io));
	}

	ComputeTotals();
}


//
//	Find (or create) a CItem for this order's CI code.
//  Use -1 as the ci code if we cannot compute one (no order).
//

void CPstConformanceReport::CStringHolder::AddCSOrder(CCSOrder* m_pOrder)
{
	int ciCode = m_pOrder->Order() == 0 ? -1 : m_pOrder->Order()->CICode();

	T_MapIntItem::iterator ii = m_items.find(ciCode);
	CItem* pItem;

	if ( ii == m_items.end() ) {
		// no item for this ciCode, create one.
		pItem = new CItem(ciCode);
		m_items.insert(T_MapIntItem::value_type(ciCode,pItem));
	}
	else
		pItem = (*ii).second;

	pItem->AddCSOrder(m_pOrder);
}


//
//  Total the summary data from each CI code.
//

void CPstConformanceReport::CStringHolder::ComputeTotals()
{
	for ( T_MapIntItem::iterator ii = m_items.begin();
		  ii != m_items.end();
		  ++ii ) 
		m_totItem.AddItem((*ii).second);
}



//
//  Write a header, a detail line for each CI code,
//  and summary info for the string.
//

void CPstConformanceReport::CStringHolder::WriteReport(ostream& ostr)
{
	WriteHeader(ostr);

	for ( T_MapIntItem::iterator ii = m_items.begin();
		  ii != m_items.end();
		  ++ii ) 
		(*ii).second->WriteDetailLine(ostr);

	ostrstream tagLine;
	tagLine << "String # "
			<< m_pString->Id().Week()
			<< "-"
			<< m_pString->Id().StringId()
			<< ends;


	ostrstream header;
	header << "Total tons for # "
		   << m_pString->CasterScen()->Caster()
		   << " caster, "
		   << tagLine.str()
		   << ends;


	m_totItem.WriteTotReport(ostr,
							 header.str(),
							 tagLine.str());
	header.freeze(false);
	tagLine.freeze(false);
}

void CPstConformanceReport::CStringHolder::WriteHeader(ostream& ostr)
{
	ostr <<	"<h3 align='center'>Planned cast string PST conformance report<br>"
		 <<	"Commodity summary<br>"
		 <<	"for # " << m_pString->CasterScen()->Caster() << " caster</h3>";

	CTime now = CTime::GetCurrentTime();
	CString timeStr = now.Format("%X %x");
	ostr << "<h4 aligh='center'>" << LPCTSTR(timeStr) << "</h4>\n";

	CItem::WriteHeader(ostr);


#if 0
	// old text version, delete eventually
	ostr <<	"                          Planned cast string PST conformance report\n"
		 <<	"                                      Commodity summary\n"
		 <<	"                                       for # " << m_pString->CasterScen()->Caster() << " caster\n";

	CTime now = CTime::GetCurrentTime();
	CString timeStr = now.Format("%X %x");
	ostr << LPCTSTR(timeStr) << "\n";

	CItem::WriteHeader(ostr);
#endif
}




//////////////////////////////////////////////////////////////////////////////
//
//  CPstConformanceReport::CItem
//
//////////////////////////////////////////////////////////////////////////////


//
//  An order can contribute summary information to more than one column.
//  Specifically, we have some overlapping categories, 
//     e.g. an order to contribute to columns  
//       CAT_PST_GT14, CAT_PST_22_28, and certainly CAT_PST_TOT.
//

void CPstConformanceReport::CItem::ComputeCategories(vector<CPstConformanceReport::CItem::E_Category>& categories, 
													 CCSOrder* pCSOrder)
{
	categories.push_back(CAT_PST_TOTAL);

	COrder* pOrder = pCSOrder->Order();
	if (  pOrder == 0 ) 
		return;

	long delta = NTime::PSDateDelta(pOrder->PlannedLatestStartDate());

	if ( delta < 10 ) {
		categories.push_back(CAT_PST_LT_8);
	}
	else if ( delta <= 16 ) {
		categories.push_back(CAT_PST_8_14);
	}
	else if ( delta <= 23 ) {
		categories.push_back(CAT_PST_15_21);
		categories.push_back(CAT_PST_GT_14);
	}
	else if ( delta <= 30 ) {
		categories.push_back(CAT_PST_22_28);
		categories.push_back(CAT_PST_GT_14);
	}
	else if ( delta <= 37 ) {
		categories.push_back(CAT_PST_29_35);
		categories.push_back(CAT_PST_GT_14);
	}
	else { 
		categories.push_back(CAT_PST_GT_35);
		categories.push_back(CAT_PST_GT_14);
	}
}


// used for debugging

static const char* CategoryToString(CPstConformanceReport::CItem::E_Category cat)
{
	switch ( cat ) {
	case CPstConformanceReport::CItem::CAT_PST_LT_8:
		return "< 8  ";

	case CPstConformanceReport::CItem::CAT_PST_8_14:
		return "8-14 ";

	case CPstConformanceReport::CItem::CAT_PST_GT_14:
		return ">14  ";

	case CPstConformanceReport::CItem::CAT_PST_15_21:
		return "15-21";

	case CPstConformanceReport::CItem::CAT_PST_22_28:
		return "22-28";

	case CPstConformanceReport::CItem::CAT_PST_29_35:
		return "29-35";

	case CPstConformanceReport::CItem::CAT_PST_GT_35:
		return ">35  ";

	case CPstConformanceReport::CItem::CAT_PST_TOTAL:
		return "TOT  ";
	}

	return "??   ";
}




void CPstConformanceReport::CItem::AddCSOrder(CCSOrder* pCSOrder)
{
	//  assume this order has the correct ciCode

	vector<E_Category> categories;
	ComputeCategories(categories,pCSOrder);

	Weight tons = pCSOrder->Tons();
	bool isPrio = pCSOrder->Order() != 0 
					&& 
				  pCSOrder->Order()->CommodityPriority() <= 10;

	for ( vector<E_Category>::iterator ic = categories.begin();
		  ic != categories.end();
		  ++ic ) {

		int index = (*ic);

		m_tonsTotal[ index ] += tons;

		if ( isPrio )
			m_tonsPrioLe10[ index ] += tons;
		else
			m_tonsPrioGt10[ index ] += tons;

#if 0
		afxDump << pCSOrder->CastString()->Id().Week() 
			    << "-"
				<< pCSOrder->CastString()->Id().StringId()
				<< ", add "
				<< long(tons)
				<< ", cat "
				<< CategoryToString((*ic))
				<< ", = "
				<< long(m_tonsPrioLe10[index])
				<< (isPrio ? "*, " : " , ")
				<< long(m_tonsPrioGt10[index])
				<< (isPrio ? " , " : "*, ")
				<< long(m_tonsTotal[index])
				<< "\n";
#endif

	}
}

 

void CPstConformanceReport::CItem::Zero()
{
	for ( int i = 0; i<NumCategories; ++i ) {
		m_tonsTotal[i]		= 0;
		m_tonsPrioLe10[i]	= 0;
		m_tonsPrioGt10[i]	= 0;
	}
}

void CPstConformanceReport::CItem::AddItem(CPstConformanceReport::CItem* pItem)
{
	for ( int i = 0; i<NumCategories; ++i ) {
		m_tonsTotal[i]		+= pItem->m_tonsTotal[i];
		m_tonsPrioLe10[i]	+= pItem->m_tonsPrioLe10[i];
		m_tonsPrioGt10[i]	+= pItem->m_tonsPrioGt10[i];
	}
}


void CPstConformanceReport::CItem::WriteTotReport(ostream& ostr,
												  const char* header,
												  const char* tagLine)
{
	ostr << "<tr><th colspan='19'><br><h3>" << header << "</h3></td></tr>\n";
	WriteDetailLine(ostr);

	if ( m_tonsTotal[CAT_PST_TOTAL] <= 0.01 ) {
		ostr << "<tr><th colspan='19'><br><h3>Can't compute summary, total too small.</h3></td></tr>\n";
		return;
	}


#if 0 
	// old text version, delete eventually

	ostr << "\n\n" << header << "\n";
	WriteDashes(ostr);
	WriteDetailLine(ostr);

	if ( m_tonsTotal[CAT_PST_TOTAL] <= 0.01 ) {
		ostr << "\n\nCan't compute summary, total too small.\n\n";
		return;
	}

	ostr << "\n";
#endif

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Raw priority =                  ",
					   m_tonsPrioLe10[CAT_PST_TOTAL]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Raw PST conformance =           ",
					   m_tonsTotal[CAT_PST_LT_8]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Raw prty tons + (PST < 8) PCT = ",
					   (m_tonsPrioLe10[CAT_PST_TOTAL]+m_tonsPrioGt10[CAT_PST_LT_8])/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Non-prty tons (PST  8-14) PCT = ",
					   m_tonsPrioGt10[CAT_PST_8_14]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Non-prty tons (PST 15-21) PCT = ",
					   m_tonsPrioGt10[CAT_PST_15_21]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Non-prty tons (PST 22-28) PCT = ",
					   m_tonsPrioGt10[CAT_PST_22_28]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Non-prty tons (PST 29-35) PCT = ",
					   m_tonsPrioGt10[CAT_PST_29_35]/m_tonsTotal[CAT_PST_TOTAL]);

	WriteTotDetailLine(ostr,
					   tagLine,
					   "Non-prty tons (PST > 35)  PCT = ",
					   m_tonsPrioGt10[CAT_PST_GT_35]/m_tonsTotal[CAT_PST_TOTAL]);


}

void CPstConformanceReport::CItem::WriteTotDetailLine(ostream& ostr,
													  const char* tagLine,
													  const char* descr,
													  double value)
{
	double percent = value*100.0;
	ostr << "<tr>"
		 << "<td colspan='6' align='right'>" << tagLine << "</td>"
		 << "<td>&nbsp;</td>"
		 << "<td colspan='7' align='left'>" << descr << "</td>"
		 << "<td>&nbsp;</td>"
		 << "<td colspan='2' align='right'>" << setw(6) << setprecision(2) << percent
		 << " %"
		 << "</td>"
		 << "<td colspan='2'>&nbsp;</td>"
		 << "</tr>\n";

#if 0
	// old text version, delete eventually
	double percent = value*100.0;
	ostr << "\t\t\t" << tagLine 
		 << "\t" << descr
		 << setw(6) << setprecision(2) << percent
		 << " %\n";
#endif
}


void CPstConformanceReport::CItem::WriteDetailLine(ostream& ostr)
{
	ostr << "<tr align='right'>";
	if ( m_ciCode < 0 )
		ostr << "<td>&nbsp;</td>";
	else
		ostr << "<td align='left'>" << setw(2) << m_ciCode << "</td>";

	ostr << "<td>&nbsp;</td>";
	ostr << "<td>"
		 << long(m_tonsPrioLe10[CAT_PST_LT_8])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioLe10[CAT_PST_8_14])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioLe10[CAT_PST_GT_14])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioLe10[CAT_PST_TOTAL])
		 << "</td>";
	ostr << "<td>&nbsp;</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_LT_8])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_8_14])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_15_21])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_22_28])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_29_35])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_GT_35])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsPrioGt10[CAT_PST_TOTAL])
		 << "</td>";
	ostr << "<td>&nbsp;</td>";
	ostr << "<td>"
		 << long(m_tonsTotal[CAT_PST_LT_8])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsTotal[CAT_PST_8_14])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsTotal[CAT_PST_GT_14])
		 << "</td>";
	ostr << "<td>"
		 << long(m_tonsTotal[CAT_PST_TOTAL])
		 << "</td>";
	ostr << "</tr>\n";

#if 0
	// old text version, delete eventually.
	if ( m_ciCode < 0 )
		ostr << "    ";
	else
		ostr << " " << setw(2) << m_ciCode << " ";

	ostr << " "
		 << setw(4) << long(m_tonsPrioLe10[CAT_PST_LT_8]);
	ostr << " "
		 << setw(4) << long(m_tonsPrioLe10[CAT_PST_8_14]);
	ostr << " "
		 << setw(4) << long(m_tonsPrioLe10[CAT_PST_GT_14]);
	ostr << " "
		 << setw(5) << long(m_tonsPrioLe10[CAT_PST_TOTAL]);
	ostr << "  "
		 << setw(4) << long(m_tonsPrioGt10[CAT_PST_LT_8]);
	ostr << " "
		 << setw(4) << long(m_tonsPrioGt10[CAT_PST_8_14]);
	ostr << " "
		 << setw(5) << long(m_tonsPrioGt10[CAT_PST_15_21]);
	ostr << " "
		 << setw(5) << long(m_tonsPrioGt10[CAT_PST_22_28]);
	ostr << " "
		 << setw(5) << long(m_tonsPrioGt10[CAT_PST_29_35]);
	ostr << " "
		 << setw(4) << long(m_tonsPrioGt10[CAT_PST_GT_35]);
	ostr << " "
		 << setw(5) << long(m_tonsPrioGt10[CAT_PST_TOTAL]);
	ostr << "  "
		 << setw(4) << long(m_tonsTotal[CAT_PST_LT_8]);
	ostr << " "
		 << setw(4) << long(m_tonsTotal[CAT_PST_8_14]);
	ostr << " "
		 << setw(4) << long(m_tonsTotal[CAT_PST_GT_14]);
	ostr << " "
		 << setw(5) << long(m_tonsTotal[CAT_PST_TOTAL]);
	ostr << "\n";
#endif
}

// static 
void CPstConformanceReport::CItem::WriteDashes(ostream& ostr)
{
	// don't need this anymore?
	//ostr <<	"---- ---- ---- ---- -----  ---- ---- ----- ----- ----- ---- -----  ---- ---- ---- -----\n";
}

// static 
void CPstConformanceReport::CItem::WriteHeader(ostream& ostr)
{
	ostr << "<table border='1'>\n";

	ostr << "<tr>"
		 << "<td>&nbsp;</td>"
		 << "<td>&nbsp;</td>"
		 << "<th colspan='4'>Prio codes &lt; 10</th>"
		 << "<td>&nbsp;</td>"
		 << "<th colspan='7'>Prio codes &gt; 10</th>"
		 << "<td>&nbsp;</td>"
		 << "<th colspan='4'>Prio totals</th>"
		 << "</tr>\n";

	ostr << "<tr valign='bottom'>"
		 << "<th width='6%'>CI<br>Code</th>"
		 << "<td>&nbsp;</td>"
		 << "<th width='6%'>PST<br>&lt;8</th>"
		 << "<th width='6%'>PST<br>8-14</th>"
		 << "<th width='6%'>PST<br>&gt;14</th>"
		 << "<th width='6%'>Total</th>"
		 << "<td>&nbsp;</td>"
		 << "<th width='6%'>PST<br>&lt;8</th>"
		 << "<th width='6%'>PST<br>8-14</th>"
		 << "<th width='6%'>PST<br>15-21</th>"
		 << "<th width='6%'>PST<br>22-28</th>"
		 << "<th width='6%'>PST<br>29-35</th>"
		 << "<th width='6%'>PST<br>&gt;35</th>"
		 << "<th width='6%'>Total</th>"
		 << "<td>&nbsp;</td>"
		 << "<th width='6%'>PST<br>&lt;8</th>"
		 << "<th width='6%'>PST<br>8-14</th>"
		 << "<th width='6%'>PST<br>&gt;14</th>"
		 << "<th width='6%'>Total</th>"
		 << "</tr>\n";


#if 0
	// old text version, delete eventually
	ostr << "       PRIO CODES <= 10               PRIORITY CODES > 10             PRIORITY TOTALS\n"
		    "     --------------------  --------------------------------------  --------------------\n"
			" CI   PST  PST  PST TOTAL   PST  PST  PST   PST   PST   PST TOTAL   PST  PST  PST TOTAL\n"
			"CODE   <8 8-14  >14 TOTAL    <8 8-14 15-21 22-28 29-35  >35 TOTAL    <8 8-14  >14 TOTAL\n";

	WriteDashes(ostr);
#endif
}



