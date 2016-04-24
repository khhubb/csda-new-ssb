#ifndef _castStringEditorGanttBars_loaded_
#define _castStringEditorGanttBars_loaded_



#include "GanttDisplay.h"
#include "modelTypes.h"

class CCastStringEditorGanttViewport;


class CCastStringEditorGanttStrandWidthGraph : public CGanttDisplayGraph 
{
public:
	CCastStringEditorGanttStrandWidthGraph(Dimension height,
										   Dimension offset,
										   double min_,
										   double max_,
										   BOOL bottom_,
										   int strandNum_,
										   CCastStringEditorGanttViewport* pVP);

	virtual ~CCastStringEditorGanttStrandWidthGraph();


	int m_strandNum;

	CCastStringEditorGanttViewport* m_pVP;

	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);

	void DrawOrder(CDC* pDC, 
				  COLORREF color, 
				  long relStartTime,
				  long relFinishTime,
				  Width width);

	void DrawGridLines(CDC* pDC,
					   long relStartTime,
					   long relFinishTime);
			


	
	// override
	virtual void OnRButtonDown(XUnit xUnit,
							   double value,
							   bool isInside,
							   bool isAbove,
							   CGanttDisplayBar* pActualBar,
							   CPoint point);


};



class CCastStringEditorGanttHeatBar: public CGanttDisplayBar
{
public:


	CCastStringEditorGanttHeatBar(Dimension height,
							Dimension offset,
							 CCastStringEditorGanttViewport* pVP);

	virtual ~CCastStringEditorGanttHeatBar();

	CCastStringEditorGanttViewport* m_pVP;

	void OnDraw(CDC* pDC,
				XUnit exposedStartXUnit,
				XUnit exposedFinishXUnit);

	void DrawHeat(CDC* pDC,
				  COLORREF color,
				  int count,
				  long startTime,
				  long finishTime);

	virtual void OnRButtonDown(XUnit xUnit,
							   double value,
							   bool isInside,
							   bool isAbove,
							   CGanttDisplayBar* pActualBar,
							   CPoint point);


};





#endif // _castStringEditorGanttBars_loaded_