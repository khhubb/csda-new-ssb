// CSOrderParmDlg.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Tip Notes: Alt key & closing bracket <> inside open { bracket will automatically take you to the matching closing } bracket.
//	           Spin Controls pNMUpDown->iPos will equal text window's m_castWidth value. pNMUpDown->iDelta will equal positive or negative 1 based on 
//               direction of spin control arrow.   
//  
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
//  k. hubbard  09-29-05: Modified these 2 fields for reprovide limits request m_MORD_PROV_CAST_SL_WTH_MAX 
//	                      m_MORD_PROV_CAST_SL_WTH_MIN by changing the definition to float from original double to handle new precision data.
//                        per P. Fronczek
//
//  k. hubbard  01-31-06: Changed default stock disposition code from 4 to 2 in order to direct slab material to #2 Slab Yard for #2 BOF 
//                        per T. Fronczek
//
//  K. Hubbard  01-24-08  Removed (comm out) stock disp code 2 assignment for millOrderNum. 
//                        Stock default disp code returned to 4 around dock. per E. Jackson (Slab Logistics project) 
//
//  K. Hubbard  01-08-09  Modified m_castWidth field to permit half inch display and editing of planned widths. 
//                        Also had to turn Auto Buddy Integer off in dialog screen spin control properties, otherwise  
//                        if Auto Buddy integer left (checked mark box) on microsoft will override decimal and set to whole number.
//                        Added a regular order and stock order spin control initialized range increment & decrement setting condition.
//
//  K. Hubbard  02-24-09  Modified m_castWidth spin control to increment or decrement by half inch amounts during reprovide
//                        of planned widths. Also set range minimum initialization of the Order Parm window to the editor's
//                        currently displayed width value in m_castWidth field. Order Parm value is editable by users.       
//
//  K. Hubbard  05-5-09   Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                        modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                        controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//                  Note: Acceptable database input values must be part of combo box data list items....   
//
//    ** Important Scenario Note ** : In order for 910 transmitted or saved data to be available immediately 
//                                    following validation or saves, field must be also present in appropriate
//                                    location in CasterScenArchiver.cpp !!! Also this will update the desired 
//                                    table in CsdaTest database.   
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
#include "Caster.h"

// #include "colordynamics.h"


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
	m_orderedCastWidth = _T("0.0");    // changed this 1-8-09 k. hubbard
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
	m_hotrollUnitName = _T("");
	m_stockReason = _T("");    // added following 3 reason fields 5-5-09 k. hubbard
	m_stockCommercialReason = _T("");    
	m_slitReason = _T("");
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
	DDV_MinMaxDouble(pDX, m_castWidth, 20., 80.);
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
	DDX_Text(pDX, IDC_STATIC_HOTROLL_UNIT_NAME, m_hotrollUnitName);  // added this field via Wizard after adding box to dialog screen Slab Sales Cust 1-24-07 k. hubbard 
	DDX_CBString(pDX, IDC_COMBO_STOCK_REASON, m_stockReason);    // added following 3 reason fields 5-5-09 k. hubbard
	DDX_CBString(pDX, IDC_COMBO_STOCK_COMMERCIAL_REASON, m_stockCommercialReason); 
	DDX_CBString(pDX, IDC_COMBO2, m_slitReason);
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
	assert( Caster::IsValidCasterValue(m_casterNum));

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
//	 ** 1-08-09 k. hubbard Important: Have to turn Auto Buddy Integer off in dialog screen spin control properties  
//   Otherwise if Auto Buddy integer left on microsoft will override decimal and set to whole number 
	if	(m_bStock)   // stock order increment decrement spin control range setting 1-08-09 k.hubbard
	{   m_spinCastWidth.SetRange(m_castWidth,m_castWidth + 20); // comm out original 1-08-09 k. hubbard
	}
	else     // Non stock order number width increment decrement spin control range setting 1-08-09 k.hubbard  
	//  {	m_spinCastWidth.SetRange(atoi(m_minCastWidth),atoi(m_maxCastWidth) + 20); // added cstring to integer and spin control initialize 1-08-09 k. hubbard
	//	{	m_spinCastWidth.SetRange((m_castWidth),atoi(m_maxCastWidth) + 20); // added cstring to integer and spin control initialize 1-08-09 k. hubbard
	{	m_spinCastWidth.SetRange((m_castWidth),80); // added cstring to integer and spin control initialize 1-08-09 k. hubbard
	}
	//	m_spinCastWidth.SetRangeAndDelta (0.1, 1.0, 0.5); // added test 1-08-09 k. hubbard

	m_spinDispCode.SetRange(0,9);

	// the OK button is enabled if we are creating
	// or we are editing a CSOrder from an editable string.
	
	GetDlgItem(IDOK)->EnableWindow( m_bCreate || m_pCSOrder->CastString()->CanEdit() );
	GetDlgItem(IDC_COMBO_STOCK_REASON)->EnableWindow( m_bStock );  // added enable stock reason combo box for stock only 5-5-09 k. hubbard
	GetDlgItem(IDC_COMBO_STOCK_COMMERCIAL_REASON)->EnableWindow( m_bStock );  // added enable stock reason combo box for stock only 5-5-09 k. hubbard
	GetDlgItem(IDC_COMBO2)->EnableWindow(( m_bCreate || m_pCSOrder->CastString()->CanEdit()) && (! m_bStock));  // added enable slit reason combo box for edit ok only 2-26-10 k. hubbard

//	GetDlgItem(IDC_COMBO2)->EnableWindow( m_bCreate || m_pCSOrder->CastString()->CanEdit() );  // added enable slit reason combo box for edit ok only 2-26-10 k. hubbard

//	GetDlgItem(IDC_COMBO1)->CColorDynamics::OnCtlColor(CDC* pDC,, m_bStock );  // added enable stock reason combo box for stock only 5-5-09 k. hubbard

//GetDlgItem(IDC_COMBO1)->CColorDynamics::m_pbrushEnabled(CDC* pDC, m_bStock );

//GetDlgItem(IDC_COMBO1)->CColorDynamics::OnCtlColor(CDC* pDC, m_pbrushEnabled );

	
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
//		m_orderedCastWidth	= "";
		m_orderedCastWidth	= _T("0.0");  // k. hubbard 1-8-09
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
		m_provCastSlabWidthMin = 0.0;  // added these 2 fields 11-11-03 k. hubbard
		m_provCastSlabWidthMax = 0.0;  
        m_hotrollUnitName = "";  // added fields 1-24-07 k. hubbard
	}
	else {
		char buf[100];
        CString strProvMin; // added these 2 fields 09-29-05 k. hubbard
        CString strProvMax;

        m_hotrollUnitName = m_pOrder->HotrollUnitName();  // added fields 1-24-07 k. hubbard

		m_orderedCastLength	= _ltoa(long(m_pOrder->SlabLength()),buf,10);

//		m_orderedCastWidth	= _ltoa(long(m_pOrder->SlabWidth()),buf,10);
		sprintf(buf,"%2.1f",m_pOrder->SlabWidth());  // added 2 lines here 1-8-09 k. hubbard testing
		m_orderedCastWidth	= buf;

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
		// Comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
		// Modified these 2 fields to convert from double to string and then from string back to double (via atof) for
        //	display in Order Parm window. Will now handle and show 2 bytes left of decimal and minimum 1 byte to the right of decimal. 09-29-05 k. hubbard
//		m_provCastSlabWidthMin = m_pOrder->ProvSlabWidthMin(); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
//		m_provCastSlabWidthMax = m_pOrder->ProvSlabWidthMax(); // 11-11-03 k. hubbard _ltoa function converts the digits of value to a null-terminated character string 
		strProvMin.Format("%2.1f",m_pOrder->ProvSlabWidthMin());
		m_provCastSlabWidthMin = atof(strProvMin);
		strProvMax.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
		m_provCastSlabWidthMax = atof(strProvMax);

	}
}


//  These are the editable (or sometimes editable) fields.
//  If a CSOrder is supplied, the values are taken directly from it.
//  If no CSOrder is supplied, but an Order is, we take the values from the it.
//  If neither is given, we set to some standard defaults.
//  Note that in the last case, we are definitely creating a stock order.

void CCSOrderParmDlg::InitCurrentFields()
{
char buf[50];  
	if ( m_pCSOrder != 0 ) {

		m_castLength	= long(m_pCSOrder->SlabLength());	// user requested truncation to integer	
		m_castWidth		= m_pCSOrder->SlabWidth();  // original code

//		m_strDecimalWidth.Format("%2.1f",m_pCSOrder->SlabWidth());    // added 2 lines here 2-20-09 k. hubbard 
//		m_castWidth		= atof(strDecimalWidth);     // Converting cstring to double here 1-9-09 k. hubbard            
//		sprintf(buf,"%2.1f",m_pCSOrder->SlabWidth());      // added 2 lines here 1-8-09 k. hubbard testing
//		m_castWidth		= atof(buf);      // Converting cstring to double here 1-9-09 k. hubbard
//ceil

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

//		m_stockReason	= m_pCSOrder->StockReason();  // added reasons 5-5-09 k. hubbard
		m_stockReason	= ( m_bCreate && m_bStock ?  "<blank>" : m_pCSOrder->StockReason() );  // added reasons 5-5-09 k. hubbard
		m_stockCommercialReason	= ( m_bCreate && m_bStock ?  "<blank>" : m_pCSOrder->StockCommercialReason() );  // added reasons 5-5-09 k. hubbard
		m_slitReason	= ( m_bCreate && !m_bStock ?  "<blank>" : m_pCSOrder->SlitReason() );  // added slit reasons 2-26-10 k. hubbard

//		m_stockReason	= (m_pCSOrder->StockReason() == ' '    // added reasons 5-5-09 k. hubbard
//			               ? " <blank>" 
//						   : CString(m_pCSOrder->StockReason()));

		               
	}
	else if ( m_pOrder != 0 ) {
        CString strProvMinStk; // added these 2 fields 09-29-05 k. hubbard
        CString strProvMaxStk;

		m_castLength	= long(m_pOrder->SlabLength());	// user requested truncation to integer	
//		m_castWidth		= m_pOrder->SlabWidth();  // comm. out original 9-13-07 k. hubbard testing

		sprintf(buf,"%2.1f",m_pOrder->SlabWidth());      // added 2 lines here 1-8-09 k. hubbard testing
		m_castWidth		= atof(buf);      // Converting cstring to double here 1-9-09 k. hubbard

		m_condnCode		= ( m_bCreate && m_bStock ?  4782 : m_pOrder->SlabCondnCode() );
		m_numPieces		= ( m_bCreate && m_bStock ?     1 : m_pSuperScen->NumSlabsDue(m_pOrder) );
		m_slitTypeCode	= " <blank>";
//		m_dispoCode		= 2;  // Changed default stock disposition code from 4 to 2 in order to direct slab material to #2 Slab Yard for #2 BOF. maint k. hubbard  01-31-06
 		m_dispoCode		= 4;  // Changed default stock disposition code from 2 back to 4 in order to direct slab material to #4 Slab Yard for Slab Logistics project maint k. hubbard  01-24-08

		m_changeIndic	= " ";
		m_expCode		= (m_pOrder->PgmExposure() == 1 ? "E" : "U");

		m_planSteelLengthMin	= long(m_pOrder->MinSlabLength());	// user requested truncation to integer	
		m_planSteelLengthMax	= long(m_pOrder->MaxSlabLength());	// user requested truncation to integer	

		// Modified these 2 fields to convert from double to string and then from string back to double (via atof) for
        //	display in Order Parm window. Will now handle and show 2 bytes left of decimal and minimum 1 byte to the right of decimal. 09-29-05 k. hubbard
//		m_provCastSlabWidthMin  = (m_pOrder->ProvSlabWidthMin()); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
//		m_provCastSlabWidthMax  = (m_pOrder->ProvSlabWidthMax()); // comes from Order.h, which comes from DalyPlanOrdersOnlySet 11-11-03 k. hubbard
		strProvMinStk.Format("%2.1f",m_pOrder->ProvSlabWidthMin());
		m_provCastSlabWidthMin = atof(strProvMinStk);
		strProvMaxStk.Format("%2.1f",m_pOrder->ProvSlabWidthMax());
		m_provCastSlabWidthMax = atof(strProvMaxStk);

		m_stockReason	= 'W';   // added reasons 5-5-09 k. hubbard
		m_stockCommercialReason	= "<blank>";   // added reasons 5-5-09 k. hubbard

	}
	else {
		// creating, definitely stock
		m_castLength	= 350;
		m_castWidth		= 55;
		m_condnCode		= 4782 ;
		m_numPieces		= 1 ;
		m_slitTypeCode  = " <blank>";
//		m_dispoCode		= 2;    // Changed default stock disposition code from 4 to 2 in order to direct slab material to #2 Slab Yard for #2 BOF. maint k. hubbard  01-31-06 
		m_dispoCode		= 4;    // Changed default stock disposition code from 2 back to 4 in order to direct slab material to #4 Slab Yard for Slab Logistics project maint k. hubbard  01-24-08
 
		m_changeIndic	= " ";
		m_expCode		= "U";
		m_planSteelLengthMin = m_castLength;
		m_planSteelLengthMax = m_castLength;

		m_stockReason	= "<blank>";   // added reasons 5-5-09 k. hubbard
		m_stockCommercialReason	= "<blank>";   // added reasons 5-5-09 k. hubbard

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
//		m_stockReason	= CString(m_pCSOrder->StockReason());  // added reasons 5-5-09 k. hubbard
//		CString(m_pCSOrder->StockReason()) = m_stockReason;  // added reasons 5-5-09 k. hubbard
		m_pCSOrder->StockReason() = m_stockReason;  // added reasons 5-5-09 k. hubbard
		m_pCSOrder->StockCommercialReason() = m_stockCommercialReason;  // added reasons 5-5-09 k. hubbard

	}

	m_pCSOrder->SlitReason() = m_slitReason;  // added slit reasons 2-26-10 k. hubbard

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

//	char m_hotrollUnitCode = HotrollUnitName[0];

	CCSOrder::ComputeSlabWidthRelatedParms(m_casterNum,
										   m_pOrder,
										   m_useTransWidths,
										   width,
										   m_condnCode,
										   m_ciCode,    // K. Hubbard 1-29-03 7BF Reline
										   m_hotrollUnitName,  // 1-24-07 k. hubbard Slab Sales Customer
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
	//	double holdWidth;

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

//	m_castWidth = pNMUpDown->iPos + pNMUpDown->iDelta;   // original code will auto advance 1" 
//	pNMUpDown->iPos will equal text window's m_castWidth value. pNMUpDown->iDelta will equal positive or negative 1 based on direction of spin control arrow. 2-24-09 k. hubbard  
//	holdWidth = pNMUpDown->iPos + 0.5;   // code will auto advance 1/2" 

	if ( m_pCSOrder != 0 && m_pOrder != 0 ) {   // adjust non stock increment and decrement
		UpdateData(true);

		if (pNMUpDown->iDelta > 0) {    
			m_castWidth = (m_castWidth * 1 + 0.5)/ 1.0;  // will increment one half inch (1/2") with each click 2-24-09 k. hubbard
//			holdWidth = m_castWidth + pNMUpDown->iDelta - 0.5;    
		}
		else
		{
			m_castWidth = (m_castWidth * 1 - 0.5)/ 1.0;  // will decrement one half inch (1/2") with each click 2-24-09 k. hubbard
//			holdWidth = m_castWidth + pNMUpDown->iDelta + 0.5;
		}

//			m_castWidth = ceil(m_castWidth * 10 + 0.5)/ 10.0 ;
//			m_castWidth*=100;
//			m_castWidth/=100.5; operator that divides and assign value 
//			pow(10.0,2);  raise power

//			if (m_castWidth > m_castWidth + .49) {   
///				m_castWidth = ceil(m_castWidth); }
///			else 
///				m_castWidth = floor(m_castWidth);

		UpdateWidthRelatedParms(m_castWidth); 
		UpdateData(false);
	}
	else {       // added this stock order spin control increment & decrement condition lines on 2-24-09 k. hubbard
		UpdateData(true);
//			m_castWidth = pNMUpDown->iPos +  widthDecimalScale;   // added scale to this line on 1-8-09 k. hubbard
        if (pNMUpDown->iDelta > 0) {
			m_castWidth = (m_castWidth * 1 + 0.5)/ 1.0;  // will increment one half inch (1/2") with each click 2-24-09 k. hubbard
		}
		else
			m_castWidth = (m_castWidth * 1 - 0.5)/ 1.0;  // will decrement one half inch (1/2") with each click 2-24-09 k. hubbard

		UpdateData(false);
	}


	*pResult = 0;

}

//
///  Dormant code, commented out k. hubbard
#if 0

		if( 0 &lt; sscanf(m_Edit,"%lf",&value) )
{
long lVal;
if(pNMUpDown-&gt;iDelta&lt;0) lVal=(long)(value*100-0.5);
else lVal=(long)(value*100+0.5);
long changeVal;
if(abs(lVal)&lt;100 || (abs(lVal)==100 && pNMUpDown-&gt;iDelta&lt;0) ) changeVal=1;
else if(abs(lVal)&lt;1000 || (abs(lVal)==1000 && pNMUpDown-&gt;iDelta&lt;0) ) changeVal=10;
else changeVal=100;
if(pNMUpDown-&gt;iDelta&lt;0)
{
lVal-=changeVal;
}
else
{
lVal+=changeVal;
}
value=(double)lVal;
value/=100;
m_Edit.Format("%lf", value);

#endif
///
///



void CCSOrderParmDlg::OnButtonClearChangeIndicator() 
{
	UpdateData(TRUE);

	m_changeIndic	= " ";

	UpdateData(FALSE);
}


bool CCSOrderParmDlg::XferToCSOrder( CCSOrder* pCSOrder )  // Will actually save into CSOrder here after validation (Save) step 2-26-10 k. hubbard 
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
	
	if ( pCSOrder->SlitReason() != m_slitReason ) {     // Actually saved after validation here, added reason 2-26-10 k. hubbard
		pCSOrder->SlitReason(m_slitReason);
		changed = true;
	}

	if ( m_bStock ) {

		if ( pCSOrder->SlabCondnCode() != m_condnCode ) {
			pCSOrder->SlabCondnCode(m_condnCode);
			changed = true;
		}

		if ( pCSOrder->LotSpec() != m_spec ) {
			pCSOrder->LotSpec(m_spec,m_casterNum);
			changed = true;
		}

		if ( pCSOrder->StockReason() != m_stockReason ) {     // added reason 5-5-09 k. hubbard
			pCSOrder->StockReason(m_stockReason,m_casterNum);
//			pCSOrder->StockReason(m_stockReason);
			changed = true;
		}

		if ( pCSOrder->StockCommercialReason() != m_stockCommercialReason ) {     // added reason 5-5-09 k. hubbard
			pCSOrder->StockCommercialReason(m_stockCommercialReason,m_casterNum);
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

//   Did not use condition, but it works here and may need later 1-31-06 k. hubbard
//    if ( m_useTransWidths ) {          // added maint. k. hubbard 1-31-06
//				(m_pCSOrder->IsTransitionChecked() == false);
//				}

	UpdateWidthRelatedParms(m_castWidth);  

	UpdateData(false);
	
}

void CCSOrderParmDlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}

