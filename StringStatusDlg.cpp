// StringStatusDlg.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "StringStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CasterScen.h"
#include "CastString.h"
#include "CastStringHeat.h"
#include "HeatDetailsDlg.h"
#include "ScenMgr.h"
#include "Snapshot.h"
#include "DBConn.h"
#include "UploadMgr.h"
#include "LongOpn.h"
//#include "UploadDlg.h"
#include "UploadProcessMgr.h"
#include "CasterScenArchiver.h"
#include "UploadValidnErrorsDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CStringStatusDlg dialog


CStringStatusDlg::CStringStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringStatusDlg::IDD, pParent)
{
	m_pCasterScen	= 0;
	m_pSnapScen		= 0;
	m_pSuperScen	= 0;

	m_currentStringIndex = -1;	// no selection

	m_wndGrid.m_pStringStatusDlg = this;

	
	//{{AFX_DATA_INIT(CStringStatusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStringStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringStatusDlg)
	DDX_Control(pDX, IDC_LIST_HEATS, m_listHeats);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CStringStatusDlg)
	ON_BN_CLICKED(IDC_BUTTON_HEAT_DETAILS, OnButtonHeatDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringStatusDlg message handlers

BOOL CStringStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Make sure the user supplied the necessary parms
	assert( m_pCasterScen	!= 0 );
	assert( m_pSnapScen		!= 0 );
	assert( m_pSuperScen	!= 0 );
	
	//	Hook up the grid
	m_wndGrid.SubclassDlgItem(IDC_GRID_STRING_STATUS, this);
	m_wndGrid.Initialize();
	m_wndGrid.GetParam()->EnableUndo(FALSE);

	// get updates
	TheSnapshot.LoadNew910s();
	AnalyzeScenario();

	SetHeatListHeader();
	SetRows();

	// Set a current string
	if ( m_wndGrid.GetRowCount() > 0 )
		m_wndGrid.SetCurrentCell(1,COL_ACTION);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



// This is needed per instructions of Stingray

BOOL CStringStatusDlg::OnNcActivate(BOOL bActive)
{
	if (GXDiscardNcActivate())
		return TRUE;

	return CDialog::OnNcActivate(bActive);
}


CSnapScenChangeRecord::CSnapScenChangeRecord()
{
	m_pSnapString = 0;
	m_pScenString = 0;
	m_snapStatus = CCastString::STRING_STATUS_NEW;
	m_bEqual = false;
	m_action = UPLOAD_NO_CHANGE;
}



CSnapScenChangeRecord::CSnapScenChangeRecord( CCastString* pSnapString, 
											  CCastString* pScenString )
{
	m_pSnapString = pSnapString;
	m_pScenString = pScenString;

	//  if there is no snap string, then the scenario has a new string.
	//  else, we are either 909 or 910 from the snapshot.

	m_snapStatus = ( pSnapString == 0 
					 ? CCastString::STRING_STATUS_NEW
					 : pSnapString->Status() );

	
	if ( m_pScenString == 0  ) {

		// No matching scenario string
		// This snapshot string has been deleted

		m_bEqual	 = false;
		m_strStatus  = ( m_snapStatus == CCastString::STRING_STATUS_910 ? "910 - deleted" : "909 - deleted" );
		m_strChoices = "No change\nDelete";
		m_strAction  = "Delete";
	}
	else if ( m_pSnapString == 0 ) {

		//  no matching snapshot string
		//  this is a new string in the scenario

		m_bEqual	 = false;
		m_strStatus  = "New";
		m_strChoices = "910\n909\nNo change";
		m_strAction  = "909";
	}
	else if ( m_snapStatus == CCastString::STRING_STATUS_910 ) {

		//  snapshot string is 910, there is matching scenario string.
		//  we could be the same or different.

		if ( m_pScenString->IsSameString(m_pSnapString) ) {
			m_bEqual	 = true;
			m_strStatus  = "910 - No change";
			m_strChoices = "No change\nDelete";
			m_strAction  = "No change";
		}
		else {
			m_bEqual	 = false;
			m_strStatus  = "910 - Modified";
			m_strChoices = "Update\nNo change\nDelete";
			m_strAction  = "Update";
		}
	}
	else {
	
		//  snapshot string is 909, there is matching scenario string.
		//  we could be the same or different.

		if ( m_pScenString->IsSameString( m_pSnapString) ){
			m_bEqual	 = true;
			m_strStatus  = "909 - No change";
			m_strChoices = "910\nNo change\nDelete";
			m_strAction  = "No change";
		}
		else {
			m_bEqual	 = false;
			m_strStatus  = "909 - Modified";
			m_strChoices = "910\nUpdate\nNo change\nDelete";
			m_strAction  = "Update";
		}
	}
}




void CStringStatusDlg::AnalyzeScenario()
{
	m_changeRecords.clear();

	vector<CCastString*>& snapStrings = m_pSnapScen->CastStrings();
	vector<CCastString*>& scenStrings = m_pCasterScen->CastStrings();

	{
		for ( vector<CCastString*>::iterator is = snapStrings.begin();
			  is != snapStrings.end();
			  ++is ) {
			
			vector<CCastString*>::iterator ic;
	
			for ( ic = scenStrings.begin();
				  ic != scenStrings.end();
				  ++ic ) {
	
				if ( (*ic)->OrigString() == (*is) )
					break;
	
			}
	
			if ( ic == scenStrings.end() ) 
				// did _not_ find a string in the scenario which matched the snapshot string
				// therefore, this snapshot string has been deleted in the scenario
				m_changeRecords.push_back( CSnapScenChangeRecord((*is),0));
			else
				//  a string in the scenario refers back to this string 
				//	add a record pairing them.
				m_changeRecords.push_back( CSnapScenChangeRecord((*is),(*ic)) );
		}
	}

	{
		for ( vector<CCastString*>::iterator is = scenStrings.begin();
			  is != scenStrings.end();
			  ++is ) 

			if ( (*is)->OrigString() == 0 )
				//  this scenario string is not linked back to any scenario string
				//  deal with it.
				m_changeRecords.push_back( CSnapScenChangeRecord(0,(*is)) );
	}
}


void CStringStatusDlg::SetRows()
{
	//## cast to ROWCOL
	m_wndGrid.SetRowCount((ROWCOL) m_changeRecords.size());
	
	int rowNum = 1;

	for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		  ir != m_changeRecords.end();
		  ++ir, ++rowNum ) 

		SetRow(rowNum,(*ir));
}



void CStringStatusDlg::SetRow(int nRow, CSnapScenChangeRecord& record)
{
	CString strSnapId;
	CString strScenId;

	if ( record.m_pSnapString == 0 )
		strSnapId = "-";
	else
		strSnapId.Format("%04d-%02d-%d",
						 record.m_pSnapString->Id().Year(),
						 record.m_pSnapString->Id().Week(),
						 record.m_pSnapString->Id().StringId());

	
	if ( record.m_pScenString == 0 )
		strScenId = "-";
	else
		strScenId.Format("%04d-%02d-%d",
						 record.m_pScenString->Id().Year(),
						 record.m_pScenString->Id().Week(),
						 record.m_pScenString->Id().StringId());


	int tabNum = 0;
	CString strSpec;
	CString strCreateDate;
	CCastString* pString = ( record.m_pScenString != 0 
							 ? record.m_pScenString 
							 : record.m_pSnapString );

	{
		vector<CCastStringHeat>::iterator firstTabNumHeat = pString->Heats().end();
		vector<CCastStringHeat>::iterator firstSpecHeat = pString->Heats().end();
	
		for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
			  ih != pString->Heats().end();
			  ++ih ) {
	
			if ( (*ih).TabNum() != 0 && firstTabNumHeat == pString->Heats().end() ) {
				firstTabNumHeat = ih;
				break;
			}
	
			if ( (*ih).Spec() != "" && firstSpecHeat == pString->Heats().end() )
				firstSpecHeat = ih;
		}
	
		vector<CCastStringHeat>::iterator firstUse;
	
		if ( firstTabNumHeat != pString->Heats().end() )
			firstUse = firstTabNumHeat;
		else
			firstUse = firstSpecHeat;

		if ( firstUse != pString->Heats().end() ) {
	
			tabNum = (*firstUse).TabNum();
			strSpec = (*firstUse).Spec();
			// TODO
			//strCreateDate = (*firstUse).CreateDate();
		}
	}


	m_wndGrid.SetStyleRange( CGXRange( nRow, COL_ACTION ),
							 CGXStyle()
								.SetControl(GX_IDS_CTRL_TEXTFIT)
								.SetChoiceList(record.m_strChoices) );
	
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_CASTER		 ), long(pString->Id().Caster())			);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_ID			 ), strScenId								);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_TONS		 ), long(pString->StringSummary().m_tons)	);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_HEATS		 ), long(pString->Heats().size())			);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_ORIG_ID	 ), strSnapId								);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_STATUS		 ), record.m_strStatus						);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_ACTION		 ), record.m_strAction						);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_TAB_NUM	 ), long(tabNum)							);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_SPEC		 ), strSpec									);
	m_wndGrid.SetValueRange( CGXRange( nRow, COL_CREATE_DATE ),	strCreateDate							);
}


#ifdef CompWidth3
#undef CompWidth3
#endif
#ifdef CompWidth2
#undef CompWidth2
#endif

#ifdef DefCol
#undef DefCol
#endif

#define DefCol(format,str,wid) \
m_listHeats.InsertColumn(colNum,str,format,wid,colNum); ++colNum


#define CompWidth3(x,label,format) ( (x) + max(m_listHeats.GetStringWidth((label)),m_listHeats.GetStringWidth((format))) )


void CStringStatusDlg::SetHeatListHeader()
{
	static const char* strNum			= "#";
	static const char* strMark			= "Mark";
	static const char* strTabNum		= "Tab#";
	static const char* strTons			= "Tons";
	static const char* strSpec			= "Spec";
	static const char* strCRange		= "Carbon";
	static const char* strMnRange		= "Mn";
	static const char* strSigElRange	= "Signif El";
	static const char* strComment		= "Comment";

	const int widthNum			= CompWidth3( 15, strNum,			"XX");
	const int widthMark			= CompWidth3( 15, strMark,			"X");
	const int widthTabNum		= CompWidth3( 15, strTabNum,		"XXXXX");
	const int widthTons			= CompWidth3( 15, strTons,			"XXXX");
	const int widthSpec			= CompWidth3( 15, strSpec,			"XXX-XX-XX");
	const int widthCRange		= CompWidth3( 15, strCRange,		"X.XXX - X.XXX");
	const int widthMnRange		= CompWidth3( 15, strMnRange,		"X.XXX - X.XXX");
	const int widthSigElRange	= CompWidth3( 15, strSigElRange,	"XX: X.XXX - X.XXX");
	const int widthComment		= CompWidth3 (15, strComment,		"XXXXXXXXXXXXXXXXXX");

	int colNum = 0;

	DefCol(	LVCFMT_LEFT,	strNum,			widthNum		);	// 0
	DefCol(	LVCFMT_CENTER,	strMark,		widthMark		);	// 1
	DefCol(	LVCFMT_RIGHT,	strTabNum,		widthTabNum		);	// 2
	DefCol(	LVCFMT_RIGHT,	strTons,		widthTons		);	// 3
	DefCol(	LVCFMT_RIGHT,	strSpec,		widthSpec		);	// 4
	DefCol(	LVCFMT_RIGHT,	strCRange,		widthCRange		);	// 5
	DefCol(	LVCFMT_RIGHT,	strMnRange,		widthMnRange	);	// 6
	DefCol(	LVCFMT_RIGHT,	strSigElRange,	widthSigElRange	);	// 7
	DefCol(	LVCFMT_LEFT,	strComment,		widthComment	);	// 8
}



void CStringStatusDlg::SetHeatListItems()
{
	m_listHeats.DeleteAllItems();

	CCastString* pString = GetCurrentCastString();

	if ( pString == 0 )
		return;

	vector<CCastStringHeat>& heats = pString->Heats();


	int index = 0;

	for ( vector<CCastStringHeat>::const_iterator ih = heats.begin();
		  ih != heats.end();
		  ++ih, ++index ) 

		AddItemForHeat( (*ih), index );
}




void CStringStatusDlg::AddItemForHeat(const CCastStringHeat& rHeat, int index)
{
	CString strIndex;
	CString strTab;
	CString strTons;
	CString strCRange;
	CString strMnRange;
	CString strSigElRange;

	strIndex.Format("%d", index+1);
	strTab.Format("%04d",rHeat.TabNum());


	if ( rHeat.MinChem(CChem::ELT_C) == 0.0 
		 &&
		 rHeat.MaxChem(CChem::ELT_C) == 100.0)
		strCRange = "";
	else
		strCRange.Format ("%5.3f - %5.3f",
						  rHeat.MinChem(CChem::ELT_C),
						  rHeat.MaxChem(CChem::ELT_C));

	if ( rHeat.MinChem(CChem::ELT_MN) == 0.0 
		 &&
		 rHeat.MaxChem(CChem::ELT_MN) == 100.0)
		strMnRange = "";
	else
		strMnRange.Format ("%5.3f - %5.3f",
						  rHeat.MinChem(CChem::ELT_MN),
						  rHeat.MaxChem(CChem::ELT_MN));


	if ( rHeat.SignifEl() == -1 )
		strSigElRange	= "";
	else {

		CString strSigEl = CChem::ToString( rHeat.SignifEl() );
		CChem::Bound sigMin = rHeat.MinChem( rHeat.SignifEl() );
		CChem::Bound sigMax = rHeat.MaxChem( rHeat.SignifEl() );

		if ( sigMin == 0.0 && sigMax == 100.0 )
			strSigElRange.Format("%2s",LPCTSTR(strSigEl));
		else 
			strSigElRange.Format("%2s: %5.3f - %5.3f",
								 strSigEl,
								 sigMin,
								 sigMax);
	}
	
	m_listHeats.InsertItem(  index, LPCTSTR(strIndex) );

	m_listHeats.SetItemText( index, 1,  rHeat.IsMarked() ? "+" : " ");
	m_listHeats.SetItemText( index, 2,	LPCTSTR(strTab)			);
	m_listHeats.SetItemText( index, 3,	LPCTSTR(strTons)			);
	m_listHeats.SetItemText( index, 4,	LPCTSTR(rHeat.Spec())		);
	m_listHeats.SetItemText( index, 5,	LPCTSTR(strCRange)			);
	m_listHeats.SetItemText( index, 6,	LPCTSTR(strMnRange)			);
	m_listHeats.SetItemText( index, 7,	LPCTSTR(strSigElRange)		);
	m_listHeats.SetItemText( index, 8,	LPCTSTR(rHeat.Comment())	);

}




void CStringStatusDlg::OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol)
{

	// column  header
	if ( nrow < 1 )
		return;

	if ( nrow-1 == m_currentStringIndex )
		return;

	m_currentStringIndex = nrow-1;
	SetHeatListItems();
}


CCastString* CStringStatusDlg::GetCurrentCastString()
{
	if ( m_currentStringIndex < 0
		 ||
		 m_currentStringIndex >= m_changeRecords.size() )
		 return 0;

	return m_changeRecords[m_currentStringIndex].m_pScenString;

}



void CStringStatusDlg::OnButtonHeatDetails() 
{
	CCastString* pString = GetCurrentCastString();

	if ( pString == 0 ) {
		MessageBox("You must select a string first.",MB_OK);
		return;
	}

	CHeatDetailsDlg dlg;
	dlg.m_pCastString	= pString;
	dlg.m_pSuperScen	= m_pSuperScen;

	dlg.DoModal();
	
	SetHeatListItems();	
}



//  We are supposed to proceed with the update.

static const int STEP_INITIALIZE	= 0;
static const int STEP_CONNECT		= 1;
static const int STEP_910_DELETES	= 2;
static const int STEP_910S			= 3;
static const int STEP_CLOSE			= 4;

void CStringStatusDlg::OnOK() 
{
	// translate from action as a string to action as an enum constant
	//	possible string values are:
	//
	//		No change
	//		Delete
	//		910
	//		909
	//		Update

	{
		int rowNum = 1;

		for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
			  ir != m_changeRecords.end();
			  ++ir, ++rowNum ) {

			CString s = m_wndGrid.GetValueRowCol(rowNum,COL_ACTION);

			if ( s == "No change" )
				(*ir).m_action = UPLOAD_NO_CHANGE;
			else if ( s == "Delete" )
				(*ir).m_action = UPLOAD_DELETE;
			else if ( s == "910" )
				(*ir).m_action = UPLOAD_PROMOTE_910;
			else if ( s == "909" )
				(*ir).m_action = UPLOAD_PROMOTE_909;
			else if ( s == "Update" )
				(*ir).m_action = UPLOAD_MODIFY;
			else	// probably should be an error
				(*ir).m_action = UPLOAD_NO_CHANGE;
		}
	}


	//  Let's make sure we are not uploading anything with a fatal error.
	//  Or at least, let's make sure the user is responsible

	CUploadValidnErrorsDlg dlg;

	CCastStringValidator validator;

	bool fatalErrSeen = false;

	for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		  ir != m_changeRecords.end();
		  ++ir) {

		if ( (*ir).m_action == UPLOAD_PROMOTE_910
			 ||
			 (*ir).m_action == UPLOAD_MODIFY ) {

			validator.Validate((*ir).m_pScenString->Heats(),
							   (*ir).m_pScenString,
							   (*ir).m_pScenString->CasterScen(),
							   TheSnapshot.SuperScen());

			if ( (*ir).m_pScenString->ValidnErrors().size() > 0 )
				dlg.m_strings.push_back( (*ir).m_pScenString );

			for ( vector<CCastStringHeatValidnError>::iterator ie = (*ir).m_pScenString->ValidnErrors().begin();
				  ie != (*ir).m_pScenString->ValidnErrors().end();
				  ++ie ) {
				if ( (*ie).Severity() == CCastStringHeatValidnError::FATAL )
					fatalErrSeen = true;
			}
		}
	}

	if ( dlg.m_strings.size() > 0 ) {

		dlg.m_bFatalErr = fatalErrSeen;

		if ( dlg.DoModal() != IDOK  || fatalErrSeen )
			return;
	}

	

	int retCode = IDOK;


	CLongOpn* pOpn;

	pOpn = new CLongOpn;
	pOpn->m_strings.push_back("Initialize");
	pOpn->m_strings.push_back("Connect to database");
	pOpn->m_strings.push_back("Write 910 deletes");
	pOpn->m_strings.push_back("Write 910/909s");
	pOpn->m_strings.push_back("Close database");
	pOpn->m_title = "Uploading ... ";
	pOpn->Go();



	pOpn->SetCurrentStep(STEP_INITIALIZE);


			
	WriteUploadRecords(pOpn);


	pOpn->Done(0);

	int mbResult = MessageBox("Do you want to display the transport window?\n"
		"YES = display now\n"
		"NO = manually upload later\n",
		"Transport?",
		MB_YESNO);

	if ( IDYES == mbResult ) {

		// old version, using  CUploadDlg(Remotedl.OCX) directly
		//CUploadDlg uDlg;
		//uDlg.m_caster = m_pCasterScen->Caster();
		//
		//if ( IDOK != uDlg.DoModal() ) {

		//	retCode = IDCANCEL;
	
		//	if ( IDYES != MessageBox("You cancelled from the transport window.\n"
		//								"Do you still want yourself and other users updated with the 910 status?",
		//								"Update?",
		//								MB_YESNO) )
		//		goto finish;
		//}

		CUploadProcessMgr mgr;
		mgr.m_caster = m_pCasterScen->Caster();
		CUploadProcessMgr::E_UploadResult uploadCode = mgr.Invoke();

		switch (uploadCode) {
		case CUploadProcessMgr::CANCEL:
			retCode = IDCANCEL;

			if (IDYES != MessageBox("You cancelled from the transport window.\n"
				"Do you still want yourself and other users updated with the 910 status?",
				"Update?",
				MB_YESNO))
				goto finish;
			break;
		case CUploadProcessMgr::FAIL:
			MessageBox("Failed to create process to show transport window\nHit OK to continue.");
			return;
		}
	}
	
	//pOpn = new CLongOpn;
	//pOpn->strings.push_back("Fixing internals ... ");
	//pOpn->Go();
	//pOpn->SetCurrentStep(0);

	FixHeatStatuses();
	Archive910s();
	
	//FixSnapScen();
	//FixOtherScens();

	//TheSnapshot.UpdateDBs(m_pSnapScen);
	TheSnapshot.LoadNew910s();
	//pOpn->Done(0);
	


finish:

	EndDialog(retCode);
}



//  We have the following possible change record values:
//
//  (NEW) m_pSnapString == 0 :  then this record represents a new string
//  (DEL) m_pScenString == 0 :  then this record represents a string which has been deleted
//  
//  in the following cases, both are != 0
//
//  (909) m_snapStatus == 909
//  (910) m_snapStatus == 910
//
//  Here is what gets written out as upload records depending on the action:
//
//		State + Action -->	upload
//		-----   ------		------
//		 NEW	910			status 3
//		 NEW	909			status 1
//		 NEW	NOCHNG		don't write
//		 DEL	NOCHNG		if 909 then status 1 else status 5
//		 DEL	DELETE		don't write
//		 910	NOCHNG		status 5
//		 910	UPDATE		status 3
//		 910	DELETE		don't write
//		 909	910			status 3
//		 909	NOCHNG		status 1
//		 909	UPDATE		status 1
//		 909	DELETE		don't write


void CStringStatusDlg::WriteUploadRecords(CLongOpn* pOpn)
{
	pOpn->SetCurrentStep(STEP_CONNECT);

	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CUploadMgr uMgr(dbc.GetDB());

	uMgr.Initialize();


	//  First, any deletions of 910s need to be written out.
	//  This could be either a 910 which is actually be deleted,
	//		or one that is being modified.

	pOpn->SetCurrentStep(STEP_910_DELETES);


	{
		for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		      ir != m_changeRecords.end();
			  ++ir )

			if ( (*ir).m_pSnapString != 0
				 &&
				 (*ir).m_snapStatus == CCastString::STRING_STATUS_910
				 &&
				 ( (*ir).m_action == UPLOAD_DELETE
				   ||
				   (*ir).m_action == UPLOAD_MODIFY ) )

			(*ir).m_pSnapString->UploadDeleteRecords(uMgr);
	}


	pOpn->SetCurrentStep(STEP_910S);

	//  Now we write out existing 910s with status of 5
	//                   new 910s with a status of 3
	//                   existing or new 909s with a status of 1

	{
		for  ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		      ir != m_changeRecords.end();
			  ++ir ) {
					
			int status = -1;			// -1 == don't write
			CCastString* pString = 0;	//  0 == don't write

			if ( (*ir).m_pSnapString == 0 ) {
				// NEW
				pString = (*ir).m_pScenString;
				if ( (*ir).m_action == UPLOAD_PROMOTE_910 )
					status = 3;
				else if ( (*ir).m_action == UPLOAD_PROMOTE_909 )
					status = 1;
			}
			else if ( (*ir).m_pScenString == 0 ) {
				// DEL
				pString = (*ir).m_pSnapString;
				if ( (*ir).m_action == UPLOAD_NO_CHANGE ) {
					if ( (*ir).m_snapStatus == CCastString::STRING_STATUS_910 )
						status = 5;
					else
						status = 1;
				}
			}
			else if ( (*ir).m_snapStatus == CCastString::STRING_STATUS_910 ) {
				// 910
				if ( (*ir).m_action == UPLOAD_NO_CHANGE ) {
					pString = (*ir).m_pSnapString;
					status = 5;
				}
				else if ( (*ir).m_action == UPLOAD_MODIFY ) {
					pString = (*ir).m_pScenString;
					status = 3;
				}
			}
			else {
				// 909
				if ( (*ir).m_action == UPLOAD_PROMOTE_910 ) {
					pString = (*ir).m_pScenString;
					status = 3;
				}
				else if ( (*ir).m_action == UPLOAD_NO_CHANGE ) {
					pString = (*ir).m_pSnapString;
					status = 1;
				}
				else if ( (*ir).m_action == UPLOAD_MODIFY ) {
					pString = (*ir).m_pScenString;
					status = 1;
				}
			}
			

			//  Right now, to save time, we only write out the new 910s

			//if ( status != -1 )
			//	pString->Upload(uMgr,status);

			if ( status == 3 )
				pString->Upload(uMgr,status);
		
		}
	}

	pOpn->SetCurrentStep(STEP_CLOSE);

	uMgr.Finalize();
}




//  We must go through the new scenario and fix the heat status and reset marks
//     on some of the heats.
//



void CStringStatusDlg::FixHeatStatuses()
{


	for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		  ir != m_changeRecords.end();
		  ++ir ) {

		CCastString* pString = (*ir).m_pScenString;
		if ( pString == 0 )
			continue;



		for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
			  ih != pString->Heats().end();
			  ++ih ) {
			
			if ( (*ir).m_action == UPLOAD_PROMOTE_910 
				 ||
				 (*ir).m_pSnapString != 0 
				 &&
				 (*ir).m_snapStatus == CCastString::STRING_STATUS_910 
				 &&
				 (*ir).m_action == UPLOAD_MODIFY ) {

				if ( (*ih).IsMarked() )
					(*ih).Status( CCastStringHeat::HEAT_STATUS_910);
				else 
					(*ih).Status( CCastStringHeat::HEAT_STATUS_909);
			}
			else if ( (*ir).m_action == UPLOAD_PROMOTE_909 )
				(*ih).Status( CCastStringHeat::HEAT_STATUS_909);
			
			(*ih).IsMarked(false);
	
			pString->SetStatus();
		}
	}
}


void CStringStatusDlg::Archive910s()
{
	for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		  ir != m_changeRecords.end();
		  ++ir ) {

		CCastString* pString = (*ir).m_pScenString;
		if ( pString == 0 )
			continue;

			
		if ( (*ir).m_action == UPLOAD_PROMOTE_910 
			 ||
			 (*ir).m_snapStatus == CCastString::STRING_STATUS_910 
			 &&
			 (*ir).m_action == UPLOAD_MODIFY ) 

			CCasterScenArchiver::ArchiveStringToMasterScen(m_pCasterScen,pString);
	}
}
 



//  To fix the snapshot and edited scenarios to match what we have just done,
//	for each change record make the appropriate correction
//
// 	State + Action -->	change to snapshot scenario, change to edited scenario
//	-----   ------		---------------------------  -------------------------
//	 NEW	910			add a copy of m_pScenString  change OrigString of m_pScenString
//	 NEW	909			add a copy of m_pScenString  change OrigString of m_pScenString
//	 NEW	NOCHNG		do nothing                   do nothing
//	 DEL	NOCHNG		do nothing                   add a copy of m_pSnapString
//	 DEL	DELETE		delete                       do nothing
//	 910	NOCHNG		do nothing;                  if m_pScenString is different, replace it
//	 910	UPDATE		replace                      change OrigString of m_pScenString
//	 910	DELETE		delete                       delete
//	 909	910			replace                      change OrigString of m_pScenString
//	 909	NOCHNG		do nothing                   if m_pScenString is different, replace it
//	 909	UPDATE		replace                      change OrigString of m_pScenString
//	 909	DELETE		delete                       delete
   

void CStringStatusDlg::FixSnapScen()
{
	for ( vector<CSnapScenChangeRecord>::iterator ir = m_changeRecords.begin();
		  ir != m_changeRecords.end();
		  ++ir ) {

		if ( (*ir).m_pSnapString == 0 ) {
	
			// NEW
			if ( (*ir).m_action == UPLOAD_PROMOTE_910 || (*ir).m_action == UPLOAD_PROMOTE_909 ) {
		
				CCastString* pNewString = new CCastString( m_pSnapScen, *((*ir).m_pScenString), 0 );
				m_pSnapScen->CastStrings().push_back(pNewString);
				(*ir).m_pScenString->OrigString( pNewString);

			}

		}
		else if ( (*ir).m_pScenString == 0 ) {

			// DEL
			if ( (*ir).m_action == UPLOAD_DELETE ) {

				m_pSnapScen->DeleteString((*ir).m_pSnapString);
	
			}
			else {

				CCastString* pNewString = new CCastString( m_pCasterScen, *((*ir).m_pSnapString), (*ir).m_pSnapString );
				m_pCasterScen->CastStrings().push_back(pNewString);

			}
		}
		else if ( (*ir).m_snapStatus == CCastString::STRING_STATUS_910 ) {
	
			// 910 or 909
			if ( (*ir).m_action == UPLOAD_NO_CHANGE ) {
				if ( ! (*ir).m_bEqual ) {

					m_pCasterScen->DeleteString((*ir).m_pScenString);
					CCastString* pNewString = new CCastString( m_pCasterScen, *((*ir).m_pSnapString), (*ir).m_pSnapString );
					m_pCasterScen->CastStrings().push_back(pNewString);

				}
			}
			else if ( (*ir).m_action == UPLOAD_MODIFY || (*ir).m_action == UPLOAD_PROMOTE_910 ) {

				m_pSnapScen->DeleteString((*ir).m_pSnapString);
				CCastString* pNewString = new CCastString( m_pSnapScen, *((*ir).m_pScenString), 0 );
				m_pSnapScen->CastStrings().push_back(pNewString);
				(*ir).m_pScenString->OrigString( pNewString);

			}
			else if ( (*ir).m_action == UPLOAD_DELETE ) {

				m_pSnapScen->DeleteString((*ir).m_pSnapString);
				m_pCasterScen->DeleteString((*ir).m_pScenString);

			}
		}
	}

	sort( m_pSnapScen->CastStrings().begin(),
		  m_pSnapScen->CastStrings().end(),
		  less_p< CCastString*> () );

	sort( m_pCasterScen->CastStrings().begin(),
		  m_pCasterScen->CastStrings().end(),
		  less_p< CCastString*> () );


}




//  We iterate through all scenarios other than the one currently being edited.
//
//  We compare each scenario to the snapshot scenario.
//
//  For each string in the scenario which has an origString which is no longer in the snapshot,
//     delete it.
//
//  For each string in the snapshot not in the scenario
//		add it (watch for duplicate string ids)
//
//


void CStringStatusDlg::FixOtherScens()
{
	int caster = m_pCasterScen->Caster();


	for ( vector<CCasterScen*>::iterator is = CScenMgr::CasterScensBegin( caster );
		  is !=CScenMgr::CasterScensEnd( caster );
		  ++is ) {

		if ( (*is) == m_pCasterScen )
			// already updated this scenario
			continue;

		FixOtherScen( (*is) );
	}
}


void CStringStatusDlg::FixOtherScen( CCasterScen* pScen )
{
	vector<CCastString*> toDelete;

	{
		for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
			  is != pScen->CastStrings().end();
			  ++is ) {

			vector<CCastString*>::iterator ifind 
				= find(m_pSnapScen->CastStrings().begin(), 
					   m_pSnapScen->CastStrings().end(),
					   (*is)->OrigString() );

			if ( (*is)->OrigString() != 0
				 &&
				 ifind == m_pSnapScen->CastStrings().end() )

				// the string from the snapshot that this was based on has died
				// get rid of it
				toDelete.push_back( (*is) );
		}
	}

	{
		for ( vector<CCastString*>::iterator is = toDelete.begin();
			  is != toDelete.end();
			  ++is ) 
			pScen->DeleteString( (*is) );
	}

	{
		for ( vector<CCastString*>::iterator is = m_pSnapScen->CastStrings().begin();
			  is != m_pSnapScen->CastStrings().end();
			  ++is ) {

			vector<CCastString*>::iterator ic; 
			vector<CCastString*>::iterator id = pScen->CastStrings().end();

			for ( ic = pScen->CastStrings().begin();
				  ic != pScen->CastStrings().end();
				  ++ic ) {

				if ( (*ic)->OrigString() == (*is) )
					break;

				if ( (*ic)->Id() == (*is)->Id() )
					id = ic;

			}

			if ( ic == pScen->CastStrings().end() ) {
				// this snapshot string does not exist 
				// create it.  Watch out for duplicates.

				CCastString* pNewString = new CCastString( pScen, (**is), (*is) );
				pScen->CastStrings().push_back(pNewString);

				if ( id != pScen->CastStrings().end() ) {
					// duplicate string id
					// TODO --
				}
			}
		}
	}
		
	sort( pScen->CastStrings().begin(),
		  pScen->CastStrings().end(),
		  less_p< CCastString* >() );
}

			