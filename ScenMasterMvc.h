// ScenMasterMvc.h: interface for the CScenMasterCtrlViewport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENMASTERMVC_H__F7B30D53_DC6A_11D4_864A_00104B2D39AC__INCLUDED_)
#define AFX_SCENMASTERMVC_H__F7B30D53_DC6A_11D4_864A_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MessageBus.h"
class CCasterScen;
class CCastString;
class CCastStringHeat;



class CScenMasterCtrlViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CScenMasterCtrlView
	//  Associated with CScenMasterCtrlController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef vector<CCasterScen*> TScenVec;

	class CScenSelChangeMsg : public CCsdaMsg_Impl<MSG_ID_SMASTER_CHANGE_SEL_SCENS>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal change in scenario selection within this editor
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CScenSelChangeMsg(vector<CCasterScen*>& selScens)
			: m_selScens(selScens) // copy
		{}
		
		vector<CCasterScen*>& SelScens() { return m_selScens; }

	protected:
		vector<CCasterScen*> m_selScens;
		// the scenarios that have been selected
	};


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CListBox* m_pListSelScens;
	CListBox* m_pListAvailScens;
	CButton* m_pBtnSelectOne;
	CButton* m_pBtnSelectAll;
	CButton* m_pBtnDeselectOne;
	CButton* m_pBtnDeselectAll;
		// interface items, passed from the view

	TScenVec m_selScens;
		// the list of scens selected for display in the graph


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CListBox* ListSelScens() { return m_pListSelScens; }
	CListBox* ListAvailScens() { return m_pListAvailScens; }

	CButton* BtnSelectOne() { return m_pBtnSelectOne; }
	CButton* BtnSelectAll() { return m_pBtnSelectAll; }
	CButton* BtnDeselectOne() { return m_pBtnDeselectOne; }
	CButton* BtnDeselectAll()  { return m_pBtnDeselectAll; }

	TScenVec& SelScens() { return m_selScens;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

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


	void SetSelScensListCtrl(CListBox* pCtrl) {  m_pListSelScens = pCtrl; }
	void SetAvailScensListCtrl(CListBox* pCtrl) { m_pListAvailScens = pCtrl; }
	void SetSelectOneBtn(CButton* pCtrl) { m_pBtnSelectOne = pCtrl; }
	void SetSelectAllBtn(CButton* pCtrl) { m_pBtnSelectAll = pCtrl; }
	void SetDeselectOneBtn(CButton* pCtrl) { m_pBtnDeselectOne = pCtrl; }
	void SetDeselectAllBtn(CButton* pCtrl) { m_pBtnDeselectAll = pCtrl; }

	void DeselectAllScens();
	void DeselectSelectedScens();
	void SelectAllScens();
	void SelectSelectedScens();

	void SetButtons();
	void SetLists();

	static void ValidateScens(vector<CCasterScen*>& scens);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void PopulateList(CListBox* pListBox, 
					  vector<CCasterScen*>& scens);
	void ComputeAvailScens(vector<CCasterScen*>& availScens);
	void GetSelectedScens(CListBox* pBox,
						  vector<CCasterScen*>& scens);


	void SendUpdateMsg();

};


class CScenMasterCtrlController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CScenMasterCtrlViewport
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

	CMessageBus* m_pBus;
		// the model


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetMyModel();

	CScenMasterCtrlViewport* GetViewport()
	{	return dynamic_cast<CScenMasterCtrlViewport*>(MvcController::GetViewport());	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

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

	// Generated message map functions
protected:


	//{{AFX_MSG(CScenMasterCtrlController)
	afx_msg void OnButtonDeselectAll();
	afx_msg void OnButtonDeselectOne();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectOne();
	afx_msg void OnSelchangeListAvailableScens();
	afx_msg void OnSelchangeListSelectedScens();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


class CScenMasterGraphViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CScenMasterGraphView
	//  Associated with CScenMasterGraphController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	typedef vector<CCasterScen*> TScenVec;
	typedef map< CCasterScen*,     CRect > TScenRectMap;
	typedef map< CCastString*,     CRect > TStringRectMap;
	typedef map< CCastStringHeat*, CRect > THeatRectMap;

	enum { 
		
		STRING_HEIGHT = 35,
		HEAT_HEIGHT = 45,
		SCEN_HEIGHT  = STRING_HEIGHT+HEAT_HEIGHT,
		SCEN_VERT_SPACING = 10,
		HEADER_WIDTH = 75,
		LEFT_MARGIN = 10,
		RIGHT_MARGIN = 10,
		TOP_MARGIN = 10,
		BOTTOM_MARGIN = 10 
	};

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	TScenRectMap		m_scenRectMap;
		// Key: scenario; value: rectangle of displayed area for the scenario
		//   computed as a header plus the combined areas of all its strings
	    // one entry for each entry in m_selScens

	TStringRectMap	m_stringRectMap;
		// Key: CCastString; value: rectangle of displayed area for the string
		//  computed as the combined area of all its heats
		// one entry for each string in each scenario

	THeatRectMap		m_heatRectMap;
		// Key: CCastStringHEat; value rectangle of displayed area for the heat,
		//   computed on basis of heat duration
		// one entry for each heat in each string in each scenario

	TScenVec m_selScens;
		// the scenarios chosen for display

	SECPanView* m_pView;
		// backlink to the view to do some specialized setup (scroll sizes)


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	TScenRectMap& ScenRectMap() { return m_scenRectMap;	}
	TStringRectMap& StringRectMap() { return m_stringRectMap;	}
	THeatRectMap& HeatRectMap() { return m_heatRectMap;	}

	TScenVec& SelScens() { return m_selScens;	}

	SECPanView* View() { return m_pView; }


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

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

	void SetView(SECPanView* pV) { m_pView = pV; }


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////
	
private:

	void ClearMaps();

	void ComputeScenLocns();
	CRect ComputeScenLocn(	CCasterScen* pScen, 
							CRect& prevRect);
	CRect ComputeStringLocn( CCastString* pString,
							 CRect& scenRect);
	CRect ComputeHeatLocn(	CCastStringHeat& rHeat,
							CRect& lastRect );

	CRect GetLocn(CCasterScen* pScen);
	CRect GetLocn(CCastString* pString);
	CRect GetLocn(CCastStringHeat& rHeat);

	void SetScrollInfo();
	CSize ComputeSize();


	void DrawScen(CCasterScen* pScen, CDC* pDC);
	void DrawScenHeader(CCasterScen* pScen, CDC* pDC);
	void DrawCastString(CCastString* pString, CDC* pDC);
	void DrawCastStringHeat(CCastStringHeat& rHeat, CDC* pDC, COLORREF clrBackg);


	void DrawCenteredText(CDC* pDC, CString& text, CRect& boundRect);
	void DrawCenteredText(CDC* pDC, vector<CString>& strings, CRect& boundRect);



};


class CScenMasterGraphController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CScenMasterGraphViewport
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

	CMessageBus* m_pBus;
		// the model


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CMessageBus* GetMyModel();

	CScenMasterGraphViewport* GetViewport()
	{	return dynamic_cast<CScenMasterGraphViewport*>(MvcController::GetViewport());	}

	
	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

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

	// Generated message map functions
protected:


	//{{AFX_MSG(CScenMasterGraphController)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_SCENMASTERMVC_H__F7B30D53_DC6A_11D4_864A_00104B2D39AC__INCLUDED_)
