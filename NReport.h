// NReport.h: interface for the NReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NREPORT_H__9117717A_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
#define AFX_NREPORT_H__9117717A_CE15_11D4_8649_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace NReport  
{
	// These are reports that don't have another home

	// --- none at the moment


	// These are support routines.

	bool OpenStdHtmlFile(ofstream& file,
							const char* shortName,
							const char* title,
							bool landscape,
							CString& filename);
	void CloseStdHtmlFile(ofstream& file);

	CString OpenHtmlFile(ofstream& file,const char* shortName);

	void WriteStdHtmlHeader(ostream& ostr,const char* title, const char* otherHeader=0);
	void WriteStdHtmlBodyOpen(ostream& ostr, bool landscape=false);
	void WriteStdHtmlBodyClose(ostream& ostr);
	void WriteHtmlNewPage(ostream& ostr);
	void SetHtmlPrintPageHeader(ostream& ostr, const char* str);
	void SetHtmlPrintPageFooter(ostream& ostr, const char* str);


};

#endif // !defined(AFX_NREPORT_H__9117717A_CE15_11D4_8649_00104B2D39AC__INCLUDED_)
