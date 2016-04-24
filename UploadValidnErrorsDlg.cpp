// UploadValidnErrorsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "UploadValidnErrorsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastStringHeatValidnError.h"
#include "CastString.h"


/////////////////////////////////////////////////////////////////////////////
// CUploadValidnErrorsDlg dialog
//
//  Used during the upload process.
//  Only called if there are strings with validation errors in the upload
//  Displays the list of strings to be uploaded.
//  Displays the validation messages of the selected string, if any.
//	Displays an informative text.
//
//	Usage:
//
//		CUploadValidnErrorsDlg dlg;
//
//		//  code to determine which strings have validation errors.
//      dlg.m_strings.push_back( <CString with errors> );
//
//		dg.m_bFatalErr = <true/false depending>;
//
//		if ( dlg.DoModal() == IDOK ) {
//			// user says to go ahead and upload
//		}
//
//
//////////////////////////////////////////////////////////////////////////////


CUploadValidnErrorsDlg::CUploadValidnErrorsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadValidnErrorsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUploadValidnErrorsDlg)
	m_msg = _T("");
	//}}AFX_DATA_INIT

	m_bFatalErr = false;
}


void CUploadValidnErrorsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadValidnErrorsDlg)
	DDX_Control(pDX, IDC_LIST_STRINGS, m_listStrings);
	DDX_Control(pDX, IDC_LIST_VALIDN, m_listValidation);
	DDX_Text(pDX, IDC_MSG, m_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUploadValidnErrorsDlg, CDialog)
	//{{AFX_MSG_MAP(CUploadValidnErrorsDlg)
	ON_LBN_SELCHANGE(IDC_LIST_STRINGS, OnSelchangeListStrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUploadValidnErrorsDlg message handlers



BOOL CUploadValidnErrorsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CCastStringHeatValidnError::SetHeader(m_listValidation);

	vector<CCastStringHeatValidnError> empty;

	ostrstream ostr;

	for ( vector<CCastString*>::iterator is = m_strings.begin();
		  is != m_strings.end();
		  ++is ) {

		ostr << "#" << (*is)->Id().Caster()
			 << ( (*is)->Id().Caster() == 3 ? "CC" : "SC" )
			 << ": "
			 << (*is)->Id().Year()
			 << " "
			 << (*is)->Id().Week()
			 << " "
			 << (*is)->Id().StringId()
			 << ends;
		m_listStrings.AddString(ostr.str());
		ostr.freeze(false);
		ostr.seekp(0);
	}


	if ( m_strings.size() == 0 )
		CCastStringHeatValidnError::UpdateList(m_listValidation,empty);
	else {
		CCastStringHeatValidnError::UpdateList(m_listValidation,m_strings.front()->ValidnErrors());
		m_listStrings.SetSel(0);
	}
		
	m_msg = "The strings selected for upload have validation errors.\n"
		    "Please examine carefully before continuing.";

#if 0
	// This was too strict for the folks, despite their requesting it.
	if ( m_bFatalErr ) {
		m_msg += "\n\nNOTE: there are fatal errors.\n"
				 "You will not be allowed to upload.";

		this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
#endif
	// Here's the less strict version..
	if ( m_bFatalErr ) {
		m_msg += "\n\nNOTE: there are fatal errors.\n"
				 "Proceed with strict caution.";
	}

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CUploadValidnErrorsDlg::OnSelchangeListStrings() 
{
	int index = m_listStrings.GetCurSel();
	

	if ( index == LB_ERR ) {

		vector<CCastStringHeatValidnError> empty;
		CCastStringHeatValidnError::UpdateList(m_listValidation,empty);
	}
	else 
		CCastStringHeatValidnError::UpdateList(m_listValidation,m_strings[index]->ValidnErrors());

}


