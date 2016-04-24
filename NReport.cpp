// NReport.cpp: implementation of the NReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "NReport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ReportDlg.h"
#include "UserInfo.h"


/////////////////////////////////////////////////////////////////////
//
//  A gathering place for miscellaneous reports and auxiliary routines.
//
//  No reason these couldn't have been methods in CSnapshot,
//    except that these do UI things.
//
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//
// Support routines
//
/////////////////////////////////////////////////////////////////////

//  
//  We do our report writing using HTML.
//  We write to a file, then bring up a dialog (CReportDlg) which hosts
//     a web browser object to view it.
//  We embed a special ActiveX control on the page to help control printing.
//  Specifically, this control allows us to print and allows us to set 
//    printing characteristics, such as landscape vs portrait.
//


//
//  Open an HTML file for writing.
//  Output a standard header.
//  Return true if open was successful.
//

bool NReport::OpenStdHtmlFile(ofstream& file,
								 const char* shortName,
								 const char* title,
								 bool landscape,
								 CString& filename)
{
	filename = OpenHtmlFile(file,shortName);

	if ( file ) {
		WriteStdHtmlHeader(file,title);
		WriteStdHtmlBodyOpen(file,landscape);
		return true;
	}
	else {
		AfxMessageBox("Unable to open temporary file for writing.");
		return false;
	}
}



//
// Close an HTML report file by writing an standard closing
//   and then closing the file.
//

void NReport::CloseStdHtmlFile(ofstream& file)
{
	WriteStdHtmlBodyClose(file);
	file.close();
}




//  
//  The runtime environment is set up with a subdirectory
//    of the parent of the working directory named temp.
//  Our standard naming convention for files is a standardized name
//  for the report with the login name concatenated in front of it.
//

CString NReport::OpenHtmlFile(ofstream& file,const char* shortName)
{
	SECFileSystem fs;
	CString currentDir = fs.GetCurrentDirectory();

	CString filename;
	filename.Format("%s\\..\\temp\\%s_%s.htm",
					LPCTSTR(currentDir),
					LPCTSTR(CUserInfo::TheUser.LoginName()),
					shortName);

	file.open(LPCTSTR(filename));
	
	return filename;
}



//
//  The standard HTML file for reports contains
//
//		(1) a title
//      (2) the inclusion of a standard javascript file that contains some printing support
//		(3) a link to a style sheet that controls fonts, headers, etc.
//		(4) anything else the user want to add to the HEAD.
//

void NReport::WriteStdHtmlHeader(ostream& ostr,const char* title, const char* otherHeader)
{
	ostr << "<HTML><HEAD><TITLE>" << title << "</TITLE>\n";
	ostr << "<SCRIPT LANGUAGE='JavaScript' src='..\\html\\std-head-include.js'></SCRIPT>\n";
	ostr << "<LINK href='../html/std.css' rel='STYLESHEET' type='text/css'>\n";
	if ( otherHeader != 0 )
		ostr << otherHeader;
	ostr << "</HEAD>\n";
}


//
//  Following the HEAD, we start the BODY
//  Probably we should allow the user to put attributes in the HEAD tag,
//    but never got around to it.
//  The beginning of the body also includes
//		(1) the inclusion of a standard piece of javascript that embeds the ActiveX control
//          that helps with printing
//      (2) a button that brings up (via that control) a print settings dialog
//      (3) a button that prints the document.
//      (4) optionally, a piece of code to default to landscape printing
//		(5) some code that sets the page header and footer for printing
//     

void NReport::WriteStdHtmlBodyOpen(ostream& ostr, bool landscape)
{
	ostr << "<BODY>";
	ostr << "<SCRIPT LANGUAGE='JavaScript' src='..\\html\\std-body-include.js'></SCRIPT>\n";
	if ( landscape ) 
		ostr << "<SCRIPT LANGUAGE='JavaScript'>myPortrait=false;</SCRIPT>\n";
	SetHtmlPrintPageHeader(ostr,"&w&b&bPage &p of &P");
	SetHtmlPrintPageFooter(ostr,"&b&b&d");
}

void NReport::SetHtmlPrintPageHeader(ostream& ostr, const char* str)
{
	ostr << "<SCRIPT LANGUAGE='JavaScript'>myHeader='" << str << "';</SCRIPT>\n";
}

void NReport::SetHtmlPrintPageFooter(ostream& ostr, const char* str)
{
	ostr << "<SCRIPT LANGUAGE='JavaScript'>myFooter='" << str << "';</SCRIPT>\n";
}



//  
//  Close the BODY and HTML tags
//

void NReport::WriteStdHtmlBodyClose(ostream& ostr)
{
	static char* footer = "</BODY></HTML>";
	ostr << footer;
}



//  
//  The standard stylesheet contains a .class descriptor
//   that will cause a page break before.
//  This writes in a DIV that will cause a page break.
//

void NReport::WriteHtmlNewPage(ostream& ostr)
{
	ostr << "<DIV class='pagebreak'>&nbsp</DIV>\n";
}
 



