// PpgOrderChemRange.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "PpgOrderChemRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "OrderSelection.h"
#include "SpecChemRangePreset.h"
#include "Snapshot.h"
#include "SpecCmp.h"


/////////////////////////////////////////////////////////////////////////////
//   CPpgOrderChemRange property page
//
//  Property page to display chem range limits for up to five chemistries
//  Also displays preset ranges for current spec and allows them to be used.
//
//  Designed for use from CPSheetAvailOrders
//
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CPpgOrderChemRange, CPropertyPage)

CPpgOrderChemRange::CPpgOrderChemRange() : CPropertyPage(CPpgOrderChemRange::IDD)
{
	//{{AFX_DATA_INIT(CPpgOrderChemRange)
	m_chem3 = _T("");
	m_chem4 = _T("");
	m_chem5 = _T("");
	m_dMaxChem1 = 0.0;
	m_dMaxChem2 = 0.0;
	m_dMaxChem3 = 0.0;
	m_dMaxChem4 = 0.0;
	m_dMaxChem5 = 0.0;
	m_dMinChem1 = 0.0;
	m_dMinChem2 = 0.0;
	m_dMinChem3 = 0.0;
	m_dMinChem4 = 0.0;
	m_dMinChem5 = 0.0;
	m_presetRange = _T("");
	//}}AFX_DATA_INIT
	
	m_hasBeenActivated = false;
	m_pOrderSelection = 0;

}

CPpgOrderChemRange::~CPpgOrderChemRange()
{
}

void CPpgOrderChemRange::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPpgOrderChemRange)
	DDX_Control(pDX, IDC_LIST_PRESET_RANGES, m_listPresetRanges);
	DDX_CBString(pDX, IDC_COMBO_CHEM3, m_chem3);
	DDX_CBString(pDX, IDC_COMBO_CHEM4, m_chem4);
	DDX_CBString(pDX, IDC_COMBO_CHEM5, m_chem5);
	DDX_Text(pDX, IDC_EDIT_MAX_CHEM1, m_dMaxChem1);
	DDX_Text(pDX, IDC_EDIT_MAX_CHEM2, m_dMaxChem2);
	DDX_Text(pDX, IDC_EDIT_MAX_CHEM3, m_dMaxChem3);
	DDX_Text(pDX, IDC_EDIT_MAX_CHEM4, m_dMaxChem4);
	DDX_Text(pDX, IDC_EDIT_MAX_CHEM5, m_dMaxChem5);
	DDX_Text(pDX, IDC_EDIT_MIN_CHEM1, m_dMinChem1);
	DDX_Text(pDX, IDC_EDIT_MIN_CHEM2, m_dMinChem2);
	DDX_Text(pDX, IDC_EDIT_MIN_CHEM3, m_dMinChem3);
	DDX_Text(pDX, IDC_EDIT_MIN_CHEM4, m_dMinChem4);
	DDX_Text(pDX, IDC_EDIT_MIN_CHEM5, m_dMinChem5);
	DDX_LBString(pDX, IDC_LIST_PRESET_RANGES, m_presetRange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPpgOrderChemRange, CPropertyPage)
	//{{AFX_MSG_MAP(CPpgOrderChemRange)
	ON_CBN_EDITCHANGE(IDC_COMBO_CHEM1, OnEditchangeComboChemAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CHEM1, OnChangeEditMaxChemAll)
	ON_EN_CHANGE(IDC_EDIT_MIN_CHEM1, OnChangeEditMinChemAll)
	ON_CBN_SELCHANGE(IDC_COMBO_CHEM1, OnSelchangeComboChemAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET_ALL, OnButtonResetAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CARBON, OnButtonResetCarbon)
	ON_BN_CLICKED(IDC_BUTTON_RESET_MANGANESE, OnButtonResetManganese)
	ON_BN_CLICKED(IDC_BUTTON_RESET_3, OnButtonReset3)
	ON_BN_CLICKED(IDC_BUTTON_RESET_4, OnButtonReset4)
	ON_BN_CLICKED(IDC_BUTTON_RESET_5, OnButtonReset5)
	ON_CBN_EDITCHANGE(IDC_COMBO_CHEM2, OnEditchangeComboChemAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_CHEM3, OnEditchangeComboChemAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_CHEM4, OnEditchangeComboChemAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_CHEM5, OnEditchangeComboChemAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CHEM2, OnChangeEditMaxChemAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CHEM3, OnChangeEditMaxChemAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CHEM4, OnChangeEditMaxChemAll)
	ON_EN_CHANGE(IDC_EDIT_MAX_CHEM5, OnChangeEditMaxChemAll)
	ON_EN_CHANGE(IDC_EDIT_MIN_CHEM2, OnChangeEditMinChemAll)
	ON_EN_CHANGE(IDC_EDIT_MIN_CHEM3, OnChangeEditMinChemAll)
	ON_EN_CHANGE(IDC_EDIT_MIN_CHEM4, OnChangeEditMinChemAll)
	ON_EN_CHANGE(IDC_EDIT_MIN_CHEM5, OnChangeEditMinChemAll)
	ON_CBN_SELCHANGE(IDC_COMBO_CHEM2, OnSelchangeComboChemAll)
	ON_CBN_SELCHANGE(IDC_COMBO_CHEM3, OnSelchangeComboChemAll)
	ON_CBN_SELCHANGE(IDC_COMBO_CHEM4, OnSelchangeComboChemAll)
	ON_CBN_SELCHANGE(IDC_COMBO_CHEM5, OnSelchangeComboChemAll)
	ON_LBN_SELCHANGE(IDC_LIST_PRESET_RANGES, OnSelchangeListPresetRanges)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPpgOrderChemRange message handlers


//
//  THis must be called before being activated
//

void CPpgOrderChemRange::SetOrderSelection(COrderSelection* pSelect)
{
	m_pOrderSelection = pSelect;
	m_hasBeenActivated = false;
}




BOOL CPpgOrderChemRange::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	//  Subclass the dropEdit buttons, set the bitmaps to display
	//  and set up the backpointer

	for ( int i=0; i<5; ++i ) {
		VERIFY( m_minEdits[i].SetBitmap(IDB_CALCBTN) );
		VERIFY( m_maxEdits[i].SetBitmap(IDB_CALCBTN) );
		m_minEdits[i].SetMyParent(this);
		m_maxEdits[i].SetMyParent(this);
	}

	VERIFY( m_minEdits[0].AttachEdit(IDC_EDIT_MIN_CHEM1,this) );
	VERIFY( m_minEdits[1].AttachEdit(IDC_EDIT_MIN_CHEM2,this) );
	VERIFY( m_minEdits[2].AttachEdit(IDC_EDIT_MIN_CHEM3,this) );
	VERIFY( m_minEdits[3].AttachEdit(IDC_EDIT_MIN_CHEM4,this) );
	VERIFY( m_minEdits[4].AttachEdit(IDC_EDIT_MIN_CHEM5,this) );

	VERIFY( m_maxEdits[0].AttachEdit(IDC_EDIT_MAX_CHEM1,this) );
	VERIFY( m_maxEdits[1].AttachEdit(IDC_EDIT_MAX_CHEM2,this) );
	VERIFY( m_maxEdits[2].AttachEdit(IDC_EDIT_MAX_CHEM3,this) );
	VERIFY( m_maxEdits[3].AttachEdit(IDC_EDIT_MAX_CHEM4,this) );
	VERIFY( m_maxEdits[4].AttachEdit(IDC_EDIT_MAX_CHEM5,this) );
	

	// set up element names and values
	// First element is (none) with value -1.
	// Note that carbon and manganese are always in the first two boxes
	//   and so are not listed as options on drop downs.

	eltNames.clear();
	elts.clear();

	eltNames.push_back("(none)");
	elts.push_back(CChem::Element(-1));

	for ( int elt=0; elt<CChem::NumElements; ++elt ) {

		if ( elt != CChem::ELT_C && elt != CChem::ELT_MN ) {

			const CString& name = CChem::ToString(CChem::Element(elt));
			eltNames.push_back(name);
			elts.push_back(CChem::Element(elt));
		}
	}
	

	SetChemListEntries();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPpgOrderChemRange::SetChemListEntries()
{
	CComboBox* boxes[5];
	//boxes[0] = (CComboBox*)GetDlgItem(IDC_COMBO_CHEM1); = CARBON, no box
	//boxes[1] = (CComboBox*)GetDlgItem(IDC_COMBO_CHEM2); = MANGANESE, no box
	boxes[2] = (CComboBox*)GetDlgItem(IDC_COMBO_CHEM3);
	boxes[3] = (CComboBox*)GetDlgItem(IDC_COMBO_CHEM4);
	boxes[4] = (CComboBox*)GetDlgItem(IDC_COMBO_CHEM5);

	{
		for ( int i=2; i<=4; ++i )
			boxes[i]->ResetContent();
	}

	for ( vector<CString>::iterator is = eltNames.begin();
		  is != eltNames.end();
		  ++is ) {

		for ( int i=2; i<=4; ++i )
			boxes[i]->AddString((*is));

	}
}




////////////////////////////////////////////////////////////////
//	
//	Callbacks
//	
////////////////////////////////////////////////////////////////


// Many of the callbacks just note that a change has occurred

void CPpgOrderChemRange::OnEditchangeComboChemAll() 
{
	SetModified(true);

}

void CPpgOrderChemRange::OnChangeEditMaxChemAll() 
{
	SetModified(true);	
}

void CPpgOrderChemRange::OnChangeEditMinChemAll() 
{
	SetModified(true);	
}

void CPpgOrderChemRange::OnSelchangeComboChemAll() 
{
	SetModified(true);	
}



BEGIN_MESSAGE_MAP(CChemBoundEdit, SECDropEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



//
// When an edit box for a chemistry bound is clicked,
//   pop up a calculator  for entry of a float.
//

void CChemBoundEdit::OnClicked()
{
	// Parse current value
	double val = 0.0;
	CString temp;

	GetWindowText(temp);
	_stscanf(temp, _T("%lf"), &val);

	SECPopupCalculator* pCalc = new SECPopupCalculator;

	// self-destruct when dismissed
	pCalc->m_bAutoDelete = TRUE;

	// Set window to receive result as WM_SETTEXT
	pCalc->SetOwner(this);

	// Set the initial value
	pCalc->SetValue(val);

	CRect r;
	GetWindowRect(&r);

	DWORD dwStyle = WS_VISIBLE | SEC_CS_RAISEDEDGE | SEC_PCS_DESTROY_ON_EQUALS;
	
	VERIFY( pCalc->Create(dwStyle, r, this) );
	
	m_pMyParent->SetModified(true);

}


//
//  If the user types into the edit box, allow only numeric entry.
//

void CChemBoundEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bHandle = FALSE;

	if ( nChar >= _T('0') && nChar <= _T('9') )
		bHandle = TRUE;
	else if ( nChar == _T('.') ) {
		CString temp;
		GetWindowText(temp);
		if (temp.Find(_T('.')) == -1 )
			bHandle = TRUE;
	}

	if ( bHandle ) {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		m_pMyParent->SetModified(true);
	}
	else
		MessageBeep(UINT(-1));
}




////////////////////////////////////////////////////////////////
//	
//	Moving data
//	
////////////////////////////////////////////////////////////////


//  a little helper
//  checks an elt/minVal/maxVal
//  if elt is not assigned, do nothing
//  else verify that the 0 <= minVal <= maxVal <= 100.0
//       if not, record the fact.

static void checkandassign(CString& eltStr,
						   double minVal,
						   double maxVal,
						   bool& bogus,
						   ostrstream& ostr,
						   double minChems[],
						   double maxChems[])
{
	static CString none = "(none)";

	if ( eltStr != none && eltStr != "") {											

		CChem::Element elt;
		bool found = CChem::ToElement(eltStr,elt);					
																
		if ( ! found || minVal < 0.0 || maxVal > 100.0 || maxVal< minVal ) {
																
			bogus = true;										
			ostr <<	"Bad range for element " << LPCTSTR(eltStr) << "\n"; 								
		}														
		else {													
			maxChems[elt] = maxVal;								
			minChems[elt] = minVal;								
		}														 
	}
}


// Move data from controls to locals to OrderSelection
// Really == true  ==> really move the data to the OrderSelection
// Really == false ==> just perform the error checking

bool CPpgOrderChemRange::LocalToOrderSelection(bool really /* = true */)
{
	if ( ! UpdateData(TRUE) )
		return false;

	double maxChems[CChem::NumElements];
	double minChems[CChem::NumElements];

	for ( int i=0; i<CChem::NumElements; ++i ) {
		maxChems[i] = 100.0;
		minChems[i] =   0.0;
	}

	
	ostrstream ostr;
	bool bogus = false;

	checkandassign(CString("C"),  m_dMinChem1,m_dMaxChem1,bogus,ostr,minChems,maxChems);
	checkandassign(CString("MN"), m_dMinChem2,m_dMaxChem2,bogus,ostr,minChems,maxChems);
	checkandassign(m_chem3,       m_dMinChem3,m_dMaxChem3,bogus,ostr,minChems,maxChems);
	checkandassign(m_chem4,       m_dMinChem4,m_dMaxChem4,bogus,ostr,minChems,maxChems);
	checkandassign(m_chem5,       m_dMinChem5,m_dMaxChem5,bogus,ostr,minChems,maxChems);

	if ( bogus ) {

		ostr << "\nPlease correct.\n"
			 << ends;

		MessageBox(ostr.str());
		ostr.freeze(false);

		return false;
	}


	if ( ! really )
		return true;


	for ( int i=0; i<CChem::NumElements; ++i ) {
		
		m_pOrderSelection->SetMaxChem(i,maxChems[i]);
		m_pOrderSelection->SetMinChem(i,minChems[i]);

	}

	SetModified(m_pOrderSelection->IsModified());

	return true;
}



//  Move data from OrderSelection to local data members

void CPpgOrderChemRange::OrderSelectionToLocal()
{
	CString* elts[5] = { 0, 
						 0, 
						 &m_chem3, 
						 &m_chem4, 
						 &m_chem5 };

	double* minChems[5] = { &m_dMinChem1,
							&m_dMinChem2,
							&m_dMinChem3,
							&m_dMinChem4,
							&m_dMinChem5 };

	double* maxChems[5] = { &m_dMaxChem1,
							&m_dMaxChem2,
							&m_dMaxChem3,
							&m_dMaxChem4,
							&m_dMaxChem5 };

	
	{
		for ( int i=0; i<5; ++i ) {
			*(minChems[i]) = 0.0;
			*(maxChems[i]) = 100.0;
		}
	}

	{ 
		for ( int i=2; i<=4; ++i ) 
			*(elts[i]) = "(none)";
	}

	m_dMinChem1 = m_pOrderSelection->MinChem(CChem::ELT_C);
	m_dMaxChem1 = m_pOrderSelection->MaxChem(CChem::ELT_C);
	m_dMinChem2 = m_pOrderSelection->MinChem(CChem::ELT_MN);
	m_dMaxChem2 = m_pOrderSelection->MaxChem(CChem::ELT_MN);
	
	int current = 2;
	for ( int elt=0; current <= 4 && elt<CChem::NumElements; ++elt ) { 

		if (elt != CChem::ELT_C 
			&&
			elt != CChem::ELT_MN
			&&
			(m_pOrderSelection->MinChem(elt) != 0.0
			 ||
			 m_pOrderSelection->MaxChem(elt) != 100.0) ) {

			*(elts[current]) = CChem::ToString(CChem::Element(elt));
			*(minChems[current]) = m_pOrderSelection->MinChem(elt);
			*(maxChems[current]) = m_pOrderSelection->MaxChem(elt);

			++current;
		}
	}

	SetPresetSelections();
}


//
//  If we have a spec selected, display the presets
//  This will be called each time the page is entered, 
//    so we pick up modification to the selected spec made on the other page.
//

void CPpgOrderChemRange::SetPresetSelections()
{

	m_listPresetRanges.ResetContent();

	selectedSpecChemRangePresets.clear();

	if ( m_pOrderSelection->UseAllSpecs() )
		return;

	const vector<CString>& selSpecs = m_pOrderSelection->SelectedSpecs();


	if ( selSpecs.size() == 0 )
		return;


	const vector<CSpecChemRangePreset>&	presets
		= TheSnapshot.SpecChemRangePresets();

	for ( vector<CSpecChemRangePreset>::const_iterator ip = presets.begin();
		  ip != presets.end();
		  ++ip ) {

		CSpecCmp cmp( (*ip).Spec() );

		if ( find_if(selSpecs.begin(),
					 selSpecs.end(),
				     cmp) != selSpecs.end() ) {

			selectedSpecChemRangePresets.push_back( (*ip) );

			CString str;
			str.Format("C: %4.2f-%4.2f%%, Mn: %4.2f-%4.2f%%", 
					   (*ip).MinC(), 
					   (*ip).MaxC(), 
					   (*ip).MinMn(),
					   (*ip).MaxMn());
			m_listPresetRanges.AddString( str );

		}
	}
}






BOOL CPpgOrderChemRange::OnSetActive() 
{
	if ( ! m_hasBeenActivated ) {
		UpdateData(FALSE);			// Make sure we have the controls init'd
		OrderSelectionToLocal();
		UpdateData(FALSE);			// get data out to controls
	}

	m_hasBeenActivated = true;

	return TRUE;
}



BOOL CPpgOrderChemRange::OnKillActive() 
{
	if ( ! CPropertyPage::OnKillActive() )
		return FALSE;

	//  error check

	return LocalToOrderSelection(false /* = not really */);
}

void CPpgOrderChemRange::OnButtonResetAll() 
{
	OnButtonResetCarbon();
	OnButtonResetManganese();
	OnButtonReset3();
	OnButtonReset4();
	OnButtonReset5();
	SetModified(true);
	
}

void CPpgOrderChemRange::OnButtonResetCarbon() 
{
	UpdateData(true);
	m_dMinChem1 = 0.0;
	m_dMaxChem1 = 100.0;
	UpdateData(false);
	SetModified(true);
}

void CPpgOrderChemRange::OnButtonResetManganese() 
{
	UpdateData(true);
	m_dMinChem2 = 0.0;
	m_dMaxChem2 = 100.0;
	UpdateData(false);
	SetModified(true);
}

void CPpgOrderChemRange::OnButtonReset3() 
{
	UpdateData(true);
	m_chem3		= "(none)";
	m_dMinChem3 = 0.0;
	m_dMaxChem3 = 100.0;
	UpdateData(false);
	SetModified(true);
}

void CPpgOrderChemRange::OnButtonReset4() 
{
	UpdateData(true);
	m_chem4		= "(none)";
	m_dMinChem4 = 0.0;
	m_dMaxChem4 = 100.0;
	UpdateData(false);
	SetModified(true);
}

void CPpgOrderChemRange::OnButtonReset5() 
{
	UpdateData(true);
	m_chem4		= "(none)";
	m_dMinChem4 = 0.0;
	m_dMaxChem4 = 100.0;
	UpdateData(false);
	SetModified(true);
}






void CPpgOrderChemRange::OnSelchangeListPresetRanges() 
{
	UpdateData(TRUE);

	int index = m_listPresetRanges.GetCurSel();

	if ( index == LB_ERR )
		return;

	const CSpecChemRangePreset& x = selectedSpecChemRangePresets[index];
	m_dMinChem1 = x.MinC();
	m_dMaxChem1 = x.MaxC();
	m_dMinChem2 = x.MinMn();
	m_dMaxChem2 = x.MaxMn();

	UpdateData(FALSE);
	SetModified(true);
}
