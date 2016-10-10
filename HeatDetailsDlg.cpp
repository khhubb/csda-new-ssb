// HeatDetailsDlg.cpp : implementation file of HeatDetailsDlg class 
//
// Program:      
//
// Description: This prog initializes display of the Heat Details screen 
//              and establishes shows some validation warnings. 
//
// Maintenance:  
//    Name          Date       Description
// -------------  --------   -----------------------------------------
// K. Hubbard     04/03/07   Populate Order Detail tab of Heat Details with order spec    
//                           when available. per P. Velasco
//
///////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "csda.h"
#include "HeatDetailsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Snapshot.h"
#include "SuperScen.h"
#include "CasterScen.h"
#include "CastString.h"
#include "CSOrder.h"
#include "Order.h"
#include "Spec.h"
#include "SpecMgr.h"
#include "MsgDlg.h"
#include "TabNumDlg.h"
#include "SpecCmp.h"
#include "HeatSpecCrossApp.h"
#include "SpecSelectionDlg.h"
#include "CasterScenArchiver.h"
#include "ScenMgr.h"
#include "LongOpn.h"
#include "Caster.h"



/////////////////////////////////////////////////////////////////////////////
//	CHeatDetailsDlg dialog
//
//	An extremely important dialog.
//  Shows details on heats in a CCastString.
//  Display elements are:
//		a list showing the heats
//      a tabbed window with tabs for
//        - showing chemistry details for the selected heat (grid)
//        - order details for the selected heat (grid)
//		  - validation error messages for the string (list)
//
//  commands include marking the heats, setting specs, chems, and tab#s, 
//     and validating the string.
//
//
//	Usage:
//
//		CHeatDetailsDlg dlg;
//		dlg.m_pCastString	= pString;
//		dlg.m_pSuperScen	= m_pSuperScen;
//
//		dlg.DoModal();
//
//
//	You should assume that the string has been modified when this exits.
//
/////////////////////////////////////////////////////////////////////////////



CHeatDetailsDlg::CHeatDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeatDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeatDetailsDlg)
	m_stringId = _T("");
	//}}AFX_DATA_INIT


	m_pCastString	 = 0;
	m_pCasterScen	 = 0;
	m_pSuperScen	 = 0;

	//  set up backlinks on the embedded grids
	m_wndDetailsGrid.m_pHDDlg	= this;
	m_wndChemsGrid.m_pHDDlg		= this;

	// no tab active yet.
	m_pActiveTabWnd	= 0;

}


void CHeatDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeatDetailsDlg)
	DDX_Control(pDX, IDC_COMBO_VALIDATION_ERROR, m_comboValidnError);
	DDX_Control(pDX, IDC_LIST_VALIDATION_STRINGS, m_listValidation);
	DDX_Control(pDX, IDC_LIST_ORDERS, m_listOrders);
	DDX_Control(pDX, IDC_TAB1, m_tabDetail);
	DDX_Text(pDX, IDC_STRING_ID, m_stringId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeatDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CHeatDetailsDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET_CHEMS, OnButtonSetChems)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_SET_SPECS, OnButtonSetSpecs)
	ON_BN_CLICKED(IDC_BUTTON_SET_TAB_NUMS, OnButtonSetTabNums)
	ON_BN_CLICKED(IDC_BUTTON_VALIDATE, OnButtonValidate)
	ON_BN_CLICKED(IDC_BUTTON_MARK_ALL, OnButtonMarkAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_MARKS, OnButtonClearMarks)
	ON_CBN_SELCHANGE(IDC_COMBO_VALIDATION_ERROR, OnSelchangeComboValidationError)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ORDERS, OnDblclkListOrders)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ORDERS1, OnButtonEditOrders1)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ORDERS2, OnButtonEditOrders2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VALIDATION_STRINGS, OnItemchangedListValidationStrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeatDetailsDlg message handlers

//
//  Gives the order in which elements are to be listed in the elements grid.
//

// 	static 
vector<int> CHeatDetailsDlg::m_eltSeq;


//
//  We used to keep our own list of the heats, for some reason.
//  This is a vestige.
//

vector<CCastStringHeat>& CHeatDetailsDlg::EditedHeats()
{
	return (m_pCastString->Heats()); 
}


BOOL CHeatDetailsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set up the details grid
	m_wndDetailsGrid.SubclassDlgItem(IDC_GRID_HEAT_DETAILS,this);
	m_wndDetailsGrid.Initialize();
	m_wndDetailsGrid.GetParam()->EnableUndo(FALSE);

	// Set up the chems grid
	m_wndChemsGrid.SubclassDlgItem(IDC_GRID_HEAT_CHEMS,this);
	m_wndChemsGrid.Initialize();
	m_wndChemsGrid.GetParam()->EnableUndo(FALSE);


	// Set up the tabs.
	TC_ITEM tabCtrlItem;
	tabCtrlItem.mask = TCIF_TEXT;

    tabCtrlItem.pszText = "Orders";
	m_tabDetail.InsertItem( 0, &tabCtrlItem );	// DETAIL_ORDERS
    tabCtrlItem.pszText = "Heat chems";
	m_tabDetail.InsertItem( 1, &tabCtrlItem );	// DETAIL_CHEMS
	tabCtrlItem.pszText = "Validation";
	m_tabDetail.InsertItem( 2, &tabCtrlItem );	// DETAIL_VALIDATION

	m_wndChemsGrid.ShowWindow(SW_HIDE);
	m_listOrders.ShowWindow(SW_HIDE);
	m_listValidation.ShowWindow(SW_HIDE);

	m_tabDetail.SetCurSel(DETAIL_ORDERS);
	m_detailType = DETAIL_ORDERS;
	OnSelchangeTab1(0,0);

	SetOrdersListHeader();
	SetValidationListHeader();

	// user must supply these
	assert( m_pCastString != 0 );
	assert( m_pSuperScen  != 0 );

	// other useful goodies, computable from required parms
	m_pCasterScen	= m_pCastString->CasterScen();

	// No selection
	m_currentHeatIndex = -1;

	// populate the details grid
	InitializeHeatDetailsRows();

	// Set a current heat
	if ( m_wndDetailsGrid.GetRowCount() > 0 )
		m_wndDetailsGrid.SetCurrentCell(1,HEAT_COL_SPEC);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




////////////////////////////////////////////////////////////////
//	
//	miscellaneous
//	
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//	
//	the heat details grid
//	
////////////////////////////////////////////////////////////////


void CHeatDetailsDlg::InitializeHeatDetailsRows()
{
	// ## add ROWCOL cast
	m_wndDetailsGrid.SetRowCount((ROWCOL)EditedHeats().size());

	int index = 0;

	m_wndDetailsGrid.LockUpdate(TRUE);

	for ( vector<CCastStringHeat>::const_iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih, ++index ) 

		UpdateDetailRowFromHeat( index+1, (*ih) );

	m_wndDetailsGrid.LockUpdate(FALSE);
	m_wndDetailsGrid.Redraw();
}


#ifdef setv
#undef setv
#endif

#define setv(col,val) m_wndDetailsGrid.SetValueRange( CGXRange( nrow, col), val)

void CHeatDetailsDlg::UpdateDetailRowFromHeat(ROWCOL nrow, const CCastStringHeat& rHeat)
{
	CString strTab;
	CString strDur;
	CString strCMin;
	CString strCMax;
	CString strMnMin;
	CString strMnMax;
	CString strSigEl;
	CString strSigMin;
	CString strSigMax;
	bool sigEditable;
	bool cRangeOk;
	bool mnRangeOk;
	bool sigRangeOk;


	strTab.Format("%04d",rHeat.TabNum());
	strDur.Format("%d min.",
				  (rHeat.Duration().GetSeconds() >= 30 
				   ? rHeat.Duration().GetTotalMinutes() + 1
				   : rHeat.Duration().GetTotalMinutes() )  );
	if ( rHeat.MinChem(CChem::ELT_C) == 0.0 )
		strCMin = "";
	else
		strCMin.Format ("%5.3f",rHeat.MinChem(CChem::ELT_C));

	if ( rHeat.MaxChem(CChem::ELT_C) == 100.0 )
		strCMax = "";
	else
		strCMax.Format ("%5.3f",rHeat.MaxChem(CChem::ELT_C));

	CChem::Bound cDelta = rHeat.MaxChem(CChem::ELT_C) - rHeat.MinChem(CChem::ELT_C);
	cRangeOk = (rHeat.MaxChem(CChem::ELT_C) < 0.024 
				?  cDelta >= 0.002
				:  cDelta >= 0.02 );


	if ( rHeat.MinChem(CChem::ELT_MN) == 0.0 )
		strMnMin = "";
	else
		strMnMin.Format("%5.3f",rHeat.MinChem(CChem::ELT_MN));

	if ( rHeat.MaxChem(CChem::ELT_MN) == 100.0 )
		strMnMax = "";
	else
		strMnMax.Format("%5.3f",rHeat.MaxChem(CChem::ELT_MN));

	mnRangeOk = rHeat.MaxChem(CChem::ELT_MN) - rHeat.MinChem(CChem::ELT_MN) >= 0.01;


	if ( rHeat.SignifEl() == -1 ) {
		strSigEl	= "";
		strSigMin	= "";
		strSigMax	= "";
		sigEditable = false;
		sigRangeOk = true;
	}
	else {
		strSigEl = CChem::ToString( rHeat.SignifEl() );
		CChem::Bound sigMin = rHeat.MinChem( rHeat.SignifEl() );
		CChem::Bound sigMax = rHeat.MaxChem( rHeat.SignifEl() );

		if ( sigMin == 0.0 )
			strSigMin = "";
		else 
			strSigMin.Format("%5.3f",sigMin);

		if ( sigMax == 100.0 )
			strSigMax = "";
		else
			strSigMax.Format("%5.3f",sigMax);

		sigRangeOk =  rHeat.MaxChem(rHeat.SignifEl()) - rHeat.MinChem(rHeat.SignifEl()) >= .08;

		sigEditable = rHeat.SpecPtr() != 0
						&& rHeat.SpecPtr()->GetChemRange(rHeat.SignifEl()) != 0
						&& rHeat.SpecPtr()->GetChemRange(rHeat.SignifEl())->IsAppSpecific();
	}

	bool cEditable = rHeat.SpecPtr() != 0
		&& rHeat.SpecPtr()->GetChemRange(CChem::ELT_C) != 0
		&& rHeat.SpecPtr()->GetChemRange(CChem::ELT_C)->IsAppSpecific();
	
	bool mnEditable = rHeat.SpecPtr() != 0
		&& rHeat.SpecPtr()->GetChemRange(CChem::ELT_MN) != 0
		&& rHeat.SpecPtr()->GetChemRange(CChem::ELT_MN)->IsAppSpecific();;


	setv( HEAT_COL_MARK,		( rHeat.IsMarked() ? short(1) : short(0) ));
	setv( HEAT_COL_TAB_NUM,		strTab				);
	setv( HEAT_COL_TONS,		rHeat.HeatTons()	);
	setv( HEAT_COL_DURATION,	strDur				);
	setv( HEAT_COL_SPEC,		rHeat.Spec()		);
	//setv( HEAT_COL_CONDN,	    rHeat.CondnCode()	);
	setv( HEAT_COL_C_MIN,		strCMin				);	
	setv( HEAT_COL_C_MAX,		strCMax				);
	setv( HEAT_COL_MN_MIN,		strMnMin			);
	setv( HEAT_COL_MN_MAX,		strMnMax			);
	setv( HEAT_COL_SIG_EL,		strSigEl			);
	setv( HEAT_COL_SIG_EL_MIN,	strSigMin			);
	setv( HEAT_COL_SIG_EL_MAX,	strSigMax			);
	setv( HEAT_COL_COMMENT,		rHeat.Comment()		);


	
	CGXStyle style;

	style.SetEnabled( sigEditable );
	style.SetTextColor( sigRangeOk ? GXSYSCOLOR(COLOR_WINDOWTEXT ) : RGB(255,0,0) );
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_SIG_EL_MIN), style);
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_SIG_EL_MAX), style);

	style.SetEnabled( cEditable );
	style.SetTextColor( cRangeOk ? GXSYSCOLOR(COLOR_WINDOWTEXT) : RGB(255,0,0) );
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_C_MIN), style);
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_C_MAX), style);
	
	style.SetEnabled( mnEditable );
	style.SetTextColor( mnRangeOk ? GXSYSCOLOR(COLOR_WINDOWTEXT) : RGB(255,0,0) );
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_MN_MIN), style);
	m_wndDetailsGrid.SetStyleRange( CGXRange(nrow,HEAT_COL_MN_MAX), style);
}




BOOL CHeatDetailsDlg::OnValidateCell(ROWCOL nrow, ROWCOL ncol)
{
	// column or row header
	if ( nrow < 1 || ncol < 1 )
		return TRUE;

	CString s;
	m_wndDetailsGrid.GetCurrentCellControl( )->GetCurrentText(s);

	switch ( ncol ) {
	case HEAT_COL_TONS:
	case HEAT_COL_DURATION:
	case HEAT_COL_SIG_EL:
		assert(0); // can't happen -- not editable
		break;

	case HEAT_COL_SPEC:
		{	
			if ( s != "" ) {

				if ( s.GetLength() != 7 
					 ||
					 TheSnapshot.SpecMgr().FindSpecMaybe(s,m_pCasterScen->Caster()) == 0 ) {

					m_wndDetailsGrid.SetWarningText("Please enter a valid spec.");
					return FALSE;
				}
			}
		}
		break;

	case HEAT_COL_TAB_NUM:
		{
			long tabNum = atol(s);

			if ( tabNum != 0 ) {

				// we know this is in the range [0,99999] because of range limit on cell

				int firstDigit = tabNum/10000;
				int secondDigit = (tabNum/1000) % 10;
				int thirdDigit = (tabNum / 100) % 10;

				if ( firstDigit != 9
					 &&
					 (m_pCasterScen->Caster() == Caster::C2 || m_pCasterScen->Caster() == Caster::C3)
					 && 
					 firstDigit != m_pCasterScen->Caster() ) {

					 m_wndDetailsGrid.SetWarningText("First digit of tab# must be 9 or same as caster.");
					 return FALSE;
				}

				if ((m_pCasterScen->Caster() == Caster::C2 || m_pCasterScen->Caster() == Caster::C3)
					 &&
					 secondDigit != 9 ) {
					m_wndDetailsGrid.SetWarningText("Second digit of tab# must be 9 (for #2BOF).");
					return FALSE;
				}

				if (m_pCasterScen->Caster() == Caster::C4) {
					if (firstDigit != 1) {
						m_wndDetailsGrid.SetWarningText("First digit of tab# for caster 4 must be a 1.");
						return FALSE;
					}
					if (secondDigit != 1) {
						m_wndDetailsGrid.SetWarningText("Second digit of tab# for caster 4 must be a 1.");
						return FALSE;
					}
					if (!(0 <= thirdDigit && thirdDigit <= 4)) {
						m_wndDetailsGrid.SetWarningText("Third digit of tab# for caster 4 must be a 0-4.");
						return FALSE;
					}
				}

				if (m_pCasterScen->Caster() == Caster::C4) {
					if (firstDigit != 5) {
						m_wndDetailsGrid.SetWarningText("First digit of tab# for caster 5 must be a 5.");
						return FALSE;
					}
					if (secondDigit != 2) {
						m_wndDetailsGrid.SetWarningText("Second digit of tab# for caster 5 must be a 2.");
						return FALSE;
					}
					if (!(5 <= thirdDigit && thirdDigit <= 9)) {
						m_wndDetailsGrid.SetWarningText("Third digit of tab# for caster 5 must be a 5-9.");
						return FALSE;
					}
				}
			}
		}
		break;

	case HEAT_COL_MARK:
	case HEAT_COL_C_MIN:
	case HEAT_COL_C_MAX:
	case HEAT_COL_MN_MIN:
	case HEAT_COL_MN_MAX:
	case HEAT_COL_SIG_EL_MIN:
	case HEAT_COL_SIG_EL_MAX:
	case HEAT_COL_COMMENT:
		// no checks here
		break;
	}

	return TRUE;
}


BOOL CHeatDetailsDlg::OnEndEditingCell(ROWCOL nrow, ROWCOL ncol)
{
	// column or row header
	if ( nrow < 1 || ncol < 1 )
		return TRUE;

	int heatIndex = nrow - 1;
	CCastStringHeat& rHeat = EditedHeats()[heatIndex];

	CString s = m_wndDetailsGrid.GetValueRowCol(nrow,ncol);

	switch ( ncol ) {
	case HEAT_COL_TONS:
	case HEAT_COL_DURATION:
		assert(0); // can't happen -- not editable
		break;

	case HEAT_COL_TAB_NUM:
		rHeat.TabNum(atol(s));
		break;

	case HEAT_COL_SPEC:
		if ( s != rHeat.Spec() ) {
			rHeat.SetSpec(s,m_pCastString->Id().Caster());

			if ( rHeat.SpecPtr() != 0 )
				SetChemsForHeat(rHeat,heatIndex);
			else
				rHeat.ResetChems();
		}
		break;

	case HEAT_COL_C_MIN:
	case HEAT_COL_C_MAX:
	case HEAT_COL_MN_MIN:
	case HEAT_COL_MN_MAX:
	case HEAT_COL_SIG_EL_MIN:
	case HEAT_COL_SIG_EL_MAX:
		{
			CChem::Element elt = (ncol == HEAT_COL_C_MIN || ncol == HEAT_COL_C_MAX 
								  ? CChem::ELT_C
								  : (ncol == HEAT_COL_MN_MIN || ncol == HEAT_COL_MN_MAX
								     ? CChem::ELT_MN
									 : rHeat.SignifEl() ));

			CChem::Bound bound = atof(s);

			bool isMin = ( ncol == HEAT_COL_C_MIN 
				           || 
						   ncol == HEAT_COL_MN_MIN 
						   || 
						   ncol == HEAT_COL_SIG_EL_MIN);

			if ( isMin )
				rHeat.MinChem(elt,bound);
			else
				rHeat.MaxChem(elt,bound);

			int caster = m_pCasterScen->Caster();
			rHeat.FixAimChem(elt,caster);


			// It would be nice to update only the row of the element involved,
			// but I was too lazy to figure out the row where it should be saved.

			InitializeHeatChemsRows();
		}
		break;

	case HEAT_COL_MARK:
		// do nothing
		break;

	case HEAT_COL_COMMENT:
		rHeat.Comment(s);
		break;
	}

	return TRUE;	
}


void CHeatDetailsDlg::OnMovedCurrentCell(ROWCOL nrow,ROWCOL ncol)
{
#if 0
	afxDump << "CHeatDetailsDlg::OnMovedCurrentCell: " << nrow << "," << ncol << "\n";
#endif


	// column or row header
	if ( nrow < 1 || ncol < 1 )
		return;

	if ( nrow-1 == m_currentHeatIndex )
		return;

	//m_wndDetailsGrid.LockUpdate(TRUE);
	ResetHeatDetailRowSelection();

	m_currentHeatIndex = nrow-1;
	SetOrdersListItems();
	InitializeHeatChemsRows();

	SetHeatDetailRowSelection();
	//m_wndDetailsGrid.LockUpdate(FALSE);
	m_wndDetailsGrid.RedrawRowCol(CGXRange().SetTable());

}



void CHeatDetailsDlg::SetHeatDetailRowSelection()
{
	if ( m_currentHeatIndex < 0  
		 ||
		 m_currentHeatIndex >=  m_wndDetailsGrid.GetRowCount() )
		 
		 return;

	CGXRange range;
	range.SetRows(m_currentHeatIndex+1);

	m_wndDetailsGrid.SetStyleRange(range,
					               CGXStyle().SetTextColor(RGB(0,0,128))
											 .SetFont(CGXFont().SetBold(TRUE)));
	m_wndDetailsGrid.RedrawRowCol(range);
}


void CHeatDetailsDlg::ResetHeatDetailRowSelection()
{
	if ( m_currentHeatIndex < 0  
		 ||
		 m_currentHeatIndex >=  m_wndDetailsGrid.GetRowCount() )
		 
		 return;

	CGXRange range;
	range.SetRows(m_currentHeatIndex+1);

	m_wndDetailsGrid.SetStyleRange(range,
					               CGXStyle().SetTextColor(GXSYSCOLOR(COLOR_WINDOWTEXT))
											 .SetFont(CGXFont().SetBold(FALSE)));
	m_wndDetailsGrid.RedrawRowCol(range);
}



void CHeatDetailsDlg::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	// column or row header
	if ( nRow < 1 || nCol < 1 )
		return;

	assert( nCol == HEAT_COL_MARK);
	CString s = m_wndDetailsGrid.GetValueRowCol(nRow,nCol);

	int heatIndex = nRow - 1;
	CCastStringHeat& rHeat = EditedHeats()[heatIndex];

	rHeat.IsMarked( s == "1" );
}


////////////////////////////////////////////////////////////////
//	
//	The tab window
//	
////////////////////////////////////////////////////////////////


void CHeatDetailsDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int sel = m_tabDetail.GetCurSel();

	CWnd* pTopWnd;

	switch ( sel ) {

	case DETAIL_ORDERS:
		pTopWnd = &m_listOrders;
		break;

	case DETAIL_CHEMS:
		pTopWnd = &m_wndChemsGrid;
		break;

	case DETAIL_VALIDATION:
		pTopWnd = &m_listValidation;
		break;

	default:
		assert(0);
	}

	m_detailType = DetailType(sel);

	if ( m_pActiveTabWnd != 0 )
		m_pActiveTabWnd->ShowWindow(SW_HIDE);

	m_pActiveTabWnd = pTopWnd;

	m_pActiveTabWnd->SetFocus();
	m_pActiveTabWnd->SetActiveWindow();
	m_pActiveTabWnd->ShowWindow(SW_SHOW);
	
	m_tabDetail.Invalidate();
	m_pActiveTabWnd->Invalidate();


	//pTopWnd->SetWindowPos(&wndTop,0,0,10,10,
	//				  SWP_SHOWWINDOW| SWP_NOMOVE | SWP_NOSIZE);

	if ( pResult != 0 )
		*pResult = 0;
}




////////////////////////////////////////////////////////////////
//	
//	Orders list
//	
////////////////////////////////////////////////////////////////


void CHeatDetailsDlg::SetOrdersListHeader()
{
	
	static vector<CString> orderStrings;
	static vector<CString> orderFormats;
	static vector<int>     orderWidths;


	if ( orderStrings.empty() ) {
		orderStrings.push_back("Order#");
		orderFormats.push_back("9999999-MMMMM");

		orderStrings.push_back("Strand");
		orderFormats.push_back("");

		orderStrings.push_back("#Pces");
		orderFormats.push_back("99");

		orderStrings.push_back("Tons");
		orderFormats.push_back("999");

		orderStrings.push_back("SlWd");
		orderFormats.push_back("99");

		orderStrings.push_back("HbWd");
		orderFormats.push_back("99.9");

		orderStrings.push_back("Condn");
		orderFormats.push_back("9999");

		orderStrings.push_back("DISPLT");
		orderFormats.push_back("9M");

		orderStrings.push_back("Prio");
		orderFormats.push_back("99-9");

		orderStrings.push_back("Spec");
		orderFormats.push_back("9999999");

		orderStrings.push_back("C");
		orderFormats.push_back("9.999 - 9.999");

		orderStrings.push_back("Mn");
		orderFormats.push_back("9.999 - 9.999");

		orderStrings.push_back("Signif");
		orderFormats.push_back("XX: 9.999 - 9.999");

		orderStrings.push_back("Comment");
		orderFormats.push_back("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

		{
			int num=0;
			for ( vector<CString>::iterator is = orderStrings.begin();
				  is != orderStrings.end();
				  ++is, ++num ) {

				int width = 15+max(m_listOrders.GetStringWidth((*is)),
					               m_listOrders.GetStringWidth( orderFormats[num] ));
		
				orderWidths.push_back( width );
			}
		}
	}

	int num = 0;
	for ( vector<CString>::iterator is = orderStrings.begin();
		  is != orderStrings.end();
		  ++is, ++num )

		m_listOrders.InsertColumn(num,(*is),LVCFMT_RIGHT,orderWidths[num],num);
}




void CHeatDetailsDlg::SetOrdersListItems()
{
	m_listOrders.DeleteAllItems();

	if ( m_currentHeatIndex < 0 )
		return;

	const vector<CCSOrder*>& strand1 = m_pCastString->Strand(1);
	const vector<CCSOrder*>& strand2 = m_pCastString->Strand(2);

	assert ( 0 <= m_currentHeatIndex && m_currentHeatIndex < EditedHeats().size() );

	int num = 0;

	{
		int orderNum = 0;

		for ( vector<CCSOrder*>::const_iterator io = strand1.begin();
			  io != strand1.end();
			  ++io, ++orderNum )
			if ( (*io)->HeatSeqNum() == m_currentHeatIndex ) {
			  SetOrdersListItem(num,m_currentHeatIndex,(*io),1,orderNum);
			  ++num;
			}
	}

	{
		int orderNum = 0;

		for ( vector<CCSOrder*>::const_iterator io = strand2.begin();
			  io != strand2.end();
			  ++io, ++orderNum )
			if ( (*io)->HeatSeqNum() == m_currentHeatIndex ) {
			  SetOrdersListItem(num,m_currentHeatIndex,(*io),2,orderNum);
			  ++num;
			}
	}
}





void CHeatDetailsDlg::SetOrdersListItem(int itemNum,
										int hIndex,
										const CCSOrder* pOrder,
										int strandNum,
										int orderNum)
{
	char buf[50];
	
	// OrderNum
	// FP change
	//m_listOrders.InsertItem(itemNum, _ltoa( pOrder->OrderNum(), buf, 10) );
	m_listOrders.InsertItem(itemNum, pOrder->FpOrderNum() );

	// strandNum
	m_listOrders.SetItemText( itemNum, 1, _itoa( strandNum,			buf, 10) );

	// #pieces
	m_listOrders.SetItemText( itemNum, 2, _itoa( pOrder->NumPieces(),	buf, 10) );

	// tons
	m_listOrders.SetItemText( itemNum, 3, _itoa( pOrder->Tons(),		buf, 10) );

	// slab width
	m_listOrders.SetItemText( itemNum, 4, _itoa( pOrder->SlabWidth(),	buf, 10) );
	
	// hb width
	if ( pOrder->Order() == 0 )
		sprintf(buf,"???");
	else
		sprintf(buf,"%4.1f",pOrder->Order()->HbWidth());
	m_listOrders.SetItemText( itemNum, 5, buf );

	// slab condn code
	m_listOrders.SetItemText( itemNum, 6, _itoa( pOrder->SlabCondnCode(), buf, 10) );

	// slitTypeCode and DispCode
	sprintf(buf,"%d-%c",
			pOrder->DispCode(),
			(pOrder->SlitTypeCode() == ' ' 
			 ? '_'
			 : pOrder->SlitTypeCode()));

	m_listOrders.SetItemText( itemNum, 7, buf);

	// prio
	if ( pOrder->Order() == 0 )
		sprintf(buf,"??-?");
	else
		sprintf(buf,"%02d-%1d",
				pOrder->Order()->CommodityPriority(),
				pOrder->Order()->OrderPriority());
	m_listOrders.SetItemText( itemNum, 8, buf);

	// spec
//	m_listOrders.SetItemText( itemNum, 9, pOrder->LotSpec()); comm. out for condition below 

	//   Populate Order Detail tab of Heat Details with order spec when available maint. 4-03-07 k. hubbard 
	if ( pOrder->Order() == 0 )
	 	m_listOrders.SetItemText( itemNum, 9, pOrder->LotSpec());
	else
		m_listOrders.SetItemText( itemNum, 9, pOrder->Order()->OrderedSpec());

	CString strChem;

	CChem::Bound minC;
	CChem::Bound maxC;
	CChem::Bound minMn;
	CChem::Bound maxMn;
	CChem::Bound minSig;
	CChem::Bound maxSig;
	CString sigSym;
	bool hasReasonCode;

	bool cOk   = pOrder->GetChemRange( CChem::ELT_C,  minC,  maxC,  hasReasonCode );
	bool mnOk  = pOrder->GetChemRange( CChem::ELT_MN, minMn, maxMn, hasReasonCode );
	bool sigOk = false;

	if ( pOrder->Order() == 0 ) {

		minSig =   0.0;
		maxSig = 100.0;
		sigSym = "";
		sigOk  = false;

	}
	else {
		minSig = pOrder->Order()->MinSignifEl();
		maxSig = pOrder->Order()->MaxSignifEl();
		sigSym = pOrder->Order()->SignifElSym();
		sigOk  = minSig != 0.0 || maxSig != 100.0;
	}


	// Carbon
	if ( cOk )
		strChem.Format("%5.3f - %5.3f",minC,maxC);
	else
		strChem.Format("-");

	m_listOrders.SetItemText( itemNum,10, LPCTSTR(strChem) );

	// Mn
	if ( mnOk )
		strChem.Format("%5.3f - %5.3f",minMn,maxMn);
	else
		strChem.Format("-");

	m_listOrders.SetItemText( itemNum,11, LPCTSTR(strChem) );

	// Signif el
	if ( sigOk )
		strChem.Format("%s: %5.3f - %5.3f",LPCTSTR(sigSym),minSig,maxSig);
	else
		strChem.Format("-");
	
	m_listOrders.SetItemText( itemNum,12, LPCTSTR(strChem) );


	//m_listOrders.SetItemText( itemNum,12, LPCTSTR(pOrder->Comment()));

	// encode the strandNum and index

	m_listOrders.SetItemData( itemNum, MAKELONG(orderNum,strandNum) );
}



////////////////////////////////////////////////////////////////
//	
//	Validation strings list
//	
////////////////////////////////////////////////////////////////


	


void CHeatDetailsDlg::SetValidationListHeader()
{
	CCastStringHeatValidnError::SetHeader(m_listValidation);
}





void CHeatDetailsDlg::SetValidationListItems()
{
	CCastStringHeatValidnError::UpdateList(m_listValidation,m_validator.ValidnErrors());
}



void CHeatDetailsDlg::SetValidationComboItems()
{
	m_comboValidnError.ResetContent();

	if ( m_validator.ValidnErrors().size() == 0 )
		m_comboValidnError.AddString( "<No errors>");
	else {

		ostrstream ostr;

		for ( vector<CCastStringHeatValidnError>::const_iterator ie = m_validator.ValidnErrors().begin();
			  ie != m_validator.ValidnErrors().end();
			  ++ie ) {
			
			ostr << LPCTSTR((*ie).SeverityAsString())
				 << " ";

			const CCastStringHeat* pHeat = (*ie).Heat();
			if ( pHeat == 0 ) 
				ostr << "# --, Tab# -----, Strand# -, ";
			else {
				ostr << "# " << setw(2) << (*ie).HeatIndex()+1
					 << ", Tab# " << setw(5) << pHeat->TabNum()
					 << ", Strand# ";
				if ( (*ie).StrandNum() == 0 )
					ostr << "-";
				else 
					ostr << setw(1) << (*ie).StrandNum();
			}

			const CCSOrder* pCSOrder = (*ie).CSOrder();

			if ( pCSOrder == 0 ) 
				ostr << "Lot " << setw(2) << "--"
				     << ",  Order# " << setw(7) << "-------";
			else 
				ostr << "Lot " << setw(2) << (*ie).LotIndex()+1
					 // FP Change
					 //<< ", Order# " << setw(7) << pCSOrder->OrderNum();
					 << ", Order# " << setw(15) << pCSOrder->FpOrderNum();
			
			ostr << ": " << LPCTSTR((*ie).Msg())
				 << ends;
				
			m_comboValidnError.AddString( ostr.str() );
			ostr.freeze(false);
			ostr.seekp(0);
		}
	}

	// Either way, there is at least one item in the list,
	// make the 1st item current

	m_comboValidnError.SetCurSel(0);
}






void CHeatDetailsDlg::OnSelchangeComboValidationError() 
{
	int posn = m_comboValidnError.GetCurSel();

	// check for no selection
	if ( posn == CB_ERR )
		return;

	// check for no errors
	if ( m_validator.ValidnErrors().size() == 0 )
		return;

	const CCastStringHeatValidnError& rErr = m_validator.ValidnErrors()[posn];

	const CCastStringHeat* pHeat = rErr.Heat();

	if ( pHeat == 0 )
		return;

	// ## add ROWCOL cast
	// unselect all cells in the grid
	m_wndDetailsGrid.SelectRange( CGXRange( 1,1, (ROWCOL)EditedHeats().size(), LAST_HEAT_COL), false );

	// set the current cell
	m_wndDetailsGrid.SetCurrentCell( rErr.HeatIndex()+1,HEAT_COL_SPEC);
	
	// select the current row.
	m_wndDetailsGrid.SelectRange( CGXRange( rErr.HeatIndex()+1, 1,
											rErr.HeatIndex()+1, LAST_HEAT_COL ) );

	// unselect all items in the order list

	for ( int i = m_listOrders.GetNextItem(-1,LVNI_SELECTED);
		  i != -1;
	      i = m_listOrders.GetNextItem(i,LVNI_SELECTED) ) 
		m_listOrders.SetItemState(i,0,LVIS_SELECTED);


	if ( rErr.CSOrder() == 0 )
		return;

		
	m_tabDetail.SetCurSel(DETAIL_ORDERS);
	m_detailType = DETAIL_ORDERS;
	OnSelchangeTab1(0,0);

	int strandNum = rErr.StrandNum();

	int index = ( strandNum == 1 
		          ? rErr.LotIndex()
				  : rErr.LotIndex() + ( pHeat->StrandEnd(1) - pHeat->StrandBegin(1) ) );

	// Select the current order

	m_listOrders.SetItemState(index,LVIS_SELECTED,LVIS_SELECTED);
}



void CHeatDetailsDlg::OnItemchangedListValidationStrings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ( pNMListView->uNewState & LVIS_FOCUSED ) {

		int posn = m_listOrders.GetNextItem(-1,LVNI_SELECTED);
		if ( posn != -1 ) {
			m_comboValidnError.SetCurSel(posn);
			OnSelchangeComboValidationError();
		}
	}

	*pResult = 0;
}




////////////////////////////////////////////////////////////////
//	
//	Chems grid
//	
////////////////////////////////////////////////////////////////


void CHeatDetailsDlg::InitializeHeatChemsRows()
{

	if ( m_eltSeq.size() == 0 ) {
		m_eltSeq.push_back(CChem::ELT_C);
		m_eltSeq.push_back(CChem::ELT_MN);
		m_eltSeq.push_back(CChem::ELT_P);
		m_eltSeq.push_back(CChem::ELT_S);
		m_eltSeq.push_back(CChem::ELT_SI);
		m_eltSeq.push_back(CChem::ELT_CU);
		m_eltSeq.push_back(CChem::ELT_SN);
		m_eltSeq.push_back(CChem::ELT_NI);
		m_eltSeq.push_back(CChem::ELT_MO);
		m_eltSeq.push_back(CChem::ELT_CR);
		m_eltSeq.push_back(CChem::ELT_CB);
		m_eltSeq.push_back(CChem::ELT_V);
		m_eltSeq.push_back(CChem::ELT_TI);
		m_eltSeq.push_back(CChem::ELT_B);
		m_eltSeq.push_back(CChem::ELT_AL);
		m_eltSeq.push_back(CChem::ELT_N);
		m_eltSeq.push_back(CChem::ELT_PB);
//		m_eltSeq.push_back(CChem::ELT_CA);  // Test 3-19-07 k. hubbard
	}


	if ( m_currentHeatIndex < 0 ) {
		m_wndChemsGrid.SetRowCount(0);
		return;
	}

	assert ( 0 <= m_currentHeatIndex && m_currentHeatIndex < EditedHeats().size() );

	m_wndChemsGrid.LockUpdate(TRUE);

	m_wndChemsGrid.SetRowCount(CChem::NumElements);

//	for ( int elt=0; elt<CChem::NumElements; ++elt )
//		UpdateHeatChemRow(m_currentHeatIndex,CChem::Element(elt));
	
	ROWCOL nrow = 1;
	for ( vector<int>::iterator ii = m_eltSeq.begin();
		  ii != m_eltSeq.end();
		  ++ii, ++nrow )
		UpdateHeatChemRow(m_currentHeatIndex,
						  nrow,
						  CChem::Element((*ii)));

	m_wndChemsGrid.LockUpdate(FALSE);
	m_wndChemsGrid.Redraw();
}




#ifdef setv
#undef setv
#endif

#define setv(col,val) m_wndChemsGrid.SetValueRange( CGXRange( nrow, col), val)


void CHeatDetailsDlg::UpdateHeatChemRow(int hIndex,
										ROWCOL nrow,
										CChem::Element elt)
{
	CString strMin;
	CString strMax;
	CString strAim;

	CCastStringHeat& rHeat = EditedHeats()[hIndex];

	if ( rHeat.MinChem(elt) == 0.0 )
		strMin = "";
	else
		strMin.Format ("%5.3f",rHeat.MinChem(elt));

	if ( rHeat.MaxChem(elt) == 100.0 || rHeat.MaxChem(elt) == 0.0 )
		strMax = "";
	else
		strMax.Format ("%5.3f",rHeat.MaxChem(elt));
	
	if ( rHeat.AimChem(elt) == 0.0 )
		strAim = "";
	else
		strAim.Format("%5.3f",rHeat.AimChem(elt));
	
	CString strChem = CChem::ToString(elt);
	bool isEditable = false;
	if ( rHeat.SpecPtr() != 0
		 &&
		 rHeat.SpecPtr()->GetChemRange(elt) != 0
		 &&
		 rHeat.SpecPtr()->GetChemRange(elt)->IsAppSpecific() ) {

		 strChem += " *";
		 isEditable = true;
	}

	setv( CHEM_COL_ELT, LPCTSTR(strChem)	);
	setv( CHEM_COL_MIN, LPCTSTR(strMin)		);
	setv( CHEM_COL_MAX, LPCTSTR(strMax)		);
	setv( CHEM_COL_AIM, LPCTSTR(strAim)		);


	CGXStyle style;

	style.SetEnabled( isEditable );
	m_wndChemsGrid.SetStyleRange( CGXRange(nrow,CHEM_COL_MIN), style);
	m_wndChemsGrid.SetStyleRange( CGXRange(nrow,CHEM_COL_MAX), style);
	m_wndChemsGrid.SetStyleRange( CGXRange(nrow,CHEM_COL_AIM), style);

	style.SetEnabled( false );
	m_wndChemsGrid.SetStyleRange( CGXRange(nrow,CHEM_COL_ELT), style);

}



BOOL CHeatDetailsDlg::OnEndEditingChemCell(ROWCOL nrow, ROWCOL ncol)
{
	// column or row header
	if ( nrow < 1 || ncol < 1 )
		return TRUE;

	assert ( 0 <= m_currentHeatIndex && m_currentHeatIndex < EditedHeats().size() );
	assert ( nrow-1 < m_eltSeq.size() );

	int caster = m_pCasterScen->Caster();

	CCastStringHeat& rHeat = EditedHeats()[m_currentHeatIndex];

	//CChem::Element elt = CChem::Element(nrow-1);
	CChem::Element elt = (CChem::Element)(m_eltSeq[ nrow-1 ]);

	CString s = m_wndChemsGrid.GetValueRowCol(nrow,ncol);

	switch ( ncol ) {
	case CHEM_COL_ELT:
		assert(0);
		break;

	case CHEM_COL_MIN:
		rHeat.MinChem(elt,atof(s));
		rHeat.FixAimChem(elt,caster);
		break;

	case CHEM_COL_MAX:
		rHeat.MaxChem(elt,atof(s));
		rHeat.FixAimChem(elt,caster);
		break;

	case CHEM_COL_AIM:
		rHeat.AimChem(elt,atof(s));
		break;
	}

	UpdateDetailRowFromHeat(m_currentHeatIndex+1,rHeat);
	//UpdateHeatChemRow(m_currentHeatIndex,elt);
	// It would be nice to update only the row of the element involved,
	// but I was too lazy to figure out the row where it should be saved.

	InitializeHeatChemsRows();


	return TRUE;
}


////////////////////////////////////////////////////////////////
//	
//	Callbacks
//	
////////////////////////////////////////////////////////////////

bool CHeatDetailsDlg::MarkedHeatsExist(bool displayWarning)
{
	for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih )

		if ( (*ih).IsMarked() )
			return true;

	if ( displayWarning )
		MessageBox("You must mark some heats for this operation.",
				   "No heats",
				   MB_ICONWARNING|MB_OK);
		
	return false;
}


void CHeatDetailsDlg::OnButtonSetSpecs() 
{
	if ( ! MarkedHeatsExist(true) )
		return;

	m_tabDetail.SetCurSel(DETAIL_ORDERS);
	m_detailType = DETAIL_ORDERS;
	OnSelchangeTab1(0,0);

	int caster = m_pCasterScen->Caster();

	for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih ) {

		if ( ! (*ih).IsMarked() )
			continue;

		int heatIndex = ih-EditedHeats().begin();

		m_currentHeatIndex = heatIndex;
		SetOrdersListItems();

		CString specName;

		switch ( GetHeatSpecFromUser( (*ih),  heatIndex, specName) ) {
		case GHSA_CONTINUE: 
			{
				CString modSpecName = AdjustSpecFor05((*ih),specName);
				(*ih).SetSpec( modSpecName, caster );
				UpdateDetailRowFromHeat(heatIndex+1, (*ih) );
			}
			break;
		case GHSA_SET_ALL:
			{
				for ( vector<CCastStringHeat>::iterator ih1 = ih;
					  ih1 != EditedHeats().end();
					  ++ih1 ) {
					if ( (*ih1).IsMarked() ) {
						CString modSpecName = AdjustSpecFor05((*ih1),specName);
						(*ih1).SetSpec( modSpecName, caster );
						int heatIndex = ih1-EditedHeats().begin();
						UpdateDetailRowFromHeat(heatIndex+1, (*ih1) );
					}
				}
			}
			goto done;
			break;
		default:
			goto done;
			break;
		}
	}

done:

	OnButtonSetChems();	
	//InitializeHeatDetailsRows();
}


CString CHeatDetailsDlg::AdjustSpecFor05(CCastStringHeat& rHeat, CString& rSpecName)
{
	if ( rSpecName.GetLength() < 7 )
		return rSpecName;

	CString spec = rSpecName;
	spec.SetAt(6,'0');
	

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(1);
				  io != rHeat.StrandEnd(1);
				  ++io )  {
	
				CString specToUse;
	
				// FP change
				//if ( (*io)->OrderNum() == 9999999 )
				if ( (*io)->FpOrderNum().Left(7) == "9999999" )
					specToUse = (*io)->LotSpec();
				else if ( (*io)->Order() != 0 ) 
					specToUse = (*io)->Order()->OrderedSpec();
	
				if ( specToUse[6] == '5' ) {
					spec.SetAt(6,'5');
					return spec;
				}
		}
	}

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(2);
				  io != rHeat.StrandEnd(2);
				  ++io )  {
	
				CString specToUse;
	
				// FP change
				//if ( (*io)->OrderNum() == 9999999 )
				if ( (*io)->FpOrderNum().Left(7) == "9999999" )
					specToUse = (*io)->LotSpec();
				else if ( (*io)->Order() != 0 ) 
					specToUse = (*io)->Order()->OrderedSpec();
	
				if ( specToUse[6] == '5' ) {
					spec.SetAt(6,'5');
					return spec;
				}
		}
	}

	return spec;
}

//
CHeatDetailsDlg::E_GetHeatSpecAction CHeatDetailsDlg::GetHeatSpecFromUser(CCastStringHeat& rHeat, 
															 int heatIndex,
															 CString& rSpecName)
{
	int caster = m_pCasterScen->Caster();

	THeatSpecCrossAppSet specs;

	bool spec05Seen = false;
	CString spec05;

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(1);
			  io != rHeat.StrandEnd(1);
			  ++io )  {

			CString specToUse;

			// FP Change
			//if ( (*io)->OrderNum() == 9999999 )
			if ( (*io)->FpOrderNum().Left(7) == "9999999" )
				specToUse = (*io)->LotSpec();
			else if ( (*io)->Order() != 0 ) {
				specToUse = (*io)->Order()->OrderedSpec();
				CCastStringValidator::FixHeatSpec(specToUse,
												  (*io)->Order()->OrderedSpec(),
												  caster);
			}
			else 
				continue;

			if ( ! spec05Seen && specToUse[6] == '5' ) {
				spec05Seen = true;
				spec05 = specToUse;
			}

//			if ( specToUse[6] == '5' )
//				spec05Seen = true;
//			specToUse.SetAt(6,'0');
			specs.insert( specToUse );
		}
	}

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(2);
			  io != rHeat.StrandEnd(2);
			  ++io ) {

			CString specToUse;

			// FP Change
			//if ( (*io)->OrderNum() == 9999999 )
			if ( (*io)->FpOrderNum().Left(7) == "9999999" )
				specToUse = (*io)->LotSpec();
			else if ( (*io)->Order() != 0 ) {
				specToUse = (*io)->Order()->OrderedSpec();
				CCastStringValidator::FixHeatSpec(specToUse,
												  (*io)->Order()->OrderedSpec(),
												  caster);
			}
			else 
				continue;

			if ( ! spec05Seen && specToUse[6] == '5' ) {
				spec05Seen = true;
				spec05 = specToUse;
			}

//			if ( specToUse[6] == '5' )
//				spec05Seen = true;
//			specToUse.SetAt(6,'0');
			specs.insert( specToUse );
		}
	}


	THeatSpecCrossAppSet xset;
	
	//if ( specs.size() == 1 )
	//	xset.insert( *(specs.begin()) );
	//else
	//	xset = m_pSnapshot->HeatSpecCrossApp().GetCrossAppSpecs(specs,caster);

	xset = TheSnapshot.HeatSpecCrossApp().GetCrossAppSpecs(specs,caster);



	THeatSpecCrossAppSet xxset;

	{
		for ( THeatSpecCrossAppSet::iterator ix = xset.begin();
			  ix != xset.end();
			  ++ix ) {
			CString xxspec = (*ix);
			if ( CCastStringValidator::FixHeatSpec(xxspec,(*ix),caster) ) {
//				if ( spec05Seen )
//					xxspec.SetAt(6,'5');
				xxset.insert( xxspec );
			}
		}
	}
		 
#if _DEBUG
	{
		afxDump << "GetHeatSpecFromUser: # " << heatIndex 
				<< ": #specs = " ;
		for ( THeatSpecCrossAppSet::iterator is = specs.begin();
			  is != specs.end();
			  ++is )
			afxDump << (*is) << " ";

		afxDump << ", xapp = ";

		for ( THeatSpecCrossAppSet::iterator ix = xset.begin();
			  ix != xset.end();
			  ++ix )
			afxDump << (*ix) << " ";

		afxDump << ", xxapp = ";

		for ( THeatSpecCrossAppSet::iterator ixx = xxset.begin();
		      ixx != xxset.end();
			  ++ixx )
			afxDump << (*ixx) << " ";

		afxDump << ".\n";
	}
#endif

	// make all specs 05 specs.

	if ( spec05Seen ) {
		THeatSpecCrossAppSet copy = specs; // copy
		specs.clear();
		for ( THeatSpecCrossAppSet::iterator is = copy.begin();
			  is != copy.end();
			  ++is ) {
			CString newSpec = (*is);
			newSpec.SetAt(6,'5');
			specs.insert(newSpec);
		}
	}
			
	// make all xxset specs 05 specs.

	if ( spec05Seen ) {
		THeatSpecCrossAppSet copy = xxset; // copy
		xxset.clear();
		for ( THeatSpecCrossAppSet::iterator is = copy.begin();
			  is != copy.end();
			  ++is ) {
			CString newSpec = (*is);
			newSpec.SetAt(6,'5');
			xxset.insert(newSpec);
		}
	}

	CSpecSelectionDlg dlg(this);

	dlg.m_caster = caster;
	dlg.m_specs = xxset; // copy
	dlg.m_orderedSpecs = specs; // copy
	dlg.m_caption.Format("Select or type spec for heat # %d (tab# %d)",
						 heatIndex+1,
						 rHeat.TabNum());
	dlg.SetInitialPos();
	if ( spec05Seen )
		dlg.m_selectedSpec = spec05;

	E_GetHeatSpecAction retval = GHSA_QUIT;

	switch ( dlg.DoModal() ) {
	case IDOK:
		rSpecName = dlg.m_selectedSpec;
		retval = GHSA_CONTINUE;
		break;
	case IDC_BUTTON_SET_ALL:
		rSpecName = dlg.m_selectedSpec;
		retval = GHSA_SET_ALL;
		break;
	default:
		rSpecName = "";
		retval = GHSA_QUIT;
		break;
	}

	return retval;
}




void CHeatDetailsDlg::OnButtonSetChems() 
{
	if ( ! MarkedHeatsExist(true) )
		return;

	int caster = m_pCasterScen->Caster();
	ostrstream ostr;
	bool isError = false;
	vector<bool> badChems;
	badChems.resize(CChem::NumElements);

	for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih ) {

		if ( (*ih).IsMarked() )
			SetChemsForHeat( (*ih), ih - EditedHeats().begin(), caster, badChems, ostr );			

	}

	ostr << ends;

	if ( isError ) {

		CMsgDlg dlg;
		dlg.m_title = "Error setting chems";
		dlg.m_message = ostr.str();
		dlg.DoModal();
		ostr.freeze(false);
	}

	InitializeHeatDetailsRows();
	InitializeHeatChemsRows();
}



void CHeatDetailsDlg::SetChemsForHeat(CCastStringHeat& rHeat, int heatIndex)
{

	int caster = m_pCasterScen->Caster();
	ostrstream ostr;
	bool isError = false;
	vector<bool> badChems;
	badChems.resize(CChem::NumElements);

	if ( ! SetChemsForHeat(rHeat,heatIndex,caster,badChems,ostr) )
		isError = true;

	ostr << ends;

	if ( isError ) {

		CMsgDlg dlg;
		dlg.m_title = "Error setting chems";
		dlg.m_message = ostr.str();
		dlg.DoModal();
		ostr.freeze(false);
	}

	InitializeHeatDetailsRows();
	InitializeHeatChemsRows();

}



bool CHeatDetailsDlg::SetChemsForHeat(CCastStringHeat& rHeat,
									  int heatIndex,
									  int caster,
									  vector<bool>& badChems,
									  ostrstream& ostr)
{
	bool isError = false;

	rHeat.ResetChems();

	if ( ! rHeat.SetChemsFromHeatSpec(ostr, heatIndex, caster) )
		isError = true;

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(1);
			  io != rHeat.StrandEnd(1);
			  ++io ) 
	
			if ( ! IntersectChems( 1, (*io), badChems, ostr ) )
				isError = true;
	}

	{
		for ( vector<CCSOrder*>::const_iterator io = rHeat.StrandBegin(2);
			  io != rHeat.StrandEnd(2);
			  ++io ) 
	
			if ( ! IntersectChems( 2, (*io), badChems, ostr ) )
				isError = true;
	}

	rHeat.FixAimChems(ostr, heatIndex, caster);

	return ! isError;
}




bool CHeatDetailsDlg::IntersectChems(int strandNum,
									 CCSOrder* pOrder,
									 vector<bool>&badChems,
									 ostrstream& ostr)
{
	CCastStringHeat& rHeat = EditedHeats()[ pOrder->HeatSeqNum()];

	if ( ! rHeat.IsMarked() )
		return true;

	if ( ! rHeat.IntersectChems( pOrder, badChems ) ) {

		for ( int elt = 0; elt < CChem::NumElements; ++elt ) {

			if ( badChems[elt] )
				ostr << "Heat# "      << setw(2) << pOrder->HeatSeqNum()+1
					 << ", strand = " << setw(1) << strandNum
					 // FP Change
					 //<< ", Order # "  << setw(7) << pOrder->OrderNum()
					 << ", Order # "  << setw(15) << pOrder->FpOrderNum()
					 << ", elt = "    << setw(2) << LPCTSTR(CChem::ToString(CChem::Element(elt)))
					 << "\n";
		}

		return false;

	}
		  
	return true;
}




void CHeatDetailsDlg::OnButtonSetTabNums() 
{
	if ( ! MarkedHeatsExist(true) )
		return;
	
	CTabNumDlg dlg;

	dlg.m_caster = m_pCastString->Id().Caster();

	if ( dlg.DoModal() == IDOK ) {

		long tabNum = dlg.m_tabNum;

		// Note: if 0 is entered, then make marked heats have tab = zero.

		for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
			  ih != EditedHeats().end();
			  ++ih)
			
			if ( (*ih).IsMarked() ) {
				(*ih).TabNum(tabNum);
				if ( tabNum != 0 )
					++tabNum;
			}

		InitializeHeatDetailsRows();
	}
}




void CHeatDetailsDlg::OnButtonValidate() 
{

	if ( ! MarkedHeatsExist(true) )
		return;

	{
		CWaitCursor cursor;
		CLongOpn* pOpn = new CLongOpn;

		pOpn->m_strings.push_back("Validate");
		pOpn->m_strings.push_back("Save");
		pOpn->m_strings.push_back("Done");

		pOpn->Go();
		pOpn->SetCurrentStep(0);

		m_validator.Validate(EditedHeats(),m_pCastString,m_pCasterScen,m_pSuperScen);

		pOpn->SetCurrentStep(1);

		CCasterScenArchiver::Archive(m_pCasterScen,true);
		CScenMgr::BackupCasterScen(m_pCasterScen);

		SetValidationListItems();
		SetValidationComboItems();
		InitializeHeatDetailsRows();
		SetOrdersListItems();
		m_tabDetail.SetCurSel(DETAIL_VALIDATION);
		m_detailType = DETAIL_VALIDATION;
		OnSelchangeTab1(0,0);
		m_tabDetail.RedrawWindow();

		pOpn->SetCurrentStep(2);
		pOpn->Done(0);
	}

	//MessageBox("Scenario saved!");

}






void CHeatDetailsDlg::OnButtonMarkAll() 
{
	for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih ) 

		(*ih).IsMarked(true);

	InitializeHeatDetailsRows();
}

void CHeatDetailsDlg::OnButtonClearMarks() 
{
	for ( vector<CCastStringHeat>::iterator ih = EditedHeats().begin();
		  ih != EditedHeats().end();
		  ++ih ) 

		(*ih).IsMarked(false);

	  	InitializeHeatDetailsRows();
}





void CHeatDetailsDlg::OnOK() 
{
	CDialog::OnOK();
}

	

void CHeatDetailsDlg::OnDblclkListOrders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int index = m_listOrders.GetNextItem(-1,LVNI_SELECTED);

	if ( index == -1 ) 
		return;

	if ( m_currentHeatIndex == -1 )
		return;

	DWORD code = m_listOrders.GetItemData(index);
	int strandNum = HIWORD(code);
	int orderNum  = LOWORD(code);
	
	CCSOrder* pCSOrder = *(EditedHeats()[m_currentHeatIndex].StrandBegin(strandNum) + orderNum);

	if ( m_pCastString->ModifyCSOrderParms(m_pSuperScen,
										   strandNum,
										   orderNum) )  {
		InitializeHeatDetailsRows();
		SetOrdersListItems();
	}
										   
	*pResult = 0;
}


void CHeatDetailsDlg::OnButtonEditOrders1() 
{
	OnButtonEditOrders(1);	
}

void CHeatDetailsDlg::OnButtonEditOrders2() 
{
	OnButtonEditOrders(2);
}

void CHeatDetailsDlg::OnButtonEditOrders(int strandNum) 
{
	if ( m_currentHeatIndex == -1 )
		return;

	if (strandNum == 2 && Caster::NumStrandsOnCaster(m_pCastString->Id().Caster()) == 1 )
		return;

	CCastStringHeat& rHeat = EditedHeats()[m_currentHeatIndex];

	//  Make sure heats are locked before we begin this
	//m_pCastString->FreezeHeats(true);
	m_pCastString->HeatStatus(CCastString::HEATS_ALL_LOCKED);


	if ( m_pCastString->EditMultipleOrders(m_currentHeatIndex,
										   strandNum,
										   rHeat.StrandBegin(strandNum),
										   rHeat.StrandEnd(strandNum),
										   m_pSuperScen) ) {

		m_pCasterScen->SetModified(true);
		InitializeHeatDetailsRows();
		SetOrdersListItems();
	}
}	
