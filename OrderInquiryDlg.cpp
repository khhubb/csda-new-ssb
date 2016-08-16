// OrderInquiryDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "OrderInquiryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Snapshot.h"
#include "Order.h"
#include "CasterScen.h"
#include "CastString.h"
#include "Caster.h"


/////////////////////////////////////////////////////////////////////////////
// COrderInquiryDlg dialog
//
//  Display information about an order.
//
// 
//	Usage:
//
// 		COrderInquiryDlg dlg;
//
//		dlg.m_pOrder		= < some COrder, can be 0 >
//		dlg.m_pSuperScen	= < a CSuperScen> ;
//
//		dlg.DoModal();
//	
//		// dlg.m_pOrder contains a pointer to the last COrder examined
//		
//
/////////////////////////////////////////////////////////////////////////////


COrderInquiryDlg::COrderInquiryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderInquiryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderInquiryDlg)
	m_castSpec = _T("");
	m_ciCode = 0;
	m_custName = _T("");
	m_hbGauge = 0.0;
	m_hbWidth = 0.0;
	m_megaLuCode = _T("");
	m_numPieces = 0;
	m_orderedSpec = _T("");
	m_slabCondnCode = 0;
	m_slabLength = 0.0;
	m_slabWeight = 0.0;
	m_slabWidth = 0.0;
	m_steelSpec = _T("");
	m_unitPref = _T("");
	m_plannedStartDate = _T("");
	m_expCode = 0;
	m_cbWidth = 0.0;
	m_hrWkYr = _T("");
	m_priorityCode = _T("");
	m_custCode = 0;
	m_maxSlabLength = 0;
	m_maxSlabWidth = 0.0;
	m_minSlabLength = 0;
	m_minSlabWidth = 0.0;
	m_tons = 0;
	m_maxTransSlabWidth = 0.0;
	m_orderedGauge = 0.0;
	m_fpOrderNum = _T("");
	m_fpIshroPrio = 0;
	m_fpMtoFlow = _T("");
	m_indexMatchingOrders = -1;
	m_fpMeltPst = 0;
	m_numSegments = 0;
	m_piecesDueOnLu = _T("");
	m_tonsDueOnLu = _T("");
	//}}AFX_DATA_INIT

	m_pOrder		= 0;
	m_pSuperScen	= 0;
}


void COrderInquiryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderInquiryDlg)
	DDX_Control(pDX, IDC_COMBO_MATCHING_ORDERS, m_comboMatchingOrders);
	DDX_Control(pDX, IDC_LIST_CHEMISTRIES, m_listChems);
	DDX_Control(pDX, IDC_LIST_ON_LINEUP, m_listLineups);
	DDX_Text(pDX, IDC_EDIT_CAST_SPEC, m_castSpec);
	DDX_Text(pDX, IDC_EDIT_CI_CODE, m_ciCode);
	DDX_Text(pDX, IDC_EDIT_CUST_NAME, m_custName);
	DDX_Text(pDX, IDC_EDIT_HB_GAUGE, m_hbGauge);
	DDX_Text(pDX, IDC_EDIT_HB_WIDTH, m_hbWidth);
	DDX_Text(pDX, IDC_EDIT_MEGA_LU_CODE, m_megaLuCode);
	DDX_Text(pDX, IDC_EDIT_NUM_PIECES, m_numPieces);
	DDX_Text(pDX, IDC_EDIT_ORDERED_SPEC, m_orderedSpec);
	DDX_Text(pDX, IDC_EDIT_SLAB_CONDN_CODE, m_slabCondnCode);
	DDX_Text(pDX, IDC_EDIT_SLAB_LENGTH, m_slabLength);
	DDX_Text(pDX, IDC_EDIT_SLAB_WEIGHT, m_slabWeight);
	DDX_Text(pDX, IDC_EDIT_SLAB_WIDTH, m_slabWidth);
	DDX_Text(pDX, IDC_EDIT_STEEL_SPEC, m_steelSpec);
	DDX_Text(pDX, IDC_EDIT_UNIT_PREF, m_unitPref);
	DDX_Text(pDX, IDC_EDIT_PLANNED_START_DATE, m_plannedStartDate);
	DDX_Text(pDX, IDC_EDIT_EXP_CODE, m_expCode);
	DDX_Text(pDX, IDC_EDIT_CB_WIDTH, m_cbWidth);
	DDX_Text(pDX, IDC_EDIT_HR_WEEK_YEAR, m_hrWkYr);
	DDX_Text(pDX, IDC_EDIT_PRIORITY_CODE, m_priorityCode);
	DDX_Text(pDX, IDC_EDIT_CUST_CODE, m_custCode);
	DDX_Text(pDX, IDC_EDIT_MAX_SLAB_LENGTH, m_maxSlabLength);
	DDX_Text(pDX, IDC_EDIT_MAX_SLAB_WIDTH, m_maxSlabWidth);
	DDX_Text(pDX, IDC_EDIT_MIN_SLAB_LENGTH, m_minSlabLength);
	DDX_Text(pDX, IDC_EDIT_MIN_SLAB_WIDTH, m_minSlabWidth);
	DDX_Text(pDX, IDC_EDIT_TONS, m_tons);
	DDX_Text(pDX, IDC_EDIT_MAX_TRANS_SLAB_WIDTH, m_maxTransSlabWidth);
	DDX_Text(pDX, IDC_EDIT_ORDERED_GAUGE, m_orderedGauge);
	DDX_Text(pDX, IDC_EDIT_ORDER_NUM, m_fpOrderNum);
	DDV_MaxChars(pDX, m_fpOrderNum, 15);
	DDX_Text(pDX, IDC_EDIT_FP_ISHRO_PRIO, m_fpIshroPrio);
	DDX_Text(pDX, IDC_EDIT_FP_MTO_FLOW, m_fpMtoFlow);
	DDX_CBIndex(pDX, IDC_COMBO_MATCHING_ORDERS, m_indexMatchingOrders);
	DDX_Text(pDX, IDC_EDIT_FP_MELT_PST, m_fpMeltPst);
	DDX_Text(pDX, IDC_EDIT_NUM_SEGMENTS, m_numSegments);
	DDX_Text(pDX, IDC_EDIT_PIECES_DUE_ONLU, m_piecesDueOnLu);
	DDX_Text(pDX, IDC_EDIT_TONS_DUE_ONLU, m_tonsDueOnLu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderInquiryDlg, CDialog)
	//{{AFX_MSG_MAP(COrderInquiryDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP, OnButtonLookup)
	ON_CBN_SELCHANGE(IDC_COMBO_MATCHING_ORDERS, OnSelchangeComboMatchingOrders)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderInquiryDlg message handlers



BOOL COrderInquiryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// we need a superScen -- shows which scenarios to do on-lineup lookups in
	assert( m_pSuperScen != 0 );
	
	// Get the lists so that we can set the headers
	UpdateData(FALSE);
	
	SetLineupListHeader();
	SetChemListHeader();


	if ( m_pOrder != 0)
		SetFields();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




#define ColWidth2( list, str ) list.GetStringWidth(str) + 15

void COrderInquiryDlg::SetLineupListHeader()
{
	static const CString strCaster		= "Caster";
	static const CString strStringId	= "String#";
	static const CString strTabNum		= "Tab#";
	static const CString strHeatNum		= "Heat#";
	static const CString strStrandNum	= "Strand#";
	static const CString strLotNum		= "Lot#";
	static const CString strNumPieces	= "# Pieces";

	static const int widthCaster	= ColWidth2( m_listLineups, strCaster    );
	static const int widthStringId	= ColWidth2( m_listLineups, "XXXX-XX-XX" );
	static const int widthTabNum	= ColWidth2( m_listLineups, "XXXXX"		 );
	static const int widthHeatNum	= ColWidth2( m_listLineups, strHeatNum	 );
	static const int widthStrandNum	= ColWidth2( m_listLineups, strStrandNum );
	static const int widthLotNum	= ColWidth2( m_listLineups, strLotNum	 );
	static const int widthNumPieces	= ColWidth2( m_listLineups, strNumPieces );

	m_listLineups.InsertColumn( 0, strCaster,    LVCFMT_LEFT,  widthCaster,    0 );
	m_listLineups.InsertColumn( 1, strStringId,  LVCFMT_RIGHT, widthStringId,  1 );
	m_listLineups.InsertColumn( 2, strTabNum,	 LVCFMT_RIGHT, widthTabNum,	   2 );
	m_listLineups.InsertColumn( 3, strHeatNum,	 LVCFMT_RIGHT, widthHeatNum,   3 );
	m_listLineups.InsertColumn( 4, strStrandNum, LVCFMT_RIGHT, widthStrandNum, 4 );
	m_listLineups.InsertColumn( 5, strLotNum,    LVCFMT_RIGHT, widthLotNum,	   5 );
	m_listLineups.InsertColumn( 6, strNumPieces, LVCFMT_RIGHT, widthNumPieces, 6 );
}


void COrderInquiryDlg::SetChemListHeader()
{
	static const CString strElt = "Elt";
	static const CString strMin = "Min";
	static const CString strMax = "Max";
	static const CString strMod = "Reason";

	static const int widthElt = ColWidth2( m_listChems, strElt    );
	static const int widthMin = ColWidth2( m_listChems, "0.00000" );
	static const int widthMax = ColWidth2( m_listChems, "0.00000" );
	static const int widthMod = ColWidth2( m_listChems, strMod    );

	m_listChems.InsertColumn( 0, strElt, LVCFMT_LEFT,  widthElt, 0 );
	m_listChems.InsertColumn( 1, strMin, LVCFMT_RIGHT, widthMin, 1 );
	m_listChems.InsertColumn( 2, strMax, LVCFMT_RIGHT, widthMax, 2 );
	m_listChems.InsertColumn( 3, strMod, LVCFMT_RIGHT, widthMod, 3 );
}


void COrderInquiryDlg::SetFields()
{
	ostrstream ostr;

	// FP change
	//m_orderNum			= m_pOrder->OrderNum();
	m_fpOrderNum			= m_pOrder->FpOrderNum();

	m_custName			= m_pOrder->CustName();
	m_custCode			= m_pOrder->CustCode();

	ostr.fill('0');
	ostr << setw(2) << m_pOrder->HrYrWk().Week()
		 << " - "   
		 << setw(4) << m_pOrder->HrYrWk().Year()
		 << ends;
	m_hrWkYr = ostr.str();
	ostr.freeze(false);
	ostr.seekp(0);

	ostr.fill('0');
	ostr << setw(8) << m_pOrder->PlannedLatestStartDate()
		 << "-"
		 << setw(4) << m_pOrder->PlannedLatestStartTime()/100
		 << ends;
	m_plannedStartDate	= ostr.str();
	ostr.freeze(false);
	ostr.seekp(0);

	m_orderedSpec		= m_pOrder->OrderedSpec();
	m_steelSpec			= m_pOrder->SteelSpec();
	m_castSpec			= m_pOrder->CastSpec();


	m_ciCode			= m_pOrder->CICode();
	m_expCode			= m_pOrder->PgmExposure();

	ostr.fill('0');
	ostr << setw(2) << m_pOrder->CommodityPriority()
		 << "-"
		 << m_pOrder->OrderPriority()
		 << ends;
	m_priorityCode = ostr.str();
	ostr.freeze(false);
	ostr.seekp(0);
	
	m_slabCondnCode		= m_pOrder->SlabCondnCode();
	m_megaLuCode		= m_pOrder->MegaLuCd();
	
	ostr.fill(' ');
	ostr << (LPCTSTR)m_pOrder->SlabUnitCd()
		 << " (" << (LPCTSTR)m_pOrder->FaclPrefCd(1)
		 << "/"  << (LPCTSTR)m_pOrder->FaclPrefCd(2)
		 << "/"  << (LPCTSTR)m_pOrder->FaclPrefCd(3)
		 << ")"  << ends;
	m_unitPref = ostr.str();
	ostr.freeze(false);
	ostr.seekp(0);

	m_cbWidth			= m_pOrder->CbWidth();

	m_hbGauge			= m_pOrder->HbGauge();
	m_hbWidth			= m_pOrder->HbWidth();

	m_orderedGauge		= m_pOrder->OrderedGauge();

	m_slabLength		= long(m_pOrder->SlabLength());	// user requested truncation to integer	
	m_slabWeight		= m_pOrder->IndivSlabWgt();
	m_slabWidth			= m_pOrder->SlabWidth();

	m_minSlabWidth		= m_pOrder->MinAllowedSlabWidth(false);
	m_maxSlabWidth		= m_pOrder->MaxAllowedSlabWidth(false);
	m_maxTransSlabWidth	= m_pOrder->MaxAllowedSlabWidth(true);

	m_minSlabLength		= long(m_pOrder->MinSlabLength());
	m_maxSlabLength		= long(m_pOrder->MaxSlabLength());

	m_numPieces			= m_pOrder->NumSlabsDue();
	m_tons				= long((m_numPieces*m_slabWeight)/2000);

	// FP Change
	m_fpMeltPst			= m_pOrder->FpMeltPstDate();
	m_fpIshroPrio		= m_pOrder->FpIshroPrty();
	m_fpMtoFlow			= m_pOrder->FpMtoFlowOrdInd();

	SetLineupList();
	SetChemList();
	SetParentData();

	UpdateData(FALSE);	
}

void COrderInquiryDlg::SetParentData()
{
	vector<COrder*>& orders = TheSnapshot.Orders();
	vector<COrder*> matchingOrders;
	COrder::FindOrdersFromMillOrderNum(m_pOrder->FpOrderNum().Left(7),orders,matchingOrders);

	// ## add cast to int
	m_numSegments = (int)matchingOrders.size();


	int numPiecesDue = 0;
	int numPiecesOnLu = 0;
	Weight tonsDue = 0.0;
	Weight tonsOnLu = 0.0;

	for ( vector<COrder*>::iterator io = matchingOrders.begin();
		  io != matchingOrders.end();
		  ++io ) {

		numPiecesDue += (*io)->NumSlabsDue();
		tonsDue += ((*io)->NumSlabsDue()*(*io)->IndivSlabWgt())/2000;

		for (int casterNum = Caster::C1; casterNum <= Caster::C3; ++casterNum) {
			CCasterScen* pScen = m_pSuperScen->CasterScen(casterNum);

			for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
				  is != pScen->CastStrings().end();
				  ++is ) {

				for ( int strandNum=1; strandNum<=2; ++strandNum )  {

					const vector<CCSOrder*>& strand = (*is)->Strand(strandNum);

					for ( vector<CCSOrder*>::const_iterator iso = strand.begin();
						  iso != strand.end();
						  ++iso) {
						if ( (*iso)->FpOrderNum().Left(7) == (*io)->FpOrderNum().Left(7) ) {
							numPiecesOnLu += (*iso)->NumPieces();
							tonsOnLu += (*iso)->Tons();
						}
					}
				}
			}
		}
	}

	m_tonsDueOnLu.Format("%d/%d",int(tonsDue+0.5),int(tonsOnLu+0.5));
	m_piecesDueOnLu.Format("%d/%d",numPiecesDue,numPiecesOnLu);
}


void COrderInquiryDlg::SetOrderList()
{
	m_comboMatchingOrders.ResetContent();
	m_indexMatchingOrders = 0;

	ostrstream ostr;

	for ( vector<COrder*>::iterator io = m_matchingOrders.begin();
	      io != m_matchingOrders.end();
		  ++io ) {
		ostr << LPCTSTR((*io)->FpOrderNum()) << "  -  " << (*io)->FpPlanNumPieces() << ends;
		m_comboMatchingOrders.AddString(ostr.str());
		ostr.freeze(false);
		ostr.seekp(0);
	}
}
	




void COrderInquiryDlg::SetLineupList()
{

	m_listLineups.DeleteAllItems();

	int count = 0;

	AddScenLineups(1,count);
	AddScenLineups(2,count);
	AddScenLineups(3,count);
}

void COrderInquiryDlg::AddScenLineups(int casterNum,int& count)
{
	ostrstream ostr;
	ostr.fill('0');

	CCasterScen* pScen = m_pSuperScen->CasterScen(casterNum);

	for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
		  is != pScen->CastStrings().end();
		  ++is ) {

		for ( int strandNum=1; strandNum<=2; ++strandNum )  {

			const vector<CCSOrder*>& strand = (*is)->Strand(strandNum);

			for ( vector<CCSOrder*>::const_iterator io = strand.begin();
				  io != strand.end();
				  ++io ) {

				// FP change
				//if ( (*io)->OrderNum() == m_pOrder->OrderNum() ) {
				if ( (*io)->FpOrderNum() == m_pOrder->FpOrderNum() ) {
					
					// casterNum
					ostr << casterNum << ends;
					m_listLineups.InsertItem(count,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);
				
					// string id					
					ostr << (*is)->Id().Year() 
						 << "-"
						 << (*is)->Id().Week()
						 << "-"
						 << (*is)->Id().StringId()
						 << ends;
					m_listLineups.SetItemText(count,1,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					// tab#
					CCastStringHeat& rHeat = (*is)->Heats()[ (*io)->HeatSeqNum() ];

					if ( rHeat.TabNum() == 0 )
						ostr << "-" << ends;
					else
						ostr << rHeat.TabNum() << ends;
					m_listLineups.SetItemText(count,2,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					// heat#
					ostr << (*io)->HeatSeqNum() + 1 << ends;
					m_listLineups.SetItemText(count,3,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					// strand #
					ostr << strandNum << ends;
					m_listLineups.SetItemText(count,4,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					// lot #
					ostr << long(1 + io - rHeat.StrandBegin(strandNum)) << ends;
					m_listLineups.SetItemText(count,5,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					// #pieces
					ostr << (*io)->NumPieces() << ends;
					m_listLineups.SetItemText(count,6,ostr.str());
					ostr.freeze(false);
					ostr.seekp(0);

					++count;
					
				}
			}
		}
	}
}


void COrderInquiryDlg::SetChemList()
{
	m_listChems.DeleteAllItems();

	ostrstream ostr;

	int count = 0;

	AddEltToChemList(CChem::ELT_C, count);
	AddEltToChemList(CChem::ELT_MN,count);

	for ( int elt = 0; elt < CChem::NumElements; ++elt ) {

		if ( elt != CChem::ELT_C && elt != CChem::ELT_MN )
			AddEltToChemList(CChem::Element(elt),count);

	}	
}


void COrderInquiryDlg::AddEltToChemList(CChem::Element elt, int& count)
{
	ostrstream ostr;

	const CString& eltStr = CChem::ToString(elt);
	
	m_listChems.InsertItem(count,eltStr);

	ostr << setw(6) << setprecision(4) << m_pOrder->MinElement(elt) << ends;
	m_listChems.SetItemText(count,1,ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	ostr << setw(6) << setprecision(4) << m_pOrder->MaxElement(elt) << ends;
	m_listChems.SetItemText(count,2,ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	ostr << setw(1) << m_pOrder->ElementReason(elt) << ends;
	m_listChems.SetItemText(count,3,ostr.str());
	ostr.freeze(false);
	ostr.seekp(0);

	++count;
}





void COrderInquiryDlg::OnButtonLookup() 
{
	//  Get the mill order num from the control

	UpdateData(TRUE);


	// look up the mill order num

	vector<COrder*>& orders = TheSnapshot.Orders();

	// FP change
	//COrder* pFound = COrder::FindOrder(m_orderNum,orders);
	COrder* pFound = COrder::FindOrder(m_fpOrderNum,orders);

	if ( pFound == 0 ) {

		// FP change -- lots
		
		vector<COrder*> matchingOrders;
		COrder::FindOrdersFromMillOrderNum(m_fpOrderNum,orders,matchingOrders);

		if ( matchingOrders.size() == 0 ) {
			ostrstream ostr;
			ostr.fill('0');
			ostr << "Order# " 
				 // FP change
				 //<< setw(7) << m_orderNum 
				 << setw(15) << LPCTSTR(m_fpOrderNum) 
				 << ": Cannot find a match!"
				 << ends;

			MessageBox(ostr.str(),"Missing order",MB_ICONERROR);
			ostr.freeze(false);
		}
		else {
			if ( matchingOrders.size() > 100 ) {
				MessageBox("Too many matching orders, list is truncated to first 100.");
				matchingOrders.resize(100);
			}
			m_matchingOrders = matchingOrders;  // copy
			m_pOrder = m_matchingOrders[0];
			m_indexMatchingOrders = 0;
			SetFields();
			SetOrderList();
		}
	}
	else {
		m_pOrder = pFound;
		m_matchingOrders.clear();
		m_indexMatchingOrders = 0;
		SetFields();
		SetOrderList();
	}
}



void COrderInquiryDlg::OnSelchangeComboMatchingOrders() 
{
	UpdateData(TRUE);
	m_pOrder = m_matchingOrders[m_indexMatchingOrders];
	SetFields();
}
