// CastStringEditorMvc.cpp: implementation of the CCastStringEditorGanttViewport class.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tip Notes: Alt key & closing bracket <> inside open { bracket will automatically take you to the matching closing } bracket.
//
//  Function   :  This dialog (GUI) program displays the editor by cast strands, sort viewing criteria, and order detail parameters. 
//                  
//	Maintenance:
//
//  K. Hubbard  02-24-09  Modified Select Groups window Sort_Width to display widths by half inch amounts and decimals within their 
//                        sort groupings. This will display regardless of significant digits behind the decimal point or not. 
//                        For example, decimal value can be > or = zero and still dispay decimal value.   
//
//  K. Hubbard  04-27-10  Increased title spacing to ensure full field data display, eliminating mandatory resizing of column. 
//
//	K. Hubbard  04-27-10  Added Hotroll Unitname column to Strand 1 & 2 detail lot windows. Also, increased Strand 1 & 2 List control 
//                        listCtrl.SetItemText column position counters greater than position 3, by 1 to make room for new Hotroll unitname 
//                        column. 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
#include "stdafx.h"
#include "csda.h"
#include "CastStringEditorMvc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ScenMru.h"
#include "CastString.h"
#include "CastStringHeat.h"
#include "CastStringValidator.h"
#include "CastStringHeatValidnError.h"
#include "MaintainListCtrlPosn.h"
#include "CastStringEditorStrandSummaryView.h"
#include "CastStringEditorScenMgrView.h"
#include "CastStringEditorTextView.h"
#include "CastStringEditorGanttBars.h"
#include "GanttColorChartDlg.h"
#include "CasterScen.h"
#include "ScenMgr.h"
#include "CasterScenArchiver.h"
#include "CastStringIdDlg.h"
#include "CasterScenInfoDlg.h"
#include "PSheetAvailOrders.h"
#include "DragSourceButton.h"
#include "NStateBitmapButton.h"
#include "ListCtrlEx.h"
#include "NTime.h"
#include "OrderInquiryDlg.h"
#include "HeatDetailsDlg.h"
#include "HeatLockDlg.h"
#include "StringStatusDlg.h"
#include "StringBuilderDlg.h"
#include "PickCasterDlg.h"
#include "PstConformanceReport.h"
#include "CastStringEditorDoc.h"
#include "PlanWorksheetReport.h"
#include "Caster.h"



// I don't know where to put these
// they gets called by more than one scenario.
// should be in the model, of course.


static void ValidateOrderSelection()
{
////////////  DM CHANGE: 2002-09-15:  not updating sufficiently, in particular, sometimes IsModified is not set,
//////////// for example, when another string is deleted, or we have edited another string and moved to this one.

//	if ( CScenMru::GlobalScenMru.CurrentCastString() != 0 
//		 &&
//		 CScenMru::GlobalScenMru.CurrentCastString()->OrderSelection().IsModified() )
//		 
//		 CScenMru::GlobalScenMru.CurrentCastString()->OrderSelection().Init(CScenMru::GlobalScenMru.CurrentSuperScen());

	if ( CScenMru::GlobalScenMru.CurrentCastString() != 0  )
		 CScenMru::GlobalScenMru.CurrentCastString()->OrderSelection().Init(CScenMru::GlobalScenMru.CurrentSuperScen());

}


static void CreateCastString(CSuperScen* pSS, int casterNum) 
{
	if ( pSS == 0 )
		return;

	vector<CCastStringId> usedIds[Caster::CasterArrayLen];

	for (int i = Caster::FirstCaster; i <= Caster::LastCaster; i++)
		pSS->CasterScen(i)->GetLineupIds(usedIds[i]);

	CCastStringId id(0,0,casterNum,0);
	CCastStringMiscProps props(1,casterNum);

	CCastStringIdDlg dlg;

	dlg.m_pId				= &id;
	for (int i = Caster::FirstCaster; i <= Caster::LastCaster; i++)
		dlg.m_usedIds[i] = usedIds[i];  // copy
	dlg.m_pProps			= &props;
	dlg.m_isEditing		= false;
	dlg.m_setInitialId	= true;

	if ( dlg.DoModal() == IDOK ) {

		if ( casterNum != id.Caster() ) {

			// Stupid user changed to a different caster
			// Make sure we can change!
			if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
				return;

			// it's okay to change. let's change.
			CScenMru::GlobalScenMru.SetCasterNum(id.Caster());
			casterNum = id.Caster();
			ValidateOrderSelection();
		}

		CCasterScen* pScen = pSS->CasterScen(casterNum);
		CCastString* pNewString = pScen->CreateNewString(id,props);
		CScenMru::GlobalScenMru.SetCurrentCastString(pNewString);
	}
}


static void ShowCastStringInfo(CCasterScen* pScen, CCastString* pString)
{
	// determine all string ids in use,
	// with the exception of the string being edited.

	vector<CCastStringId> usedIds;

	pScen->GetLineupIds(usedIds,&pString->Id());

	CCastStringIdDlg dlg;

	CCastStringId id = pString->GetId();
	CCastStringMiscProps props = pString->GetProps();

	dlg.m_pId						= &id;
	dlg.m_usedIds[pScen->Caster()]	= usedIds;  // copy
	dlg.m_pProps					= &props;	// non-const accessor
	dlg.m_isEditing				= true;
	dlg.m_setInitialId			= false;


	if ( dlg.DoModal() == IDOK ) {

		if ( id != pString->Id() ) {
			//	This indirects through the casterScen
			//		because the sort order might change.
			//  This is why we need to reselect the string: CScenMru is index-based.
			pScen->ModifyCastStringId(pString,id);
			CScenMru::GlobalScenMru.SetCurrentCastString(pString);
		}
		
		if ( props != pString->GetProps() )
			pString->ModifyMiscProps(props);

		pScen->SetModified(true);

		CCastStringEditorTextViewport::CStringInfoChangedMsg msg(pString);
		TheBus().UpdateAllObservers(0,&msg);
	}	
}



//////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttViewport Class
//////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
//   Basic viewport functionality
//
////////////////////////////////////////////////////////////


CMessageBus* CCastStringEditorGanttViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(m_pModel);
}


BOOL CCastStringEditorGanttViewport::OnCreate()
{
	return TRUE;
}


BOOL CCastStringEditorGanttViewport::CreateController()
{
	m_pCtlr =  new CCastStringEditorGanttController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


//
// Drawing is deferred to the gantt display
//

void CCastStringEditorGanttViewport::Draw(CDC* pDC)
{
	m_pGanttDisplay->Chart()->OnDraw(pDC);
}





void CCastStringEditorGanttViewport::OnInitialUpdate()
{
	assert( m_pGanttDisplay != 0 );
	assert( m_pView != 0 );

	m_pGanttDisplay->SetMargins(5,5,5,5);


	static vector<COLORREF> dayColors;
	static vector<COLORREF> hourColors;
	static COLORREF dayTextColor = RGB(0xFF,0xFF,0xFF);

	if ( dayColors.size() == 0 ) {
		dayColors.push_back(RGB(0x00,0x00,0x50));
		dayColors.push_back(RGB(0x00,0x00,0xA0));

		hourColors.push_back(RGB(0x10,0x10,0x20));
		hourColors.push_back(RGB(0x18,0x18,0x30));
		hourColors.push_back(RGB(0x20,0x20,0x40));
		hourColors.push_back(RGB(0x28,0x28,0x50));
		hourColors.push_back(RGB(0x30,0x30,0x60));
		hourColors.push_back(RGB(0x38,0x38,0x70));
		hourColors.push_back(RGB(0x40,0x40,0x80));
		hourColors.push_back(RGB(0x48,0x48,0x90));
		hourColors.push_back(RGB(0x50,0x50,0xA0));
		hourColors.push_back(RGB(0x58,0x58,0xB0));
		hourColors.push_back(RGB(0x60,0x60,0xC0));
		hourColors.push_back(RGB(0x68,0x68,0xD0));
		hourColors.push_back(RGB(0x70,0x70,0xE0));
		hourColors.push_back(RGB(0x68,0x68,0xD0));
		hourColors.push_back(RGB(0x60,0x60,0xC0));
		hourColors.push_back(RGB(0x58,0x58,0xB0));
		hourColors.push_back(RGB(0x50,0x50,0xA0));
		hourColors.push_back(RGB(0x48,0x48,0x90));
		hourColors.push_back(RGB(0x40,0x40,0x80));
		hourColors.push_back(RGB(0x38,0x38,0x70));
		hourColors.push_back(RGB(0x30,0x30,0x60));
		hourColors.push_back(RGB(0x28,0x28,0x50));
		hourColors.push_back(RGB(0x20,0x20,0x40));
		hourColors.push_back(RGB(0x18,0x18,0x30));
	}

	CGanttDisplayBar* pDay  = new CGanttDisplayRelDay(10,0,dayColors,dayTextColor);
	CGanttDisplayBar* pHour = new CGanttDisplayRelHour(10,0,hourColors,dayTextColor);
	m_pHeatBar				= new CCastStringEditorGanttHeatBar(10,5,this);
	m_pStrand1WidthGraph	= new CCastStringEditorGanttStrandWidthGraph(61,5,20,80,true,1,this);
	m_pStrand2WidthGraph	= new CCastStringEditorGanttStrandWidthGraph(61,5,20,80,true,2,this);

	pDay->AddLabel("Day");
	pHour->AddLabel("Hour");

	m_pHeatBar->AddLabel("Heat");

	m_pStrand1WidthGraph->AddLabel("20",20.0);
	m_pStrand1WidthGraph->AddLabel("40",40.0);
	m_pStrand1WidthGraph->AddLabel("60",60.0);
	m_pStrand1WidthGraph->AddLabel("80",80.0);

	m_pStrand2WidthGraph->AddLabel("20",20.0);
	m_pStrand2WidthGraph->AddLabel("40",40.0);
	m_pStrand2WidthGraph->AddLabel("60",60.0);
	m_pStrand2WidthGraph->AddLabel("80",80.0);
	
	vector<CGanttDisplayBar*> bars;

	bars.push_back(pDay);
	bars.push_back(pHour);
	bars.push_back(m_pStrand1WidthGraph);
	bars.push_back(m_pHeatBar);
	bars.push_back(m_pStrand2WidthGraph);

	m_pGanttDisplay->AddBars(bars);

	m_pGanttDisplay->SetPixelsPerXUnit(60.0/3600.0);
	m_pGanttDisplay->SetXLimits(0,4*3600);         // 4*3600 = 4 hours
}




void CCastStringEditorGanttViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {
		
	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_MRU_PROMOTE_PRODN_SCEN:
	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_CSE_ACTIVATED:
	case MSG_ID_CSE_ORDERS_DELETED:
	case MSG_ID_CSE_ORDERS_INSERTED:
	case MSG_ID_CSE_LOCKED_HEATS_STATUS_CHANGED:
	case MSG_ID_CSE_ORDERS_MOVED:
		ComputeLimits();
		Invalidate();
		break;

	default:
		// do nothing
		break;
	}
}


///////////////////////////////////////////////////////
//
//  Miscellaneous drawing details
//
///////////////////////////////////////////////////////

void CCastStringEditorGanttViewport::ComputeLimits()
{
	long totalDuration = 0;

	TCastStringVec& strings = CScenMru::GlobalScenMru.CurrentCasterScen()->CastStrings();
	CCastString* pSelString = CScenMru::GlobalScenMru.CurrentCastString();
	
	if ( pSelString == 0 )

		totalDuration = 0;

	else

		totalDuration = pSelString->TotalDuration();

	// go to the nearest day in duration

	long totalDays = 1+max(1,ceil(totalDuration/3600/24));

	totalDuration = totalDays*3600*24;
	
	m_pGanttDisplay->SetXLimits(0,totalDuration);
}




//////////////////////////////////////////////////////////////////////
// CCastStringEditorGanttController Class
//////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CCastStringEditorGanttController, MvcController)
	//{{AFX_MSG_MAP(CCastStringEditorGanttController)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CCastStringEditorGanttController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}



//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorScenMgrViewport
//
//  Manages display for CCastStringEditorScenMgrView
//
//  list-style combo boxes for each caster (1,2,3,4,5) showing available scens.
//  buttons to indicate selected caster
//  buttons to created, etc. scens
//  list-style combo to show strings on selected caster.
//
//
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

void CCastStringEditorScenMgrViewport::OnInitialUpdate()
{
	// the view needs to set these before we get here

	assert( m_pView   != 0 );
	
	Synchronize(true);
}


CMessageBus* CCastStringEditorScenMgrViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CCastStringEditorScenMgrViewport::CreateController()
{
	m_pCtlr = new CCastStringEditorScenMgrController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CCastStringEditorScenMgrViewport::OnCreate()
{
	// do nothing
	return TRUE;
}



//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CCastStringEditorScenMgrViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	// any change in scenarios which exist or in our selection

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
	case MSG_ID_SMGR_CREATE_PRODN_SCEN:
	case MSG_ID_SMGR_DELETE_PRODN_SCEN:
	case MSG_ID_SMGR_PRODN_SCEN_INFO_CHANGED:
	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_MRU_PROMOTE_PRODN_SCEN:
	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_CSE_STRING_INFO_CHANGED:
	case MSG_ID_CSE_ACTIVATED:
	case MSG_ID_CSI_CASTER_SCEN_INFO_CHANGED:
		Synchronize(true);
		break;

	default:
		// do nothing
		break;
	}
}


/////////////////////////////////////////////////////
//
//  Managing the display
//
////////////////////////////////////////////////////

int CCastStringEditorScenMgrViewport::GetCasterScenIndex(int casterNum) const
{
	return CScenMgr::CasterScenIndex(casterNum,
									 CScenMru::GlobalScenMru.CurrentCasterScen(casterNum));
}


void CCastStringEditorScenMgrViewport::Synchronize(bool updateLists)
{
	if ( updateLists ) 
		UpdateLists();
	
	SelectComboBoxItem( GetCasterScenIndex(Caster::C1), IDC_COMBO_CASTER_1 );
	SelectComboBoxItem(GetCasterScenIndex(Caster::C2), IDC_COMBO_CASTER_2);
	SelectComboBoxItem(GetCasterScenIndex(Caster::C3), IDC_COMBO_CASTER_3);
	SelectComboBoxItem(GetCasterScenIndex(Caster::C4), IDC_COMBO_CASTER_4);
	SelectComboBoxItem(GetCasterScenIndex(Caster::C5), IDC_COMBO_CASTER_5);

	SelectComboBoxItem( CScenMru::GlobalScenMru.CurrentCastStringIndex(CScenMru::GlobalScenMru.CasterNum()),
						IDC_COMBO_STRING	);

	// offset is 0 for the buttons.
	m_pView->m_casterNumMinus1 = CScenMru::GlobalScenMru.CasterNum()-1;  

	// set the radio button for casterNum!
	m_pView->UpdateData(false); 
}



void CCastStringEditorScenMgrViewport::SelectComboBoxItem( int index, int ctrlId )
{	
	CComboBox* pBox = (CComboBox*) m_pView->GetDlgItem(ctrlId);
	pBox->SetCurSel(index);
}



void CCastStringEditorScenMgrViewport::UpdateLists()
{
	UpdateCasterList(1,IDC_COMBO_CASTER_1);
	UpdateCasterList(2,IDC_COMBO_CASTER_2);
	UpdateCasterList(3,IDC_COMBO_CASTER_3);
	UpdateProdnList(IDC_COMBO_PRODN);

	UpdateStringList();
}


void CCastStringEditorScenMgrViewport::UpdateCasterList(int casterNum, int ctrlId)
{
	CComboBox* pBox = (CComboBox*) m_pView->GetDlgItem(ctrlId);
	pBox->ResetContent();

	ostrstream ostr;

	for ( TCasterScenVec::iterator ics = CScenMgr::CasterScensBegin(casterNum);
		  ics != CScenMgr::CasterScensEnd(casterNum);
		  ++ics ) {

			ostr << left
				 << setw(4) << (*ics)->Id() 
				 << " "      << (LPCTSTR)(*ics)->Comment() 
				 << ends;
			pBox->AddString(ostr.str());
			ostr.freeze(false);
			ostr.seekp(0);
	}
}



void CCastStringEditorScenMgrViewport::UpdateProdnList(int ctrlId)
{
}



void CCastStringEditorScenMgrViewport::UpdateStringList()
{
	CComboBox* pBox = (CComboBox*) m_pView->GetDlgItem(IDC_COMBO_STRING);
	pBox->ResetContent();

	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pScen == 0 ) 
		return;

	ostrstream ostr;

	for ( TCastStringVec::iterator is = pScen->CastStrings().begin();
		  is != pScen->CastStrings().end();
		  ++is ) {

		const CCastStringId& id = (*is)->Id();

		CString strStatus((*is)->Status() == CCastString::STRING_STATUS_910
						   ? "910"
						   : (*is)->Status() == CCastString::STRING_STATUS_909
						   ? "909"
						   : "  ");
		
		CString strSpec = "       ";

		// find first heat with a spec assigned.

		for ( vector<CCastStringHeat>::iterator ih = (*is)->Heats().begin();
		      ih != (*is)->Heats().end();
			  ++ih ) {
			
			if ( (*ih).Spec() != ""  && (*ih).Spec() != "       " ) {
				  strSpec = (*ih).Spec();
				  break;
			  }
		}

		ostr.fill('0'); 
		ostr << setw(2) << id.Year()%100
			 << "-" 
			 << setw(2) << id.Week()
			 << " "
			 << setw(2) << id.StringId()
			 << "   "
			 << setw(5) << long((*is)->SchedTons()+0.5) 
			 << " tons, "
			 << setw(3) << LPCTSTR(strStatus)
			 << ", "
			 << setw(7) << LPCTSTR(strSpec)
			 << ends;

		pBox->AddString(ostr.str());
		ostr.freeze(false);
		ostr.seekp(0);
	}
}

	






//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorScenMgrController Class
//
//  Handles events display for CCastStringEditorScenMgrView/Viewport
//
//////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CCastStringEditorScenMgrController, MvcController)
	//{{AFX_MSG_MAP(CCastStringEditorScenMgrController)
	ON_CBN_SELCHANGE(IDC_COMBO_CASTER_1, OnSelchangeComboCaster1)
	ON_CBN_SELCHANGE(IDC_COMBO_CASTER_2, OnSelchangeComboCaster2)
	ON_CBN_SELCHANGE(IDC_COMBO_CASTER_3, OnSelchangeComboCaster3)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODN, OnSelchangeComboProdn)
	ON_CBN_SELCHANGE(IDC_COMBO_STRING, OnSelchangeComboString)
	ON_BN_CLICKED(IDC_RB_CASTER_1, OnRbCaster1)
	ON_BN_CLICKED(IDC_RB_CASTER_2, OnRbCaster2)
	ON_BN_CLICKED(IDC_RB_CASTER_3, OnRbCaster3)
	ON_BN_CLICKED(ID_EDIT_CREATE_STRING, OnEditCreateString)
	ON_BN_CLICKED(ID_EDIT_DELETE_STRING, OnEditDeleteString)
	ON_BN_CLICKED(ID_SCEN_COPY, OnScenCopy)
	ON_BN_CLICKED(ID_SCEN_CREATE, OnScenCreate)
	ON_BN_CLICKED(ID_SCEN_DELETE, OnScenDelete)
	ON_BN_CLICKED(ID_SCEN_INFO, OnScenInfo)
	ON_UPDATE_COMMAND_UI(ID_SCEN_COPY, OnUpdateScenCopy)
	ON_UPDATE_COMMAND_UI(ID_SCEN_DELETE, OnUpdateScenDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATE_STRING, OnUpdateEditCreateString)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_STRING, OnUpdateEditDeleteString)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRING_INFO, OnUpdateEditStringInfo)
	ON_BN_CLICKED(ID_EDIT_STRING_INFO, OnEditStringInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CCastStringEditorScenMgrController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}




//  Someone changes a caster scenario selection


void CCastStringEditorScenMgrController::OnSelchangeComboCaster1() 
{
	ChangeCasterScen(Caster::C1,IDC_COMBO_CASTER_1);	
}

void CCastStringEditorScenMgrController::OnSelchangeComboCaster2() 
{
	ChangeCasterScen(Caster::C2, IDC_COMBO_CASTER_2);
}

void CCastStringEditorScenMgrController::OnSelchangeComboCaster3() 
{
	ChangeCasterScen(Caster::C3, IDC_COMBO_CASTER_3);
}

void CCastStringEditorScenMgrController::OnSelchangeComboCaster4()
{
	ChangeCasterScen(Caster::C4, IDC_COMBO_CASTER_4);
}

void CCastStringEditorScenMgrController::OnSelchangeComboCaster5()
{
	ChangeCasterScen(Caster::C5, IDC_COMBO_CASTER_5);
}

void CCastStringEditorScenMgrController::ChangeCasterScen(int whichCaster, int ctrlId)
{
	CComboBox* pBox = (CComboBox*) GetViewport()->View()->GetDlgItem(ctrlId);
	int index = pBox->GetCurSel();

	if ( index == CB_ERR )
		return;

	CCasterScen* pNewScen = CScenMgr::CasterScen(whichCaster,index);

	if ( pNewScen == 0 )
		// this probably indicates something bad, but what the hell.
		return;

	//  We better check to make sure this is okay
	//  We are only going to switch displayed scenarios
	//     if the caster on which the selected scenario is changing
	//     is the same as the caster currently begin displayed.

	if ( whichCaster == CScenMru::GlobalScenMru.CasterNum() 
		 &&
		 CScenMru::GlobalScenMru.CurrentCasterScen(whichCaster) == pNewScen )
		 // nochange
		 return;

	if ( whichCaster == CScenMru::GlobalScenMru.CasterNum() ) {
		if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
			return;
	}

	CScenMru::GlobalScenMru.PromoteCasterScen(pNewScen);

	//  Any update will happen indirectly 
}


bool CCastStringEditorScenMgrController::IsEditing() const
{
	return CScenMru::GlobalScenMru.CurrentSuperScen() != 0;	
}



// a new prodn scen has been selected.

void CCastStringEditorScenMgrController::OnSelchangeComboProdn() 
{
	// do nothing	
}



// The selected string has been changed

void CCastStringEditorScenMgrController::OnSelchangeComboString() 
{
	CCasterScen* pScen =  CScenMru::GlobalScenMru.CurrentCasterScen();
	
	if ( pScen == 0 )
		return;

	CComboBox* pBox = (CComboBox*) GetViewport()->View()->GetDlgItem(IDC_COMBO_STRING);
	int index = pBox->GetCurSel();

	CScenMru::GlobalScenMru.SetCurrentCastString(index);
}



// the selected caster num has been changed

void CCastStringEditorScenMgrController::OnRbCaster1() 
{
	SetCasterNum(Caster::C1);	
}

void CCastStringEditorScenMgrController::OnRbCaster2() 
{
	SetCasterNum(Caster::C2);
}

void CCastStringEditorScenMgrController::OnRbCaster3() 
{
	SetCasterNum(Caster::C3);
}

void CCastStringEditorScenMgrController::OnRbCaster4()
{
	SetCasterNum(Caster::C4);
}

void CCastStringEditorScenMgrController::OnRbCaster5()
{
	SetCasterNum(Caster::C5);
}



void CCastStringEditorScenMgrController::SetCasterNum(int newCasterNum)
{
	if ( newCasterNum == CScenMru::GlobalScenMru.CasterNum() )
		return;

	//  We better check to make sure this is okay
	
	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;

	CScenMru::GlobalScenMru.SetCasterNum(newCasterNum);
}




// Clicks on various buttons.


void CCastStringEditorScenMgrController::OnEditCreateString() 
{
	// if we are not editing 
	//    then there is no scenario in which to make a string

	if ( ! IsEditing() )
		return;

	assert( CScenMru::GlobalScenMru.CurrentCasterScen() != 0 );

	CreateCastString(CScenMru::GlobalScenMru.CurrentSuperScen(),CScenMru::GlobalScenMru.CasterNum());
}





void CCastStringEditorScenMgrController::OnEditDeleteString() 
{
	// if not editing, do nothing.
	if ( ! IsEditing() )
		return;

	CCasterScen* pCScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	// if no string selected, do nothing.
	if ( pString == 0 )
		return;

	// Verify this is okay
	if ( IDYES != AfxMessageBox("Okay to delete this string?",MB_YESNO) )
		return;

	pCScen->DeleteString(pString);

	pCScen->SetModified(true);



	// ScenMRU is index-based -- it will adjust automatically
	//    to the deletion of the string
	//  but we need to cause it to send a notification
	  
	ValidateOrderSelection();

	CScenMru::GlobalScenMru.SetCurrentCastString(CScenMru::GlobalScenMru.CurrentCastStringIndex(CScenMru::GlobalScenMru.CasterNum()));
}

void CCastStringEditorScenMgrController::OnEditStringInfo() 
{
	// if we are not editing 
	//    then there is no scenario in which to make a string

	// if not editing, do nothing.
	if ( ! IsEditing() )
		return;

	CCasterScen* pCScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pCScen == 0 ) 
		return;

	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	// if no string selected, do nothing.
	if ( pString == 0 )
		return;
	
	ShowCastStringInfo(pCScen,pString);	
}



void CCastStringEditorScenMgrController::OnScenCopy() 
{
	//  We may not be editing, but we can copy the current one anyway.
	//  We better check to make sure this is okay>>>

	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;

	//  However, there must already be a caster scen on the current caster.

	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pScen == 0 ) 
		return;

	CCasterScen* pNewScen = CScenMgr::CopyCasterScen(pScen,true,false);

	CScenMru::GlobalScenMru.PromoteCasterScen(pNewScen);

	// we will be notified indirectly
}



void CCastStringEditorScenMgrController::OnScenCreate() 
{
	//  We may not be editing, but we can copy the current one anyway.
	//  We better check to make sure this is okay>>>

	if ( IDCANCEL == CScenMru::GlobalScenMru.MaybeCasterScenChangeOk(true) )
		return;
		
	//  if we ever support multiple snapshots
	//		we'll have to do a dialog to select one

	CCasterScen* pNewScen = TheSnapshot.CopyInitialCasterScen(CScenMru::GlobalScenMru.CasterNum());

	if ( pNewScen == 0 ) return;

	CScenMru::GlobalScenMru.PromoteCasterScen(pNewScen);

	// We will be notified indirectly
}

void CCastStringEditorScenMgrController::OnScenDelete() 
{
	//  if we are not editing, then nothing to delete anyway.
	if ( ! IsEditing() )
		return;

	if ( IDYES != AfxMessageBox("Are you sure you want to delete this scenario?",
								MB_ICONQUESTION|MB_YESNO) )
		return;

	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	CScenMgr::DeleteCasterScen(pScen);

	// CScenMRU will be notified of the change
	//    and we will be selecting the next most recent
	// We will get notifcation
}



void CCastStringEditorScenMgrController::OnUpdateScenCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CScenMru::GlobalScenMru.CurrentCasterScen() != 0);	
}


void CCastStringEditorScenMgrController::OnUpdateScenDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CScenMru::GlobalScenMru.CurrentCasterScen() != 0);	
}

void CCastStringEditorScenMgrController::OnUpdateEditCreateString(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CScenMru::GlobalScenMru.CurrentCasterScen() != 0);	
}

void CCastStringEditorScenMgrController::OnUpdateEditDeleteString(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CScenMru::GlobalScenMru.CurrentCastString() != 0);	
}

void CCastStringEditorScenMgrController::OnUpdateEditStringInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CScenMru::GlobalScenMru.CurrentCastString() != 0);	
}





void CCastStringEditorScenMgrController::OnScenInfo() 
{
	CCasterScen* pScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pScen == 0 )
		return;

	CCasterScenInfoDlg dlg;
	dlg.m_pCasterScen = pScen;
	dlg.DoModal();
	// update handled via message
}



//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorStrandSummaryViewport Class
//
//  Manages display for CCastStringEditorStrandSummaryView
//
//  Manages a single list ctrl that displays summary information 
//    on the strands of the currently displayed string.
//
//	There are some check boxes that control the information displayed. 
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

void CCastStringEditorStrandSummaryViewport::OnInitialUpdate()
{
	// the view needs to set these before we get here

	assert( m_pListSummary != 0 );
	assert( m_pView != 0  );

	InitHeaderDetails();

}


CMessageBus* CCastStringEditorStrandSummaryViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CCastStringEditorStrandSummaryViewport::CreateController()
{
	m_pCtlr = new CCastStringEditorStrandSummaryController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CCastStringEditorStrandSummaryViewport::OnCreate()
{
	// do nothing
	return TRUE;
}

//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CCastStringEditorStrandSummaryViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_MRU_PROMOTE_PRODN_SCEN:
	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
	case MSG_ID_CSE_STRING_VALIDATED:
	case MSG_ID_CSE_ACTIVATED:
	case MSG_ID_CSE_STRING_INFO_CHANGED:
	case MSG_ID_CSE_ORDERS_DELETED:
	case MSG_ID_CSE_ORDERS_INSERTED:
	case MSG_ID_CSE_LOCKED_HEATS_STATUS_CHANGED:
	case MSG_ID_CSE_ORDERS_MOVED:
	case MSG_ID_CSE_ORDER_CHANGED:
		SetListItems();
		break;

	default:
		// do nothing
		break;
	}
}

//////////////////////////////////////////////////////
//
//  Manage display
//
//////////////////////////////////////////////////////



void CCastStringEditorStrandSummaryViewport::SetDisplay()
{
	SetListHeader();
	SetListItems();
}



//  Column Assigns are as follows
//
//	Col  0 - Which
//  Col  1 - #Pces
//	Col  2 - Tons
//  Col  3 - Balance
//  Col  4 - HH:MM
//	Col	 5 -- #radicals
//  Col  6 - #CMS
//  Col  7 - #CMS/flips
//  Col  8 - #flips
//  Col  9 - #slits
//  Col 10 - #splits
//  Col 11 - #stock
//  Col 12 - #back
//  Col 13 - #curr
//  Col 14 - #future
//  Col 15 - avg width
//  FP Change
//  Col 16 - tons manual
//  Col 17 - tons computer
//  Col 18 - tons nonprioritized

vector<CString> CCastStringEditorStrandSummaryViewport::headerStrings;
vector<int> CCastStringEditorStrandSummaryViewport::headerWidths;

void CCastStringEditorStrandSummaryViewport::InitHeaderDetails()
{
	m_pView->UpdateData(false);  // first time thru, must make sure m_listCtrl is set.

	headerStrings.push_back("Which");
	headerStrings.push_back("#Pces");
	headerStrings.push_back("Tons");
	headerStrings.push_back("Balance");
	headerStrings.push_back("HH:MM");
	headerStrings.push_back("#RadWd");
	headerStrings.push_back("#CMS");
	headerStrings.push_back("#CMS/flips");
	headerStrings.push_back("#flips");
	headerStrings.push_back("#slits");
	headerStrings.push_back("#splits");
	headerStrings.push_back("#stock");
	headerStrings.push_back("#back");
	headerStrings.push_back("#curr");
	headerStrings.push_back("#future");
	headerStrings.push_back("avg width");
	// FP Change
	headerStrings.push_back("ManualT");
	headerStrings.push_back("CmptrT");
	headerStrings.push_back("NonprioT");

	int num = 0;
	for ( vector<CString>::iterator is = headerStrings.begin();
		  is != headerStrings.end();
		  ++is, ++num ) {

		int width = 15+m_pListSummary->GetStringWidth((*is));

		if ( (*is) == "Which" )
			width += 5;
		else if ( (*is) == "#Pces" )
			width = 15+m_pListSummary->GetStringWidth("99 + 99 = 99");
		else if ( (*is) == "Tons" )
			width = 15+m_pListSummary->GetStringWidth("999 + 999 = 999");
		else if ( (*is) == "avg width" )
			width = 15+m_pListSummary->GetStringWidth("999.9 - 999.9");

		headerWidths.push_back( width );

		m_pListSummary->InsertColumn(num,(*is),LVCFMT_RIGHT,width,num);
	}
}


void CCastStringEditorStrandSummaryViewport::SetListHeader()
{

	vector<int> currColWidths(headerWidths.size());

	currColWidths[0] = headerWidths[0];
	currColWidths[1] = headerWidths[1];
	currColWidths[2] = headerWidths[2];
	currColWidths[3] = headerWidths[3];
	currColWidths[4] = headerWidths[4];
	currColWidths[5] = headerWidths[5];

	if ( m_pView->m_showCmsFlips ) {
		currColWidths[6] = headerWidths[6];
		currColWidths[7] = headerWidths[7];
		currColWidths[8] = headerWidths[8];
	}

	if ( m_pView->m_showSlitsSplits ) {
		currColWidths[ 9] = headerWidths[ 9];
		currColWidths[10] = headerWidths[10];
	}	
	
	if ( m_pView->m_showStock )
		currColWidths[11] = headerWidths[11];

	if ( m_pView->m_showCurrentBackFuture ) {
		currColWidths[12] = headerWidths[12];
		currColWidths[13] = headerWidths[13];
		currColWidths[14] = headerWidths[14];
	}

	if ( m_pView->m_showAvgWidth )
		currColWidths[15] = headerWidths[15];

	// FP Change
	currColWidths[16] = headerWidths[16];
	currColWidths[17] = headerWidths[17];
	currColWidths[18] = headerWidths[18];


	for ( int i=0; i<currColWidths.size(); ++i )

		m_pListSummary->SetColumnWidth(i,currColWidths[i]);
}




void CCastStringEditorStrandSummaryViewport::SetListItems()
{
	CMaintainListCtrlPosn x( *m_pListSummary, true );

	m_pListSummary->DeleteAllItems();

	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	if ( pString == 0 )
		return;

	SetListItem( 0, "Strand 1", pString->StrandSummary(1) );
	SetListItem( 1, "Strand 2", pString->StrandSummary(2) );
	SetListItem( 2, "Total",     pString->StringSummary()  );

	const vector<CCastStringSummaryRecord>& heatBoth	= pString->HeatSummaries(0);
	const vector<CCastStringSummaryRecord>& heatStrand1	= pString->HeatSummaries(1);
	const vector<CCastStringSummaryRecord>& heatStrand2	= pString->HeatSummaries(2);
	const vector<CCastStringHeat>& heats                = pString->Heats();

	assert( heatBoth.size() == heatStrand1.size()
		    &&
			heatBoth.size() == heatStrand2.size() 
			&&
			heats.size() == heatBoth.size() );

	char buf[30];
	int count;

	for ( count=0; count<heatBoth.size(); ++count ) {

		sprintf(buf,"Heat #%d",count+1);

		if ( m_pView->m_showHeatStrands )
			SetListItem(count+3,buf,heatStrand1[count],heatStrand2[count],heats[count],pString);
		else
			SetListItem(count+3,buf,heatBoth[count]);
	}

	// ## add int cast
	count = ((int)heatBoth.size()) + 3;

	{
		for ( CCastStringSummaryRecord::HsmLuTypeToTonsMap::const_iterator im 
				= pString->StringSummary().m_luTypeTonsMap.begin();
			  im != pString->StringSummary().m_luTypeTonsMap.end();
			  ++im, ++count ) 
			
			SetLuTypeTonsListItem(count,(*im).first,(*im).second);
	}

	{
		for ( CCastStringSummaryRecord::CICodeToTonsMap::const_iterator im 
				= pString->StringSummary().m_ciCodeTonsMap.begin();
			  im != pString->StringSummary().m_ciCodeTonsMap.end();
			  ++im, ++count ) 
			
			SetCICodeTonsListItem(count,(*im).first,(*im).second);
	}
}



void CCastStringEditorStrandSummaryViewport::SetListItem(int itemNum,
													 CString str,
													 const CCastStringSummaryRecord& rec)
{
	m_pListSummary->InsertItem(itemNum,str);

	char buf[50];

	m_pListSummary->SetItemText( itemNum,  1, _itoa( rec.m_numPieces,			buf,10) );
	m_pListSummary->SetItemText( itemNum,  2, _ltoa( long(rec.m_tons+0.5),	buf,10) );


	m_pListSummary->SetItemText( itemNum,  3, "");

	sprintf(buf,
			"%d:%02d",
			rec.m_duration.GetTotalHours(),
			rec.m_duration.GetMinutes());

	m_pListSummary->SetItemText( itemNum,  4, buf );

	m_pListSummary->SetItemText( itemNum,	 5,	_itoa( rec.m_numRadicals,			buf,10)	);

	m_pListSummary->SetItemText( itemNum,  6, _itoa( rec.m_numCMS,				buf,10) );
	m_pListSummary->SetItemText( itemNum,  7, _itoa( rec.m_numCMSFlips,			buf,10) );
	m_pListSummary->SetItemText( itemNum,  8, _itoa( rec.m_numFlips,				buf,10) );
	m_pListSummary->SetItemText( itemNum,  9, _itoa( rec.m_numSlits,				buf,10) );
	m_pListSummary->SetItemText( itemNum, 10, _itoa( rec.m_numSplits,				buf,10) );
	m_pListSummary->SetItemText( itemNum, 11, _itoa( rec.m_numStockPieces,		buf,10) );
	m_pListSummary->SetItemText( itemNum, 12, _itoa( rec.m_numBackHRPieces,		buf,10) );
	m_pListSummary->SetItemText( itemNum, 13, _itoa( rec.m_numCurrentHRPieces,	buf,10) );
	m_pListSummary->SetItemText( itemNum, 14, _itoa( rec.m_numFutureHRPieces,		buf,10) );

	sprintf(buf,"%6.1f",rec.m_avgWidth);

	m_pListSummary->SetItemText( itemNum, 15, buf );

	// FP Change
	m_pListSummary->SetItemText( itemNum, 16, _ltoa(rec.m_tonsManual,   buf, 10) );
	m_pListSummary->SetItemText( itemNum, 17, _ltoa(rec.m_tonsComputer, buf, 10) );
	m_pListSummary->SetItemText( itemNum, 18, _ltoa(rec.m_tonsNonprio,  buf, 10) );
	
}



// Someday, get rid of the duplicate code

#define ostreset() ostr.freeze(false);ostr.seekp(0)

void CCastStringEditorStrandSummaryViewport::SetListItem(int itemNum,
													 CString str,
													 const CCastStringSummaryRecord& rec1,
													 const CCastStringSummaryRecord& rec2,
													 const CCastStringHeat& rHeat,
													 CCastString* pString)
{
	m_pListSummary->InsertItem(itemNum,str);

	ostrstream ostr;
	char buf[100];

	ostr << setw(2) << rec1.m_numPieces << " + " 
		 << setw(2) << rec2.m_numPieces << " = "
		 << setw(2) << (rec1.m_numPieces + rec2.m_numPieces) << ends;
	m_pListSummary->SetItemText( itemNum,  1, ostr.str() );
	ostreset();

	ostr << setw(3) << long(rec1.m_tons+0.5) << " + " 
		 << setw(3) << long(rec2.m_tons+0.5) << " = "
		 << setw(3) << long(rHeat.HeatTons() + 0.5) << ends;
	m_pListSummary->SetItemText( itemNum,  2, ostr.str() );
	ostreset();

	int aimHeatSize = pString->Props().AimHeatSize();

	if ( Caster::NumStrandsOnCaster(pString->Id().Caster()) == 1 )
		m_pListSummary->SetItemText( itemNum, 3, _itoa(aimHeatSize,buf,10));
	else {

		int strand1Tons;
		int strand2Tons;
		rHeat.ComputeApproximateStrandBalance(aimHeatSize,strand1Tons,strand2Tons);

		ostr << setw(3) << strand1Tons << " + " 
			 << setw(3) << strand2Tons << ends;
		m_pListSummary->SetItemText( itemNum,  3, ostr.str() );
		ostreset();
	}


	CTimeSpan dur = max( rec1.m_duration,rec2.m_duration);
	sprintf(buf,
			"%d:%02d",
			dur.GetTotalHours(),
			dur.GetMinutes());

	m_pListSummary->SetItemText( itemNum,  4, buf );

	ostr << setw(2) << rec1.m_numRadicals << " - " 
		 << setw(2) << rec2.m_numRadicals << ends;
	m_pListSummary->SetItemText( itemNum,	 5,	ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numCMS << " - " 
		 << setw(2) << rec2.m_numCMS << ends;
	m_pListSummary->SetItemText( itemNum,  6, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numCMSFlips << " - " 
		 << setw(2) << rec2.m_numCMSFlips << ends;
	m_pListSummary->SetItemText( itemNum,  7, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numFlips << " - " 
		 << setw(2) << rec2.m_numFlips << ends;
	m_pListSummary->SetItemText( itemNum,  8, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numSlits << " - " 
		 << setw(2) << rec2.m_numSlits << ends;
	m_pListSummary->SetItemText( itemNum,  9, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numSplits << " - " 
		 << setw(2) << rec2.m_numSplits << ends;
	m_pListSummary->SetItemText( itemNum, 10, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numStockPieces << " - " 
		 << setw(2) << rec2.m_numStockPieces << ends;
	m_pListSummary->SetItemText( itemNum, 11, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numBackHRPieces << " - " 
		 << setw(2) << rec2.m_numBackHRPieces << ends;
	m_pListSummary->SetItemText( itemNum, 12, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numCurrentHRPieces << " - " 
		 << setw(2) << rec2.m_numCurrentHRPieces << ends;
	m_pListSummary->SetItemText( itemNum, 13, ostr.str() );
	ostreset();

	ostr << setw(2) << rec1.m_numFutureHRPieces << " - " 
		 << setw(2) << rec2.m_numFutureHRPieces << ends;
	m_pListSummary->SetItemText( itemNum, 14, ostr.str() );
	ostreset();

	ostr.setf(ios_base::fixed, ios_base::floatfield);	
	ostr << setw(6) << setprecision(1) << rec1.m_avgWidth << " - " 
		 << setw(6) << setprecision(1) << rec2.m_avgWidth << ends;
	m_pListSummary->SetItemText( itemNum, 15, ostr.str() );
	ostreset();

	// FP Change
	ostr << setw(3) << rec1.m_tonsManual << " - "
		 << setw(3) << rec2.m_tonsManual << ends;
	m_pListSummary->SetItemText( itemNum, 16, ostr.str() );
	ostreset();
	ostr << setw(3) << rec1.m_tonsComputer << " - "
		 << setw(3) << rec2.m_tonsComputer << ends;
	m_pListSummary->SetItemText( itemNum, 17, ostr.str() );
	ostreset();
	ostr << setw(3) << rec1.m_tonsNonprio << " - "
		 << setw(3) << rec2.m_tonsNonprio << ends;
	m_pListSummary->SetItemText( itemNum, 18, ostr.str() );
	ostreset();


}



void CCastStringEditorStrandSummaryViewport::SetLuTypeTonsListItem(int itemNum,
															   const CString& luType,
															   Weight tons)
{
	m_pListSummary->InsertItem(itemNum,luType);

	char buf[20];

	m_pListSummary->SetItemText( itemNum, 2, _ltoa( long(tons+0.5), buf, 10 ) );
}



void CCastStringEditorStrandSummaryViewport::SetCICodeTonsListItem(int itemNum,
															   int ciCode,
															   Weight tons)
{
	char buf[20];	
	
	sprintf(buf,"CI %02d",ciCode);

	m_pListSummary->InsertItem(itemNum,buf);

	m_pListSummary->SetItemText( itemNum, 2, _ltoa( long(tons+0.5), buf, 10 ) );
}







//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorStrandSummaryController Class
//
//  Handles events display for CCastStringEditorStrandSummaryView/Viewport
//
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCastStringEditorStrandSummaryController, MvcController)
	//{{AFX_MSG_MAP(CCastStringEditorStrandSummaryController)
	ON_BN_CLICKED(IDC_CHECK_AVG_WIDTH, OnCheckAvgWidth)
	ON_BN_CLICKED(IDC_CHECK_CMS_FLIPS, OnCheckCmsFlips)
	ON_BN_CLICKED(IDC_CHECK_CURRENT_BACK_FUTURE, OnCheckCurrentBackFuture)
	ON_BN_CLICKED(IDC_CHECK_SLITS_SPLITS, OnCheckSlitsSplits)
	ON_BN_CLICKED(IDC_CHECK_STOCK, OnCheckStock)
	ON_BN_CLICKED(IDC_CHECK_HEAT_STRANDS, OnCheckHeatStrands)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CCastStringEditorStrandSummaryController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


void CCastStringEditorStrandSummaryController::OnCheckAvgWidth() 
{
	GetViewport()->View()->UpdateData(true);
	GetViewport()->SetListHeader();
}

void CCastStringEditorStrandSummaryController::OnCheckCmsFlips() 
{
	GetViewport()->View()->UpdateData(true);
	GetViewport()->SetListHeader();
}

void CCastStringEditorStrandSummaryController::OnCheckCurrentBackFuture() 
{
	GetViewport()->View()->UpdateData(true);
	GetViewport()->SetListHeader();
}

void CCastStringEditorStrandSummaryController::OnCheckSlitsSplits() 
{
	GetViewport()->View()->UpdateData(true);
	GetViewport()->SetListHeader();
}

void CCastStringEditorStrandSummaryController::OnCheckStock() 
{
	GetViewport()->View()->UpdateData(true);
	GetViewport()->SetListHeader();
}

void CCastStringEditorStrandSummaryController::OnCheckHeatStrands() 
{
	GetViewport()->View()->UpdateData(true);	
	GetViewport()->SetListItems();
}





//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorTextViewport Class
//
//  Manages display for CCastStringEditorTextView
//
//////////////////////////////////////////////////////////////////////

/*

  CCastStringEditorTextView has the following interface elements:

	Strand 1 and strand 2 lists
		Show the sequence of orders lined up on each strand

    Group list
	    available orders, grouped by certain selected fields

    Selected orders list
	    available orders, 
		    those belonging to whichever groups which are selected
			plus 'special' selected orders that are of interest.

    Group/sort/avail fields list
	     show the current sorting/grouping criteria

    Miscellaneous interface elements, primarily push buttons 



  The primary operations that change state are:

	(1) Selection on a strand list
	(2) Selection on the group list
	(3) Selection on the selected order list
	(4) Change in sort/group criteria
	(5) Drag from strand causing orders to move between strands
	(6) Drag from strand causing orders to be deleted
	(7) Drag from group list or selected orders list to insert orders into a strand
	(8) Change in #pieces on an order in a strand list (by double click)
	(9) Change in the filter for available orders

  We are sometimes interested in maintaining the position in one of the lists
  when an update to its contents occur.

  We are sometimes interested in maintaining the selection in one of the lists
  (specifically, the group list) when an update to its contents occur.

  Here is what we try to do (OS here refers to the attached COrderSelection
  for the string being edited):


	(1) Selection on a strand list

		We want groups that match the selected order(s) (by spec and width)
		to be selected.
		If there is an order which does not appear in any group (it
		has been filtered out) then it appears in the special list.
		Position in group list is maintained, but not in the order list.
	
		How:	Determine selected orders on the strand
		        OS->SelectMatchingGroups()
				Update selection on group list (items still okay)
				Update selected orders list
	

	(2) Selection on the group list
		
		The selected groups' orders are displayed in the selected orders list.

		How:	Determine selected groups in the group list
				Update OS (OS->SetSelectedGroups)
				Update selected orders list


	(3) Selection on the selected order list

		Nothing to do here.


	(4) Change in sort/group criteria

		We need to regroup.  Whatever orders are in selected groups
		before, we can make the groups they are in after regrouping
		be selected.

		How:	OS.SetGroupAndSortFields
				update group list
				update orders list


	(5) Drag from strand causing orders to move between strands

		Only the strand lists need to be updated.
		We should maintain position.

		
		We need to update the strand list.
		We should maintain position.
		We should cause the orders which were just deleted to appear
		  in the selected orders list

		How:	Update strand list
				OS.UpdateGroupsFromOrderChange()
				Update selection on group list (items still okay)
				Update selected orders list
			  

	(7) Drag from group list or selected orders list to insert orders into a strand

		Same as the previous item, except group list items may change

	(8) Change in #pieces on an order in a strand list (by double click)

		Similar to previous item, but we need only update the strand list item
		corresponding to the changed item.

	(9) Change in the filter for available orders

		How:	Refilter/Regroup
				update group list (no need to maintain position)
				update orders list


	We also use this viewport to do updates on the document title.

*/

///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

CCastStringEditorTextViewport::CCastStringEditorTextViewport()
{
	m_pOrderForInsert	= 0;
}


CCastStringEditorTextViewport::~CCastStringEditorTextViewport()
{
}


void CCastStringEditorTextViewport::OnInitialUpdate()
{
	// the view, etc. needs to set these before we get here

	assert( m_pBtnEditable != 0  );
	assert( m_pBtnHeatsLocked != 0  );
	assert( m_pBtnSave != 0  );
	assert( m_pInsertStockOrderButton != 0  );
	assert( m_pInsertOrderButton != 0  );
	assert( m_pListOrdersEx != 0  );
	assert( m_pListGroupsEx != 0  );
	assert( m_pStrand1OrderListEx != 0  );
	assert( m_pStrand2OrderListEx != 0  );
	assert( m_pLbeGroupFields != 0  );
	assert( m_pLbeSortFields != 0  );

	assert( m_pView != 0  );

	SetButtons();
	SetStrandOrderListHeaders();
	SetGroupListHeader();
	SetGroupOrdersListHeader();
	SetOrderForInsert();

}


CMessageBus* CCastStringEditorTextViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CCastStringEditorTextViewport::CreateController()
{
	m_pCtlr = new CCastStringEditorTextController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CCastStringEditorTextViewport::OnCreate()
{
	// do nothing
	return TRUE;
}


//////////////////////////////////////////////
//
//  various auxiliary functions
//
///////////////////////////////////////////////


bool CCastStringEditorTextViewport::IsEditing() const
{
	return CScenMru::GlobalScenMru.CurrentSuperScen() != 0;	
}


int CCastStringEditorTextViewport::CasterNum()
{	
	return CScenMru::GlobalScenMru.CasterNum();
}


CCastString* CCastStringEditorTextViewport::CurrentCastString()
{
	return CScenMru::GlobalScenMru.CurrentCastString();
}

CCasterScen* CCastStringEditorTextViewport::CurrentCasterScen()
{
	return CScenMru::GlobalScenMru.CurrentCasterScen();
}

CCasterScen* CCastStringEditorTextViewport::CurrentCasterScen(int casterNum)
{
	return CScenMru::GlobalScenMru.CurrentCasterScen(casterNum);
}


COrderSelection* CCastStringEditorTextViewport::OrderSelection()
{ 
	CCastString* pSelectedString = CurrentCastString();

	return pSelectedString == 0 ? 0 : &pSelectedString->OrderSelection();
}


CSuperScen* CCastStringEditorTextViewport::SuperScen()
{
	return CScenMru::GlobalScenMru.CurrentSuperScen();
}


/////////////////////////////////////////////////////////
//
//  Messages
//
////////////////////////////////////////////////////////

void CCastStringEditorTextViewport::CCSOrderChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: CSOrder changed");
}

void CCastStringEditorTextViewport::COrderForInsertChangeMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: order-for-insert changes");
}

void CCastStringEditorTextViewport::COrdersMovedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: orders moved");
}

void CCastStringEditorTextViewport::CStringCanEditStatusChanged::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: string can-edit status changed");
}

void CCastStringEditorTextViewport::CLockedHeatsStatusChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: string locked heats status changed");
}

void CCastStringEditorTextViewport::CScenSavedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: scenario saved");
}

void CCastStringEditorTextViewport::COrdersInsertedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: orders inserted");
}

void CCastStringEditorTextViewport::COrdersDeletedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: orders deleted");
}

void CCastStringEditorTextViewport::CStringInfoChangedMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: string info changed");
}





//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CCastStringEditorTextViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_MRU_PROMOTE_PRODN_SCEN:
	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_CSE_ACTIVATED:

		ValidateOrderSelection();

		if ( OrderSelection() != 0 ) {
			View()->m_availSortFields	= OrderSelection()->AvailSortFields();	// copy
			View()->m_availGroupFields	= OrderSelection()->AvailGroupFields();	// copy
			View()->m_sortFields		= OrderSelection()->SortFields();		// copy
			View()->m_groupFields		= OrderSelection()->GroupFields();		// copy
		}

		View()->UpdateData(FALSE);
		SetButtons();
		SetGroupOrdersListItems();
		SetStrandOrderLists(false);
		SetOrderForInsert();
		SetGroupListItems();
		UpdateTitle();
		break;


	case MSG_ID_CSE_STRING_INFO_CHANGED:
		SetButtons();
		SetStrandOrderLists(true);
		UpdateTitle();
		break;


	case MSG_ID_CSE_ORDER_FOR_INSERT_CHANGED:
		SetOrderForInsert();
		break;

	// This corresponds to: (5) Drag from strand causing orders to move between strands

	case MSG_ID_CSE_ORDERS_MOVED:
		{
			COrdersMovedMsg* pM = dynamic_cast<COrdersMovedMsg*>(pMsg);
			if ( pM != 0 ) {
				SetButtons();
				CStrandOrderPosnHint hint(pM->StrandNum(),
										  pM->HeatIndex(),
										  pM->OrderIndex());
				SetStrandOrderLists(true,&hint);
			}
		}
		break;


	// This corresponds to: (6) Drag from strand causing orders to be deleted

	case MSG_ID_CSE_ORDERS_DELETED:  
		SetButtons();
		SetStrandOrderLists(true);
		SetGroupOrdersListItems();
		SetGroupListItems(true);
		break;


	// This corresponds to:	(7) Drag from group list or selected orders list to insert orders into a strand
	// This corresponds to: (8) Change in #pieces on an order in a strand list (by double click)
	//    We could be a little smarter in the (8) case, if we were told which one had changed.

	case MSG_ID_CSE_ORDERS_INSERTED:  
		{
			COrdersInsertedMsg* pM = dynamic_cast<COrdersInsertedMsg*>(pMsg);
			if ( pM != 0 ) {
				SetButtons();
				CStrandOrderPosnHint hint(pM->StrandNum(),
										  pM->HeatIndex(),
										  pM->OrderIndex());
				SetStrandOrderLists(true,&hint);
				SetGroupOrdersListItems();
				SetGroupListItems(true);
				}
		}
		break;

	
	case MSG_ID_CSE_ORDER_CHANGED:  
		{
			CCSOrderChangedMsg* pM = dynamic_cast<CCSOrderChangedMsg*>(pMsg);
			if ( pM != 0 ) {
				SetButtons();
				CStrandOrderPosnHint hint(pM->StrandNum(),
										  -1,
										  pM->Index());
				SetStrandOrderLists(true,&hint);
				SetGroupOrdersListItems();
				SetGroupListItems(true);
				}
		}
		break;


	case MSG_ID_CSE_CAN_EDIT_STATUS_CHANGED:
		SetButtons();
		break;


	case MSG_ID_CSE_LOCKED_HEATS_STATUS_CHANGED:
		SetButtons();
		SetStrandOrderLists(true);
		break;


	case MSG_ID_CSE_SCEN_SAVED:
		SetButtons();
		break;

	default:
		break;
	}
}



//////////////////////////////////////////////////////
//
//  Manage display
//
//////////////////////////////////////////////////////



void CCastStringEditorTextViewport::SetOrderForInsert()
{
	COrder* pOrder = OrderForInsert();
	

	if ( pOrder == 0 ) {
		m_pInsertOrderButton->SetWindowText("-");
		m_pInsertOrderButton->EnableWindow(FALSE);
	}
	else {
		// FP change
		//char buf[20];
		//m_pInsertOrderButton->SetWindowText(_ltoa(pOrder->OrderNum(),buf,10));
		m_pInsertOrderButton->SetWindowText(pOrder->FpOrderNum());
		m_pInsertOrderButton->EnableWindow(TRUE);
	}
}




void CCastStringEditorTextViewport::SetButtons()
{
	int editState;
	int heatStatus;
	bool canSave;

	CCastString* pString = (IsEditing() 
							? CurrentCastString()
							: 0);

	if ( pString == 0 ) {
		editState = 0;
		canSave   = false;
		heatStatus = 0;
	}
	else {

		editState = pString->CanEdit() ? 1 : 0;
		canSave	  = pString->CasterScen()->IsModified();	
		heatStatus = pString->HeatStatus();
	}

	m_pBtnEditable->SetCurrentState( editState );
	m_pBtnHeatsLocked->SetCurrentState( heatStatus );


	m_pBtnSave->EnableWindow( canSave );
}



/////////////////////////////////////////////////////////////////
//
//  Document title
//
/////////////////////////////////////////////////////////////////


void CCastStringEditorTextViewport::UpdateTitle()
{
	if ( CurrentCastString() == 0 ) 
		View()->GetDocument()->SetTitle("No string selected.");
	else {
		ostrstream ostr;
		ostr << "Editing string "
			 << CurrentCastString()->Id().Year()
			 << "-"
			 << CurrentCastString()->Id().Week()
			 << "-"
			 << CurrentCastString()->Id().StringId()
			 << ", caster "
			 << CurrentCastString()->Id().Caster()
			 << ", scen# "
			 << CurrentCastString()->CasterScen()->Id()
			 << ends;
		View()->GetDocument()->SetTitle(ostr.str());
		ostr.freeze(false);
	}
}



////////////////////////////////////////////////////////////////
//	
//	The strand order lists
//	
////////////////////////////////////////////////////////////////

//	There are two lists (type CListCtrl) which display the orders (CSOrders)
//		lined up on strand #1 and strand #2. 
//	The headers on these lists are initialized in OnInitialUpdate 



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
m_pStrand1OrderListEx->InsertColumn(colNum,str,format,wid,colNum); \
m_pStrand2OrderListEx->InsertColumn(colNum,str,format,wid,colNum); \
++colNum


#define CompWidth3(x,label,format) ( max((x)+m_pStrand1OrderListEx->GetStringWidth((label)),m_pStrand1OrderListEx->GetStringWidth((format))) )
#define CompWidth2(x,label) ( (x) + m_pStrand1OrderListEx->GetStringWidth((label)) )


void CCastStringEditorTextViewport::SetStrandOrderListHeaders()
{
	static const char* strOrder		= "Ord#"   ;
	static const char* strChange	= "+"      ;
	static const char* strHrUnit	= "HR Unit";  // Added 4-28-10 k. hubbard
	static const char* strHeat		= "Ht"     ;
	static const char* strSpec		= "Spec"   ;
	static const char* strHBWidth	= "HBW"   ;
	static const char* strSlabWidth	= "SLW"   ;
	static const char* strCI		= "CI"     ;
	static const char* strCondn		= "Condn"  ;
	static const char* strPcs		= "Pcs"    ;
	static const char* strPrio		= "Prio"   ;
	static const char* strLength	= "Lngth"  ;
	static const char* strHrWk		= " HR "	   ; // Increased title spacing to ensure full field data display, eliminating mandatory resizing of column 4-26-10 k. hubbard
	static const char* strTons		= "Tons"   ;
	static const char* strDispSlit	= "DSPLT"  ;
	static const char* strStartTime	= "PST"	   ;
	static const char* str80Type	= "  80Lu  "   ; // Increased title spacing to ensure full field data display, eliminating mandatory resizing of column 4-26-10 k. hubbard
	static const char* strCust		= "Cust"   ;
	static const char* strC			= "C"      ;
	static const char* strMn		= "Mn"     ;
	static const char* strSignifEl	= "Signif" ;

	static const int widthOrder		= CompWidth2( 15,               ">>>9999999-MMMMM"	);
	static const int widthChange	= CompWidth3( 15, strChange,	"X"				);
	static const int widthHRUnit	= CompWidth2( 15,               "MMMM"			);  // Added 4-28-10 k. hubbard
	static const int widthHeat		= CompWidth2( 15,               "99"			);
	static const int widthSpec		= CompWidth2( 15,               ">>>999-99-99"	);
	static const int widthHbWidth	= CompWidth3( 15, strHBWidth,   "99.9"			);
	static const int widthSlabWidth	= CompWidth3( 15, strSlabWidth, "99"		    );
	static const int widthCI		= CompWidth2( 15,               "99"		    );
	static const int widthCondn		= CompWidth3( 15, strCondn,     "Unlocked"		    );
	static const int widthPcs		= CompWidth3( 15, strPcs,       "99"		    );
	static const int widthPrio		= CompWidth2( 15,               ">>>99-9"		);
	static const int widthLength	= CompWidth3( 15, strLength,    "999"			);
	static const int widthHrWk		= CompWidth3( 15, strHrWk,      "9999"			);
	static const int widthTons		= CompWidth2( 15,               "9999"			);
	static const int widthDispSlit  = CompWidth3( 15, strDispSlit,	"9M"			);
	static const int widthStartTime	= CompWidth3( 15, strStartTime, "-9999"			);
	static const int width80Type	= CompWidth3( 15, str80Type,	"MMMM"			);
	static const int widthCust		= CompWidth3( 15, strCust,		"MMMMMMMMMM"	);
	static const int widthC         = CompWidth2( 15, "9.999 - 9.999"				);
	static const int widthMn        = CompWidth2( 15, "9.999 - 9.999"				);
	static const int widthSignifEl  = CompWidth2( 15, "XX: 9.999 - 9.999"			);

	int colNum = 0;

	DefCol(LVCFMT_RIGHT,  strOrder,     widthOrder     ); //  0
	DefCol(LVCFMT_RIGHT,  strChange,	widthChange    ); //  1
	DefCol(LVCFMT_RIGHT,  strHrUnit,    widthHRUnit    ); //  2  // Added 4-28-10 k. hubbard
	DefCol(LVCFMT_RIGHT,  strHeat,      widthHeat      ); //  3
	DefCol(LVCFMT_RIGHT,  strSpec,      widthSpec      ); //  4
	DefCol(LVCFMT_RIGHT,  strHBWidth,   widthHbWidth   ); //  5
	DefCol(LVCFMT_RIGHT,  strSlabWidth, widthSlabWidth ); //  6
	DefCol(LVCFMT_RIGHT,  strCondn,     widthCondn     ); //  7
	DefCol(LVCFMT_RIGHT,  strDispSlit,	widthDispSlit  ); //  8
	DefCol(LVCFMT_RIGHT,  strPcs,	    widthPcs       ); //  9
	DefCol(LVCFMT_RIGHT,  strTons,      widthTons      ); // 10
	DefCol(LVCFMT_RIGHT,  strPrio,      widthPrio      ); // 11
	DefCol(LVCFMT_RIGHT,  strStartTime,	widthStartTime ); // 12
	DefCol(LVCFMT_RIGHT,  strLength,    widthLength    ); // 13
//	DefCol(LVCFMT_RIGHT,  strHrWk,      widthHrWk      ); // 14
	DefCol(LVCFMT_CENTER, strHrWk,      widthHrWk      ); // 14
	DefCol(LVCFMT_RIGHT,  strCI,        widthCI        ); // 15
	DefCol(LVCFMT_CENTER, str80Type,	width80Type	   ); // 16
	DefCol(LVCFMT_LEFT,	  strCust,		widthCust	   ); // 17
	DefCol(LVCFMT_RIGHT,  strC,         widthC         ); // 18
	DefCol(LVCFMT_RIGHT,  strMn,        widthMn        ); // 19
	DefCol(LVCFMT_RIGHT,  strSignifEl,  widthSignifEl  ); // 20
}




void CCastStringEditorTextViewport::SetStrandOrderLists(bool maintainPosition,
														CStrandOrderPosnHint* pHint)
{

	CCastString* pString = (IsEditing() 
							? CurrentCastString()
							: 0);

	// in case we are not editing, the following method works on 0
	// (it just empties the list).

	SetStrandOrderList(pString,1,*m_pStrand1OrderListEx,maintainPosition);
	SetStrandOrderList(pString,2,*m_pStrand2OrderListEx,maintainPosition);

// original arg usage
	if ( pHint != 0 && pHint->m_heatIndex == -1 && pHint->m_orderIndex == -1 ) {
		// move to end
		CListCtrl* pListCtrl = ( pHint->m_strandNum == 1 
			                    ? m_pStrand1OrderListEx 
								: m_pStrand2OrderListEx);
		if ( pListCtrl->GetItemCount() > 0 )
			pListCtrl->EnsureVisible(pListCtrl->GetItemCount()-1,false);
	}
}



void CCastStringEditorTextViewport::SetStrandOrderList(CCastString* pString,
												   int strandNum, 
												   CListCtrl& listCtrl,
												   bool maintainPosition)
{
	CMaintainListCtrlPosn x(listCtrl,maintainPosition);

	listCtrl.DeleteAllItems();

	if ( pString == 0 )
		return;


	CCastString::T_HeatVec& heats = pString->Heats();
	const CCastString::T_CSOrderVec& strand = pString->Strand(strandNum);

	int orderCount	= 0;
	int listCount	= 0;
	int heatCount	= 0;

	for ( CCastString::T_HeatVec::iterator ih = heats.begin();
		  ih != heats.end();
		  ++ih, ++heatCount ) {

		for ( CCastString::T_CSOrderVec::const_iterator is = (*ih).StrandBegin(strandNum);
		      is != (*ih).StrandEnd(strandNum);
			  ++is, ++orderCount ) {

			SetStrandOrderListItem( (*is),strandNum,listCtrl,listCount,orderCount,true);
			++listCount;
		}

		SetStrandOrderListItem( (*ih), strandNum, pString, listCtrl, listCount, heatCount, true);
		++listCount;
	}
}

//			ListCtrl.SetItemState(m_nCheckedItem,
//				INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);




// TODO -- we could optimize -- 
//		on !doInsert, we will only change slabWidth, slabLength, condnCode, numPieces

void CCastStringEditorTextViewport::SetStrandOrderListItem(CCSOrder* pCSOrder,
													   int strandNum, 
												       CListCtrl& listCtrl,
													   int itemNum,
													   int orderCount,
													   bool doInsert)
{

	char buf[50];

	// Col 0 -- Order#
	
	//FP Change
	//sprintf(buf,"%s%07d",
	//		( pCSOrder->OrderNum()/10000%10 == 0 // 4th digit
	//		  ? ">>>"
	//		  : "   "),
	//		pCSOrder->OrderNum());
	sprintf(buf,"%s%s",
			( pCSOrder->FpOrderNum().GetLength() >= 5 && pCSOrder->FpOrderNum()[4] == '0' // 4th digit
			  ? ">>>"
			  : "   "),
			LPCTSTR(pCSOrder->FpOrderNum()));

	if ( doInsert )
		listCtrl.InsertItem(itemNum,buf);
	else
		listCtrl.SetItemText(itemNum,0,buf);

	// Col 1 -- changeFlag
	listCtrl.SetItemText(itemNum,1,CString(pCSOrder->ChangeFlag()));


	// Col 2 -- Heat#
//	listCtrl.SetItemText(itemNum,2,_itoa(pCSOrder->HeatSeqNum()+1,buf,10)); moved column count by 1 to make room for new Hotroll unitname column 4-28-10 k. hubbard
	listCtrl.SetItemText(itemNum,3,_itoa(pCSOrder->HeatSeqNum()+1,buf,10));

	// TODO -- fix this, when we get a common order table

	// Even with a common order table, 
	// it is possible that we might be missing an order
	// matched to a CSOrder, so we must protect the reference to ->Order()->
	//
	// Col  3 -- spec

	const CString& spec = (pCSOrder->Order() == 0 
					       ? pCSOrder->LotSpec()
						   : pCSOrder->Order()->OrderedSpec());

	CString rightDig = spec.Right(1);

	ostrstream ostr;

	if ( rightDig == "2" || rightDig == "5" )
		ostr << "* ";
	
	ostr << LPCTSTR(spec.Left(3))
		 << "-"
		 << LPCTSTR(spec.Mid(3,2))
		 << "-"
		 << LPCTSTR(spec.Right(2))
		 << ends;
//	listCtrl.SetItemText(itemNum,  3, ostr.str()); increased column count by 1 to make room for new Hotroll unitname column 4-28-10 k. hubbard
	listCtrl.SetItemText(itemNum,  4, ostr.str()); 
	ostr.freeze(false);
	ostr.seekp(0);

	// Col  5 -- HB Width
	// Col 15 -- CI Code
	// Col 11 -- priority
	// Col 14 -- HR Week
	// Col 12 -- StartTime
	// Col 16-- MegaLuCd
	// Col 17 -- Customer

	//	Increased column count by 1 to make room for new Hotroll unitname column 4-28-10 k. hubbard
	if ( pCSOrder->Order() == 0 ) {
		listCtrl.SetItemText(itemNum, 2,"?");
		listCtrl.SetItemText(itemNum, 5,"?");
		listCtrl.SetItemText(itemNum,15,"?");
		listCtrl.SetItemText(itemNum,11,"?");
		listCtrl.SetItemText(itemNum,14,"?");
		listCtrl.SetItemText(itemNum,12,"?");
		listCtrl.SetItemText(itemNum,16,"?");
		listCtrl.SetItemText(itemNum,17,"?");
	}
	else {

	// Col 2 -- Hotroll UnitName #  Added 4-28-10 k. hubbard
//		listCtrl.SetItemText(itemNum,2,_itoa(pCSOrder->Order()->HotrollUnitName(),buf,10));
		strcpy(buf,LPCTSTR(pCSOrder->Order()->HotrollUnitName().Left(4)));
		listCtrl.SetItemText(itemNum,2,buf);

		sprintf(buf,"%4.1f",pCSOrder->Order()->HbWidth());
		listCtrl.SetItemText(itemNum, 5,buf);

		listCtrl.SetItemText(itemNum,15,_itoa(pCSOrder->Order()->CICode(),buf,10));

		sprintf(buf,"%s%2d-%2d",
				(pCSOrder->Order()->CommodityPriority() == 99 ? ">>>" : "   "),
			    pCSOrder->Order()->CommodityPriority(),
				pCSOrder->Order()->OrderPriority());
		listCtrl.SetItemText(itemNum,11,buf);

		listCtrl.SetItemText(itemNum,14,_itoa(pCSOrder->Order()->HrYrWk().Week(),buf,10));

		//sprintf(buf,"%08d",
		//		pCSOrder->Order()->PlannedLatestStartDate());
		sprintf(buf,"%5d",
				NTime::PSDateDelta(pCSOrder->Order()->PlannedLatestStartDate()));

		listCtrl.SetItemText(itemNum,12,buf);

		strcpy(buf,LPCTSTR(pCSOrder->Order()->CustName().Left(10)));
		listCtrl.SetItemText(itemNum,17,buf);

		listCtrl.SetItemText(itemNum,16,pCSOrder->Order()->MegaLuCd());

	}


	// Col  6 -- Slab width
	// Col  7 -- Condn code
	// Col	8 -- Disp/Slit
	// Col  9 -- numPieces
	// Col 13 -- Slab length
	// Col 10 -- Tons
//	sprintf(buf,"%2.1f",pCSOrder->Order()->SlabWidth());        // 9-14-07 k.hubbard testing
	sprintf(buf,"%2.1f",pCSOrder->SlabWidth());        // 9-14-07 k.hubbard decimal display testing works
	listCtrl.SetItemText(itemNum,6,buf);               // 9-14-07 k.hubbard decimal display testing works
//	listCtrl.SetItemText( itemNum,  5, _ltoa(pCSOrder->SlabWidth(),  buf, 10));  // 9-14-07 k.hubbard testing

//	listCtrl.SetItemText( itemNum,  5, _ltoa(long(pCSOrder->SlabWidth()),  buf, 10));  // 9-14-07 k.hubbard original comm. out b4 testing above.
	listCtrl.SetItemText( itemNum,  7, _itoa(pCSOrder->SlabCondnCode(),    buf, 10));
	listCtrl.SetItemText( itemNum,  9, _itoa(pCSOrder->NumPieces(),        buf, 10));
	listCtrl.SetItemText( itemNum, 13, _ltoa(long(pCSOrder->SlabLength()), buf, 10));
	listCtrl.SetItemText( itemNum, 10, _ltoa(long(pCSOrder->Tons()),       buf, 10));

	sprintf(buf,"%d-%c",
			pCSOrder->DispCode(),
			(pCSOrder->SlitTypeCode() == ' ' 
			 ? '_'
			 : pCSOrder->SlitTypeCode()));

	listCtrl.SetItemText(itemNum,8,buf);

	
	listCtrl.SetItemState(itemNum,
						  INDEXTOSTATEIMAGEMASK( pCSOrder->ChangeMarked() ? 2 : 1 ), 
							  LVIS_STATEIMAGEMASK);

	
	// Col 18 -- Carbon
	// Col 19 -- Manganese
	// Col 20 -- SignifEl

	CString strChem;

	CChem::Bound minC;
	CChem::Bound maxC;
	CChem::Bound minMn;
	CChem::Bound maxMn;
	CChem::Bound minSig;
	CChem::Bound maxSig;
	CString sigSym;
	bool hasReasonCode;

	bool cOk   = pCSOrder->GetChemRange( CChem::ELT_C,  minC,  maxC, hasReasonCode  );
	bool mnOk  = pCSOrder->GetChemRange( CChem::ELT_MN, minMn, maxMn, hasReasonCode );
	bool sigOk = false;

	if ( pCSOrder->Order() == 0 ) {

		minSig =   0.0;
		maxSig = 100.0;
		sigSym = "";
		sigOk  = false;

	}
	else {
		minSig = pCSOrder->Order()->MinSignifEl();
		maxSig = pCSOrder->Order()->MaxSignifEl();
		sigSym = pCSOrder->Order()->SignifElSym();
		sigOk  = minSig != 0.0 || maxSig != 100.0;
	}


	// Carbon
	if ( cOk )
		strChem.Format("%5.3f - %5.3f",minC,maxC);
	else
		strChem.Format("-");

	listCtrl.SetItemText( itemNum,18, LPCTSTR(strChem) );

	// Mn
	if ( mnOk )
		strChem.Format("%5.3f - %5.3f",minMn,maxMn);
	else
		strChem.Format("-");

	listCtrl.SetItemText( itemNum,19, LPCTSTR(strChem) );

	// Signif el
	if ( sigOk )
		strChem.Format("%s: %5.3f - %5.3f",LPCTSTR(sigSym),minSig,maxSig);
	else
		strChem.Format("-");
	
	listCtrl.SetItemText( itemNum,20, LPCTSTR(strChem) );

	//listCtrl.SetItemData(itemNum,EncodeOrderIndex(orderCount));

}


// This one's for Heat's Summary Line inside each strand window.  k. hubbard

void CCastStringEditorTextViewport::SetStrandOrderListItem(CCastStringHeat& rHeat,
													   int strandNum, 
													   CCastString* pString,
												       CListCtrl& listCtrl,
													   int itemNum,
													   int heatCount,
													   bool doInsert)
{

	char buf[50];

	// Col 0 -- Order#
	if ( doInsert )
		listCtrl.InsertItem(itemNum,"Heat");
	else
		listCtrl.SetItemText(itemNum,0,"Heat");

	// Col 1 -- changeFlag
	//		leave blank

//	Increased all listCtrl column counts greater than 1, by 1 to make room for new Hotroll unitname column 4-28-10 k. hubbard
	// Col 2 -- Heat#
	listCtrl.SetItemText(itemNum,3,_itoa(heatCount+1,buf,10));

	// Col  3 -- spec
	//		heat spec

	CString spec = rHeat.Spec();

	if ( spec.GetLength() < 7 )
		spec = CString(spec + "       ").Left(7);
	
	CString rightDig = spec.Right(1);

	ostrstream ostr;

	ostr << ( rightDig == "2" || rightDig == "5" ? ">>>" : "   ")
		 << LPCTSTR(spec.Left(3))
		 << "-"
		 << LPCTSTR(spec.Mid(3,2))
		 << "-"
		 << LPCTSTR(spec.Right(2))
		 << ends;

	listCtrl.SetItemText(itemNum,  4, ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	// Col  6 -- Lock status
	CCastString::E_HeatLockStatus lockStatus = pString->HeatStatus();
	bool locked = ( lockStatus == CCastString::HEATS_ALL_LOCKED
					||
					lockStatus == CCastString::HEATS_SOME_LOCKED
					&&
					heatCount < pString->NumLockedHeats());
	listCtrl.SetItemText(itemNum, 7, (locked ? "Locked" : "Unlocked"));

	// Col  5 -- HB Width
	// Col  7 -- Condn code
	// Col	8 -- Disp/Slit
	// Col 11 -- priority
	// Col 13 -- Slab length
	// Col 14 -- HR Week
	// Col 15 -- CI Code

	//		all blank


	// Col  6 -- Slab width
	// Col  9 -- numPieces
	// Col 10 -- Tons

	// TODO - compute beginning, end slab width
	//                total # pieces on strand
	//				  total tons on strand

//	listCtrl.SetItemText( itemNum,  5, _ltoa(long(pCSOrder->SlabWidth()),  buf, 10));
//	listCtrl.SetItemText( itemNum,  6, _itoa(pCSOrder->SlabCondnCode(),    buf, 10));
//	listCtrl.SetItemText( itemNum,  8, _itoa(pCSOrder->NumPieces(),        buf, 10));
//	listCtrl.SetItemText( itemNum, 12, _ltoa(long(pCSOrder->SlabLength()), buf, 10));
//	listCtrl.SetItemText( itemNum,  9, _ltoa(long(pCSOrder->Tons()),       buf, 10));

	listCtrl.SetItemText( itemNum, 10, _ltoa(long(rHeat.HeatTons()),	buf, 10));
	
	listCtrl.SetItemState(itemNum,
						  INDEXTOSTATEIMAGEMASK( 1 ), 
						  LVIS_STATEIMAGEMASK);

	//listCtrl.SetItemData(itemNum,EncodeHeatIndex(heatCount));
}





void CCastStringEditorTextViewport::DecodeStrandListItem(int strandNum,
													 int listIndex,
													 int& heatIndex,
													 int& orderIndex)
{
	CCastString* pString = (IsEditing() 
							? CurrentCastString()
							: 0);

	heatIndex = -1;
	orderIndex = -1;

	if ( pString == 0 )
		return;

	CCastString::T_HeatVec& heats = pString->Heats();
	const CCastString::T_CSOrderVec& strand = pString->Strand(strandNum);

	int count = 0;

	for ( CCastString::T_HeatVec::iterator ih = heats.begin();
		  ih != heats.end();
		  ++ih ) {

		// add the number of orders in this heat.
		count += (*ih).StrandEnd(strandNum) - (*ih).StrandBegin(strandNum);
		if ( listIndex < count ) {
			// determine where we are in the strand list
			// we are at listIndex - groupIndex;
			orderIndex = listIndex - ( ih - heats.begin() );
			break;
		}
		++count;
		if ( listIndex < count ) {
			// we are on a group
			heatIndex = ih - heats.begin();
			break;
		}
	}

//	afxDump << "DecodeStrandListItem: " << strandNum 
//			<< "-" << listIndex 
//			<< " ==> " << heatIndex 
//			<< "-" << orderIndex 
//			<< "\n";

}





////////////////////////////////////////////////////////////////
//	
//	Group list
//	
////////////////////////////////////////////////////////////////

//  The available orders are controlled by the filters 
//	  set on the COrderSelection associated with the CCasterScen
//    being edited.
//
//  This includes the group/sort fields.  The group fields indicate
//	  how the selected orders are to be grouped.  
//
//  The groups are displayed in a group list.  For each group, certain
//    standard information is always displayed.  Also, the currently
//    selected grouping fields are displayed.  Thus, the header on the 
//    group list changes dynamically.
//
//  One or more groups may be selected on the group list.  The orders
//    associated with those groups are displayed in another list,
//    the groupOrderList.  This list needs to be updated whenever there
//    is a change in selected groups.
 

#ifdef CompWidth3
#undef CompWidth3
#endif
#ifdef CompWidth2
#undef CompWidth2
#endif

#ifdef DefCol
#undef DefCol
#endif

#define DefCol(format,str,wid) 	m_pListGroupsEx->InsertColumn(count,str,format,(count == 0 ? wid+10 : wid),count);

#define CompWidth3(x,label,format) ( (x) + max(m_pListGroupsEx->GetStringWidth((label)),m_pListGroupsEx->GetStringWidth((format))) )
#define CompWidth2(x,label) ( (x) + m_pListGroupsEx->GetStringWidth((label)) )


void CCastStringEditorTextViewport::SetGroupListHeader()   // Select Group quadrant window headings and column positions set in here k. hubbard
{
	
	static const char* strCI			= "CI";
	static const char* strSpec			= "Spec";
	static const char* strSlabWidth		= "SlbWd";   // tested 1-27-09 k. hubbard
	static const char* strSlabLength	= "SlLn";
	static const char* strLuType		= "LUtype";
	static const char* strPrioCode		= "Prty";
	static const char* strHrWeek		= "HRWk";
	static const char* strPstWeek		= "PSTWk";
	static const char* strCondnCode2	= "Condn";
	static const char* strNumPieces		= "#Pc";
	static const char* strNumOrders		= "#Ord";
	static const char* strTons			= "Tons";
	static const char* strTonsCB		= "Curr/Bk";
	static const char* strTonsF			= "Future";
	static const char* strCarbon		= "Carbon";
	static const char* strMn			= "Mn";
	static const char* strSignifEl		= "SignifEl";
	static const char* strCustomer		= "Customer";
	static const char* strStartTime		= "PST";

	static const int widthCI		 = CompWidth3( 15, strCI,     		"99"			);
	static const int widthSpec		 = CompWidth2( 15,					">>>999-99-99"	);
	static const int widthSlabWidth	 = CompWidth3( 15, strSlabWidth,	"99"			);  
	static const int widthSlabLength = CompWidth3( 15, strSlabLength,	"999"			);
	static const int widthLuType	 = CompWidth2( 15, strLuType						);
	static const int widthPrioCode	 = CompWidth3( 15, strPrioCode,		">>>XX-X"		);
	static const int widthHrWeek	 = CompWidth3( 15, strHrWeek,		"99"			);
	static const int widthPstWeek	 = CompWidth3( 15, strPstWeek,		"99"			);
	static const int widthCondnCode2 = CompWidth3( 15, strCondnCode2,	"99"			);
	static const int widthNumPieces	 = CompWidth3( 15, strNumPieces,	"99"			);
	static const int widthNumOrders	 = CompWidth3( 15, strNumOrders,	"99"			);
	static const int widthTons		 = CompWidth2( 15,					"9999"			);
	static const int widthTonsCB     = CompWidth2( 15,					"9999"		    );
	static const int widthTonsF      = CompWidth2( 15,					"9999"			);
	static const int widthCarbon	 = CompWidth3( 15, strCarbon,		"0.999 - 0.999"	);
	static const int widthMn		 = CompWidth3( 15, strMn,			"0.999 - 0.999"	);
	static const int widthSignifEl	 = CompWidth3( 15, strSignifEl,		"XX: 0.999 - 0.999"	);
	static const int widthCustomer	 = CompWidth3( 15, strCustomer,		"XXXXXXXXXXXXXXXXX");
	static const int widthStartTime	 = CompWidth3( 15, strStartTime,	"-9999");

	for ( int i=m_groupHeaderCount-1; i >= 0; --i )
		m_pListGroupsEx->DeleteColumn(i);

	m_groupHeaderCount = 0;
	
	if ( OrderSelection() == 0 )
		return;

	int count = 0;
	

	for ( COrderSelection::T_SortVec::const_iterator is = OrderSelection()->GroupFields().begin();
		  is != OrderSelection()->GroupFields().end();
		  ++is, ++count ) {

		switch ( (*is) ) {

		case COrderSelection::SORT_CI:
	
			DefCol(	LVCFMT_RIGHT,	strCI,			widthCI			);
			break;

		case COrderSelection::SORT_SPEC:
	
			DefCol(	LVCFMT_RIGHT,	strSpec,		widthSpec		);
			break;

		case COrderSelection::SORT_WIDTH:

			DefCol(	LVCFMT_RIGHT,	strSlabWidth,		widthSlabWidth	);
			break;

		case COrderSelection::SORT_SLAB_LENGTH:

			DefCol(LVCFMT_RIGHT,	strSlabLength,		widthSlabLength	);
			break;
	
		case COrderSelection::SORT_80HSM_LU_TYPE:

			DefCol( LVCFMT_LEFT,	strLuType,		widthLuType		);
			break;

		case COrderSelection::SORT_PRIORITY_CODE:

			DefCol( LVCFMT_RIGHT,	strPrioCode,	widthPrioCode	);
			break;

		case COrderSelection::SORT_CONDN_CODE2:

			DefCol( LVCFMT_RIGHT,	strCondnCode2,	widthCondnCode2	);
			break;

		case COrderSelection::SORT_HR_WEEK:

			DefCol( LVCFMT_RIGHT,	strHrWeek,		widthHrWeek		);
			break;

		case COrderSelection::SORT_PST_DELTA_WEEK:

			DefCol( LVCFMT_RIGHT,	strPstWeek,		widthPstWeek	);
			break;

		case COrderSelection::SORT_CARBON:

			DefCol( LVCFMT_RIGHT,	strCarbon,		widthCarbon		);
			break;

		case COrderSelection::SORT_MANGANESE:

			DefCol( LVCFMT_RIGHT,	strMn,			widthMn			);
			break;

		case COrderSelection::SORT_SIGNIF_EL:

			DefCol( LVCFMT_RIGHT,	strSignifEl,	widthSignifEl	);
			break;

		case COrderSelection::SORT_CUSTOMER:

			DefCol( LVCFMT_RIGHT,	strCustomer,	widthCustomer	);
			break;

		case COrderSelection::SORT_PLANNED_START:

			DefCol( LVCFMT_RIGHT,	strStartTime,	widthStartTime	);
			break;
		}
	}


	DefCol( LVCFMT_RIGHT,	strNumPieces,	widthNumPieces	);  
	++count;

	DefCol( LVCFMT_RIGHT,	strNumOrders,	widthNumOrders	);
	++count;

	DefCol( LVCFMT_RIGHT,	strTons,		widthTons		);	
	++count;

	DefCol( LVCFMT_RIGHT,	strTonsCB,		widthTonsCB     );
	++count;

	DefCol( LVCFMT_RIGHT,	strTonsF,		widthTonsF		);
	++count;

	m_groupHeaderCount = count;
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

#define DefCol(field) \
		{/*afxDump << "Inserting field " << field << " col# " << colNum << ": " << strings[field] << "\n";*/\
		m_pListOrdersEx->InsertColumn(colNum,			\
		                          strings[field],	\
								  LVCFMT_RIGHT,		\
								  widths[field],	\
								  colNum);			\
		m_availOrderListFieldIndexes[field] = colNum;	\
		++colNum;}


#define CompWidth3(x,label,format) ( (x) + max(m_pListOrdersEx->GetStringWidth((label)),m_pListOrdersEx->GetStringWidth((format))) )
#define CompWidth2(x,label) ( (x) + m_pListOrdersEx->GetStringWidth((label)) )


void CCastStringEditorTextViewport::SetGroupOrdersListHeader()
{

	static vector<CString> strings;
	static vector<int> widths;
	static vector<int> mapping;
	bool initialized = false;

	if ( ! initialized ) {
		
		initialized = true;
		
		strings.resize(NumAvailOrderFields);
		widths.resize(NumAvailOrderFields);

		strings[AVAIL_ORDER_FIELD_CI]			= "CI";
		strings[AVAIL_ORDER_FIELD_SPEC]			= "Spec";
		strings[AVAIL_ORDER_FIELD_PRIO]			= "Prty";
		strings[AVAIL_ORDER_FIELD_MEGA_LU]		= "MegaLU";
		strings[AVAIL_ORDER_FIELD_ORDER_NUM]	= "Order#";
		strings[AVAIL_ORDER_FIELD_CUST]			= "Cust";
		strings[AVAIL_ORDER_FIELD_LENGTH]		= "Lnth";
		strings[AVAIL_ORDER_FIELD_HB_WIDTH]		= "HBWd";
		strings[AVAIL_ORDER_FIELD_SLAB_WIDTH]	= "SlWd";
		strings[AVAIL_ORDER_FIELD_NUM_PIECES]	= "#Pc";
		strings[AVAIL_ORDER_FIELD_TONS]			= "Tons";
		strings[AVAIL_ORDER_FIELD_WEIGHTS]		= "Wgt:IND/MIN/MAX";
		strings[AVAIL_ORDER_FIELD_CONDN_CODE]	= "Condn";
//		strings[AVAIL_ORDER_FIELD_HR_WEEK]		= "HRWk";   comm out and switched to below 2-08-05 k. hubbard
		strings[AVAIL_ORDER_FIELD_HR_WEEK]		= "DmandWk";
//		strings[AVAIL_ORDER_FIELD_PST_WEEK]		= "PSTWk";
		strings[AVAIL_ORDER_FIELD_PLANNED_START]= "PST";
		strings[AVAIL_ORDER_FIELD_CARBON]		= "Carbon";
		strings[AVAIL_ORDER_FIELD_MANGANESE]	= "Mn";
		strings[AVAIL_ORDER_FIELD_SIGNIF_EL]	= "Signif";

		widths[AVAIL_ORDER_FIELD_CI]			= CompWidth2( 15, "99"										);
		widths[AVAIL_ORDER_FIELD_SPEC]			= CompWidth2( 15, ">>>999-99-99"								);
		widths[AVAIL_ORDER_FIELD_PRIO]			= CompWidth2( 15, ">>>99-9"									);
		widths[AVAIL_ORDER_FIELD_MEGA_LU]		= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_MEGA_LU]		);
		widths[AVAIL_ORDER_FIELD_ORDER_NUM]		= CompWidth2( 25, ">>>99999999-MMMMM"								);
		widths[AVAIL_ORDER_FIELD_CUST]			= CompWidth2( 15, "MMMMMMMM"								);
		widths[AVAIL_ORDER_FIELD_LENGTH]		= CompWidth2( 15, "999"										);
		widths[AVAIL_ORDER_FIELD_HB_WIDTH]		= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_HB_WIDTH]		);
		widths[AVAIL_ORDER_FIELD_SLAB_WIDTH]	= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_SLAB_WIDTH]		);
		widths[AVAIL_ORDER_FIELD_NUM_PIECES]	= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_NUM_PIECES]		);
		widths[AVAIL_ORDER_FIELD_TONS]			= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_TONS]			);
		widths[AVAIL_ORDER_FIELD_WEIGHTS]		= CompWidth2( 15, "9999 - 9999 - 9999"						);
		widths[AVAIL_ORDER_FIELD_CONDN_CODE]	= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_CONDN_CODE]		);
		widths[AVAIL_ORDER_FIELD_HR_WEEK]		= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_HR_WEEK]		);
		widths[AVAIL_ORDER_FIELD_PLANNED_START]	= CompWidth2( 15, "-9999"	);
//		widths[AVAIL_ORDER_FIELD_PST_WEEK]		= CompWidth2( 15, strings[AVAIL_ORDER_FIELD_PST_WEEK]	);
		widths[AVAIL_ORDER_FIELD_CARBON]		= CompWidth2( 15, "9.999-9.999"								);
		widths[AVAIL_ORDER_FIELD_MANGANESE]		= CompWidth2( 15, "9.999-9.999"								);
		widths[AVAIL_ORDER_FIELD_SIGNIF_EL]		= CompWidth2( 15, "XX: 9.999-9.999"							);

		//  This vector maps COrderSelection::SortFields to AvailOrderListFields
		mapping.resize(COrderSelection::NumSortCodes);
		mapping[COrderSelection::SORT_CI]				= AVAIL_ORDER_FIELD_CI;
		mapping[COrderSelection::SORT_SPEC]				= AVAIL_ORDER_FIELD_SPEC;
		mapping[COrderSelection::SORT_WIDTH]			= AVAIL_ORDER_FIELD_SLAB_WIDTH;
		mapping[COrderSelection::SORT_80HSM_LU_TYPE]	= AVAIL_ORDER_FIELD_MEGA_LU;
		mapping[COrderSelection::SORT_PRIORITY_CODE]	= AVAIL_ORDER_FIELD_PRIO;
		mapping[COrderSelection::SORT_HR_WEEK]			= AVAIL_ORDER_FIELD_HR_WEEK;
//		mapping[COrderSelection::SORT_PST_DELTA_WEEK]	= AVAIL_ORDER_FIELD_PST_WEEK;
		mapping[COrderSelection::SORT_CONDN_CODE2]		= AVAIL_ORDER_FIELD_CONDN_CODE;
		mapping[COrderSelection::SORT_PLANNED_START]	= AVAIL_ORDER_FIELD_PLANNED_START;
		mapping[COrderSelection::SORT_SLAB_LENGTH]		= AVAIL_ORDER_FIELD_LENGTH;
		mapping[COrderSelection::SORT_CARBON]			= AVAIL_ORDER_FIELD_CARBON;
		mapping[COrderSelection::SORT_MANGANESE]		= AVAIL_ORDER_FIELD_MANGANESE;
		mapping[COrderSelection::SORT_SIGNIF_EL]		= AVAIL_ORDER_FIELD_SIGNIF_EL;
		mapping[COrderSelection::SORT_CUSTOMER]			= AVAIL_ORDER_FIELD_CUST;

	}



	//	We have to compute the layout of columns.
	//	The computed positions are stored in an array.
	//  Indexes in the array indicate the column.
	//	The AVAIL_ORDER_FIELD_xxx constants are the columns.
	//  The order is as follows:
	//    
	//	0 = AVAIL_ORDER_FIELD_ORDER_NUM
	//	1 = AVAIL_ORDER_FIELD_NUM_PIECES,
	//	2 = AVAIL_ORDER_FIELD_TONS,
	//
	//  Among 
	//		AVAIL_ORDER_FIELD_CI,
	//		AVAIL_ORDER_FIELD_SPEC,
	//		AVAIL_ORDER_FIELD_PRIO,
	//		AVAIL_ORDER_FIELD_MEGA_LU,
	//		AVAIL_ORDER_FIELD_LENGTH,
	//		AVAIL_ORDER_FIELD_SLAB_WIDTH,
	//		AVAIL_ORDER_FIELD_CONDN_CODE,
	//		AVAIL_ORDER_FIELD_HR_WEEK,
//	//      AVAIL_ORDER_FIELD_PST_WEEK,
	//		AVAIL_ORDER_FIELD_PLANNED_START,
	//		AVAIL_ORDER_FIELD_CARBON
	//		AVAIL_ORDER_FIELD_MANGANESE
	//		AVAIL_ORDER_FIELD_SIGNIF_EL
	//		AVAIL_ORDER_FIELD_CUST
	//
	//	the fields are chosen in the order given by the sort fields
	//      looking first at the group sorts, then the avail order sorts,
	//  if the field does not occur in either sort, then it appears after the others.
	//	
	//	we also toss in some really kludgy code to make
	//		AVAIL_ORDER_FIELD_HB_WIDTH
	//  come next to AVAIL_ORDER_FIELD_SLAB_WIDTH
	//
	//	At the end are:
	//		AVAIL_ORDER_FIELD_WEIGHTS,
	//
	//  Method:  
	//
	//		we initialize the array entries to -1.
	//		Set the 0,1,2 positions
	//      Run through entries in the order sort and group sort
	//          and create a sequence of AVAIL_ORDER_FIELD_xxx entries.
	//      Set the array entries at those positions sequentially.
	//      Set the array entries still -1 sequentially.
	
	{
		//  THis loop begins one greater than you'd think,
		//   because we add a seond AVAIL_ORDER_FIELD_ORDER_NUM

		for ( int i=NumAvailOrderFields; i >= 0; --i )
			m_pListOrdersEx->DeleteColumn(i);
	}


	{
		for ( int i=0; i<NumAvailOrderFields; ++i )
			m_availOrderListFieldIndexes[i] = -1;
	}

	int colNum = 0;

	DefCol(AVAIL_ORDER_FIELD_ORDER_NUM)	// colNum = 0
	DefCol(AVAIL_ORDER_FIELD_NUM_PIECES)	// colNum = 1;
	DefCol(AVAIL_ORDER_FIELD_TONS)		// colNum = 2;

	
	{
		COrderSelection::T_SortVec combined;

		if ( OrderSelection() != 0 ) {
			combined.insert(combined.end(),
							OrderSelection()->GroupFields().begin(),
							OrderSelection()->GroupFields().end());
			combined.insert(combined.end(),
							OrderSelection()->SortFields().begin(),
							OrderSelection()->SortFields().end());
		}

#if 0
		{
			afxDump << "Group fields size = " << OrderSelection()->GroupFields().size() << "\n";
			afxDump << "Sort fields size = " << OrderSelection()->SortFields().size() << "\n";

			for ( COrderSelection::T_SortVec::const_iterator is = combined.begin();
				  is != combined.end();
				  ++is ) 

				afxDump << "Sort " << (*is) << " --> " << mapping[(*is)] << "\n";
		}
#endif


		for ( COrderSelection::T_SortVec::const_iterator is = combined.begin();
			  is != combined.end();
			  ++is ) {

			// we want an extra OrderNum column at position 8
			if ( colNum == 8 )
				DefCol(AVAIL_ORDER_FIELD_ORDER_NUM)	

			int j = mapping[(*is)];
			if ( m_availOrderListFieldIndexes[j] == -1 ) {
				DefCol(j)
				if ( j == AVAIL_ORDER_FIELD_SLAB_WIDTH ) 
					DefCol(AVAIL_ORDER_FIELD_HB_WIDTH)
			}
		}
	}

	{
		for ( int i=0; i<NumAvailOrderFields; ++i ) {
		
			// we want an extra OrderNum column at position 8
			if ( colNum == 8 )
				DefCol(AVAIL_ORDER_FIELD_ORDER_NUM)	

			if (i != AVAIL_ORDER_FIELD_HB_WIDTH
				&&
				m_availOrderListFieldIndexes[i] == -1 ) {

				DefCol(i)

				if ( i == AVAIL_ORDER_FIELD_SLAB_WIDTH ) 
					DefCol(AVAIL_ORDER_FIELD_HB_WIDTH)

			}
		}
	}

#if 0
	{
		for ( int i=0; i<NumAvailOrderFields; ++i ) 
			afxDump << "Field " << i
					<< " ("  << strings[i] 
					<< ") (" << widths[i]
					<< ") --> index " << m_availOrderListFieldIndexes[i] << "\n";
	}
#endif

}






static void SetItem(CListCtrl& ctrl, int nItem, int nSubItem, LPCTSTR lpszText)
{

	if ( nSubItem == 0 )

		ctrl.InsertItem(nItem,lpszText);

	else

		ctrl.SetItemText(nItem,nSubItem,lpszText);
}




void CCastStringEditorTextViewport::OnGetdispinfoListGroups(NMHDR* pNMHDR) // Select Groups for Sorting window, upper right hand corner field display adjustments done here, 2-24-09 k. hubbard 
{                                                      // Select Groups for Sorting window, upper right hand corner field display adjustments done in here.
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;


	LVITEM& lvi = pDispInfo->item;

	//  Determine which group (row) and which field (column) is being requested
	//  Make sure this makes sense


	COrderSelection* os = OrderSelection();
	if ( os == 0 ) {
		return;
	}

	if ( lvi.iItem >= OrderSelection()->Groups().size() ) {
		static bool fired = false;
#ifdef _DEBUG
		afxDump << "Look for group item # " << int(lvi.iItem) << ", but only "
				<< int(OrderSelection()->Groups().size()) << " items.\n";
#endif
		if (!fired)
			View()->MessageBox("Report to Miller: Message #112");
		fired = true;
		lstrcpyn(lvi.pszText,"?????",lvi.cchTextMax);
		return;
	}

	COrderSelection::T_GroupVec::const_iterator ig = OrderSelection()->Groups().begin() + lvi.iItem;

	// ## add int cast
	int numGroupFields = (int)OrderSelection()->GroupFields().size();
	
	int sf;

	if ( lvi.iSubItem <  numGroupFields )
		//  This subitem corresponds to one of the group fields
		//  Determine which field by looking in the vector of group fields.
		sf = OrderSelection()->GroupFields()[lvi.iSubItem];
	else
		//  This subitem corresponds to one of the fields after the group fields.
		//  These are determined by an offset from NumSortCodes
		sf = COrderSelection::NumSortCodes + (lvi.iSubItem - numGroupFields);


	COrder* pOrder = *((*ig)->GroupBegin());



	if (  lvi.mask & LVIF_TEXT ) {

		char buf[100];
		ostrstream ostr(buf,100);


		switch ( sf ) {
	
		case COrderSelection::SORT_CI:
			_itoa(pOrder->CICode(),buf,10);
			break;
	
		case COrderSelection::SORT_SPEC:
				
			{
				CString rightDig = pOrder->CastSpec().Right(1);
	
				ostr << ( rightDig == "2" || rightDig == "5" ? ">>>" : "   ")
					<< LPCTSTR(pOrder->CastSpec().Left(3))
					<< "-"
					<< LPCTSTR(pOrder->CastSpec().Mid(3,2))
					<< "-"
					<< LPCTSTR(pOrder->CastSpec().Right(2))
					<< ends;
			}
			break;
	
		case COrderSelection::SORT_WIDTH:
	
//			ostr << setw(2) << long(pOrder->SlabWidth()) << ends;  // original 02-09-09 k. hubbard
//  works too but only if significant digit decimal > 0 ( i.e. widith = .5	ostr << setw(3) << double(pOrder->SlabWidth()) << ends;  // test 02-09-09 k. hubbard
//	Combination below changes all slab widths significant or not to show decimal value of tenths 02-24-09 k. hubbard
			ostr << fixed << setprecision(1) << (pOrder->SlabWidth()) << ends;  // successfully tested 02-24-09 k. hubbard
			break;

		case COrderSelection::SORT_SLAB_LENGTH:
	
			ostr << setw(3) << long(pOrder->SlabLength()) << ends;
			break;
	
		case COrderSelection::SORT_80HSM_LU_TYPE:
	
			strcpy(buf,pOrder->MegaLuCd());
			break;
	
	
		case COrderSelection::SORT_CUSTOMER:
			
			strcpy(buf,pOrder->CustName());
			break;
	
		case COrderSelection::SORT_PRIORITY_CODE:
					
			ostr.fill('0');
			ostr << ( pOrder->CommodityPriority() == 99 ? ">>>" : "   ")
				 << setw(2) << pOrder->CommodityPriority()
				 << "-"
				 << pOrder->OrderPriority()
				 << ends;
			ostr.fill(' ');
			break;
		
		case COrderSelection::SORT_CONDN_CODE2:
	
			_ltoa(long(pOrder->SlabCondnCode()/100),buf,10);
			break;
	
		case COrderSelection::SORT_HR_WEEK:
	
			_ltoa(long(pOrder->HrYrWk().Week()),buf,10);
			break;
	
		case COrderSelection::SORT_PST_DELTA_WEEK:
	
			_ltoa(long(NTime::PSDateDeltaWeekTruncated(pOrder->PlannedLatestStartDate())),buf,10);
			break;

		case COrderSelection::SORT_PLANNED_START:
	
			ostr << setw(4) << NTime::PSDateDelta(pOrder->PlannedLatestStartDate())
				 << ends;
			break;
	
	
		case COrderSelection::SORT_CARBON:
		case COrderSelection::SORT_MANGANESE:
		case COrderSelection::SORT_SIGNIF_EL:
			{
				CChem::Element elt;
	
				if ( sf  == COrderSelection::SORT_CARBON )
					elt = CChem::ELT_C;
				else if ( sf  == COrderSelection::SORT_MANGANESE )
					elt = CChem::ELT_MN;
				else {
					if ( ! CChem::ToElement(pOrder->SignifElSym(),elt) ) {
						strcpy(buf,"???");
						break;
					}
				}
	
	
				if ( sf == COrderSelection::SORT_SIGNIF_EL )
					ostr << setw(2) << LPCTSTR(pOrder->SignifElSym()) << ": ";
					ostr.fill('0');
					ostr << setw(5) << setprecision(3) << pOrder->MinElement(elt)
						 << " - "
						 << setw(5) << setprecision(3) << pOrder->MaxElement(elt)
						 << ends;
					ostr.fill(' ');
			}
			break;
	
		case COrderSelection::NumSortCodes:
		
			_itoa((*ig)->NumPieces(),buf,10);
			break;
	
		case COrderSelection::NumSortCodes+1:
	
			_ltoa(long((*ig)->NumOrders()),buf,10);
			break;
	
		case COrderSelection::NumSortCodes+2:
	
			_ltoa(long((*ig)->Tons()),buf,10);
			break;
	
		case COrderSelection::NumSortCodes+3:
	
			_ltoa(long((*ig)->CurrentBackTons()),buf,10);
			break;
	
		case COrderSelection::NumSortCodes+4:
	
			_ltoa(long((*ig)->FutureTons()),buf,10);
			break;

		default:
			sprintf(buf,"!$%?^@*");
			break;
		}
	
		
		lstrcpyn(lvi.pszText,buf,lvi.cchTextMax);
	}
	
	if ( lvi.iSubItem == 0 ) {

		if ( lvi.mask & LVIF_IMAGE ) {
			lvi.iImage = 0;
		}

		if ( lvi.mask & LVIF_INDENT ) {
			lvi.iIndent = 0;
		}
	}
}


void CCastStringEditorTextViewport::SetGroupListItems(bool maintainPosition)
{
	CMaintainListCtrlPosn x( *m_pListGroupsEx, maintainPosition );
	SetGroupListHeader();

	if ( OrderSelection() == 0 )
		return;

#ifdef _DEBUG
	afxDump << "Setting list groups count to " << int(OrderSelection()->Groups().size()) << "\n";
#endif

	// ## add int cast
	m_pListGroupsEx->SetItemCount((int)OrderSelection()->Groups().size());
	SetGroupListSelections();

	m_pListGroupsEx->UpdateWindow();
}



void CCastStringEditorTextViewport::SetGroupListSelections()
{
	// unselect all items.

	for ( int i = m_pListGroupsEx->GetNextItem(-1,LVNI_SELECTED);
		  i != -1;
	      i = m_pListGroupsEx->GetNextItem(i,LVNI_SELECTED) ) 
		m_pListGroupsEx->SetItemState(i,0,LVIS_SELECTED);


	if ( OrderSelection() == 0 ) 
		return;

	for ( COrderSelection::T_GroupVec::const_iterator is = OrderSelection()->SelectedGroups().begin();
		  is != OrderSelection()->SelectedGroups().end();
		  ++is ) {
 
		COrderSelection::T_GroupVec::const_iterator ifind = find(OrderSelection()->Groups().begin(),
															   OrderSelection()->Groups().end(),
																(*is));

		if ( ifind != OrderSelection()->Groups().end() ) {
			
			int index = ifind - OrderSelection()->Groups().begin();
			m_pListGroupsEx->SetItemState(index,LVIS_SELECTED,LVIS_SELECTED);

		}
	}
}

	

void CCastStringEditorTextViewport::SetGroupOrdersListItems()
{
	CMaintainListCtrlPosn x(*m_pListOrdersEx,true);

	m_pListOrdersEx->DeleteAllItems();

	SetGroupOrdersListHeader();


#if 0
	{
		for ( int i=0; i<NumAvailOrderFields; ++i ) 
			afxDump << "Field " << i 
			        << " --> index " 
					<< availOrderListFieldIndexes[i] << "\n";
	}
#endif

	ostrstream ostr;

	ostr.setf(ios_base:: fixed, ios_base:: floatfield);

	if (OrderSelection() == 0
		||
		OrderSelection()->SelectedOrders().size() == 0 ) {

	   m_pListOrdersEx->InsertItem(0,"No selection");
	   return;
	
	}

	COLORREF ColorRed = RGB(255,0,0);   // red order color test 12-1-05 k.hubbard
	COLORREF  defaultColor = m_pListOrdersEx->GetTextColor();

	int count = 0;
	for ( T_OrderVec::const_iterator io = OrderSelection()->SelectedOrders().begin();
		  io != OrderSelection()->SelectedOrders().end();
		  ++io, ++count )

			  try {		               // red order color test 12-1-05 k.hubbard
		 
		AddGroupOrderListItem(count,(*io),ostr,OrderSelection()->SelectedOrderIsSpecial(count));   // Available Orders quadrant window List Control field settings: lower right hand corner k. hubbard
// Test area begin: 12-1-05 k. hubbard

//			try {	 
//				AddGroupOrderListItem(count,(*io),ostr,OrderSelection()->SelectedOrderIsSpecial(count));

//			  COLORREF defaultColor = m_pListOrdersEx->GetTextColor();   // red order color test 12-1-05 k.hubbard
//			try {	 

				for (int ix=0; ix<=count; ++ix) {
	
					if ((*io)->CriticalCustInd() == 'Y') { 
						m_pListOrdersEx->SetTextColor(ColorRed);   // red works here too, order color test 12-1-05 k.hubbard
						m_pListOrdersEx->RedrawItems(count,count);
						m_pListOrdersEx->UpdateWindow();
						m_pListOrdersEx->SetTextColor(defaultColor);
					}
				}
			}

				catch ( CDBException* e )
					{
					   AfxMessageBox( e->m_strError,   
		               MB_ICONEXCLAMATION );
					   e->Delete();
					}
// Test area end: 12-1-05 k. hubbard

}



void CCastStringEditorTextViewport::AddGroupOrderListItem(int count,	// Available Orders quadrant window List Control field settings: lower right hand corner k. hubbard
													  COrder* pOrder,
													  ostrstream& ostr,
													  bool isSpecial)
{
	char buf[100];
	COLORREF ColorRed = RGB(255,0,0);   // red order color test 2-18-05 k.hubbard
	COLORREF  defaultColor = m_pListOrdersEx->GetTextColor();


	// We handle column 0 separately,
	//   because we have to do InsertItem instead of 

	// Col  0 -- OrderNum
	// Col  8 -- OrderNum

	// FP change
	//sprintf(buf,"%s%07d",
	//		( pOrder->OrderNum()/10000%10 == 0 // 4th digit
	//		  ? ">>>"
	//		  : "   "),
	//		pOrder->OrderNum());
	sprintf(buf,"%s%s",
			( pOrder->FpOrderNum().GetLength() >= 5 && pOrder->FpOrderNum()[4] == '0' // 4th digit
			  ? ">>>"
			  : "   "),
			LPCTSTR(pOrder->FpOrderNum()));

	
//			red order color test 10-20-05 k.hubbard
//			if (pOrder->FpMtoFlowOrdInd() == 'M') { 
//   VERY IMPORTANT !!!!! COLOR WILL NOT WORK IN DEBUG WITH BREAKPOINTS INSIDE OR AT IF STATEMENT LINE!!!!!!!!! 
//  	Will intefere with physical redraw and update DO NOT USE BREAKPOINTS HERE 11-1-05 k. hubbard
//	for (int ix=0; ix<count; ++ix) {
////		if (pOrder->CriticalCustInd() == 'Y') { 
////			m_pListOrdersEx->SetTextColor(RGB(255,0,0));   // red works here too, order color test 11-1-05 k.hubbard
////			m_pListOrdersEx->RedrawItems(count,count);
////		    m_pListOrdersEx->UpdateWindow();
////			m_pListOrdersEx->SetTextColor(defaultColor);   // reset to default color test 11-1-05 k.hubbard
////		}

//	if ( m_pListOrdersEx->uNewState & LVIS_FOCUSED ) {
//		GetViewport()->UpdateSelectedGroups();
//		GetViewport()->SetGroupOrdersListItems();
//	}


	m_pListOrdersEx->InsertItem(count,buf);
	m_pListOrdersEx->SetItemText(count,8,buf);

	//	Spec

				
	{ 
		CString rightDig = pOrder->OrderedSpec().Right(1);

		ostr << ( rightDig == "2" || rightDig == "5" ? ">>>" : "   ")
			 << (LPCTSTR)pOrder->OrderedSpec().Left(3)
			 << "-"
			 << (LPCTSTR)pOrder->OrderedSpec().Mid(3,2)
			 << "-"
			 << (LPCTSTR)pOrder->OrderedSpec().Right(2)
			 << ends;
	}

	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_SPEC],
							 ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	//	CI Code
	m_pListOrdersEx->SetItemText(count, 
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_CI],
							 _itoa(pOrder->CICode(),buf,10));

	//	Condn code
	m_pListOrdersEx->SetItemText(count, 
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_CONDN_CODE],
							 _itoa(pOrder->SlabCondnCode(),buf,10));

	//	HR Week
	m_pListOrdersEx->SetItemText(count, 
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_HR_WEEK],
							 _itoa(pOrder->HrYrWk().Week(),buf,10));

//	m_pListOrdersEx->SetItemText(count, 
//							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_PST_WEEK],
//							 _itoa(NTime::PSDateDeltaWeekTruncated(pOrder->PlannedLatestStartDate()),buf,10));

	//	Priority
	ostr << (pOrder->CommodityPriority() == 99 ? ">>>" : "   ")
		 << setw(2) << pOrder->CommodityPriority()
		 << "-" 
		 << setw(1) << pOrder->OrderPriority()
		 
		 << ends;


	m_pListOrdersEx->SetItemText(count, 
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_PRIO],
							 ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);


	//	#Pieces
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_NUM_PIECES],
							 _itoa(SuperScen()->NumSlabsDue(pOrder),buf,10));

	//	Tons
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_TONS],
							 _ltoa(long(SuperScen()->NumSlabsDue(pOrder)*pOrder->IndivSlabWgt()/2000),buf,10));

	//	Length
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_LENGTH],
							 _ltoa(long(pOrder->SlabLength()),buf,10));

	//	HBWidth
	ostr << setw(4) << setprecision(1) << pOrder->HbWidth() << ends;
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_HB_WIDTH],
							 ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	//	Slab width
	ostr << setw(2) << pOrder->SlabWidth() << ends;
//	ostr << setw(4) << setprecision(1) << pOrder->SlabWidth() << ends;  // 9-13-07 k.hubbard testing affects Available Order window display only.


	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_SLAB_WIDTH],
							 ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	//	Custname
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_CUST],
							 pOrder->CustName().Left(10));

	//	Weight
	ostr <<	setw(3) << long(pOrder->IndivSlabWgt()/100)
		 << " - " 
		 << setw(3) << long(pOrder->MinSlabWgt()/100)
		 << " - "
		 << setw(3) << long(pOrder->MaxSlabWgt()/100)
		 << ends;

	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_WEIGHTS],
							 ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	//	MegaLU
	m_pListOrdersEx->SetItemText(count,
							 m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_MEGA_LU],
							 pOrder->MegaLuCd());


	//  Carbon
	ostr.fill('0');
	ostr << setw(5) << setprecision(3) << pOrder->MinElement(CChem::ELT_C)
		 << " - "
		 << setw(5) << setprecision(3) << pOrder->MaxElement(CChem::ELT_C)
		 << ends;
	m_pListOrdersEx->SetItemText(count,
							   m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_CARBON],
							   ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);
	ostr.fill(' ');

	//  Manganese
	ostr.fill('0');
	ostr << setw(5) << setprecision(3) << pOrder->MinElement(CChem::ELT_MN)
		 << " - "
		 << setw(5) << setprecision(3) << pOrder->MaxElement(CChem::ELT_MN)
		 << ends;
	m_pListOrdersEx->SetItemText(count,
							   m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_MANGANESE],
							   ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);
	ostr.fill(' ');

	//  Signif el
	CChem::Element elt;
	if ( ! CChem::ToElement(pOrder->SignifElSym(),elt) ) 
		ostr << "???" << ends;
	else {
			ostr << setw(2) << LPCTSTR(pOrder->SignifElSym()) << ": ";
			ostr.fill('0');
			ostr << setw(5) << setprecision(3) << pOrder->MinElement(elt)
				 << " - "
				 << setw(5) << setprecision(3) << pOrder->MaxElement(elt)
				 << ends;
	}
				
	m_pListOrdersEx->SetItemText(count,
							   m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_SIGNIF_EL],
							   ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);
	ostr.fill(' ');

	//ostr.fill('0');
	//ostr << setw(8) << pOrder->PlannedLatestStartDate()
	//	 << ends;
	ostr << setw(4) << NTime::PSDateDelta(pOrder->PlannedLatestStartDate())
		 << ends;
	m_pListOrdersEx->SetItemText(count,
							   m_availOrderListFieldIndexes[AVAIL_ORDER_FIELD_PLANNED_START],
							   ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);
	//ostr.fill(' ');
	

	m_pListOrdersEx->SetItemState(count,
						      INDEXTOSTATEIMAGEMASK( isSpecial ? 2 : 1 ), 
							  LVIS_STATEIMAGEMASK);

}





////////////////////////////////////////////////////////////////
//	
//	Find selections
//	
////////////////////////////////////////////////////////////////

//  We have four lists on which selections can be done
//  and from which we need to form an indication of the selections.
//
//  The lists are:
//
//		m_strand1OrderListEx
//		m_strand2OrderListEx
//		m_listGroupsEx
//		m_listOrdersEx
//
//  From the strands, we are interested in getting indexes into the list.
//		We do this by iterating through the selected items in the list.
//
//	From the groups list, we want the orders associated with the selected groups.
//		All selections from this list are fed back to the underlying COrderSelection object
//      so we get the data from there.       

//  From the orders list, we want the orders that are selected.
//      We need also to iterate through the selected items in the list,
//      but we need to identify the corresponding orders.  
//		The only way to do that is to first iterate throught the selected groups (see above)
//      and then determine which of their orders are selected.


void CCastStringEditorTextViewport::GetSelectedCSOrderIndexes(vector<int>& vec, 
														  int strandNum)
{
	// this used for strand1 vs strand2, by the controller
	GetSelectedCSOrderIndexes(vec,
							  (strandNum == 1 
								? m_pStrand1OrderListEx
								: m_pStrand2OrderListEx ),
							  strandNum);
}

void CCastStringEditorTextViewport::GetSelectedCSOrderIndexes(vector<int>& vec, 
														  CListCtrl* pList, 
														  int strandNum)
{
	vec.erase(vec.begin(),vec.end());

	int index = pList->GetNextItem(-1,LVNI_SELECTED);
	int heatIndex;
	int orderIndex;
	
	while ( index != -1 ) {
		//DecodeStrandListItem(pList,index,heatIndex,orderIndex);
		DecodeStrandListItem(strandNum,index,heatIndex,orderIndex);
		if ( orderIndex >= 0 )
			vec.push_back(orderIndex);
		index = pList->GetNextItem(index,LVNI_SELECTED);
	}
}


void CCastStringEditorTextViewport::GetOrdersForSelectedGroups(vector<COrder*>& selectedOrders)
{
	selectedOrders.clear();

	if ( OrderSelection() == 0 )
		return;

	for ( COrderSelection::T_GroupVec::const_iterator ig = OrderSelection()->SelectedGroups().begin();
		  ig != OrderSelection()->SelectedGroups().end();
		  ++ig ) 

		for ( T_OrderVec::const_iterator iv = (*ig)->GroupBegin();
			  iv != (*ig)->GroupEnd();
			  ++iv) 
				  
			selectedOrders.push_back((*iv));
}


void CCastStringEditorTextViewport::GetSelectedOrders(vector<COrder*>& selectedOrders)
{
	selectedOrders.clear();

	if ( OrderSelection() == 0 )
		return;

	const T_OrderVec& orderVec = OrderSelection()->SelectedOrders();

	int count = 0;
	for ( T_OrderVec::const_iterator io = orderVec.begin();
		  io != orderVec.end();
		  ++io, ++count )
		 
		if ( m_pListOrdersEx->GetItemState(count,LVIS_SELECTED) )
			selectedOrders.push_back(const_cast<COrder*>(*io));
}



//  Update the underlying COrderSelection to reflect the currently selected groups

void CCastStringEditorTextViewport::UpdateSelectedGroups()
{
	if ( OrderSelection() == 0 )
		return;

	vector<int> indexes;

	int index = m_pListGroupsEx->GetNextItem(-1,LVNI_SELECTED);
	
	while ( index != -1 ) {
			indexes.push_back(index);
			index = m_pListGroupsEx->GetNextItem(index,LVNI_SELECTED);
	}

	OrderSelection()->SetSelectedGroups(indexes,SuperScen());
}




//////////////////////////////////////
//
//  Commands
//
//////////////////////////////////////


//	The following three methods 
//		(OnButtonGroup1, OnButtonGroup2, OnButtonUpdateGrouping)
//	correspond to: (4) Change in sort/group criteria


// Two buttons are available for standardized grouping

void CCastStringEditorTextViewport::OnButtonGroup1() 
{
	if ( OrderSelection() == 0 )
		return;

	static vector<int> groupFields;

	if ( groupFields.size() == 0 ) {
		groupFields.push_back(COrderSelection::SORT_SPEC);
		groupFields.push_back(COrderSelection::SORT_WIDTH);
		groupFields.push_back(COrderSelection::SORT_CI);
	}

	OrderSelection()->SetGroupFields(groupFields,SuperScen());

	OrderSelection()->Init(SuperScen());

	View()->m_availGroupFields	= OrderSelection()->AvailGroupFields();	// copy
	View()->m_groupFields		= OrderSelection()->GroupFields();		// copy

	View()->UpdateData(false);

	SetGroupOrdersListItems();
	SetGroupListItems();

}


// TODO -- remove duplicate redundancy code

void CCastStringEditorTextViewport::OnButtonGroup2() 
{

	if ( OrderSelection() == 0 )
		return;

	static vector<int> groupFields;

	if ( groupFields.size() == 0 ) {
		groupFields.push_back(COrderSelection::SORT_WIDTH);
		groupFields.push_back(COrderSelection::SORT_SPEC);
		groupFields.push_back(COrderSelection::SORT_CI);
	}

	OrderSelection()->SetGroupFields(groupFields,SuperScen());
	OrderSelection()->Init(SuperScen());


	View()->m_availGroupFields	= OrderSelection()->AvailGroupFields();	// copy
	View()->m_groupFields		= OrderSelection()->GroupFields();		// copy

	View()->UpdateData(false);

	SetGroupOrdersListItems();
	SetGroupListItems();

}




//   The user clicks on a button when a change to the grouping/sorting
//      is to be applied.


void CCastStringEditorTextViewport::OnButtonUpdateGrouping() 
{
	// transfer data from list controls down to local lists

	View()->UpdateData(true);

	// transfer data from local lists to orderSelection lists

	OrderSelection()->SetGroupFields(View()->m_groupFields,SuperScen());


	SetGroupOrdersListItems();
	SetGroupListItems();
}


void CCastStringEditorTextViewport::OnButtonUpdateSorting() 
{
	// we could be smarter here and be careful about when the groups really change
	// (adding/removing a sort field does not change the groups, only the sort
	// of orders on selected groups

	// transfer data from list controls down to local lists

	View()->UpdateData(true);

	// transfer data from local lists to orderSelection lists

	OrderSelection()->SetSortFields(View()->m_sortFields,SuperScen());

	//SetGroupListItems();
	SetGroupOrdersListItems();
}



//  This the method that CPSheetAvailOrders calls to indicate an APPLY/OK
//  which has changed the filter.

//  This corresponds to: (9) Change in the filter for available orders


void CCastStringEditorTextViewport::ReinitOrderSelection()
{
	View()->UpdateData(true);

	if ( OrderSelection() != 0 )
		OrderSelection()->Init(SuperScen());

	SetGroupOrdersListItems();
	SetGroupListItems();
}


void CCastStringEditorTextViewport::ChangeOrderForInsert(COrder* pOrder)
{
	if ( pOrder != m_pOrderForInsert ) {
		m_pOrderForInsert = pOrder;
		COrderForInsertChangeMsg msg(pOrder);
		TheBus().UpdateAllObservers(0,&msg);
	}
}





//	TODO: handle multiple selection properly.

void CCastStringEditorTextViewport::OnItemchangedStrandOrderList(int strandNum)
{
#if 0
	afxDump << "SelectMatchingGroups\n";
#endif

		
	CCastString* pString = (IsEditing() 
							? CurrentCastString()
							: 0);

	if ( pString == 0 )
		return;

	if ( OrderSelection() == 0 )
		return;

	// Determine selected orders on the strand

	vector<int> indexes;
	GetSelectedCSOrderIndexes(indexes,strandNum);

	vector<COrder*> orders;

	const vector<CCSOrder*>& strand = pString->Strand(strandNum);


	for ( vector<int>::const_iterator ii = indexes.begin();
		  ii != indexes.end();
		  ++ii ) {

		COrder* pOrder = strand[ (*ii)  ]->Order();

		if ( pOrder != 0 )
			orders.push_back(pOrder);

	}


	//  Compute which groups should be selected

	OrderSelection()->SelectMatchingGroups(orders,SuperScen());


	// Update the selections on the group list (groups themselves haven't changed).
	SetGroupListSelections();

	// Update selected orders list

	SetGroupOrdersListItems();

	//  Make the first order be the orderForInsert

	if ( orders.size() > 0 )
		ChangeOrderForInsert( orders[0] );
}




void CCastStringEditorTextViewport::DeleteOrders(vector<int>& selectedItemIndexes, 
										int strandNum)
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CCastString* pSelectedString = CurrentCastString();

	if ( pSelectedString == 0 )
		return;

	if ( ! pSelectedString->CanEdit() )
		return;

	if ( selectedItemIndexes.size() == 0 )
		return;

	vector<COrder*> deletedOrders;
	pSelectedString->DeleteOrders(selectedItemIndexes,strandNum,deletedOrders);
	OrderSelection()->UpdateGroupsFromOrderChange(deletedOrders,SuperScen());
	
	pScen->SetModified(true);

	COrdersDeletedMsg msg(pSelectedString);
	TheBus().UpdateAllObservers(0,&msg);
}


void CCastStringEditorTextViewport::MoveOrders(vector<int>& selectedItemIndexes, 
								int sourceStrand, 
								int destnStrand,
								int heatIndex,
								int orderIndex)
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CCastString* pSelectedString = CurrentCastString();

	if ( pSelectedString == 0 )
		return;
	
	if ( ! pSelectedString->CanEdit() )
		return;

	if ( selectedItemIndexes.size() == 0 )
		return;

	pSelectedString->MoveOrders(selectedItemIndexes,
								sourceStrand,
								destnStrand,
								heatIndex,
								orderIndex);
	pScen->SetModified(true);
	
	// here is the only place we use a hint!!!!

	//CUpdateHint hint(destnStrand,heatIndex,orderIndex);
	//UpdateAllViews(0,ORDERS_MOVED,&hint);

	COrdersMovedMsg msg(pSelectedString,destnStrand,heatIndex,orderIndex);
	TheBus().UpdateAllObservers(0,&msg);
}





void CCastStringEditorTextViewport::AddOrders(vector<COrder*>& selectedItems, 
									 int strandNum,
									 bool query)
{
	InsertOrders(selectedItems,strandNum,-1,-1,query);
}



void CCastStringEditorTextViewport::InsertOrders(vector<COrder*>& selectedItems, 
								  int strandNum, 
								  int heatIndex,
								  int orderIndex,
								  bool query)	
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CCastString* pSelectedString = CurrentCastString();

	if ( pSelectedString == 0 )
		return;

	if ( ! pSelectedString->CanEdit() )
		return;

	if ( selectedItems.size() == 0 )
		return;


	pSelectedString->InsertOrders(selectedItems,
								  SuperScen(),
								  strandNum,
								  heatIndex,
								  orderIndex,
								  query);
	OrderSelection()->UpdateGroupsFromOrderChange(SuperScen());

	pScen->SetModified(true);

	//CUpdateHint hint(strandNum,heatIndex,orderIndex);
	//UpdateAllViews(0,ORDERS_INSERTED,&hint);

	COrdersInsertedMsg msg(pSelectedString,strandNum,heatIndex,orderIndex);
	TheBus().UpdateAllObservers(0,&msg);
}





void CCastStringEditorTextViewport::AddStockOrder(int strandNum)
{
	InsertStockOrder(strandNum,-1,-1);
}



void CCastStringEditorTextViewport::InsertStockOrder(int strandNum, 
										    int heatIndex,
											int orderIndex)	
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CCastString* pSelectedString = CurrentCastString();

	if ( pSelectedString == 0 )
		return;

	if ( ! pSelectedString->CanEdit() )
		return;

	if ( pSelectedString->InsertStockOrder(SuperScen(),
										   strandNum,
										   heatIndex,
										   orderIndex) != 0 ) {
		pScen->SetModified(true);
		
		//CUpdateHint hint(strandNum,heatIndex,orderIndex);
		//UpdateAllViews(0,ORDERS_INSERTED,&hint);
		COrdersInsertedMsg msg(pSelectedString,strandNum,heatIndex,orderIndex);
		TheBus().UpdateAllObservers(0,&msg);
	}
}



void CCastStringEditorTextViewport::DoOrderInquiry(vector<int>& selectedItemIndexes,
									int strandNum)
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CCastString* pSelectedString = CurrentCastString();

	if ( pSelectedString == 0 )
		return;

	if ( selectedItemIndexes.size() == 0 ) return;

	const vector<CCSOrder*>& strand = pSelectedString->Strand(strandNum);
	CCSOrder* pCSOrder = strand[selectedItemIndexes[0]];

	if ( pCSOrder->Order() == 0 ) {
		ostrstream ostr;
		// FP change
//		ostr << "Missing order information for mill order # " 
//			 << pCSOrder->OrderNum()
//			 << ends;
		ostr << "Missing order information for mill order # " 
			 << LPCTSTR(pCSOrder->FpOrderNum())
			 << ends;
		AfxMessageBox(ostr.str(),MB_ICONERROR);
		ostr.freeze(false);
	}
	else {

		COrderInquiryDlg dlg;

		dlg.m_pOrder		= pCSOrder->Order();
		dlg.m_pSuperScen	= SuperScen();

		dlg.DoModal();

		if ( dlg.m_pOrder != 0 )
			ChangeOrderForInsert( dlg.m_pOrder );
	}
}



void CCastStringEditorTextViewport::DoOrderInquiry(vector<COrder*>& selectedItems)
{
	CCasterScen* pScen = CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	if ( selectedItems.size() == 0 ) return;

	COrderInquiryDlg dlg;

	dlg.m_pOrder		= selectedItems[0];
	dlg.m_pSuperScen	= SuperScen();

	dlg.DoModal();

	if ( dlg.m_pOrder != 0 )
		ChangeOrderForInsert( dlg.m_pOrder );

}






//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorTextController Class
//
//  Handles events display for CCastStringEditorTextView/Viewport
//
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCastStringEditorTextController, MvcController)
	//{{AFX_MSG_MAP(CCastStringEditorTextController)
	ON_BN_CLICKED(ID_VIEW_AVAILABLEORDERS, OnViewAvailableorders)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GROUPS, OnItemchangedListGroups)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_1, OnButtonGroup1)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_2, OnButtonGroup2)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_GROUPING, OnButtonUpdateGrouping)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_SORTING, OnButtonUpdateSorting)
	ON_NOTIFY(NM_DBLCLK, IDC_STRAND1_ORDER_LIST, OnDblclkStrand1OrderList)
	ON_NOTIFY(NM_DBLCLK, IDC_STRAND2_ORDER_LIST, OnDblclkStrand2OrderList)
	ON_BN_CLICKED(ID_EDIT_ORDER_INQUIRY, OnEditOrderInquiry)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STRAND1_ORDER_LIST, OnItemchangedStrand1OrderList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STRAND2_ORDER_LIST, OnItemchangedStrand2OrderList)
	ON_BN_CLICKED(ID_EDIT_CREATE_STRING, OnEditCreateString)
	ON_BN_CLICKED(ID_EDIT_HEAT_DETAILS, OnEditHeatDetails)
	ON_BN_CLICKED(IDC_BUTTON_EDITABLE, OnButtonEditable)
	ON_BN_CLICKED(IDC_BUTTON_HEATS_LOCKED, OnButtonHeatsLocked)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, OnButtonUpload)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_GROUPS, OnGetdispinfoListGroups)
	ON_COMMAND(ID_EDIT_STRINGBUILDER, OnEditStringbuilder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRINGBUILDER, OnUpdateEditStringbuilder)
	ON_COMMAND(ID_EDIT_PSTCONFORMANCEREPORT, OnEditPstconformancereport)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PSTCONFORMANCEREPORT, OnUpdateEditPstconformancereport)
	ON_COMMAND(ID_EDIT_MOVESTRINGTOOTHERCASTER, OnEditMovestringtoothercaster)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVESTRINGTOOTHERCASTER, OnUpdateEditMovestringtoothercaster)
	ON_COMMAND(ID_EDIT_STRING_INFO, OnEditStringInfo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRING_INFO, OnUpdateEditStringInfo)
	ON_COMMAND(ID_EDIT_PLANWORKSHEET, OnEditPlanworksheet)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLANWORKSHEET, OnUpdateEditPlanworksheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CMessageBus* CCastStringEditorTextController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


// delegate
void CCastStringEditorTextController::OnGetdispinfoListGroups(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( pResult != 0 )
		*pResult = 0;

	GetViewport()->OnGetdispinfoListGroups(pNMHDR);
}

void CCastStringEditorTextController::OnButtonGroup1()
{
	GetViewport()->OnButtonGroup1();
}

void CCastStringEditorTextController::OnButtonGroup2()
{
	GetViewport()->OnButtonGroup2();
}

void CCastStringEditorTextController::OnButtonUpdateGrouping()
{
	GetViewport()->OnButtonUpdateGrouping();
}

void CCastStringEditorTextController::OnButtonUpdateSorting()
{
	GetViewport()->OnButtonUpdateSorting();
}


void CCastStringEditorTextController::OnViewAvailableorders() 
{
	CCasterScen* pScen = GetViewport()->CurrentCasterScen();

	if ( pScen == 0 ) return;

	COrderSelection* pSelect = GetViewport()->OrderSelection();

	if ( pSelect == 0 ) return;
	
/////////// DM CHANGE: 2002-09-14: added superscen arg
	
	CPSheetAvailOrders dlg("Available orders",
						   GetViewport()->View());

	dlg.Init(pSelect,GetViewport()->SuperScen(),GetViewport());
	dlg.DoModal();
}



// This corresponds to: (2) Selection on the group list


// When a change in selection is made on the groups list
//   we must update the underlying COrderSelection
//   and we must change the order list items

void CCastStringEditorTextController::OnItemchangedListGroups(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

#if 0
	afxDump << "OnItemChangedListGroups: " 
			<< pNMListView->iItem << " "
			<< pNMListView->iSubItem << " "
			<< pNMListView->uNewState << " "
			<< pNMListView->uOldState << " "
			<< pNMListView->uChanged << "\n";
#endif

	if ( pNMListView->uNewState & LVIS_FOCUSED ) {

#if 0
		afxDump << "UpdateSelectedGroups\n";
#endif
	
		GetViewport()->UpdateSelectedGroups();
		GetViewport()->SetGroupOrdersListItems();

	}
	
	*pResult = 0;
}






void CCastStringEditorTextController::OnDblclkStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblclickStrandOrderList(1);

	*pResult = 0;
}

void CCastStringEditorTextController::OnDblclkStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblclickStrandOrderList(2);

	*pResult = 0;
}





// This corresponds to: (8) Change in #pieces on an order in a strand list (by double click)


void CCastStringEditorTextController::OnDblclickStrandOrderList(int strandNum)
{
	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CCastString* pString = GetViewport()->CurrentCastString();

	if ( pString == 0 ) 
		return;
	
	if ( GetViewport()->OrderSelection() == 0 )
		return;

	vector<int> indexes;

	// find which CSOrder has been selected
	GetViewport()->GetSelectedCSOrderIndexes(indexes,strandNum);

	if ( indexes.size() == 0 )
		return;

	int index = indexes[0];

	if ( pString->ModifyCSOrderParms(GetViewport()->SuperScen(),
									 strandNum,
									 index)  ) {

		CCastStringEditorTextViewport::CCSOrderChangedMsg msg(pString,strandNum,index);
		TheBus().UpdateAllObservers(0,&msg);
	}
}



void CCastStringEditorTextController::OnEditOrderInquiry() 
{
	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	COrderInquiryDlg dlg;

	dlg.m_pOrder		= GetViewport()->OrderForInsert();
	dlg.m_pSuperScen	= GetViewport()->SuperScen();

	dlg.DoModal();

	if ( dlg.m_pOrder != 0 )
		GetViewport()->ChangeOrderForInsert( dlg.m_pOrder );
}




// These correspond to: (1) Selection on a strand list


void CCastStringEditorTextController::OnItemchangedStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

#if 0
	afxDump << "OnItemChangedStrand1List: " 
			<< pNMListView->iItem << " "
			<< pNMListView->iSubItem << " "
			<< pNMListView->uNewState << " "
			<< pNMListView->uOldState << " "
			<< pNMListView->uChanged << "\n";
#endif	

	if ( pNMListView->uNewState & LVIS_FOCUSED )
		//  something just got the focus.
		//  Even if it was a shift-select, this means we are done
		//     so let's make a change

		GetViewport()->OnItemchangedStrandOrderList(1);

	*pResult = 0;
}

void CCastStringEditorTextController::OnItemchangedStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

#if 0
	afxDump << "OnItemChangedStrand1List: " 
			<< pNMListView->iItem << " "
			<< pNMListView->iSubItem << " "
			<< pNMListView->uNewState << " "
			<< pNMListView->uOldState << " "
			<< pNMListView->uChanged << "\n";
#endif	

	if ( pNMListView->uNewState & LVIS_FOCUSED )
		//  something just got selected and  got the focus.
		//  Even if it was a shift-select, this means we are done
		//     so let's make a change

		GetViewport()->OnItemchangedStrandOrderList(2);

	*pResult = 0;
}


void CCastStringEditorTextController::OnEditCreateString() 
{
	// if we are not editing 
	//    then there is no scenario in which to make a string

	if ( ! GetViewport()->IsEditing() )
		return;

	assert( CScenMru::GlobalScenMru.CurrentCasterScen() != 0 );

	CreateCastString(CScenMru::GlobalScenMru.CurrentSuperScen(),CScenMru::GlobalScenMru.CasterNum());
}





void CCastStringEditorTextController::OnEditHeatDetails() 
{
	CCastString* pString = (GetViewport()->IsEditing() 
							? GetViewport()->CurrentCastString()
							: 0);

	if ( pString == 0 )
		return;

	CHeatDetailsDlg dlg;

	dlg.m_pCastString	= pString;
	dlg.m_pSuperScen	= GetViewport()->SuperScen();

	if ( dlg.DoModal() == IDOK )
		GetViewport()->CurrentCasterScen()->SetModified(TRUE);

	{
		CCastStringEditorTextViewport::COrdersMovedMsg msg(pString);
		TheBus().UpdateAllObservers(0,&msg);
	}

	{ 
		CCastStringEditorValidnViewport::CValidnMsg msg(pString);
		TheBus().UpdateAllObservers(0,&msg);
	}
}



void CCastStringEditorTextController::OnButtonEditable() 
{
	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CCastString* pString = GetViewport()->CurrentCastString();

	if ( pString == 0 ) 
		return;
	
	pString->CanEdit( ! pString->CanEdit() );

	CCastStringEditorTextViewport::CStringCanEditStatusChanged msg(pString);
	TheBus().UpdateAllObservers(0,&msg);
}



void CCastStringEditorTextController::OnButtonHeatsLocked() 
{
	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CCastString* pString = GetViewport()->CurrentCastString();

	if ( pString == 0 ) 
		return;
	
	CRect buttonRect;
	GetViewport()->View()->m_btnHeatsLocked.GetWindowRect(buttonRect);


	CHeatLockDlg dlg;
	dlg.m_heatStatus = pString->HeatStatus();
	dlg.m_numLockedHeats = pString->NumLockedHeats();

	dlg.m_bClientPos = true;

	CPoint point = buttonRect.TopLeft();
	dlg.m_x = point.x;
	dlg.m_y = point.y;

	if ( dlg.DoModal() != IDOK ) 
		return;

	int oldStatus	= pString->HeatStatus();
	int newStatus	= dlg.m_heatStatus;
	int oldNum		= pString->NumLockedHeats();
	int newNum		= dlg.m_numLockedHeats;

	bool ask = (newStatus == CCastString::HEATS_NONE_LOCKED 
		        &&
				oldStatus != CCastString::HEATS_NONE_LOCKED)
			   ||
			   (newStatus == CCastString::HEATS_SOME_LOCKED
			    &&
				oldStatus == CCastString::HEATS_ALL_LOCKED)
			   ||
			   (newStatus == CCastString::HEATS_SOME_LOCKED
			    &&
				oldStatus == CCastString::HEATS_SOME_LOCKED
				&& 
				newNum < oldNum);

	if ( ! ask 
		 ||
		 IDYES == AfxMessageBox("Unlocking heats will allow the system to redistribute orders"
			                        " as it sees fit?\nAre you sure you want to unlock?",
									MB_YESNO|MB_ICONWARNING) ) {

		pString->HeatStatus( dlg.m_heatStatus );
		pString->NumLockedHeats( dlg.m_numLockedHeats );
		pString->CalculateHeats();
		pString->CalculateSummary();
		pCScen->SetModified(true);

		CCastStringEditorTextViewport::CLockedHeatsStatusChangedMsg msg(pString);
		TheBus().UpdateAllObservers(0,&msg);
	}
}



void CCastStringEditorTextController::OnButtonSave() 
{
	CCasterScen* pScen = GetViewport()->CurrentCasterScen();

	if ( pScen == 0 ) return;

	CWaitCursor wc;

	int result = AfxMessageBox("Yes = Save changes\n"
							   "No = Do not save changes, leave as is\n"
							   "Cancel = Restore to previous state\n",
							   MB_YESNOCANCEL|MB_ICONQUESTION);

	switch ( result ) {
	case IDYES:
		CCasterScenArchiver::Archive(pScen,true);
		CScenMgr::BackupCasterScen(pScen);
		{
			CCastStringEditorTextViewport::CScenSavedMsg msg(pScen);
			TheBus().UpdateAllObservers(0,&msg);
		}
		break;

	case IDNO:
		// do nothing
		break;

	case IDCANCEL:
		CCasterScen* pBackup = CScenMgr::RestoreCasterScenBackup(GetViewport()->CurrentCasterScen());
		CScenMru::GlobalScenMru.PromoteCasterScen(pBackup);
		break;
	}
}
	
	


void CCastStringEditorTextController::OnButtonUpload() 
{
	CCasterScen* pScen = GetViewport()->CurrentCasterScen();

	if  ( pScen == 0 )
		return;

	CSuperScen* pSuperScen = GetViewport()->SuperScen();

	if ( pSuperScen == 0 ) {
		AfxMessageBox("You need to have a scenario on each caster to do this.",MB_OK);
		return;
	}

	CStringStatusDlg dlg;

	dlg.m_pCasterScen	= pScen;
	dlg.m_pSnapScen		= TheSnapshot.SuperScen()->CasterScen(GetViewport()->CasterNum());
	dlg.m_pSuperScen	= pSuperScen;

	dlg.DoModal();
}


void CCastStringEditorTextController::OnEditStringbuilder() 
{
	CCasterScen* pScen = GetViewport()->CurrentCasterScen();

	if ( pScen == 0 ) return;

	CStringBuilderDlg sbd;
	for (int i = Caster::FirstCaster; i <= Caster::LastCaster; i++)
		sbd.m_casterScen[i] = GetViewport()->CurrentCasterScen(i);
	sbd.m_pScen = pScen;
	sbd.m_caster = GetViewport()->CasterNum();
	sbd.m_id = CCastStringId(0,0,pScen->Caster(),0);
	sbd.m_pSuperScen = GetViewport()->SuperScen();

	sbd.DoModal();
}



void CCastStringEditorTextController::OnUpdateEditStringbuilder(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( GetViewport()->CurrentCasterScen() != 0 );
}



void CCastStringEditorTextController::OnEditPstconformancereport() 
{
	CSuperScen* pSuperScen = GetViewport()->SuperScen();

	if ( pSuperScen == 0 ) return;

	CPstConformanceReport report(pSuperScen);
	report.Create();
}



void CCastStringEditorTextController::OnUpdateEditPstconformancereport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( GetViewport()->SuperScen() != 0 );
}



void CCastStringEditorTextController::OnEditPlanworksheet() 
{
	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CPlanWorksheetReport report(pCScen);
	report.Create();
}

void CCastStringEditorTextController::OnUpdateEditPlanworksheet(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( GetViewport()->CurrentCasterScen() != 0 );
}



void CCastStringEditorTextController::OnEditMovestringtoothercaster() 
{
	if ( ! GetViewport()->IsEditing() )
		return;

	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	CCastString* pString = GetViewport()->CurrentCastString();

	// if no string selected, do nothing.
	if ( pString == 0 )
		return;

	CPickCasterDlg dlg;
	dlg.m_ignoreCaster = pString->Id().Caster();
	dlg.m_caster = pString->Id().Caster() == Caster::C1 ? Caster::C2 : Caster::C1;
	dlg.m_label = "Pick caster to move string to:";
	
	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.m_caster == dlg.m_ignoreCaster ) // shouldn't happen, but might as well check
		return;

	CCasterScen* pToScen = GetViewport()->CurrentCasterScen(dlg.m_caster);
	if ( pToScen == 0 ) {
		AfxMessageBox("No scenario for that caster, unable to perform move");
		return;
	}

	// determine if we will have an id conflict.
	// also make a copy of existing ids for later use 
	//		(so don't break out of loop early).

	const CCastStringId& origId = pString->Id();
	vector<CCastStringId> usedIds;

	bool idConflict = false;
	{

		for ( vector<CCastString*>::iterator is = pToScen->CastStrings().begin();
			  is != pToScen->CastStrings().end();
			  ++is ) {
				const CCastStringId& thisId = (*is)->Id();
				usedIds.push_back(thisId);
				if ( thisId.Year() == origId.Year()
					 &&
					 thisId.Week() == origId.Week()
					 &&
					 thisId.StringId() == origId.StringId() ) {
					idConflict = true;
				}
		}
	}
			
	CCastStringId id(origId.Year(),
					 origId.Week(),
					 dlg.m_caster,
					 origId.StringId());

	CCastStringMiscProps props = pString->GetProps();
	
	if ( idConflict ) {
		// this id duplicates on in the destination scenario
		// have the user enter a new id.

		CCastStringIdDlg idDlg;



		idDlg.m_pId						= &id;
		idDlg.m_usedIds[GetViewport()->CasterNum()]	= usedIds;  // copy
		idDlg.m_pProps					= &props;	// non-const accessor
		idDlg.m_isEditing				= true;
		idDlg.m_setInitialId			= false;


		if ( idDlg.DoModal() != IDOK ) 
			return;
	}

	CCastString* pNewString = pToScen->CreateCopyString(id,props,pString);

	pCScen->DeleteString(pString);

	pCScen->SetModified(true);
	pToScen->SetModified(true);


	// switch to destination scenario
	CScenMru::GlobalScenMru.SetCasterNum(dlg.m_caster);

	// make the new string current
	CScenMru::GlobalScenMru.SetCurrentCastString(pNewString);

	
	ValidateOrderSelection();
//	UpdateTitle();
}



void CCastStringEditorTextController::OnUpdateEditMovestringtoothercaster(CCmdUI* pCmdUI) 
{

	CCastString* pString = (GetViewport()->IsEditing() 
							? GetViewport()->CurrentCastString()
							: 0);

	pCmdUI->Enable( pString != 0 );

}




void CCastStringEditorTextController::OnEditStringInfo() 
{
	// if we are not editing 
	//    then there is no scenario in which to make a string

	if ( ! GetViewport()->IsEditing() )
		return;

	CCasterScen* pCScen = GetViewport()->CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CCastString* pSelectedString = GetViewport()->CurrentCastString();

	if ( pSelectedString == 0 )
		return;
	
	ShowCastStringInfo(pCScen,pSelectedString);
}

void CCastStringEditorTextController::OnUpdateEditStringInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewport()->CurrentCastString() != 0);			
}




//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorValidnViewport 
//
//  Manages display for CCastStringEditorValidnView
//
//  Manages a single list ctrl that displays validation errors
//    for the currently displayed string.
//
//  There is one button that triggers a new validation.
// 
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

void CCastStringEditorValidnViewport::OnInitialUpdate()
{
	// the view needs to set these before we get here

	assert( m_pListValidn   != 0 );

	CCastStringHeatValidnError::SetHeader(*m_pListValidn);
}


CMessageBus* CCastStringEditorValidnViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CCastStringEditorValidnViewport::CreateController()
{
	m_pCtlr = new CCastStringEditorValidnController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CCastStringEditorValidnViewport::OnCreate()
{
	// do nothing
	return TRUE;
}


/////////////////////////////
//  
//  message
//
/////////////////////////////

void CCastStringEditorValidnViewport::CValidnMsg::Sprint(CString& strCmd)
{
	strCmd.Format("CastStringEditor: Validate cast string");
}




//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CCastStringEditorValidnViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	// any change in scenarios which exist or in our selection

	case MSG_ID_MRU_PROMOTE_CASTER_SCEN:
	case MSG_ID_MRU_SELECT_CASTER:
	case MSG_ID_MRU_SELECT_CAST_STRING:
	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
	case MSG_ID_CSE_STRING_VALIDATED:
		UpdateList();
		break;

	default:
		// do nothing
		break;
	}
}




//////////////////////////////////////////////////////
//
//  Manage display
//
//////////////////////////////////////////////////////


void CCastStringEditorValidnViewport::UpdateList()
{
	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	if ( pString == 0 )
		return;

	CCastStringHeatValidnError::UpdateList(*m_pListValidn,pString->ValidnErrors());
}




//////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorValidnController 
//
//  Handles events display for CCastStringEditorValidnView/Viewport
//
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCastStringEditorValidnController, MvcController)
	//{{AFX_MSG_MAP(CCastStringEditorValidnController)
	ON_BN_CLICKED(ID_VALIDATE, OnValidate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RESULTS, OnItemchangedListResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CCastStringEditorValidnController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}



void CCastStringEditorValidnController::OnValidate() 
{
	CCasterScen* pCScen = CScenMru::GlobalScenMru.CurrentCasterScen();

	if ( pCScen == 0 ) return;

	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	if ( pString == 0 )
		return;

	CSuperScen* pSS = CScenMru::GlobalScenMru.CurrentSuperScen();
	if ( pSS == 0 )
		return;

	CCastStringValidator validator;

	vector<bool> marks;
	{
		for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
			  ih != pString->Heats().end();
			  ++ih )  {
			marks.push_back( (*ih).IsMarked() );
			(*ih).IsMarked(true);
		}
	}


	validator.Validate(pString->Heats(),
					   pString,
					   pCScen,
					   pSS);

	{	
		vector<CCastStringHeat>::iterator ih;
		vector<bool>::iterator ib;

		for ( ih = pString->Heats().begin(),
			  ib = marks.begin();
			  ih != pString->Heats().end();
			  ++ih,
			  ++ib )  {
			(*ih).IsMarked((*ib));
		}
	}

	CCastStringEditorValidnViewport::CValidnMsg msg(pString);
	TheBus().UpdateAllObservers(0,&msg);
}




void CCastStringEditorValidnController::OnItemchangedListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;

#if 0
	CCastString* pString = CScenMru::GlobalScenMru.CurrentCastString();

	if ( pString == 0 )
		return;

	if ( ! (pNMListView->uNewState & LVIS_FOCUSED) ) 
		return;

	int posn = m_listValidation.GetNextItem(-1,LVNI_SELECTED);
	if ( posn == -1 ) 
		return;


	// check for no errors
	if ( pString->ValidnErrors().size() == 0 )
		return;

	const CCastStringHeatValidnError& rErr = pString->ValidnErrors()[posn];

	const CCastStringHeat* pHeat = rErr.GetHeat();

	if ( pHeat == 0 )
		return;
#endif

	// unselect all items in the order list

//	for ( int i = m_listOrders.GetNextItem(-1,LVNI_SELECTED);
//		  i != -1;
//	      i = m_listOrders.GetNextItem(i,LVNI_SELECTED) ) 
//		m_listOrders.SetItemState(i,0,LVIS_SELECTED);

//
//	if ( rErr.GetCSOrder() == 0 )
//		return;

		
//	int strandNum = rErr.GetStrandNum();

//	int index = ( strandNum == 1 
//		          ? rErr.GetLotIndex()
//				  : rErr.GetLotIndex() + ( pHeat->StrandEnd(1) - pHeat->StrandBegin(1) ) );

	// Select the current order

//	m_listOrders.SetItemState(index,LVIS_SELECTED,LVIS_SELECTED);		

}


