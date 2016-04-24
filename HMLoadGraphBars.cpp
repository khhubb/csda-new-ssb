// HMLoadGraphBars.cpp: implementation of the CHMLoadGraphGraphBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "HMLoadGraphBars.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "HMLoadGraphView.h"
#include "HMLoadGraphDoc.h"
#include "CastString.h"
#include "HMSupplyMgr.h"
#include "Snapshot.h"



CHMLoadGraphGraphBar::CHMLoadGraphGraphBar(Dimension height,
										   Dimension offset,
										   double min,
										   double max,
										   CHMLoadGraphGanttViewport* pView)
: CGanttDisplayGraph(height,offset,min,max,TRUE),
  m_pView(pView)
{
}


#define at_end(i) (iters[i] == (records[i])->end())
#define maybe_advance(i,t) if ( ! at_end(i) && (*(iters[i])).m_time == (t) ) ++(iters[i])
#define safe_value(i,t) ( ( at_end(i) || (*(iters[i])).m_time != (t) ) ? 0.0 : (*(iters[i])).m_tons )

#define supply_at_end (isupply == (supplyRecords)->end())
#define supply_maybe_advance(t) if ( ! supply_at_end && (*isupply).GetDate() == (t) ) ++isupply
#define supply_safe_value(t) ( ( supply_at_end || (*isupply).GetDate() != (t) ) ? 0.0 : (*isupply).GetTonsPerHour() )

void CHMLoadGraphGraphBar::OnDraw(CDC* pDC,
								  XUnit exposedStartXUnit,
								  XUnit exposedFinishXUnit)
{
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();

	ClearToBackground(pDC,exposedStartXUnit,exposedFinishXUnit);

	COLORREF color = m_pView->DemandColor();
	COLORREF supplyColor = m_pView->SupplyColor();

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

		if ( rightTime < exposedStartXUnit )
			goto advance;

		if ( leftTime > exposedFinishXUnit )
			break;

		{
			Weight val1 = safe_value(3,someTime);
			Weight val2 = val1 + safe_value(2,someTime);
			Weight val3 = val2 + safe_value(1,someTime);
	
			if ( val1 != 0 )
				DrawBorderedRect(pDC,
							  m_pView->CasterColor(3),
							  leftTime,
							  rightTime,
							  0,
							  val1);
	
			if ( val2 != val1 )
				DrawBorderedRect(pDC,
							  m_pView->CasterColor(2),
							  leftTime,
							  rightTime,
							  val1,
							  val2);
	
			if ( val3 != val2 )
				DrawBorderedRect(pDC,
							  m_pView->CasterColor(1),
							  leftTime,
							  rightTime,
							  val2,
							  val3);

			DrawGraphLine(pDC,
						  supplyColor,
						  leftTime,
						  rightTime,
						  supply_safe_value(someTime),
						  5);
		}
advance:
		
		maybe_advance(1,someTime);
		maybe_advance(2,someTime);
		maybe_advance(3,someTime);
		supply_maybe_advance(someTime);
	}
 

	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  0);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  250);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  500);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  750);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  1000);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  1250);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  1500);



}





/////////////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphLoadCountBar
//
/////////////////////////////////////////////////////////////////////////


CHMLoadGraphLoadCountBar::CHMLoadGraphLoadCountBar(Dimension height,
												   Dimension offset,
												   double min,
												   double max,
												   CHMLoadGraphGanttViewport* pView)
: CGanttDisplayGraph(height,offset,min,max,TRUE),
  m_pView(pView)
{
}




void CHMLoadGraphLoadCountBar::OnDraw(CDC* pDC,
									  XUnit exposedStartXUnit,
									  XUnit exposedFinishXUnit)
{
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();
	double tonsPerPugh = TheSnapshot.HMSupplyMgr().TonsPerPugh();

	ClearToBackground(pDC,exposedStartXUnit,exposedFinishXUnit);


	COLORREF color = RGB(0x80,0x80,0x00);
	COLORREF supplyColor = m_pView->SupplyColor();

	vector<CHMLoadRecord>& records = pDoc->GetLoadRecords(0);

	for ( vector<CHMLoadRecord>::iterator ir = records.begin();
		  ir != records.end();
		  ++ir ) {

		CTime now = (*ir).m_time;
	
		time_t leftTime = now.GetTime();
		time_t rightTime = leftTime + 3600;

		if ( rightTime < exposedStartXUnit )
			continue;

		if ( leftTime > exposedFinishXUnit )
			break;

		if ( (*ir).m_tons == 0 )
			continue;
		else 
			DrawBorderedRect(pDC,
						  color,
						  leftTime,
						  rightTime,
						  0,
						  (*ir).m_tons/tonsPerPugh);
	}
 

	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  0);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  -10);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  -20);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  -30);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  -40);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  10);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  20);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  30);
	DrawGraphLine(pDC,
				  RGB(0x00,0x00,0x00),
				  exposedStartXUnit,
				  exposedFinishXUnit,
				  40);
}


/////////////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphCasterBar
//
/////////////////////////////////////////////////////////////////////////


CHMLoadGraphNumericBar::CHMLoadGraphNumericBar(Dimension height,
											   Dimension offset,
											   int which,
											   CHMLoadGraphGanttViewport* pView)
: CGanttDisplayBar(height,offset),
  m_pView(pView),
  m_type( CHMLoadGraphNumericBar::BarType( which ) )
{
}


void CHMLoadGraphNumericBar::OnDraw(CDC* pDC,
									XUnit exposedStartXUnit,
									XUnit exposedFinishXUnit)
{
	ClearToBackground(pDC,exposedStartXUnit,exposedFinishXUnit);
	
	switch ( m_type ) {
		case BAR_TYPE_CASTER1:
		case BAR_TYPE_CASTER2:
		case BAR_TYPE_CASTER3:
			DrawCasterLoad(pDC,exposedStartXUnit,exposedFinishXUnit);
			break;

		case BAR_TYPE_DEMAND:
			DrawCasterDemand(pDC,exposedStartXUnit,exposedFinishXUnit);
			break;

		case BAR_TYPE_SUPPLY:
			DrawCasterSupply(pDC,exposedStartXUnit,exposedFinishXUnit);
			break;
	}
}


void CHMLoadGraphNumericBar::DrawCasterLoad(CDC* pDC,
											XUnit exposedStartXUnit,
											XUnit exposedFinishXUnit)
{
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();

	COLORREF color = m_pView->CasterColor(m_type);

	vector<CHMLoadRecord>& records = pDoc->GetLoadRecords(m_type); // caster

	for ( vector<CHMLoadRecord>::iterator ir = records.begin();
		  ir != records.end();
		  ++ir ) {

		time_t leftTime = (*ir).m_time.GetTime();
		time_t rightTime = leftTime + 3600;

		if ( rightTime < exposedStartXUnit )
			continue;
		
		if ( leftTime > exposedFinishXUnit )
			return;
		
		CString strValue;
		strValue.Format("%ld",long((*ir).m_tons+0.5));
	
		DrawCenteredText(pDC,
						 strValue,
						 leftTime,
						 rightTime,
						 RGB(0xFF,0xFF,0xFF),
						 color);
	}
}


void CHMLoadGraphNumericBar::DrawCasterDemand(CDC* pDC,
											  XUnit exposedStartXUnit,
											  XUnit exposedFinishXUnit)
{
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();

	COLORREF color = m_pView->DemandColor();

	vector<CHMLoadRecord>* records[4];
	records[1] = &(pDoc->GetLoadRecords(1));
	records[2] = &(pDoc->GetLoadRecords(2));
	records[3] = &(pDoc->GetLoadRecords(3));

	vector<CHMLoadRecord>::iterator iters[4];
	iters[1] = records[1]->begin();
	iters[2] = records[2]->begin();
	iters[3] = records[3]->begin();

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

		
		time_t leftTime = someTime.GetTime();
		time_t rightTime = leftTime + 3600;

		if ( rightTime < exposedStartXUnit )
			goto advance;

		if ( leftTime > exposedFinishXUnit )
			break;

		{
			Weight w = safe_value(1,someTime) 
						+ safe_value(2,someTime)
						+ safe_value(3,someTime);
	
			CString strValue;
			strValue.Format("%ld",long(w+0.5));
		
			DrawCenteredText(pDC,
							 strValue,
							 leftTime,
							 rightTime,
							 RGB(0xFF,0xFF,0xFF),
							 color);
		}

advance:
		maybe_advance(1,someTime);
		maybe_advance(2,someTime);
		maybe_advance(3,someTime);
	}
 
}


void CHMLoadGraphNumericBar::DrawCasterSupply(CDC* pDC,
											  XUnit exposedStartXUnit,
											  XUnit exposedFinishXUnit)
{	
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();

	COLORREF supplyColor = m_pView->SupplyColor();

	vector<CHMSupplyRecord>* supplyRecords = &pDoc->GetSupplyRecords();
	
	for ( vector<CHMSupplyRecord>::iterator isupply = supplyRecords->begin();
		  isupply != supplyRecords->end();
		  ++isupply ) {

		time_t leftTime = (*isupply).GetDate().GetTime();
		time_t rightTime = leftTime + 3600;

		if ( rightTime < exposedStartXUnit )
			continue;

		if ( leftTime > exposedFinishXUnit )
			break;


		CString strValue;
//		strValue.Format("%ld+%ld+%ld=%ld",
//						long((*isupply).GetTonsPerHour(BF_5)+0.5),
//						long((*isupply).GetTonsPerHour(BF_6)+0.5),
//						long((*isupply).GetTonsPerHour(BF_7)+0.5),
//						long((*isupply).GetTonsPerHour()+0.5));

		strValue.Format("%ld",
						long((*isupply).GetTonsPerHour()+0.5));

		
		DrawCenteredText(pDC,
						 strValue,
						 leftTime,
						 rightTime,
						 RGB(0xFF,0xFF,0xFF),
						 supplyColor);
	}

}




/////////////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphCasterBar
//
/////////////////////////////////////////////////////////////////////////



CHMLoadGraphCasterBar::CHMLoadGraphCasterBar(Dimension height,
											 Dimension offset,
											 int caster,
											 CHMLoadGraphGanttViewport* pView)
: CGanttDisplayBar(height,offset),
  m_pView(pView),
  m_caster(caster),
  m_selectedStringIndex(-1)
{
}

void CHMLoadGraphCasterBar::OnDraw(CDC* pDC,
								   XUnit exposedStartXUnit,
								   XUnit exposedFinishXUnit)
{
	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();
	vector<CCastString*>& strings = pDoc->GetStrings(m_caster);

	ClearToBackground(pDC,exposedStartXUnit,exposedFinishXUnit);

	int index = 0;

	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is, ++index ) 
		DrawCastString((*is),index,pDC,exposedStartXUnit,exposedFinishXUnit);
}


void CHMLoadGraphCasterBar::DrawCastString(CCastString* pString,
										   int index,
										   CDC* pDC,
										   XUnit exposedStartXUnit,
										   XUnit exposedFinishXUnit)
{
	time_t start = pString->CastStartTime().GetTime();
	time_t finish = start + pString->TotalDuration();

	if ( finish < exposedStartXUnit )
		return;

	if ( start > exposedFinishXUnit )
		return;

	DrawCastString(pString,index,pDC);
}


void CHMLoadGraphCasterBar::DrawCastString(CCastString* pString,
										   int index,
										   CDC* pDC)
{
	time_t start = pString->CastStartTime().GetTime();
	time_t finish = start + pString->TotalDuration();

	COLORREF stringColor = m_pView->CasterColor(m_caster);

	bool bSelected = ( index == m_selectedStringIndex );

	CPen pen(PS_INSIDEFRAME,
			 (bSelected ? 5 : 1),
			 (bSelected ? RGB(0xFF,0x44,0x00) : RGB(0x00,0x00,0x00)));

	CPen* oldPen = pDC->SelectObject(&pen);

	DrawBorderedRect(pDC,
				  stringColor,
				  start,
				  finish,
				  1.0);

	pDC->SelectObject(oldPen);

	//if ( index == m_selectedStringIndex ) 
	//	DrawBorderRectangle(pDC,
	//						RGB(0xFF,0x88,0x00),
	//						start,
	//						finish,
	//						1.0);


	time_t heatStart = pString->CastStartTime().GetTime();
	int heatIndex = 0;

	for ( vector<CCastStringHeat>::iterator ih = pString->Heats().begin();
		  ih != pString->Heats().end();
		  ++ih, ++heatIndex  ) {

		time_t heatFinish = heatStart + (*ih).Duration().GetTotalSeconds();

		COLORREF heatColor = m_pView->HeatColor(m_caster,heatIndex);
		DrawBorderedRect(pDC,heatColor,heatStart,heatFinish,0.25);
		heatStart = heatFinish;
	}
				  

	static COLORREF textColor = RGB(0xFF,0xFF,0xFF);

	CString text;
	text.Format("%02d-%02d",
				pString->Id().Week(),
				pString->Id().StringId());

	DrawCenteredText(pDC,
					 text,
					 start,
					 finish,
					 stringColor,
					 textColor);
}



bool CHMLoadGraphCasterBar::OnLButtonDown(XUnit xUnit,
										  double value,
										  bool isInside,
										  bool isAbove,
										  CGanttDisplayBar* pActualBar)
{

#if 0
	afxDump << "CasterBar::LBD " << xUnit << " " << value << " " 
			<< isInside << " " << isAbove << "\n";
#endif

	
	if ( ! isInside )
		return false;

	int hitIndex;
	CCastString* pString = CastStringHitTest(xUnit,hitIndex);


#if 0
	afxDump << "Hit test returns ";
	if ( pString == 0 )
		afxDump << " NULL\n";
	else
		afxDump << pString->Id().Week() << "-" 
				<< pString->Id().StringId() << "\n";
#endif


	if ( pString == 0 )
		return false;

	if ( hitIndex == m_selectedStringIndex ) {
		// hit the same one again
		// let's start tracking
		m_bTracking = true;
		CastStringRect(pString,m_rectTracking);
		m_pTrackedString = pString;
		m_mouseXOffset = xUnit - m_rectTracking.TopLeft().x;
		m_dlgStartTime.m_pString = m_pTrackedString;
		m_dlgStartTime.Create(CPoint(0,0),m_pView->GetWnd());
		m_dlgStartTime.SetTime(pString->CastStartTime(),
							   pString->CastStartTime()+pString->TotalDuration());
		DrawTrackingOutline(m_rectTracking);
#if 0
		afxDump << "Begin tracking: " << m_rectTracking 
				<< ", offset = " << m_mouseXOffset
				<< "\n";
#endif
		return true;
	}
	else {

		CClientDC dc(m_pView->GanttDisplay()->Chart());
		m_pView->OnPrepareDC(&dc);
		
		//  undraw the focus around currently selected string., if any
		vector<CCastString*>& strings = m_pView->GetDocument()->GetStrings(m_caster);
		if ( 0 <= m_selectedStringIndex && m_selectedStringIndex < strings.size() ) {

#if 0
			afxDump << "Unselect previous selection at index " << m_selectedStringIndex << "\n";
#endif
			CCastString* pString = strings[m_selectedStringIndex];
			int stringIndex = m_selectedStringIndex;
			m_selectedStringIndex = -1;
			DrawCastString(pString,stringIndex,&dc);
		}

#if 0
		afxDump << "Select at index " << hitIndex << "\n";
#endif
		// Draw the focus around the newly selected string.
		m_selectedStringIndex = hitIndex;
		DrawCastString(pString,hitIndex,&dc);
	}


	return false;
}


void CHMLoadGraphCasterBar::OnMouseMove(XUnit xUnit,
									    double /* value */,
										CGanttDisplayBar* /* pActualBar */)
{
	if ( ! m_bTracking )
		return;

	assert( m_pTrackedString != 0 );


	CRect oldRect( m_rectTracking );

	int deltaX = xUnit - m_rectTracking.TopLeft().x - m_mouseXOffset;
	m_rectTracking += CSize(deltaX,0);

	//  Determine if we are close to the end (within 5 pixels) of the viewed area.
	
	CRect rect;
	m_pView->GanttDisplay()->Chart()->GetViewedRect(rect);

	Position xCoord = m_pView->GanttDisplay()->Chart()->XlateXUnitToXCoordNoTruncate(xUnit);

#if 0
	afxDump << "ViewedRect is " << rect << ", xCoord = " << long(xCoord) << " -- ";
#endif

	if ( xCoord - rect.TopLeft().x <= 5 ) {
		DrawTrackingOutline(oldRect);
		m_pView->ScrollTime(-3600);
		DrawTrackingOutline(m_rectTracking);
	}
	else if ( rect.BottomRight().x - xCoord <= 5 ) {
		DrawTrackingOutline(oldRect);
		m_pView->ScrollTime(3600);
		DrawTrackingOutline(m_rectTracking);
	}
	else 
		MoveTrackingOutline(m_rectTracking,oldRect);

#if	0
	afxDump << "Moving to " << m_rectTracking << "\n";
#endif

	m_dlgStartTime.SetTime(CTime(m_rectTracking.TopLeft().x),
						   CTime(m_rectTracking.BottomRight().x));
}


void CHMLoadGraphCasterBar::OnLButtonUp(XUnit xUnit,
										double /* value */,
										CGanttDisplayBar* /* pActualBar */)
{	
	if ( ! m_bTracking )
		return;

	assert( m_pTrackedString != 0 );

	m_dlgStartTime.DestroyWindow();

	DrawTrackingOutline(m_rectTracking);
	MoveString(m_pTrackedString,CTime(m_rectTracking.TopLeft().x));

	m_bTracking = 0;
	m_pTrackedString = 0;
	m_rectTracking.SetRectEmpty();
	m_mouseXOffset = 0;

}






void CHMLoadGraphCasterBar::MoveTrackingOutline(CRect& newRect,
												CRect& oldRect)
{
	CGanttChart* pChart = m_pView->GanttDisplay()->Chart();
	CClientDC dc(pChart);
	m_pView->OnPrepareDC(&dc);

	CRect xNewRect(pChart->XlateXUnitToXCoordNoTruncate(newRect.TopLeft().x),
				 	m_topY,
					pChart->XlateXUnitToXCoordNoTruncate(newRect.BottomRight().x),
					m_bottomY);

	CRect xOldRect(pChart->XlateXUnitToXCoordNoTruncate(oldRect.TopLeft().x),
				 	m_topY,
					pChart->XlateXUnitToXCoordNoTruncate(oldRect.BottomRight().x),
					m_bottomY);

	//  DrawDragRect does not adjust properly for the scroll position.
	//  Sigh.

	CPoint scrollPt = pChart->GetScrollPosition();
	xNewRect -= scrollPt;
	xOldRect -= scrollPt;

#if 0
	afxDump << "MoveRect: from " << xOldRect << " to " << xNewRect << "\n";
#endif

	dc.DrawDragRect(&xNewRect,CSize(5,5),&xOldRect,CSize(5,5),0,0);

#if 0
	afxDump << "MoveRect: from ";
#endif


	//DrawTrackingOutline(oldRect);

#if 0
	afxDump << "MoveRect: to ";
#endif


	//DrawTrackingOutline(newRect);
}


void CHMLoadGraphCasterBar::DrawTrackingOutline(CRect& rect)
{
	CGanttChart* pChart = m_pView->GanttDisplay()->Chart();
	CClientDC dc(pChart);
	pChart->OnPrepareDC(&dc);

	CRect xrect(pChart->XlateXUnitToXCoordNoTruncate(rect.TopLeft().x),
				m_topY,
				pChart->XlateXUnitToXCoordNoTruncate(rect.BottomRight().x),
				m_bottomY);
#if 0
	afxDump << "DrawRect: " << xrect << "originally " << rect << "\n";
#endif

	//  DrawDragRect does not adjust properly for the scroll position.
	//  Sigh.

	CPoint scrollPt = pChart->GetScrollPosition();
	xrect -= scrollPt;

	dc.DrawDragRect(&xrect,CSize(5,5),0,CSize(5,5),0,0);

	//dc.SetROP2(R2_NOT);
	//dc.SelectStockObject(HOLLOW_BRUSH);
	//dc.SelectStockObject(WHITE_PEN);
	//DrawRectangle(&dc,
	//			  RGB(0xFF,0xFF,0xFF),
	//			  m_rectTracking.TopLeft().x,
	//			  m_rectTracking.BottomRight().x,
	//			  1.0);

	//dc.Rectangle(xrect);
}




CCastString* CHMLoadGraphCasterBar::CastStringHitTest(double xUnit,int& rIndex)
{
	time_t time = time_t(xUnit);

	CHMLoadGraphDoc* pDoc = m_pView->GetDocument();
	vector<CCastString*>& strings = pDoc->GetStrings(m_caster);

	int index = 0;
	for ( vector<CCastString*>::iterator is = strings.begin();
		  is != strings.end();
		  ++is, ++index ) {

		time_t start = (*is)->CastStartTime().GetTime();
		time_t finish = start + (*is)->TotalDuration();
#if _DEBUG
		afxDump << "String " << (*is)->Id().Week() 
				<< "-" << (*is)->Id().StringId()
				<< ": " << start << " " << finish 
				<< " vs " << time
				<< "\n";
#endif
		if ( start <= time && time <= finish ) {
			rIndex = index;
			return (*is);
		}
	}

	rIndex = -1;
	return 0;
}


void CHMLoadGraphCasterBar::CastStringRect(CCastString* pString, CRect& rect)
{
	time_t start = pString->CastStartTime().GetTime();
	time_t finish = start + pString->TotalDuration();

	rect.left	= start;
	rect.right	= finish;
	rect.top	= m_topY;
	rect.bottom	= m_bottomY;
}


void CHMLoadGraphCasterBar::MoveString(CCastString* pString, CTime newStartTime)
{
#if _DEBUG
	afxDump << "Moving string to " << newStartTime << "\n";
#endif
	
	CTime currentComputedStartTime = pString->CastStartTime();

//	pString->SetUserCastStartTime(newStartTime);
	pString->SetCastStartTime(newStartTime);

	m_pView->GetDocument()->ComputeLoad();
	
	if ( currentComputedStartTime != pString->CastStartTime() )
		m_pView->Invalidate();
}

