// HMLoadGraphMvc.h: interface for the CHMLoadGraphGanttViewport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMLOADGRAPHMVC_H__D0B67EB3_DDA0_11D4_864A_00104B2D39AC__INCLUDED_)
#define AFX_HMLOADGRAPHMVC_H__D0B67EB3_DDA0_11D4_864A_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHMLoadGraphNumericBar;
class CHMLoadGraphGraphBar;
class CHMLoadGraphCasterBar;
class CHMLoadGraphLoadCountBar;
class CGanttDisplay;
class CGanttDisplayHour;
class CGanttDisplayDay;
class CMessageBus;
class CHMLoadGraphDoc;
class CCastString;

#include "modelTypes.h"
#include "HMStartTimeDlg.h"



////////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphGanttViewport/Controller
//
////////////////////////////////////////////////////////////////////



class CHMLoadGraphGanttViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Viewport managing the display of CHMLoadGraphView
	//  Associated with CHMLoadGraphGanttController
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

	CHMLoadGraphDoc* m_pDoc;

	CGanttDisplay* m_pGanttDisplay;
		// The view is a gantt display, which is how we want to know it.

	CGanttDisplayHour*			m_pHourBar;
	CGanttDisplayDay*			m_pDayBar;
	CHMLoadGraphNumericBar*		m_pNumBars[5];  // caster = 1,2,3, demand = 0, supply = 5
	CHMLoadGraphGraphBar*		m_pGraphBar;
	CHMLoadGraphCasterBar*		m_pCasterBars[4];  // caster = 1,2,3
	CHMLoadGraphLoadCountBar*	m_pLoadBar;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:


	CGanttDisplay* GanttDisplay() 
	{	return m_pGanttDisplay;	}

	void SetGanttDisplay(CGanttDisplay* pd)
	{	
		assert ( m_pGanttDisplay == 0 );
		m_pGanttDisplay = pd;	
	}

	CHMLoadGraphDoc* GetDocument() { return m_pDoc; }
	void SetDocument(CHMLoadGraphDoc* pDoc) { m_pDoc = pDoc; }

	COLORREF CasterColor(int caster);
	COLORREF HeatColor(int caster, int heatIndex);
	COLORREF DemandColor();
	COLORREF SupplyColor();


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CHMLoadGraphGanttViewport()  : m_pGanttDisplay(0),m_pDoc(0)
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


	CMessageBus* GetMyModel();
	virtual BOOL CreateController();
	virtual void OnInitialUpdate();
	virtual BOOL OnCreate();
	void OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg);

	virtual void Draw(CDC* pDC);

	void ScrollTime(long seconds);


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void ComputeLimits();

	static void DrawCenteredText(CDC* pDC, 
								CString& text,
								CRect& boundRect);
	static void DrawCenteredText(CDC* pDC, 
								 vector<CString>& strings, 
								 CRect& boundRect);


};


class CHMLoadGraphGanttController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CHMLoadGraphGanttView/CHMLoadGraphGanttViewport
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

	CMessageBus* m_pMru;
		// the model


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetMyModel();

	CHMLoadGraphGanttViewport* GetViewport()
	{	return dynamic_cast<CHMLoadGraphGanttViewport*>(MvcController::GetViewport());	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:


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


	// Generated message map functions

	//{{AFX_MSG(CHMLoadGraphGanttController)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



////////////////////////////////////////////////////////////////
//
//  CHMLoadGraphScenSeqViewport/Controller
//
////////////////////////////////////////////////////////////////


class CHMLoadGraphScenSeqViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Viewport managing the display of CHMLoadGraphScenSeqView
	//  Associated with CHMLoadGraphScenSeqController
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

private:

	CHMLoadGraphDoc* m_pDoc;

	int m_caster;
	SECListBoxEditor* m_pLbeStrings;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	
	int Caster() const
	{	
		return m_caster;
	}

	void SetCaster(int caster)
	{
		assert ( caster == 1 || caster == 2 || caster == 3 );
		m_caster = caster;
	}

	void SetLbeStrings(SECListBoxEditor* pWnd) { m_pLbeStrings = pWnd; }

	CHMLoadGraphDoc* GetDocument() { return m_pDoc; }
	void SetDocument(CHMLoadGraphDoc* pDoc) { m_pDoc = pDoc; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CHMLoadGraphScenSeqViewport()  : m_pDoc(0)
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


	CMessageBus* GetMyModel();
	virtual BOOL CreateController();
	virtual void OnInitialUpdate();
	virtual BOOL OnCreate();
	void OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg);

	void OnButtonResetStringSeq();
	void OnButtonUpdateStringSeq();



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void InitStringList();

};



class CHMLoadGraphScenSeqController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CHMLoadGraphScenSeqView/CHMLoadGraphScenSeqViewport
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

	CMessageBus* m_pMru;
		// the model


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetMyModel();

	CHMLoadGraphScenSeqViewport* GetViewport()
	{	return dynamic_cast<CHMLoadGraphScenSeqViewport*>(MvcController::GetViewport());	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:


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


	// Generated message map functions

	//{{AFX_MSG(CHMLoadGraphScenSeqController)
	afx_msg void OnButtonResetStringSeq();
	afx_msg void OnButtonUpdateStringSeq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





////////////////////////////////////////////////////////////
//
//  CHMLoadGraphSheetViewport/Controller
//
////////////////////////////////////////////////////////////

class CHMLoadGraphSheetViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Viewport managing the display of CHMLoadGraphSheetView
	//  Associated with CHMLoadGraphSheetController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_HT { HT_OUTSIDE,
					   HT_INSIDE,
					   HT_DATE,
					   HT_HM,
					   HT_HOURS,
					   HT_CASTER,		
					   HT_HOLD_AREA,
					   HT_LOAD,
					};



	// Construct one of these on the stack to set up drawing for a CDayArea or a CHoldArea

	class CDCState 
	{
		friend class CDayArea;

	public:
		CDC* m_pDC;
		CFont* m_pOldFont;
		int m_oldMode;

		CDCState(CDC* pDC);
		~CDCState();
	};


	//  Holds information about the placement of a CastString in a CDayArea or a CHoldArea

	class CCastStringData 
	{

	public:

		CCastString* m_pString;
		CRect m_rect;
		COLORREF m_backColor;
		vector<CString> m_vecText;

		CCastStringData() : m_pString(0) {}
		CCastStringData(CCastString* pString,
						const CRect& rect,
						COLORREF color,
						const vector<CString>& vecText)
						: m_pString(pString),
						  m_rect(rect),
						  m_backColor(color),
						  m_vecText(vecText)
		{}
		CCastStringData(CCastString* pString,
						const CRect& rect,
						COLORREF color)
						: m_pString(pString),
						  m_rect(rect),
						  m_backColor(color)
		{}

		void Draw(CDC* pDC, bool isSelected) const;

	};


	// This class encapsulates information about where a mouse click occured.
	class CHoldArea;
	class CDayArea;

	class CHitData 
	{
	public:

		E_HT m_hitArea;
		int m_casterNum;
		CCastString* m_pString;
		CRect m_rect;
		vector<CString> m_vecText;
		const CDayArea* m_pDayArea;
		const CHoldArea* m_pHoldArea;
		CTime m_time;
		
		CHitData() 
			: m_hitArea(HT_OUTSIDE),
			  m_casterNum(0),
			  m_pString(0),
			  m_pDayArea(0),
			  m_pHoldArea(0),
			  m_time(CTime::GetCurrentTime())
		{}
	};




	// This class encapsulates the basic functionality required to draw the holding pen
	class CHoldArea
	{
		friend class CHMLoadGraphSheetViewport;

	public:
	
		static const int m_iconHeight;
		static const int m_iconMargin;
		static const int m_iconSpace; 

		int NumVisible();

		SECBitmapButton m_btnUp;
		SECBitmapButton m_btnDown;

		int m_offset;

		void Initialize(CWnd* parentWnd);

		CHMLoadGraphSheetViewport* m_pViewport;
		CRect m_rectEntire;
		CRect m_rectBody;

		vector <CCastStringData> m_castStringDataVec;

		void ComputeRects(CHMLoadGraphSheetViewport* pViewport, 
	 					  const CRect& rectEntire);

		void ComputeRects();

		void Draw( CDC* pDC );
		void DrawCastStringText( CDC* pDC, 
						         const CCastStringData& data);
		void RedrawString(CCastString* pString);

		void ComputeCastStringText(CCastString* pString,
								   vector<CString>& texts);

		bool HitTest(const CPoint& pt, CHitData& rHitData) const;
		vector< CCastStringData >::const_iterator GetCastStringDataAtPoint(const CPoint& pt) const;

		void ScrollUp();
		void ScrollDown();
		void OnUpdateScrollUp(CCmdUI* pCmdUI);
		void OnUpdateScrollDown(CCmdUI* pCmdUI);
	};



	// This class encapsulates the basic functionality required to draw
	//   a day on the sheet.

	class CDayArea 
	{
		friend class CHMLoadGraphSheetViewport;

	public:

		CHMLoadGraphSheetViewport* m_pViewport;

		CTime m_day;
		CRect m_rectEntire;
		CRect m_rectDate;
		CRect m_rectHM;
		CRect m_rectHours;
		CRect m_rectCasterText[4];  // caster = 1,2,3
		CRect m_rectCasterBar[4];	// caster = 1,2,3
		CRect m_rectLoadGraph;

		CString m_strDate;

		

		vector< CCastStringData > m_castStringTextData;
		vector< CCastStringData > m_castStringBarData;

		Weight m_supplyValues[24];  // hours of the day
		Weight m_loadValues[4][24];  // caster, hours of the day

		void ComputeRects(CHMLoadGraphSheetViewport* pViewport, 
					 const CTime& day, 
					 const CRect& rectEntire);
		void ComputeStringRects();
		void ComputeStringRects(int caster, vector<CCastString*>& strings);
		void ComputeCastStringText(CCastString* pString,
								   vector<CString>& texts);
		void ComputeLoadGraphValues();
		void Draw( CDC* pDC );
		void DrawBackground( CDC* pDC );
		void DrawDay( CDC* pDC );
		void DrawHM( CDC* pDC );
		void DrawHours( CDC* pDC );
		void DrawCastStringBackgrounds( CDC* pDC );
		void DrawCastStringText( CDC* pDC, const CCastStringData& data);
		void DrawCastStringBar( CDC* pDC, const CCastStringData& data);
		void DrawLoads(CDC* pDC);
		void DrawLoadRect(CDC* pDC,
						  COLORREF color, 
						  int startHour, 
						  double val1, 
						  double val2);
		void DrawLoadLine(CDC* pDC,
						  COLORREF color,
						  int startHour,
						  int finishHour,
						  double val1,
						  double val2,
						  int penWidth=1);
		void DrawLoadLineLabel(CDC* pDC,
							   COLORREF color,
							   const char* label,
							   double val);
		void DrawLoadBG(CDC* pDC);
		void RedrawString(CCastString* pString);


		bool HitTest(const CPoint& pt, CHitData& rHitData) const;
		vector< CCastStringData >::const_iterator GetCastStringTextDataAtPoint(const CPoint& pt) const;

		static double DayFraction(const CTime& x, const CTime& y);
		CTime XlatePoint(const CPoint& pt) const;	
	};
	
	// this class needs a lot of access to internals.
	friend class CDayArea;

	
	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////
	
public:

	// Some colors

	static const COLORREF sm_colorBack;
	static const COLORREF sm_colorDateBack;
	static const COLORREF sm_colorDateFore;
	static const COLORREF sm_colorCasterBack[4];
	static const COLORREF sm_colorCasterFore[4];
	static const COLORREF sm_colorLoad[4][3];
	static const COLORREF sm_colorSupply;
	static const COLORREF sm_colorLoadBack;
	static const COLORREF sm_color909Border;
	static const COLORREF sm_color910Border;
	static const COLORREF sm_colorSelectedHilite;

private:

	CHMLoadGraphDoc* m_pDoc;

	bool m_initialized;

	int m_numRows;
	int m_numCols;

	CTime m_startDay;

	vector<CDayArea> m_days;

	CHoldArea m_holdArea;

	CCastString* m_pSelectedString;



	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	
	CHMLoadGraphDoc* GetDocument() { return m_pDoc; }
	void SetDocument(CHMLoadGraphDoc* pDoc) { m_pDoc = pDoc; }


	int NumRows() const { return m_numRows; }
	int NumCols() const { return m_numCols; }

	CTime StartDay() const { return m_startDay; }

	vector<CDayArea>& Days() { return m_days; }

	CHoldArea& HoldArea() { return m_holdArea; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CHMLoadGraphSheetViewport();


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


	CMessageBus* GetMyModel();
	virtual BOOL CreateController();
	virtual void OnInitialUpdate();
	virtual BOOL OnCreate();
	void OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg);

	void Draw(CDC* pDC);      // overridden to draw this view
	CSize SetSize(int cx, int cy);



	static void DrawCenteredText(CDC* pDC, 
								 const CString& text, 
								 const CRect& boundRect);

	static void DrawCenteredText(CDC* pDC, 
								 const vector<CString>& strings, 
								 const CRect& boundRect);

	static CSize GetTextSize(CDC* pDC,
							 const vector<CString>& strings);

	CCastString* SelectedString()
	{	return m_pSelectedString;	}

	void DeltaRowCol(int rDelta, int cDelta);

	void DoVertScroll(CTimeSpan delta, int scrollDelta);
	void DoHorizScroll(CTimeSpan delta, int scrollDelta);

	bool IsStringSelected() const
	{	return m_pSelectedString != 0;	}

	void SetStartDay(CTime newVal);

	void MoveStringToHoldArea(CCastString* string);

	bool HitTest(const CPoint& point, CHitData& rHitData) const;

	bool SelectStringAtPoint(CPoint pt, CHitData& rHitData);

	void MoveTrackingOutline(CDC& dcString,
							 CBitmap* pBitmap,
							 const CRect& newRect,
							 const CString& newString,
							 const CRect& oldRect,
							 const CString& oldString);

	void DrawTrackingOutline(CDC& dcString,
							 CBitmap* pBitmap,
							 const CRect& rect, 
							 const CString& str);
	void DrawTrackingString(CDC& dc,
							CDC& dcString,
							 CBitmap* pBitmap,
							const CRect& rect, 
							const CString& str);

	void FindConfiningBar(int caster, 
						  const CPoint& point, 
						  int& rTop, 
						  int& rBottom);

	bool FindTimeAtPoint(const CPoint& point,
						 int caster,
						 CTime& time);


	void SetStringStartTime(CCastString* string,CTime time);
	
	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	const CDayArea& FindBestDayY(int caster,
								 const CPoint& point);

	const CDayArea* FindContainingDay(int caster,
									  const CPoint& point);



	void SetDisplaySize(int nr, int nc);

	void SetNominalScrollSize();
	void SetNominalScrollPos();
	void SetNominalScrollPos(int bar);
	void IncrementScrollPos(int bar, int incr);

	void UndrawSelectedString();
	void RedrawSelectedString();
	void SelectString(CCastString* pString);




};



class CHMLoadGraphSheetController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CHMLoadGraphSheetView/CHMLoadGraphSheetViewport
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

	CMessageBus* m_pMru;
		// the model


	bool m_bTracking;
	bool m_bTrackingDraw;
	CRect m_rectTracking;
	CPoint m_ptFirst;
	int m_mouseXOffset;
	CString m_strTime;

	CDC m_dcString;
	CBitmap* m_pbmString;

	CHMStartTimeDlg m_dlgStartTime;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetMyModel();

	CHMLoadGraphSheetViewport* GetViewport()
	{	return dynamic_cast<CHMLoadGraphSheetViewport*>(MvcController::GetViewport());	}

	CHMLoadGraphDoc* GetDocument() { return GetViewport()->GetDocument(); }

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:


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

	void GetEmailBody(int caster, 
					  vector<CCastString*>& strings1,
					  ostrstream& ostr);

	// Generated message map functions

	//{{AFX_MSG(CHMLoadGraphSheetController)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditCreateString();
	afx_msg void OnViewSetDate();
	afx_msg void OnViewDayPlus();
	afx_msg void OnViewDayMinus();
	afx_msg void OnViewWeekMinus();
	afx_msg void OnViewWeekPlus();
	afx_msg void OnViewAddCol();
	afx_msg void OnViewAddRow();
	afx_msg void OnViewDelCol();
	afx_msg void OnViewDelRow();
	afx_msg void OnUpdateViewAddCol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewAddRow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDelCol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDelRow(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditDeleteString();
	afx_msg void OnEditStringInfo();
	afx_msg void OnEditMoveToHoldArea();
	afx_msg void OnViewShow910s();
	afx_msg void OnUpdateViewShow910s(CCmdUI* pCmdUI);
	afx_msg void OnViewScrollholddown();
	afx_msg void OnUpdateViewScrollholddown(CCmdUI* pCmdUI);
	afx_msg void OnViewScrollholdup();
	afx_msg void OnUpdateViewScrollholdup(CCmdUI* pCmdUI);
	afx_msg void OnEditLoadhm();
	afx_msg void OnEditSendhm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




#endif // !defined(AFX_HMLOADGRAPHMVC_H__D0B67EB3_DDA0_11D4_864A_00104B2D39AC__INCLUDED_)
