// PpgCICodeSelect.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "PpgCICodeSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Snapshot.h"
#include "OrderSelection.h"

#include "CIBogieSet.h"

/////////////////////////////////////////////////////////////////////////////
//   CPpgCICodeSelect property page
//
//   Display CI code bogies and allow selection of CI codes to filter
//
//	Designed to be used from CPSheetAvailOrders
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CPpgCICodeSelect, CPropertyPage)

CPpgCICodeSelect::CPpgCICodeSelect() : CPropertyPage(CPpgCICodeSelect::IDD)
{
	//{{AFX_DATA_INIT(CPpgCICodeSelect)
	m_bUseAllCodes = FALSE;
	m_bShowAllColumns = FALSE;
	m_selectedCodes = _T("");
	//}}AFX_DATA_INIT

	m_pBogieMgr			= 0;
	m_pSelectedBogies	= 0;
	m_pOrderSelection	= 0;

	m_hasBeenActivated	= false;
}


CPpgCICodeSelect::~CPpgCICodeSelect()
{
}


void CPpgCICodeSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPpgCICodeSelect)
	DDX_Control(pDX, IDC_CHECK_SHOW_ALL, m_showAllCheckbox);
	DDX_Control(pDX, IDC_CHECK_ALL_CODES, m_allCodesCheckbox);
	DDX_Control(pDX, IDC_LIST_CI_CODES, m_ciCodesList);
	DDX_Check(pDX, IDC_CHECK_ALL_CODES, m_bUseAllCodes);
	DDX_Check(pDX, IDC_CHECK_SHOW_ALL, m_bShowAllColumns);
	DDX_Text(pDX, IDC_EDIT_SELECTED_CODES, m_selectedCodes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPpgCICodeSelect, CPropertyPage)
	//{{AFX_MSG_MAP(CPpgCICodeSelect)
	ON_BN_CLICKED(IDC_CHECK_ALL_CODES, OnCheckAllCodes)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CI_CODES, OnClickListCiCodes)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ALL, OnCheckShowAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPpgCICodeSelect message handlers


//
//  Must be called before initial activation
//

void CPpgCICodeSelect::SetOrderSelection(COrderSelection* pSelect)
{
	m_pOrderSelection	= pSelect;
	m_pBogieMgr			= &TheSnapshot.BogieMgr();
	m_pSelectedBogies	= &pSelect->SelectedBogies();

	m_hasBeenActivated = false;
}


static int StdHeaderWidths[50];


BOOL CPpgCICodeSelect::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	//  Set the headers for the list control.

	static const char* strCodes		= "CI Codes";
	static const char* strCommodity	= "Commodity";
	static const char* strBogie		= "Bogie";
	static const char* strRolled	= "Rolled";
	static const char* strOnLu		= "On LU";
	static const char* strRemaining = "Remaining";
	static const char* strOver      = "Over";
	static const char* strSlabbed   = "Slabbed";
	static const char* strCMS		= "CMS";
	static const char* strShaves	= "Shaves";
	static const char* strOther		= "Other";
	static const char* strOn910     = "910";
	static const char* strOn909		= "909";
	static const char* strAfter910  = "After910";
	static const char* strAfter909	= "After909";

	StdHeaderWidths[ 0] = 15 + max(m_ciCodesList.GetStringWidth(strCodes),
								   m_ciCodesList.GetStringWidth("99,99,99"));
	StdHeaderWidths[ 1] = 15 + max(m_ciCodesList.GetStringWidth(strCommodity),
								   m_ciCodesList.GetStringWidth("MMMMMMMMMMMMM"));
	StdHeaderWidths[ 2] = 15 + max(m_ciCodesList.GetStringWidth(strBogie),
							       m_ciCodesList.GetStringWidth("99999"));
	StdHeaderWidths[ 3] = 15 + max(m_ciCodesList.GetStringWidth(strRolled),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 4] = 15 + max(m_ciCodesList.GetStringWidth(strOnLu),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 5] = 15 + max(m_ciCodesList.GetStringWidth(strRemaining),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 6] = 15 + max(m_ciCodesList.GetStringWidth(strOver),
							       m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 7] = 15 + max(m_ciCodesList.GetStringWidth(strSlabbed),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 8] = 15 + max(m_ciCodesList.GetStringWidth(strCMS),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[ 9] = 15 + max(m_ciCodesList.GetStringWidth(strShaves),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[10] = 15 + max(m_ciCodesList.GetStringWidth(strOther),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[11] = 15 + max(m_ciCodesList.GetStringWidth(strOn910),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[12] = 15 + max(m_ciCodesList.GetStringWidth(strOn909),
							       m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[13] = 15 + max(m_ciCodesList.GetStringWidth(strAfter910),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[14] = 15 + max(m_ciCodesList.GetStringWidth(strAfter909),
								   m_ciCodesList.GetStringWidth("9999"));
	StdHeaderWidths[15] = 15 + max(m_ciCodesList.GetStringWidth(strOver),
							       m_ciCodesList.GetStringWidth("9999"));


	m_ciCodesList.InsertColumn(  0, strCodes,		LVCFMT_CENTER,	StdHeaderWidths[ 0],  0);
	m_ciCodesList.InsertColumn(  1, strCommodity,	LVCFMT_LEFT,	StdHeaderWidths[ 1],  1);
	m_ciCodesList.InsertColumn(  2, strBogie,		LVCFMT_RIGHT,	StdHeaderWidths[ 2],  2);
	m_ciCodesList.InsertColumn(  3, strRolled,		LVCFMT_RIGHT,	StdHeaderWidths[ 3],  3);
	m_ciCodesList.InsertColumn(  4, strOnLu,		LVCFMT_RIGHT,	StdHeaderWidths[ 4],  4);
	m_ciCodesList.InsertColumn(  5, strRemaining,	LVCFMT_RIGHT,	StdHeaderWidths[ 5],  5);
	m_ciCodesList.InsertColumn(  6, strOver,		LVCFMT_RIGHT,	StdHeaderWidths[ 6],  6);
	m_ciCodesList.InsertColumn(  7, strSlabbed,		LVCFMT_RIGHT,	StdHeaderWidths[ 7],  7);
	m_ciCodesList.InsertColumn(  8, strCMS,			LVCFMT_RIGHT,	StdHeaderWidths[ 8],  8);
	m_ciCodesList.InsertColumn(  9, strShaves,		LVCFMT_RIGHT,	StdHeaderWidths[ 9],  9);
	m_ciCodesList.InsertColumn( 10, strOther,		LVCFMT_RIGHT,	StdHeaderWidths[10], 10);
	m_ciCodesList.InsertColumn( 11, strOn910,		LVCFMT_RIGHT,	StdHeaderWidths[11], 11);
	m_ciCodesList.InsertColumn( 12, strOn909,		LVCFMT_RIGHT,	StdHeaderWidths[12], 12);
	m_ciCodesList.InsertColumn( 13, strAfter910,	LVCFMT_RIGHT,	StdHeaderWidths[13], 13);
	m_ciCodesList.InsertColumn( 14, strAfter909,	LVCFMT_RIGHT,	StdHeaderWidths[14], 14);
	m_ciCodesList.InsertColumn( 15, strOver,		LVCFMT_RIGHT,	StdHeaderWidths[15], 15);

	SetCodeListHeaderWidths();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


////////////////////////////////////////////////////////////////
//	
//	Callbacks
//	
////////////////////////////////////////////////////////////////

// We detect changes to all controls of signficance,
//    and mark the page modified.

void CPpgCICodeSelect::OnCheckAllCodes() 
{
	SetModified(true);
}



void CPpgCICodeSelect::OnClickListCiCodes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetModified(true);
	
	*pResult = 0;
}



//  This callback has UI effects only,
//    doesn't affect the filter.

void CPpgCICodeSelect::OnCheckShowAll() 
{
	SetCodeListHeaderWidths();
}



void CPpgCICodeSelect::SetCodeListHeaderWidths()
{
	bool useZero = m_showAllCheckbox.GetCheck() != 1;
	
	for ( int i=3; i<=12; ++i )
		if ( useZero )
			m_ciCodesList.SetColumnWidth(  i, 0 );
		else
			m_ciCodesList.SetColumnWidth(  i, StdHeaderWidths[i] );
}




////////////////////////////////////////////////////////////////
//	
//	Moving data
//	
////////////////////////////////////////////////////////////////

//
// Move data from controls to local data members to orderSelection
// Note: COrderSelection is responsible for setting its modified flag.
//
	
bool CPpgCICodeSelect::LocalToOrderSelection()
{
	// move data from controls to local data members, detect error condition

	if ( ! UpdateData(true) )
		return false;


	// move data from local data members to OrderSelection

	vector<int> indexes;

	for ( int index = m_ciCodesList.GetNextItem(-1,LVNI_SELECTED);
		  index != -1;
	      index = m_ciCodesList.GetNextItem(index,LVNI_SELECTED) ) 

		indexes.push_back(index);


	m_pOrderSelection->SetSelectedBogies(indexes);

	m_pOrderSelection->SetUseAllCICodes(m_bUseAllCodes);

	// we are really modified only if the OrderSelection is modified.


	SetModified(m_pOrderSelection->IsModified());

	return true;
}



//  Move data to local data members

void CPpgCICodeSelect::OrderSelectionToLocal()
{

	m_bUseAllCodes  = m_pOrderSelection->UseAllCICodes();
	SetCodeListItems();
}



//  a little helper macro
//  if a value is zero or negative, we want to print nothing

#define strval(x) ( (x) <= 0 ? "" : _itoa((x),buf,10) )



void CPpgCICodeSelect::SetCodeListItems()
{
	char buf[50];
	ostrstream ostr;


	m_ciCodesList.DeleteAllItems();

	// sort 'em, someday

	{

		int count = 0;
		for ( CCICodeBogieMgr::T_BogiesVec::const_iterator ib = m_pBogieMgr->BogiesBegin();
		      ib != m_pBogieMgr->BogiesEnd();
			  ++ib, ++count ) {
	
			// write out the codes, for column 0
			for ( vector<int>::const_iterator ii = (*ib)->GetCICodeGroup().Codes().begin();
				  ii != (*ib)->GetCICodeGroup().Codes().end();
				  ++ii ) {
	
				if ( ii != (*ib)->GetCICodeGroup().Codes().begin() )
					ostr << ",";
				ostr << (*ii);
			}
	
			ostr << ends;
	
			m_ciCodesList.InsertItem(count,ostr.str());
			ostr.freeze(false);
			ostr.seekp(0);
	
			//  write the name
			m_ciCodesList.SetItemText( count,  1, (*ib)->GetCICodeGroup().Name() );
	
			//  write the various fields
			m_ciCodesList.SetItemText( count,  2, strval( (*ib)->m_bogie				) );
			m_ciCodesList.SetItemText( count,  3, strval( (*ib)->m_rolled				) );
//			m_ciCodesList.SetItemText( count,  3, strval( (*ib)->m_pCIBogieSet->m_ROLLED				) );
			m_ciCodesList.SetItemText( count,  4, strval( (*ib)->m_onLu					) );
			m_ciCodesList.SetItemText( count,  5, strval( (*ib)->m_remainingToSchedule	) );
			m_ciCodesList.SetItemText( count,  6, strval( (*ib)->m_amountOverOnLu		) );
			m_ciCodesList.SetItemText( count,  7, strval( (*ib)->m_slabbed				) );
			m_ciCodesList.SetItemText( count,  8, strval( (*ib)->m_cms					) );
			m_ciCodesList.SetItemText( count,  9, strval( (*ib)->m_shaves				) );
			m_ciCodesList.SetItemText( count, 10, strval( (*ib)->m_otherUnavail			) );
			m_ciCodesList.SetItemText( count, 11, strval( (*ib)->m_on910				) );
			m_ciCodesList.SetItemText( count, 12, strval( (*ib)->m_on909				) );
			m_ciCodesList.SetItemText( count, 13, strval( (*ib)->m_remainingAfter910	) );
			m_ciCodesList.SetItemText( count, 14, strval( (*ib)->m_remainingAfter909	) );
			m_ciCodesList.SetItemText( count, 15, strval( (*ib)->m_amountOverBogie		) );
		}

	}

	// Mark the selected items

	{
		for ( CCICodeBogieMgr::T_BogiesVec::const_iterator ib = m_pSelectedBogies->begin();
		      ib != m_pSelectedBogies->end();
		      ++ib ) {
			
			CCICodeBogieMgr::T_BogiesVec::const_iterator ix = find(m_pBogieMgr->BogiesBegin(),
														   m_pBogieMgr->BogiesEnd(),
														   (*ib));

			if ( ix != m_pBogieMgr->BogiesEnd() ) {
				int index = ix - m_pBogieMgr->BogiesBegin();
				if ( m_ciCodesList.SetItemState(index,LVIS_SELECTED,LVIS_SELECTED) )
					TRACE("Successful setting state.\n");
				else
					TRACE("NOT successful setting state.\n");
			}
		}
	}
	
}




BOOL CPpgCICodeSelect::OnSetActive() 
{
	//  MS documentation says to call CPropertyPage::OnSetActive() first,
	//  but it does not call UpdateData the first time.
	
	if ( ! m_hasBeenActivated ) {
		OrderSelectionToLocal();
		UpdateData(false);
	}

	m_hasBeenActivated = true;

	return TRUE;
}
