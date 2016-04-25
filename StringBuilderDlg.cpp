// StringBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "StringBuilderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "CasterScen.h"
#include "CastStringIdDlg.h"
#include "PSheetAvailOrders.h"
#include "CastStringValidator.h"
#include "SSBOutput.h"
#include "DBConn.h"
#include "ScenMru.h"
#include "UserInfo.h"


/////////////////////////////////////////////////////////////////////////////
//	CStringBuilderDlg dialog
//
//	Allows the user to pick a string id & heat size,
//    and select orders,
//  to send to the string builder.
//  when the string builder returns, user can create the string selected 
//    in the string builder
//
//	Usage:
//
//		// there is a lot of redundant information in this.
//		// the minimum data that is needed would be the superScen and a caster#.
//		// sigh.
//
//		CStringBuilderDlg sbd;
//		sbd.m_casterScen[1] = GetViewport()->CurrentCasterScen(1);
//		sbd.m_casterScen[2] = GetViewport()->CurrentCasterScen(2);
//		sbd.m_casterScen[3] = GetViewport()->CurrentCasterScen(3);
//		sbd.m_pScen = pScen;
//		sbd.m_caster = GetViewport()->CasterNum();
//		sbd.m_id = CCastStringId(0,0,pScen->Caster(),0);
//		sbd.m_pSuperScen = GetViewport()->SuperScen();
//
//		sbd.DoModal();
//
/////////////////////////////////////////////////////////////////////////////


CStringBuilderDlg::CStringBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringBuilderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStringBuilderDlg)
	m_strId = _T("");
	m_numOrders = 0;
	//}}AFX_DATA_INIT

	m_casterScen[0] = 0;
	m_casterScen[1] = 0;
	m_casterScen[2] = 0;
	m_casterScen[3] = 0;
	m_pScen = 0;
	m_caster = 1;
	m_pSuperScen = 0;
	m_pOrderSelection = 0;
}

CStringBuilderDlg::~CStringBuilderDlg()
{
	delete m_pOrderSelection;
}



void CStringBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringBuilderDlg)
	DDX_Control(pDX, IDC_BUTTON_CREATE_STRING, m_btnCreateString);
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	DDX_Text(pDX, IDC_EDIT_NUM_ORDERS, m_numOrders);
	//  DDX_Control(pDX, IDC_SBDRIVER1, m_sbDriver);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringBuilderDlg, CDialog)
	//{{AFX_MSG_MAP(CStringBuilderDlg)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_STRING, OnButtonCreateString)
	ON_BN_CLICKED(IDC_BUTTON_ID, OnButtonId)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_START_SB, OnButtonStartSb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderDlg message handlers

BEGIN_EVENTSINK_MAP(CStringBuilderDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CStringBuilderDlg)
//	ON_EVENT(CStringBuilderDlg, IDC_SBDRIVER1, 1 /* Cancel */, OnCancelSbdriver1, VTS_NONE)
//	ON_EVENT(CStringBuilderDlg, IDC_SBDRIVER1, 2 /* Done */, OnDoneSbdriver1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



BOOL CStringBuilderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// we keep our own orderSelection object.
	m_pOrderSelection = new COrderSelection(m_caster);

	//  The creatString button is visible only after a successful run of the stringbuilder
	m_btnCreateString.ShowWindow(SW_HIDE);

	SetControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

 
//
//  We display the currently selected caststring id
//     and the number of selected orders.
//

void CStringBuilderDlg::SetControls()
{
	ostrstream ostr;
	ostr << m_id.Caster() << " " << m_id.Year() << " " << m_id.Week() << " " << m_id.StringId() << ends;
	m_strId = ostr.str();
	ostr.seekp(0);

	//## cast to int
	m_numOrders = (int) m_pOrderSelection->Orders().size();

	UpdateData(FALSE);
}


//
//  Callback from the embedded ocx that starts the string builder.
//  If the stringbuilder is cancelled, we exit.
//

//void CStringBuilderDlg::OnCancelSbdriver1() 
//{
//	OnOK();	
//}


//
//  Callback from the embedded ocs that starts the string builder
//  If the stringbuilder returns successfully, we can now show
//    the createString button
//

//void CStringBuilderDlg::OnDoneSbdriver1() 
//{
//	m_btnCreateString.ShowWindow(SW_SHOW);
//}


//
//  The user wants to create a string based on the output of the string builder
//

void CStringBuilderDlg::OnButtonCreateString() 
{
	// Here we go:

	vector<CCastString*> strings;

	CDBConn dbc(CDBConn::DB_DPA);
	dbc.Connect();

	CSSBOutput sbSet(dbc.GetDB());

	try
	{
		//		sbSet.Open(CRecordset::forwardOnly,       Access original code comm out for SQL Conv. 3-7-03 k. hubbard 
		//				   LPCTSTR(OutputTableName()),  
		//						 CRecordset::readOnly);

		//sbSet.Open(CRecordset::dynamic,

		sbSet.m_userIdParam = atol(CUserInfo::TheUser.SBId());

		sbSet.Open(CRecordset::dynaset,
			LPCTSTR(NewOutputTableName()),
			CRecordset::none);
		m_pScen->LoadFromNewStringBuilderOutput(sbSet, strings);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError,
			MB_ICONEXCLAMATION);
		e->Delete();
	}

	// inform the user
	{
		ostrstream ostr;
		ostr << "New string(s) created on caster # " << m_caster << ":\n";
		for (vector<CCastString*>::iterator is = strings.begin();
			is != strings.end();
			++is)
			ostr << "-- " << (*is)->Id().Year()
			<< " - " << (*is)->Id().Week()
			<< " - " << (*is)->Id().StringId()
			<< "\n";
		ostr << ends;
		MessageBox(ostr.str());
		ostr.freeze(false);
	}


	// give each new string a copy of the initiating COrderSelection
	{
		for (vector<CCastString*>::iterator is = strings.begin();
			is != strings.end();
			++is)
			(*is)->OrderSelection() = *m_pOrderSelection;	// copy
	}

	// Maybe promote the new string?
	if (strings.size() > 0)
		CScenMru::GlobalScenMru.SetCurrentCastString(m_caster, strings.front());


	OnOK();

	// Old VB version

//	vector<CCastString*> strings;
//
//	CDBConn dbc(CDBConn::DB_DPA);
//	dbc.Connect();
//
//	CStringBuilderOutputSet sbSet(dbc.GetDB());
//
//	try
//	{
////		sbSet.Open(CRecordset::forwardOnly,       Access original code comm out for SQL Conv. 3-7-03 k. hubbard 
////				   LPCTSTR(OutputTableName()),  
////						 CRecordset::readOnly);
//
//		//sbSet.Open(CRecordset::dynamic,
//
//		sbSet.Open(CRecordset::dynaset,
//				   LPCTSTR(OutputTableName()),
//						 CRecordset::none);
//		m_pScen->LoadFromStringBuilderOutput(sbSet,strings);
//	}
//	catch ( CDBException* e )
//	{
//	   AfxMessageBox( e->m_strError,   
//                      MB_ICONEXCLAMATION );
//	   e->Delete();
//	}
//
//	// inform the user
//	{
//		ostrstream ostr;
//		ostr << "New string(s) created on caster # " << m_caster << ":\n";
//		for ( vector<CCastString*>::iterator is = strings.begin();
//		  	  is != strings.end();
//			  ++is ) 
//			ostr << "-- " << (*is)->Id().Year()
//			<< " - " << (*is)->Id().Week()
//			<< " - " << (*is)->Id().StringId()
//			<< "\n";
//		ostr << ends;
//		MessageBox(ostr.str());
//		ostr.freeze(false);
//	}
//
//	
//	// give each new string a copy of the initiating COrderSelection
//	{
//		for ( vector<CCastString*>::iterator is = strings.begin();
//		  	  is != strings.end();
//			  ++is ) 
//			(*is)->OrderSelection() = *m_pOrderSelection;	// copy
//	}
//
//	// Maybe promote the new string?
//	if ( strings.size() > 0 )
//		CScenMru::GlobalScenMru.SetCurrentCastString(m_caster,strings.front());
//
//	
//	OnOK();
}



//
//  Allow the user to select a cast string id to use
//

void CStringBuilderDlg::OnButtonId() 
{
	vector<CCastStringId> usedIds[4];

	m_casterScen[1]->GetLineupIds(usedIds[1]);
	m_casterScen[2]->GetLineupIds(usedIds[2]);
	m_casterScen[3]->GetLineupIds(usedIds[3]);

	CCastStringMiscProps props(1,m_pScen->Caster());

	CCastStringIdDlg dlg;
	CCastStringId id = m_id;

	dlg.m_pId				= &id;
	dlg.m_usedIds[1]		= usedIds[1];  // copy
	dlg.m_usedIds[2]		= usedIds[2];  // copy
	dlg.m_usedIds[3]		= usedIds[3];  // copy
	dlg.m_pProps			= &props;
	dlg.m_isEditing		= true;	// prevent change of caster
	dlg.m_setInitialId	= true;

	if ( dlg.DoModal() != IDOK )
		return;

	m_heatSizeAim = props.AimHeatSize();
	m_id = id;
	SetControls();

}


//
//  Bring up the orderSelection dialog 
//
//  We edit a copy here, and replace if the dialog returns ok.
//

void CStringBuilderDlg::OnButtonSelect() 
{

	COrderSelection osel = *m_pOrderSelection;	//copy

	CPSheetAvailOrders dlgo("Select orders for string builder",this);

///////////// DM CHANGE: 2002-09-14: added superscen arg
	dlgo.Init(&osel,m_pSuperScen,0);


//	dlgo.Init(&osel,0);  see 2 lines above, so comment this line out 2-7-03 k. hubbard

	if ( dlgo.DoModal() != IDOK ) 
		return;

	delete m_pOrderSelection;
	m_pOrderSelection = new COrderSelection(osel);
	m_pOrderSelection->Init(m_pSuperScen);

	SetControls();
	
}


//
//  Start the string builder.
//  THis involves writing several tables with order information.
//

void CStringBuilderDlg::OnButtonStartSb() 
{
	{
		CWaitCursor wc;
		m_pOrderSelection->WriteOrdersForNewStringBuilder(m_pSuperScen, atoi(CUserInfo::TheUser.SBId()));

		CDBConn dbc(CDBConn::DB_DPA);
		dbc.Connect();

		ClearNewOutputTable(dbc.GetDB(), atoi(CUserInfo::TheUser.SBId()));
	}

	ostrstream ostr;
	ostr << ".\\StringBuilder\\StringBuilder.exe ";
	ostr << " aimheatsize " << m_heatSizeAim;
	ostr << " dateformat PST";
	ostr << " caster " << m_id.Caster();
	ostr << " strinstringidgId " << m_id.StringId();
	ostr << " planwk " << m_id.Year() * 100 + m_id.Week();
	ostr << " userid " << atoi(CUserInfo::TheUser.SBId());
	if (m_pOrderSelection->Orders().size() > 0)
		ostr << " startspec " << m_pOrderSelection->Orders().front()->OrderedSpec();
	ostr << " maxnumtries 10"; 
	ostr << " dbname  ihe-SQL03p"; // TODO: This should be detected or come from a config file
	ostr << ends;

	//CString curDir; 
	//GetCurrentDirectory(MAX_PATH, curDir.GetBufferSetLength(MAX_PATH)); 
	//curDir.ReleaseBuffer();

	//OutputDebugString("*******************************");
	//OutputDebugString("*******************************");
	//OutputDebugString(ostr.str());
	//OutputDebugString(curDir);
	//OutputDebugString("*******************************");
	//OutputDebugString("*******************************");

	try {
		int val = system(ostr.str());
		CString strVal;
		strVal.Format(_T("VAL = %d"), val);
		OutputDebugString(strVal);
	}
	catch (...)
	{
		OutputDebugString("Got an error");
	}

	// Code from the original VB version.
	//{
	//	CWaitCursor wc;
	//	m_pOrderSelection->WriteOrdersForStringBuilder(m_pSuperScen);

	//	CDBConn dbc(CDBConn::DB_DPA);
	//	dbc.Connect();

	//	CreateOutputTable(dbc.GetDB());
	//	ClearOutputTable(dbc.GetDB());
	//}

	//CStringBuilderDlg sbd;

	//m_sbDriver.SetAimHeatWeight( m_heatSizeAim );
	//m_sbDriver.SetDateformat("PST");

	//{
	//	ostrstream ostr;
	//	ostr << m_id.Caster() << ends;
	//	m_sbDriver.SetPlanCasterUnit( ostr.str() );
	//	ostr.freeze(false);
	//	ostr.seekp(0);

	//	ostr << m_id.StringId() << ends;
	//	m_sbDriver.SetStringID( ostr.str() );
	//	ostr.freeze(false);
	//	ostr.seekp(0);

	//	ostr << m_id.Year()*100 + m_id.Week() << ends;
	//	m_sbDriver.SetPlanningWeek( ostr.str() );
	//	ostr.freeze(false);
	//}

	//CString dummy;

	//if ( m_pOrderSelection->Orders().size() == 0 )
	//	dummy = "";
	//else {
	//	dummy = m_pOrderSelection->Orders().front()->OrderedSpec();
	//}
	// 
	//m_sbDriver.SetStartingSpec(dummy);

	//m_sbDriver.SetMaxNumberOfTries( 10 );
	//m_sbDriver.SetDNSdata( "dpaTest" );
	//m_sbDriver.SetDNSrules( "rules" );
	//m_sbDriver.SetDsnforBOFSpeedCalc( "DSN=csdaTest" );
	//m_sbDriver.SetOrderSource( LPCTSTR(InputTableName()) );
	//m_sbDriver.SetOutputTable( LPCTSTR(OutputTableName()) );
	//
	//m_sbDriver.MakeString();
}


//
// The table names used for input to and output from the string builder
//   are StringBuilderInput N
//   and StringBuilderOutput N
//
// where N is a number associated with the user.
//
// The number N comes from the Users table in csdaTest.
//
// This is used to prevent overlaps when two planners are using
//   the stringbuilder at the same time.
//

//// static
//const CString& CStringBuilderDlg::BaseInputTableName()
//{
//	static const CString name("StringBuilderInput");
//	return name;
//}
//
//// static
//const CString& CStringBuilderDlg::BaseOutputTableName()
//{
//	static const CString name("StringBuilderOutput");
//	return name;
//}

// static
const CString& CStringBuilderDlg::BaseNewInputTableName()
{
	static const CString name("SSBInput");
	return name;
}

// static
const CString& CStringBuilderDlg::BaseNewOutputTableName()
{
	static const CString name("SSBOutput");
	return name;
}

////	static 
//CString CStringBuilderDlg::InputTableName()
//{
//	return BaseInputTableName() + CUserInfo::TheUser.SBId();
//}
//
//
////	static 
//CString CStringBuilderDlg::OutputTableName()
//{
//	return BaseOutputTableName() + CUserInfo::TheUser.SBId();
//}

//	static 
CString CStringBuilderDlg::NewInputTableName()
{
	return BaseNewInputTableName();
}


//	static 
CString CStringBuilderDlg::NewOutputTableName()
{
	return BaseNewOutputTableName();
}



//
//  We have to make sure the tables actually exists
//
//  We do so by copying from the tables StringBuilderInput and StringBuilderOutput
//    which are known to exist.
//

////	static 
//void CStringBuilderDlg::CreateInputTable(CDatabase* pDB)
//{
//	CreateCopyTable(pDB,InputTableName(),BaseInputTableName());
//}

////	static 
//void CStringBuilderDlg::ClearInputTable(CDatabase* pDB)
//{
//	ClearTable(pDB,InputTableName());
//}

//	static 
void CStringBuilderDlg::ClearNewInputTable(CDatabase* pDB, int userId)
{
	ClearNewTable(pDB, NewInputTableName(), userId);
}

////	static 
//void CStringBuilderDlg::CreateOutputTable(CDatabase* pDB)
//{
//	CreateCopyTable(pDB,OutputTableName(),BaseOutputTableName());
//}


//// static 
//void CStringBuilderDlg::ClearOutputTable(CDatabase* pDB)
//{
//	ClearTable(pDB,OutputTableName());
//}


// static 
void CStringBuilderDlg::ClearNewOutputTable(CDatabase* pDB, int userId)
{
	ClearNewTable(pDB, NewOutputTableName(), userId);
}

//// static 
//void CStringBuilderDlg::CreateCopyTable(CDatabase* pDB,
//										const CString& copyName,
//										const CString& srcName)
//{
//
//
//
//	try {
//		CString sql = "DROP TABLE [";
//		sql += copyName;
//		sql += "]";
//
//		pDB->ExecuteSQL(sql);
//
//	} catch ( CDBException* e )	{
//		// usually the table already exists, show error message only if not
//		if ( e->m_strError.Find("already exists") == -1 )
//			AfxMessageBox( e->m_strError,   
//					       MB_ICONEXCLAMATION );
//	   e->Delete();
//	}
//
//	try {
//		CString sql = "SELECT * INTO [";
//		sql += copyName;
//		sql += "] FROM [";
//		sql += srcName;
//		sql += "]";
////if ("StringBuilderOutput") 
////{
////	sql += " ORDER BY [";
////	sql +=	srcName;
////	sql	+= ".PLAN_HEAT_SEQUENCE_NO]";
////}
//
////  sql += srcName ".PLAN_HEAT_SEQUENCE_NO]";  //;
////  sql += ".PLAN_HEAT_SEQUENCE_NO]";
//
//		pDB->ExecuteSQL(sql);
//
//	} catch ( CDBException* e )	{
//		// usually the table already exists, show error message only if not
//		if ( e->m_strError.Find("already exists") == -1 )
//			AfxMessageBox( e->m_strError,   
//					       MB_ICONEXCLAMATION );
//	   e->Delete();
//	}
//}
//

//// static 
//void CStringBuilderDlg::ClearTable(CDatabase* pDB ,const CString& tableName)
//{
//	CString sql = "DELETE FROM [";
//	sql += tableName;
//	sql += "]";
//
//	try {
//
//		pDB->ExecuteSQL(sql);
//
//	} catch ( CDBException* e )	{
//	   AfxMessageBox( e->m_strError,   
//                    MB_ICONEXCLAMATION );
//	   e->Delete();
//	}
//}

// static 
void CStringBuilderDlg::ClearNewTable(CDatabase* pDB, const CString& tableName, int userId)
{
	//CString sql = "DELETE FROM [";
	//sql += tableName;
	//sql += "]";
	//sql += " WHERE [USER_ID] = ";
	//sql += itoa(userId);

	//CString sql;
	//sql.Format(_T("DELETE FROM [%s] where [USER_ID] = %d", tableName, userId));

	CString sql = "DELETE FROM [";
	sql += tableName;
	sql += "]";

	CString strUserId;
	strUserId.Format(_T("%d"), userId);
	sql += " WHERE [USER_ID] = ";
	sql += strUserId;

	try {

		pDB->ExecuteSQL(sql);

	}
	catch (CDBException* e)	{
		AfxMessageBox(e->m_strError,
			MB_ICONEXCLAMATION);
		e->Delete();
	}
}
