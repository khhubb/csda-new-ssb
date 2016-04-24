// CastStringEditorMvc.h: interface for the CCastStringEditorGanttViewport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTSTRINGEDITORMVC_H__902B6843_2074_11D5_864F_00104B2D39AC__INCLUDED_)
#define AFX_CASTSTRINGEDITORMVC_H__902B6843_2074_11D5_864F_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MessageBus.h"
#include "modelTypes.h"


class CCastStringHeat;
class CCastString;
class COrderSelection;
class CCSOrder;
class CCasterScen;
class CCastStringSummaryRecord;
class CCastStringEditorStrandSummaryView;
class CCastStringEditorScenMgrView;
class CCastStringEditorGanttView;
class CCastStringEditorTextView;
class CCastStringEditorGanttStrandWidthGraph;
class CCastStringEditorGanttHeatBar;
class CGanttDisplay;
class CSuperScen;
class COrder;
class CCastStringId;
class CCastStringMiscProps;
class CNStateBitmapButton;
class CDragSourceButton;
class CPSheetAvailOrders;
class CListCtrlEx;



////////////////////////////////////////////////////////////////////
//
//  CCastStringEditorGanttViewport/Controller
//
////////////////////////////////////////////////////////////////////



class CCastStringEditorGanttViewport : public MvcViewport  
{
public:
	////////////////////////////////////////////////////////////////
	//	
	//	Viewport managing the display of CCastStringEditorGanttView
	//  Associated with CCastStringEditorGanttController
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

	CGanttDisplay* m_pGanttDisplay;
		// The view is a gantt display, which is how we want to know it.

	CCastStringEditorGanttStrandWidthGraph*	m_pStrand1WidthGraph;
	CCastStringEditorGanttStrandWidthGraph*	m_pStrand2WidthGraph;
	CCastStringEditorGanttHeatBar*			m_pHeatBar;

	CCastStringEditorGanttView* m_pView;


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


	CCastStringEditorGanttView* View() { return m_pView;	}

	void SetView(CCastStringEditorGanttView* pView) { m_pView = pView;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:
	
	CCastStringEditorGanttViewport()  : m_pGanttDisplay(0)
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


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void ShowContextMenu(CPoint point);
	void ComputeLimits();


};


class CCastStringEditorGanttController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CCastStringEditorGanttView/CHMLoadGraphGanttViewport
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

	CCastStringEditorGanttViewport* GetViewport()
	{	return dynamic_cast<CCastStringEditorGanttViewport*>(MvcController::GetViewport());	}


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

	//{{AFX_MSG(CCastStringEditorGanttController)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



class CCastStringEditorScenMgrViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CCastStringEditorScenMgrView
	//  Associated with CCastStringEditorScenMgrController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:



	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CCastStringEditorScenMgrView* m_pView;
		// decided it was easier just to deal with the view

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringEditorScenMgrView* View() { return m_pView; }
	void SetView(CCastStringEditorScenMgrView* view) { m_pView = view; }


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

	void Synchronize(bool updateLists);

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	int GetCasterScenIndex(int casterNum) const;


	void SelectComboBoxItem( int index, int ctrlId );
	void UpdateLists();
	void UpdateCasterList(int casterNum, int ctrlId);
	void UpdateProdnList(int ctrlId);

	void UpdateStringList();

};


class CCastStringEditorScenMgrController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CCastStringEditorScenMgrViewport
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

	CCastStringEditorScenMgrViewport* GetViewport()
	{	return dynamic_cast<CCastStringEditorScenMgrViewport*>(MvcController::GetViewport());	}


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


	//{{AFX_MSG(CCastStringEditorScenMgrController)
	afx_msg void OnSelchangeComboCaster1();
	afx_msg void OnSelchangeComboCaster2();
	afx_msg void OnSelchangeComboCaster3();
	afx_msg void OnSelchangeComboProdn();
	afx_msg void OnSelchangeComboString();
	afx_msg void OnRbCaster1();
	afx_msg void OnRbCaster2();
	afx_msg void OnRbCaster3();
	afx_msg void OnEditCreateString();
	afx_msg void OnEditDeleteString();
	afx_msg void OnScenCopy();
	afx_msg void OnScenCreate();
	afx_msg void OnScenDelete();
	afx_msg void OnScenInfo();
	afx_msg void OnUpdateScenCopy(CCmdUI* pCmdUI) ;
	afx_msg void OnUpdateScenDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCreateString(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateEditDeleteString(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditStringInfo(CCmdUI* pCmdUI);
	afx_msg void OnEditStringInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	void ChangeCasterScen(int whichCaster, int ctrlId);
	void SetSelectedCaster(int newNum);
	
	void CreateNewString(CCastStringId& id,
						CCastStringMiscProps& props);

	bool IsEditing() const;

	void SetCasterNum(int newCasterNum);

};



class CCastStringEditorStrandSummaryViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CCastStringEditorStrandSummaryView
	//  Associated with CCastStringEditorStrandSummaryController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CListCtrl* m_pListSummary;
	CCastStringEditorStrandSummaryView* m_pView;

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CListCtrl* ListSummary() { return m_pListSummary; }
	CCastStringEditorStrandSummaryView* View() { return m_pView; }

	
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


	void SetSummaryListCtrl(CListCtrl* pCtrl) {  m_pListSummary = pCtrl; }
	void SetView(CCastStringEditorStrandSummaryView* pView) { m_pView = pView; }

	void SetListItems();
	void SetListHeader();

	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void SetDisplay();

	void SetAimListItem(int itemNum,
					    CString str,
					    int size);

	void SetListItem(int itemNum,
					 CString str,
					 const CCastStringSummaryRecord& rec);

	void SetListItem(int itemNum,
					 CString str,
					 const CCastStringSummaryRecord& rec1,
					 const CCastStringSummaryRecord& rec2,
					 const CCastStringHeat& rHeat,
					 CCastString* pString);

	void SetLuTypeTonsListItem(int itemNum,
							   const CString& luType,
							   Weight tons);
	void SetCICodeTonsListItem(int itemNum,
							   int ciCode,
							   Weight tons);



	static vector<CString> headerStrings;
	static vector<int> headerWidths;

	void InitHeaderDetails();
};


class CCastStringEditorStrandSummaryController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CCastStringEditorStrandSummaryViewport
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

	CCastStringEditorStrandSummaryViewport* GetViewport()
	{	return dynamic_cast<CCastStringEditorStrandSummaryViewport*>(MvcController::GetViewport());	}


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


	//{{AFX_MSG(CCastStringEditorStrandSummaryController)
	afx_msg void OnCheckAvgWidth();
	afx_msg void OnCheckCmsFlips();
	afx_msg void OnCheckCurrentBackFuture();
	afx_msg void OnCheckSlitsSplits();
	afx_msg void OnCheckStock();
	afx_msg void OnCheckHeatStrands();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class CCastStringEditorTextViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CCastStringEditorTestView
	//  Associated with CCastStringEditorTextController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	class CStrandOrderPosnHint 
	{
	public:
		CStrandOrderPosnHint(int strandNum,int heatIndex, int orderIndex)
			: m_strandNum(strandNum), m_orderIndex(orderIndex), m_heatIndex(heatIndex)
		{}

		int m_strandNum;
		int m_orderIndex;
		int m_heatIndex;
	};


	class CCSOrderChangedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ORDER_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal change in CSOrder parms within this editor
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CCSOrderChangedMsg(CCastString* pString,int strandNum, int index)
			: m_pCastString(pString), m_strandNum(strandNum), m_index(index)
		{}
		
		CCastString* CastString() { return m_pCastString; }
		int StrandNum() const { return m_strandNum;	 }
		int Index() const { return m_index;	}

	protected:
		CCastString* m_pCastString;
		// the cast string that has been validated
		int m_strandNum;
		int m_index;
			// strand# and index of CSOrder that was modified.
	};


	class COrderForInsertChangeMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ORDER_FOR_INSERT_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal change in the order-for-insert
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		COrderForInsertChangeMsg(COrder* pOrder)
			: m_pOrder(pOrder)
		{}
		
		COrder* Order() { return m_pOrder; }

	protected:
		COrder* m_pOrder;
			// the new order-for-insert
	};


	class COrdersMovedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ORDERS_MOVED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		COrdersMovedMsg(CCastString* pString)
			: m_pCastString(pString), m_strandNum(-1), m_heatIndex(-1), m_orderIndex(-1)
		{}
		
		COrdersMovedMsg(CCastString* pString,int strandNum, int heatIndex, int orderIndex)
			: m_pCastString(pString), m_strandNum(strandNum), m_heatIndex(heatIndex), m_orderIndex(orderIndex)
		{}

		CCastString* CastString() { return m_pCastString; }

		int StrandNum() { return m_strandNum;	}
		int HeatIndex() { return m_heatIndex;	}
		int OrderIndex() { return m_orderIndex;	}

	protected:
		CCastString* m_pCastString;
			// the cast string that has been moved
		int m_strandNum;
		int m_heatIndex;
		int m_orderIndex;
			// hint as to focus.
	};


	class CStringCanEditStatusChanged : public CCsdaMsg_Impl<MSG_ID_CSE_CAN_EDIT_STATUS_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CStringCanEditStatusChanged(CCastString* pString)
			: m_pCastString(pString)
		{}
		
		CCastString* CastString() { return m_pCastString; }

	protected:
		CCastString* m_pCastString;
		// the cast string that has been moved
	};

	
	class CLockedHeatsStatusChangedMsg : public CCsdaMsg_Impl<	MSG_ID_CSE_LOCKED_HEATS_STATUS_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CLockedHeatsStatusChangedMsg(CCastString* pString)
			: m_pCastString(pString)
		{}
		
		CCastString* CastString() { return m_pCastString; }

	protected:
		CCastString* m_pCastString;
		// the cast string that has been moved
	};

	class CScenSavedMsg : public CCsdaMsg_Impl<	MSG_ID_CSE_SCEN_SAVED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that a scenario has been saved
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CScenSavedMsg(CCasterScen* pScen)
			: m_pCasterScen(pScen)
		{}
		
		CCasterScen* CasterScen() { return m_pCasterScen; }

	protected:
		CCasterScen* m_pCasterScen;
		// the caster scenario that has been moved
	};


	class COrdersDeletedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ORDERS_DELETED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		COrdersDeletedMsg(CCastString* pString)
			: m_pCastString(pString)
		{}
		
		CCastString* CastString() { return m_pCastString; }

	protected:
		CCastString* m_pCastString;
		// the cast string that has been moved
	};

	class COrdersInsertedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_ORDERS_INSERTED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that orders have been moved in a string
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		COrdersInsertedMsg(CCastString* pString)
			: m_pCastString(pString), m_strandNum(-1), m_heatIndex(-1), m_orderIndex(-1)
		{}
		
		COrdersInsertedMsg(CCastString* pString,int strandNum, int heatIndex, int orderIndex)
			: m_pCastString(pString), m_strandNum(strandNum), m_heatIndex(heatIndex), m_orderIndex(orderIndex)
		{}

		CCastString* CastString() { return m_pCastString; }

		int StrandNum() { return m_strandNum;	}
		int HeatIndex() { return m_heatIndex;	}
		int OrderIndex() { return m_orderIndex;	}

	protected:
		CCastString* m_pCastString;
			// the cast string that has been moved
		int m_strandNum;
		int m_heatIndex;
		int m_orderIndex;
			// hint as to focus.
	};


		

	class CStringInfoChangedMsg : public CCsdaMsg_Impl<MSG_ID_CSE_STRING_INFO_CHANGED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal that string info has changed
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CStringInfoChangedMsg(CCastString* pString)
			: m_pCastString(pString)
		{}
		
		CCastString* CastString() { return m_pCastString; }

	protected:
		CCastString* m_pCastString;
		// the cast string that has been moved
	};




	enum E_AvailOrderListField {
		AVAIL_ORDER_FIELD_CI,
		AVAIL_ORDER_FIELD_SPEC,
		AVAIL_ORDER_FIELD_PRIO,
		AVAIL_ORDER_FIELD_MEGA_LU,
		AVAIL_ORDER_FIELD_ORDER_NUM,
		AVAIL_ORDER_FIELD_CUST,
		AVAIL_ORDER_FIELD_LENGTH,
		AVAIL_ORDER_FIELD_PLANNED_START,
		AVAIL_ORDER_FIELD_SLAB_WIDTH,
		AVAIL_ORDER_FIELD_NUM_PIECES,
		AVAIL_ORDER_FIELD_TONS,
		AVAIL_ORDER_FIELD_WEIGHTS,
		AVAIL_ORDER_FIELD_CONDN_CODE,
		AVAIL_ORDER_FIELD_HR_WEEK,
//		AVAIL_ORDER_FIELD_PST_WEEK,
		AVAIL_ORDER_FIELD_HB_WIDTH,
		AVAIL_ORDER_FIELD_CARBON,
		AVAIL_ORDER_FIELD_MANGANESE,
		AVAIL_ORDER_FIELD_SIGNIF_EL,
	};


	enum { NumAvailOrderFields = AVAIL_ORDER_FIELD_SIGNIF_EL+1 };


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CNStateBitmapButton* m_pBtnEditable;
	CNStateBitmapButton* m_pBtnHeatsLocked;
	SECBitmapButton* m_pBtnSave;
	CDragSourceButton* m_pInsertStockOrderButton;
	CDragSourceButton* m_pInsertOrderButton;
	CListCtrlEx* m_pListOrdersEx;
	CListCtrlEx* m_pListGroupsEx;
	CListCtrlEx* m_pStrand1OrderListEx;
	CListCtrlEx* m_pStrand2OrderListEx;

	SECListBoxEditor* m_pLbeGroupFields;
	SECListBoxEditor* m_pLbeSortFields;

	CCastStringEditorTextView* m_pView;

	CPSheetAvailOrders* m_pAvailOrdersDlg;

	COrder* m_pOrderForInsert;

	int m_availOrderListFieldIndexes[NumAvailOrderFields];
	int m_groupHeaderCount;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	COrder* OrderForInsert() { return m_pOrderForInsert;	}

	CCastStringEditorTextView* View() { return m_pView; }

	
	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringEditorTextViewport();
	~CCastStringEditorTextViewport();

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


	void SetEditableBtn(CNStateBitmapButton* pBtn)			{ m_pBtnEditable			= pBtn;	}
	void SetHeatsLockedBtn(CNStateBitmapButton* pBtn)		{ m_pBtnHeatsLocked			= pBtn;	}
	void SetSaveBtn(SECBitmapButton* pBtn)					{ m_pBtnSave				= pBtn;	}
	void SetInsertStockOrderButton(CDragSourceButton* p)	{ m_pInsertStockOrderButton	= p;	}
	void SetInsertOrderButton(CDragSourceButton* p)			{ m_pInsertOrderButton		= p;	}
	void SetListOrdersEx(CListCtrlEx* p)					{ m_pListOrdersEx			= p;	}
	void SetListGroupsEx(CListCtrlEx* p)					{ m_pListGroupsEx			= p;	}
	void SetStrand1OrderListEx(CListCtrlEx* p)				{ m_pStrand1OrderListEx		= p;	}
	void SetStrand2OrderListEx(CListCtrlEx* p)				{ m_pStrand2OrderListEx		= p;	}
	void SetLbeGroupFields(SECListBoxEditor* p)				{ m_pLbeGroupFields			= p;	}
	void SetLbeSortFields(SECListBoxEditor* p)				{ m_pLbeSortFields			= p;	}

	void SetView(CCastStringEditorTextView* pView) { m_pView = pView; }

	bool IsEditing() const;
	CCastString* CurrentCastString();
	CCasterScen* CurrentCasterScen();
	CCasterScen* CurrentCasterScen(int casterNum);
	int CasterNum();
	COrderSelection* OrderSelection();
	CSuperScen* SuperScen();
	
	void DecodeStrandListItem(int strandNum,
									 int listIndex,
									 int& heatIndex,
									 int& orderIndex);
	void OnGetdispinfoListGroups(NMHDR* pNMHDR);

	void GetSelectedCSOrderIndexes(vector<int>&,int strandNum);
	void GetSelectedCSOrderIndexes(vector<int>&,CListCtrl* pList,int strandNum);
	void GetSelectedOrders(vector<COrder*>& selectedOrders);
	void GetOrdersForSelectedGroups(vector<COrder*>& selectedOrders);


	void OnButtonGroup1();
	void OnButtonGroup2();
	void OnButtonUpdateGrouping();
	void OnButtonUpdateSorting();

	void ReinitOrderSelection();

	void SetGroupOrdersListItems();
	void UpdateSelectedGroups();

	void ChangeOrderForInsert(COrder* pOrder);
	void DeleteOrders(vector<int>& selectedItemIndexes, 
		              int strand);
	void MoveOrders(vector<int>& selectedItemIndexes, 
				    int sourceStrand, 
					int strand,
					int heatIndex,
					int orderIndex);
	void AddOrders(vector<COrder*>& selectedItems,
		           int strand,
				   bool query);
	void InsertOrders(vector<COrder*>& selectedItems, 
		              int strand, 
					  int heatIndex,
					  int orderIndex,
					  bool query);

	void AddStockOrder(int strand);
	void InsertStockOrder(int strand, int heatIndex, int orderIndex);
	void DoOrderInquiry(vector<int>& selectedItemIndexes,
					    int strand);
	void DoOrderInquiry(vector<COrder*>& selectedItems);


	void OnItemchangedStrandOrderList(int strandNum);



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void UpdateTitle();

	void SetButtons();
	void SetGroupListHeader();
	void SetGroupOrdersListHeader();
	void SetStrandOrderListHeaders();
	void SetOrderForInsert();

	void SetStrandOrderLists(bool maintainPosition = false, CStrandOrderPosnHint* pHint = 0);
	void SetStrandOrderList(CCastString* pString,
							int strandNum, 
							CListCtrl& listCtrl,
							bool maintainPosition);
	void SetStrandOrderListItem(CCSOrder* pCSOrder,
								int strandNum, 
								CListCtrl& listCtrl,
								int itemNum,
								int orderCount,
								bool doInsert);
	void SetStrandOrderListItem(CCastStringHeat& rHeat,
								int strandNum, 
								CCastString* pString,
								CListCtrl& listCtrl,
								int itemNum,
								int heatCount,
								bool doInsert);


	void SetGroupListItems(bool maintainPosition = false);
	void SetGroupListSelections();
	void AddGroupOrderListItem(int count,		
		                       COrder* pOrder,
							   ostrstream& ostr,
							   bool isSpecial);


};


class CCastStringEditorTextController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CCastStringEditorTextViewport
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

	CCastStringEditorTextViewport* GetViewport()
	{	return dynamic_cast<CCastStringEditorTextViewport*>(MvcController::GetViewport());	}


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


	//{{AFX_MSG(CCastStringEditorTextController)
	afx_msg void OnViewAvailableorders();
	afx_msg void OnItemchangedListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonGroup1();
	afx_msg void OnButtonGroup2();
	afx_msg void OnButtonUpdateGrouping();
	afx_msg void OnButtonUpdateSorting();
	afx_msg void OnDblclkStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditOrderInquiry();
	afx_msg void OnItemchangedStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCreateString();
	afx_msg void OnEditHeatDetails();
	afx_msg void OnButtonEditable();
	afx_msg void OnButtonHeatsLocked();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonUpload();
	afx_msg void OnGetdispinfoListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditStringbuilder();
	afx_msg void OnUpdateEditStringbuilder(CCmdUI* pCmdUI);
	afx_msg void OnEditPstconformancereport();
	afx_msg void OnUpdateEditPstconformancereport(CCmdUI* pCmdUI);
	afx_msg void OnEditMovestringtoothercaster();
	afx_msg void OnUpdateEditMovestringtoothercaster(CCmdUI* pCmdUI);
	afx_msg void OnEditStringInfo();
	afx_msg void OnUpdateEditStringInfo(CCmdUI* pCmdUI);
	afx_msg void OnEditPlanworksheet();
	afx_msg void OnUpdateEditPlanworksheet(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnDblclickStrandOrderList(int strandNum);

};


class CCastStringEditorValidnViewport : public MvcViewport  
{
	////////////////////////////////////////////////////////////////
	//	
	//	This component is responsible for maintaining the display
	//    of the CCastStringEditorValidnView
	//  Associated with CCastStringEditorValidnController
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, enums, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	class CValidnMsg : public CCsdaMsg_Impl<MSG_ID_CSE_STRING_VALIDATED>
	{
		////////////////////////////////////////////////////////////////
		//	
		//	Used to signal change in scenario selection within this editor
		//	
		////////////////////////////////////////////////////////////////
		
	public:
		
		virtual void Sprint(CString& strCmd);
		
		CValidnMsg(CCastString* pString)
			: m_pCastString(pString)
		{}
		
		CCastString* CastString() { return m_pCastString; }

	protected:
		CCastString* m_pCastString;
		// the cast string that has been validated
	};



	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CListCtrl* m_pListValidn;


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CListCtrl* ListValidn() { return m_pListValidn; }


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


	void SetValidnListCtrl(CListCtrl* pCtrl) {  m_pListValidn = pCtrl; }


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

private:

	void UpdateList();


};


class CCastStringEditorValidnController : public MvcController  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Controller for CCastStringEditorValidnViewport
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

	CCastStringEditorValidnViewport* GetViewport()
	{	return dynamic_cast<CCastStringEditorValidnViewport*>(MvcController::GetViewport());	}


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


	//{{AFX_MSG(CCastStringEditorValidnController)
	afx_msg void OnValidate();
	afx_msg void OnItemchangedListResults(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_CASTSTRINGEDITORMVC_H__902B6843_2074_11D5_864F_00104B2D39AC__INCLUDED_)
