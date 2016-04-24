// GanttDisplay.h: interface for the CGanttDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTDISPLAY_H__609ECDB5_B510_11D0_9767_0000C0C2484C__INCLUDED_)
#define AFX_GANTTDISPLAY_H__609ECDB5_B510_11D0_9767_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
 

// forward declarations

class CGanttDisplayBar;
class CGanttDisplay;


// types Position and Dimension are leftovers from
// the version of this code that ran under X Windows.

typedef int Position;
typedef int Dimension;


//  Though most often the X-axis is time, we define the
//  X-axis to be units of XUnit.

typedef double XUnit;



/////////////////////////////////////////////////////////////////////////////
// CGanttChart view

// A CGanttChart is the drawing area portion of the CGanttDisplay.
// Much of the real functionality is attached to this class.


class CGanttChart : public CScrollView
{
protected:
	DECLARE_DYNCREATE(CGanttChart)

// Attributes
public:

	CGanttChart();

	CGanttDisplay* m_pGanttDisplay;
		// The CGanttDisplay which contains this CGanttChart, if there is one.

	CGanttDisplayBar* m_pCaptureBar;
		// Used in mouse tracking, this is the bar which initiated the capture
		// on the mouse.

 
	COLORREF	m_xCursorColor;
	XUnit		m_xCursorValue;
		//	The chart supports a vertical line called the X Cursor.
		//  These variables store the color and position where it should be drawn.

    XUnit		m_startXUnit;
    XUnit		m_finishXUnit;
		//	The values delineating the display area.

	XUnit		m_pixelsPerXUnit;
		// Maps XUnits to pixels.  Zooming is implemented by modifying this value.


	Dimension	m_topMargin;
	Dimension	m_bottomMargin;
	Dimension	m_leftMargin;
	Dimension	m_rightMargin;
		// The margin between the border and drawn are on the chart.

	Dimension   m_areaHeight;
    Dimension   m_areaWidth;
		// Current height and width.

	CFont*		m_pFont;
		// Font being used for character strings.

	vector<CGanttDisplayBar*>	m_bars;
		// The bars registered for display on this chart.



// Operations
public:


	XUnit		GetStartXUnit()  { return m_startXUnit; }
	XUnit		GetFinishXUnit() { return m_finishXUnit; }
	CGanttDisplay* GanttDisplay()
	{	return m_pGanttDisplay;	}
		// accessors

    Position	XlateXUnitToXCoord( XUnit xUnit );
    XUnit		XlateXCoordToXUnit( Position xCoord );
		//  Users do drawing, etc, with respect to XUnits.
		//  We need to know pixels in order to actually do the drawing.
		//  Takes into account margins and m_pixelsPerXUnit

    Position	XlateXUnitToXCoordNoTruncate( XUnit xUnit );
    XUnit		XlateXCoordToXUnitNoTruncate( Position xCoord );
		// The versions of these above (without the 'NoTruncate')
		//  truncate values to lie between m_startXUnit and m_finishXUnit.
		// These do not.


    void AddBar( CGanttDisplayBar* pBar);
    void AddBars( vector<CGanttDisplayBar*>& bars);
    void RemoveAllBars();
    void RecalculateBars();

    
	void CalculateWidth();
	void SetDimensions();
	void SetXLimits(XUnit startXUnit_, XUnit finishXUnit_);
	void SetPixelsPerXUnit(double value);
	void SetMargins(Dimension left,
					Dimension top,
					Dimension right,
					Dimension bottom);

 	CGanttDisplayBar* XlateYCoordToBar( int yCoord ); 
	CGanttDisplayBar* XlateYCoordToBar( int yCoord, 
										bool& isInside, 
										bool& isAbove );

	void DrawRectBorder(CDC*		pDC,
							 COLORREF	color,
							 XUnit		leftXUnit,
							 XUnit		rightXUnit,
				             Position	topY,
				             Position	bottomY);

	void DrawBorderedRect(CDC*		pDC,
					   COLORREF	color,
					   XUnit	leftXUnit,
					   XUnit	rightXUnit,
					   Position	topY,
					   Position	bottomY);

	void DrawRect(CDC*		pDC,
					   COLORREF	color,
					   XUnit	leftXUnit,
					   XUnit	rightXUnit,
					   Position	topY,
					   Position	bottomY);

	void DrawHorizLine(CDC*		pDC,
					   COLORREF	color,
					   XUnit	leftXUnit,
					   XUnit	rightXUnit,
					   Position	yCoord,
					   unsigned	lineWidth);

	void DrawVertLine( CDC*		pDC,
					   COLORREF	color,
					   XUnit	xUnit,
					   Position	topY,
					   Position	bottomY,
					   unsigned	lineWidth);

	void DrawVertLine( CDC*		pDC,
					   COLORREF	color,
					   XUnit	xUnit,
					   Position	topY,
					   Position	bottomY,
					   unsigned	lineWidth,
					   int		horizShift);

	void DrawVertBar(  CDC*		pDC,
					   COLORREF	color,
					   XUnit	xUnit,
					   int		width,
					   Position	topY,
					   Position	bottomY,
					   unsigned	lineWidth);

	void DrawCenteredText(CDC*		pDC,
						  const CString&  string,
						  XUnit		leftXUnit,
						  XUnit		rightXUnit,
						  Position	topY,
						  Position	bottomY,
						  COLORREF	bkColor,
						  COLORREF	textColor);



	void DrawXCursorLine( CDC*	pDC,
						  XUnit exposedStartXUnit,
				          XUnit exposedFinishXUnit );

	void SetXCursorValue( XUnit newVal );
	void SetXCursorColor( COLORREF color ) { m_xCursorColor = color; }


	void GetViewedRect(CRect& rRect);


	void Scroll(XUnit delta, XUnit boundsDelta);


	void ZoomIn();
	void ZoomOut();
	void ZoomFit();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttChart)
	public:
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGanttChart();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGanttChart)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


class CGanttLabelView;



class CGanttDisplay  
{

protected:

	
	CSplitterWnd		m_splitterWnd;
		// the splitter window holding the chart and label view

    CGanttChart*		m_pChart;
		// the chart being displayed.

    CGanttLabelView*	m_pLabelView;
		// the view holding the labesl

     CFont*				m_pLabelFont;
		// the font for labels.


public:

	CGanttDisplay();
	virtual ~CGanttDisplay();

	CGanttChart* Chart() { return m_pChart;	};
 
	bool CGanttDisplay::Create(CWnd* pParentWnd, 
							   CRuntimeClass* pViewClass,
							   SIZE labelSize, 
							   SIZE chartSize, 
							   CCreateContext* pContext,
							   DWORD dwStyle  = WS_CHILD | WS_VISIBLE | WS_VSCROLL , 
							   UINT nID = AFX_IDW_PANE_FIRST);

	void CreateBarLabels( CGanttDisplayBar* );

	CSplitterWnd* GetSplitter() { return &m_splitterWnd; }
 
	// operations that defer to m_pChart

	CGanttDisplayBar* XlateYCoordToBar( int yCoord ) 
	{ return m_pChart->XlateYCoordToBar(yCoord); }

    CGanttDisplayBar* XlateYCoordToBar( int yCoord, bool& isInside, bool& isAbove )
	{ return m_pChart->XlateYCoordToBar(yCoord,isInside,isAbove); }


 	void SetMargins(Dimension top,
					Dimension left,
					Dimension bottom,
					Dimension right)
	{ m_pChart->SetMargins(left,top,bottom,right); }

    void SetXLimits( XUnit start, XUnit finish);


	XUnit		GetStartXUnit()  
	{ return m_pChart->m_startXUnit; }

	XUnit		GetFinishXUnit() 
	{ return m_pChart->m_finishXUnit; }


	void SetPixelsPerXUnit(double value);
  
	void SetXCursorValue( XUnit newVal )
	{ m_pChart->SetXCursorValue(newVal); }

    void SetXCursorColor( COLORREF color )
	{ m_pChart->SetXCursorColor(color); }
  
    void AddBar( CGanttDisplayBar* pBar)
	{ m_pChart->AddBar(pBar);}

    void AddBars( vector<CGanttDisplayBar*>& bars)
	{ m_pChart->AddBars(bars); }

    void RemoveAllBars()
	{ m_pChart->RemoveAllBars();}

    void RecalculateBars()
	{ m_pChart->RecalculateBars(); }

	vector<CGanttDisplayBar*>& GetBars()
	{
		return m_pChart->m_bars;
	}

    void SetDimensions();

	void Scroll(XUnit delta, XUnit boundsDelta)
	{	m_pChart->Scroll(delta,boundsDelta);	}

	void ZoomIn()
	{	m_pChart->ZoomIn();	}

	void ZoomOut()
	{	m_pChart->ZoomOut();	}

	void ZoomFit()
	{	m_pChart->ZoomFit();	}

	
};







class CGanttDisplayBarLabel
{
	////////////////////////////////////////////////////////////////
	//	
	//	Holds information for a label in the label view of a gantt display.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

public:

    CString	m_text;
		// the text to display for the label.

    double	m_value;
		// the value determining vertical location relative to 
		// the associated bar's y-axis coordinates
		//
    bool	m_isButton;
		// Some labels are buttons that allow the user to click

	CWnd* m_pWnd;
		// the window (probably a CStatic or CButton) which will hold 
		// the label

	CGanttDisplayBar* m_pBar;
		// the bar this label is associated with.
		// needed in order to determine the vertical location.

	CWnd* m_pParent;
		// the window (probably a CGanttDisplayLabelView) 
		// in which to create the label
		// the parent window of pWnd.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	// decided not to bother.


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayBarLabel() 
		: m_value(1.0), 
		  m_isButton(false), 
		  m_pWnd(0), 
		  m_pBar(0) 
	{}

    CGanttDisplayBarLabel( const CString& text, 
						   double value, 
						   bool isButton )
		: m_text(text), 
		  m_value(value), 
		  m_isButton(isButton), 
		  m_pWnd(0), 
		  m_pBar(0) 
	{}


    CGanttDisplayBarLabel( const CGanttDisplayBarLabel& x )
		: m_text(x.m_text), 
		  m_value(x.m_value),
		  m_isButton(x.m_isButton), 
		  m_pWnd(0), 
		  m_pBar(0) 
	{}

    ~CGanttDisplayBarLabel() 
	{ 
		if ( m_pWnd != 0 ) { 
			m_pWnd->DestroyWindow(); 
			delete m_pWnd;
		}  
	}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

    //  Need these for vectors

    bool operator==(const CGanttDisplayBarLabel& x ) const 
    {
        return m_text == x.m_text;
    }

    bool operator<(const CGanttDisplayBarLabel& x ) const 
    {
        return m_text < x.m_text;
    }

    CGanttDisplayBarLabel& operator=(const CGanttDisplayBarLabel& x ) 
    {
		if ( this != &x ) 
			m_text = x.m_text;

        return *this;
	}
	

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void CreateWnd(CGanttLabelView* pParent, 
				   CFont* pFont, 
				   CGanttDisplayBar* pBar_);
	void PlaceWnd();


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

};





class CGanttDisplayBar
{
	////////////////////////////////////////////////////////////////
	//	
	//	The base class for bars in a gantt display
	//	Has a position with the gantt display, can have labels 
	//  associated with it, and supports virtual callbacks for
	//	drawing and responding to mouse events.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

    friend class CGanttDisplay;
	friend class CGanttChart;


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    CGanttChart*    m_pChart;
		// the gantt chart this bar is attached to.

    Dimension m_height;
		// vertical size of bar.

    Dimension m_offsetFromPreviousBar;
		// offset (in pixels) from previous bar.

    vector<CGanttDisplayBarLabel> m_labels;
		// the labels associated with this bar

    Position          m_topY;
    Position          m_bottomY;
		// The y-coords of the top and bottom of the bar.
		// we should have m_height = m_bottomY - m_topY.
 
	CGanttDisplayBar* m_pReferToBar;
		// This bar can let another handle its events.
		
	COLORREF m_bgColor;
		// background color for the bar.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	// the data members can be accessed by the friend classes declared above.

public:

    Position TopY()    { return m_topY; }
    Position BottomY() { return m_bottomY; }
    CGanttChart* Chart() { return m_pChart; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayBar( Dimension height, Dimension offset )
		: m_height(height), 
		  m_offsetFromPreviousBar(offset),
		  m_topY(0), 
		  m_bottomY(0), 
		  m_pChart(0), 
		  m_pReferToBar(0),
		  m_bgColor(::GetSysColor(COLOR_BTNFACE))
	{}
	
	
	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	void CreateBarLabels(CGanttLabelView* pParentWnd,
						 CFont* pLabelFont);

	void PositionLabels();

    void AddLabel( const CString&, double=1.0, bool=false );

    virtual void ReferEvents( CGanttDisplayBar* pReferBar ) 
	{ m_pReferToBar = pReferBar; }

	void SetBgColor(COLORREF color) { m_bgColor = color; }

	
    virtual Position XlateValueToYCoord(double value);
	virtual double XlateYCoordToValue(Position pos);

	virtual void ClearToBackground(CDC* pDC, XUnit startXUnit,XUnit finishXUnit);

	virtual void DrawBorderedRect( CDC*		pDC,
								COLORREF	color,
								XUnit		startTime,
								XUnit		finishTime,
 								double		value );
 
	virtual void DrawRectBorder( CDC*		pDC,
									COLORREF	color,
									XUnit		startTime,
									XUnit		finishTime,
 									double		value );
 
	virtual void DrawRect( CDC*		pDC,
								COLORREF	color,
								XUnit		startTime,
								XUnit		finishTime,
 								double		value );

	virtual void DrawRect( CDC*		pDC,
								COLORREF	color,
						        XUnit		startTime,
								XUnit		finishTime,
 								double		value1,
								double		value2);
 
 	
	virtual void DrawBorderedRect( CDC*		pDC,
								COLORREF	color,
						        XUnit		startTime,
								XUnit		finishTime,
 								double		value1,
								double		value2);
 
	virtual void DrawRectBorder( CDC*		pDC,
									COLORREF	color,
							        XUnit		startTime,
									XUnit		finishTime,
 									double		value1,
									double		value2);


	virtual void DrawGraphLine(CDC*		pDC,
							   COLORREF	color,
							   XUnit	startXUnit,
							   XUnit	finishXUnit,
							   double	value,
							   unsigned lineWidth = 2);

	virtual void DrawVertLine(CDC*		pDC,
							  COLORREF	color,
							  XUnit		xUnit,
							  double	minValue = 0.0,
							  double	maxValue = 1.0,
							  unsigned	lineWidth = 1);

    virtual void DrawCenteredText(CDC* pDC,
								  const CString& text,
								  XUnit leftXUnit,
								  XUnit rightXUnit,
								  COLORREF  bkColor,
								  COLORREF  textColor );

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:




	Position	XlateXUnitToXCoord( XUnit xUnit )
	{ return m_pChart->XlateXUnitToXCoord(xUnit); }

    XUnit		XlateXCoordToXUnit( Position xCoord )
	{ return m_pChart->XlateXCoordToXUnit(xCoord); }

    Position	XlateXUnitToXCoordNoTruncate( XUnit xUnit )
 	{ return m_pChart->XlateXUnitToXCoordNoTruncate(xUnit); }

    XUnit		XlateXCoordToXUnitNoTruncate( Position xCoord )
	{ return m_pChart->XlateXCoordToXUnitNoTruncate(xCoord); }

	virtual bool OnLButtonDown(XUnit xUnit,
							   double value,
							   bool isInside,
							   bool isAbove,
							   CGanttDisplayBar* pActualBar,
							   CPoint point,
							   UINT flags)
	{ return false; }

	virtual void OnMouseMove(XUnit xUnit,
							 double value,
							 CGanttDisplayBar* pActualBar,
							   CPoint point,
							   UINT flags)
	{}

	virtual void OnLButtonUp(XUnit xUnit,
							 double value,
							 CGanttDisplayBar* pActualBar,
							   CPoint point,
							   UINT flags)
	{}

	virtual void OnRButtonDown(XUnit xUnit,
							   double value,
							   bool isInside,
							   bool isAbove,
							   CGanttDisplayBar* pActualBar,
							   CPoint point,
							   UINT flags)
	{}

	virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit,
						XUnit exposedFinishXUnit)
	{}

	virtual void OnXCursorChange(XUnit oldVal, XUnit newVal)
	{}
 

    virtual Position XlateValueToDistance( double value=0.0 );

	virtual void ComputeRectYs(double value, Position& rTopY, Position& rBotY);
	virtual void ComputeRectYs(double value1, double value2, Position& rTopY, Position& rBotY);



 
};




class CGanttDisplayGraph : public CGanttDisplayBar
{
	////////////////////////////////////////////////////////////////
	//	
	//	Intended to support drawing simple graphs, such as bar charts.
	//	Pays attention to the 'value' (= y-axis coordinate)
	//    on some of the drawing operations.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////
	
public:

    double m_maxValue;
    double m_minValue;
		// the range of values displayed on the bar

    bool   m_originAtBottom;
		// if true, m_minValue maps to higher y-coords than m_maxValue.
		// if false, the reverse.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	// everything public


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayGraph( Dimension height_, 
						Dimension offset_,
		                double min_, 
						double max_, 
						BOOL bottom_ = true )
                        : CGanttDisplayBar( height_, offset_ ),
                          m_minValue(min_), 
						  m_maxValue(max_), 
						  m_originAtBottom(bottom_) 
	{}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

public:



    virtual Position  XlateValueToDistance( double value );
    virtual Position  XlateValueToYCoord( double value );
 	virtual double XlateYCoordToValue(Position pos);
 
	virtual void ComputeRectYs(double value, Position& rTopY, Position& rBotY);
	virtual void ComputeRectYs(double value1, double value2, Position& rTopY, Position& rBotY);


};




class CGanttDisplayDay: public CGanttDisplayBar 
{

	////////////////////////////////////////////////////////////////
	//	
	//	Specialized to display different colors in the bar on
	//  alternating days, and to display the text of the date.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    vector<COLORREF> m_dayColors;
		// colors to use on alternating days

    COLORREF m_textColor;
		// color to use for date text


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

    void  SetTextColor( COLORREF color ) { m_textColor = color; }
    COLORREF TextColor() const { return m_textColor; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////
public:

    CGanttDisplayDay( Dimension height_, 
					  Dimension offset_,
                      vector<COLORREF>& m_Colors,
					  COLORREF textColor );


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:


    virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit);


    void DrawSpecificDay(CDC* pDC, time_t thisDay);

};



class CGanttDisplayHour: public CGanttDisplayBar 
{
	////////////////////////////////////////////////////////////////
	//	
	//	Specialized to display different colors in the bar on
	//  alternating hours, and to display the text of the hour.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    vector<COLORREF> m_hourColors;
		// colors to use on alternating hours

    COLORREF m_textColor;
		// color to draw hour text

	bool m_bMilitary;
		// use military time convention for hour text

	bool m_b2Digits;
		// use 2 digits for hour
	

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

    void  SetTextColor( COLORREF color ) { m_textColor = color; }
    COLORREF TextColor() { return m_textColor; }
	
	bool& IsMilitary()	{ return m_bMilitary;	}
	bool& Use2Digits()	{ return m_b2Digits;	}

	bool IsMilitary() const	{ return m_bMilitary;	}
	bool Use2Digits() const	{ return m_b2Digits;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayHour( Dimension height_, 
					   Dimension offset_,
					   vector<COLORREF>& m_Colors,
					   COLORREF textColor );

	
	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:

    virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit);

    void DrawSpecificHour(CDC* pDC, time_t hour);
};





class CGanttDisplayRelDay: public CGanttDisplayBar 
{

	////////////////////////////////////////////////////////////////
	//	
	//	Never ended up using this class.  
	//	CGanttDisplayDay used absolute times.
	//  This uses seconds of offset from some unspecified start times.
	//  I.e., time deltas.
	//  Otherwise, similar.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    vector<COLORREF> m_dayColors;
    COLORREF m_textColor;

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

    void  SetTextColor( COLORREF color ) { m_textColor = color; }
    COLORREF TextColor() { return m_textColor; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayRelDay( Dimension height_, 
					     Dimension offset_,
                         vector<COLORREF>& m_Colors,
					     COLORREF textColor );

	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:

    virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit);

    void DrawSpecificDay(CDC* pDC, long count);
};



class CGanttDisplayRelHour: public CGanttDisplayBar 
{
	////////////////////////////////////////////////////////////////
	//	
	//	Similar to CGanttDisplayRelDay and CGanttDisplayHour
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    vector<COLORREF> m_hourColors;
    COLORREF m_textColor;
	bool m_bMilitary;
	bool m_b2Digits;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

    void  SetTextColor( COLORREF color ) { m_textColor = color; }
    COLORREF TextColor() { return m_textColor; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayRelHour( Dimension height_, 
					      Dimension offset_,
					      vector<COLORREF>& m_Colors,
					      COLORREF textColor );


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:

    virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit);

    void DrawSpecificHour(CDC* pDC, long hour);
};




class CGanttDisplayXCursorBar: public CGanttDisplayBar 
{

	////////////////////////////////////////////////////////////////
	//	
	//	A bar to display the current value of the XCursor	
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

    COLORREF m_textColor;
	XUnit m_startXUnit;
	XUnit m_finishXUnit;

	
	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

    void  SetTextColor( COLORREF color ) { m_textColor = color; }
    COLORREF TextColor() { return m_textColor; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayXCursorBar( Dimension height_, 
					  Dimension offset_,
					  COLORREF textColor );


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////


protected:

    virtual void OnDraw(CDC* pDC,
						XUnit exposedStartXUnit, 
						XUnit exposedFinishXUnit);

	virtual void OnXCursorChange(XUnit oldVal, XUnit newVal);

	virtual CString Text() { return "!";}
};


class CGanttDisplayXCursorTimeBar: public CGanttDisplayXCursorBar 
{

	////////////////////////////////////////////////////////////////
	//	
	//	For displaying the value of the XCursor when the x-axis is time.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

protected:

	CString m_strFormat;
		// format string suitable for CTime::Format().
		

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	void SetFormatString(const CString& newVal)
	{	m_strFormat = newVal;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

    CGanttDisplayXCursorTimeBar( Dimension height_, 
					  Dimension offset_,
					  COLORREF textColor );


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

protected:

	virtual CString Text();

};

    
 /////////////////////////////////////////////////////////////////////////////
// CGanttLabelView view

class CGanttLabelView : public CScrollView
{
protected:
	CGanttLabelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGanttLabelView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttLabelView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

public:

	CGanttDisplay* m_pGanttDisplay;
		// the gantt display which parents this view

	void PositionLabels();

	void SetHeight(Dimension height);

	virtual ~CGanttLabelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Class wizard doesn't do this one for me here
	afx_msg HBRUSH OnCtlColor( CDC*, CWnd*, UINT );

	// Generated message map functions
	//{{AFX_MSG(CGanttLabelView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
   

#endif 
// !defined(AFX_GANTTDISPLAY_H__609ECDB5_B510_11D0_9767_0000C0C2484C__INCLUDED_)
