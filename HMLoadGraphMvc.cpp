// HMLoadGraphMvc.cpp: implementation of the CHMLoadGraphGanttViewport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "HMLoadGraphMvc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "MessageBus.h"
#include "GanttDisplay.h"
#include "HMLoadGraphBars.h"
#include "HMLoadGraphDoc.h"
#include "NTime.h"
#include "CastString.h"
#include "CastStringIdDlg.h"
#include "CasterScen.h"
#include "DateTimeDlg.h"
#include "EmailDlg.h"



////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
//  CHMLoadGraphGanttViewport/Controller
//
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
//   Basic viewport functionality
//
////////////////////////////////////////////////////////////


CMessageBus* CHMLoadGraphGanttViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(m_pModel);
}


BOOL CHMLoadGraphGanttViewport::OnCreate()
{
	return TRUE;
}


BOOL CHMLoadGraphGanttViewport::CreateController()
{
	m_pCtlr =  new CHMLoadGraphGanttController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


//
// Drawing is deferred to the gantt display
//

void CHMLoadGraphGanttViewport::Draw(CDC* pDC)
{
	m_pGanttDisplay->Chart()->OnDraw(pDC);
}





void CHMLoadGraphGanttViewport::OnInitialUpdate()
{
	assert( m_pDoc != 0);
	assert( m_pGanttDisplay != 0 );

	// Initialize colors

	static vector<COLORREF> dayColors;
	static vector<COLORREF> hourColors;
	static COLORREF dayTextColor = RGB(0xFF,0xFF,0xFF);

	if ( dayColors.size() == 0 ) {

		dayColors.push_back( RGB(0x00,0x00,0x50) );
		dayColors.push_back( RGB(0x00,0x00,0xA0) );

		hourColors.push_back( RGB(0x70,0x70,0xE0) );
		hourColors.push_back( RGB(0x30,0x20,0x60) );

	}


	//  Set up the gantt chart
	m_pGanttDisplay->SetMargins(5,5,5,5);

	m_pDayBar			= new CGanttDisplayDay(10,0,dayColors,dayTextColor);
	m_pHourBar			= new CGanttDisplayHour(10,0,hourColors,dayTextColor);
	m_pNumBars[0]		= new CHMLoadGraphNumericBar(10,0,0,this);
	m_pNumBars[1]		= new CHMLoadGraphNumericBar(10,5,1,this);
	m_pNumBars[2]		= new CHMLoadGraphNumericBar(10,0,2,this);
	m_pNumBars[3]		= new CHMLoadGraphNumericBar(10,0,3,this);
	m_pNumBars[4]		= new CHMLoadGraphNumericBar(10,0,4,this);
	m_pGraphBar			= new CHMLoadGraphGraphBar(180,15,0,1500,this);
	m_pCasterBars[0]	= 0;
	m_pCasterBars[1]	= new CHMLoadGraphCasterBar(30,5,1,this);
	m_pCasterBars[2]	= new CHMLoadGraphCasterBar(30,5,2,this);
	m_pCasterBars[3]	= new CHMLoadGraphCasterBar(30,5,3,this);
	m_pLoadBar			= new CHMLoadGraphLoadCountBar(140,15,-40,40,this);


	m_pDayBar->AddLabel("Day");
	m_pHourBar->AddLabel("Hour");

	m_pHourBar->IsMilitary() = false;
	m_pHourBar->Use2Digits() = true;

	m_pNumBars[1]->AddLabel("#1SC");
	m_pNumBars[2]->AddLabel("#2SC");
	m_pNumBars[3]->AddLabel("#3CC");
	m_pNumBars[0]->AddLabel("Demand");
	m_pNumBars[4]->AddLabel("Supply");

	m_pCasterBars[1]->AddLabel("#1SC");
	m_pCasterBars[2]->AddLabel("#2SC");
	m_pCasterBars[3]->AddLabel("#3CC");

	

	m_pLoadBar->AddLabel( "-40", -40 );
	m_pLoadBar->AddLabel( "-30", -30 );
	m_pLoadBar->AddLabel( "-20", -20 );
	m_pLoadBar->AddLabel( "-10", -10 );
	m_pLoadBar->AddLabel(   "0",   0 );
	m_pLoadBar->AddLabel(  "10",  10 );
	m_pLoadBar->AddLabel(  "20",  20 );
	m_pLoadBar->AddLabel(  "30",  30 );
	m_pLoadBar->AddLabel(  "40",  40 );

	m_pGraphBar->AddLabel(  "  0",    0 );
	m_pGraphBar->AddLabel(  "250",  250 );
	m_pGraphBar->AddLabel(  "500",  500 );
	m_pGraphBar->AddLabel(  "750",  750 );
	m_pGraphBar->AddLabel( "1000", 1000 );
	m_pGraphBar->AddLabel( "1250", 1250 );
	m_pGraphBar->AddLabel( "1500", 1500 );

	m_pLoadBar		->SetBgColor(RGB(192,192,192));
	m_pGraphBar		->SetBgColor(RGB(168,168,168));
	m_pNumBars[0]	->SetBgColor(RGB(192,192,192));
	m_pNumBars[1]	->SetBgColor(RGB(192,192,192));
	m_pNumBars[2]	->SetBgColor(RGB(192,192,192));
	m_pNumBars[3]	->SetBgColor(RGB(192,192,192));
	m_pNumBars[4]	->SetBgColor(RGB(192,192,192));
	m_pDayBar		->SetBgColor(RGB(192,192,192));
	m_pHourBar		->SetBgColor(RGB(192,192,192));
	m_pCasterBars[1]->SetBgColor(RGB(192,192,192));
	m_pCasterBars[2]->SetBgColor(RGB(192,192,192));
	m_pCasterBars[3]->SetBgColor(RGB(192,192,192));


	vector <CGanttDisplayBar*> bars;

	bars.push_back(m_pLoadBar);
	bars.push_back(m_pGraphBar);
	bars.push_back(m_pNumBars[1]);
	bars.push_back(m_pNumBars[2]);
	bars.push_back(m_pNumBars[3]);
	bars.push_back(m_pNumBars[0]);
	bars.push_back(m_pNumBars[4]);
	bars.push_back(m_pDayBar);
	bars.push_back(m_pHourBar);
	bars.push_back(m_pCasterBars[1]);
	bars.push_back(m_pCasterBars[2]);
	bars.push_back(m_pCasterBars[3]);

	m_pGanttDisplay->AddBars(bars);

	m_pGanttDisplay->SetPixelsPerXUnit(30.0/3600.0);
	ComputeLimits();
}




void CHMLoadGraphGanttViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_HM_SCEN_UPDATE:
		ComputeLimits();
		Invalidate();
		break;
	}
}




/////////////////////////////////////////////////////
//
//  Computing colors
//
/////////////////////////////////////////////////////


// static
COLORREF CHMLoadGraphGanttViewport::CasterColor(int caster)
{
	COLORREF color = RGB(0x00,0x00,0x00);

	switch (caster) {
	case 1:
		color = RGB(0x00,0xC0,0x00);
		break;

	case 2:
		color = RGB(0x00,0x80,0x80);
		break;

	case 3:
		color = RGB(0x00,0x00,0xC0);
		break;
	}

	return color;
}


// static
COLORREF CHMLoadGraphGanttViewport::HeatColor(int caster, int heatIndex)
{
	COLORREF color = RGB(0x00,0x00,0x00);

	switch (caster) {
	case 1:
		if ( heatIndex % 2 == 0 )
			color = RGB(0x00,0x40,0x80);
		else
			color = RGB(0x00,0x40,0xC0);
		break;

	case 2:
		if ( heatIndex % 2 == 0 )
			color = RGB(0x00,0x80,0x80);
		else
			color = RGB(0x00,0x80,0xC0);
		break;

	case 3:
		if ( heatIndex % 2 == 0 )
			color = RGB(0x00,0x80,0x40);
		else
			color = RGB(0x00,0xC0,0x40);
		break;
	}

	return color;
}


// static
COLORREF CHMLoadGraphGanttViewport::DemandColor()
{
	return RGB(0x40,0x40,0x40);
}

COLORREF CHMLoadGraphGanttViewport::SupplyColor()
{
	return RGB(0xFF,0x80,0x00);
}



/////////////////////////////////////////////////////////
//
//  Other drawing details
//
/////////////////////////////////////////////////////////



void CHMLoadGraphGanttViewport::ComputeLimits()
{
	CHMLoadGraphDoc* pDoc = GetDocument();
	
	CTime et;
	CTime lt;

	pDoc->ComputeTimeRange(et,lt);

	m_pGanttDisplay->Chart()->SetXLimits(NTime::ThisDayMidnight(et.GetTime()),
										 NTime::NextDayMidnight(lt.GetTime()));
		
}




// static
void CHMLoadGraphGanttViewport::DrawCenteredText(CDC* pDC, 
										CString& text,
										CRect& boundRect)
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


// static
void CHMLoadGraphGanttViewport::DrawCenteredText(CDC* pDC, 
										vector<CString>& strings, 
										CRect& boundRect)
{
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

	CPoint center = boundRect.CenterPoint();
	int xPos = center.x - width/2;
	int yPos = center.y - height/2;

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



void CHMLoadGraphGanttViewport::ScrollTime(long seconds)
{
	CGanttChart* pChart = m_pGanttDisplay->Chart();

	CPoint pt = pChart->GetScrollPosition();

	XUnit currentTime = pChart->XlateXCoordToXUnitNoTruncate(pt.x);
	XUnit newTime = currentTime + seconds;

#if _DEBUG
	afxDump << "Pt = " << pt 
			<< ", currentTime = " << currentTime
			<< ", newTime = " << newTime
			<< "\n";
#endif

	if ( newTime < pChart->GetStartXUnit() ) {
#if _DEBUG
		afxDump << "Setting earlier limit: [" << pChart->GetStartXUnit() - 3600*24 
				<< "," << pChart->GetFinishXUnit()
				<< "]\n";
#endif
		m_pGanttDisplay->SetXLimits(pChart->GetStartXUnit()-3600*24,
				   pChart->GetFinishXUnit());
	}
	else {
		CRect rect;
		pChart->GetClientRect(&rect);
		XUnit rightXUnit = pChart->XlateXCoordToXUnitNoTruncate(pt.x + rect.Width())+seconds;
		if ( rightXUnit > pChart->GetFinishXUnit() ) {
#if _DEBUG
		afxDump << "Setting later limit: [" << pChart->GetStartXUnit() 
				<< "," << pChart->GetFinishXUnit()  + 3600*24
				<< "]\n";
#endif
			m_pGanttDisplay->SetXLimits(pChart->GetStartXUnit(),
					   pChart->GetFinishXUnit() + 24*3600);
		}
	}

	Position newXCoord = pChart->XlateXUnitToXCoordNoTruncate(newTime);
	pChart->ScrollToPosition(CPoint(newXCoord,pt.y));

	// do it now, so we can draw our tracking rectangle without a problem.
	pChart->UpdateWindow();

}




BEGIN_MESSAGE_MAP(CHMLoadGraphGanttController, MvcController)
	//{{AFX_MSG_MAP(CHMLoadGraphGanttController)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CHMLoadGraphGanttController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
//  CHMLoadGraphScenSeqViewport/Controller
//
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
//   Basic viewport functionality
//
////////////////////////////////////////////////////////////


CMessageBus* CHMLoadGraphScenSeqViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(m_pModel);
}


BOOL CHMLoadGraphScenSeqViewport::OnCreate()
{
	return TRUE;
}


BOOL CHMLoadGraphScenSeqViewport::CreateController()
{
	m_pCtlr = new CHMLoadGraphScenSeqController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}




void CHMLoadGraphScenSeqViewport::OnInitialUpdate()
{
	assert( m_pDoc != 0);
	assert( m_pLbeStrings != 0 );

	
	assert(m_caster == 1 || m_caster == 2 || m_caster == 3);
	InitStringList();
}



void CHMLoadGraphScenSeqViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_HM_SCEN_UPDATE:
		InitStringList();
		break;
	}
}



void CHMLoadGraphScenSeqViewport::InitStringList()
{
	m_pLbeStrings->GetListBoxPtr()->ResetContent();

	CHMLoadGraphDoc* pDoc = GetDocument();
	vector<CCastString*>& strings = pDoc->GetStrings(m_caster);

	ostrstream ostr;

	int count = 0;
	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is, ++count ) {


		ostr << setfill('0')
			 << setw(2) << (*is)->Id().Week()
			 << "-"
			 << setw(2) << (*is)->Id().StringId()
			 << " : "
			 << setfill(' ');


		CString strSpec = CHMLoadGraphDoc::FindSpecToUse( (*is) );
		if ( strSpec.GetLength() < 7 )
			strSpec = (strSpec + "???????").Left(7);

		ostr << LPCTSTR(strSpec.Left(3))  << "-" 
			 << LPCTSTR(strSpec.Mid(3,2)) << "-"
			 << LPCTSTR(strSpec.Right(2)) << " ";

		ostr << setfill('0') << setw(2) << long((*is)->Heats().size()) << " hts, ";

		long strand1Begin;
		long strand1End;
		long strand2Begin;
		long strand2End;
		CHMLoadGraphDoc::FindWidthsToUse((*is),
										 strand1Begin,
										 strand1End,
										 strand2Begin,
										 strand2End);

		ostr << setw(2) << strand1Begin
			 << "-"
			 << setw(2) << strand1End
			 << "\"";
	
		if ( m_caster != 2 ) {

			ostr << ", ";
				ostr << setw(2) << strand2Begin
				     << "-"
					 << setw(2) << strand2End
					 << "\"";
		}

		ostr << ends;

		int index = m_pLbeStrings->GetListBoxPtr()->AddString( ostr.str() );
		m_pLbeStrings->GetListBoxPtr()->SetItemData(index,DWORD(count));
		ostr.freeze(false);
		ostr.seekp(0);
	}
}
			 
void CHMLoadGraphScenSeqViewport::OnButtonUpdateStringSeq() 
{
	CListBox* pBox = m_pLbeStrings->GetListBoxPtr();

	vector<int> indexes;

	{
		for ( int i=0; i<pBox->GetCount(); ++i ) 
			indexes.push_back( int( pBox->GetItemData(i) ) );
	}

	//	I don't trust this code.  
	//	Let's run some sanity checks:
	//	We should have a permutation in indexes.
	//  Make sure all entries are in range [0,size)
	//  Make sure each entry occurs exactly once.

	CHMLoadGraphDoc* pDoc = GetDocument();
	vector<CCastString*>& strings = pDoc->GetStrings(m_caster);

	if ( indexes.size() != strings.size() ) {
		GetWnd()->MessageBox("Message 123: Wrong number of strings in box.  Please notify Miller.");
		return;
	}

	{
		for ( int i=0; i<indexes.size(); ++i )
			if ( indexes[i] < 0 || indexes[i] >= indexes.size() ) {
				GetWnd()->MessageBox("Message 124: Wrong number of strings in box.  Please notify Miller.");
				return;
			}
	}

	bool* flags = new bool [indexes.size() ];

	{
		for ( int i=0; i<indexes.size(); ++i ) 
			flags[i] = false;
	}

	{	
		for ( int i=0; i<indexes.size(); ++i ) {
			if ( flags[ indexes[i] ] ) {
				GetWnd()->MessageBox("Message 125: Index appears twice in box.  Please notify Miller.");
				delete [] flags;
				return;
			}
			flags[ indexes[i] ] = true;
		}
	}
		
	{
		for ( int i=0; i<indexes.size(); ++i ) {
			if ( ! flags[ i ] ) {
				GetWnd()->MessageBox("Message 126: Index fails to appear in box.  Please notify Miller.");
				delete [] flags;
				return;
			}
		}
	}

	// Looks like a decent permutation.
	// Lets act on it.

	pDoc->ResequenceStrings(m_caster,indexes);
}



void CHMLoadGraphScenSeqViewport::OnButtonResetStringSeq() 
{
	CHMLoadGraphDoc* pDoc = GetDocument();

	pDoc->RefreshStrings(m_caster);

}




BEGIN_MESSAGE_MAP(CHMLoadGraphScenSeqController, MvcController)
	//{{AFX_MSG_MAP(CHMLoadGraphScenSeqController)
	ON_BN_CLICKED(IDC_BUTTON_RESET_STRING_SEQ, OnButtonResetStringSeq)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_STRING_SEQ, OnButtonUpdateStringSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CHMLoadGraphScenSeqController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


void CHMLoadGraphScenSeqController::OnButtonUpdateStringSeq() 
{
	GetViewport()->OnButtonUpdateStringSeq();
}



void CHMLoadGraphScenSeqController::OnButtonResetStringSeq() 
{
	GetViewport()->OnButtonResetStringSeq();
}



////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport/Controller
//
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport
//
////////////////////////////////////////////////////////////////


// a few static data members

const COLORREF CHMLoadGraphSheetViewport::sm_colorBack		  = RGB(192,192,192);
const COLORREF CHMLoadGraphSheetViewport::sm_colorDateBack	  = RGB(192,192,192);
const COLORREF CHMLoadGraphSheetViewport::sm_colorDateFore	  = RGB(0,0,0);
const COLORREF CHMLoadGraphSheetViewport::sm_colorCasterBack[4] = { RGB(192,192,192),
																		  RGB(192,192,192),
																		  RGB(192,192,192),
																		  RGB(192,192,192) };
const COLORREF CHMLoadGraphSheetViewport::sm_colorCasterFore[4] = { RGB(255,255,255),
																		  RGB(255,255,255),
																		  RGB(255,255,255),
																		  RGB(255,255,255)	};
const COLORREF CHMLoadGraphSheetViewport::sm_colorLoad[4][3] = 

{ 
	{ RGB(  0,  0,  0 ), RGB(  0,  0,  0 ), RGB(  0,  0,  0 ) },
	{ RGB( 16, 80,  0 ), RGB( 32,164,  0 ), RGB( 24,122,  0 ) },
	{ RGB(  0, 66, 82 ), RGB(  0,132,164 ), RGB(  0, 98,132 ) },
	{ RGB( 82,  0, 22 ), RGB(164,  0, 44 ), RGB(122,  0, 32 ) } 
};

const COLORREF CHMLoadGraphSheetViewport::sm_colorLoadBack = RGB(192,192,192);

const COLORREF CHMLoadGraphSheetViewport::sm_colorSupply	= RGB(0xFF,0x80,0x00);


const COLORREF CHMLoadGraphSheetViewport::sm_color909Border = RGB(0xFF,0xFF,0x00);
const COLORREF CHMLoadGraphSheetViewport::sm_color910Border = RGB(0x70,0x70,0x00);

const COLORREF CHMLoadGraphSheetViewport::sm_colorSelectedHilite = RGB(0xFF,0x80,0x80);




//////////////////////////////////////////////////
//
//  standard viewport stuff
//
/////////////////////////////////////////////////

CHMLoadGraphSheetViewport::CHMLoadGraphSheetViewport()
{
	m_pDoc = 0;
	 
	m_initialized = false;

	m_numRows = 2;
	m_numCols = 4;
	m_startDay = CTime::GetCurrentTime();

	m_startDay = CTime(m_startDay.GetYear(),
					   m_startDay.GetMonth(),
					   m_startDay.GetDay(),
					   0,0,0);

	m_pSelectedString = 0;

	m_startDay = CTime::GetCurrentTime();

	m_startDay = CTime(m_startDay.GetYear(),
					   m_startDay.GetMonth(),
					   m_startDay.GetDay(),
					   0,0,0);
}




CMessageBus* CHMLoadGraphSheetViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(m_pModel);
}



BOOL CHMLoadGraphSheetViewport::OnCreate()
{
	return TRUE;
}


BOOL CHMLoadGraphSheetViewport::CreateController()
{
	m_pCtlr = new CHMLoadGraphSheetController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


void CHMLoadGraphSheetViewport::OnInitialUpdate()
{
	assert( m_pDoc != 0);

	m_initialized = true;

	SetDisplaySize(2,4);
	SetNominalScrollSize();	
	
	m_holdArea.Initialize(this->GetWnd());
}


void CHMLoadGraphSheetViewport::Draw(CDC* pDC)
{
	m_holdArea.Draw(pDC);

	for ( vector<CDayArea>::iterator id = m_days.begin();
		  id != m_days.end();
		  ++id )
		(*id).Draw(pDC);
}


CSize CHMLoadGraphSheetViewport::SetSize(int cx, int cy) 
{
	CSize result = MvcViewport::SetSize(cx,cy);
	if ( m_initialized ) {
		SetDisplaySize(m_numRows,m_numCols);
		SetNominalScrollPos();
	}
	return result;
}


void CHMLoadGraphSheetViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	case MSG_ID_HM_SCEN_UPDATE:
		SetDisplaySize(m_numRows,m_numCols);
		Invalidate();
		break;
	}
}




//////////////////////////////////
//
//  Some simple drawing functions
//
//////////////////////////////////



//  This is duplicate code from ScenMasterGraphView.
//  We should consolidate, someday.


// static
void CHMLoadGraphSheetViewport::DrawCenteredText(CDC* pDC, 
											 const CString& text, 
											 const CRect& boundRect)
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




// static
void CHMLoadGraphSheetViewport::DrawCenteredText(CDC* pDC, 
											 const vector<CString>& strings, 
											 const CRect& boundRect)
{
	CSize sizeText = GetTextSize(pDC,strings);
	
	CPoint center = boundRect.CenterPoint();
	int xPos = center.x - sizeText.cx/2;
	int yPos = center.y - sizeText.cy/2;

	{
		for ( vector<CString>::const_iterator is = strings.begin();
			  is != strings.end();
			  ++is ) {
			pDC->ExtTextOut(xPos,yPos,ETO_CLIPPED,boundRect,(*is),0);
			CSize size = pDC->GetTextExtent((*is));
			yPos += size.cy;
		}
	}

}



// static
CSize CHMLoadGraphSheetViewport::GetTextSize(CDC* pDC,
									   const vector<CString>& strings)
{
	int height = 0;
	int width = 0;

	{
		for ( vector<CString>::const_iterator is = strings.begin();
			  is != strings.end();
			  ++is ) {

			CSize textSize = pDC->GetTextExtent( (*is) );
			height += textSize.cy;
			width = max(width,textSize.cx);
		}
	}

	return CSize(width,height);
}


////////////////////////////////////////////////////////////////
//
//  Drawing functions
//
/////////////////////////////////////////////////////////////////



void CHMLoadGraphSheetViewport::SetDisplaySize(int nr, int nc)
{
	m_numRows = nr;
	m_numCols = nc;

	m_days.clear();
	m_days.resize(nr*nc);

	CRect rectClient;
	GetWnd()->GetClientRect(&rectClient);

	int widthHold = 100;
	int leftXHold = max(85,rectClient.Width()-100);
	CRect rectHold(leftXHold,0,leftXHold+90,rectClient.Height());

	CHMLoadGraphDoc* pDoc = GetDocument();

	int widthDays = max(85,leftXHold-25);

	int rowHeight = rectClient.Height()/nr;
	int colWidth  = int(widthDays/nc);
	CSize size(colWidth,rowHeight);

	for ( int r=0; r<nr; ++r ) {
		for ( int c=0; c<nc; ++c ) {
			int daySerial = r*nc + c;
			CTime day = m_startDay + CTimeSpan(daySerial,0,0,0);
			CRect rect( CPoint(colWidth*c, rowHeight*r),
						size );
			m_days[daySerial].ComputeRects(this,day,rect);
			m_days[daySerial].ComputeStringRects();
			m_days[daySerial].ComputeLoadGraphValues();
		}
	}

	m_holdArea.ComputeRects(this,rectHold);
}




void CHMLoadGraphSheetViewport::SetNominalScrollSize()
{
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMax = 7;
	info.nMin = 0;
	info.nPage = 1;
	info.nPos = 3;

	GetWnd()->SetScrollInfo(SB_HORZ,&info);
	GetWnd()->SetScrollInfo(SB_VERT,&info);
}




void CHMLoadGraphSheetViewport::SetNominalScrollPos()
{
	SetNominalScrollPos(SB_HORZ);
	SetNominalScrollPos(SB_VERT);
}

void CHMLoadGraphSheetViewport::SetNominalScrollPos(int bar)
{
	int pos = GetWnd()->GetScrollPos(bar);
	if ( pos <= 0 || pos >= 6 ) 
		GetWnd()->SetScrollPos(bar,3);

}

void CHMLoadGraphSheetViewport::IncrementScrollPos(int bar, int incr)
{
	int newPos = min(6,max(0,GetWnd()->GetScrollPos(bar) + incr));
	GetWnd()->SetScrollPos(bar,newPos);
	SetNominalScrollPos(bar);
}



///////////////////////////////////////////////////////////////
//
//  Mouse
//
///////////////////////////////////////////////////////////////


bool CHMLoadGraphSheetViewport::HitTest(const CPoint& point,
									CHMLoadGraphSheetViewport::CHitData& rHitData) const
{
	bool result = m_holdArea.HitTest(point,rHitData);

	if ( ! result )  {
		vector<CDayArea>::const_iterator id;
		for ( id = m_days.begin();
			  id != m_days.end();
			  ++id ) {

			if ( (result = (*id).HitTest(point,rHitData)) )
				break;
		}
	}

	return result;
}



///////////////////////////////////////////////////////////////
//
//  Selected string
//
///////////////////////////////////////////////////////////////

void CHMLoadGraphSheetViewport::UndrawSelectedString()
{
	CCastString* pString = m_pSelectedString;
	m_pSelectedString = 0;

	m_holdArea.RedrawString(pString);

	for ( vector<CDayArea>::iterator id = m_days.begin();
		  id != m_days.end();
		  ++id )
		(*id).RedrawString(pString);

	m_pSelectedString = pString;
}

void CHMLoadGraphSheetViewport::RedrawSelectedString()
{
	m_holdArea.RedrawString(m_pSelectedString);

	for ( vector<CDayArea>::iterator id = m_days.begin();
		  id != m_days.end();
		  ++id )
		(*id).RedrawString(m_pSelectedString);
}


void CHMLoadGraphSheetViewport::SelectString(CCastString* pString)
{
	if ( m_pSelectedString != 0 )
		UndrawSelectedString();
	m_pSelectedString = pString;
	RedrawSelectedString();
}


bool CHMLoadGraphSheetViewport::SelectStringAtPoint(CPoint pt,CHitData& rHitData)
{
	bool result = HitTest(pt,rHitData);

	if ( rHitData.m_hitArea == HT_CASTER || rHitData.m_hitArea == HT_HOLD_AREA )
		SelectString(rHitData.m_pString);
	else
		SelectString(0);

#if 0
	{
		afxDump << "SelectStringAtPoint " << pt
				<< ", found";
		if ( m_pSelectedString == 0 )
			afxDump << " nothing!\n";
		else
			afxDump << "string " << m_pSelectedString->Id().Caster()
					<< " " << m_pSelectedString->Id().Year()
					<< "-" << m_pSelectedString->Id().Week()
					<< "-" << m_pSelectedString->Id().StringId()
					<< "\n";
	}
#endif

	return m_pSelectedString != 0;
}


///////////////////////////////////////////////////////////////
//
//  Location, location, location
//
///////////////////////////////////////////////////////////////




const CHMLoadGraphSheetViewport::CDayArea& CHMLoadGraphSheetViewport::FindBestDayY(int caster,
																		   const CPoint& point)
{
	//  iterate through all days
	//  Find one where y coord of point is either within the designated caster area
	//    or the one with the minimum distance.

	int bestDist = INT_MAX;
	vector<CDayArea>::iterator iBestDay = m_days.begin();

	for ( vector<CDayArea>::iterator id = m_days.begin();
		  id != m_days.end();
		  ++id ) {
		const CRect& rect =  (*id).m_rectCasterText[caster];
		if ( rect.top <= point.y && point.y <= rect.bottom ) {
			iBestDay = id;
			break;
		}
		else {
			int dist = min( abs(rect.top - point.y), abs(rect.bottom - point.y) );
			if ( dist < bestDist ) {
				bestDist = dist;
				iBestDay = id;
			}
		}
	}

	return *iBestDay;
}

const CHMLoadGraphSheetViewport::CDayArea* CHMLoadGraphSheetViewport::FindContainingDay(int caster,
																				const CPoint& point)
{
	//  iterate through all days
	//  find day containing point

	for ( vector<CDayArea>::iterator id = m_days.begin();
		  id != m_days.end();
		  ++id ) {
		const CRect& rect =  (*id).m_rectCasterText[caster];
		if ( rect.PtInRect(point) )
			return &*id;
	}
	return 0;
}



void CHMLoadGraphSheetViewport::FindConfiningBar(int caster, 
											 const CPoint& point, 
											 int& rTop, 
											 int& rBottom)
{
	const CDayArea& day = FindBestDayY(caster,point);
	const CRect& bestRect = day.m_rectCasterText[caster];
	rTop = bestRect.top;
	rBottom = bestRect.bottom;
}



bool CHMLoadGraphSheetViewport::FindTimeAtPoint(const CPoint& point,int caster,CTime& time)
{
	const CDayArea* pDay = FindContainingDay(caster,point);
	if ( pDay == 0 ) {
		time = CTime::GetCurrentTime();
		return false;
	} 
	else {
		time = pDay->m_day + 3600*24*(point.x - pDay->m_rectCasterText[caster].left)/pDay->m_rectCasterText[caster].Width();
		return true;
	}
}



/////////////////////////////////////////////////////////////////
//
//  Dragging support
//
/////////////////////////////////////////////////////////////////

void CHMLoadGraphSheetViewport::MoveTrackingOutline(CDC& dcString,
												    CBitmap* pBitmap,
													const CRect& newRect,
													const CString& newString,
													const CRect& oldRect,
													const CString& oldString)
{
	CClientDC dc(this->GetWnd());
	OnPrepareDC(&dc);
	CDCState state(&dc);

	DrawTrackingString(dc,dcString,pBitmap,oldRect,oldString);

	dc.DrawDragRect(&newRect,CSize(5,5),&oldRect,CSize(5,5),0,0);

	DrawTrackingString(dc,dcString,pBitmap,newRect,newString);
}


void CHMLoadGraphSheetViewport::DrawTrackingOutline(CDC& dcString,
													CBitmap* pBitmap,
													const CRect& rect, 
												    const CString& str)
{
	CClientDC dc(this->GetWnd());
	CDCState state(&dc);

	dc.DrawDragRect(&rect,CSize(5,5),0,CSize(5,5),0,0);

	DrawTrackingString(dc,dcString,pBitmap,rect,str);
}

void CHMLoadGraphSheetViewport::DrawTrackingString(CDC& dc,
												   CDC& dcString,													
												   CBitmap* pBitmap,
												   const CRect& rect, 
												   const CString& str)
{
	CBitmap* pBM = dcString.SelectObject(pBitmap);
	dcString.DrawText(str,CRect(0,0,200,200),DT_LEFT|DT_TOP|DT_WORDBREAK|DT_NOPREFIX);
	CSize sizeStr = dcString.GetTextExtent(str);
	
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	dc.BitBlt(rect.left+5,rect.top+5,
			  min(sizeStr.cx,bm.bmWidth),
			  min(sizeStr.cy,bm.bmHeight),
			  &dcString,0,0,
			  SRCINVERT);

	dcString.SelectObject(pBM);

}



///////////////////////////////////////////////////////////////
//
//  Operations
//
//////////////////////////////////////////////////////////////

void CHMLoadGraphSheetViewport::SetStartDay(CTime newVal)
{
	m_startDay = newVal;
	SetDisplaySize(m_numRows,m_numCols);
	Invalidate();
}


void CHMLoadGraphSheetViewport::DoVertScroll(CTimeSpan delta, int scrollDelta)
{
	IncrementScrollPos(SB_VERT,scrollDelta);

	m_startDay += delta;
	
	SetDisplaySize(m_numRows,m_numCols);
	Invalidate();
}


void CHMLoadGraphSheetViewport::DoHorizScroll(CTimeSpan delta, int scrollDelta)
{
	IncrementScrollPos(SB_HORZ,scrollDelta);

	m_startDay += delta;
	
	SetDisplaySize(m_numRows,m_numCols);
	Invalidate();
}



void CHMLoadGraphSheetViewport::DeltaRowCol(int rDelta, int cDelta)
{
	SetDisplaySize(m_numRows+rDelta,m_numCols+cDelta);
	Invalidate();
}






void CHMLoadGraphSheetViewport::SetStringStartTime(CCastString* pString,
											   CTime time)
{

	pString->SetCastStartTime(time);

	CHMLoadGraphDoc* pDoc = GetDocument();
	pDoc->GetCurrentCasterScen(pString->Id().Caster())->SetModified(true);
	pDoc->RefreshStrings(pString->Id().Caster());
}


void CHMLoadGraphSheetViewport::MoveStringToHoldArea(CCastString* pString)
{
	pString->SetIsStartTimeValid(false);

	CHMLoadGraphDoc* pDoc = GetDocument();
	pDoc->GetCurrentCasterScen(pString->Id().Caster())->SetModified(true);
	pDoc->SetStrings();
	pDoc->RefreshStrings(pString->Id().Caster());
}




////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetController
//
////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CHMLoadGraphSheetController, MvcController)
	//{{AFX_MSG_MAP(CHMLoadGraphSheetController)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_EDIT_CREATE_STRING, OnEditCreateString)
	ON_COMMAND(ID_VIEW_SET_DATE, OnViewSetDate)
	ON_COMMAND(ID_VIEW_DAY_PLUS, OnViewDayPlus)
	ON_COMMAND(ID_VIEW_DAY_MINUS, OnViewDayMinus)
	ON_COMMAND(ID_VIEW_WEEK_MINUS, OnViewWeekMinus)
	ON_COMMAND(ID_VIEW_WEEK_PLUS, OnViewWeekPlus)
	ON_COMMAND(ID_VIEW_ADD_COL, OnViewAddCol)
	ON_COMMAND(ID_VIEW_ADD_ROW, OnViewAddRow)
	ON_COMMAND(ID_VIEW_DEL_COL, OnViewDelCol)
	ON_COMMAND(ID_VIEW_DEL_ROW, OnViewDelRow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ADD_COL, OnUpdateViewAddCol)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ADD_ROW, OnUpdateViewAddRow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEL_COL, OnUpdateViewDelCol)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEL_ROW, OnUpdateViewDelRow)
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_DELETE_STRING, OnEditDeleteString)
	ON_COMMAND(ID_EDIT_STRING_INFO, OnEditStringInfo)
	ON_COMMAND(ID_EDIT_MOVE_TO_HOLD_AREA, OnEditMoveToHoldArea)
	ON_COMMAND(ID_VIEW_SHOW910S, OnViewShow910s)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW910S, OnUpdateViewShow910s)
	ON_COMMAND(ID_VIEW_SCROLLHOLDDOWN, OnViewScrollholddown)
	ON_BN_CLICKED(ID_VIEW_SCROLLHOLDDOWN, OnViewScrollholddown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCROLLHOLDDOWN, OnUpdateViewScrollholddown)
	ON_COMMAND(ID_VIEW_SCROLLHOLDUP, OnViewScrollholdup)
	ON_BN_CLICKED(ID_VIEW_SCROLLHOLDUP, OnViewScrollholdup)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCROLLHOLDUP, OnUpdateViewScrollholdup)
	ON_COMMAND(ID_EDIT_LOADHM, OnEditLoadhm)
	ON_COMMAND(ID_EDIT_SENDHM, OnEditSendhm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMessageBus* CHMLoadGraphSheetController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


//  This is an MFC framework function
//  do we need this?
#if 0
BOOL CHMLoadGraphSheetController::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	CTimeSpan delta;

	if( sizeScroll.cy > 0 )
		delta = CTimeSpan(m_numRows,0,0,0);
	else if ( sizeScroll.cy < 0 )
		delta = CTimeSpan(-m_numRows,0,0,0);
	else if ( sizeScroll.cx > 0 ) 
		delta = CTimeSpan(1,0,0,0);
	else if ( sizeScroll.cy < 0 )
		delta = CTimeSpan(-1,0,0,0);
	else
		return FALSE;

	m_startDay += delta;
	
	SetDisplaySize(m_numRows,m_numCols);
	Invalidate();

	SetNominalScrollPos();

	return true;
}
#endif



/////////////////////////////////////////////////////////////////////////
//
//  Windows events
//
/////////////////////////////////////////////////////////////////////////


void CHMLoadGraphSheetController::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CTimeSpan delta(0,0,0,0);
	int scrollDelta = 0;

	int numCols = GetViewport()->NumCols();

	switch (nSBCode) {

	case SB_TOP:		//   Scroll to top. 
	case SB_LINEUP:		//   Scroll one line up.
	case SB_PAGEUP:		//   Scroll one page up.
		delta = CTimeSpan(-numCols,0,0,0);
		scrollDelta = -1;
		break;

	case SB_BOTTOM:		//   Scroll to bottom.
	case SB_LINEDOWN:	//   Scroll one line down.
	case SB_PAGEDOWN:	//   Scroll one page down.
		delta = CTimeSpan(numCols,0,0,0);
		scrollDelta = 1;
		break;

	case SB_THUMBPOSITION:	//   Scroll to the absolute position. 
	case SB_THUMBTRACK:		//   Drag scroll box to specified position. 
							//   The current position is provided in nPos.
		{
			SCROLLINFO info;
			info.cbSize = sizeof(SCROLLINFO);
			info.fMask = SIF_POS;
			GetViewport()->GetWnd()->GetScrollInfo(SB_VERT,&info,SIF_POS);
			delta = CTimeSpan((nPos - info.nPos)*numCols,0,0,0);
			scrollDelta = nPos-info.nPos;
		}
		break;

	case SB_ENDSCROLL:
		break;
	

	default:
		break;
	}

	GetViewport()->DoVertScroll(delta,scrollDelta);
}


void CHMLoadGraphSheetController::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CTimeSpan delta(0,0,0,0);
	int scrollDelta = 0;


	switch ( nSBCode ) {
	case SB_LEFT:		//	Scroll to far left.
	case SB_LINELEFT:	//	Scroll left.
	case SB_PAGELEFT:   //	Scroll one page left.
		delta = CTimeSpan(-1,0,0,0);  
		scrollDelta = -1;
		break;

	case SB_LINERIGHT:	//	Scroll right.
	case SB_PAGERIGHT:	//	Scroll one page right.
	case SB_RIGHT:		//	Scroll to far right.
		delta = CTimeSpan(1,0,0,0);  
		scrollDelta = 1;
		break;

	case SB_THUMBPOSITION:	//	Scroll to absolute position. 
	case SB_THUMBTRACK:		//	Drag scroll box to specified position. 
							//	The current position is specified by the nPos parameter. 

		{
			SCROLLINFO info;
			info.cbSize = sizeof(SCROLLINFO);
			info.fMask = SIF_POS;
			GetViewport()->GetWnd()->GetScrollInfo(SB_HORZ,&info,SIF_POS);
			delta = CTimeSpan(nPos - info.nPos,0,0,0);
			scrollDelta = nPos-info.nPos;
		}
		break;

	case SB_ENDSCROLL:
		break;
	

	default:
		break;
	}

	GetViewport()->DoHorizScroll(delta,scrollDelta);
}


/////////////////////////////////////////////////////////////////
//
//  Commands
//
/////////////////////////////////////////////////////////////////


void CHMLoadGraphSheetController::OnEditCreateString() 
{
	vector<CCastStringId> usedIds[4];

	CHMLoadGraphDoc* pDoc = GetViewport()->GetDocument();

	pDoc->GetCurrentCasterScen(1)->GetLineupIds(usedIds[1]);
	pDoc->GetCurrentCasterScen(2)->GetLineupIds(usedIds[2]);
	pDoc->GetCurrentCasterScen(3)->GetLineupIds(usedIds[3]);

	CCastStringId id(0,0,1,0);
	CCastStringMiscProps props(1,1);

	CCastStringIdDlg dlg;

	dlg.m_pId				= &id;
	dlg.m_usedIds[1]		= usedIds[1];  // copy
	dlg.m_usedIds[2]		= usedIds[2];  // copy
	dlg.m_usedIds[3]		= usedIds[3];  // copy
	dlg.m_pProps			= &props;
	dlg.m_isEditing		= false;
	dlg.m_setInitialId	= true;

	if ( dlg.DoModal() == IDOK ) 
		pDoc->CreateString(id,props);
}


void CHMLoadGraphSheetController::OnViewSetDate() 
{
	CDateTimeDlg dlg;
	dlg.m_bDateOnly = true;
	dlg.m_dateTime = GetViewport()->StartDay();

	if ( dlg.DoModal() == IDOK ) 
		GetViewport()->SetStartDay(dlg.m_dateTime);
}



void CHMLoadGraphSheetController::OnViewDayPlus() 
{
	GetViewport()->SetStartDay( GetViewport()->StartDay() + CTimeSpan(1,0,0,0) );
}


void CHMLoadGraphSheetController::OnViewDayMinus() 
{
	GetViewport()->SetStartDay( GetViewport()->StartDay() - CTimeSpan(1,0,0,0) );
}

void CHMLoadGraphSheetController::OnViewWeekMinus() 
{
	GetViewport()->SetStartDay( GetViewport()->StartDay() - CTimeSpan(GetViewport()->NumCols(),0,0,0));
}

void CHMLoadGraphSheetController::OnViewWeekPlus() 
{
	GetViewport()->SetStartDay( GetViewport()->StartDay() + CTimeSpan(GetViewport()->NumCols(),0,0,0));
}

void CHMLoadGraphSheetController::OnViewAddCol() 
{
	GetViewport()->DeltaRowCol(0,1);
}


void CHMLoadGraphSheetController::OnViewAddRow() 
{
	GetViewport()->DeltaRowCol(1,0);
}

void CHMLoadGraphSheetController::OnViewDelCol() 
{
	GetViewport()->DeltaRowCol(0,-1);
}

void CHMLoadGraphSheetController::OnViewDelRow() 
{
	GetViewport()->DeltaRowCol(-1,0);
}


void CHMLoadGraphSheetController::OnUpdateViewAddCol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewport()->NumCols() < 50);	
}

void CHMLoadGraphSheetController::OnUpdateViewAddRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewport()->NumRows() <3);	
}

void CHMLoadGraphSheetController::OnUpdateViewDelCol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewport()->NumCols() > 2);	
}

void CHMLoadGraphSheetController::OnUpdateViewDelRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewport()->NumRows() > 1);	
}


void CHMLoadGraphSheetController::OnEditDeleteString() 
{
	if ( ! GetViewport()->IsStringSelected() )
		return;

	GetDocument()->DeleteString(GetViewport()->SelectedString());
}



void CHMLoadGraphSheetController::OnEditMoveToHoldArea() 
{
	if ( ! GetViewport()->IsStringSelected() )
		return;
	
	GetViewport()->MoveStringToHoldArea(GetViewport()->SelectedString());
}


// the updating needs work here.

void CHMLoadGraphSheetController::OnEditStringInfo() 
{
	if ( ! GetViewport()->IsStringSelected() )
		return;

	CCastString* pString = GetViewport()->SelectedString();
	int casterNum = pString->Id().Caster();

	vector<CCastStringId> usedIds;

	GetDocument()->GetCurrentCasterScen(casterNum)->GetLineupIds(usedIds,&pString->Id());

	CCastStringIdDlg dlg;

	CCastStringId id = pString->GetId();
	CCastStringMiscProps props = pString->GetProps();

	dlg.m_pId						= &id;
	dlg.m_usedIds[casterNum]		= usedIds;  // copy
	dlg.m_pProps					= &props;	// non-const accessor
	dlg.m_isEditing				= true;
	dlg.m_setInitialId			= false;
	
	if ( dlg.DoModal() == IDOK ) {

		bool change = false;

		if ( id != pString->Id() ) {
			//	This indirects through the casterScen
			//		because the sort order might change.
			//  This is why we need to reselect the string: CScenMru is index-based.
			GetDocument()->GetCurrentCasterScen(casterNum)->ModifyCastStringId(pString,id);
			GetDocument()->GetScenMru().SetCurrentCastString(pString);
			change = true;
		}
		
		if ( props != pString->GetProps() ) {
			pString->ModifyMiscProps(props);
			GetDocument()->Update();
			change = true;
		}

		if ( change ) {
			GetDocument()->GetCurrentCasterScen(casterNum)->SetModified(true);
			GetDocument()->Update();
		}
	}
}


void CHMLoadGraphSheetController::OnViewShow910s() 
{
	GetDocument()->ToggleShow910s();	
}

void CHMLoadGraphSheetController::OnUpdateViewShow910s(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( GetDocument()->Show910s() );
}

void CHMLoadGraphSheetController::OnViewScrollholddown() 
{
	GetViewport()->HoldArea().ScrollDown();	
}

void CHMLoadGraphSheetController::OnUpdateViewScrollholddown(CCmdUI* pCmdUI) 
{
	GetViewport()->HoldArea().OnUpdateScrollDown(pCmdUI);	
}

void CHMLoadGraphSheetController::OnViewScrollholdup() 
{
	GetViewport()->HoldArea().ScrollUp();	
}

void CHMLoadGraphSheetController::OnUpdateViewScrollholdup(CCmdUI* pCmdUI) 
{
	GetViewport()->HoldArea().OnUpdateScrollUp(pCmdUI);	
	
}

void CHMLoadGraphSheetController::OnEditLoadhm() 
{
	if ( TheSnapshot.LoadHMSupplyInfo() ) 
		GetDocument()->Update();
	else
		MessageBox("Error loading HM Supply information.  Contact system maintainer.");
}


void CHMLoadGraphSheetController::OnEditSendhm() 
{
	
	CHMLoadGraphDoc* pDoc = GetDocument();

	ostrstream ostr;
	
	GetEmailBody(1,pDoc->GetStrings(1),ostr);
	GetEmailBody(2,pDoc->GetStrings(2),ostr);
	GetEmailBody(3,pDoc->GetStrings(3),ostr);
	ostr << ends;

	CEMailDlg dlg;

	dlg.SetBody( CString(ostr.str()) );
	ostr.freeze(false);

	dlg.SetSubject("New cast string start times available");
	dlg.AddRecipient("tvfron@inland.com");
	
	dlg.DoModal();
}

void CHMLoadGraphSheetController::GetEmailBody(int caster, 
										 vector<CCastString*>& strings1,
										 ostrstream& ostr)
{
	
	vector<CCastString*> strings = strings1;	//copy;

	strings.erase(remove_if(strings.begin(),
							strings.end(),
							CCastString::CFilterInvalidStartTimes()),
				  strings.end());

	stable_sort(strings.begin(),strings.end(),CCastString::CStartTimeCmp());

	ostr << "\r\nCaster # " << caster;

	if ( strings.size() == 0 ) {
		ostr << " -- no strings listed.\r\n\r\n";
		return;
	}
	else
		ostr << "\r\n"
			 << "  Wk ### #Hts CastStart__ CastEnd____ Spec___ Widths_____ \r\n";

	bool show910s = GetDocument()->Show910s();


	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is ) {

		if ( ! (*is)->IsStartTimeValid() )
			continue;

		if ( (*is)->Status() == CCastString::STRING_STATUS_910 
			 &&
			 ! show910s )
			 continue;

		CTime stringStart = (*is)->CastStartTime();
		CTime stringFinish = stringStart + CTimeSpan(0,0,0,(*is)->TotalDuration());
		
		long start1;
		long end1;
		long start2;
		long end2;
		CHMLoadGraphDoc::FindWidthsToUse((*is),start1,end1,start2,end2);

		
		ostr << "  " 
			 << setw(2) << (*is)->Id().Week()
			 << " "
			 << setw(3) << (*is)->Id().StringId()
			 << " "
			 << setw(4) << max((*is)->Heats().size(),(*is)->Props().NumHeats())
			 << " "
			 << setw(11) << LPCTSTR(stringStart.Format("%m/%d %H:%M"))
			 << " "
			 << setw(11) << LPCTSTR(stringFinish.Format("%m/%d %H:%M"))
			 << " " 
			 << setw(7) << LPCTSTR(CHMLoadGraphDoc::FindSpecToUse( (*is) ))
			 << " " << setw(2) << start1
			 << "/" << setw(2) << end1;

		if ( (*is)->Id().Caster() != 2 ) {
			ostr << " " << setw(2) << start2 
				 << "/" << setw(2) << end2;
		}

		ostr << "\r\n";
	}
}




///////////////////////////////////////////////////////////////
//
//  Dragging on the display & other mouse
//
///////////////////////////////////////////////////////////////



void CHMLoadGraphSheetController::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CHMLoadGraphSheetViewport::CHitData hitData;
	bool result = GetViewport()->SelectStringAtPoint(point,hitData);
	
	if ( ! GetViewport()->IsStringSelected() ) {
		// didn't click on a string, so get out
		m_bTracking = false;
		return;
	}

	// let's start tracking, but not drawing until we move sufficiently far.
	m_bTracking = true;

	m_bTrackingDraw = false;
	m_ptFirst = point;
	
	//  We play a little game here.
	//  If we are coming from one of the CDayAreas, 
	//    the rectangle's width reflects its duration.
	//  If we are coming from the CHoldArea,
	//    the rectangle's width is arbitray.
	//  In the latter case, we would like to change the width
	//    to reflect the former case.
	//  We compute a width using the first Day.

	if ( hitData.m_hitArea == CHMLoadGraphSheetViewport::HT_HOLD_AREA && GetViewport()->Days().size() >= 1 ) {
		const CHMLoadGraphSheetViewport::CDayArea& day = GetViewport()->Days().front();
		CTime start = CTime::GetCurrentTime();
		CTime finish = start +  CTimeSpan(0,0,0,GetViewport()->SelectedString()->TotalDuration());
		double fraction = CHMLoadGraphSheetViewport::CDayArea::DayFraction(finish,start);
		int width = max(10,day.m_rectEntire.Width()*fraction);
		m_rectTracking = CRect(hitData.m_rect.left,
							   hitData.m_rect.top,
							   hitData.m_rect.left+width,
							   hitData.m_rect.bottom);
		m_strTime = "";
	}
	else {
		m_rectTracking = hitData.m_rect;
		m_strTime = GetViewport()->SelectedString()->CastStartTime().Format(" %H:%M -- %m/%d");
	}
	
	m_pbmString = 0;

	m_mouseXOffset = point.x - m_rectTracking.TopLeft().x;	
}


void CHMLoadGraphSheetController::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( ! m_bTracking )
		return;

#if 0
	afxDump << "OnMouseMove: string selected? "
			<< (m_pSelectedString == 0 ? "No" : "Yes")
			<< "\n";
#endif

	if ( ! GetViewport()->IsStringSelected() ) 
		return;


	// we are tracking.
	// determine if we have started drawing.
	
	if ( ! m_bTrackingDraw ) {

		//  see if we should start

		CSize diff = point - m_ptFirst;

#if 0
		afxDump << "Started at " << m_ptFirst << ", now at " << point << "\n";
#endif
		if ( abs(diff.cx) > 2 || abs(diff.cy) > 2 ) {
			// we've moved far enough to start drawing
			CClientDC dc(GetViewport()->GetWnd());
			
#ifdef _DEBUG
			if (!m_dcString.CreateCompatibleDC(&dc))
				afxDump << "Unable to CreateCompatibleDC!\n";
#endif
			m_pbmString = new CBitmap;
#ifdef _DEBUG
			if (!m_pbmString->CreateCompatibleBitmap(&m_dcString, 200, 200))
				afxDump << "Unable to CreateCompatibleBitmap:1\n";
#endif
			m_dcString.SetBkMode(OPAQUE);
			m_dcString.SetBkColor(RGB(0,0,0));
			m_dcString.SetTextColor(RGB(0xff,0xff,0xff));

			m_bTrackingDraw = true;
			m_dlgStartTime.m_pString = GetViewport()->SelectedString();
			m_dlgStartTime.Create(CPoint(0,0),this);
			m_dlgStartTime.SetTimeInvalid();
			GetViewport()->DrawTrackingOutline(m_dcString,m_pbmString,m_rectTracking,m_strTime);
			GetViewport()->GetWnd()->SetCapture();

		}
		else
			return;
	}

	if ( GetViewport()->GetWnd()->GetCapture() != GetViewport()->GetWnd() )
		return;


	// we are drawing

	CRect oldRect( m_rectTracking );
	CString oldString( m_strTime );

	// Because we have multiple rows, determining the top and bottom is a little difficult.
	// We use the mouse coordinate and try to find the bounds of a rectangle for casters
	// that is closest in y coordinate to the mouse point

	int caster = GetViewport()->SelectedString()->Id().Caster();
	int top;
	int bottom;
	GetViewport()->FindConfiningBar(caster,point,top,bottom);
	
	int deltaX = point.x - m_rectTracking.TopLeft().x - m_mouseXOffset;
	m_rectTracking.SetRect(point.x - m_mouseXOffset,
						   top,
						   point.x - m_mouseXOffset + oldRect.Width(),
						   bottom);
	CTime newTime;
	
	if ( GetViewport()->FindTimeAtPoint(m_rectTracking.TopLeft(),caster,newTime) ) {
		//we are in the day area;
		m_dlgStartTime.SetTime(newTime,
							   newTime+GetViewport()->SelectedString()->TotalDuration());
		m_strTime = newTime.Format("%H:%M -- %m/%d");
	}
	else {
		m_dlgStartTime.SetTimeInvalid();
		m_strTime = "";
	}

	GetViewport()->MoveTrackingOutline(m_dcString,m_pbmString,m_rectTracking,m_strTime,oldRect,oldString);
}


void CHMLoadGraphSheetController::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( ! m_bTracking )
		return;

	if ( ! m_bTrackingDraw ) {
		// we haven't started drawing yet, so let's just quit
		m_bTracking = false;
		return;
	}

	CWnd* pWnd = GetViewport()->GetWnd();

	if ( pWnd->GetCapture() != pWnd )
		return;

	 ReleaseCapture();

	// we are really tracking and drawing.

	m_dlgStartTime.DestroyWindow();

	GetViewport()->DrawTrackingOutline(m_dcString,m_pbmString,m_rectTracking,m_strTime);

	CCastString* pString = GetViewport()->SelectedString();

	CTime time;
	if ( GetViewport()->FindTimeAtPoint(m_rectTracking.TopLeft(),pString->Id().Caster(),time) )
		GetViewport()->SetStringStartTime(pString,time);
	else
		GetViewport()->MoveStringToHoldArea(pString);

	m_bTracking = false;
	m_bTrackingDraw = false;
	m_rectTracking.SetRectEmpty();
	m_mouseXOffset = 0;
	m_dcString.DeleteDC();
	delete m_pbmString;
	m_pbmString = 0;
}


void CHMLoadGraphSheetController::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint clientPt(point);
	GetViewport()->GetWnd()->ScreenToClient(&clientPt);

	CHMLoadGraphSheetViewport::CHitData hitData;
	GetViewport()->SelectStringAtPoint(clientPt,hitData);

	CMenu menu;
	menu.LoadMenu(IDR_HM_LOAD_GRAPH_CONTEXT_MENU);
	CMenu* pContextMenu = menu.GetSubMenu(0);
	UINT flags = MF_BYCOMMAND;
	flags |= (GetViewport()->IsStringSelected() ? MF_ENABLED : MF_GRAYED);
	menu.EnableMenuItem(ID_EDIT_STRING_INFO,flags);
	menu.EnableMenuItem(ID_EDIT_DELETE_STRING,flags);
	menu.EnableMenuItem(ID_EDIT_MOVE_TO_HOLD_AREA,flags);

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
								 point.x, 
								 point.y, 
								 this);
}	


void CHMLoadGraphSheetController::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// A double click on the graph will pop up some numbers.

	CHMLoadGraphSheetViewport::CHitData hitData;
	bool result = GetViewport()->HitTest(point,hitData);

	switch ( hitData.m_hitArea ) {
	case CHMLoadGraphSheetViewport::HT_LOAD:
		if ( hitData.m_pDayArea != 0 ) {
			int hour = (hitData.m_time - hitData.m_pDayArea->m_day).GetTotalHours();
			if ( 0 <= hour && hour < 24 ) {
				int val1 = hitData.m_pDayArea->m_loadValues[1][hour];
				int val2 = hitData.m_pDayArea->m_loadValues[2][hour];
				int val3 = hitData.m_pDayArea->m_loadValues[3][hour];
				int val4 = hitData.m_pDayArea->m_supplyValues[hour];
				CString strDate = hitData.m_time.Format("%a, %b/%d/%Y %I:00 %p");
				CString str;
				str.Format("Time: %s\n"
						   "Caster #1: %3d tons\n"
						   "Caster #2: %3d tons\n"
						   "Caster #3: %3d tons\n"
						   "Total    : %3d tons\n"
						   "Supply   : %3d tons",
						   LPCTSTR(strDate),
						   val1,
						   val2,
						   val3,
						   val1+val2+val3,
						   val4);
				MessageBox(LPCTSTR(str));
			}
		}
		break;
	}
}




////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport::CDayArea
//
//	This class caches areas on the graph
//  matching a specific day.
//
/////////////////////////////////////////////////////////////////


void CHMLoadGraphSheetViewport::CDayArea::ComputeRects(CHMLoadGraphSheetViewport* pViewport,
												   const CTime& day,
												   const CRect& rectEntire)
{
	m_pViewport			= pViewport;
	m_day			= day;
	m_rectEntire	= rectEntire;

	m_strDate = m_day.Format("%a, %b %d");


	CPoint origin(rectEntire.TopLeft());

	// Probably we should do some of this proportionately...
	const int dateHeight = 10;
	const int hmBalHeight = 20;
	const int turnHeight = 10;
	const int casterHeight = 60;
//	const int barHeight = 10;
	const int graphHeight = 60;

	m_rectDate.SetRect(0,0,m_rectEntire.Width(),dateHeight);
	m_rectDate.OffsetRect(origin);
	
	m_rectHM.SetRect(0,0,m_rectEntire.Width(),hmBalHeight);
	m_rectHM.OffsetRect(CPoint(0,dateHeight));
	m_rectHM.OffsetRect(origin);

	m_rectHours.SetRect(0,0,m_rectEntire.Width(),turnHeight);
	m_rectHours.OffsetRect(CPoint(0,dateHeight+hmBalHeight));
	m_rectHours.OffsetRect(origin);

	{
		for ( int caster=1; caster<=3; ++caster ) {
			CRect rect1(CPoint(0,0),CSize(10,10));
//			int yOffset = dateHeight+hmBalHeight+turnHeight+(caster-1)*(casterHeight+barHeight);
			int yOffset = dateHeight+hmBalHeight+turnHeight+(caster-1)*casterHeight;
			CPoint corner(0,yOffset);
			CSize size(m_rectEntire.Width(),casterHeight);
			CRect rect(corner,size);
			rect.OffsetRect(origin);
			m_rectCasterText[caster] = rect;

			
//			m_rectCasterBar[caster] = CRect(CPoint(m_rectCasterText[caster].left,
//												   m_rectCasterText[caster].bottom),
//											CSize(m_rectEntire.Width(),
//												  barHeight));
		}
	}

	m_rectLoadGraph.SetRect(0,0,m_rectEntire.Width(),graphHeight);
	m_rectLoadGraph.OffsetRect(CPoint(0,dateHeight+hmBalHeight+turnHeight+3*casterHeight));
	m_rectLoadGraph.OffsetRect(origin);

#if 0
	{
		afxDump << "CDayArea::ComputeRects: day = " << m_strDate 
			 << " serial = " << (m_day - m_pViewport->m_startDay).GetDays()
			 << "\n"
			 << "Date: " << m_rectDate 
			 << ", HM = " << m_rectHM
			 << ", Hours = " << m_rectHours 
			 << ", Load = " << m_rectLoadGraph
			 << "\n";
		
		for ( int caster=1; caster<=3; ++caster ) 
			afxDump << "Caster #" << caster
				 << ", text = " << m_rectCasterText[caster]
//				 << ", bar = "  << m_rectCasterBar[caster]
				 << "\n";
	}
#endif
	
}

void CHMLoadGraphSheetViewport::CDayArea::ComputeStringRects()
{
	m_castStringTextData.clear();
//	m_castStringBarData.clear();

	CHMLoadGraphDoc* pDoc = m_pViewport->GetDocument();



	ComputeStringRects(1,pDoc->GetStrings(1));
	ComputeStringRects(2,pDoc->GetStrings(2));
	ComputeStringRects(3,pDoc->GetStrings(3));

#if 0
	{
		
		afxDump << "CDayArea::ComputeStringRects: day = " << m_strDate 
			 << " serial = " << (m_day - m_pViewport->m_startDay).GetDays()
			 << "\n";
		
		for ( vector< CCastStringData >::iterator id = m_castStringTextData.begin();
			  id != m_castStringTextData.end();
			  ++id ) {

			afxDump << "Caster # " << (*id).m_pString->Id().Caster()
				 << " " << (*id).m_rect;

			for ( vector<CString>::iterator is = (*id).m_vecText.begin();
				  is != (*id).m_vecText.end();
				  ++is )
				afxDump << LPCTSTR((*is)) << "|";

			afxDump << "\n";
		}

//		for (  id = m_castStringBarData.begin();
//			  id != m_castStringBarData.end();
//			  ++id ) {
//
//			afxDump << "Caster # " 
//				 << (*id).m_pString->Id().Caster()
//				 << "-" << (*id).m_pString->Id().Week()
//				 << "-" << (*id).m_pString->Id().StringId()
//				 << " " << (*id).m_rect;
//
//			afxDump << "\n";
//		}
	}
#endif

}

void CHMLoadGraphSheetViewport::CDayArea::ComputeStringRects(int caster, 
														 vector<CCastString*>& strings1)
{

	vector<CCastString*> strings = strings1;	//copy;

	strings.erase(remove_if(strings.begin(),strings.end(),CCastString::CFilterInvalidStartTimes()),
				  strings.end());

	stable_sort(strings.begin(),strings.end(),CCastString::CStartTimeCmp());


	int index = 0;
	bool show910s = m_pViewport->GetDocument()->Show910s();

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is, ++index ) {

		if ( ! (*is)->IsStartTimeValid() )
			continue;

		if ( (*is)->Status() == CCastString::STRING_STATUS_910 
			 &&
			 ! show910s )
			 continue;

		CTime finishTime = m_day + CTimeSpan(1,0,0,0);

		CTime stringStart = (*is)->CastStartTime();
		CTime stringFinish = stringStart + CTimeSpan(0,0,0,(*is)->TotalDuration());

#if 0
		{
			CString strStart;
			CString strFinish;
			CString strStrStart;
			CString strStrFinish;

			strStart = m_day.Format("%x %X, ");
			strFinish = finishTime.Format("%x %X, ");
			strStrStart = stringStart.Format("%x %X, ");
			strStrFinish = stringFinish.Format("%x %X");

			afxDump << "ComputeStringRect for "
					<< (*is)->Id().Caster() << "-"
					<< (*is)->Id().Week() << "-"
					<< (*is)->Id().StringId() 
					<< ", " 
					<< strStart << strFinish << strStrStart << strStrFinish;
			if ( stringFinish < m_day || stringStart >= finishTime )
				afxDump << " -- ignore ";
			else if ( stringStart < finishTime )
				afxDump << " -- bar & text";
			else
				afxDump << " -- bar only";
			afxDump << "\n";
		}
#endif

		if ( stringFinish < m_day || stringStart >= finishTime )
			continue;

//		COLORREF color = (index%2==0 
//						  ? RGB(255,224,183) 
//						  : RGB(183,255,255));
		COLORREF color = sm_colorLoad[caster][index%2];

		if ( stringStart < finishTime ) {

			double startFraction = DayFraction(stringStart,m_day);
			double finishFraction = DayFraction(stringFinish,m_day);
			int startOffset = int(m_rectEntire.Width()*startFraction);
			int finishOffset = int(m_rectEntire.Width()*finishFraction);
			CRect& casterRect = m_rectCasterText[ (*is)->Id().Caster() ];
			CRect rect(CPoint(casterRect.left+startOffset,casterRect.top),
					   CSize(finishOffset-startOffset,casterRect.Height()));
			vector<CString> texts;
			ComputeCastStringText((*is),texts);
			CCastStringData data((*is),
								 rect,
								 color,
								 texts);
			m_castStringTextData.push_back( data );
		}

//		CTime barStart( max(m_day,stringStart) );
//		CTime barFinish( min(finishTime,stringFinish) );
//		double startFraction = DayFraction(barStart,m_day);
//		double finishFraction = DayFraction(barFinish,m_day);
//		int startOffset = int(m_rectEntire.Width()*startFraction);
//		int finishOffset = int(m_rectEntire.Width()*finishFraction);
//		CRect& casterRect = m_rectCasterBar[ (*is)->Id().Caster() ];
//		CRect rect(CPoint(casterRect.left+startOffset,casterRect.top),
//				   CSize(finishOffset-startOffset,casterRect.Height()));
//		m_castStringBarData.push_back( CCastStringData((*is),
//														rect,
//														color) );
	}
}




#define ADDTEXT(vec,ostr) \
		vec.push_back(ostr.str()); \
		ostr.freeze(false); \
		ostr.seekp(0)
		

void CHMLoadGraphSheetViewport::CDayArea::ComputeCastStringText(CCastString* pString,
															vector<CString>& texts)
{
	texts.clear();

	ostrstream ostr;
	ostr << "(" << pString->Id().Week() 
		 << "-" << pString->Id().StringId()
		 << ")" 
		 << ends;
	
	ADDTEXT(texts,ostr);

	texts.push_back( CHMLoadGraphDoc::FindSpecToUse(pString) );

	long start1;
	long end1;
	long start2;
	long end2;
	CHMLoadGraphDoc::FindWidthsToUse(pString,start1,end1,start2,end2);

	ostr << start1 
		 << "/"
		 << end1
		 << ends;
	ADDTEXT(texts,ostr);

	if ( pString->Id().Caster() != 2 ) {
		ostr << start2 
			<< "/"
			<< end2
			<< ends;
		ADDTEXT(texts,ostr);
	}

	ostr << max(pString->Heats().size(),pString->Props().NumHeats())
		 << " heats"
		 << ends;
	ADDTEXT(texts,ostr);
}







#define at_end(i) (iters[i] == (records[i])->end())
#define maybe_advance(i,t) if ( ! at_end(i) && (*(iters[i])).m_time == (t) ) ++(iters[i])
#define safe_value(i,t) ( ( at_end(i) || (*(iters[i])).m_time != (t) ) ? 0.0 : (*(iters[i])).m_tons )

#define supply_at_end (isupply == (supplyRecords)->end())
#define supply_maybe_advance(t) if ( ! supply_at_end && (*isupply).GetDate() == (t) ) ++isupply
#define supply_safe_value(t) ( ( supply_at_end || (*isupply).GetDate() != (t) ) ? 0.0 : (*isupply).GetTonsPerHour() )

void CHMLoadGraphSheetViewport::CDayArea::ComputeLoadGraphValues()
{
	{
		for ( int i=0; i<24; ++i ) {
			m_loadValues[1][i] = 0;
			m_loadValues[2][i] = 0;
			m_loadValues[3][i] = 0;
			m_supplyValues[i]  = 0;
		}
	}	
	
	CHMLoadGraphDoc* pDoc = m_pViewport->GetDocument();

	vector<CHMLoadRecord>* records[4];
	records[1] = &(pDoc->GetLoadRecords(1));
	records[2] = &(pDoc->GetLoadRecords(2));
	records[3] = &(pDoc->GetLoadRecords(3));

	vector<CHMLoadRecord>::iterator iters[4];
	iters[1] = records[1]->begin();
	iters[2] = records[2]->begin();
	iters[3] = records[3]->begin();

	vector<CHMSupplyRecord>* supplyRecords = &pDoc->GetSupplyRecords();
	vector<CHMSupplyRecord>::iterator isupply = supplyRecords->begin();

	CTime startTime  = m_day;
	CTime finishTime = startTime + CTimeSpan(1,0,0,0);



	while ( !( at_end(1) && at_end(2) && at_end(3) ) ) {

		CTime someTime;
		{
			for ( int i=1; i<=3; ++i ) {
				if ( ! at_end(i) ) {
					someTime  = (*(iters[i])).m_time;
					break;
				}
			}
		}
		{
			for ( int i=1; i<=3; ++i ) {
				if ( ! at_end(i) )
					someTime = min(someTime,(*(iters[i])).m_time);
			}
		}

		if ( ! supply_at_end )
			someTime = min(someTime, (*isupply).GetDate());

		time_t leftTime = someTime.GetTime();
		time_t rightTime = leftTime + 3600;

		if ( rightTime <= startTime.GetTime() )
			goto advance;

		if ( leftTime >= finishTime.GetTime() )
			break;

		{
			int index = (someTime - startTime).GetTotalHours();
			assert( 0 <= index && index < 24);

			m_loadValues[1][index] = safe_value(1,someTime);
			m_loadValues[2][index] = safe_value(2,someTime);
			m_loadValues[3][index] = safe_value(3,someTime);
			m_supplyValues[index]  = supply_safe_value(someTime);
		}
	
advance:
		
		maybe_advance(1,someTime);
		maybe_advance(2,someTime);
		maybe_advance(3,someTime);
		supply_maybe_advance(someTime);
	}

#if 0

	{

		CString strStart;
		CString strFinish;
		CString strStrStart;
		CString strStrFinish;
		strStart = startTime.Format("%x %X, ");
		strFinish = finishTime.Format("%x %X, ");
		afxDump << "Loads for day: " << LPCTSTR(strStart) 
				<< " to " << LPCTSTR(strFinish) 
				<< "\n";

		for ( int caster=1; caster<=3; ++caster ) {
			afxDump << "Caster #" << caster << ": ";
			for ( int i=0; i<24; ++i ) {
				afxDump << m_loadValues[caster][i];
			}
			afxDump << "\n";
		}

		afxDump << "Supply  ";
		for ( int i=0; i<24; ++i ) 
			afxDump << m_supplyValues[i];
		afxDump << "\n";
	}
#endif

}


///////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport::CDCState
//
///////////////////////////////////////////////////////////////////


CHMLoadGraphSheetViewport::CDCState::CDCState(CDC* pDC)
{
	m_pDC = pDC;

	// Set up the font
	CFont font;
	font.CreatePointFont(70,"Arial");
	m_pOldFont = pDC->SelectObject(&font);
	font.Detach();

	// Draw borders inside
	//CPen pen;
	//pen.CreatePen(PS_INSIDEFRAME,0,RGB(0,0,0));
	//CPen* m_pOldPen = pDC->SelectObject(&pen);
	// pen.Detach();

	m_oldMode = pDC->SetBkMode(TRANSPARENT);
}


CHMLoadGraphSheetViewport::CDCState::~CDCState()
{
	m_pDC->SelectObject(m_pOldFont);
	//m_pDC->SelectObject(m_pOldPen);
	m_pDC->SetBkMode(m_oldMode);
}


void CHMLoadGraphSheetViewport::CDayArea::Draw( CDC* pDC )
{
	if ( ! pDC->RectVisible(&m_rectEntire) )
		return;

	CDCState state(pDC);
	

	DrawBackground(pDC);
	DrawDay(pDC);
	DrawHM(pDC);
	DrawHours(pDC);
	DrawCastStringBackgrounds(pDC);

	{
		for ( vector< CCastStringData >::iterator id = m_castStringTextData.begin();
			  id != m_castStringTextData.end();
			  ++id ) 
			DrawCastStringText( pDC, (*id) );
	}

//	{	
//		for ( vector< CCastStringData >::iterator id = m_castStringBarData.begin();
//			  id != m_castStringBarData.end();
//			  ++id ) 
//			DrawCastStringBar( pDC, (*id) );
//	}

	DrawLoads(pDC);


}


void CHMLoadGraphSheetViewport::CDayArea::DrawBackground( CDC* pDC )
{
	// Draw the background for the whole area.
	
	CBrush br(sm_colorBack);
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->Rectangle(&m_rectDate);
	pDC->SelectObject(pOldBrush);
}


void CHMLoadGraphSheetViewport::CDayArea::DrawDay( CDC* pDC )
{

	CBrush br(sm_colorDateBack);
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->Rectangle(&m_rectDate);
	pDC->SelectObject(pOldBrush);

	COLORREF oldColor = pDC->SetTextColor(sm_colorDateFore);
	DrawCenteredText(pDC,m_strDate,m_rectDate);
	pDC->SetTextColor(oldColor);
}

void CHMLoadGraphSheetViewport::CDayArea::DrawHM( CDC* pDC )
{
	CBrush br(sm_colorDateBack);
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->Rectangle(&m_rectHM);
	pDC->SelectObject(pOldBrush);
}

void CHMLoadGraphSheetViewport::CDayArea::DrawHours( CDC* pDC )
{
	static CString strTurnText[] = { "1", "2", "3" };

	CBrush br(sm_colorDateBack);
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->Rectangle(&m_rectHours);
	pDC->SelectObject(pOldBrush);

	COLORREF oldColor = pDC->SetTextColor(sm_colorDateFore);

	// for each turn
	for ( int i=0; i<3; ++i ) {
		CPoint pt(m_rectHours.left+i*m_rectHours.Width()/3,
				  m_rectHours.top);
		CSize  sz(m_rectHours.Width()/3,
				  m_rectHours.Height());
		CRect rect(pt,sz);

// The compiler barfed on this.
//		CRect rect(CPoint(m_rectHours.left+i*m_rectHours.Width()/3,
//						  m_rectHours.top),
//				   CSize(m_rectHours.Width()/3,
//						 m_rectHours.Height()));


		DrawCenteredText(pDC,strTurnText[i],rect);
	}
		
	pDC->SetTextColor(oldColor);
}


void CHMLoadGraphSheetViewport::CDayArea::DrawCastStringBackgrounds( CDC* pDC )
{
	for ( int caster=1; caster<=3; ++caster ) {
		CBrush br(sm_colorCasterBack[caster]);
		CBrush* pOldBrush = pDC->SelectObject(&br);
		pDC->Rectangle(&m_rectCasterText[caster]);
//		pDC->Rectangle(&m_rectCasterBar[caster]);
		pDC->SelectObject(pOldBrush);
	}
}

void CHMLoadGraphSheetViewport::CDayArea::RedrawString(CCastString* pString)
{
	CClientDC dc(m_pViewport->GetWnd());

	CDCState state(&dc);

	{
		for ( vector< CCastStringData >::iterator id = m_castStringTextData.begin();
			  id != m_castStringTextData.end();
			  ++id ) 

			if ( (*id).m_pString == pString )
				DrawCastStringText(&dc,(*id));
	}
}


void CHMLoadGraphSheetViewport::CDayArea::DrawCastStringText(CDC* pDC, 
														 const CHMLoadGraphSheetViewport::CCastStringData& data )
{	
	data.Draw(pDC,data.m_pString == m_pViewport->SelectedString() );
}

void CHMLoadGraphSheetViewport::CCastStringData::Draw(CDC* pDC, bool isSelected) const
{
	bool is910 = m_pString->Status() == CCastString::STRING_STATUS_910;
	CBrush br(m_backColor);

	COLORREF borderColor = ( isSelected
							 ? CHMLoadGraphSheetViewport::sm_colorSelectedHilite
							 : ( is910
							     ? CHMLoadGraphSheetViewport::sm_color910Border
								 : CHMLoadGraphSheetViewport::sm_color909Border) );
	CPen pen(PS_SOLID,2,borderColor);

	CBrush* pOldBrush = pDC->SelectObject(&br);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(&m_rect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	CSize textSize = GetTextSize(pDC,m_vecText);

	CRect rect(m_rect.TopLeft()+CPoint(2,2),
			   textSize);

	COLORREF oldColor = pDC->SetTextColor((is910 ? RGB(255,255,255) : CHMLoadGraphSheetViewport::sm_color909Border));
	CHMLoadGraphSheetViewport::DrawCenteredText(pDC,m_vecText,rect);
	pDC->SetTextColor(oldColor);
}


void CHMLoadGraphSheetViewport::CDayArea::DrawCastStringBar(CDC* pDC, 
														 const CHMLoadGraphSheetViewport::CCastStringData& data )
{
//	CBrush br(data.m_backColor);
//
//	CRect rect = data.m_rect;
//	rect.DeflateRect(0,3,0,0);
//	pDC->FillRect(&rect,&br);
}



void CHMLoadGraphSheetViewport::CDayArea::DrawLoads(CDC* pDC)
{
	CHMLoadGraphDoc* pDoc = m_pViewport->GetDocument();
	DrawLoadBG(pDC);

	for ( int hour=0; hour<24; ++hour ) {

		Weight val1 = m_loadValues[3][hour];
		Weight val2 = val1 + m_loadValues[2][hour];
		Weight val3 = val2 + m_loadValues[1][hour];

		if ( val1 != 0 )
			DrawLoadRect(pDC,sm_colorLoad[3][2],hour,0,   val1);
		if ( val1 != val2 )
			DrawLoadRect(pDC,sm_colorLoad[2][2],hour,val1,val2);
		if ( val2 != val3 )
			DrawLoadRect(pDC,sm_colorLoad[1][2],hour,val2,val3);

		Weight supply = m_supplyValues[hour];

		DrawLoadLine(pDC,sm_colorSupply,hour,hour+1,supply,supply,3);
	}

	DrawLoadLine(pDC,
				  RGB(0x00,0x00,0x00),
				  0,24,
				  0,0);
	DrawLoadLine(pDC,
				  RGB(0x00,0x00,0x00),
				  0,24,
				  300,300);
	DrawLoadLine(pDC,
				  RGB(0x00,0x00,0x00),
				  0,24,
				  600,600);
	DrawLoadLine(pDC,
				  RGB(0x00,0x00,0x00),
				  0,24,
				  900,900);
	DrawLoadLine(pDC,
				  RGB(0x00,0x00,0x00),
				  0,24,
				  1200,1200);

	DrawLoadLineLabel(pDC,
					  RGB(0xFF,0xFF,0xFF),
					  "300",300);
	DrawLoadLineLabel(pDC,
					  RGB(0xFF,0xFF,0xFF),
					  "600",600);
	DrawLoadLineLabel(pDC,
					  RGB(0xFF,0xFF,0xFF),
					  "900",900);

}

void CHMLoadGraphSheetViewport::CDayArea::DrawLoadBG(CDC* pDC)
{
	pDC->FillSolidRect(m_rectLoadGraph,sm_colorLoadBack);
}


void CHMLoadGraphSheetViewport::CDayArea::DrawLoadRect(CDC* pDC,
												   COLORREF color, 
												   int startHour, 
												   double val1, 
												   double val2)
{
	int width = m_rectLoadGraph.Width();
	int height = m_rectLoadGraph.Height();

	val1 = max(0.0,min(val1,1200.0));
	val2 = max(0.0,min(val2,1200.0));


	int x1 = m_rectLoadGraph.left + width*startHour/24.0;
	int x2 = m_rectLoadGraph.left + width*(startHour+1)/24.0;

	int y2 = m_rectLoadGraph.bottom - height*val1/1200.0;
	int y1 = m_rectLoadGraph.bottom - height*val2/1200.0;

#if 0
	afxDump << "DrawLoadRect " << startHour 
			<< " " << val1 
			<< " " << val2
			<< " --> " << m_rectLoadGraph
			<< " " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
#endif

	//pDC->FillSolidRect(CRect(x1,y1,x2,y2),color);
	CBrush brush(color);
	pDC->FillRect(CRect(x1,y1,x2,y2),&brush);
}


void CHMLoadGraphSheetViewport::CDayArea::DrawLoadLine(CDC* pDC,
												   COLORREF color,
												   int startHour,
												   int finishHour,
												   double val1,
												   double val2,
												   int penWidth)
{
	int width = m_rectLoadGraph.Width();
	int height = m_rectLoadGraph.Height();

	val1 = max(0.0,min(val1,1200.0));
	val2 = max(0.0,min(val2,1200.0));

	int x1 = m_rectLoadGraph.left + width*startHour/24.0;
	int x2 = m_rectLoadGraph.left + width*finishHour/24.0;

	int y2 = m_rectLoadGraph.bottom - height*val1/1200.0;
	int y1 = m_rectLoadGraph.bottom - height*val2/1200.0;

#if 0
	afxDump << "DrawLoadLine " << startHour 
			<< " " << finishHour
			<< " " << val1 
			<< " " << val2
			<< " --> " << m_rectLoadGraph
			<< " " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
#endif

	CPen pen(PS_SOLID,penWidth,color);
	CPen* pOld = pDC->SelectObject(&pen);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
	pDC->SelectObject(pOld);

}

void CHMLoadGraphSheetViewport::CDayArea::DrawLoadLineLabel(CDC* pDC,
														COLORREF color,
														const char* label,
														double val)
{
	int width = m_rectLoadGraph.Width();
	int height = m_rectLoadGraph.Height();

	val = max(0.0,min(val,1200.0));
	int x1 = m_rectLoadGraph.left;
	int x2 = m_rectLoadGraph.left+25;

	int y2 = m_rectLoadGraph.bottom - height*val/1200.0 + 10;
	int y1 = y2 - 20;


#if 0
	afxDump << "DrawLoadLineLabel " << label 
			<< " " << val 
			<< " --> " << m_rectLoadGraph
			<< " " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
#endif

	COLORREF oldColor = pDC->SetTextColor(color);
	DrawCenteredText(pDC,CString(label),CRect(x1,y1,x2,y2));
	pDC->SetTextColor(oldColor);

}



bool CHMLoadGraphSheetViewport::CDayArea::HitTest(const CPoint& pt,
											  CHitData& rHitData) const
{
	rHitData.m_time = XlatePoint(pt);
	
	if ( m_rectLoadGraph.PtInRect(pt) ) {
		rHitData.m_hitArea = HT_LOAD;
		rHitData.m_pDayArea = this;
		return true;
	}
	else {
		vector< CCastStringData >::const_iterator id = GetCastStringTextDataAtPoint(pt);
		if ( id != m_castStringTextData.end() ) {
			rHitData.m_casterNum = (*id).m_pString->Id().Caster();
			rHitData.m_pString = (*id).m_pString;
			rHitData.m_vecText = (*id).m_vecText;
			rHitData.m_rect = (*id).m_rect;
			rHitData.m_hitArea = HT_CASTER;
			rHitData.m_pDayArea = this;
			return true;
		}
	}

	return false;
}

		

vector< CHMLoadGraphSheetViewport::CCastStringData >::const_iterator CHMLoadGraphSheetViewport::CDayArea::GetCastStringTextDataAtPoint(const CPoint& pt) const
{
	{
		for ( vector< CCastStringData >::const_iterator id = m_castStringTextData.begin();
			  id != m_castStringTextData.end();
			  ++id ) {

			if ( (*id).m_rect.PtInRect(pt) ) 
				return id;
		}
	}

	return m_castStringTextData.end();
}


CTime CHMLoadGraphSheetViewport::CDayArea::XlatePoint(const CPoint& pt) const
{
	const double NumSecsPerDay = 24*3600;

	if ( m_rectEntire.Width() == 0 )
		return m_day;
	
	CTimeSpan delta(0,0,0,long(double(pt.x - m_rectEntire.left)/m_rectEntire.Width() * NumSecsPerDay));

	return min(m_day+CTimeSpan(1,0,0,0),max(m_day,m_day + delta));
}


// static
double CHMLoadGraphSheetViewport::CDayArea::DayFraction(const CTime& x, const CTime& y)
{
	CTimeSpan delta = x - y;
	const double NumSecsPerDay = 24*3600;

	return delta.GetTotalSeconds()/NumSecsPerDay;
}







////////////////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport::CHoldArea
//
////////////////////////////////////////////////////////////////////////////

//static
const int CHMLoadGraphSheetViewport::CHoldArea::m_iconHeight = 15;
const int CHMLoadGraphSheetViewport::CHoldArea::m_iconMargin =  3;
const int CHMLoadGraphSheetViewport::CHoldArea::m_iconSpace  
	= CHMLoadGraphSheetViewport::CHoldArea::m_iconHeight 
	  + CHMLoadGraphSheetViewport::CHoldArea::m_iconMargin;


int CHMLoadGraphSheetViewport::CHoldArea::NumVisible()
{
	return m_rectBody.Height() / m_iconSpace;
}


void CHMLoadGraphSheetViewport::CHoldArea::Initialize(CWnd* pParentWnd)
{
	m_btnUp.Create(0,
				   WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
				   SECBitmapButton::Al_Center,
				   IDB_SMALL_UP,
				   CRect(0,0,100,100),
				   pParentWnd,
				   ID_VIEW_SCROLLHOLDUP);

	m_btnDown.Create(0,
					 WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
					 SECBitmapButton::Al_Center,
					 IDB_SMALL_DOWN,
					 CRect(0,0,100,100),
					 pParentWnd,
					 ID_VIEW_SCROLLHOLDDOWN);

	m_offset = 0;
}


void CHMLoadGraphSheetViewport::CHoldArea::ComputeRects(CHMLoadGraphSheetViewport* pView, 
												   const CRect& rectEntire)
{
	m_pViewport = pView;
	m_rectEntire = rectEntire;

	m_rectBody = m_rectEntire;

	// we leave 22 pixels at the top and at the bottom for the buttons
	m_rectBody.InflateRect(0,-22,0,-22);

	// Placing the buttons
	if ( ::IsWindow(m_btnUp.m_hWnd) )
		m_btnUp.MoveWindow(m_rectEntire.left,
						   m_rectEntire.top,
						   m_rectEntire.Width(),
						   20);
	if ( ::IsWindow(m_btnDown.m_hWnd) )
		m_btnDown.MoveWindow(m_rectEntire.left,
							 m_rectEntire.bottom-20,
							 m_rectEntire.Width(),
							 20);

	ComputeRects();

}


void CHMLoadGraphSheetViewport::CHoldArea::ComputeRects()
{


	CHMLoadGraphDoc* pDoc = m_pViewport->GetDocument();

	m_castStringDataVec.clear();

	//  Determine if we need to adjust the offset.
	//  First, count the strings.

	bool show910s = m_pViewport->GetDocument()->Show910s();

	int count = 0;
	{
		for ( int caster=1; caster<=3; ++caster ) {
			vector<CCastString*>& strings = pDoc->GetStrings(caster);
			for ( vector<CCastString*>::iterator is = strings.begin();
				  is != strings.end();
				  ++is ) {
				
				if ( (*is)->IsStartTimeValid() )
					continue;
	
				if ( (*is)->Status() == CCastString::STRING_STATUS_910
					 &&
					 ! show910s )
					 continue;
	
				++count;
			}
		}
	}


	if ( count - m_offset < NumVisible() )
		m_offset = max(0,count - NumVisible());

	if ( ::IsWindow(m_btnUp.m_hWnd) )
		m_btnUp.EnableWindow( m_offset != 0 );
	if ( ::IsWindow(m_btnDown.m_hWnd) )
		m_btnDown.EnableWindow( m_offset + NumVisible() < count );


	int lastY = -m_iconSpace*m_offset + 22;
	int index = 0;


	for ( int caster=1; caster<=3; ++caster ) {
		vector<CCastString*>& strings = pDoc->GetStrings(caster);
		for ( vector<CCastString*>::iterator is = strings.begin();
			  is != strings.end();
			  ++is ) {
			
			if ( (*is)->IsStartTimeValid() )
				continue;

			if ( (*is)->Status() == CCastString::STRING_STATUS_910
				 &&
				 ! show910s )
				 continue;

			CPoint origin(m_rectEntire.left,lastY);
			CSize size(m_rectEntire.Width(),m_iconHeight);
			CRect rect(origin,size);
			vector<CString> texts;
			ComputeCastStringText((*is),texts);
			COLORREF color = CHMLoadGraphSheetViewport::sm_colorLoad[caster][index%2];
			CCastStringData data((*is),
								 rect,
								 color,
								 texts);
			m_castStringDataVec.push_back( data );
			++index;
			lastY += m_iconSpace;
		}
	}
}


void CHMLoadGraphSheetViewport::CHoldArea::ScrollUp()
{
	m_offset -= max(1,NumVisible()-2);
	m_offset = max(m_offset,0);
	ComputeRects();

	CClientDC dc(m_pViewport->GetWnd());
	CDCState state(&dc);
	Draw(&dc);
}

void CHMLoadGraphSheetViewport::CHoldArea::ScrollDown()
{
	m_offset += max(1,NumVisible()-2);
	ComputeRects();

	CClientDC dc(m_pViewport->GetWnd());
	CDCState state(&dc);
	Draw(&dc);
}

void CHMLoadGraphSheetViewport::CHoldArea::OnUpdateScrollUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_offset > 0 );
}


void CHMLoadGraphSheetViewport::CHoldArea::OnUpdateScrollDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_offset+NumVisible() < m_castStringDataVec.size() );
}




void CHMLoadGraphSheetViewport::CHoldArea::ComputeCastStringText(CCastString* pString,
															 vector<CString>& texts)
{
	CString str;

	str.Format("%d-%d-%d %s - %d",
			    pString->Id().Caster(),
				pString->Id().Week(),
				pString->Id().StringId(),
				LPCTSTR(CHMLoadGraphDoc::FindSpecToUse(pString)),
				pString->Heats().size());
	texts.push_back( str );

}


void CHMLoadGraphSheetViewport::CHoldArea::RedrawString(CCastString* pString)
{
	CClientDC dc(m_pViewport->GetWnd());

	CDCState state(&dc);

	{
		for ( vector< CCastStringData >::iterator id = m_castStringDataVec.begin();
			  id != m_castStringDataVec.end();
			  ++id ) 

			if ( (*id).m_pString == pString )
				DrawCastStringText(&dc,(*id));
	}
}


void CHMLoadGraphSheetViewport::CHoldArea::Draw( CDC* pDC )
{
	if ( ! pDC->RectVisible(&m_rectBody) )
		return;

	pDC->SaveDC();

	CDCState state(pDC);

	CRgn rgn;
	if ( ! rgn.CreateRectRgnIndirect(m_rectBody) )
		AfxMessageBox("Problem creating region");

	int result = pDC->SelectClipRgn(&rgn,RGN_AND);
	if ( result == ERROR )
		AfxMessageBox("Problem selecting clip region.");
	else if ( result == NULLREGION ) {
		AfxMessageBox("Clip region unexpectedly null.");
		return;
	}
	
	{
		CBrush br(CHMLoadGraphSheetViewport::sm_colorCasterBack[1]);
		CBrush* pOldBrush = pDC->SelectObject(&br);
		pDC->Rectangle(&m_rectBody);
		pDC->SelectObject(pOldBrush);
	}

	{
		for ( vector< CCastStringData >::iterator id = m_castStringDataVec.begin();
			  id != m_castStringDataVec.end();
			  ++id ) 
			DrawCastStringText( pDC, (*id) );
	}

	if ( ! pDC->RestoreDC(-1) )
		AfxMessageBox("Failed to restore DC.");

	if ( ! rgn.DeleteObject() )
		AfxMessageBox("Error deleting region.");
}

	
void CHMLoadGraphSheetViewport::CHoldArea::DrawCastStringText( CDC* pDC, 
														   const CHMLoadGraphSheetViewport::CCastStringData& data)
{
	data.Draw(pDC,data.m_pString == m_pViewport->SelectedString() );
}


bool CHMLoadGraphSheetViewport::CHoldArea::HitTest(const CPoint& pt,
											   CHitData& rHitData) const
{

	vector< CCastStringData >::const_iterator id = GetCastStringDataAtPoint(pt);
	if ( id != m_castStringDataVec.end() ) {
		rHitData.m_casterNum = (*id).m_pString->Id().Caster();
		rHitData.m_pString = (*id).m_pString;
		rHitData.m_vecText = (*id).m_vecText;
		rHitData.m_rect = (*id).m_rect;
		rHitData.m_hitArea = HT_HOLD_AREA;
		rHitData.m_pHoldArea = this;
		return true;
	}

	return false;
}




vector< CHMLoadGraphSheetViewport::CCastStringData >::const_iterator CHMLoadGraphSheetViewport::CHoldArea::GetCastStringDataAtPoint(const CPoint& pt) const
{
	{
		for ( vector< CCastStringData >::const_iterator id = m_castStringDataVec.begin();
			  id != m_castStringDataVec.end();
			  ++id ) {

			if ( (*id).m_rect.PtInRect(pt) ) 
				return id;
		}
	}

	return m_castStringDataVec.end();

}



