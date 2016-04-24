#if !defined(AFX_CASTSTRINGEDITORTEXTVIEW_H__15713300_13EB_11D1_8027_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGEDITORTEXTVIEW_H__15713300_13EB_11D1_8027_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CastStringEditorTextView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorTextView form view
 
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CastStringEditorMvc.h"
#include "DragSourceButton.h"
#include "NStateBitmapButton.h"
#include "ListCtrlEx.h"
#include "Notify.h"
#include "OrderSelection.h"

class CCastStringEditorDoc;
class COrder;


class CCastStringEditorTextView : public CFormView
{
protected:
	CCastStringEditorTextView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCastStringEditorTextView)


// Form Data
public:
	//{{AFX_DATA(CCastStringEditorTextView)
	enum { IDD = IDD_DLG_CAST_STRING_EDITOR_TEXT_VIEW };
	CStatic	m_iconStrand2End;
	CStatic	m_iconStrand1End;
	CStatic	m_iconWastebasket;
	CString	m_strOrderToInsert;
	//}}AFX_DATA

// Attributes
public:


	CCastStringEditorTextViewport m_VP;

	CCastStringEditorDoc* GetDocument();


	SECBitmapButton m_secBtnOrderInquiry;
	SECBitmapButton m_secBtnSave;
	SECBitmapButton m_secBtnUpload;

	CNStateBitmapButton m_btnEditable;
	CNStateBitmapButton m_btnHeatsLocked;

	CImageList m_imageList;

	CDragSourceButton m_insertStockOrderButton;
	CDragSourceButton m_insertOrderButton;

	CListCtrlEx m_listOrdersEx;
	CListCtrlEx m_listGroupsEx;
	CListCtrlEx m_strand1OrderListEx;
	CListCtrlEx m_strand2OrderListEx;

	SECListBoxEditor m_lbeGroupFields;
	SECListBoxEditor m_lbeSortFields;

	COrderSelection::T_SortVec m_availSortFields;
	COrderSelection::T_SortVec m_availGroupFields;
	COrderSelection::T_SortVec m_sortFields;
	COrderSelection::T_SortVec m_groupFields;
	
	void InitLayout();
	SECLayoutFactory m_LFactory;


	class CInsertNotify : public CNotify
	{
	public:
		CInsertNotify()  { pView = 0; }
		~CInsertNotify() { /* nada */ }

		void SetView(CCastStringEditorTextView* p) { pView = p; }

		CCastStringEditorTextView* pView;

		void Notify() {  }
	};

	class CInsertStockOrderNotify : public CInsertNotify
	{
	public:

		void Notify() { pView->BeginInsertStockOrder(); }
	};

	class CInsertOrderNotify : public CInsertNotify
	{
	public:

		void Notify() { pView->BeginInsertOrder(); }
	};

	CInsertStockOrderNotify m_insertStockOrderNotify;
	CInsertOrderNotify m_insertOrderNotify;

	void BeginInsertStockOrder();
	void BeginInsertOrder();



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCastStringEditorTextView)
	public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

 
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );


	static void AFXAPI DDX_GroupSortAvailFieldList(CDataExchange* pDX, 
												   int nIDC, 
												   CListBox& box, 
												   COrderSelection::T_SortVec& selectVec,
												   COrderSelection::T_SortVec& availVec);

	static const CString sm_strAvail;
	static const CString sm_strGroup;
	static const CString sm_strSort;

	static void CanonicalizeFieldsListBox(CListBox& box);
	static bool AddFieldsListContents(CListBox& box,
									  COrderSelection::T_SortVec& vec,
									  const CString& str);


	
	// Implementation



	COleDropTarget m_oleDropTarget;


	enum DragSource { SOURCE_STRAND,
					  SOURCE_GROUP,
					  SOURCE_GROUP_ORDER,
					  SOURCE_STOCK_ORDER,
					  SOURCE_INSERT_ORDER,
					  SOURCE_OTHER };

	enum DropTarget { TARGET_STRAND,
					  TARGET_STRAND_END,
					  TARGET_WASTEBASKET,
					  TARGET_QUESTION,
					  TARGET_OTHER };

	// initiating drag -- state variables
	DragSource m_dragSource;
	int m_sourceStrand;
	CListCtrl* m_pSourceList;
	vector<int> m_selectedCSOrderIndexes;
	vector<COrder*> m_selectedOrders;

	// drag in process -- state variables
	bool m_bRightButton;
	bool m_bDragActive;
	bool m_bDropEffect;
	DropTarget m_dropTarget;
	int m_targetStrand;
	int m_targetItem;
	CListCtrl* m_pTargetList;



	void CleanupDropEffect();
	DROPEFFECT SetDropEffect(DropTarget nextTarget,
							 int nextStrand,
							 int nextItem,
							 CListCtrl* pNextList,
							 UINT flags);
	DROPEFFECT ComputeDROPEFFECT();







protected:
	virtual ~CCastStringEditorTextView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCastStringEditorTextView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBegindragStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragListOrders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdragListGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdragListOrders(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTSTRINGEDITORTEXTVIEW_H__15713300_13EB_11D1_8027_006097B38214__INCLUDED_)


#if 0 // mvc


class COrderSelection;
class COrder;

class CCastString;
class CCastStringHeat;
class CCSOrder;

#include "OrderSelection.h"

#include "Notify.h"


// message map



//	void UpdateGroups();



	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);






	void OnDblclickStrandOrderList(CListCtrl& listCtrl, int strandNum);
	void OnItemchangedStrandOrderList(int strandNum,
									  CListCtrl* pCtrl);






public:

	void OpenAvailOrdersDlg();
	void CloseAvailOrdersDlg();






	//
	//////////////////////////////END////////////////////////////////////



#endif // mvc