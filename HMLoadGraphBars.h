// HMLoadGraphBars.h: interface for the CHMLoadGraphGraphBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMLOADGRAPHBARS_H__26B18989_0AB9_11D2_808D_006097B38214__INCLUDED_)
#define AFX_HMLOADGRAPHBARS_H__26B18989_0AB9_11D2_808D_006097B38214__INCLUDED_

#include "GanttDisplay.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CHMLoadGraphGanttViewport;
class CCastString;

#include "HMStartTimeDlg.h"

class CHMLoadGraphGraphBar : public CGanttDisplayGraph 
{
public:
	virtual ~CHMLoadGraphGraphBar() {}

	CHMLoadGraphGraphBar(Dimension height,
						 Dimension offset,
						 double min,
						 double max,
						 CHMLoadGraphGanttViewport* pView);

	CHMLoadGraphGanttViewport* m_pView;
	

	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);


};


class CHMLoadGraphLoadCountBar : public CGanttDisplayGraph 
{
public:
	virtual ~CHMLoadGraphLoadCountBar() {}

	CHMLoadGraphLoadCountBar(Dimension height,
						 Dimension offset,
						 double min,
						 double max,
						 CHMLoadGraphGanttViewport* pView);

	CHMLoadGraphGanttViewport* m_pView;
	

	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);


};



class CHMLoadGraphNumericBar : public CGanttDisplayBar  
{
public:
	virtual ~CHMLoadGraphNumericBar() {}

	enum BarType { BAR_TYPE_DEMAND = 0,
				   BAR_TYPE_CASTER1,
				   BAR_TYPE_CASTER2,
				   BAR_TYPE_CASTER3,
				   BAR_TYPE_SUPPLY };


	CHMLoadGraphNumericBar(Dimension height,
						   Dimension offset,
						   int which,
						   CHMLoadGraphGanttViewport* pView);

	CHMLoadGraphGanttViewport* m_pView;
	BarType m_type;

	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);

	void DrawCasterLoad(CDC* pDC,
						XUnit exposedStartXUnit,
						XUnit exposedFinishXUnit);

	void DrawCasterDemand(CDC* pDC,
						  XUnit exposedStartXUnit,
						  XUnit exposedFinishXUnit);

	void DrawCasterSupply(CDC* pDC,
						  XUnit exposedStartXUnit,
						  XUnit exposedFinishXUnit);


};


class CHMLoadGraphCasterBar : public CGanttDisplayBar
{
public:
	virtual ~CHMLoadGraphCasterBar() {}

	CHMLoadGraphCasterBar(Dimension height,
						 Dimension offset,
						 int caster_,
						 CHMLoadGraphGanttViewport* pView);

	CHMLoadGraphGanttViewport* m_pView;
	int m_caster;
	int m_selectedStringIndex;

	CHMStartTimeDlg m_dlgStartTime;

	
	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);

	void DrawCastString(CCastString* pString,
						int index,
						CDC* pDC,
						XUnit exposedStartXUnit,
						XUnit exposedFinishXUnit);

	void DrawCastString(CCastString* pString,
						int index,
						CDC* pDC);


	virtual bool OnLButtonDown(XUnit xUnit,
							   double value,
							   bool isInside,
							   bool isAbove,
							   CGanttDisplayBar* pActualBar);

	virtual void OnMouseMove(XUnit xUnit,
							 double value,
							 CGanttDisplayBar* pActualBar);

	virtual void OnLButtonUp(XUnit xUnit,
							 double value,
							 CGanttDisplayBar* pActualBar);

	void DrawTrackingOutline(CRect&);
	void MoveTrackingOutline(CRect&,CRect&);


	CCastString* CastStringHitTest(double xUnit,int& hitIndex);
	void CastStringRect(CCastString* pString, CRect& rect);
	void MoveString(CCastString* pString, CTime newStartTime);

	void ResetSelection() { m_selectedStringIndex = -1; }

	bool m_bTracking;
	CRect m_rectTracking;
	CCastString* m_pTrackedString;
	int m_mouseXOffset;


};

#endif // !defined(AFX_HMLOADGRAPHBARS_H__26B18989_0AB9_11D2_808D_006097B38214__INCLUDED_)
