// CastStringEditorTextView.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CastStringEditorTextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastStringEditorDoc.h"
#include "ClipFormat.h"
#include "CastString.h"


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorTextView
//  Text View for the cast string editor.
//
//  Display managed by CCastStringEditorTextViewport
//  Events  managed by CCastStringEditorTextController
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCastStringEditorTextView, CFormView)

CCastStringEditorTextView::CCastStringEditorTextView()
	: CFormView(CCastStringEditorTextView::IDD)
{
	//{{AFX_DATA_INIT(CCastStringEditorTextView)
	m_strOrderToInsert = _T("");
	//}}AFX_DATA_INIT

	m_bDragActive	= false;
	m_bDropEffect	= false;
	m_dragSource	= SOURCE_OTHER; 
	m_dropTarget	= TARGET_OTHER;
	m_sourceStrand	=  0;
	m_targetStrand	=  0;
	m_targetItem	= -1;
	m_pSourceList	=  0;
	m_pTargetList	=  0;
}

CCastStringEditorTextView::~CCastStringEditorTextView()
{
}

void CCastStringEditorTextView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCastStringEditorTextView)
	DDX_Control(pDX, IDC_STRAND2_END, m_iconStrand2End);
	DDX_Control(pDX, IDC_STRAND1_END, m_iconStrand1End);
	DDX_Control(pDX, IDC_WASTEBASKET, m_iconWastebasket);
	DDX_Text(pDX, IDC_ORDER_TO_INSERT, m_strOrderToInsert);
	//}}AFX_DATA_MAP


	DDX_GroupSortAvailFieldList(pDX,
								IDC_LIST_GROUP_FIELDS,
								*((CListBox*)GetDlgItem(IDC_LIST_GROUP_FIELDS)),
								m_groupFields,
								m_availGroupFields);

	DDX_GroupSortAvailFieldList(pDX,
								IDC_LIST_SORT_FIELDS,
								*((CListBox*)GetDlgItem(IDC_LIST_SORT_FIELDS)),
								m_sortFields,
								m_availSortFields);

}


BEGIN_MESSAGE_MAP(CCastStringEditorTextView, CFormView)
	//{{AFX_MSG_MAP(CCastStringEditorTextView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_STRAND1_ORDER_LIST, OnBegindragStrand1OrderList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_STRAND2_ORDER_LIST, OnBegindragStrand2OrderList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_GROUPS, OnBegindragListGroups)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_ORDERS, OnBegindragListOrders)
	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LIST_GROUPS, OnBeginrdragListGroups)
	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LIST_ORDERS, OnBeginrdragListOrders)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorTextView diagnostics

#ifdef _DEBUG
void CCastStringEditorTextView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCastStringEditorTextView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCastStringEditorTextView message handlers


////////////////////////////////////////////////////////////////
//	
//	Some auxiliaries
//	
////////////////////////////////////////////////////////////////


CCastStringEditorDoc* CCastStringEditorTextView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCastStringEditorDoc)));
	return  (CCastStringEditorDoc*)(m_pDocument);
}



////////////////////////////////////////////////////////////////
//	
//	Some basic infrastructure
//	
////////////////////////////////////////////////////////////////


int CCastStringEditorTextView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//  Register as a drop target
	m_oleDropTarget.Register(this);

	//  create viewport
	m_VP.Create(this,0);
	m_VP.CreateController();
	m_VP.SetModel(GetDocument()->GetModel());


	return 0;
}



void CCastStringEditorTextView::InitLayout()
{

	//  The outermost node is relative,
	//  This allows us to position the buttons on the left
	//     and let everything else stretch to fit.

	SECLNRelative* pTopNode
		= (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));

	//  We can use the initial positions of the buttons just fine.
	//  Create a generic SECLayoutNode to hold them.

	SECLayoutNode* pButtonsNode	= m_LFactory.CreateNode(NODE_CLASS(SECLayoutNode));

	//  add the buttons

	UINT uIDs[] = { IDC_BUTTON_UPLOAD,
					ID_EDIT_STRING_INFO,
					ID_EDIT_HEAT_DETAILS,
					IDC_EDIT_INSERT_STOCK_ORDER,
					ID_EDIT_ORDER_INQUIRY,
					IDC_ORDER_TO_INSERT,
					IDC_WASTEBASKET,
					ID_VIEW_AVAILABLEORDERS,
					IDC_BUTTON_EDITABLE,
					IDC_BUTTON_HEATS_LOCKED,
					IDC_BUTTON_SAVE };

	m_LFactory.CreateNodeWndArray(uIDs,sizeof(uIDs)/sizeof(UINT),this, pButtonsNode);
	pButtonsNode->SetInitialRect();



	//  Occupying the rest of the space (to the right of the buttons)
	//    we place a splitter which will be in the following configuration:
	//
	//    +----------------------------+----------+
	//    |                            |          |
	//    |             A              |          |
	//    |                            |          |
	//    |                            |          |
	//    +----------------------------+     C    +
	//    |                            |          |
	//    |             B              |          |
	//    |                            |          |
	//    |                            |          |
	//    +----------------------------+----------+
	//
	//    where A is the strand1 items, B is the strand2 items and C is the grouping/sorting items


	SECLayoutNodeSplitter* pSplitNode 
		= (SECLayoutNodeSplitter*) m_LFactory.CreateNode(NODE_CLASS(SECLayoutNodeSplitter));
	pSplitNode->Create(this);
	//pSplitNode->SetSplitterFlags(SEC_SPLITTER_REALTIME_DRAG);




	// Now, create the panes residing inside the splitter.
	// A & B in the picture above are essentially the same.
	//
	// A group box surrounding a listbox with an icon just below the list
	//
	//       +---Group--------------------------+
	//       |                                  |
	//       |  +----------------------------+  | 
	//       |  |                            |  |
	//       |  |                            |  |
	//       |  |           List             |  |
	//       |  |                            |  |
	//       |  |                            |  |
	//       |  +----------------------------+  | 
	//       |                                  |
	//       |  +----------------------------+  | 
	//       |  |           Icon             |  |
	//       |  +----------------------------+  | 
	//       |                                  |
	//       +----------------------------------+
	//
	//
	//  We will start off using a scale, but that might not handle the icon properly.

	SECLNRelative* pRel1 
		= (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));
	SECLayoutNodeWnd* pBox1  = m_LFactory.CreateNodeWnd( IDC_STATIC_STRAND1_GROUP, this );
	SECLayoutNodeWnd* pList1 = m_LFactory.CreateNodeWnd( IDC_STRAND1_ORDER_LIST,   this );
	SECLayoutNodeWnd* pIcon1 = m_LFactory.CreateNodeWnd( IDC_STRAND1_END,          this );

	pRel1->AddLayoutNode(pList1);
	pRel1->AddLayoutNode(pBox1);
	pRel1->AddLayoutNode(pIcon1);
	pRel1->SetConstraint( pBox1,  REL_MOVEL,   pRel1, REL_LEFT,     5 );
	pRel1->SetConstraint( pBox1,  REL_MOVET,   pRel1, REL_TOP,      5 );
	pRel1->SetConstraint( pBox1,  REL_RIGHT,   pRel1, REL_RIGHT,   -5 );
	pRel1->SetConstraint( pBox1,  REL_BOTTOM,  pRel1, REL_BOTTOM,  -5 );
	pRel1->SetConstraint( pIcon1, REL_MOVEB,   pBox1, REL_BOTTOM,  -5 );
	pRel1->SetConstraint( pIcon1, REL_MOVEL,   pBox1, REL_LEFT,     5 );
	pRel1->SetConstraint( pIcon1, REL_RIGHT,   pBox1, REL_RIGHT,   -5 );
	pRel1->SetConstraint( pList1, REL_MOVEL,   pBox1, REL_LEFT,     5 );
	pRel1->SetConstraint( pList1, REL_MOVET,   pBox1, REL_TOP,     15 );
	pRel1->SetConstraint( pList1, REL_RIGHT,   pBox1, REL_RIGHT,   -5 );
	pRel1->SetConstraint( pList1, REL_BOTTOM,  pIcon1, REL_TOP,	    0 );

	pRel1->SetInitialRect();

	pSplitNode->AddPane(0,0,pRel1);

	SECLNRelative* pRel2 
		= (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));
	SECLayoutNodeWnd* pBox2  = m_LFactory.CreateNodeWnd( IDC_STATIC_STRAND2_GROUP, this );
	SECLayoutNodeWnd* pList2 = m_LFactory.CreateNodeWnd( IDC_STRAND2_ORDER_LIST,   this );
	SECLayoutNodeWnd* pIcon2 = m_LFactory.CreateNodeWnd( IDC_STRAND2_END,          this );

	pRel2->AddLayoutNode(pList2);
	pRel2->AddLayoutNode(pBox2);
	pRel2->AddLayoutNode(pIcon2);
	pRel2->SetConstraint( pBox2,  REL_MOVEL,   pRel2, REL_LEFT,     5 );
	pRel2->SetConstraint( pBox2,  REL_MOVET,   pRel2, REL_TOP,      5 );
	pRel2->SetConstraint( pBox2,  REL_RIGHT,   pRel2, REL_RIGHT,   -5 );
	pRel2->SetConstraint( pBox2,  REL_BOTTOM,  pRel2, REL_BOTTOM,  -5 );
	pRel2->SetConstraint( pIcon2, REL_MOVEB,   pBox2, REL_BOTTOM,  -5 );
	pRel2->SetConstraint( pIcon2, REL_MOVEL,   pBox2, REL_LEFT,     5 );
	pRel2->SetConstraint( pIcon2, REL_RIGHT,   pBox2, REL_RIGHT,   -5 );
	pRel2->SetConstraint( pList2, REL_MOVEL,   pBox2, REL_LEFT,     5 );
	pRel2->SetConstraint( pList2, REL_MOVET,   pBox2, REL_TOP,     15 );
	pRel2->SetConstraint( pList2, REL_RIGHT,   pBox2, REL_RIGHT,   -5 );
	pRel2->SetConstraint( pList2, REL_BOTTOM,  pIcon2, REL_TOP,	    0 );

	pRel2->SetInitialRect();
	
	pSplitNode->AddPane(1,0,pRel2);



	//  For the C pane in the picture, we have:
	//
	//  
	//       +---Group--------------------------+
	//       |                                  |
	//       |  +-----------------+  +--------+ | 
	//       |  |                 |  |        | |
	//       |  |                 |  |        | |
	//       |  |      List1      |  |  List2 | |
	//       |  |                 |  |        | |
	//       |  |                 |  |        | |
	//       |  |                 |  |        | |
	//       |  |                 |  +--------+ |
	//       |  |                 |             |
	//       |  |                 |  +--------+ |
	//       |  |                 |  | button | |
	//       |  +-----------------+  +--------+ |
	//       |                                  |
	//		 +----------------------------------+
	//
	//  And we have two of these, one below the other.
	//
	//  To handle having two, we can use a grid, 2 rows, 1 column.
	//
	//  For each of these items we can use a relative and make sure some things align.
	//  
	//  Well, it used to be a grid, decided instead to use a nested splitter.

	
	//SECLNGrid* pGrid = (SECLNGrid*) m_LFactory.CreateNode(NODE_CLASS(SECLNGrid));
	//pGrid->SetGridDimensions(0,1);

	SECLayoutNodeSplitter* pSplitNode1 
		= (SECLayoutNodeSplitter*) m_LFactory.CreateNode(NODE_CLASS(SECLayoutNodeSplitter));
	pSplitNode1->Create(this);
	//pSplitNode1->SetSplitterFlags(SEC_SPLITTER_REALTIME_DRAG);

	//  Create the top (group) relative node and populate.

	SECLNRelative* pGroupRel = (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));

	SECLayoutNodeWnd* pGroupGroup  = m_LFactory.CreateNodeWnd( IDC_STATIC_GROUP_GROUP,     this, pGroupRel );
	//SECLayoutNodeWnd* pGroupList1  = m_LFactory.CreateNodeWnd( IDC_LIST_GROUPS,            this, pGroupRel );
	SECLayoutNodeWnd* pGroupList1  = m_LFactory.CreateNodeWnd( IDC_LIST_GROUPS,            this, pGroupRel );
	SECLayoutNodeWnd* pGroupList2  = m_LFactory.CreateNodeWnd( &m_lbeGroupFields,		   this, pGroupRel );
	SECLayoutNodeWnd* pGroupButton = m_LFactory.CreateNodeWnd( IDC_BUTTON_UPDATE_GROUPING, this, pGroupRel );

	// space the box 5 pixels margin inside the parent
	pGroupRel->SetConstraint( pGroupGroup, REL_MOVET,  pGroupRel, REL_TOP,     5 );
	pGroupRel->SetConstraint( pGroupGroup, REL_BOTTOM, pGroupRel, REL_BOTTOM, -5 );
	pGroupRel->SetConstraint( pGroupGroup, REL_MOVEL,  pGroupRel, REL_LEFT,    5 );
	pGroupRel->SetConstraint( pGroupGroup, REL_RIGHT,  pGroupRel, REL_RIGHT,  -5 );

	//  move the button right and top to 5 pixels margin inside the group box.
	pGroupRel->SetConstraint( pGroupButton, REL_MOVEB, pGroupGroup, REL_BOTTOM, -5 );
	pGroupRel->SetConstraint( pGroupButton, REL_MOVER, pGroupGroup, REL_RIGHT,  -5 );


	//  move list2 to right and top, 5 pixels margin inside the group box
	//  also list2 attaches at bottom 5 pixels above the button
	//   and list2 attaches to buttons left side.

	pGroupRel->SetConstraint( pGroupList2, REL_MOVET,  pGroupGroup,  REL_TOP,   15 );
	pGroupRel->SetConstraint( pGroupList2, REL_MOVER,  pGroupGroup,  REL_RIGHT, -5 );
	pGroupRel->SetConstraint( pGroupList2, REL_BOTTOM, pGroupButton, REL_TOP,   -5 );
	pGroupRel->SetConstraint( pGroupList2, REL_LEFT,   pGroupButton, REL_LEFT,   0 );

	// Now deal with list1
	//     top --> list2 top
	//     left --> box left + 5
	//     right --> list2 left -5
	//	   bottom --> button bottom

	pGroupRel->SetConstraint( pGroupList1, REL_TOP,    pGroupList2,  REL_TOP,     0 );
	pGroupRel->SetConstraint( pGroupList1, REL_LEFT,   pGroupGroup,  REL_LEFT,    5 );
	pGroupRel->SetConstraint( pGroupList1, REL_RIGHT,  pGroupList2,  REL_LEFT,   -5 );
	pGroupRel->SetConstraint( pGroupList1, REL_BOTTOM, pGroupButton, REL_BOTTOM,  0 );

	pGroupRel->SetInitialRect();

	//pGrid->AddLayoutNode(pGroupRel);
	pSplitNode1->AddPane(0,0,pGroupRel);

	// and the same for sort/avail orders constellation

	SECLNRelative* pSortRel = (SECLNRelative*) m_LFactory.CreateNode(NODE_CLASS(SECLNRelative));

	SECLayoutNodeWnd* pSortGroup  = m_LFactory.CreateNodeWnd( IDC_STATIC_AVAIL_GROUP,    this, pSortRel );
	SECLayoutNodeWnd* pSortList1  = m_LFactory.CreateNodeWnd( IDC_LIST_ORDERS,           this, pSortRel );
	//SECLayoutNodeWnd* pSortList2  = m_LFactory.CreateNodeWnd( IDC_LIST_SORT_FIELDS,      this, pSortRel );
	SECLayoutNodeWnd* pSortList2  = m_LFactory.CreateNodeWnd( &m_lbeSortFields,		     this, pSortRel );
	SECLayoutNodeWnd* pSortButton = m_LFactory.CreateNodeWnd( IDC_BUTTON_UPDATE_SORTING, this, pSortRel );

	// space the box 5 pixels margin inside the parent
	pSortRel->SetConstraint( pSortGroup, REL_MOVET,  pSortRel, REL_TOP,     5 );
	pSortRel->SetConstraint( pSortGroup, REL_BOTTOM, pSortRel, REL_BOTTOM, -5 );
	pSortRel->SetConstraint( pSortGroup, REL_MOVEL,  pSortRel, REL_LEFT,    5 );
	pSortRel->SetConstraint( pSortGroup, REL_RIGHT,  pSortRel, REL_RIGHT,  -5 );

	//  move the button right and top to 5 pixels margin inside the group box.
	pSortRel->SetConstraint( pSortButton, REL_MOVEB, pSortGroup, REL_BOTTOM, -5 );
	pSortRel->SetConstraint( pSortButton, REL_MOVER, pSortGroup, REL_RIGHT,  -5 );


	//  move list2 to right and top, 5 pixels margin inside the group box
	//  also list2 attaches at bottom 5 pixels above the button
	//   and list2 attaches to buttons left side.

	pSortRel->SetConstraint( pSortList2, REL_MOVET,  pSortGroup,  REL_TOP,   15 );
	pSortRel->SetConstraint( pSortList2, REL_MOVER,  pSortGroup,  REL_RIGHT, -5 );
	pSortRel->SetConstraint( pSortList2, REL_BOTTOM, pSortButton, REL_TOP,   -5 );
	pSortRel->SetConstraint( pSortList2, REL_LEFT,   pSortButton, REL_LEFT,   0 );

	// Now deal with list1
	//     top --> list2 top
	//     left --> box left + 5
	//     right --> list2 left -5
	//	   bottom --> button bottom

	pSortRel->SetConstraint( pSortList1, REL_TOP,    pSortList2,  REL_TOP,     0 );
	pSortRel->SetConstraint( pSortList1, REL_LEFT,   pSortGroup,  REL_LEFT,    5 );
	pSortRel->SetConstraint( pSortList1, REL_RIGHT,  pSortList2,  REL_LEFT,   -5 );
	pSortRel->SetConstraint( pSortList1, REL_BOTTOM, pSortButton, REL_BOTTOM,  0 );

	pSortRel->SetInitialRect();

	//pGrid->AddLayoutNode(pSortRel);
	//pGrid->SetInitialRect();
	//pSplitNode->AddPane(0,1,pGrid);

	pSplitNode1->AddPane(1,0,pSortRel);
	pSplitNode->AddPane(0,1,pSplitNode1);

	pSplitNode->SetInitialRect();

	//  Attach the buttons to the left and top of the parent
	//  Attach the splitter node accordingly

	pTopNode->AddLayoutNode( pButtonsNode );
	pTopNode->AddLayoutNode( pSplitNode   );

	pTopNode->SetConstraint( pButtonsNode, REL_MOVEL,  pTopNode,     REL_LEFT,  5 );
	pTopNode->SetConstraint( pButtonsNode, REL_MOVET,  pTopNode,     REL_TOP,   5 );
	pTopNode->SetConstraint( pSplitNode,   REL_MOVEL,  pButtonsNode,  REL_RIGHT,    5 );
	pTopNode->SetConstraint( pSplitNode,   REL_RIGHT,  pTopNode,      REL_RIGHT,   -5 );
	pTopNode->SetConstraint( pSplitNode,   REL_MOVET,  pTopNode,      REL_TOP,      5 );
	pTopNode->SetConstraint( pSplitNode,   REL_BOTTOM, pTopNode,      REL_BOTTOM,  -5 );

	SECLayoutWndListener* pListener = m_LFactory.CreateLayoutWndListener();
	pListener->AutoInit(pTopNode,this);
}




void CCastStringEditorTextView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// Subclass the Insert Stock Order button to get a drag initiate notify.

	m_insertStockOrderButton.SubclassDlgItem(IDC_EDIT_INSERT_STOCK_ORDER,this);
	m_insertStockOrderButton.SetNotify(&m_insertStockOrderNotify);
	m_insertStockOrderNotify.SetView(this);


	m_insertOrderButton.SubclassDlgItem(IDC_ORDER_TO_INSERT,this);
	m_insertOrderButton.SetNotify(&m_insertOrderNotify);
	m_insertOrderNotify.SetView(this);


	//  Set up the SECListBoxEditor for the group/sort field lists
	m_lbeGroupFields.Initialize(this,
								IDC_LIST_GROUP_FIELDS,
								LBE_DRAGNDROP);
	m_lbeGroupFields.SetWindowText("Group fields");

	m_lbeSortFields.Initialize(this,
							   IDC_LIST_SORT_FIELDS,
							   LBE_DRAGNDROP);
	m_lbeSortFields.SetWindowText("Sort fields");

	//  Attach the SECBitmapButton
	m_secBtnOrderInquiry.AttachButton(ID_EDIT_ORDER_INQUIRY,
								SECBitmapButton::Al_Center,
								IDB_BITMAP_ORDER_INQUIRY,
								this);

	m_secBtnSave.AttachButton(IDC_BUTTON_SAVE,
							  SECBitmapButton::Al_Center,
							  IDB_SAVE,
							  this);

	m_secBtnUpload.AttachButton(IDC_BUTTON_UPLOAD,
								SECBitmapButton::Al_Center,
								IDB_UPLOAD,
								this);

	// Subclass the Editable and HeatsLocked buttons
	// and initialize bitmaps and states.

	m_btnEditable.SubclassDlgItem(IDC_BUTTON_EDITABLE,this);
	m_btnHeatsLocked.SubclassDlgItem(IDC_BUTTON_HEATS_LOCKED,this);

	{
		vector<int> editableIds;
		editableIds.push_back(IDB_LOCKED);
		editableIds.push_back(IDB_UNLOCKED);

		vector<int> lockedIds;
		lockedIds.push_back(IDB_UNLOCKED_HEATS);
		lockedIds.push_back(IDB_SEMILOCKED_HEATS);
		lockedIds.push_back(IDB_LOCKED_HEATS);

		m_btnEditable.SetBitmaps(editableIds);
		m_btnHeatsLocked.SetBitmaps(lockedIds);
	}


		
	//m_secBtnEditable.AttachButton(IDC_BUTTON_EDITABLE,
	//							  SECBitmapButton::Al_Center,
	//							  IDB_LOCKED,
	//							  this);

	//m_secBtnHeatsLocked.AttachButton(IDC_BUTTON_HEATS_LOCKED,
	//							 	 SECBitmapButton::Al_Center,
	//								 IDB_LOCKED_HEATS,
	//								 this);

	BOOL createOk =  m_imageList.Create(IDB_ORDER_IMAGE_LIST,7,0,RGB(192,192,192));
	assert(createOk);

	m_listOrdersEx.SubclassDlgItem(IDC_LIST_ORDERS,this);
	m_strand1OrderListEx.SubclassDlgItem(IDC_STRAND1_ORDER_LIST,this);
	m_strand2OrderListEx.SubclassDlgItem(IDC_STRAND2_ORDER_LIST,this);

	m_listGroupsEx.SubclassDlgItem(IDC_LIST_GROUPS,this);

	UpdateData(FALSE);

	m_listGroupsEx.SetImageList(&m_imageList,LVSIL_STATE);
	m_listOrdersEx.SetImageList(&m_imageList,LVSIL_STATE);
	m_strand1OrderListEx.SetImageList(&m_imageList,LVSIL_STATE);
	m_strand2OrderListEx.SetImageList(&m_imageList,LVSIL_STATE);

	InitLayout();

	m_VP.SetEditableBtn(&m_btnEditable);
	m_VP.SetHeatsLockedBtn(&m_btnHeatsLocked);
	m_VP.SetSaveBtn(&m_secBtnSave);
	m_VP.SetInsertStockOrderButton(&m_insertStockOrderButton);
	m_VP.SetInsertOrderButton(&m_insertOrderButton);
	m_VP.SetListOrdersEx(&m_listOrdersEx);
	m_VP.SetListGroupsEx(& m_listGroupsEx);
	m_VP.SetStrand1OrderListEx(&m_strand1OrderListEx);
	m_VP.SetStrand2OrderListEx(&m_strand2OrderListEx);
	m_VP.SetLbeGroupFields(&m_lbeGroupFields);
	m_VP.SetLbeSortFields(&m_lbeSortFields);


	m_VP.SetView(this);
	m_VP.OnInitialUpdate();	

}



//
//  Standard delegations to viewport
//

void CCastStringEditorTextView::OnDraw(CDC* pDC) 
{
	m_VP.Draw(pDC);	
}



// This is required to make the layout manager work.

void CCastStringEditorTextView::OnSize(UINT nType, int cx, int cy) 
{
	if ( cx > 0  &&  cy > 0 )
		SetScaleToFitSize(CSize(cx,cy));

	m_VP.SetOrigin(0,0);
	m_VP.SetSize(cx,cy);
	CFormView::OnSize(nType, cx, cy);
}


BOOL CCastStringEditorTextView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( m_VP.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo) )
		return TRUE;
	else
		return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CCastStringEditorTextView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( m_VP.OnWndMsg(message,wParam,lParam,pResult) )
		return TRUE;
	else
		return CFormView::OnWndMsg(message,wParam,lParam,pResult);
}





//  A local DDX (data exchange) function to transfer data between the fields list box
//    and three vectors (for the group/sort/avail lists).
//  THis is a static method

void AFXAPI CCastStringEditorTextView::DDX_GroupSortAvailFieldList(CDataExchange* pDX, 
																   int nIDC, 
																   CListBox& box, 
																   COrderSelection::T_SortVec& selectVec,
																   COrderSelection::T_SortVec& availVec)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);


	if ( pDX->m_bSaveAndValidate ) {

		//  move elements from control to the vectors.

		//  canonicalize position of marker elements, first.
		CanonicalizeFieldsListBox(box);

		selectVec.clear();
		availVec.clear();

		int apos = box.FindString( -1, sm_strAvail );
		int gpos = box.FindString( -1, sm_strGroup );
		int spos = box.FindString( -1, sm_strSort  );

		assert( apos != LB_ERR );
		//assert( gpos != LB_ERR );
		//assert( spos != LB_ERR );

		for ( int i = 0; i< box.GetCount(); ++i ) {

			CString item;

			box.GetText(i,item);

			COrderSelection::T_SortFieldStringMapInverse::const_iterator is 
				= COrderSelection::StringsToSortFields().find(item);

			if ( i == apos || i == gpos || i == spos ) {
				// we are on a marker -- do nothing
			}

			else if ( is == COrderSelection::StringsToSortFields().end() ) {
				// not found
				AfxMessageBox("Failed to find sort field string!");
				pDX->Fail();
			}
			else {

				//  we are not on a marker and a valid field string has been found.
				//  determine which group we are in based on position relative to the markers
				//  and add the field to the corresponding list.

				if ( i < gpos || i < spos)
					selectVec.push_back( (*is).second );
				else
					availVec.push_back( (*is).second );
			}
		}
	}
			
	else { // ! pDX->m_bSaveAndValidate

		// move elements from groupVec/sortVec/availVec to control

		box.ResetContent();

		if (! AddFieldsListContents(box,selectVec,sm_strGroup)
			||
			! AddFieldsListContents(box,availVec,sm_strAvail) ) {
	
			AfxMessageBox("Failed to find string for sort field!");
			pDX->Fail();
		}
	}
}




////////////////////////////////////////////////////////////////
//	
//	Field list
//	
////////////////////////////////////////////////////////////////

//  The group/sort/avail fields list box is used to indicate the fields used
//    for grouping and sorting available orders.
//  The items placed in the list box correspond to fields for which 
//    grouping and sorting can be done.
//  There are three additional items added to serve as markers for the boundaries
//    between group, sort, and avail fields.
//  Thus the list looks like:
//
//          group-field-1
//          group-field-2
//          ..
//          group-field-k
//          ^^^ GROUP ^^^
//          sort-field-1
//          sort-field-2
//          ...
//          sort-field-m
//          ^^^ SORT ^^^
//          avail-field-1
//          avail-field-2
//          ...
//			avail-field-n
//          ^^^ AVAIL ^^^
//
//	Each field has a string associated with it, which we use as the text
//    for the item associated with that field. This allows us to identify 
//	  which field is which after the user moves things around.
//
//  The only ordering requirement is that the GROUP/SORT/AVAIL marker items
//    appear in that order in the list.  Moreover, AVAIL must appear at the end.
//  Since it is possible for the user to move list items around arbitrariy, 
//    it is possible for these items to get out of order.  
//  This will be noticed during the validation process and automatically 
//    corrected by moving AVAIL to the end (if it is not there already)
//    and SORT to immediately follow GROUP (in case it precedes it) --
//    this is performed by CCastStringEditorTextView::CanonicalizeFieldsListBox
  
 
//  The strings to be used for the special marker items

const CString CCastStringEditorTextView::sm_strAvail = "^^^ AVAIL ^^^";
const CString CCastStringEditorTextView::sm_strGroup = "^^^ GROUP ^^^";
const CString CCastStringEditorTextView::sm_strSort  = "^^^ SORT  ^^^";


//  Ensure that the AVAIL marker is at the end,
//  and that SORT comes after GROUP.
//  This is a static method.

void CCastStringEditorTextView::CanonicalizeFieldsListBox(CListBox& box)
{
	int count = box.GetCount();

	assert( count > 0 );


	//  Move AVAIL marker to end, if not there already.

	int apos= box.FindString(-1,sm_strAvail);

	assert( apos != LB_ERR );

	if ( apos != count -1 ) {

		box.DeleteString(apos);
		box.AddString(sm_strAvail);

	}


	//  Make sure the SORT marker follows the GROUP marker

//	int gpos = box.FindString(-1,strGroup);
//	int spos = box.FindString(-1,strSort);
//
//	assert( gpos != LB_ERR );
//	assert( spos != LB_ERR );
//
//	if ( spos < gpos ) {
//		box.InsertString(gpos+1,strSort);
//		box.DeleteString(spos);
//	}

}



//	Auxiliary function used to populate the fields list box.
//		vec is one of the field lists (group or sort or avail),
//		str is the string used to mark this particular set of fields.
//	Add the strings corresponding to the fields in vec, then add str to the box.
//	This is a static method
	
bool CCastStringEditorTextView::AddFieldsListContents(CListBox& box,
													  COrderSelection::T_SortVec& vec,
													  const CString& str)
{

	for ( COrderSelection::T_SortVec::iterator is = vec.begin();
		  is != vec.end();
		  ++is ) {

		COrderSelection::T_SortFieldStringMap::const_iterator im
			= COrderSelection::SortFieldStrings().find(COrderSelection::E_SortField((*is)));

		if ( im == COrderSelection::SortFieldStrings().end() ) 
			return false;
		else
			box.AddString(LPCTSTR((*im).second));
	}
	
	box.AddString(str);

	return true;
}





////////////////////////////////////////////////////////////////
//	
//	Drag and drop
//	
////////////////////////////////////////////////////////////////

//  Several functionalities are available via drag-and-drop.
//
//  (a) From the available orders group list to (1) one of the strands
//                                              (2) the question mark icon
//
//  (b) From the group orders list to (1) one of the strands
//                                    (2) the question mark icon
//
//  (c) From a strand list to (1) itself
//                            (2) the other strand
//                            (3) the wastebasket icon
//                            (4) the question mark icon
//
//  The action associated with a drop are:
//
//     (a)(1) insert into strand (with dialog, if right click)
//        (2) display order info dialog (for first order)
//
//     (b) same as (a)
//        
//     (c)(1) move orders within strand
//        (2) move orders between strands
//        (3) delete orders from strand
//        (4) display order info dialog (for first order)
//
//
//	The methods associated with responding to a drag into the window are:
//
//		OnDragEnter*
//		OnDragOver*
//		OnDragLeave*
//		OnDrop*
//		CleanupDropEffect
//		SetDropEffect
//  
//		* = standard virtual function for handling this
//
//  The methods associated with initiating a drag from one of the lists:
//
//		OnBegindragStrand1List
//		OnBegindragStrand2List
//		OnBegindragListGroups
//		OnBegindragListOrders
//		OnBeginrdragListGroups
//		OnBeginrdragListOrders
//
//	The following instance variables hold the state regarding initiating a drag:
//
//
//		DragSource m_dragSource;
//				Indicates where the drag of interest is coming from.
//				one of	SOURCE_STRAND
//						SOURCE_GROUP
//						SOURCE_GROUP_ORDER
//						SOURCE_STOCK_ORDER
//						SOURCE_OTHER
//
//		int m_sourceStrand
//				Of interest only if m_dragSource == SOURCE_STRAND
//              value is 1 or 2, indicating which strand list is the source
//
//		CListCtrl* m_pSourceList
//				of interest only when m_dragSource != SOURCE_OTHER.
//				points to the list which is the source
//
//		vector<int> m_selectedCSOrderIndexes;
//				Of interest only if m_dragSource == SOURCE_STRAND
///				indicates which items in the source list were selected when the drag began.
//
//
//		vector<COrder*> m_selectedOrders;
//				Of interest only if m_dragSource == SOURCE_GROUP or SOURCE_GROUP_ORDER
//				indicates which orders in the source list were selected when the drag began
//				
//
//
//	The following variables hold the state when a drag is active over the window:
//
//
//		bool m_bDragActive;
//				Indicates that a drag into the window from a source of interest is occuring.
//				Set by OnDragEnter.
//				Reset by OnDragLeave, OnDrop
//				Referenced by OnDragOver.
//	
//		bool m_bDropEffect;
//				Indicates that a drop effect (a visual manifestation that a drop will succeed,
//				perhaps indicating the location where the drop will happen) is in effect.
//				Maintained by SetDropEffect, CleanupDropEffect.
//				The primary drop effect is highlighting in the strand lists to indicate where a 
//				a drop will occur.
//
//		DropTarget m_dropTarget;
//				Indicates the current target if the drag is dropped
//				one of	TARGET_STRAND
//						TARGET_STRAND_END
//						TARGET_WASTEBASKET
//						TARGET_QUESTION
//						TARGET_OTHER
//		
//		int m_targetStrand
//				Of interest only if m_dropTarget == TARGET_STRAND(_END)
//				value is 1 or 2 indicating which strand list is the target
//
//		int m_targetItem
//				Of interest only if m_dropTarget == TARGET_STRAND(_END)
//				value is the index of the item before which insertion will take place
//				      is -1 if insertion is to occur at the end 
//				
//		CListCtrl* m_pTargetList
//				of interest only when m_dropTarget = TARGET_STRAND(_END)
//				points to the strand list which is the target.
//


DROPEFFECT CCastStringEditorTextView::OnDragEnter(COleDataObject* pDataObject, 
												  DWORD dwKeyState, 
												  CPoint point) 
{
	// Drag should not yet be active.

	assert( ! m_bDragActive );



	
	//  Check to see if the drag is of a format of interest.

	if ( pDataObject->IsDataAvailable(CClipFormat::AvailOrderOrderFormat()) )
		m_dragSource = SOURCE_GROUP_ORDER;
	else if ( pDataObject->IsDataAvailable(CClipFormat::AvailOrderGroupFormat()) )
		m_dragSource = SOURCE_GROUP;
	else if ( pDataObject->IsDataAvailable(CClipFormat::CasterScenOrderFormat()) )
		m_dragSource = SOURCE_STRAND;
	else if ( pDataObject->IsDataAvailable(CClipFormat::InsertStockOrderFormat()) )
		m_dragSource = SOURCE_STOCK_ORDER;
	else if ( pDataObject->IsDataAvailable(CClipFormat::InsertOrderFormat()) )
		m_dragSource = SOURCE_INSERT_ORDER;
	else
		m_dragSource = SOURCE_OTHER;

	// set a few of the state variables to nominal values
	
	m_dropTarget	= TARGET_OTHER;
	m_targetStrand	=  0;
	m_targetItem	= -1;
	m_pTargetList	= 0;
	m_bRightButton	= (m_dragSource == SOURCE_INSERT_ORDER)
					  ||
					  ((dwKeyState & MK_RBUTTON) != 0);
	// in case of INSERT_ORDER, we want to force the user to input info,
	// so we always treat it as a right button drag.

#if 0
	afxDump << "CSETV::OnDragEnter, source = "
			<< ( m_dragSource == SOURCE_STRAND 
			    ? "STRAND"
				: (m_dragSource == SOURCE_GROUP
				   ? "GROUP"
				   : (m_dragSource == SOURCE_GROUP_ORDER
				      ? "ORDER"
					  : "OTHER")))
			<< "\n";
#endif
	
	if ( m_dragSource == SOURCE_OTHER ) {

		// we are not interested -- this drag comes from somewhere unknown

		m_bDragActive = false;
		m_bDropEffect = false;


		return DROPEFFECT_NONE;
	}


	// we are interested

	m_bDragActive = true;
	m_bDropEffect = false;

	return OnDragOver(pDataObject,dwKeyState,point);
}





void CCastStringEditorTextView::OnDragLeave() 
{
#if 0
	afxDump << "CSETV::OnDragLeave\n";
#endif

	CleanupDropEffect();
	m_bDragActive = false;
}




DROPEFFECT CCastStringEditorTextView::OnDragOver(COleDataObject* pDataObject, 
												 DWORD dwKeyState, 
												 CPoint point) 
{
	//  Check to make sure we are interested

	if ( ! m_bDragActive ) {

		return DROPEFFECT_NONE;
	}



	//  Determine which window contains the point

	CWnd* pWnd = ChildWindowFromPoint(point);
	HWND hwnd = pWnd->GetSafeHwnd();

	DropTarget nextTarget;
	int nextStrand;
	CListCtrl* pNextList;
	
	if ( hwnd == m_strand1OrderListEx.m_hWnd ) {
		nextTarget = TARGET_STRAND;
		nextStrand = 1;
		pNextList  = &m_strand1OrderListEx;
	}
	else if ( hwnd == m_strand2OrderListEx.m_hWnd ) {
		nextTarget = TARGET_STRAND;
		nextStrand = 2;
		pNextList  = &m_strand2OrderListEx;
	}
	else if ( hwnd == m_iconStrand1End.m_hWnd ) {
		nextTarget = TARGET_STRAND_END;
		nextStrand = 1;
		pNextList  = &m_strand1OrderListEx;
	}
	else if ( hwnd == m_iconStrand2End.m_hWnd ) {
		nextTarget = TARGET_STRAND_END;
		nextStrand = 2;
		pNextList  = &m_strand2OrderListEx;
	}
	else if ( hwnd == m_iconWastebasket.m_hWnd ) {
		nextTarget = TARGET_WASTEBASKET;
		nextStrand = 0;
		pNextList  = 0;
	}
	else if ( hwnd == m_secBtnOrderInquiry.m_hWnd ) {
		nextTarget = TARGET_QUESTION;
		nextStrand = 0;
		pNextList  = 0;
	}
	else {
		nextTarget = TARGET_OTHER;
		nextStrand = 0;
		pNextList  = 0;
	}

	
	//  if we are on a strand, find out which item we are over in the list.

	int nextItem	= -1;
	UINT flags		= 0;


	if ( nextTarget == TARGET_STRAND ) {

		//  Coordinates arrive in client coords
		//  We convert to screen coords relative to the window

		CPoint spt = point;
		ClientToScreen(&spt);
	
		// then convert to client coordinates in the list
		pNextList->ScreenToClient(&spt);

		// then find out where we are in the list
		nextItem = pNextList->HitTest(spt,&flags);
	}


#if 0
	afxDump << "CSETV::OnDragOver, target = "
			<< (nextTarget == TARGET_STRAND
				? "STRAND"
				: (nextTarget == TARGET_WASTEBASKET
					? "WASTE"
					: (nextTarget == TARGET_QUESTION
						? "INFO "
						: "OTHER")));
	if ( nextTarget == TARGET_STRAND ) {
		afxDump << ", strand = " << nextStrand 
				<< ", item = " << nextItem
				<< ", flags = ";
		if ( flags & LVHT_ABOVE				) afxDump << "ABOVE   ";
		if ( flags & LVHT_BELOW				) afxDump << "BELOW   ";
		if ( flags & LVHT_NOWHERE           ) afxDump << "NOWHERE ";
		if ( flags & LVHT_ONITEMICON		) afxDump << "IICON   ";
		if ( flags & LVHT_ONITEMLABEL		) afxDump << "ILABEL  ";
		if ( flags & LVHT_ONITEMSTATEICON	) afxDump << "ISTATEI ";
		if ( flags & LVHT_TOLEFT			) afxDump << "LEFT    ";
		if ( flags & LVHT_TORIGHT			) afxDump << "RIGHT   ";
	}
#endif


	//  Determine if there has been a change in target
	//  The target is identified by 
	//     (old) m_dropTarget, m_targetStrand, m_targetItem
	//     (new)   nextTarget,     nextStrand,     nextItem
	//
	//  Note that m_targetStrand(nextTarget) and m_targetItem(nextItem)
	//  are relevant only if m_dropTarget == TARGET_STRAND(_END).
	//  However, in case m_dropTarget != TARGET_STRAND(_END), 
	//  these variables will have been set to default values.
	//  Thus, we can compare all three variables to determine change
	//  Note, if we are scrolling (above or below the list), then
	//     we want to do SetDropEffect to cause a scrolling action.

	if ( nextTarget == m_dropTarget
		 &&
		 nextStrand == m_targetStrand
		 &&
		 nextItem == m_targetItem 
		 &&
		 ((flags & (LVHT_ABOVE|LVHT_BELOW)) == 0) ) {

		// no change, so no need to update the drop effect
		// However, we still must compute the correct DROPEFFECT return value.

#if 0
		afxDump << " -- no change\n";
#endif

		return ComputeDROPEFFECT();
	}

	else {

#if 0
		afxDump << " -- change\n";
#endif

		//  change in current position
		//  undo (if necessary) current highlight effect
		CleanupDropEffect();

		// Set drop effect
		return SetDropEffect(nextTarget,nextStrand,nextItem,pNextList,flags);
	}
}


DROPEFFECT CCastStringEditorTextView::ComputeDROPEFFECT()
{
	DROPEFFECT val;

	CCastStringEditorDoc* pDoc = GetDocument();


	CCastString* pString = (m_VP.IsEditing() 
							? m_VP.CurrentCastString()
							: 0);

	

	switch ( m_dropTarget ) {
	case TARGET_STRAND:
	case TARGET_STRAND_END:
		val = ( ! pString->CanEdit()
			    ? DROPEFFECT_NONE
				: (m_dragSource == SOURCE_STRAND 
				   ? DROPEFFECT_MOVE
				   : DROPEFFECT_COPY));
		break;

	case TARGET_WASTEBASKET:
		val = (m_dragSource == SOURCE_STRAND
			   ? DROPEFFECT_MOVE
			   : DROPEFFECT_NONE);
		break;

	case TARGET_QUESTION:
		val = DROPEFFECT_COPY;
		break;

	case TARGET_OTHER:
		val = DROPEFFECT_NONE;
		break;
	}

	return val;
}



BOOL CCastStringEditorTextView::OnDrop(COleDataObject* pDataObject, 
									   DROPEFFECT dropEffect, 
									   CPoint point) 
{
	assert( m_bDragActive );

	// we need to clean up the drag/drop stuff before the document causes a refresh
	// first we need to cache the critcal values.
	DropTarget dropTarget	= m_dropTarget;
	int targetStrand		= m_targetStrand;
	int targetItem			= m_targetItem;

	int heatIndex  = -1;
	int orderIndex = -1;

	if ( m_pTargetList != 0 )
		//DecodeStrandListItem(m_pTargetList,m_targetItem,heatIndex,orderIndex);
		m_VP.DecodeStrandListItem(targetStrand,m_targetItem,heatIndex,orderIndex);

	CleanupDropEffect();	

	m_bDragActive = false;

	switch ( m_dragSource ) {

	case SOURCE_STRAND:

		switch ( m_dropTarget ) {
		case TARGET_STRAND:
		case TARGET_STRAND_END:
			m_VP.MoveOrders(m_selectedCSOrderIndexes,
									  m_sourceStrand,
									  targetStrand,
									  heatIndex,
									  orderIndex);
			break;
		case TARGET_WASTEBASKET:
			m_VP.DeleteOrders(m_selectedCSOrderIndexes,
										m_sourceStrand);
			break;
		case TARGET_QUESTION:
			m_VP.DoOrderInquiry(m_selectedCSOrderIndexes,
										  m_sourceStrand);
			break;
		case TARGET_OTHER:
			// this shouldn't happen
			assert(0);
			break;
		}			
		break;

	case SOURCE_GROUP:
	case SOURCE_GROUP_ORDER:
	case SOURCE_INSERT_ORDER:

		switch ( m_dropTarget ) {
		case TARGET_STRAND:
		case TARGET_STRAND_END:
			
			if ( targetItem == -1 )
				m_VP.AddOrders(m_selectedOrders,
										 targetStrand,
										 m_bRightButton);
			else
				m_VP.InsertOrders(m_selectedOrders,
											targetStrand,
											heatIndex,
											orderIndex,
											m_bRightButton);
			break;
		case TARGET_QUESTION:
			m_VP.DoOrderInquiry(m_selectedOrders);
			break;
		case TARGET_WASTEBASKET:
		case TARGET_OTHER:
			// this shouldn't happen
			assert(0);
			break;
		}			
		break;

	case SOURCE_STOCK_ORDER:
		
		switch ( m_dropTarget ) {

		case TARGET_STRAND:
		case TARGET_STRAND_END:
			if ( targetItem == -1 )
				m_VP.AddStockOrder(targetStrand);
			else
				m_VP.InsertStockOrder(targetStrand,
												heatIndex,
												orderIndex);
			break;

		default:
			break;
		}
		break;


	case SOURCE_OTHER:
		// this shouldn't happen
		assert(0);
		break;
	}
	
	return true;
}


DROPEFFECT CCastStringEditorTextView::SetDropEffect(CCastStringEditorTextView::DropTarget nextTarget,
													int nextStrand,
													int nextItem,
													CListCtrl* pNextList,
													UINT flags)
{
	m_dropTarget	= nextTarget;
	m_targetStrand	= nextStrand;
	m_targetItem	= nextItem;
	m_pTargetList	= pNextList;

	if ( nextTarget == TARGET_STRAND ) {
		
		if ( nextItem == -1 ) { 
			
			if ( flags & LVHT_ABOVE ) {
				// try to scroll the item above the top to be visible
				if ( m_pTargetList->GetTopIndex() > 0 )
					m_pTargetList->EnsureVisible( m_pTargetList->GetTopIndex() - 1,
												 false );
				return DROPEFFECT_NONE;
			}
			else if ( flags & LVHT_BELOW ) {
			
				// try to scroll up so the item below the bottom is visible
				if ( m_pTargetList->GetTopIndex() + m_pTargetList->GetCountPerPage()
					 <=
					 m_pTargetList->GetItemCount() )
					 m_pTargetList->EnsureVisible(m_pTargetList->GetTopIndex() 
													+ m_pTargetList->GetCountPerPage(),
												  false);
				
				return DROPEFFECT_NONE;
			}
		}
		else {
			// we are on a strand list, and over an item
			pNextList->SetItemState(nextItem,LVIS_DROPHILITED,LVIS_DROPHILITED);
			pNextList->RedrawItems(nextItem,nextItem);
			pNextList->UpdateWindow();

			m_bDropEffect = true;
		}
	}

	return ComputeDROPEFFECT();
}


void CCastStringEditorTextView::CleanupDropEffect()
{
	// we only need to clean up the drop effect if we are
	// over an item in one of the strand lists.
	//  then, we must clean up the hilite.

	if ( m_bDropEffect ) {

		assert ( m_targetItem != -1 );
		assert ( m_pTargetList != 0 );


		m_pTargetList->SetItemState(m_targetItem,0,LVIS_DROPHILITED);
		m_pTargetList->RedrawItems(m_targetItem,m_targetItem);
		m_pTargetList->UpdateWindow();
	}

	m_bDropEffect	= false;
}



////////////////////////////////////////////////////////////////
//	
//	Initiating drags
//	
////////////////////////////////////////////////////////////////


void CCastStringEditorTextView::OnBegindragStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_dragSource = SOURCE_STRAND;
	m_sourceStrand = 1;
	m_pSourceList = &m_strand1OrderListEx;

	m_VP.GetSelectedCSOrderIndexes(m_selectedCSOrderIndexes,m_pSourceList,m_sourceStrand);

#if 0
	afxDump << "Start drag: STRAND 1\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::CasterScenOrderFormat());
	source.DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: STRAND 1\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;

	*pResult = 0;
}


void CCastStringEditorTextView::OnBegindragStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_dragSource = SOURCE_STRAND;
	m_sourceStrand = 2;
	m_pSourceList = &m_strand2OrderListEx;

	m_VP.GetSelectedCSOrderIndexes(m_selectedCSOrderIndexes,m_pSourceList,m_sourceStrand);

#if 0
	afxDump << "Start drag: STRAND 2\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::CasterScenOrderFormat());
	source.DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: STRAND 2\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;

	*pResult = 0;
}


void CCastStringEditorTextView::OnBegindragListGroups(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_dragSource = SOURCE_GROUP;
	m_sourceStrand = -1;
	m_pSourceList = &m_listGroupsEx;

	m_VP.GetOrdersForSelectedGroups(m_selectedOrders);

#if 0
	afxDump << "Start drag: GROUP\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::AvailOrderGroupFormat());
	source.DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: GROUP\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;


	*pResult = 0;
}

void CCastStringEditorTextView::OnBegindragListOrders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_dragSource = SOURCE_GROUP_ORDER;
	m_sourceStrand = -1;
	m_pSourceList = &m_listOrdersEx;

	m_VP.GetSelectedOrders(m_selectedOrders);

#if 0
	afxDump << "Start drag: ORDER\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::AvailOrderOrderFormat());
	source.DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: ORDER\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;

	*pResult = 0;
}



void CCastStringEditorTextView::OnBeginrdragListGroups(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	OnBegindragListGroups(pNMHDR,pResult);

	*pResult = 0;
}

void CCastStringEditorTextView::OnBeginrdragListOrders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	OnBegindragListOrders(pNMHDR,pResult);
	
	*pResult = 0;
}



void CCastStringEditorTextView::BeginInsertStockOrder()
{
	m_dragSource = SOURCE_STOCK_ORDER;
	m_sourceStrand = -1;
	m_pSourceList = 0;


#if 0
	afxDump << "Start drag: STOCK\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::InsertStockOrderFormat());
	source.DoDragDrop(DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: STOCK\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;
}





void CCastStringEditorTextView::BeginInsertOrder() 
{
	if ( m_VP.OrderForInsert() == 0 )
		return;


	m_dragSource = SOURCE_INSERT_ORDER;
	m_sourceStrand = -1;
	m_pSourceList = 0;

	m_selectedOrders.clear();
	m_selectedOrders.push_back(m_VP.OrderForInsert());

#if 0
	afxDump << "Start drag: INSERT_ORDER\n";
#endif

	COleDataSource source;
	source.DelayRenderData(CClipFormat::InsertOrderFormat());
	source.DoDragDrop(DROPEFFECT_COPY);	

#if 0
	afxDump << "End drag: INSERT_ORDER\n";
#endif

	m_dragSource = SOURCE_OTHER;
	m_sourceStrand = -1;
	m_pSourceList = 0;
}


#if 0
static void ProcessMessages()
{
	
    MSG msg;
    while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
    { 
        if ( !AfxGetApp()->PumpMessage( ) ) 
        { 
            ::PostQuitMessage(0); 
            break; 
        } 
    } 
    // let MFC do its idle processing
    LONG lIdle = 0;
    while ( AfxGetApp()->OnIdle(lIdle++ ) )
		;
}

#endif




#if 0
// we are not currently using virtual lists here,
// because they do not support state images and DROPHILITEing.

void CCastStringEditorTextView::SetStrandOrderList(CCastString* pString,
												   int strandNum, 
												   CListCtrl& listCtrl,
												   bool maintainPosition)
{
	CMaintainListCtrlPosn x( listCtrl, maintainPosition );
	SetGroupListHeader();

	if ( pString == 0 )
		listCtrl.SetItemCount(0);
	else
		listCtrl.SetItemCount( pString->Heats().size() + pString->Strand(strandNum).size() );

	listCtrl.UpdateWindow();
}





void CCastStringEditorTextView::OnGetdispinfoStrand1OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	DoGetdispinfoStrandOrderList(pDispInfo,1);
	
	*pResult = 0;
}

void CCastStringEditorTextView::OnGetdispinfoStrand2OrderList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	DoGetdispinfoStrandOrderList(pDispInfo,2);
	
	*pResult = 0;
}


void CCastStringEditorTextView::DoGetdispinfoStrandOrderList(LV_DISPINFO* pDispInfo,
															 int strandNum)
{
	CCastStringEditorDoc* pDoc = GetDocument();

	CCastString* pString = (pDoc->IsEditing() 
							? pDoc->GetCurrentCastString()
							: 0);

	if ( pString == 0 )
		return;

	LVITEM& lvi = pDispInfo->item;

	int heatIndex;
	int orderIndex;

	DecodeStrandListItem(strandNum,lvi.iItem,heatIndex,orderIndex);
	
	if ( heatIndex != -1 )
		DoGetdispinfoStrandOrderListItem( lvi, pString->Heats()[heatIndex], heatIndex );
	else if ( orderIndex != -1 )
		DoGetdispinfoStrandOrderListItem( lvi, pString->Strand(strandNum)[orderIndex] );
}



void CCastStringEditorTextView::DoGetdispinfoStrandOrderListItem(LVITEM& lvi,
																 CCSOrder* pCSOrder )
{
	if (  lvi.mask & LVIF_TEXT ) {

		char buf[100];
		ostrstream ostr(buf,100);


		switch ( lvi.iSubItem ) {

		case 0:	
			// Order#
			sprintf(buf,"%s%07d",
					( pCSOrder->OrderNum()/10000%10 == 0 // 4th digit
					  ? ">>>"
					  : "   "),
					pCSOrder->OrderNum());
			break;

		case 1:
			// changeFlag
			ostr << pCSOrder->ChangeFlag() << ends;
			break;

		case 2:
			// Heat#
			ostr << pCSOrder->HeatSeqNum()+1 << ends;
			break;

		case 3:
			// Spec
			{
				const CString& spec = (pCSOrder->Order() == 0 
								       ? pCSOrder->LotSpec()
									   : pCSOrder->Order()->OrderedSpec());

				CString rightDig = spec.Right(1);

				if ( rightDig == "2" || rightDig == "5" )
					ostr << "* ";
				
				ostr << LPCTSTR(spec.Left(3))
					<< "-"
					<< LPCTSTR(spec.Mid(3,2))
					<< "-"
					<< LPCTSTR(spec.Right(2))
					<< ends;
			}
			break;

		case 4:
			// HB Width
			if ( pCSOrder->Order() == 0 )
				sprintf(buf,"?");
			else
				sprintf(buf,"%4.1f",pCSOrder->Order()->HbWidth());
			break;

		case 5:
			// Slab width
			ostr << long(pCSOrder->SlabWidth()) << ends;
			break;

		case 6:
			// Condn code
			ostr << pCSOrder->SlabCondnCode() << ends;
			break;

		case 7:
			// Disp/Slit
			sprintf(buf,"%d-%c",
					pCSOrder->DispCode(),
					(pCSOrder->SlitTypeCode() == ' ' 
					 ? '_'
					 : pCSOrder->SlitTypeCode()));
			break;

		case 8:
			// Col  8 -- numPieces
			ostr << pCSOrder->NumPieces() << ends;
			break;

		case 9:
			// Tons
			ostr << long(pCSOrder->Tons()) << ends;
			break;

		case 10:
			// priority
			if ( pCSOrder->Order() == 0 )
				sprintf(buf,"?");
			else
				sprintf(buf,"%s%2d-%2d",
						(pCSOrder->Order()->CommodityPriority() == 99 ? ">>>" : "   "),
					    pCSOrder->Order()->CommodityPriority(),
						pCSOrder->Order()->OrderPriority());
			break;

		case 11:
			// Slab length
			ostr << long(pCSOrder->SlabLength()) << ends;
			break;

		case 12:
			// HR Week
			if ( pCSOrder->Order() == 0 )
				sprintf(buf,"?");
			else
				ostr << pCSOrder->Order()->HrYrWk().Week() << ends;
			break;

		case 13:
			// CI Code
			if ( pCSOrder->Order() == 0 )
				sprintf(buf,"?");
			else
				ostr << pCSOrder->Order()->CICode() << ends;
			break;

		case 14:
			// StartTime
			if ( pCSOrder->Order() == 0 )
				sprintf(buf,"?");
			else
				sprintf(buf,"%5d",
					    NTime::PSDateDelta(pCSOrder->Order()->PlannedLatestStartDate()));
			break;

		case 15:
		case 16:
		case 17:
			// 15 = carbon
			// 16 = manganese
			// 17 = signif el
			// these are close enough that we try to share code
			{
				CString prefix;
				CChem::Bound minVal;
				CChem::Bound maxVal;
				bool eltOk;
				bool hasReasonCode;

				switch ( lvi.iSubItem ) {
				case 15:
					eltOk = pCSOrder->GetChemRange( CChem::ELT_C,  minVal,  maxVal, hasReasonCode );
					prefix = "";
					break;
				case 16:
					eltOk = pCSOrder->GetChemRange( CChem::ELT_MN, minVal,  maxVal, hasReasonCode );
					prefix = "";
					break;
				case 17:
					if ( pCSOrder->Order() == 0 ) {
						minVal =   0.0;
						maxVal = 100.0;
						prefix = "";
						eltOk  = false;
					}
					else {
						minVal = pCSOrder->Order()->MinSignifEl();
						maxVal = pCSOrder->Order()->MaxSignifEl();
						prefix.Format("%s: ",LPCTSTR(pCSOrder->Order()->SignifElSym()));
						eltOk  = minVal != 0.0 || maxVal != 100.0;
					}
					break;
				}

				if ( eltOk ) 
					sprintf(buf,"%s%5.3f-%5.3f",LPCTSTR(prefix),minVal,maxVal);
				else
					sprintf(buf,"-");
			}
			break;

		default:
			sprintf(buf,"!$%?^@*");
			break;
		}
	
		//listCtrl.SetItemData(itemNum,EncodeOrderIndex(orderCount));
		
		lstrcpyn(lvi.pszText,buf,lvi.cchTextMax);
	}
	
	if ( lvi.iSubItem == 0 ) {

		if ( lvi.mask & LVIF_IMAGE ) {
			lvi.iImage = INDEXTOSTATEIMAGEMASK(pCSOrder->ChangeMarked() ? 2 : 1);
			lvi.state |= LVIS_STATEIMAGEMASK;
			lvi.stateMask |= LVIS_STATEIMAGEMASK;

			//listCtrl.SetItemState(itemNum,
			//						INDEXTOSTATEIMAGEMASK( pCSOrder->ChangeMarked() ? 2 : 1 ), 
			//						LVIS_STATEIMAGEMASK);
		}

		if ( lvi.mask & LVIF_INDENT ) {
			lvi.iIndent = 0;
		}
		if ( lvi.mask & LVIF_STATE ) {
			afxDump << "State\n";
		}

		lvi.state |= LVIS_DROPHILITED;
		lvi.stateMask |= LVIS_DROPHILITED;
	}
}
	


// This one's for heats.

void CCastStringEditorTextView::DoGetdispinfoStrandOrderListItem(LVITEM& lvi,
																 CCastStringHeat& rHeat,
																 int heatCount)
{


	if (  lvi.mask & LVIF_TEXT ) {

		char buf[100];
		ostrstream ostr(buf,100);


		switch ( lvi.iSubItem ) {

		case 0:	
			// This is order# for orders, we just put the text "Heat"
			strcpy(buf,"Heat");
			break;
	
		case 1:
			// This is changeFlag for orders, leave blank
			strcpy(buf,"");
			break;

		case 2:
			// Heat#
			ostr << heatCount+1 << ends;
			break;

		case 3:
			// heat spec
			{
				CString spec = rHeat.Spec();

				if ( spec.GetLength() < 7 )
					spec = CString(spec + "       ").Left(7);
				
				CString rightDig = spec.Right(1);

				ostr << ( rightDig == "2" || rightDig == "5" ? ">>>" : "   ")
					 << LPCTSTR(spec.Left(3))
					 << "-"
					 << LPCTSTR(spec.Mid(3,2))
					 << "-"
					 << LPCTSTR(spec.Right(2))
					 << ends;
			}
			break;

		// TODO - compute beginning, end slab width
		//                total # pieces on strand
		//				  total tons on strand

		default:
			strcpy(buf,"");
			break;
		}
	
		//listCtrl.SetItemData(itemNum,EncodeHeatIndex(heatCount));
		
		lstrcpyn(lvi.pszText,buf,lvi.cchTextMax);
	}
	
	if ( lvi.iSubItem == 0 ) {

		if ( lvi.mask & LVIF_IMAGE ) {
			lvi.iImage = 0;
		}

		if ( lvi.mask & LVIF_INDENT ) {
			lvi.iIndent = 0;
		}
	}
}


// we encode the index of the heat (in the vector of heats)
//        or the index of the order (in the strand vector
// and place the code in the data element of the list item.
// The encoding is   code = orderIndex
//                   code = heatIndex + 0x8000
// (i.e., we set the high bit on a heatIndex).

// We could make these static methods on CCastStringEditorTextView.
// At the time, I was in a hurry and didn't want a long recompile.

#if 0
static void DecodeStrandListItem(CListCtrl* pList, 
								 int listIndex, 
								 int& heatIndex, 
								 int& orderIndex)
{
	if ( listIndex < 0 || listIndex >= pList->GetItemCount() ) {
		orderIndex = -1;
		heatIndex = -1;
	}
	else {

		DWORD datum = pList->GetItemData(listIndex);
		if ( datum >= 0x8000 ) {
			orderIndex	= -1;
			heatIndex	= datum - 0x8000;
		}
		else {
			orderIndex	= datum;
			heatIndex	= -1;
		}
	}
}


static DWORD EncodeHeatIndex(int heatIndex)
{
	return heatIndex + 0x8000;
}

static DWORD EncodeOrderIndex(int orderIndex)
{
	return DWORD(orderIndex);
}

#endif


#if 0
void CCastStringEditorTextView::SetGroupListItems(bool maintainPosition)
{
	CMaintainListCtrlPosn x( m_listGroupsEx,maintainPosition);

	//int posn1 = m_listGroupsEx.GetTopIndex();
	//int posn2 = posn1 + m_listGroupsEx.GetCountPerPage() - 1;


	char buf[50];
	ostrstream ostr;

	ostr.setf(ios_base:: fixed, ios_base:: floatfield);

	int grpCount = 0;

	//m_listGroupsEx.ShowWindow(SW_HIDE);

	//m_listGroupsEx.DeleteAllItems();

	//m_listGroupsEx.SetRedraw(FALSE);


	SetGroupListHeader();

	if ( GetOrderSelection() == 0 )
		return;

	m_listGroupsEx.SetItemCount(GetOrderSelection()->Groups().size());


#if 0
	int itemCount = 1;

	for ( COrderSelection::GroupVec::const_iterator ig = GetOrderSelection()->Groups().begin();
		  ig != GetOrderSelection()->Groups().end();
		  ++ig, ++grpCount ) {


		//if ( grpCount % 50 == 0 )
		//	ProcessMessages();


		int count = 0;
	

		for ( COrderSelection::SortVec::const_iterator is = GetOrderSelection()->GroupFields().begin();
			  is != GetOrderSelection()->GroupFields().end();
			  ++is, ++count, ++itemCount ) {


			COrder* pOrder = *((*ig)->GroupBegin());

			switch ( (*is) ) {

			case COrderSelection::SORT_CI:
				SetItem(m_listGroupsEx,grpCount,count,_itoa(pOrder->CICode(),buf,10));
				break;

			case COrderSelection::SORT_SPEC:
			
				{
					CString rightDig = pOrder->CastSpec().Right(1);

					ostr << ( rightDig == "2" || rightDig == "5" ? ">>>" : "   ")
						 << LPCTSTR(pOrder->CastSpec().Left(3))
						 << "-"
						 << LPCTSTR(pOrder->CastSpec().Mid(3,2))
						 << "-"
						 << LPCTSTR(pOrder->CastSpec().Right(2))
						 << ends;
				}

				SetItem(m_listGroupsEx,grpCount,count,ostr.str());
				ostr.freeze(false);
				ostr.seekp(0);
				break;

			case COrderSelection::SORT_WIDTH:

//				ostr << setw(2) << long(pOrder->SlabWidth()) << ends;   

				ostr << setw(1) << double(pOrder->SlabWidth()) << ends;  // testing 2-09-09 k. hubbard 
				SetItem(m_listGroupsEx,grpCount,count,ostr.str());
				ostr.freeze(false);
				ostr.seekp(0);
				break;

			case COrderSelection::SORT_SLAB_LENGTH:

				ostr << setw(3) << long(pOrder->SlabLength()) << ends;
				SetItem(m_listGroupsEx,grpCount,count,ostr.str());
				ostr.freeze(0);
				ostr.seekp(0);
				break;

			case COrderSelection::SORT_80HSM_LU_TYPE:

				SetItem(m_listGroupsEx,grpCount,count,pOrder->MegaLuCd());
				break;

			case COrderSelection::SORT_CUSTOMER:

				SetItem(m_listGroupsEx,grpCount,count,pOrder->CustName());
				break;
	
			case COrderSelection::SORT_PRIORITY_CODE:
				
				ostr.fill('0');
				ostr << ( pOrder->CommodityPriority() == 99 ? ">>>" : "   ")
					 << setw(2) << pOrder->CommodityPriority()
					 << "-"
					 << pOrder->OrderPriority()
					 << ends;
				SetItem(m_listGroupsEx,grpCount,count,ostr.str());
				ostr.freeze(false);
				ostr.seekp(0);
				ostr.fill(' ');
				break;
	
			case COrderSelection::SORT_CONDN_CODE2:

				SetItem(m_listGroupsEx,grpCount,count,_ltoa(long(pOrder->SlabCondnCode()/100),buf,10));
				break;

			case COrderSelection::SORT_HR_WEEK:

				SetItem(m_listGroupsEx,grpCount,count,_ltoa(long(pOrder->HrYrWk().Week()),buf,10));
				break;

			case COrderSelection::SORT_PST_WEEK_DELTA:

				SetItem(m_listGroupsEx,grpCount,count,_ltoa(NTime::PSDateDeltaWeekTruncated(pOrder->PlannedLatestStartDate()),buf,10));
				break;

			case COrderSelection::SORT_PLANNED_START:

				//ostr.fill('0');
				//ostr << setw(8) << pOrder->PlannedLatestStartDate()
				//	 << ends;
				ostr << setw(4) << NTime::PSDateDelta(pOrder->PlannedLatestStartDate())
					 << ends;
				SetItem(m_listGroupsEx,grpCount,count,ostr.str());
				ostr.freeze(false);
				ostr.seekp(0);
				//ostr.fill(' ');
				break;


			case COrderSelection::SORT_CARBON:
			case COrderSelection::SORT_MANGANESE:
			case COrderSelection::SORT_SIGNIF_EL:
				{
					CChem::Element elt;

					if ( (*is)  == COrderSelection::SORT_CARBON )
						elt = CChem::ELT_C;
					else if ( (*is)  == COrderSelection::SORT_MANGANESE )
						elt = CChem::ELT_MN;
					else {
						if ( ! CChem::ToElement(pOrder->SignifElSym(),elt) ) {
							SetItem(m_listGroupsEx,grpCount,count,"???");
							break;
						}
					}


					if ( (*is) == COrderSelection::SORT_SIGNIF_EL )
						ostr << setw(2) << LPCTSTR(pOrder->SignifElSym()) << ": ";
					ostr.fill('0');
					ostr << setw(5) << setprecision(3) << pOrder->MinElement(elt)
						 << " - "
						 << setw(5) << setprecision(3) << pOrder->MaxElement(elt)
						 << ends;
					SetItem(m_listGroupsEx,grpCount,count,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);
					ostr.fill(' ');
				}
				break;
			}
		}
	
		SetItem(m_listGroupsEx,grpCount,count,_itoa((*ig)->NumPieces(),buf,10));
		++count;

		//SetItem(m_listGroupsEx,grpCount,count,_ltoa(long((*ig)->NumPriorityOrders()),buf,10));
		SetItem(m_listGroupsEx,grpCount,count,_ltoa(long((*ig)->NumOrders()),buf,10));
		++count;

		SetItem(m_listGroupsEx,grpCount,count,_ltoa(long((*ig)->Tons()),buf,10));
		++count;

		SetItem(m_listGroupsEx,grpCount,count,_ltoa(long((*ig)->CurrentBackTons()),buf,10));
		++count;

		SetItem(m_listGroupsEx,grpCount,count,_ltoa(long((*ig)->FutureTons()),buf,10));
		++count;
	}
#endif
	//m_listGroupsEx.SetRedraw(TRUE);
	//m_listGroupsEx.Invalidate();

	//m_listGroupsEx.ShowWindow(SW_SHOW);

	SetGroupListSelections();

	//if ( maintainPosition ) {
	//	m_listGroupsEx.EnsureVisible(posn1,false);
	//	m_listGroupsEx.EnsureVisible(posn2,false);
	//}
}


#endif


#endif

