// PlanWorksheetReport.h: interface for the CPlanWorksheetReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANWORKSHEETREPORT_H__436DB9CA_F48B_4B5F_B59C_3614A0649934__INCLUDED_)
#define AFX_PLANWORKSHEETREPORT_H__436DB9CA_F48B_4B5F_B59C_3614A0649934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCasterScen;
class CCastString;
class CCastStringSummaryRecord;


class CPlanWorksheetReport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Display Plan Worksheet Report
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CCasterScen* m_pScen;
	vector<CCastString*> m_selectedStrings;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CCasterScen* CasterScen() 
	{	return m_pScen;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CPlanWorksheetReport(CCasterScen* pScen) : m_pScen(pScen) {}
	virtual ~CPlanWorksheetReport() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void Create();


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void CreateReport();
	void WriteReport(CString& filename);
	void DisplayReport(CString& filename);
	void DeleteFile(CString& filename);

	void WriteReportHeader(ostream& ostr);
	void WritePageHeader(ostream& ostr, int pageNum, int week);
	void WriteSummaryPageHeader(ostream& ostr, int pageNum);
	void WritePageFooter(ostream& ostr,int pageNum);
	void WriteCastStringData(ostream& ostr, CCastString* pString);
	void WriteSummaryData(ostream& ostr, CCastStringSummaryRecord& rRecord, int heatCount);

	CString ComputeStringSpec(CCastString* pString);
	CString ComputeStrandWidths(CCastString* pString, int strandNum);

	void WriteCiCodeData(ostream& ostr, const CCastStringSummaryRecord& record);
	void WriteLuTypeData(ostream& ostr, const CCastStringSummaryRecord& record);




};

#endif // !defined(AFX_PLANWORKSHEETREPORT_H__436DB9CA_F48B_4B5F_B59C_3614A0649934__INCLUDED_)
