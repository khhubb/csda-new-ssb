// ScenMasterMvc.cpp: implementation of the CScenMasterCtrlViewport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "ScenMasterMvc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ScenMru.h"
#include "CasterScen.h"
#include "ScenMgr.h"
#include "CastString.h"
#include "CastStringHeat.h"



//////////////////////////////////////////////////////////////////////
//
//  CScenMasterCtrlViewport
//
//  Manages display for CScenMasterCtrlView
//
//  Two lists: available scenarios, selected scenarios
//  Plus buttons to move scenarios from one to the other.
//  The list of selected scenarios determines which scenarios
//    are shown in the graph (CScenMasterGraphView/Viewport/Controller).
//
//  Behavior:
//
//  All scenarios currently defined are displayed in the two lists.
//  Scenarios selected for graphing are in m_selScens and displayed in listbox m_pListSelScens,
//  scenarios not selected are computed by difference and displayed in listbox m_pListAvailScens.
//  
//  Buttons are for moving scenarios from selected to unselected and vice versa.
//  Buttons are: select all    (active if there are any unselected scens)
//               select some   (active if some entry in m_pListAvailScens is selected)
//               deselect all  (active if there are any selected scens)
//               deselect some (active if some entry in m_pListSelScens is selected)
//
//  Corresponding actions are to move scenarios (all or selected) from one category to the other,
//    and update the displays.
//
//
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

void CScenMasterCtrlViewport::OnInitialUpdate()
{
	// the view needs to set these before we get here

	assert( m_pListSelScens   != 0 );
	assert( m_pListAvailScens != 0 );
	assert( m_pBtnSelectOne   != 0 );
	assert( m_pBtnSelectAll   != 0 );
	assert( m_pBtnDeselectOne != 0 );
	assert( m_pBtnDeselectAll != 0 );

	SetButtons();
	SetLists();
}


CMessageBus* CScenMasterCtrlViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CScenMasterCtrlViewport::CreateController()
{
	m_pCtlr = new CScenMasterCtrlController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CScenMasterCtrlViewport::OnCreate()
{
	// do nothing
	return TRUE;
}


///////////////////////////////////////////////////////
//
//  Notifications
//
///////////////////////////////////////////////////////

void CScenMasterCtrlViewport::CScenSelChangeMsg::Sprint(CString& strCmd)
{
	strCmd.Format("ScenMaster: Changing selection scenarios");
}



///////////////////////////////////////////////////////
//
//  Display
//
///////////////////////////////////////////////////////


//
//  Set status of buttons depending on conditions in the boxes
//

void CScenMasterCtrlViewport::SetButtons()
{
	m_pBtnSelectOne->EnableWindow(m_pListAvailScens->GetSelCount() > 0);
	m_pBtnSelectAll->EnableWindow(m_pListAvailScens->GetCount() > 0);
	m_pBtnDeselectOne->EnableWindow(m_pListSelScens->GetSelCount() > 0);
	m_pBtnDeselectAll->EnableWindow(m_pListSelScens->GetCount() > 0);

}


//
//  Sets the contents of the two list boxes.
//

void CScenMasterCtrlViewport::SetLists()
{
	// Compute the available scens = scens not selected
	vector<CCasterScen*> availScens;
	ComputeAvailScens(availScens);

	PopulateList(m_pListAvailScens,availScens);
	PopulateList(m_pListSelScens,m_selScens);
}



//
//  Set the contents of a listbox to represent a sequence of scenarios
//

void CScenMasterCtrlViewport::PopulateList(CListBox* pListBox, 
									   vector<CCasterScen*>& scens)
{
	pListBox->ResetContent();

	ostrstream ostr;

	for ( vector<CCasterScen*>::iterator is = scens.begin();
		  is != scens.end();
		  ++is ) {

		char* strCaster;
		switch ( (*is)->Caster() ) {
		case 1:
			strCaster = "#1SC";
			break;
		case 2:
			strCaster = "#2SC";
			break;
		case 3:
			strCaster = "#3CC";
			break;
		default:
			assert(0);
		}

		ostr << strCaster 
			 << " " << setw(4) << (*is)->Id()
			 << " " << (*is)->Owner()
			 << " " << LPCTSTR((*is)->Comment())
			 << ends;

		int posn = pListBox->AddString( ostr.str() );
		ostr.freeze(false);
		ostr.seekp(0);

		// we keep a pointer to the scenario for later reference
		pListBox->SetItemDataPtr(posn, (void*)(*is));
	}
}



//
//  The available scens are the scens which have not been selected.
//

void CScenMasterCtrlViewport::ComputeAvailScens(vector<CCasterScen*>& availScens)
{
	availScens.clear();

	for ( int i=1; i<=3; ++i )
		for ( TScenVec::iterator is = CScenMgr::CasterScensBegin(i);
		      is != CScenMgr::CasterScensEnd(i);
			  ++is )
			if ( find(m_selScens.begin(),
				      m_selScens.end(),
					  (*is)) == m_selScens.end() )
				availScens.push_back( (*is) );
}



///////////////////////////////////////////////////////
//
//  Actions (delegated from Controller)
//
///////////////////////////////////////////////////////



void CScenMasterCtrlViewport::DeselectAllScens() 
{
	m_selScens.clear();

	SendUpdateMsg();
}


void CScenMasterCtrlViewport::DeselectSelectedScens() 
{
	vector<CCasterScen*> deselScens;
	GetSelectedScens(m_pListSelScens,deselScens);
	
	for ( vector<CCasterScen*>::iterator is = deselScens.begin();
		  is != deselScens.end();
		  ++is ) {

		vector<CCasterScen*>::iterator i2 
			= find( m_selScens.begin(),
					m_selScens.end(),
					(*is));

		if ( i2 != m_selScens.end() )
			m_selScens.erase( i2 );
	}
	
	ValidateScens(m_selScens);

	SendUpdateMsg();
}


void CScenMasterCtrlViewport::SelectAllScens() 
{
	m_selScens.clear();

	for ( int i=1; i<=3; ++i )
		m_selScens.insert(m_selScens.end(),
		  			   CScenMgr::CasterScensBegin(i),
					   CScenMgr::CasterScensEnd(i));

	SendUpdateMsg();
}


void CScenMasterCtrlViewport::SelectSelectedScens() 
{
	vector<CCasterScen*> selScens;
	GetSelectedScens(m_pListAvailScens,selScens);
	
	m_selScens.insert(m_selScens.end(),
				   selScens.begin(),
				   selScens.end());
	ValidateScens(m_selScens);

	SendUpdateMsg();
}



//////////////////////////////////////////////////////////
// 
//  Implementation details
//
//////////////////////////////////////////////////////////

//
//  Because a scenario might have been deleted by outside forces,
//    make sure we don't have any out of date pointers in our lists.
//

// static
void CScenMasterCtrlViewport::ValidateScens(vector<CCasterScen*>&scens)
{
	for ( TScenVec::iterator is = scens.begin();
	      is != scens.end();
		  ++is ) {

		TScenVec::iterator i2 
			= find(CScenMgr::CasterScensBegin( (*is)->Caster() ),
				   CScenMgr::CasterScensEnd( (*is)->Caster() ),
				   (*is));

		if ( i2 == CScenMgr::CasterScensEnd( (*is)->Caster() ) )

			is = scens.erase( is );
	}
}


//
//  Iterate through the selected items in the list box and aggregate
//    the associated scenarios.
//

void CScenMasterCtrlViewport::GetSelectedScens(CListBox* pBox,
										   vector<CCasterScen*>& scens)
{
	int count = pBox->GetSelCount();
	int* arr = new int [count];
	pBox->GetSelItems(count,arr);

	for ( int i=0; i<count; ++i )
		scens.push_back( (CCasterScen*)pBox->GetItemDataPtr(arr[i]) );

	delete [] arr;
}



//
//  Whenever the selection changes, we notify the world.
//

void CScenMasterCtrlViewport::SendUpdateMsg()
{
	CScenSelChangeMsg msg(m_selScens);
	TheBus().UpdateAllObservers(0,&msg);
}



//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CScenMasterCtrlViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	// any change in scenarios which exist or in our selection

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
	case MSG_ID_SMASTER_CHANGE_SEL_SCENS:
		SetLists();
		SetButtons();
		break;

	default:
		// do nothing
		break;
	}
}


//////////////////////////////////////////////////////////////////////
//
//  CScenMasterCtrlController
//
//  Handles events display for CScenMasterCtrlView/Viewport
//
//////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CScenMasterCtrlController, MvcController)
	//{{AFX_MSG_MAP(CScenMasterCtrlController)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL, OnButtonDeselectAll)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ONE, OnButtonDeselectOne)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ONE, OnButtonSelectOne)
	ON_LBN_SELCHANGE(IDC_LIST_AVAILABLE_SCENS, OnSelchangeListAvailableScens)
	ON_LBN_SELCHANGE(IDC_LIST_SELECTED_SCENS, OnSelchangeListSelectedScens)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CScenMasterCtrlController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


//
// interface actions get delegated back to the viewport
//


void CScenMasterCtrlController::OnButtonDeselectAll() 
{
	GetViewport()->DeselectAllScens();
}


void CScenMasterCtrlController::OnButtonDeselectOne() 
{
	GetViewport()->DeselectSelectedScens();
}


void CScenMasterCtrlController::OnButtonSelectAll() 
{
	GetViewport()->SelectAllScens();
}


void CScenMasterCtrlController::OnButtonSelectOne() 
{
	GetViewport()->SelectSelectedScens();
}



void CScenMasterCtrlController::OnSelchangeListAvailableScens() 
{
	GetViewport()->SetButtons();
}


void CScenMasterCtrlController::OnSelchangeListSelectedScens() 
{
	GetViewport()->SetButtons();	
}





//////////////////////////////////////////////////////////////////////
//
//  CScenMasterGraphViewport
//
//  Manages display for CScenMasterGraphView
//
//  one big SECPanView to graph on.
//
//  Each selected scenario will be drawn as 
//
//    +---+-----------------------------------+----------------------------+
//    |Hdr| String  1 id                      | String  2 id               | 
//    |   +-----------------------------------+----------------------------+ ...
//    |   | Heat 1 |  Heat 2 | Heat 3 | ...   | Heat 1 | ...               |
//    +---+-----------------------------------+----------------------------+
//
//  We compute and cache the rectangles designated for each heat, string and scenario.
//
//  TODO: zooming
//
//////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////
//
//  Standard viewport stuff
//
///////////////////////////////////////////////

void CScenMasterGraphViewport::OnInitialUpdate()
{
	assert( m_pView != 0 );

	ComputeScenLocns();
}


CMessageBus* CScenMasterGraphViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CScenMasterGraphViewport::CreateController()
{
	m_pCtlr = new CScenMasterGraphController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CScenMasterGraphViewport::OnCreate()
{
	// do nothing
	return TRUE;
}


//////////////////////////////////////////////////////////////
//
//  Drawing on the display
//
/////////////////////////////////////////////////////////////


//
//  Called in response to OnDraw on the view.
//  Draw each scenario.
//

void CScenMasterGraphViewport::Draw(CDC* pDC)
{
	for ( TScenVec::iterator is = m_selScens.begin();
		  is != m_selScens.end();
		  ++is )

		DrawScen( (*is), pDC );
}


//
// To draw a scenario, draw its header, then draw each string in the scenario
//

void CScenMasterGraphViewport::DrawScen(CCasterScen* pScen, CDC* pDC)
{
	DrawScenHeader(pScen,pDC);

	for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
		  is != pScen->CastStrings().end();
		  ++is )

		DrawCastString( (*is), pDC );
}



//
// The scen header contains basic identifying information for the scenario
//

void CScenMasterGraphViewport::DrawScenHeader(CCasterScen* pScen, CDC* pDC)
{
	// the header occupies HEADER_WIDTH pixels of width at the left 
	//   of the space computed for the scenario.

	CRect headerRect = GetLocn(pScen);
	headerRect.right = headerRect.left + HEADER_WIDTH;

	if ( ! pDC->RectVisible(headerRect) )
		return;

	char* strCaster;
	switch ( pScen->Caster() ) {
	case 1:
		strCaster = "#1SC";
		break;

	case 2:
		strCaster = "#2SC";
		break;

	case 3:
		strCaster = "#3CC";
		break;

	default:
		strCaster = "??Caster";
		break;
	}


	CString text1;
	CString text2;

	text1.Format("%s %s",
				 strCaster,
				 pScen->Owner());
	text2.Format("#%d",
				 pScen->Id());

	vector<CString> strings;
	strings.push_back(text1);
	strings.push_back(text2);


	int oldMode = pDC->SetBkMode(TRANSPARENT);
	
	pDC->FillSolidRect(headerRect,RGB(192,192,255));
	pDC->DrawEdge(headerRect,EDGE_RAISED,BF_RECT);
	DrawCenteredText(pDC,strings,headerRect);

	pDC->SetBkMode(oldMode);
}



//
//  Information about the cast string itself appears in the top half of the allocated space
//  The heats draw themselves in the bottom half.
//

void CScenMasterGraphViewport::DrawCastString(CCastString* pString, CDC* pDC)
{
	CRect stringRect = GetLocn(pString);
	stringRect.bottom = stringRect.top + STRING_HEIGHT;

	if ( ! pDC->RectVisible(stringRect) )
		return;

	CString text1;

	text1.Format("%04d-%02d-%02d, %ld tons, %d pcs, %02ldH%02dM",
				 pString->Id().Year(),
				 pString->Id().Week(),
				 pString->Id().StringId(),
				 long(0.5+pString->StringSummary().m_tons),
				 pString->StringSummary().m_numPieces,
				 pString->StringSummary().m_duration.GetTotalHours(),
				 pString->StringSummary().m_duration.GetMinutes());

	COLORREF clrBackg = ( pString->Status() == CCastString::STRING_STATUS_NEW
						  ? RGB(192,255,192)
						  : pString->Status() == CCastString::STRING_STATUS_909
						  ? RGB(255,255,128) 
						  : RGB(255,192,192) );


	int oldMode = pDC->SetBkMode(TRANSPARENT);
	
	pDC->FillSolidRect(stringRect,clrBackg);
	pDC->DrawEdge(stringRect,EDGE_RAISED,BF_RECT);
	DrawCenteredText(pDC,text1,stringRect);



	for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
		  ih != pString->Heats().end();
		  ++ih )

		DrawCastStringHeat( (*ih), pDC, clrBackg );


	pDC->SetBkMode(oldMode);

		
}


//
//  Each heat writes identifying information in its alloted space.
//

void CScenMasterGraphViewport::DrawCastStringHeat(CCastStringHeat& rHeat, 
											  CDC* pDC,
											  COLORREF clrBackg)
{
	CRect heatRect = GetLocn(rHeat);
	if ( ! pDC->RectVisible(heatRect) )
		return;

	Width begWid1;
	Width begWid2;
	Width endWid1;
	Width endWid2;

	if ( rHeat.StrandBegin(1) == rHeat.StrandEnd(1) ) {
		begWid1 = 0;
		endWid1 = 0;
	}
	else {
		begWid1 = (*(rHeat.StrandBegin(1)))->SlabWidth();
		endWid1 = (*(rHeat.StrandEnd(1)-1))->SlabWidth();
	}

	if ( rHeat.StrandBegin(2) == rHeat.StrandEnd(2) ) {
		begWid2 = 0;
		endWid2 = 0;
	}
	else {
		begWid2 = (*(rHeat.StrandBegin(2)))->SlabWidth();
		endWid2 = (*(rHeat.StrandEnd(2)-1))->SlabWidth();
	}


	vector<CString> strings;
	strings.resize(3);

	strings[0].Format("Tab %d, %ld tons, %s",
				 rHeat.TabNum(),
				 rHeat.HeatTons(),
				 rHeat.Spec());
	strings[1].Format("1. %2ld\" - %2ld\"",
				 long(begWid1),
				 long(endWid1));
	strings[2].Format("2. %2ld\" - %2ld\"",
				 long(begWid2),
				 long(endWid2));

	pDC->FillSolidRect(heatRect,clrBackg);
	pDC->DrawEdge(heatRect,EDGE_RAISED,BF_RECT);
	DrawCenteredText(pDC,strings,heatRect);
}


// 
//  Draw a single string centered horizontally within the bounding rectangle
//  Clip output to that rectangle.
//

void CScenMasterGraphViewport::DrawCenteredText(CDC* pDC, CString& text, CRect& boundRect)
{
	CSize textSize = pDC->GetOutputTextExtent(text);

	CPoint center = boundRect.CenterPoint();

	pDC->ExtTextOut(center.x - textSize.cx/2,
					center.y - textSize.cy/2,
					ETO_CLIPPED,
					boundRect,
					text,
					0);
}

//
//  Draw multiple strings, one per line, centered horizontally and vertically
//    within the bounding rectangle.
//  Output is clipped to that rectangle.
//

void CScenMasterGraphViewport::DrawCenteredText(CDC* pDC, vector<CString>& strings, CRect& boundRect)
{
	// compute combined height (sum)  and width (max) for all the strings

	int height = 0;
	int width = 0;

	{
		for ( vector<CString>::iterator is = strings.begin();
			  is != strings.end();
			  ++is ) {

			CSize textSize = pDC->GetTextExtent( (*is) );
			height += textSize.cy;
			width = max(width,textSize.cx);
		}
	}

	//  Determine the upper left corner of the text box

	CPoint center = boundRect.CenterPoint();
	int xPos = center.x - width/2;
	int yPos = center.y - height/2;

	// write each string, working down the page
	{
		for ( vector<CString>::iterator is = strings.begin();
			  is != strings.end();
			  ++is ) {
			pDC->ExtTextOut(xPos,yPos,ETO_CLIPPED,boundRect,(*is),0);
			CSize size = pDC->GetTextExtent((*is));
			yPos += size.cy;
		}
	}


}





//////////////////////////////////////////////////////////////
//
//  Computing locations
//
//////////////////////////////////////////////////////////////


//
//  get rid of cached info
//

void CScenMasterGraphViewport::ClearMaps()
{
	m_scenRectMap.clear();
	m_stringRectMap.clear();
	m_heatRectMap.clear();
}


//
// Compute the location of each selected scenario
//
// The vertical location of each scenario depends on the vertical location
//  of the preceding scenario

void CScenMasterGraphViewport::ComputeScenLocns()
{
	ClearMaps();

	CRect prevRect;

	prevRect.SetRectEmpty();

	for ( TScenVec::iterator is = m_selScens.begin();
		  is != m_selScens.end();
		  ++is ) {
		prevRect = ComputeScenLocn((*is),prevRect);
	}
}


//
//  Scenario rectangle is header + rectangles for its strings.
//  prevRect gives the rect of the previous scenario.

CRect CScenMasterGraphViewport::ComputeScenLocn(CCasterScen* pScen,
									 CRect& prevRect)
{
	int top = ( prevRect.IsRectEmpty() 
		        ? TOP_MARGIN 
				: prevRect.BottomRight().y + SCEN_VERT_SPACING );

	int bottom = top + SCEN_HEIGHT;

	int left = LEFT_MARGIN;

	int right = left + HEADER_WIDTH;

	// Initial header is 
	CRect rect(left,top,right,bottom);


	//  Add in the rectangles of its strings

	for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
		  is != pScen->CastStrings().end();
		  ++is ) 

		rect |= ComputeStringLocn((*is),rect);

	// we now know our place
	m_scenRectMap.insert( TScenRectMap::value_type(pScen,rect));

	return rect;
}


//
//  Compute the location of a cast string.
//  scenRect, tells us what has been computed for the scenario so far,
//    i.e. our top, bottom and left.  we will add our space on to the right.
//

CRect CScenMasterGraphViewport::ComputeStringLocn( CCastString* pString,
										 CRect& scenRect)
{

	int left = scenRect.BottomRight().x;
	int stringTop = scenRect.TopLeft().y;
	int stringBot = scenRect.BottomRight().y;
	int heatTop = stringTop+STRING_HEIGHT;
	
	// we allows ourselves at least 100 pixels of space
	CRect strRect(left,stringTop,left+100,stringBot);

	// compute the beginning coordinates for the heats
	CRect lastRect(left-2,heatTop,left,stringBot);

	// compute the areas for the heats
	for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
		  ih != pString->Heats().end();
		  ++ih ) {
		lastRect = ComputeHeatLocn( (*ih), lastRect );
		strRect |= lastRect;
	}

	// record the computed value.
	m_stringRectMap.insert( TStringRectMap::value_type(pString,strRect) );

	return strRect;
}


// 
// Compute the location for heat.
// lastRect gives us top, bottom and left side.
// size is 2 pixels per minute.

CRect CScenMasterGraphViewport::ComputeHeatLocn( CCastStringHeat& rHeat,
									   CRect& lastRect )
{
	int left	= lastRect.BottomRight().x;
	int top		= lastRect.TopLeft().y;
	int bottom	= lastRect.BottomRight().y;
	int right	= left+max(1,2*rHeat.Duration().GetTotalMinutes());

	CRect rect(left,top,right,bottom);

	m_heatRectMap.insert( THeatRectMap::value_type(&rHeat,rect) );

	return rect;
}


//
//  the following are failsafe retrievals of rectangles 
//    cached for scens, strings, heats
//

CRect CScenMasterGraphViewport::GetLocn(CCasterScen* pScen)
{
	CRect rect;
	rect.SetRectEmpty();

	TScenRectMap::iterator im = m_scenRectMap.find(pScen);

	if ( im != m_scenRectMap.end() )
		rect = (*im).second;

	return rect;
}

CRect CScenMasterGraphViewport::GetLocn(CCastString* pString)
{
	CRect rect;
	rect.SetRectEmpty();

	TStringRectMap::iterator im = m_stringRectMap.find(pString);

	if ( im != m_stringRectMap.end() )
		rect = (*im).second;

	return rect;
}

CRect CScenMasterGraphViewport::GetLocn(CCastStringHeat& rHeat)
{
	CRect rect;
	rect.SetRectEmpty();

	THeatRectMap::iterator im = m_heatRectMap.find(&rHeat);

	if ( im != m_heatRectMap.end() )
		rect = (*im).second;

	return rect;
}


//
//  Set the scrolling on the view
//


void CScenMasterGraphViewport::SetScrollInfo()
{
	CSize sizeTotal = ComputeSize();
	
	m_pView->SetScrollSizes(MM_TEXT, sizeTotal);
}



//  
//  the computed size for the view is the sum of all
//  rectangles for the scenarios, plus a little margin 
//  (the scenarios have computed margins on the top and left already).
//

CSize CScenMasterGraphViewport::ComputeSize() 
{
	CRect rect(0,0,1,1);

	for ( TScenVec::iterator is = m_selScens.begin();
		  is != m_selScens.end();
		  ++is )

		rect |= GetLocn( (*is) );

	rect.InflateRect(0,0,RIGHT_MARGIN,BOTTOM_MARGIN);

	return rect.Size();
}



//////////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////////


void CScenMasterGraphViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_SMASTER_CHANGE_SEL_SCENS:
		// change to selected scens caused by the CScenMasterCtrlViewport/Controller.
		// update our copy of the selected scens and update the display.
		{
			CScenMasterCtrlViewport::CScenSelChangeMsg* pM = dynamic_cast<CScenMasterCtrlViewport::CScenSelChangeMsg*>(pMsg);
			assert( pM != 0 );

			m_selScens = pM->SelScens(); // copy
		}
		//fall through

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
		// if only a change to available scens, we may have to update
		// this might fail on a deletion, so we clean up first
		CScenMasterCtrlViewport::ValidateScens(m_selScens);
		ComputeScenLocns();
		SetScrollInfo();
		Invalidate();
		break;

	default:
		// do nothing
		break;
	}
}



BEGIN_MESSAGE_MAP(CScenMasterGraphController, MvcController)
	//{{AFX_MSG_MAP(CScenMasterGraphController)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CScenMasterGraphController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}

