// PlanWorksheetReport.cpp: implementation of the CPlanWorksheetReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "PlanWorksheetReport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "CasterScen.h"
#include "CastString.h"
#include "CSOrder.h"
#include "Order.h"
#include "CastStringSummaryRecord.h"
#include "MultiSelectDlg.h"
#include "NReport.h"
#include "ReportDlg.h"

void CPlanWorksheetReport::Create()
{
	assert( m_pScen != 0 );

	// Determine which strings to report on
	CMultiSelectDlg dlg;

	{ 
		ostrstream ostr;
		for ( vector<CCastString*>::iterator is = m_pScen->CastStrings().begin();
			  is != m_pScen->CastStrings().end();
			  ++is ) {
			ostr << "Caster " << m_pScen->Caster()
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
		}
	}
	

	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.m_posns.size() == 0 )
		return;

	m_selectedStrings.clear();
	CopyVectorItems(m_selectedStrings,
						m_selectedStrings.end(),
						m_pScen->CastStrings(),
						dlg.m_posns);

	sort(m_selectedStrings.begin(),
		 m_selectedStrings.end(),
		 less_p<CCastString*>());


	//  Write the summary information to a file and make available for printing.
	CreateReport();
}


void CPlanWorksheetReport::CreateReport()
{
	CString filename;;

	WriteReport(filename);
	DisplayReport(filename);
	DeleteFile(filename);
}


void CPlanWorksheetReport::WriteReport(CString& filename)
{
	// open a temporary file

	ofstream ofs;
	
	if ( ! NReport::OpenStdHtmlFile(ofs,
									"PlanWorksheetReport",
									"Plan Worksheet Report",
									true, // need landscape
									filename) )
		return;

	ofs.setf(ios_base:: fixed, ios_base:: floatfield);

	WriteReportHeader(ofs);

	CCastStringSummaryRecord summary;
	int heatCount = 0;

	int week = m_selectedStrings[0]->Id().Week();
	int pageNum = 1;

	WritePageHeader(ofs,pageNum,week);

	for ( vector<CCastString*>::iterator is = m_selectedStrings.begin();
		  is != m_selectedStrings.end();
		  ++is ) {

		if ( week != (*is)->Id().Week() ) {
			WritePageFooter(ofs,pageNum);
			week = (*is)->Id().Week();
			++pageNum;
			WritePageHeader(ofs,pageNum,week);
		}

		WriteCastStringData(ofs,(*is));
		summary.Sum((*is)->StringSummary());
		// ## add cast to int
		heatCount += (int)(*is)->Heats().size();
	}
	
	WritePageFooter(ofs,pageNum);
	
	++pageNum;
	WriteSummaryPageHeader(ofs,pageNum);
	WriteSummaryData(ofs,summary,heatCount);
	WritePageFooter(ofs,pageNum);



	// done
	NReport::CloseStdHtmlFile(ofs);

}


void CPlanWorksheetReport::WriteReportHeader(ostream& ostr)
{
	// nothing
}


void CPlanWorksheetReport::WritePageHeader(ostream& ostr, int pageNum, int week)
{
	ostr << "<h2>Plan Worksheet, Caster = " << m_pScen->Caster() 
		 << "</h2>\n<h3> Week = " << week
	     << "</h3>\n<h4> Scen # = " << m_pScen->Id()
		 << "</h4>\n";
	ostr << "<table border='1' cellpadding='1' cellspacing='0'>\n";
	ostr << "<tr>"
		 << "<th>String</th>"
		 << "<th>Spec</th>"
		 << "<th>Hts</th>"
		 << "<th>W Str 1</th>"
		 << "<th>W Str 2</th>"
		 << "<th>#CMS</th>"
		 << "<th>CI Codes</th>"
		 << "<th>LU Types</th>"
		 << "</tr>\n";
}

void CPlanWorksheetReport::WriteSummaryPageHeader(ostream& ostr, int pageNum)
{
	ostr << "<h2>Plan Worksheet, Caster = " << m_pScen->Caster() 
	     << "</h3>\n<h4> Scen # = " << m_pScen->Id()
		 << "</h4>\n"
		 << "<h4>Summary</h4>\n";
}



void CPlanWorksheetReport::WritePageFooter(ostream& ostr, int pageNum)
{
	ostr << "</table>";
	ostr << "<p align='right'>Page = " << pageNum << "</p>";
	NReport::WriteHtmlNewPage(ostr);
}


void CPlanWorksheetReport::WriteCastStringData(ostream& ostr, CCastString* pString)
{
	ostr << "<tr>";

	ostr << "<td align='center'>" << pString->Id().StringId() << "</td>";
	ostr << "<td align='center'>" << LPCTSTR(ComputeStringSpec(pString)) << "</td>";
	ostr << "<td align='center'>" << int(pString->Heats().size()) << "</td>";
	ostr << "<td align='center'>" << LPCTSTR(ComputeStrandWidths(pString,1)) << "</td>";
	ostr << "<td align='center'>" << LPCTSTR(ComputeStrandWidths(pString,2)) << "</td>";
	ostr << "<td align='center'>" << pString->StringSummary().m_numCMS  + pString->StringSummary().m_numCMSFlips << "</td>";

	WriteCiCodeData(ostr,pString->StringSummary());
	WriteLuTypeData(ostr,pString->StringSummary());

	ostr << "</tr>\n";
}


void CPlanWorksheetReport::WriteSummaryData(ostream& ostr, 
											CCastStringSummaryRecord& rRecord,
											int heatCount)
{
	ostr << "<table border='1' cellpadding='1' cellspacing='0'>\n";
	ostr << "<tr>"
		 << "<td># Heats</td><td>" << heatCount << "</td>"
		 << "</tr>\n"
		 << "<tr>"
		 << "<td># CMS</td><td>" << rRecord.m_numCMS  + rRecord.m_numCMSFlips << "</td>"
		 << "</tr>\n"
		 << "</table>";

	// extra table needed because the following routines write embedded tables, including the td's.
	ostr << "<table border='0' cellpadding='0' cellspacing='0'>\n";
	ostr << "<tr>";
	WriteCiCodeData(ostr,rRecord);
	ostr << "</tr>";
	ostr << "</table>\n";

	ostr << "<table border='0' cellpadding='0' cellspacing='0'>\n";
	ostr << "<tr>";
	WriteLuTypeData(ostr,rRecord);
	ostr << "</tr>";
	ostr << "</table>\n";
}


CString CPlanWorksheetReport::ComputeStringSpec(CCastString* pString)
{
	// find first heat with a spec assigned.
	// use spec of first order otherwise

	CString strSpec =  "       ";
	{
		CCSOrder* pCSO = 0;
		if ( pString->Strand(1).size() != 0 )
			pCSO = pString->Strand(1)[0];
		else if ( pString->Strand(2).size() != 0 )
			pCSO = pString->Strand(2)[0];
		if ( pCSO != 0 ) 
			strSpec = (pCSO->Order() == 0 
					   ? pCSO->LotSpec()
					   : pCSO->Order()->OrderedSpec());
	}

	for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
	      ih != pString->Heats().end();
		  ++ih ) {
			
		if ( (*ih).Spec() != ""  && (*ih).Spec() != "       " ) {
			  strSpec = (*ih).Spec();
			  break;
		  }
	}

	return strSpec;
}


CString CPlanWorksheetReport::ComputeStrandWidths(CCastString* pString, int strandNum)
{
	vector<CCSOrder*>& strand = pString->Strand(strandNum);
	if ( strand.size() == 0 )
		return "?? - ??";

	int startWidth = strand.front()->SlabWidth();
	int endWidth = strand.back()->SlabWidth();

	int minWidth = min(startWidth,endWidth);
	int maxWidth = max(startWidth,endWidth);
	
	bool increasing = startWidth < endWidth;

	bool intermediateMinFound = false;
	bool intermediateMaxFound = false;

	for ( vector<CCSOrder*>::iterator io = strand.begin();
	      io != strand.end();
		  ++io ) {
		if ( (*io)->SlabWidth() < minWidth ) {
			minWidth = (*io)->SlabWidth();
			intermediateMinFound = true;
		}
		else if ( (*io)->SlabWidth() > maxWidth ) {
			maxWidth = (*io)->SlabWidth();
			intermediateMaxFound = true;
		}
	}

	ostrstream ostr;
	ostr << startWidth;
	if ( intermediateMinFound ) 
		ostr << "-" << minWidth;
	if ( intermediateMaxFound )
		ostr << "-" << maxWidth;
	ostr << "-" << endWidth;
	ostr << ends;

	CString str = ostr.str();
	ostr.freeze(false);

	return str;
}


void CPlanWorksheetReport::WriteCiCodeData(ostream& ostr, const CCastStringSummaryRecord& record)
{
	vector<int> ciCodes;
	vector<int> tons;

	static int numEntries = 5;

	for ( CCastStringSummaryRecord::CICodeToTonsMap::const_iterator im 
			= record.m_ciCodeTonsMap.begin();
		  im != record.m_ciCodeTonsMap.end();
		  ++im ) { 
		ciCodes.push_back((*im).first);
		tons.push_back(int((*im).second+0.5));
	}

//	{
//		int deficit = max(0,numEntries-ciCodes.size());
//
//		for ( int i =0; i<deficit; ++i ) {
//			ciCodes.push_back(0);
//			tons.push_back(0);
//		}
//	}
	
		  // ## add cast to int
	int percentage = 100 / (int) ciCodes.size();

	ostr << "<td>";
//	ostr << "<table border='1'  width ='100%' cellpadding='1' cellspacing='0'>";
	ostr << "<table border='1'  cellpadding='1' cellspacing='0'>";

	ostr << "<tr>";
	{
		for ( vector<int>::iterator ii = ciCodes.begin();
			  ii != ciCodes.end();
			  ++ii ) {
//			ostr << "<td width=" << percentage << "%>";
			ostr << "<td width='40'>";
			if ( (*ii) == 0 )
				ostr << "&nbsp;";
			else
				ostr << (*ii);
			ostr << "</td>";
		}
	}
	ostr << "</tr>";

	ostr << "<tr>";
	{
		for ( vector<int>::iterator ii = tons.begin();
			  ii != tons.end();
			  ++ii ) {
//			ostr << "<td width=" << percentage << "%>";
			ostr << "<td width='40'>";
			if ( (*ii) == 0 )
				ostr << "&nbsp;";
			else
				ostr << (*ii);
			ostr << "</td>";
		}
	}
	ostr << "</tr>";

	ostr << "</table>";
	ostr << "</td>";
}


void CPlanWorksheetReport::WriteLuTypeData(ostream& ostr, const CCastStringSummaryRecord& record)
{
	vector<CString> luTypes;
	vector<int> tons;

	static int numEntries = 5;

	for ( CCastStringSummaryRecord::HsmLuTypeToTonsMap::const_iterator im 
			= record.m_luTypeTonsMap.begin();
		  im != record.m_luTypeTonsMap.end();
		  ++im ) { 
		luTypes.push_back((*im).first);
		tons.push_back(int((*im).second+0.5));
	}

//	{
//		int deficit = max(0,numEntries-luTypes.size());
//
//		for ( int i =0; i<deficit; ++i ) {
//			luTypes.push_back("&nbsp;");
//			tons.push_back(0);
//		}
//	}
	
	// ## add cast to int
	int percentage = 100 / (int)luTypes.size();

	ostr << "<td>";
//	ostr << "<table border='1'  width ='100%' cellpadding='1' cellspacing='0'>";
	ostr << "<table border='1'  cellpadding='1' cellspacing='0'>";

	ostr << "<tr>";
	{
		for ( vector<CString>::iterator is = luTypes.begin();
			  is != luTypes.end();
			  ++is ) 
//			ostr << "<td width=" << percentage << "%>" << LPCTSTR((*is)) << "</td>";
			ostr << "<td width='40'>" << LPCTSTR((*is)) << "</td>";
	}
	ostr << "</tr>";

	ostr << "<tr>";
	{
		for ( vector<int>::iterator ii = tons.begin();
			  ii != tons.end();
			  ++ii ) {
//			ostr << "<td width=" << percentage << "%>";
			ostr << "<td width='40'>";
			if ( (*ii) == 0 )
				ostr << "&nbsp;";
			else
				ostr << (*ii);
			ostr << "</td>";
		}
	}
	ostr << "</tr>";

	ostr << "</table>";
	ostr << "</td>";
}



// 
//  Make the report available for printing.
//

void CPlanWorksheetReport::DisplayReport(CString& filename)
{

	CReportDlg dlg;
	dlg.m_filename = filename;
	dlg.DoModal();
}


// 
//  get rid of temporary file
//

void CPlanWorksheetReport::DeleteFile(CString& filename)
{
	unlink(LPCTSTR(filename));
}

