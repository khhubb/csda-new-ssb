// CastStringEditorGanttBars.cpp: implementation of the CCastStringEditorGanttStrandWidthGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "CastStringEditorGanttBars.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CastStringEditorMvc.h"
#include "CasterScen.h"
#include "CastString.h"
#include "CSOrder.h"
#include "OrderColor.h"
#include "ScenMru.h"
#include "CastStringEditorGanttView.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastStringEditorGanttStrandWidthGraph::CCastStringEditorGanttStrandWidthGraph
(Dimension height,
 Dimension offset,
 double min_,
 double max_,
 BOOL bottom_,
 int strandNum_,
 CCastStringEditorGanttViewport* pVP)
	:	CGanttDisplayGraph(height,offset,min_,max_,bottom_)
{
	m_strandNum	= strandNum_;
	m_pVP		= pVP;
}



CCastStringEditorGanttStrandWidthGraph::~CCastStringEditorGanttStrandWidthGraph()
{

}



void CCastStringEditorGanttStrandWidthGraph::OnDraw(CDC* pDC,
												    XUnit exposedStartXUnit,
													XUnit exposedFinishXUnit)
{
	TCastStringVec& strings = CScenMru::GlobalScenMru.CurrentCasterScen()->CastStrings();
	CCastString* pSelectedString = CScenMru::GlobalScenMru.CurrentCastString();

	if ( pSelectedString == 0 ) 

		DrawCenteredText(pDC,
					     "No string selected.",
						 0,250,
						 RGB(255,255,255),
						 RGB(0,0,0));

	else {

		const CCastString::T_CSOrderVec& orders = pSelectedString->Strand(m_strandNum);
		int count = 0;
	
		for ( CCastString::T_CSOrderVec::const_iterator io = orders.begin();
		      io != orders.end();
			  ++io, ++count )  {

			if ( (*io)->RelStartTime() > exposedFinishXUnit )
				goto grid2;

			if ( (*io)->RelFinishTime() >= exposedStartXUnit ) 
				DrawOrder(pDC,
						  COrderColor::GetColor((*io)),
						  (*io)->RelStartTime(),
						  (*io)->RelFinishTime(),
						  (*io)->SlabWidth());
		}
grid2:
		DrawGridLines(pDC,exposedStartXUnit,exposedFinishXUnit);

	}
}



void CCastStringEditorGanttStrandWidthGraph::DrawOrder(CDC* pDC, 
													   COLORREF color, 
													   long relStartTime,
													   long relFinishTime,
													   Width width)
{
	DrawBorderedRect(pDC,
				  color,
				  relStartTime,
				  relFinishTime,
				  width);

}

void CCastStringEditorGanttStrandWidthGraph::DrawGridLines(CDC* pDC,
														   long startXUnit,
														   long endXUnit)
{
	DrawGraphLine(pDC,
				  RGB(0,0,0),
				  startXUnit,
				  endXUnit,
				  20.0);

	DrawGraphLine(pDC,
				  RGB(0,0,0),
				  startXUnit,
				  endXUnit,
				  40.0);

	DrawGraphLine(pDC,
				  RGB(0,0,0),
				  startXUnit,
				  endXUnit,
				  60.0);

	DrawGraphLine(pDC,
				  RGB(0,0,0),
				  startXUnit,
				  endXUnit,
				  80.0);
}



//  At the moment, this is not context sensitive,
//  so we just send the request up to the view.

void CCastStringEditorGanttStrandWidthGraph::OnRButtonDown(XUnit xUnit,
														   double value,
														   bool isInside,
														   bool isAbove,
														   CGanttDisplayBar* pActualBar,
														   CPoint point)
{
	m_pVP->View()->ShowContextMenu(point);	
}








void CCastStringEditorGanttHeatBar::OnRButtonDown(XUnit xUnit,
												  double value,
												  bool isInside,
												  bool isAbove,
												  CGanttDisplayBar* pActualBar,
												  CPoint point)
{
	m_pVP->View()->ShowContextMenu(point);	
}



CCastStringEditorGanttHeatBar::CCastStringEditorGanttHeatBar(Dimension height,
															 Dimension offset,
															 CCastStringEditorGanttViewport* pVP)
	:	CGanttDisplayBar(height,offset)
{
	m_pVP = pVP;
}


CCastStringEditorGanttHeatBar::~CCastStringEditorGanttHeatBar()
{
}


void CCastStringEditorGanttHeatBar::OnDraw(CDC* pDC,
										   XUnit exposedStartXUnit,
										   XUnit exposedFinishXUnit)
{
	static COLORREF colors[2] = { RGB(190,127,63), RGB(63,127,190) };

	
	TCastStringVec& strings = CScenMru::GlobalScenMru.CurrentCasterScen()->CastStrings();
	CCastString* pSelectedString = CScenMru::GlobalScenMru.CurrentCastString();


	if ( pSelectedString != 0 ) {

		const CCastString::T_HeatVec& heats = pSelectedString->Heats();
		int count = 0;
		Weight heatSize = pSelectedString->AimHeatSize();
		Weight tons = pSelectedString->Props().Tons();
		int numHeats = pSelectedString->Props().NumHeats();
		bool showTonsLimit = tons > 0;
		bool showHeatsLimit = numHeats > 0;

	
		for ( CCastString::T_HeatVec::const_iterator ih = heats.begin();
		      ih != heats.end();
			  ++ih, ++count )  {

			if ( (*ih).RelStartTime() > exposedFinishXUnit )
				return;

			if ( (*ih).RelFinishTime() >= exposedStartXUnit ) {
				DrawHeat(pDC,
						 colors[count%2],
						 count,
						 (*ih).RelStartTime(),
						 (*ih).RelFinishTime());
				if (showTonsLimit && (count-1)*heatSize < tons && tons < count*heatSize
					||
					count == numHeats-1 )
					DrawVertLine(pDC,
								 RGB(255,0,0),
								 (*ih).RelFinishTime(),
								 0.0,1.0,4);
			}
		}	
	}
}



void CCastStringEditorGanttHeatBar::DrawHeat(CDC* pDC,
											 COLORREF color,
											 int count,
											 long startTime,
											 long finishTime)
{
	DrawBorderedRect(pDC,color,XUnit(startTime),XUnit(finishTime),1);

	CString str;
	str.Format("Heat #%d",count+1);
	DrawCenteredText(pDC,
					 str,
					 XUnit(startTime),
					 XUnit(finishTime),
					 color,
				     RGB(255,255,255));

}
		  
