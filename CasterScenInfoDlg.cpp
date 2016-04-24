// CasterScenInfoDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterScenInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CasterScen.h"
#include "CasterScenArchiver.h"
#include "CasterScenariosSet.h"
#include "DBConn.h"


// get rid of this when we move messages
#include "CastStringEditorMvc.h"


/////////////////////////////////////////////////////////////////////////////
//
//  CCasterScenInfoDlg dialog
//
//    Displays basic information about a caster scenario.
//    Allows the comment to be changed.
//
//    User can save the comment or cause an archive.
//
//
//  Usage:
//
//		CCasterScenInfoDlg dlg;
//		dlg.m_pCasterScen = <pointer to a CCasterScen>;
//		dlg.DoModal();
//
//
//////////////////////////////////////////////////////////////////////////////



CCasterScenInfoDlg::CCasterScenInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCasterScenInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCasterScenInfoDlg)
	m_casterNum = 0;
	m_comment = _T("");
	m_creationDate = time_t(0);
	m_id = 0;
	m_lastSaveDate = time_t(0);
	m_owner = _T("");
	m_version = 0;
	//}}AFX_DATA_INIT

	m_pCasterScen = 0;
}


void CCasterScenInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCasterScenInfoDlg)
	DDX_Text(pDX, IDC_EDIT_CASTER_NUM, m_casterNum);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_comment);
	DDX_Text(pDX, IDC_EDIT_CREATION_DATE, m_creationDate);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_LAST_SAVE_DATE, m_lastSaveDate);
	DDX_Text(pDX, IDC_EDIT_OWNER, m_owner);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_version);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCasterScenInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCasterScenInfoDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_COMMENT, OnButtonUpdateComment)
	ON_EN_CHANGE(IDC_EDIT_COMMENT, OnChangeEditComment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CCasterScenInfoDlg message handlers


//////////////////////////////
//  
//  Managing the display
//
//////////////////////////////


//
//  Enable buttons depending.
//

void CCasterScenInfoDlg::SetButtonStates()
{
	GetDlgItem(IDC_BUTTON_UPDATE_COMMENT)->EnableWindow(m_bCommentModified);

	GetDlgItem( IDC_BUTTON_SAVE   )->EnableWindow(m_pCasterScen->IsModified());
	GetDlgItem( IDC_BUTTON_REVERT )->EnableWindow(m_pCasterScen->IsModified());
}



//
//  To initialize, place caster scenario information on the screen.
//  We need to pick up create/lastsave times and version # from the db.
//

BOOL CCasterScenInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( m_pCasterScen != 0 );

	CDBConn dbc(CDBConn::DB_CSDA);
	dbc.Connect();
	CDatabase* pDBCSDA = dbc.GetDB();

	CCasterScenariosSet rs(pDBCSDA);


	bool result = CCasterScenArchiver::LocateCasterScenRecord(rs,
															  CRecordset::forwardOnly,
															  CRecordset::readOnly,
															  m_pCasterScen);

	m_casterNum		= m_pCasterScen->Caster();
	m_comment		= m_pCasterScen->Comment();
	m_id			= m_pCasterScen->Id();
	m_owner			= m_pCasterScen->Owner();

	if ( result ) {
		m_creationDate	= rs.m_CreateTime.GetTime();
		m_lastSaveDate	= rs.m_LastSaveTime.GetTime();
		m_version		= rs.m_Version;
	}
	else {
		m_version		= -1;
	}

	if ( rs.IsOpen() )
		rs.Close();

	UpdateData(false);

	m_bCommentModified = false;

	SetButtonStates();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////
//
//  Actions
//
/////////////////////////////////////////



void CCasterScenInfoDlg::OnButtonSave() 
{
	CCasterScenArchiver::Archive(m_pCasterScen,true);	
	SetButtonStates();

	CCastStringEditorTextViewport::CScenSavedMsg msg(m_pCasterScen);
	TheBus().UpdateAllObservers(0,&msg);
}


void CCasterScenInfoDlg::OnButtonUpdateComment() 
{
	if ( ! m_bCommentModified ) 
		return;

	m_bCommentModified = false;

	UpdateData(true);

	m_pCasterScen->SetComment(m_comment);

	CCasterScenArchiver::UpdateCasterScenInfo(m_pCasterScen);

	SetButtonStates();

	CCasterScenInfoChangedMsg msg(m_pCasterScen);
	TheBus().UpdateAllObservers(0,&msg);
}


void CCasterScenInfoDlg::OnChangeEditComment() 
{
	m_bCommentModified = true;
	GetDlgItem(IDC_BUTTON_UPDATE_COMMENT)->EnableWindow(true);
	
}

void CCasterScenInfoDlg::OnOK() 
{
	if ( m_bCommentModified ) {

		int result = MessageBox("The comment has been modified.\n"
								"Save change before closing?",
								"Save comment change?",
								MB_ICONQUESTION | MB_YESNOCANCEL);

		switch ( result ) {
		case IDYES:
			OnButtonUpdateComment();
			break;

		case IDNO:
			break;

		case IDCANCEL:
			return;
		}
	}
	
	CDialog::OnOK();
}


///////////////////////////////////////////////////////////
//
//  message
//
///////////////////////////////////////////////////////////


void CCasterScenInfoDlg::CCasterScenInfoChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("Caster Scen Info changed");
}

