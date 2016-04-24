// GanttDisplay.cpp: implementation of the CGanttDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "GanttDisplay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
 

#include <math.h>
#include "nTime.h"

 

//////////////////////////////////////////////////////////////////////////////
//
//  The CGanttDisplay class displays a gantt chart.
//  
//  The display is a CSplitterWnd, with two panes (static).
//  The left pane hold labels.
//  The right pane is a drawing area (a CGanttChart)
//		upon which is drawn the chart itself.
//  The drawing area is derived from CScrollView, 
//		so scrolling works accordingly.
//  Button clicks also invoke callbacks depending on the caller.

//  The drawing area is to be used to display gantt charts or similar 
//		time-based graphs.   The horizontal axis is the time axis.
//  There is no need for the units to be time-based, however --
//		all values for the x coordinate are double, with a range and a mapping
//		from user units to pixels supplied by the user 
//		(thus also allowing zooming).
//  The vertical axis is broken into several different components, 
//		derived from CGanttDisplayBar.
  
//  The CGanttChart can be used alone, 
//		but there won't be labels for the CGanttDisplayBar.
//  Almost all methods of CGanttDisplay, 
//		except creation of the display and labels, 
//		delegate to the contained CGanttChart.
//
//////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
//
//  GanttDisplay c-tor, d-tor and related.
//
////////////////////////////////////////////////////////////////////////


CGanttDisplay::CGanttDisplay()
{
	m_pChart		= 0;
	m_pLabelView	= 0;
	m_pLabelFont	= 0;
}


CGanttDisplay::~CGanttDisplay()
{
	// the m_pChart and m_pLabelView, both views, will be destroyed elsewhere,
	// if anyone cares.

	delete m_pLabelFont;
}



//  The display is created with an outer splitter window. 1x2.
//  The left pane is a CScrollView that will hold any buttons or labels
//  The right pane is a CGanttChart (derived from CScrollView).
//
//  Args:
//
//		pParentWnd -- the window which is to parent the splitter
//		pViewClass -- usually CGanttChart's class
//                    allows the gantt view class to be specified
//		labelSize  -- how big the label view should be
//		chartSize  -- how big the chart should be
//		pContext   -- to be passed to the views' Create
//		dwStyle    -- to be passed to the splitter's CreateStatic
//		nID		   -- ditto
//

bool CGanttDisplay::Create(CWnd* pParentWnd, 
						   CRuntimeClass* pViewClass,
						   SIZE labelSize, 
						   SIZE chartSize, 
						   CCreateContext* pContext,
						   DWORD dwStyle, 
						   UINT nID)
{

	assert( m_pChart == 0 && m_pLabelView == 0 );

	assert( pViewClass->IsDerivedFrom(RUNTIME_CLASS(CGanttChart)) );


	if ( ! m_splitterWnd.CreateStatic(pParentWnd,1,2,dwStyle,nID) ) {
		TRACE0("Unable to create splitter window in CGanttDisplay.");
		return false;
	}

	m_splitterWnd.SetScrollStyle(WS_VSCROLL|WS_HSCROLL);

	if ( ! m_splitterWnd.CreateView(0,0,
									 RUNTIME_CLASS(CGanttLabelView),
									 labelSize,
									 pContext) ) {
		TRACE0("Unable to create label view in CGanttDisplay.");
		return false;
	}

	if ( ! m_splitterWnd.CreateView(0,1,
									pViewClass,
									chartSize,
									pContext) ) {
		TRACE0("Unable to create chart view in CGanttDisplay.");
		return false;
	}


	m_pLabelView	= dynamic_cast<CGanttLabelView*>(m_splitterWnd.GetPane(0,0));
	m_pChart		= dynamic_cast<CGanttChart*>(m_splitterWnd.GetPane(0,1));

	assert( m_pLabelView != 0 );
	assert( m_pChart != 0 );

	// set backpointers
	m_pLabelView->m_pGanttDisplay = this;
	m_pChart->m_pGanttDisplay = this;

	// create font for labels
	m_pLabelFont = new CFont;
	m_pLabelFont->CreatePointFont( 70, "MS Dialog" );  // Courier New
	m_pLabelView->SetFont( m_pLabelFont, false );

	return true;
}


// One of the few operations that don't directly delegate
// after resizing, we must also change the height of the label view
//   to match the chart.
void CGanttDisplay::SetDimensions()
{ 
	m_pChart->SetDimensions();
	m_pLabelView->SetHeight(m_pChart->m_areaHeight);
}





//  The GanttDisplayBarLabel class contains
//    a text string to display in the label/pushbutton
//    a value to control the placement (e.g., the label on a graph line)
//    an indication of whether a label or pushbutton is desired
//
//  For a pushbutton, a default callback is is added which will
//    call a virtual method on the bar itself.
//
//  TODO -- figure out how to handle the callbacks.
//  The old version of KIIP used the push buttons, the new one does not,
//   so I never got around to implementing it.


//  To create the labels for a bar belonging to a gantt display,
//    call this entry point.  The bar will do the actual creation,
//    but needs to know the labelView and font, 
//    which come from the CGanttDisplay.

void CGanttDisplay::CreateBarLabels( CGanttDisplayBar* pBar)
{
	pBar->CreateBarLabels(m_pLabelView,m_pLabelFont);
}



// Prior to doing this, you would have added labels (via AddLabel)
//  to the bar, which are stored in a list.
// Iterate through the list and create the control for each label

void CGanttDisplayBar::CreateBarLabels(CGanttLabelView* pParentWnd,
									   CFont* pLabelFont)
{

   
    for ( vector<CGanttDisplayBarLabel>::iterator il = m_labels.begin();
	      il != m_labels.end();
	      ++il) 

		(*il).CreateWnd(pParentWnd,pLabelFont,this);
}


// Call this after a bar has been resized in some way.
// Iterate through the list of labels and have each label position itself.

void CGanttDisplayBar::PositionLabels()
{
   for ( vector<CGanttDisplayBarLabel>::iterator il = m_labels.begin();
	      il != m_labels.end();
	      ++il) 

		(*il).PlaceWnd();
}


//  Create the control for a label

void CGanttDisplayBarLabel::CreateWnd(CGanttLabelView* pParent_, 
										 CFont* pFont, 
										 CGanttDisplayBar* pBar_)
{
	m_pBar = pBar_;
	m_pParent = pParent_;

	long width = 90;

	// this static variable is just to make sure that each bar label
	// that is created is offset vertically from the others.  
	//  Not important in the final scheme of things, 
	//  but it was useful when first debugging this code.

	static int bogus = 0;

	CRect initRect(0,bogus,100,bogus+20);
	bogus += 25;

	
	if ( m_isButton ) {
		
		// Here is one place we'd have to contemplate dealing
		//  with callbacks to handle the button click.
	
		CButton* pButton = new CButton;
		pButton->Create(m_text,
						WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
						initRect,
						m_pParent,
						15);
		m_pWnd = pButton;
	}
	else {
		CStatic* pStatic = new CStatic;
		pStatic->Create(m_text, 
						WS_CHILD|WS_VISIBLE|SS_RIGHT,
			            initRect,
						m_pParent,
						15);
		m_pWnd = pStatic;
	}

	m_pWnd->SetFont( pFont, false );
	m_pWnd->ShowWindow( SW_RESTORE );
	
	PlaceWnd();
}


//  Place a bar label according to its size and value relative to its bar

void CGanttDisplayBarLabel::PlaceWnd()
{

	// Determine how much space the text takes up,
	// and find out the required size of the button/static to fit that.

	CClientDC dc(m_pWnd);
	CSize textSize = dc.GetOutputTextExtent(m_text);

	CRect rect(CPoint(0,0),textSize);
	m_pWnd->CalcWindowRect(rect);

 

	// Find out how big the parent window is

	CRect parentRect;
	m_pParent->GetClientRect(parentRect);

	

	// Calculate the position of the button/static so that it lies
	//   along the right edge of the parent, at the appropriate height.

	Position center = m_pBar->XlateValueToYCoord(m_value);

	CRect finalRect = rect + CSize(parentRect.Width()-rect.Width()-5,
			      center-rect.Height()/2);

	  CPoint scrollPosn = -m_pBar->Chart()->GetScrollPosition();
	  finalRect.OffsetRect(scrollPosn);

	m_pWnd->MoveWindow(finalRect);

#if 0
	afxDump << "Label '" << m_text << "' at coordinate " << m_value << "\n"
			<< "      textSize   = " << textSize << "\n"
			<< "      windowRect = " << rect << "\n"
			<< "      parentRect = " << parentRect << "\n"
			<< "          center = " << center << "\n"
			<< "       finalRect = " << finalRect << "\n\n";
#endif
														
}


 

////////////////////////////////////////////////////////////////////////
//
//  GanttDisplayBar
//  GanttDisplayGraph
//  GanttDisplayDay
//  GanttDisplayHour
//
////////////////////////////////////////////////////////////////////////


// Calculate an offset from the base line corresponding to a particular value.
// Values outside the [minValue,maxValue] range for the bar are moved to 
//   the nearest value.
// The default implementation just gives the midpoint 
//   between topY and bottomY.
// The implementation for GanttDisplayGraph actual does some translation.

Position CGanttDisplayBar::XlateValueToDistance(double)
{
  return (m_bottomY - m_topY)/2;
}


// This returns an offset from the baseLine.
// Whether this should be added to the topY or subtracted from the bottomY  
//   depends on the graph.
 
Position CGanttDisplayGraph::XlateValueToDistance(double value)
{
  // Move the value to within range.
  value = max(value,m_minValue);
  value = min(value,m_maxValue);


  // Compute the fraction of the distance the value is between
  //  minValue and maxValue;
  double fraction = ( value - m_minValue ) / ( m_maxValue - m_minValue);

  //  Compute the offset.
  double foffset = fraction * m_height;

  //  Round
  Position delta = Position(floor(foffset+0.5));
	 
  return(delta);
}


// Compute the absolute (drawing area, not relative to the bar) Ycoord 
//    corresonding to a value.
// GanttDisplayBar: just add the relative coord to the topY.
// GanttDisplayGraph:  depends on whehter top or bottom is the baseline.


Position CGanttDisplayBar::XlateValueToYCoord( double value )
{
  return m_topY +  XlateValueToDistance(value);
}


Position CGanttDisplayGraph::XlateValueToYCoord( double value )
{
  if ( m_originAtBottom ) 
    return m_bottomY - XlateValueToDistance(value);
  else
    return m_topY + XlateValueToDistance(value);
}



//  And, going back the other way.

double CGanttDisplayBar::XlateYCoordToValue(Position)
{
	// translate to the midpoint

	return 0.5;
}


double CGanttDisplayGraph::XlateYCoordToValue(Position y)
{
	// here we do a little interpolation

	// Compute the fraction of the distance the value is between

	double fraction = double( y - m_topY ) / ( m_bottomY - m_topY);

	//  Compute the offset.
	double foffset = fraction * (m_maxValue-m_minValue);

	return m_minValue+foffset;
}



// Given start and finish times and a value, draw the appropriate rectangle.
// The value is used by GanttDisplayGraph.
//
// Philosophy: the bars are responsible for computing Y-coords
//      The ganttDisplay is responsible for computing X-coords from XUnits.
//
//
//  The value here defaults to 1.0, meaning to draw across the entire bar.
//  A value between 0 and 1.0 indicates the fraction of the bar to draw across.
//
//  The following flavors of drawing are provided:
//	  (B = for CGanttDisplayBar, G for CGanttDisplayGraph)
//	BG  DrawBorderedRect -- draw a rect of given color, with border using current pen
//  BG  DrawRect         -- draw a rect of given color, without border
//  BG  DrawRectBorder   -- draw only the border of a rect
//	 G  DrawBorderedRect -- 2 values, 
//   G  DrawRect         -- 2 values,
//   G  DrawRectBorder   -- 2 values,
//  
//  In the 1 value versions, we go from the bottom to the indicated value on the y-axis
//  In the 2 value versions, both rectangle y-axis borders are given
//
//  We capture the difference between CGanttDisplayBar and CGanttDisplayGraph on the
//     1 value version in the virtual method ComputeRectY


void CGanttDisplayBar::ComputeRectYs(double value, Position& rTopY, Position& rBotY)
{
	value = max(0.0,min(1.0,value));
	rTopY = m_bottomY - Position(value*(m_bottomY-m_topY));
	rBotY = m_bottomY;
}

void CGanttDisplayGraph::ComputeRectYs(double value, Position& rTopY, Position& rBotY)
{

	if ( m_originAtBottom ) {
		rBotY = m_bottomY;
		rTopY    = XlateValueToYCoord(value);
	}
	else {
		rTopY    = m_topY;
		rBotY = XlateValueToYCoord(value);
	}
}


void CGanttDisplayBar::ComputeRectYs(double value1, double value2, Position& rTopY, Position& rBotY)
{
	Position y1 = XlateValueToYCoord(value1);
	Position y2 = XlateValueToYCoord(value2);

	if ( y2 < y1 ) {
		Position temp = y1;
		y1 = y2;
		y2 = temp;
	}

	rTopY = y1;
	rBotY = y2;
}



void CGanttDisplayGraph::ComputeRectYs(double value1, double value2, Position& rTopY, Position& rBotY)
{
	Position y1 = XlateValueToYCoord(value1);
	Position y2 = XlateValueToYCoord(value2);

	if ( y2 < y1 ) {
		Position temp = y1;
		y1 = y2;
		y2 = temp;
	}

	rTopY = y1;
	rBotY = y2;
}



void CGanttDisplayBar::DrawBorderedRect( CDC*		pDC,
										COLORREF	color,
										XUnit		startTime,
										XUnit		finishTime,
										double   value )
{
	Position topY;
	Position botY;

	ComputeRectYs(value,topY,botY);

	m_pChart->DrawBorderedRect( pDC,
								color,
								startTime,
								finishTime,
								topY,
								botY);                  
}


void CGanttDisplayBar::DrawRect( CDC*		pDC,
								COLORREF	color,
								XUnit		startTime,
								XUnit		finishTime,
								double   value )
{
	Position topY;
	Position botY;

	ComputeRectYs(value,topY,botY);

	m_pChart->DrawRect( pDC,
						color,
						startTime,
						finishTime,
						topY,
						botY);                  
}



void CGanttDisplayBar::DrawRectBorder(CDC* pDC,
										   COLORREF color,
											XUnit startX,
											XUnit finishX,
											double value)
{
	Position topY;
	Position botY;

	ComputeRectYs(value,topY,botY);

	m_pChart->DrawRectBorder( pDC,
							  color,
							  startX,
							  finishX,
							  topY,
							  botY);                  
}






void CGanttDisplayBar::DrawBorderedRect(CDC*			pDC,
									   COLORREF		color,
									   XUnit		startXUnit,
									   XUnit		finishXUnit,
									   double		value1,
									   double		value2)
{  
	Position topY;
	Position botY;

	ComputeRectYs(value1,value2,topY,botY);

	m_pChart->DrawBorderedRect( pDC,
								color,
								startXUnit,
								finishXUnit,
								topY,
								botY);                  
}


void CGanttDisplayBar::DrawRect(CDC*			pDC,
								  COLORREF		color,
								  XUnit		startXUnit,
								  XUnit		finishXUnit,
								  double		value1,
								  double		value2)
{  
	Position topY;
	Position botY;

	ComputeRectYs(value1,value2,topY,botY);

	m_pChart->DrawRect( pDC,
						color,
						startXUnit,
						finishXUnit,
						topY,
						botY);                  
}



void CGanttDisplayBar::DrawRectBorder(CDC*			pDC,
										   COLORREF		color,
										   XUnit		startXUnit,
										   XUnit		finishXUnit,
										   double		value1,
										   double		value2)
{  
	Position topY;
	Position botY;

	ComputeRectYs(value1,value2,topY,botY);

	m_pChart->DrawRectBorder(pDC,
							 color,
							 startXUnit,
							 finishXUnit,
							 topY,
							 botY);                  

}



//  Draw a line of the appropriate color at an indicated value.
//  For GanttDisplayBar, this will use the midpoint (value is ignored)
//  For GanttDisplayGraph, the value is xlated to a y-coord.


void CGanttDisplayBar::DrawGraphLine(CDC*		pDC,
									 COLORREF	color,
									 XUnit		startXUnit,
									 XUnit		finishXUnit,
				                     double		value,
									 unsigned	pixelWidth)
{
   m_pChart->DrawHorizLine(pDC,
						   color,
						   startXUnit,
						   finishXUnit,	
						   XlateValueToYCoord(value),
			               pixelWidth );
}



// Draw a vertical line of the appropriate color at the indicated x-value
// running from specified value to specified value.

void CGanttDisplayBar::DrawVertLine(CDC*		pDC,
									COLORREF	color,
									XUnit		xUnit,
									double		minValue,
									double		maxValue,
									unsigned	lineWidth)
{
	minValue = max(0.0,min(1.0,minValue));
	maxValue = max(0.0,max(1.0,maxValue));

	Position topToUse = m_bottomY - Position(maxValue*(m_bottomY-m_topY));
	Position botToUse = m_bottomY - Position(minValue*(m_bottomY-m_topY));

	
	m_pChart->DrawVertLine(pDC,
						   color,
						   xUnit,
						   botToUse,
						   topToUse,
						   lineWidth);
}




//  Display text centered horiz between the given times,
//               centered vert in the bar's region.


void CGanttDisplayBar::DrawCenteredText(CDC*			pDC,
										const CString&	text,
				                        XUnit			leftXUnit,
				                        XUnit			rightXUnit,
                                        COLORREF		bkColor,
                                        COLORREF		textColor )
{
    m_pChart->DrawCenteredText(pDC,
							   text,
							   leftXUnit,
							   rightXUnit,
							   m_topY,
							   m_bottomY,
							   bkColor,
							   textColor );
}



// hmmm, clear to background

void CGanttDisplayBar::ClearToBackground(CDC* pDC,
										 XUnit leftXUnit,
										 XUnit rightXUnit)
{
	CGdiObject* pOldPen = pDC->SelectStockObject(NULL_PEN);

	m_pChart->DrawBorderedRect(pDC,
							m_bgColor,
							leftXUnit,
							rightXUnit,
							m_topY,
							m_bottomY);
	pDC->SelectObject(pOldPen);
}



// Add a label to a bar.

void CGanttDisplayBar::AddLabel(const CString& text, 
								double value, 
								bool isButton )
{
    CGanttDisplayBarLabel label(text,value,isButton);

    m_labels.push_back( label );

#if 0
  afxDump << "GanttDisplayBar::AddLabel -- " << text 
		   << " " << value 
		   << " " << (isButton ? "button" : "label")
		   << "\nCurrently:\n";

  for ( vector<CGanttDisplayBarLabel>::iterator ib = m_labels.begin();
	    ib != m_labels.end();
	    ++ib ) 
    afxDump << "  --> " << (*ib).m_text 
	 << " " << (*ib).m_value
	 << " " << ((*ib).m_isButton ? "button" : "label")
	 << "\n";

  cout << "\n";
#endif
  
}




//  The class GanttDisplayDay, derived from GanttDisplayBar,
//    is used to graph days and hours on the chart.
//  It defines the exposure callback.

//  Almost identical class CGanttDisplayRelDay/CGanttDisplayRelHour, 
//    also derived from GanttDisplayBar,
//    graphs days/hours offsets (relative times).


CGanttDisplayDay::CGanttDisplayDay( Dimension height_, 
				                    Dimension offset_, 
				                    vector<COLORREF>& colors,
                                    COLORREF textColor )

	: CGanttDisplayBar(height_,offset_)
{
  m_dayColors = colors;
  SetTextColor( textColor );
}

CGanttDisplayRelDay::CGanttDisplayRelDay( Dimension height_, 
				                          Dimension offset_, 
				                          vector<COLORREF>& colors,
                                          COLORREF textColor )

	: CGanttDisplayBar(height_,offset_)
{
  m_dayColors = colors;
  SetTextColor( textColor );
}



//  Graph the region on the chart corresponding to a particular day.
//  time_t day -- guaranteed to be a multiple of 24*3600

//  For relative, the long count indicates the ordinal of the day


void CGanttDisplayDay::DrawSpecificDay(CDC* pDC, time_t thisDay )
{
    time_t nextDay = thisDay+24*3600;
  
    DrawBorderedRect( pDC,
				   m_dayColors[(thisDay/(24*3600)) % m_dayColors.size()],
		           thisDay,
		           nextDay,
				   1);

  char buffer[200];

  strftime(buffer,200,"%a %m/%d",localtime(&thisDay));

  DrawCenteredText( pDC, 
					buffer, 
					XUnit(thisDay), 
					XUnit(nextDay), 
                   m_dayColors[(thisDay/(24*3600)) % m_dayColors.size()], 
					TextColor() );

#if 0
  afxDump << "Drawing " << buffer 
		  << " at [" << thisDay 
		  << "," << nextDay 
		  << "] => [" << m_pChart->XlateXUnitToXCoord(XUnit(thisDay))
		  << "," << m_pChart->XlateXUnitToXCoord(XUnit(nextDay))
		  << "].\n";
#endif

}


void CGanttDisplayRelDay::DrawSpecificDay(CDC* pDC, long count )
{
	long thisDay = count*24*3600;
	long nextDay = thisDay+24*3600;
  
    DrawBorderedRect( pDC,
				   m_dayColors[count % m_dayColors.size()],
		           thisDay,
		           nextDay,
				   1);

  char buffer[200];

  sprintf(buffer,"Day %d",count+1);

  DrawCenteredText( pDC, 
					buffer, 
					XUnit(thisDay), 
					XUnit(nextDay), 
                   m_dayColors[count % m_dayColors.size()], 
					TextColor() );
}



// Draw all days that overlap the exposed times.

void CGanttDisplayDay::OnDraw(CDC* pDC,
							  XUnit  exposedStartXUnit,
				              XUnit  exposedFinishXUnit)
{
	time_t exposedStartTime = time_t(floor(exposedStartXUnit));
	time_t exposedFinishTime = time_t(ceil(exposedFinishXUnit));

  for ( time_t day = NTime::ThisDayMidnight(exposedStartTime); 
        day<=exposedFinishTime; 
        day += 24*3600 ) 
    DrawSpecificDay(pDC,day /* scrOffset */ );
    
}


// Draw all days that overlap the exposed times.

void CGanttDisplayRelDay::OnDraw(CDC* pDC,
					  		     XUnit  exposedStartXUnit,
				                 XUnit  exposedFinishXUnit)
{
	long exposedStartTime = long(floor(exposedStartXUnit));
	long exposedFinishTime = long(ceil(exposedFinishXUnit));

	long startCount = exposedStartTime/(24*3600);
	long finishCount = exposedFinishTime/(24*3600);

  for ( long dayOrdinal = startCount; 
        dayOrdinal <= finishCount; 
        ++dayOrdinal) 
    DrawSpecificDay(pDC,dayOrdinal);
    
}


CGanttDisplayHour::CGanttDisplayHour( Dimension height_, 
				                      Dimension offset_, 
				                      vector<COLORREF>& colors,
                                      COLORREF textColor )
                                       : CGanttDisplayBar(height_,offset_ )
{
   m_bMilitary = true;
   m_b2Digits  = true;
   m_hourColors = colors;
   SetTextColor( textColor );
}

CGanttDisplayRelHour::CGanttDisplayRelHour( Dimension height_, 
				                            Dimension offset_, 
				                            vector<COLORREF>& colors,
                                            COLORREF textColor )
           : CGanttDisplayBar(height_,offset_ )
{
   m_bMilitary = true;
   m_b2Digits  = true;
   m_hourColors = colors;
   SetTextColor( textColor );
}



static char *Military2HourLabels[] = {
  "00", "01", "02", "03", "04", "05", 
  "06", "07", "08", "09", "10", "11",
  "12", "13", "14", "15", "16", "17",
  "18", "19", "20", "21", "22", "23"
};

static char *Military4HourLabels[] = {
  "0000", "0100", "0200", "0300", "0400", "0500", 
  "0600", "0700", "0800", "0900", "1000", "1100",
  "1200", "1300", "1400", "1500", "1600", "1700",
  "1800", "1900", "2000", "2100", "2200", "2300"
};

static char *English2HourLabels[] = {
  "12M", " 1A", " 2A", " 3A", " 4A", " 5A", 
  " 6A", " 7A", " 8A", " 9A", "10A", "11A",
  "12N", " 1P", " 2P", " 3P", " 4P", " 5P",
  " 6P", " 7P", " 8P", " 9P", "10P", "11P"
};

static char *English4HourLabels[] = {
  "12:00 M", " 1:00 A", " 2:00 A", " 3:00 A", " 4:00 A", " 5:00 A", 
  " 6:00 A", " 7:00 A", " 8:00 A", " 9:00 A", "10:00 A", "11:00 A",
  "12:00 N", " 1:00 P", " 2:00 P", " 3:00 P", " 4:00 P", " 5:00 P",
  " 6:00 P", " 7:00 P", " 8:00 P", " 9:00 P", "10:00 P", "11:00 P"
};




void CGanttDisplayHour::DrawSpecificHour(CDC* pDC, time_t hour) 
{
    time_t hourIndex = (hour - NTime::ThisDayMidnight(hour)) / 3600;

    DrawBorderedRect(pDC,
				  m_hourColors[hourIndex % m_hourColors.size()],
				  XUnit(hour),
				  XUnit(hour + 3600),
				  1);

	char ** labels;

	if ( m_bMilitary ) {
		if ( m_b2Digits ) 
			labels = Military2HourLabels;
		else
			labels = Military4HourLabels;
	}
	else if ( m_b2Digits )
		labels = English2HourLabels;
	else
		labels = English4HourLabels;

   DrawCenteredText(pDC,
					labels[hourIndex], 
					XUnit(hour), 
					XUnit(hour+3600), 
                    m_hourColors[hourIndex % m_hourColors.size()], 
					TextColor() );

#if 0
  afxDump << "Drawing hour" << hourIndex 
		  << " at [" << hour 
		  << "," << hour+3600
		  << "] => [" << m_pChart->XlateXUnitToXCoord(XUnit(hour))
		  << "," << m_pChart->XlateXUnitToXCoord(XUnit(hour+3600))
		  << "].\n";
#endif

}


void CGanttDisplayRelHour::DrawSpecificHour(CDC* pDC, long hour) 
{
	XUnit startXUnit = hour*3600;
	XUnit finishXUnit = startXUnit + 3600;

    DrawBorderedRect(pDC,
				  m_hourColors[hour % m_hourColors.size()],
				  startXUnit,
				  finishXUnit,
				  1);

	char ** labels;

	if ( m_bMilitary ) {
		if ( m_b2Digits ) 
			labels = Military2HourLabels;
		else
			labels = Military4HourLabels;
	}
	else if ( m_b2Digits )
		labels = English2HourLabels;
	else
		labels = English4HourLabels;

	
   DrawCenteredText(pDC,
					labels[hour % 24], 
				    startXUnit,
				    finishXUnit,
                    m_hourColors[hour % m_hourColors.size()], 
					TextColor() );
}



void CGanttDisplayHour::OnDraw( CDC* pDC,
							    XUnit exposedStartXUnit,
							    XUnit exposedFinishXUnit)
{
    for ( time_t leftHour = floor(exposedStartXUnit/3600)*3600; 
        leftHour < exposedFinishXUnit; 
        leftHour += 3600 ) 

        DrawSpecificHour(pDC, leftHour);

}

void CGanttDisplayRelHour::OnDraw( CDC* pDC,
							    XUnit exposedStartXUnit,
							    XUnit exposedFinishXUnit)
{
	long startCount = floor(exposedStartXUnit/3600);
	long finishCount = ceil(exposedFinishXUnit/3600);

    for ( time_t hour = startCount; 
          hour <= finishCount; 
          ++hour ) 

        DrawSpecificHour(pDC, hour);

}




//  The class CGanttDisplayXCursorBar, derived from GanttDisplayBar,
//    is used to draw a little bit of text next to the XCursor line.
//  It defines the exposure callback.

//  Almost identical class CGanttDisplayRelDay/CGanttDisplayRelHour, 
//    also derived from GanttDisplayBar,
//    graphs days/hours offsets (relative times).


CGanttDisplayXCursorBar::CGanttDisplayXCursorBar( Dimension height_, 
												 Dimension offset_, 
												 COLORREF textColor )

	: CGanttDisplayBar(height_,offset_)
{
  SetTextColor( textColor );
}


void CGanttDisplayXCursorBar::OnXCursorChange(XUnit oldVal, XUnit newVal)
{

	CClientDC dc(m_pChart);
	m_pChart->OnPrepareDC(&dc);
	
	// clear out old text.
	ClearToBackground(&dc,m_startXUnit,m_finishXUnit);

	// compute new position, in XUnits
	m_startXUnit = newVal;

	// this is the size in pixels.
	CSize size = dc.GetTextExtent(Text());
	
	// translate size in pixels to delta in XUnits.

	
	double delta = (m_pChart->m_pixelsPerXUnit == 0 
					? 1 
					: size.cx / m_pChart->m_pixelsPerXUnit );

	m_finishXUnit = m_startXUnit + delta;

	// invalidate new position

	CPoint scrollPosn = -m_pChart->GetScrollPosition();
	Position start = XlateXUnitToXCoord(m_startXUnit);
	Position finish = XlateXUnitToXCoord(m_finishXUnit);
	CRect r(start,m_topY,finish,m_bottomY);
	r.OffsetRect(scrollPosn);
	m_pChart->InvalidateRect(&r,true);
	
}



void CGanttDisplayXCursorBar::OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit)
{
	if ( exposedFinishXUnit < m_startXUnit	
		 ||
		 exposedStartXUnit > m_finishXUnit )
		 return;

	DrawCenteredText(pDC,Text(),m_startXUnit,m_finishXUnit,m_bgColor,m_textColor);
}


// This handles the common case where the XUnit is time, specifically, time_t.
CGanttDisplayXCursorTimeBar::CGanttDisplayXCursorTimeBar( Dimension height_, 
														 Dimension offset_,
														 COLORREF textColor )
 :CGanttDisplayXCursorBar(height_,offset_,textColor)
{	
	m_strFormat = "%X %x";
}



CString CGanttDisplayXCursorTimeBar::Text()
{
	CTime t(m_pChart->m_xCursorValue);

	return t.Format(LPCTSTR(m_strFormat));
}





////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGanttChart



IMPLEMENT_DYNCREATE(CGanttChart, CScrollView)

CGanttChart::CGanttChart()
{
	m_pGanttDisplay = 0;

	m_pFont = 0;

	m_pCaptureBar	= 0;

	m_pixelsPerXUnit = 1.0;
	
	m_topMargin		= 0;
	m_bottomMargin	= 0;
	m_leftMargin	= 0;
	m_rightMargin	= 0;
	
	m_areaHeight	= 0;
	m_areaWidth		= 0;

	m_xCursorColor = RGB(240,0,200);
	m_xCursorValue = 0.0;
}



CGanttChart::~CGanttChart()
{
	for ( vector<CGanttDisplayBar*>::iterator ib = m_bars.begin();
		  ib != m_bars.end();
		  ++ib )
		delete (*ib);

	m_bars.erase(m_bars.begin(),m_bars.end());

	delete m_pFont;
}




BEGIN_MESSAGE_MAP(CGanttChart, CScrollView)
	//{{AFX_MSG_MAP(CGanttChart)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttChart drawing



void CGanttChart::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	m_pFont = new CFont;
	m_pFont->CreatePointFont( 70, "New Century Schoolbook" );
 
	SetFont(m_pFont, false);
         
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}



/////////////////////////////////////////////////////////////////////////////
// CGanttChart diagnostics

#ifdef _DEBUG
void CGanttChart::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGanttChart::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG







/////////////////////////////////////////////////////////////////////////////
// CGanttChart message handlers



////////////////////////////////////////////////////////////////////////
//
//  GanttChart -- managing the GanttDisplayBars
//
////////////////////////////////////////////////////////////////////////


void CGanttChart::AddBars(vector<CGanttDisplayBar*>& newBars)
{
    //  We could probably do this more efficiently, but who cares
    for ( vector<CGanttDisplayBar*>::iterator ib = newBars.begin();
        ib != newBars.end();
	    ++ib ) {
        AddBar((*ib));
    }
}
    


void CGanttChart::AddBar( CGanttDisplayBar* pBar )
{
    if ( m_bars.size() == 0 ) {
        // this is the first bar
        pBar->m_topY = m_topMargin;
    }
    else {
        //  Offset from prior bar
        CGanttDisplayBar* pPrevBar = m_bars.back();
        pBar->m_topY = pPrevBar->m_bottomY + pBar->m_offsetFromPreviousBar;
    }

    pBar->m_bottomY = pBar->m_topY + pBar->m_height;
    pBar->m_pChart = this;

    m_bars.push_back(pBar);

    m_areaHeight = pBar->m_bottomY + m_bottomMargin;

#if 0
    afxDump << "GanttDisplay::AddBar #" << m_bars.size()
			<< ": topY = " << pBar->m_topY
			<< ", botY = " << pBar->m_bottomY
			<< ", set areaHeight = " << m_areaHeight << "\n";
#endif

	if ( m_pGanttDisplay != 0 )
		m_pGanttDisplay->CreateBarLabels(pBar);

}


  


void CGanttChart::RemoveAllBars()
{
    for ( vector<CGanttDisplayBar*>::iterator ib = m_bars.begin();
          ib != m_bars.end();
          ++ib ) {
        (*ib)->m_pChart      = 0;
    }

	release(m_bars.begin(),m_bars.end());
    m_bars.erase(m_bars.begin(),m_bars.end());
    m_areaHeight = m_topMargin + m_bottomMargin;
}



//  Something involving layout has changed in some bar
//  Recalculate layout related parameters.
//  We cheat shamelessly here, holding a temporary vector,
//    releasing the existing bar-related info,
//    and then adding the bars again.

void CGanttChart::RecalculateBars()
{
  vector<CGanttDisplayBar*> tempBars(m_bars);
  RemoveAllBars();
  AddBars(tempBars);
}








////////////////////////////////////////////////////////////////////////
//
//  CGanttChart -- sizing the display
//
////////////////////////////////////////////////////////////////////////



// If we know the values of
//			m_pixelsPerHour
//			m_startXUnit
//			m_finishXUnit
//          the margins
//		then we know how large to make the graph.
//  (This only sets m_areaWidth.  m_areaHeight is controlled by the bars.)

void CGanttChart::CalculateWidth()
{
	m_areaWidth 
		= min( Dimension(INT_MAX),
	           Dimension(ceil(m_leftMargin 
		                      + m_rightMargin 
		                      + m_pixelsPerXUnit*(m_finishXUnit - m_startXUnit))));
}



//  If height and width of have been calculated,
//   we can set the dimensions of the drawing area 
//   and position all labels.

void CGanttChart::SetDimensions()
{
    SetScrollSizes( MM_TEXT, CSize(m_areaWidth,m_areaHeight) );
 
#if 0
  afxDump << "GanttDisplay:: Resizing to " << m_areaWidth 
       << " x " << m_areaHeight
       << "\n";
#endif

}



//  Set the start and finish times and resize the drawing area widget.

void CGanttChart::SetXLimits( XUnit startXUnit_,
			                   XUnit finishXUnit_ )
{
  m_startXUnit  = startXUnit_;
  m_finishXUnit = finishXUnit_;
  
  //SetPixelsPerHourLimits();
  CalculateWidth();
  SetDimensions();

}

void CGanttDisplay::SetXLimits( XUnit start, XUnit finish)
{
	m_pChart->SetXLimits(start,finish); 
	m_pLabelView->SetHeight(m_pChart->m_areaHeight);
}


// Given a new value for pixelsPerXUnit, resize the display.

void CGanttChart::SetPixelsPerXUnit(double pixelsPerXUnit_)
{
  m_pixelsPerXUnit = pixelsPerXUnit_;
  
  // Cheat, and let this truncate the ppx value to be within the limits.
  // This also recalculates the min/max values, but who cares?

  //SetPixelsPerHourLimits();
  CalculateWidth();
  SetDimensions();
}

void CGanttDisplay::SetPixelsPerXUnit(double value)
{ 
	m_pChart->SetPixelsPerXUnit(value); 
	m_pLabelView->SetHeight(m_pChart->m_areaHeight);
}



// rather obviously, I did not design this to use after adding bars.
// use this when first creating the gantt chart.
void CGanttChart::SetMargins(Dimension topMargin_,
							   Dimension leftMargin_,
							   Dimension bottomMargin_,
							   Dimension rightMargin_)
{
    m_topMargin		= topMargin_;
    m_leftMargin	= leftMargin_;
    m_bottomMargin	= bottomMargin_;
    m_rightMargin	= rightMargin_;
}




////////////////////////////////////////////////////////////////////////
//
//  CGanttChart -- translations
//
////////////////////////////////////////////////////////////////////////


//  Given an X coordinate in user units (XUnit),
//     determine the X coordinate in pixels it maps to.
//  The time is truncated to lie between the given start and finish limits.
//  This is an absolute position, i.e., not adjusted for margins.

Position CGanttChart::XlateXUnitToXCoord( XUnit xUnit)
{
	xUnit = max(xUnit,m_startXUnit);
	xUnit = min(xUnit,m_finishXUnit);
	
	double offset = xUnit - m_startXUnit;
	
	
	// Convert to hours, then to pixels, and round the value.
	
	Position relativePosn = Position( offset*m_pixelsPerXUnit + 0.5 );
	
	Position posn = m_leftMargin  + relativePosn;
	
	return posn;
}




//  Given an X coordinate in user units (XUnit),
//     determine the X coordinate in pixels it maps to.
//  There is not truncation here.
//  This is an absolute position, i.e., not adjusted for margins.

Position CGanttChart::XlateXUnitToXCoordNoTruncate( XUnit xUnit)
{
   double offset = xUnit - m_startXUnit;


  // Convert to hours, then to pixels, and round the value.

  Position relativePosn = Position( offset*m_pixelsPerXUnit + 0.5 );

  Position posn = m_leftMargin  + relativePosn;

  return posn;
}


// Given an absolute X coord on the drawing area, 
//   convert to the corresponding user unit (XUnit).
// Truncates to lie in between start and finish limits.

XUnit CGanttChart::XlateXCoordToXUnit( Position xCoord )
{
  Position relativeX = xCoord - m_rightMargin;

  double offset = relativeX / m_pixelsPerXUnit;
 
  XUnit result = m_startXUnit + offset;
  
  result = max(result,m_startXUnit);
  result = min(result,m_finishXUnit);

  return result ;
}


// Given an absolute X coord on the drawing area, 
//   convert to the corresponding user unit (XUnit).

XUnit CGanttChart::XlateXCoordToXUnitNoTruncate( Position xCoord )
{
  Position relativeX = xCoord - m_rightMargin;

  double offset = relativeX / m_pixelsPerXUnit;
 
  XUnit result = m_startXUnit + offset;
  
  return result ;
}


//  Determine which bar the y-coord is closest to.
//  If the Y coord lies between the topY and bottomY of a bar, 
//    then that is the group.  
//  Else for any given group, use the distance to the closer 
//    of the topY and bottomY, and pick the group that minimizes this value.
//
//  One override computes the following Indirect returns:
//    isInside -- the y-coord is between the topY and bottomY
//    isAbove  -- the y-coord is closer to topY than bottomY
//               (thus if isInside is false, 
//                  tells us if we are above or below).

CGanttDisplayBar* CGanttChart::XlateYCoordToBar( Position yCoord )
{
	bool isInside_dummy;
	bool isAbove_dummy;

	return XlateYCoordToBar(yCoord,isInside_dummy,isAbove_dummy);
}


CGanttDisplayBar* CGanttChart::XlateYCoordToBar( Position yCoord,
						                         bool& isInside,
						                         bool& isAbove )
{
	CGanttDisplayBar* pBestBar     = 0;
	long              bestDistance = LONG_MAX;
	
	isInside = false;
	isAbove  = false;
	
	
	for ( vector<CGanttDisplayBar*>::iterator ib = m_bars.begin();
		  ib != m_bars.end();
		  ++ib ) {

		CGanttDisplayBar* pBar = (*ib);
		
		long topDistance = abs(yCoord - pBar->m_topY);
		long botDistance = abs(yCoord - pBar->m_bottomY);
		
		long distance;
		bool thisIsAbove;
		
		if ( topDistance < botDistance ) {
			distance    = topDistance;
			thisIsAbove = true;
		}
		else {
			distance    = botDistance;
			thisIsAbove = false;
		}
		
		if ( pBar->m_topY < yCoord && yCoord < pBar->m_bottomY ) {
			pBestBar     = pBar;
			bestDistance = distance;
			isInside     = true;
			isAbove      = thisIsAbove;
			break;
		}
		else if ( distance < bestDistance ) {
			pBestBar     = pBar;
			bestDistance = distance;
			isAbove      = thisIsAbove;
		}
	}
	
	return pBestBar;
}





////////////////////////////////////////////////////////////////////////
//
//  CGanttChart -- drawing
//
////////////////////////////////////////////////////////////////////////

// Given the coordinates (time and Ycoords) and a color,
//   draw a border rectangle.

void CGanttChart::DrawRectBorder(CDC*		pDC,
									  COLORREF	color,
									  XUnit		leftXUnit,
									  XUnit		rightXUnit,
				                      Position	topY,
				                      Position	bottomY)
{
    Position leftX  = XlateXUnitToXCoord(leftXUnit);
    Position rightX = XlateXUnitToXCoord(rightXUnit)+1; 

	CBrush newBrush, *oldBrush;

	newBrush.CreateSolidBrush( color );
	oldBrush = pDC->SelectObject( &newBrush );

    pDC->FrameRect( CRect(leftX,topY,rightX,bottomY), &newBrush );

	pDC->SelectObject( oldBrush );
}


// Given the coordinates (time and Ycoords) and a color, 
//   draw a filled rectangle.

void CGanttChart::DrawBorderedRect(CDC*		pDC,
								COLORREF	color,
								XUnit		leftXUnit,
								XUnit		rightXUnit,
								Position	topY,
								Position	bottomY)
{
    Position leftX  = XlateXUnitToXCoord(leftXUnit);
    Position rightX = XlateXUnitToXCoord(rightXUnit)+1; 

	CBrush newBrush, *oldBrush;

	newBrush.CreateSolidBrush( color );
	oldBrush = pDC->SelectObject( &newBrush );

    pDC->Rectangle( leftX, topY, rightX, bottomY );
                    
	pDC->SelectObject( oldBrush );
}


void CGanttChart::DrawRect(CDC*		pDC,
						   COLORREF	color,
						   XUnit		leftXUnit,
						   XUnit		rightXUnit,
						   Position	topY,
						   Position	bottomY)
{
    Position leftX  = XlateXUnitToXCoord(leftXUnit);
    Position rightX = XlateXUnitToXCoord(rightXUnit)+1; 

	CBrush newBrush, *oldBrush;

	newBrush.CreateSolidBrush( color );
	oldBrush = pDC->SelectObject( &newBrush );

	CGdiObject* pOldPen = pDC->SelectStockObject(NULL_PEN);

 
    pDC->Rectangle( leftX, topY, rightX, bottomY );
                    
	pDC->SelectObject( oldBrush );
	pDC->SelectObject( pOldPen  );
}

//  Given the user X coordinates and a color, draw a horizontal line.

void CGanttChart::DrawHorizLine(CDC*		pDC,
								COLORREF	color,
								XUnit		leftXUnit,
								XUnit		rightXUnit,
								Position	yCoord,
								unsigned	lineWidth)
{
    Position leftX  = XlateXUnitToXCoord(leftXUnit);
    Position rightX = XlateXUnitToXCoord(rightXUnit)+1; 
 
    CPen newPen( PS_SOLID, lineWidth, color ), *oldPen;

    oldPen = pDC->SelectObject( &newPen );
 
	pDC->MoveTo( leftX,  yCoord );
    pDC->LineTo( rightX, yCoord );
    
    pDC->SelectObject( oldPen );
}



//  Given user X coordinates and a color, draw a vertical line.

void CGanttChart::DrawVertLine( CDC*		pDC,
								  COLORREF	color,
								  XUnit		xUnit,
				                  Position	topY,
				                  Position	bottomY,
				                  unsigned	lineWidth )
{
	Position xCoord  = XlateXUnitToXCoord(xUnit);

    CPen newPen( PS_SOLID, lineWidth, color ), *oldPen;

    oldPen = pDC->SelectObject( &newPen );
 
	pDC->MoveTo( xCoord, topY );
    pDC->LineTo( xCoord, bottomY );
    
    pDC->SelectObject( oldPen );
 }



void CGanttChart::DrawVertLine( CDC*		pDC,
								  COLORREF	color,
								  XUnit		xUnit,
				                  Position	topY,
				                  Position	bottomY,
				                  unsigned	lineWidth,
				                  int		horizShift)
{
	Position xCoord  = XlateXUnitToXCoord(xUnit);

    CPen newPen( PS_SOLID, lineWidth, color ), *oldPen;

    oldPen = pDC->SelectObject( &newPen );
 
	pDC->MoveTo( xCoord+horizShift, topY );
    pDC->LineTo( xCoord+horizShift, bottomY );
    
    pDC->SelectObject( oldPen );
}



void CGanttChart::DrawVertBar(  CDC*		pDC,
								  COLORREF	color,
								  XUnit		xUnit,
								  int		width,
				                  Position	topY,
				                  Position	bottomY,
				                  unsigned	lineWidth)
{
	Position leftX  = XlateXUnitToXCoord(xUnit);
	Position rightX = leftX+width;
 
	CBrush newBrush, *oldBrush;

	newBrush.CreateSolidBrush( color );
	oldBrush = pDC->SelectObject( &newBrush );
 
	pDC->Rectangle( leftX, topY, rightX, bottomY );

	pDC->SelectObject( oldBrush );
}


//  Draw text centered in a box of given corners. 

void CGanttChart::DrawCenteredText(CDC*		pDC,
								   const CString&  string,
								   XUnit	leftXUnit,
								   XUnit	rightXUnit,
								   Position topY,
								   Position bottomY,
								   COLORREF bkColor,
								   COLORREF textColor )
{
    Position leftX  = XlateXUnitToXCoord(leftXUnit);
    Position rightX = XlateXUnitToXCoord(rightXUnit)+1; 

    CFont* pOldFont;

    pOldFont = pDC->SelectObject( m_pFont );

    pDC->SetTextColor( textColor );
	//pDC->SetTextColor( GetColorRef(White) );
    pDC->SetBkMode( TRANSPARENT );
    //pDC->SetBkColor( bkColor ); get rid of the passed bkColor
    pDC->DrawText(string, 
				  CRect(leftX,topY,rightX,bottomY),
                  DT_CENTER );

    pDC->SelectObject( pOldFont );
}







// The CGanttChart will overlay a vertical line at a particular
//   user X coordinate.  Because this is usually called during
//   an exposure event, we pass the exposure time endpoints.

void CGanttChart::DrawXCursorLine( CDC* pDC,
									 XUnit exposedStartXUnit,
				                     XUnit exposedFinishXUnit )
{

  if ( exposedStartXUnit <= m_xCursorValue
       && 
       m_xCursorValue < exposedFinishXUnit ) {

    DrawVertLine( pDC,
				  m_xCursorColor,
		          m_xCursorValue,
		          0,
		          m_areaHeight,
				  2);
  }
}



//  This method may be used by clients to set the user X cursor.
//  Setting the value outside the start and finish XUnit values
//    of the displayed area effectively disables it.

void CGanttChart::SetXCursorValue( XUnit newVal )
{
	if ( m_xCursorValue == newVal )
		return;
	
	
	XUnit oldVal = m_xCursorValue;
	m_xCursorValue = newVal; // important to set before we do this.

	Position oldX = XlateXUnitToXCoord(oldVal);
	Position newX = XlateXUnitToXCoord(newVal);

	bool oldIsValid =  m_startXUnit <= oldVal && oldVal <= m_finishXUnit;
	bool newIsValid =  m_startXUnit <= newVal && newVal <= m_finishXUnit;

	// If the existing value is valid, invalidate a rectangle around the old position.

	CPoint scrollPosn = -GetScrollPosition();

	if ( oldIsValid ) {

		CRect r(oldX-5,0,oldX+5,m_areaHeight);
		r.OffsetRect(scrollPosn);
		InvalidateRect(&r,true);

	}

	// invalidate a rectangle around the new position,
	//    unless the new position is out of bounds
	//        or the invalidation on the old position did the trick

	if ( newIsValid && ( ! oldIsValid || newX < oldX-5 || newX >= oldX+5 ) ) {

		CRect r(newX-5,0,newX+5,m_areaHeight);
		r.OffsetRect(scrollPosn);
		InvalidateRect(&r,true);

	}

	for ( vector<CGanttDisplayBar*>::iterator ib = m_bars.begin();
		  ib != m_bars.end();
		  ++ib ) 
		(*ib)->OnXCursorChange(oldVal,newVal);

}


// When an exposure occurs, call a virtual method on each bar in the display.
// The exposure may defer!


void CGanttChart::OnDraw(CDC* pDC)
{
	CRect rect;
	
	// use client rect if clip region is null -- I don't remember why.
	if ( pDC->GetClipBox(&rect) == NULLREGION ) 
		GetClientRect(&rect);
	
	
	vector<CGanttDisplayBar*> exposedBars;
	
	XUnit exposedStartXUnit  = XlateXCoordToXUnit( rect.TopLeft().x );
	XUnit exposedFinishXUnit = XlateXCoordToXUnit( rect.BottomRight().x );
	
	DrawXCursorLine( pDC, exposedStartXUnit, exposedFinishXUnit );
	
	Position topY    = rect.TopLeft().y;
	Position bottomY = rect.BottomRight().y;
	
	//  Find unique set of bars to draw
	//  a bar may be nominated by another bar via refer-to,
	//  so we check to list each bar only once.

	for ( vector<CGanttDisplayBar*>::iterator ib = m_bars.begin();
		  ib != m_bars.end();
		  ++ib) {
		
		if ( (*ib)->m_bottomY < topY || (*ib)->m_topY > bottomY )
			continue;
		
		CGanttDisplayBar* pBarToUse  = ( (*ib)->m_pReferToBar == 0 
										 ? (*ib)
										 : (*ib)->m_pReferToBar );
		
		if ( find(exposedBars.begin(),
				  exposedBars.end(),
				  pBarToUse)  ==  exposedBars.end() )
			exposedBars.push_back(pBarToUse);
	}
	
	
	//  Draw on the selected bars

	for ( vector<CGanttDisplayBar*>::iterator ib1 = exposedBars.begin();
		  ib1 != exposedBars.end();
		  ++ib1) {
		
		(*ib1)->OnDraw(pDC,exposedStartXUnit,exposedFinishXUnit);
	}
}



void CGanttChart::GetViewedRect(CRect& rRect)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CPoint pt = GetDeviceScrollPosition();
	//clientRect += pt;

	rRect = clientRect + pt;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Input event handling
//
////////////////////////////////////////////////////////////////////////////////


// We handle mouse clicks with LButtonDown, MouseMove and LButtonUp separately.
//
//	The events are referred to handlers on the bar on which the click occurs.
//  The bar's OnLButtonDown handler returns a boolean value indicating if
//     a mouse capture should be done (so it can track movement).
//  If a mouse capture is done, we record the bar that had the LButtonDown
//		so that we can direct the subsequent MouseMove and LButtonUp events
//		to it.

void CGanttChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	bool isInside;
	bool isAbove;
	
	  CPoint scrollPosn = GetScrollPosition();
	  point.Offset(scrollPosn);

    CGanttDisplayBar* pBar = XlateYCoordToBar( point.y, isInside, isAbove );

    if ( pBar != 0 ) {


	  SetCapture();
	  m_pCaptureBar = pBar;

      CGanttDisplayBar* pNotifyBar = (pBar->m_pReferToBar == 0 
									  ? pBar 
									  : pBar->m_pReferToBar );
 
	  if ( ! pNotifyBar->OnLButtonDown( XlateXCoordToXUnit(point.x),
				 					    pBar->XlateYCoordToValue(point.y),								 
										isInside,
										isAbove,
										pBar,
										point,
										nFlags) ) {
		  ReleaseCapture();
		  m_pCaptureBar = 0;
	  }
    }
}



void CGanttChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( GetCapture() != this )
		return;
	
	assert( m_pCaptureBar != 0 );

    CGanttDisplayBar* pNotifyBar = (m_pCaptureBar->m_pReferToBar == 0 
									  ? m_pCaptureBar 
									  : m_pCaptureBar->m_pReferToBar );
 

	  CPoint scrollPosn = GetScrollPosition();
	  point.Offset(scrollPosn);

	pNotifyBar->OnMouseMove( XlateXCoordToXUnitNoTruncate(point.x),
							 m_pCaptureBar->XlateYCoordToValue(point.y),								 
							 m_pCaptureBar,
							 point,
							 nFlags );
}

	


void CGanttChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( GetCapture() != this )
		return;

	ReleaseCapture();

	assert( m_pCaptureBar != 0 );

    CGanttDisplayBar* pNotifyBar = (m_pCaptureBar->m_pReferToBar == 0 
									  ? m_pCaptureBar 
									  : m_pCaptureBar->m_pReferToBar );
 

	  CPoint scrollPosn = GetScrollPosition();
	  point.Offset(scrollPosn);

	pNotifyBar->OnLButtonUp( XlateXCoordToXUnitNoTruncate(point.x),
							 m_pCaptureBar->XlateYCoordToValue(point.y),								 
							 m_pCaptureBar,
							 point,
							 nFlags );

	m_pCaptureBar = 0;
}




void CGanttChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	bool isInside;
	bool isAbove;
	

	  CPoint scrollPosn = GetScrollPosition();
	  point.Offset(scrollPosn);

    CGanttDisplayBar* pBar = XlateYCoordToBar( point.y, isInside, isAbove );

    if ( pBar != 0 ) {

      CGanttDisplayBar* pNotifyBar = (pBar->m_pReferToBar == 0 
									  ? pBar 
									  : pBar->m_pReferToBar );
	  pNotifyBar->OnRButtonDown( XlateXCoordToXUnit(point.x),
				 			     pBar->XlateYCoordToValue(point.y),								 
								 isInside,
								 isAbove,
								 pBar,
								 point,
								 nFlags);
    }
}



//////////////////////////////////////////////
//
//  Scrolling
//
//////////////////////////////////////////////


void CGanttChart::Scroll(XUnit delta, XUnit boundsDelta)
{
	CPoint pt = GetScrollPosition();

	XUnit xunit = XlateXCoordToXUnitNoTruncate(pt.x);
	XUnit newxunit = xunit + delta;


	if ( newxunit < m_startXUnit ) {
		SetXLimits(m_startXUnit-boundsDelta,
				   m_finishXUnit);
	}
	else {
		CRect rect;
		GetClientRect(&rect);
		XUnit rightXUnit = XlateXCoordToXUnitNoTruncate(pt.x + rect.Width())+delta;
		if ( rightXUnit > m_finishXUnit ) {
			SetXLimits(m_startXUnit,
					   m_finishXUnit + boundsDelta);
		}
	}

	Position newXCoord = XlateXUnitToXCoordNoTruncate(newxunit);
	ScrollToPosition(CPoint(newXCoord,pt.y));

	UpdateWindow();
}


/////////////////////////////////////////////////////////////////////////////
//
//	Zooming
//
/////////////////////////////////////////////////////////////////////////////

// TODO: Place limits on how far we can zoom in or out.

void CGanttChart::ZoomIn()
{
	SetPixelsPerXUnit(m_pixelsPerXUnit * 2);
	Invalidate();
}

void CGanttChart::ZoomOut()
{
	SetPixelsPerXUnit(m_pixelsPerXUnit / 2);
	Invalidate();
}


void CGanttChart::ZoomFit()
{
	CRect rect;
	GetViewedRect(rect);
	rect.InflateRect(-m_leftMargin,0,-m_rightMargin,0);
	if ( rect.IsRectEmpty() ) 
		rect = CRect(100,100,100,100);
	
	XUnit delta = m_finishXUnit  - m_startXUnit;

	if ( delta == 0 )
		return;

	SetPixelsPerXUnit( rect.Width() / delta );
	Invalidate();
}




/////////////////////////////////////////////////////////////////////////////
// CGanttLabelView

IMPLEMENT_DYNCREATE(CGanttLabelView, CScrollView)

CGanttLabelView::CGanttLabelView()
{
	m_pGanttDisplay = 0;
}

CGanttLabelView::~CGanttLabelView()
{
}


BEGIN_MESSAGE_MAP(CGanttLabelView, CScrollView)
	//{{AFX_MSG_MAP(CGanttLabelView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

	ON_WM_CTLCOLOR() // have to do this myself

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttLabelView drawing

void CGanttLabelView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGanttLabelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CGanttLabelView::SetHeight(Dimension height)
{	
	CRect rect;
	GetClientRect(&rect);
	CSize sizeTotal(rect.Width(),height);
	SetScrollSizes(MM_TEXT,sizeTotal);
}


/////////////////////////////////////////////////////////////////////////////
// CGanttLabelView diagnostics

#ifdef _DEBUG
void CGanttLabelView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGanttLabelView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGanttLabelView message handlers

void CGanttChart::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	SetDimensions();
}

void CGanttLabelView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	PositionLabels();	
}


HBRUSH CGanttLabelView::OnCtlColor( CDC* pDC, CWnd*, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_BTN
		||
		nCtlColor == CTLCOLOR_STATIC )
		pDC->SetBkMode(TRANSPARENT);

	return HBRUSH(::GetCurrentObject(pDC->GetSafeHdc(), OBJ_BRUSH));
}


void CGanttLabelView::PositionLabels()
{
	if ( m_pGanttDisplay == 0 ) return;

	vector<CGanttDisplayBar*> bars = m_pGanttDisplay->GetBars();

	for ( vector<CGanttDisplayBar*>::iterator ib = bars.begin();
		  ib != bars.end();
		  ++ib )
		
		(*ib)->PositionLabels();
}


BOOL CGanttChart::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (WS_VSCROLL | WS_HSCROLL);
	return CScrollView::PreCreateWindow(cs);
}

BOOL CGanttLabelView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (WS_VSCROLL | WS_HSCROLL);
	return CScrollView::PreCreateWindow(cs);
}
