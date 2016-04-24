// CSOrderParmDlg.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function   :  This dialog (GUI) program displays editable order parameters including the screen used for reproviding planned widths. 
//                Order data resulting from Unapplied Orders Set and Daily Plan Orders Only Set is presented here  
//                and makes order info available to other class modules throughout via Order.h connects via m_pOrder->field x
//                including the Single String Builder.       
//	Maintenance:
//
//  k. hubbard  11-11-03: Added these 2 fields for reprovide limits request m_MORD_PROV_CAST_SL_WTH_MAX 
//	                      m_MORD_PROV_CAST_SL_WTH_MIN. per P. Fronczek
//
//  k. hubbard  11-13-03: Disabled Transition check box, and began loading m_minCastWidth / Trans (max too) fields with new prov fields 
//                        from comment above per P. Fronczek.
//  
//  k. hubbard  12-11-03: Reactivated Transition check box with new prov fields. per P. Fronczek  
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//

#include "stdafx.h"


#include "csda.h"
#include "CSOrderParmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Order.h"
#include "CSOrder.h"
#include "SuperScen.h"
#include "Snapshot.h"
#include "Spec.h"
#include "SpecMgr.h"
#include "CastString.h"
#include "CastStringValidator.h"
#include "MiscConstants.h"



/////////////////////////////////////////////////////////////////////////////
// CCSOrderParmDlg dialog


CCSOrderParmDlg::CCSOrderParmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSOrderParmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSOrderParmDlg)
	m_castLength = 0.0;
	m_castWidth = 0.0;
	m_condnCode = 0;
	m_numPieces = 0;
	m_operation = _T("");
	m_orderNum = _T("");
	m_orderedCastLength = _T("");
	m_orderedCastWidth = _T("");
	m_orderedCondnCode = _T("");
	m_orderedNumPieces = _T("");
	m_spec = _T("");
	m_maxCastLength = _T("");
	m_maxCastWidth = _T("");
	m_minCastLength = _T("");
	m_minCastWidth = _T("");
	m_slitTypeCode = _T("");
	m_dispoCode = 0;
	m_changeIndic = _T("");
	m_expCode = _T("");
	m_ciCode = 0;
	m_hbWidth = _T("");
	m_useTransWidths = FALSE;
	m_maxCastWidthTrans = _T("");
	m_minCastWidthTrans = _T("");
	m_planSteelLengthMin = 0;
	m_planSteelLengthMax = 0;
	m_provCastSlabWidthMin = 0.0;  // added these 2 fields 11-11-03 k. hubbard
	m_provCastSlabWidthMax = 0.0;
	//}}AFX_DATA_INIT

	m_pCSOrder		= 0;
	m_pOrder		= 0;
	m_pSuperScen	= 0;
	m_casterNum		= 0;

	m_precedingWidth = 0;

	m_bStock		= false;
	m_bCreate		= false;
}



void CCSOrderParmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSOrderParmDlg)
	DDX_Control(pDX, IDC_SPIN_DISP_CODE, m_spinDispCode);
	DDX_Control(pDX, IDC_EDIT_CONDN_CODE, m_editCondnCode);
	DDX_Control(pDX, IDC_EDIT_SPEC, m_editSpec);
	DDX_Control(pDX, IDC_SPIN_CAST_WIDTH, m_spinCastWidth);
	DDX_Control(pDX, IDC_SPIN_CAST_LENGTH, m_spinCastLength);
	DDX_Control(pDX, IDC_SPIN_NUM_PIECES, m_spinNumPieces);
	DDX_Text(pDX, IDC_EDIT_CAST_LENGTH, m_castLength);
	DDX_Text(pDX, IDC_EDIT_CAST_WIDTH, m_castWidth);
	DDX_Text(pDX, IDC_EDIT_CONDN_CODE, m_condnCode);
	DDX_Text(pDX, IDC_EDIT_NUM_PIECES, m_numPieces);
	DDV_MinMaxInt(pDX, m_numPieces, 1, 999);
	DDX_Text(pDX, IDC_STATIC_OPERATION, m_operation);
	DDX_Text(pDX, IDC_STATIC_ORDER_NUM, m_orderNum);
	DDX_Text(pDX, IDC_STATIC_ORDERED_CAST_LENGTH, m_orderedCastLength);
	DDX_Text(pDX, IDC_STATIC_ORDERED_CAST_WIDTH, m_orderedCastWidth);
	DDX_Text(pDX, IDC_STATIC_ORDERED_CONDN_CODE, m_orderedCondnCode);
	DDX_Text(pDX, IDC_STATIC_ORDERED_NUM_PIECES, m_orderedNumPieces);
	DDX_Text(pDX, IDC_EDIT_SPEC, m_spec);
	DDX_Text(pDX, IDC_STATIC_MAX_CAST_LENGTH, m_maxCastLength);
	DDX_Text(pDX, IDC_STATIC_MAX_CAST_WIDTH, m_maxCastWidth);
	DDX_Text(pDX, IDC_STATIC_MIN_CAST_LENGTH, m_minCastLength);
	DDX_Text(pDX, IDC_STATIC_MIN_CAST_WIDTH, m_minCastWidth);
	DDX_CBString(pDX, IDC_COMBO_SLIT_TYPE_CODE, m_slitTypeCode);
	DDX_Text(pDX, IDC_EDIT_DISPO_CODE, m_dispoCode);
	DDV_MinMaxInt(pDX, m_dispoCode, 0, 9);
	DDX_Text(pDX, IDC_EDIT_CHANGE_INDICATOR, m_changeIndic);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_CODE, m_expCode);
	DDX_Text(pDX, IDC_EDIT_CI_CODE, m_ciCode);
	DDX_Text(pDX, IDC_EDIT_HB_WIDTH, m_hbWidth);
	DDX_Check(pDX, IDC_CHECK_USE_TRANS_WIDTHS, m_useTransWidths);
	DDX_Text(pDX, IDC_STATIC_MAX_CAST_WIDTH_TRANS, m_maxCastWidthTrans);
	DDX_Text(pDX, IDC_STATIC_MIN_CAST_WIDTH_TRANS, m_minCastWidthTrans);
	DDX_Text(pDX, IDC_EDIT_CAST_LENGTH2, m_planSteelLengthMin);
	DDX_Text(pDX, IDC_EDIT_CAST_LENGTH3, m_planSteelLengthMax);
	DDX_Text(pDX, IDC_STATIC_PROV_CAST_WIDTH_MIN, m_provCastSlabWidthMin);  // added these 2 fields 11-11-03 k. hubbard
	DDX_Text(pDX, IDC_STATIC_PROV_CAST_WIDTH_MAX, m_provCastSlabWidthMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSOrderParmDlg, CDialog)
	//{{AFX_MSG_MAP(CCSOrderParmDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_CAST_WIDTH, OnKillfocusEditCastWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAST_WIDTH, OnDeltaposSpinCastWidth)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_CHANGE_INDICATOR, OnButtonClearChangeIndicator)
	ON_BN_CLICKED(IDC_CHECK_USE_TRANS_WIDTHS, OnCheckUseTransWidths)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSOrderParmDlg message handlers



// We have several modes:
//
//    Editing vs creating
//    Stock vs regular (non-stock)
//
//    Initial values for the edited and displayed parameters
//		come from a COrder or a CCSOrder.
//
//    When editing, a CSOrder will be supplied.  
//	  It may or may not have an Order attached.
//    (A stock order definitely will not have an Order attached.
//     A regular order usually will have an Order attached, 
//     but it is possible that it doesn't.)
//
//
//	  When creating, an Order may be supplied, or a CSOrder may be supplied.
//    If there is an Order attached to the CSOrder, then it will be used.
//
//	  The user must set the flag m_bCreate prior to calling DoModal.
//	  If m_bCreate is true, then the user must also specify a value for m_bStock.
//	  If m_bCreate is false, then we will compute m_bStock via 
//			m_bStock = m_pCSOrder->OrderNum() == 9999999 
//
//    Depending on the values of these flags, different parameters must be initialized
//       by the user.  In the table below, R = required, O = optional, I = ignored
//
//								  m_pSuperScen  m_pCSOrder m_pOrder  m_casterNum
//	    m_bCreate &&   m_bStock        R             O        O          R
//	    m_bCreate && ! m_bStock        R             O        R          R
//	  ! m_bCreate &&   m_bStock        R             R        I          R
//	  ! m_bCreate && ! m_bStock        R             R        I          R
//
//	  Technically, m_casterNum is only used (at the moment) when m_bStock is true
//		so that we can verify the spec.  We make it required in case that changes
//	    in the future.



BOOL CCSOrderParmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//  user must supply a super scenario
	assert( m_pSuperScen != 0 );
	
	//  user must supply the casterNum
	assert( 1 <= m_casterNum && m_casterNum <= 3 );

	//  if the user supplies a CSOrder, then we use its Order, if it has one.
	if ( m_pCSOrder != 0 && m_pCSOrder->Order() != 0 )
		m_pOrder = m_pCSOrder->Order();

	//  if we are editing, the user must supply a CSOrder
	assert( m_bCreate || m_pCSOrder != 0 );

	//  if we are editing, then we can compute whether this is stock or not.
	if ( ! m_bCreate ) 
		// FP Change
		//m_bStock = (m_pCSOrder->OrderNum() == 9999999);
		m_bStock = (m_pCSOrder->FpOrderNum().Left(7) == "9999999");

	// if we are creating a non-stock order, the user must supply an order
	assert( ! m_bCreate || m_bStock || m_pOrder != 0 );


	InitOrderedFields();
	InitCurrentFields();
	InitMiscFields();


	// Set the spin control range for numPieces, etc;
	m_spinNumPieces.SetRange(0,1000);
	m_spinCastLength.SetRange(200,400);
	m_spinCastWidth.SetRange(20,80);
	m_spinDispCode.SetRange(0,9);

	// the OK button is enabled if we are creating
	// or we are editing a CSOrder from an editable string.
	
	GetDlgItem(IDOK)->EnableWindow( m_bCreate || m_pCSOrder->CastString()->CanEdit() );

	UpdateData(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



// TODO -- if stock, modify piece wgt on exit.


//  We know ordered values of various parameters
//  only if an Order has been supplied.


void CCSOrderParmDlg::InitOrderedFields()
{
	if ( m_pOrder == 0 ) {
		m_orderedCastLength	= "";
		m_orderedCastWidth	= "";
		m_orderedNumPieces	= "";
		m_orderedCondnCode	= "";
		m_minCastLength		= "";
		m_maxCastLength		= "";
		m_minCastWidth		= "";
		m_maxCastWidth		= "";
		m_minCastWidthTrans	= "";
		m_maxCastWidthTrans	= "";
		m_hbWidth			= "";
		m_ciCode			= 0;
		m_provCastSlabWidthMin = 0;  // added these 2 fields 11-11-03 k. hubbard
		m_provCastSlabWidthMax = 0;
	}
	else {
		char buf[100];

		m_orderedCastLength	= _ltoa(long(m_pOrder->SlabLength()),buf,10);
		m_orderedCastWidth	= _ltoa(long(m_pOrder->SlabWidth()),buf,10);
		m_orderedNumPieces	= _itoa(m_pSuperScen->NumSlabsDue(m_pOrder),buf,10);

		ostrstream ostr;
		ostr.fill('0');
		ostr << setw(4) << m_pOrder->SlabCondnCode() << ends;

		m_orderedCondnCode	= ostr.str();

		ostr.freeze(false);
		ostr.seekp(0);

		m_minCastLength		= _ltoa(long(m_pOrder->MinSlabLength()),buf,10);
		m_maxCastLength		= _ltoa(long(m_pOrder->MaxSlabLength()),buf,10);

		bool isTrans = (m_pCSOrder != 0 && m_pCSOrder->IsTransition());

		sprintf(buf,"%5.1f",m_pOrder->MinAllowedSlabWidth(false)); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 12-11-03 k. hubbard
//		sprintf(buf,"%5.1f",m_pOrder->ProvSlabWidthMin());  // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-13-03 k. hubbard
		m_minCastWidth		= buf;
//		m_minCastWidth		= m_pOrder->ProvSlabWidthMin(); 

		sprintf(buf,"%5.1f",m_pOrder->MaxAllowedSlabWidth(false)); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 12-11-03 k. hubbard
//		sprintf(buf,"%5.1f",m_pOrder->ProvSlabWidthMax());  // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-13-03 k. hubbard
		m_maxCastWidth		= buf;
//		m_maxCastWidth		= m_pOrder->ProvSlabWidthMax(); 

		sprintf(buf,"%5.1f",m_pOrder->MinAllowedSlabWidth(true)); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 12-11-03 k. hubbard
//		sprintf(buf,"%5.1f",m_pOrder->ProvSlabWidthMin());  // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-13-03 k. hubbard
		m_minCastWidthTrans	= buf;
//		m_minCastWidthTrans	= m_pOrder->ProvSlabWidthMin(); 

		sprintf(buf,"%5.1f",m_pOrder->MaxAllowedSlabWidth(true)); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 12-11-03 k. hubbard
//		sprintf(buf,"%5.1f",m_pOrder->ProvSlabWidthMax());  // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-13-03 k. hubbard
		m_maxCastWidthTrans	= buf;
//		m_maxCastWidthTrans	= m_pOrder->ProvSlabWidthMax(); 



		sprintf(buf,"%5.1f",m_pOrder->HbWidth());
		m_hbWidth			= buf;

		m_ciCode = m_pOrder->CICode();

		// added these 2 fields 11-11-03 k. hubbard
		m_provCastSlabWidthMin = m_pOrder->ProvSlabWidthMin(); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
		m_provCastSlabWidthMax = m_pOrder->ProvSlabWidthMax(); // 11-11-03 k. hubbard _ltoa function converts the digits of value to a null-terminated character string 
		                           //and stores the result (up to 33 bytes) in string.

	}
}



//  These are the editable (or sometimes editable) fields.
//  If a CSOrder is supplied, the values are taken directly from it.
//  If no CSOrder is supplied, but an Order is, we take the values from the it.
//  If neither is given, we set to some standard defaults.
//  Note that in the last case, we are definitely creating a stock order.

void CCSOrderParmDlg::InitCurrentFields()
{
	if ( m_pCSOrder != 0 ) {

		m_castLength	= long(m_pCSOrder->SlabLength());	// user requested truncation to integer	
		m_castWidth		= m_pCSOrder->SlabWidth();
		m_condnCode		= ( m_bCreate && m_bStock ?  4782 : m_pCSOrder->SlabCondnCode() );
		m_numPieces		= ( m_bCreate && m_bStock ?     1 : m_pCSOrder->NumPieces()     );
		m_slitTypeCode	= (m_pCSOrder->SlitTypeCode() == ' ' 
			               ? " <blank>" 
						   : CString(m_pCSOrder->SlitTypeCode()));
		m_dispoCode		= m_pCSOrder->DispCode();
		m_changeIndic	= CString(m_pCSOrder->ChangeFlag());
		m_expCode		= CString(m_pCSOrder->ExposureCode());

		m_planSteelLengthMin	= m_pCSOrder->PlanSteelLengthMin();
		m_planSteelLengthMax	= m_pCSOrder->PlanSteelLengthMax();
	}
	else if ( m_pOrder != 0 ) {


		m_castLength	= long(m_pOrder->SlabLength());	// user requested truncation to integer	
		m_castWidth		= m_pOrder->SlabWidth();
		m_condnCode		= ( m_bCreate && m_bStock ?  4782 : m_pOrder->SlabCondnCode() );
		m_numPieces		= ( m_bCreate && m_bStock ?     1 : m_pSuperScen->NumSlabsDue(m_pOrder) );
		m_slitTypeCode	= " <blank>";
		m_dispoCode		= 4;
		m_changeIndic	= " ";
		m_expCode		= (m_pOrder->PgmExposure() == 1 ? "E" : "U");

		m_planSteelLengthMin	= long(m_pOrder->MinSlabLength());	// user requested truncation to integer	
		m_planSteelLengthMax	= long(m_pOrder->MaxSlabLength());	// user requested truncation to integer	

		m_provCastSlabWidthMin  = (m_pOrder->ProvSlabWidthMin()); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
		m_provCastSlabWidthMax  = (m_pOrder->ProvSlabWidthMax()); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard

	
	}
	else {
		// creating, definitely stock
		m_castLength	= 350;
		m_castWidth		= 55;
		m_condnCode		= 4782 ;
		m_numPieces		= 1 ;
		m_slitTypeCode  = " <blank>";
		m_dispoCode		= 4;
		m_changeIndic	= " ";
		m_expCode		= "U";
		m_planSteelLengthMin = m_castLength;
		m_planSteelLengthMax = m_castLength;
	}
//    Uncommented 12-11-03 to open transition widths. Following line controls Transition check box, comm out per P. Fronczek. maint. 11-13-03 k. hubbard 
	m_useTransWidths = ( m_precedingWidth != 0 && m_castWidth != m_precedingWidth);

}



void CCSOrderParmDlg::InitMiscFields()
{
	m_operation = ( m_bCreate ? "Creating" : "Editing" );
	
	//FP Change
	//long orderNum;
	//if ( m_bStock )
	//	orderNum = 9999999;
	//else if ( m_pCSOrder != 0 )
	//	orderNum = m_pCSOrder->OrderNum();
	//else
	//	orderNum = m_pOrder->OrderNum();
	//ostrstream ostr;
	//ostr.fill('0');
	//ostr << setw(7) << orderNum << ends;
	//
	//m_orderNum = ostr.str();
	//
	//ostr.freeze(false);
	//ostr.seekp(0);

	CString fpOrderNum;
	if ( m_bStock )
		fpOrderNum = "9999999";
	else if ( m_pCSOrder != 0 )
		fpOrderNum = m_pCSOrder->FpOrderNum();
	else
		fpOrderNum = m_pOrder->FpOrderNum();

	m_orderNum = fpOrderNum;

		               
	if ( m_pCSOrder != 0 )
		m_spec = m_pCSOrder->LotSpec();
	else if ( m_pOrder != 0 ) {
		m_spec = m_pOrder->CastSpec();
		CCastStringValidator::FixHeatSpec(m_spec,
										  m_spec,
										  m_casterNum);
	}
	else
		m_spec = "";

	m_editSpec.EnableWindow( m_bStock );
	m_editCondnCode.EnableWindow(m_bStock);
}


void CCSOrderParmDlg::OnOK() 
{

	if ( ! UpdateData(true) )
		return;

	if ( m_expCode != "U" && m_expCode != "E" ) {
		MessageBox("Exposure code must be E or U.");
		return;
	}

	//////////DM CHANGE: 2002-09-14: added GetLength test

	if ( m_bStock ) {
		if ( (m_spec.GetLength() != 7 ) || TheSnapshot.SpecMgr().FindSpecMaybe(m_spec,m_casterNum) == 0 ) {
			MessageBox("You must enter a valid lot spec.","Bad spec",MB_ICONERROR);
			return;
		}
	}

	CDialog::OnOK();
}



void CCSOrderParmDlg::OnKillfocusEditCastWidth() 
{

	if ( m_pCSOrder == 0 || m_pOrder == 0 )
		return;

	// Whenever the user changes slabWidth, 
	//    we modify slabLength,
	// also, we may need to recompute the slit type code
	//    unless R is true.

	UpdateData(true);

	UpdateWidthRelatedParms(m_castWidth);

	UpdateData(false);

}


void CCSOrderParmDlg::UpdateWidthRelatedParms(int width)
{
	assert( m_pCSOrder != 0 && m_pOrder != 0 );

	char slitTypeCode = m_slitTypeCode[0];

	CCSOrder::ComputeSlabWidthRelatedParms(m_casterNum,
										   m_pOrder,
										   m_useTransWidths,
										   width,
										   m_condnCode,
										   m_ciCode,    // K. Hubbard 1-29-03 7BF Reline
										   m_dispoCode,
										   slitTypeCode,
										   m_numPieces,
										   m_planSteelLengthMin,
										   m_castLength,
										   m_planSteelLengthMax,
										   m_dispoCode,
										   slitTypeCode,
										   m_numPieces,
										   m_planSteelLengthMin,
										   m_castLength,
										   m_planSteelLengthMax);

	if ( slitTypeCode == ' ' )
		m_slitTypeCode = " <blank>";
	else
		m_slitTypeCode = CString(slitTypeCode);

#if 0
	Length limit = CMiscConstants::CasterSlabLengthMax(m_casterNum);
	if ( m_pOrder != 0 && m_pOrder->MaxSlabLength() > 0 )
		limit = max(limit,m_pOrder->MaxSlabLength());

	if ( m_castLength >  limit ) {
		ostrstream ostr;
		ostr << "Warning: You have just set the length to "
			 << long(m_castLength)
			 << ", greater than the max length "
			 << long(limit)
			 << ends;
		MessageBox(ostr.str(),"Bad length",MB_OK);
		ostr.freeze(false);
	}
#endif
}


void CCSOrderParmDlg::OnDeltaposSpinCastWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if ( m_pCSOrder != 0 && m_pOrder != 0 ) {
		UpdateData(true);
		m_castWidth = pNMUpDown->iPos + pNMUpDown->iDelta;
		UpdateWidthRelatedParms(m_castWidth);
		UpdateData(false);
	}

	*pResult = 0;
}




void CCSOrderParmDlg::OnButtonClearChangeIndicator() 
{
	UpdateData(TRUE);

	m_changeIndic	= " ";

	UpdateData(FALSE);
}


bool CCSOrderParmDlg::XferToCSOrder( CCSOrder* pCSOrder )
{
	bool changed = false;
	
	if ( pCSOrder->SlabWidth() != m_castWidth ) {
		pCSOrder->SlabWidth(m_castWidth);
		changed = true;
	}

	if ( pCSOrder->SlabLength() != m_castLength ) {
		// Per request, truncate slab length
		pCSOrder->SlabLength(long(m_castLength));
		changed = true;
	}

	if ( pCSOrder->NumPieces() != m_numPieces ) {
		pCSOrder->NumPieces(m_numPieces);
		changed = true;
	}

	if ( pCSOrder->DispCode() != m_dispoCode ) {
		pCSOrder->DispCode(m_dispoCode);
		changed = true;
	}
// test truncation 2-19-03 k. hubbard
	if ( pCSOrder->PlanSteelLengthMin() != m_planSteelLengthMin ) {
		pCSOrder->PlanSteelLengthMin(long(m_planSteelLengthMin));
		changed = true;
	}
// test truncation 2-19-03 k. hubbard
	if ( pCSOrder->PlanSteelLengthMax() != m_planSteelLengthMax ) {
		pCSOrder->PlanSteelLengthMax(long(m_planSteelLengthMax));
		changed = true;
	}

	char newSlitTypeCode = ( m_slitTypeCode.GetLength() == 0 
							 ? ' '
							 : m_slitTypeCode[0] );

	if ( pCSOrder->SlitTypeCode() != newSlitTypeCode ) {
		pCSOrder->SlitTypeCode(newSlitTypeCode);
		changed = true;
	}

	char newChangeFlag = ( m_changeIndic.GetLength() == 0 
		                   ? ' ' 
						   : m_changeIndic[0] );

	if ( CString(pCSOrder->ChangeFlag()) != newChangeFlag ) {
		pCSOrder->ChangeFlag(newChangeFlag);
		changed = true;
	}

	char newExpCode = ( m_expCode.GetLength() == 0 
						? 'U'
						: m_expCode[0] );
	
	if ( m_bStock ) {

		if ( pCSOrder->SlabCondnCode() != m_condnCode ) {
			pCSOrder->SlabCondnCode(m_condnCode);
			changed = true;
		}

		if ( pCSOrder->LotSpec() != m_spec ) {
			pCSOrder->LotSpec(m_spec,m_casterNum);
			changed = true;
		}
	}

	return changed;
}



void CCSOrderParmDlg::OnCheckUseTransWidths() 
{

	if ( m_pCSOrder == 0 || m_pOrder == 0 )
		return;

	// Whenever the user toggles 'UseTransWidths', 
	//    we might change the slitTypeCode.

	UpdateData(true);

	UpdateWidthRelatedParms(m_castWidth);

	UpdateData(false);
	
}

void CCSOrderParmDlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}
