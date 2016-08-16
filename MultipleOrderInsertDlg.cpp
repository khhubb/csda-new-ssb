// MultipleOrderInsertDlg.cpp : implementation file
//
//  K. Hubbard  05-5-09   Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                        modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                        controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#include "csda.h"
#include "MultipleOrderInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SuperScen.h"
#include "Snapshot.h"
#include "CSOrder.h"
#include "Order.h"
#include "OrderNumDlg.h"
#include "OrderInquiryDlg.h"
#include "SpecMgr.h"
#include "Caster.h"


/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertDlg dialog


CMultipleOrderInsertDlg::CMultipleOrderInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultipleOrderInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultipleOrderInsertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strandNum = 0;
	m_casterNum = 0;
	m_pSuperScen = 0;

	m_wndGrid.m_pMOIDlg = this;

}


void CMultipleOrderInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultipleOrderInsertDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultipleOrderInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CMultipleOrderInsertDlg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SPLIT, OnButtonSplit)
	ON_BN_CLICKED(IDC_BUTTON_JOIN, OnButtonJoin)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_NEW_STOCK, OnButtonNewStock)
	ON_BN_CLICKED(IDC_BUTTON_ORDER_INQUIRY, OnButtonOrderInquiry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultipleOrderInsertDlg message handlers

BOOL CMultipleOrderInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndGrid.SubclassDlgItem(IDC_GRID1, this);
	m_wndGrid.Initialize();
	m_wndGrid.GetParam()->EnableUndo(FALSE);


	// Set up a bitmap button
	m_secBtnOrderInquiry.AttachButton(IDC_BUTTON_ORDER_INQUIRY,
									SECBitmapButton::Al_Center,
									IDB_BITMAP_ORDER_INQUIRY,
									this);



	assert( m_pSuperScen != 0 );
	assert( m_strandNum == 1 || m_strandNum == 2 );
	assert(Caster::IsValidCasterValue(m_casterNum));

	InitializeCellContents();

	if ( m_wndGrid.GetRowCount() > 0 )
		m_wndGrid.SetCurrentCell(1,COL_NUM_PIECES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMultipleOrderInsertDlg::InitializeCellContents()
{
	// ## Add cast to ROWCOL
	m_wndGrid.SetRowCount((ROWCOL)m_orders.size());

	int index = 0;

	for ( vector<CCSOrder*>::iterator io = m_orders.begin();
		  io != m_orders.end();
		  ++io, ++index )

		UpdateRowFromOrder(	index+1, (*io) );
}


void CMultipleOrderInsertDlg::UpdateRowFromOrder( ROWCOL row, CCSOrder* pCSOrder )
{

	CString strOrdWidth;
	CString strOrdLength;
	CString strOrdNumPcs;
	
	COrder* pOrder = pCSOrder->Order();

	if ( pOrder != 0 ) {

		strOrdWidth.Format("%4.1f %ld %4.1f",
						   pOrder->MinAllowedSlabWidth(false),
						   long(pOrder->SlabWidth()),
						   pOrder->MaxAllowedSlabWidth(false));

		strOrdLength.Format("%ld %ld %ld",
							long( pOrder->MinSlabLength() ),
							long( pOrder->SlabLength()    ),
							long( pOrder->MaxSlabLength() ));

		strOrdNumPcs.Format("%d",m_pSuperScen->NumSlabsDue(pOrder));



	}


	
	m_wndGrid.SetValueRange( CGXRange( row, COL_ORD_WIDTH		),	strOrdWidth					);
	m_wndGrid.SetValueRange( CGXRange( row, COL_ORD_LENGTH		),	strOrdLength				);
	m_wndGrid.SetValueRange( CGXRange( row, COL_ORD_NUM_PIECES	),	strOrdNumPcs				);
	// FP Change
	//m_wndGrid.SetValueRange( CGXRange( row, COL_ORDER_NUM		),	pCSOrder->OrderNum()		);
	m_wndGrid.SetValueRange( CGXRange( row, COL_ORDER_NUM		),	pCSOrder->FpOrderNum()		);
	m_wndGrid.SetValueRange( CGXRange( row, COL_SLAB_WIDTH		),	pCSOrder->SlabWidth()		);
	m_wndGrid.SetValueRange( CGXRange( row, COL_SLAB_LENGTH		),	long(pCSOrder->SlabLength()		));	// user requested truncation to integer	
	m_wndGrid.SetValueRange( CGXRange( row, COL_NUM_PIECES		),	LONG(pCSOrder->NumPieces())	);
	m_wndGrid.SetValueRange( CGXRange( row, COL_SLAB_CONDN_CODE	),	LONG(pCSOrder->SlabCondnCode())	);
	m_wndGrid.SetValueRange( CGXRange( row, COL_SPEC			),	pCSOrder->LotSpec()			);
	//m_wndGrid.SetValueRange( CGXRange( row, COL_COMMENT			),	pCSOrder->Comment()			);

	CGXStyle style;
	// FP Change
	//style.SetEnabled( pCSOrder->OrderNum() == 9999999 );
	style.SetEnabled( pCSOrder->FpOrderNum().Left(7) == "9999999" );
	m_wndGrid.SetStyleRange( CGXRange(row,COL_SLAB_CONDN_CODE), style);
	m_wndGrid.SetStyleRange( CGXRange(row,COL_SPEC           ), style);
	
}


void CMultipleOrderInsertDlg::UpdateAllOrders()
{
	ROWCOL rowCount = m_wndGrid.GetRowCount();

	for ( ROWCOL row = 1; row <= rowCount; ++row ) 
		UpdateOrderFromRow(row, m_orders[row-1] );
}


void CMultipleOrderInsertDlg::UpdateOrderFromRow(ROWCOL row, CCSOrder* pCSOrder)
{
	//  we don't do any verification here.
	//  hopefully, this happens when we lose focus.

	//CString strOrderNum	= m_wndGrid.GetValueRowCol(row,COL_ORDER_NUM);
	CString strWidth	= m_wndGrid.GetValueRowCol(row,COL_SLAB_WIDTH);
	CString strLength	= m_wndGrid.GetValueRowCol(row,COL_SLAB_LENGTH);
	CString strNumPces	= m_wndGrid.GetValueRowCol(row,COL_NUM_PIECES);
	CString strCondn	= m_wndGrid.GetValueRowCol(row,COL_SLAB_CONDN_CODE);
	CString strSpec		= m_wndGrid.GetValueRowCol(row,COL_SPEC);
	CString strComment	= m_wndGrid.GetValueRowCol(row,COL_COMMENT);
	
	pCSOrder->SlabWidth( Width(atol(strWidth)) );
	pCSOrder->SlabLength(  Length(atol(strLength)) );
	pCSOrder->NumPieces( atoi(strNumPces) );
	
	// FP change
	//if ( pCSOrder->OrderNum() == 9999999 ) {
	if ( pCSOrder->FpOrderNum().Left(7) == "9999999" ) {
		pCSOrder->LotSpec( strSpec, m_casterNum );
		pCSOrder->SlabCondnCode( atoi(strCondn) );
	}

	//pCSOrder->Comment( strComment );
}



// Add a new row (order) after the current row.

void CMultipleOrderInsertDlg::OnButtonNew() 
{
	COrderNumDlg dlg;

	dlg.m_type = COrderNumDlg::TYPE_ORDER_NUM;
	dlg.m_caption = "Please enter MILL_ORDER_NUM for new order:";

	if ( dlg.DoModal() != IDOK )
		return;

	
	CCSOrder* pCSOrder = new CCSOrder(dlg.m_pOrder,
									  m_strandNum,
									  m_casterNum);

	AddNewOrder(pCSOrder);
}



void CMultipleOrderInsertDlg::OnButtonNewStock() 
{

	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// insert at end
		row = m_wndGrid.GetRowCount();

	//  we want to copy values from the preceding order, if there is one.
	//  else set to some default values

	Width width;
	Length length;
	CString spec;
	CString InclusionModified;  // added material direction see caststring.h def k. hubbard 5-20-08
	CString StockReason;            // added reasons 5-5-09 k. hubbard
    CString StockCommercialReason;  // added reasons 5-5-09 k. hubbard


	if ( row == 0 ) {

		length	= 350;
		width	= 55;

		COrderNumDlg dlg;

		dlg.m_type		= COrderNumDlg::TYPE_LOT_SPEC;
		dlg.m_casterNum	= m_casterNum;
		dlg.m_caption	= "Please enter lot spec for new order:";

		if ( dlg.DoModal() != IDOK )
			return;

		spec	= dlg.m_spec;
	}
	else {

		CCSOrder* pPrevOrder = m_orders[row-1];
		UpdateOrderFromRow(row,pPrevOrder);

		length = pPrevOrder->SlabLength();
		width  = pPrevOrder->SlabWidth();
		spec   = pPrevOrder->LotSpec();
		InclusionModified = pPrevOrder->CRInclModified(); // added material direction see caststring.h def k. hubbard 5-20-08
	}


	CCSOrder* pCSOrder = CCSOrder::CreateStockOrder(spec,
													width,
													length,
													4782,
													1,
													m_strandNum,
													m_casterNum,
													InclusionModified, // added material direction see caststring.h def k. hubbard 5-20-08
													StockReason,  // added reasons 5-5-09 k. hubbard
													StockCommercialReason);  // added reasons 5-5-09 k. hubbard

	AddNewOrder(pCSOrder);
}





void CMultipleOrderInsertDlg::AddNewOrder(CCSOrder* pCSOrder)
{
	ROWCOL row;
	ROWCOL col;
	ROWCOL insertRow;

	if ( m_wndGrid.GetCurrentCell(row,col) ) {
		insertRow = row+1;
		m_orders.insert( m_orders.begin()+insertRow-1, pCSOrder );
	}
	else {
		insertRow = m_wndGrid.GetRowCount()+1;
		m_orders.push_back( pCSOrder );
	}

	m_wndGrid.InsertRows(insertRow,1);

	UpdateRowFromOrder(insertRow,pCSOrder);
	
	m_wndGrid.SetCurrentCell(insertRow,COL_NUM_PIECES);
}

		
// Delete the current row

void CMultipleOrderInsertDlg::OnButtonDelete() 
{
	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// no current cell, so can't do
		return;

	vector<CCSOrder*>::iterator io = m_orders.begin() + row - 1;
	delete (*io);
	m_orders.erase(io);

	m_wndGrid.RemoveRows(row,row);
}

	


// Split the current row

void CMultipleOrderInsertDlg::OnButtonSplit() 
{
	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// no current cell, so can't do
		return;

	UpdateAllOrders();

	CCSOrder* pOrig = m_orders[row-1];


	if ( pOrig->NumPieces() > 1 ) {
		pOrig->NumPieces( pOrig->NumPieces()-1 );
		UpdateRowFromOrder(row,pOrig);
	}

	CCSOrder* pNew = new CCSOrder(*pOrig);
	pNew->NumPieces(1);
	AddNewOrder(pNew);
}


// Join the current row with the one following it, if possible
//  (possible = same order)

void CMultipleOrderInsertDlg::OnButtonJoin() 
{
	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// no current cell, so can't do
		return;

	if ( row == m_wndGrid.GetRowCount() )
		// we are at the end already, do nothing;
		return;

	CCSOrder* pFirst = m_orders[row-1];
	CCSOrder* pSecond = m_orders[row];

	// FP Change
	//if ( pFirst->OrderNum() != pSecond->OrderNum() )
	if ( pFirst->FpOrderNum() != pSecond->FpOrderNum() )
		return;

	pFirst->NumPieces( pFirst->NumPieces() + pSecond->NumPieces() );
	
	// Set focus to second row and delete it
	m_wndGrid.SetCurrentCell(row+1,col);

	OnButtonDelete();

}


// Move current row up

void CMultipleOrderInsertDlg::OnButtonUp() 
{
	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// no current cell, so can't do
		return;

	if ( row == 1 )
		// we are at the first row, do nothing
		return;

	int index = row-1;

	CCSOrder* pTemp = m_orders[index];
	m_orders[index] = m_orders[index-1];
	m_orders[index-1] = pTemp;

	m_wndGrid.MoveRows(row,row,row-1);

	
}


// Move current row down

void CMultipleOrderInsertDlg::OnButtonDown() 
{
	ROWCOL row;
	ROWCOL col;

	if ( ! m_wndGrid.GetCurrentCell(row,col) )
		// no current cell, so can't do
		return;

	if ( row == m_wndGrid.GetRowCount() )
		// we are at the end already, do nothing;
		return;

	int index = row-1;
	
	CCSOrder* pTemp = m_orders[index];
	m_orders[index] = m_orders[index+1];
	m_orders[index+1] = pTemp;
	
	m_wndGrid.MoveRows(row+1,row+1,row);
}






BOOL CMultipleOrderInsertDlg::OnValidateCell(ROWCOL nrow, ROWCOL ncol)
{
	// we are interested in movements from certain columns

	CString s;
	m_wndGrid.GetCurrentCellControl( )->GetCurrentText(s);

#ifdef _DEBUG
	afxDump << "OnValidateCell, row = " << nrow << ", col = " << ncol  
			<< ", value = " << s << "\n";
#endif

	switch ( ncol ) {

	case COL_ORD_WIDTH:			// not enabled, ever	
	case COL_ORD_LENGTH:		// not enabled, ever
	case COL_ORD_NUM_PIECES:	// not enabled, ever
	case COL_COMMENT:			// all values allowed			
		break;

	case COL_SLAB_WIDTH:	
		{
			Width wid = atof(s);

			if ( wid < 20 || wid > 99 )	{	// include case of non-numeric
				m_wndGrid.SetWarningText("SLAB_WIDTH value is bad -- please reenter.");
				return FALSE;
			}
		}
		break;

	case COL_SLAB_LENGTH:		
		{
			Length len = atof(s);

			if ( len < 100 || len > 999 )	{	// include case of non-numeric
				m_wndGrid.SetWarningText("SLAB_LENGTH value is bad -- please reenter.");
				return FALSE;
			}
		}
		break;

	case COL_NUM_PIECES:
		{
			s.TrimRight();
			s.TrimLeft();

			int num = atoi(s);

			if (num < 0 
				|| 
				num > 99
				||
				(num == 0 && s != "0" ) )	{	
				m_wndGrid.SetWarningText("#PC value is bad -- please reenter.");
				return FALSE;
			}
		}
		break;


	case COL_ORDER_NUM:
		
		// we must verify that such an order exists
		{
			vector<COrder*>& orders = TheSnapshot.Orders();
			// FP change
			//long orderNum = atol(LPCTSTR(s));
	
			//COrder* pOrder = COrder::FindOrder(orderNum,orders);
			COrder* pOrder = COrder::FindOrder(s,orders);
	
			//if ( orderNum != 9999999 && pOrder == 0 ) {
			if ( s != "9999999" && pOrder == 0 ) {
				m_wndGrid.SetWarningText("No such order -- please reenter.");
				return FALSE;
			}
		}

		break;

	case COL_SPEC:

		// this must be a stock order
		// we must verify that such a spec exists
		{
			CSpec* pSpec = TheSnapshot.SpecMgr().FindSpecMaybe(s,m_casterNum);
	 
			if ( pSpec == 0 ) {
				m_wndGrid.SetWarningText("No such spec -- please reenter");
				return FALSE;
			}
		}
		break;

	case COL_SLAB_CONDN_CODE:
		
		// Need a numeric code.
		// I use zero as a validity check for the conversion.
		// I don't have any value ranges for this.
		// I hope zero is not a possibility.

		{
			int code = atoi(LPCTSTR(s));
	
			if ( code <= 0 || code > 9999 ) {
				m_wndGrid.SetWarningText("Please enter a valid condition code");
				return FALSE;
			}
		}
		break;

	}


	return TRUE;
}


BOOL CMultipleOrderInsertDlg::OnEndEditingCell(ROWCOL nrow, ROWCOL ncol)
{
	// we are interested in movements from certain columns

	CString s = m_wndGrid.GetValueRowCol(nrow,ncol);

	if ( nrow < 1 )
		return TRUE;

	if ( nrow > m_orders.size() )
		return TRUE;

#ifdef _DEBUG
	afxDump << "OnEndEditingCell, row = " << nrow << ", col = " << ncol 
			<< ", value = " << s << "\n";
#endif

	CCSOrder* pCSOrder = m_orders[nrow-1];


	switch ( ncol ) {

	case COL_ORD_WIDTH:		
	case COL_ORD_LENGTH:	
	case COL_ORD_NUM_PIECES:
	case COL_COMMENT:

		// don't care about these fields
		break;

	case COL_ORDER_NUM:

		// we have already validated that this is the number of an existing order
		// (or 9999999)
		// if the ordernum has changed, let's create a new CSOrder to match the
		//   designated Order.
		// Question: which fields do we maintain?
		//   answer, if we are becoming a stock order, copy all that matter.
		//   otherwise, only the comment.
		// This requires us to create a new CSOrder and replace the existing one.

		{

			vector<COrder*>& orders = TheSnapshot.Orders();
		
			// FP changes
			//long orderNum = atol(LPCTSTR(s));
			CString fpOrderNum = s;
			
			//COrder* pOrder = COrder::FindOrder(orderNum,orders);
			COrder* pOrder = COrder::FindOrder(fpOrderNum,orders);
	
			//if ( orderNum == pCSOrder->OrderNum() )
			if ( fpOrderNum == pCSOrder->FpOrderNum() )
				break;

			//if ( orderNum != 9999999 && pOrder == 0 ) {
			if ( fpOrderNum.Left(7) != "9999999" && pOrder == 0 ) {
				m_wndGrid.SetWarningText("No such order -- please reenter.");
				return FALSE;
			}

			CCSOrder* pNewCSOrder;

			//if ( orderNum == 9999999 )
			if ( fpOrderNum.Left(7) == "9999999" )

				pNewCSOrder = CCSOrder::CreateStockOrder(pCSOrder->LotSpec(),
														 pCSOrder->SlabWidth(),
														 pCSOrder->SlabLength(),
														 pCSOrder->SlabCondnCode(),
														 pCSOrder->NumPieces(),
														 m_strandNum,
														 m_casterNum,
														 pCSOrder->CRInclModified(), // added cast rule slab sale material direction spec rule data k. hubbard 5-20-08
										                 pCSOrder->StockReason(),  // added reasons 5-5-09 k. hubbard
									                     pCSOrder->StockCommercialReason());  // added reasons 5-5-09 k. hubbard
			else {

				pNewCSOrder = new CCSOrder(pOrder,
										   m_strandNum,
										   m_casterNum);
				//pNewCSOrder->Comment   ( pCSOrder->Comment()    );
				pNewCSOrder->SlabLength( pCSOrder->SlabLength() );
				pNewCSOrder->SlabWidth ( pCSOrder->SlabWidth()  );
				pNewCSOrder->NumPieces ( pCSOrder->NumPieces()  );
				pNewCSOrder->SlitReason( pCSOrder->SlitReason() );  // added slit reason here 2-26-10 k. hubbard

			}

			delete pCSOrder;
			m_orders[nrow-1] = pNewCSOrder;
			UpdateRowFromOrder(nrow,pNewCSOrder);
		}
		
		break;

	case COL_SLAB_WIDTH:

		// if the slab width has changed, we need to recompute the length

		{
			Width wid = atof(s);

			if ( pCSOrder->SlabWidth() == wid )
				break;

			Length len = pCSOrder->ComputeSlabLengthFromWidth(wid);
			// Per request, truncate slab length
			pCSOrder->SlabLength(long(len));
			pCSOrder->SlabWidth(wid);
			UpdateRowFromOrder(nrow,pCSOrder);
		}
		break;
			
	case COL_SLAB_LENGTH:
	case COL_NUM_PIECES:
	case COL_SLAB_CONDN_CODE:
	case COL_SPEC:
		
		// just update the CSOrder from the data in the row.
		UpdateOrderFromRow(nrow,pCSOrder);
		break;
	}

	return TRUE;
}



void CMultipleOrderInsertDlg::OnButtonOrderInquiry() 
{
	COrderInquiryDlg dlg;

	dlg.m_pSuperScen = m_pSuperScen;

	dlg.DoModal();

}
