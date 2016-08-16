// CastStringIdDlg.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CastStringIdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SpecMgr.h"
#include "Snapshot.h"
#include "SpecCmp.h"
#include "MiscConstants.h"


//////////////////////////////////////////////////////////////////////////
//
//  CCastStringIdDlg
//
//	This dialog is used to edit basic cast string information,
//	including 
//	
//		(1) string id
//		(2) aim heat size
//		(3) num heats limit
//		(4)	spec + min/max mn & c
//		(5)	start/end widths on 2 strands
//		(6)	cast start time
//      and lots more -- we just kept adding information.
//	
//	It is used (a) to gather information in order to create a new string
//	           (b) to edit this information on an existing string
//	
//	
//	Elements are:
//      (1) string id = caster + year + week + seqNum
//      (2) heat size = min + aim + max
//      (3) limits = #heats  + tons
//	    (4) buttons = ok + cancel
//      and lots more
//
//  Certain elements have special processing associated with them:
//
//  Caster -- can change when in create mode
//            is disabled when in edit mode
//
//            when changed: modify min/max heat size
//
//  Aim heat size -- when changed: recompute tons
//
//	Num heats -- when changed: recompute tons
//
//	OnOK -- verify that the id is not already in use.
//          (dialog is initialized with (for each caster) 
//				a list of ids which are in use 
//	           -- when editing, we need the list only for the caster in the id,
//                since that can't be changed).
//		 -- verify that the aim heat size lies between min/max values
//	
//
//  Usage: creating
//
//		// you will need to compute in-use lineup ids 
//      // Assume that CurrentCasterScen(casterNum) returns some casterScen of interest.
//    
//		vector<CCastStringId> usedIds[4];
//
//		CurrentCasterScen(1)->GetLineupIds(usedIds[1]);
//		GetCurrentCasterScen(2)->GetLineupIds(usedIds[2]);
//		GetCurrentCasterScen(3)->GetLineupIds(usedIds[3]);
//
//		CCastStringId id(0,0,1,0);           // set to desired initial value
//		CCastStringMiscProps props(1,1);     // set to desired initial value
//
//		CCastStringIdDlg dlg;
//	
//		dlg.m_pId				= &id;
//		dlg.m_usedIds[Caster::C1]		= usedIds[Caster::C1];  // copy
//		dlg.m_usedIds[Caster::C2]		= usedIds[Caster::C2];  // copy
//		dlg.m_usedIds[Caster::C3]		= usedIds[Caster::C3];  // copy
//		dlg.m_usedIds[Caster::C4]		= usedIds[Caster::C4];  // copy
//		dlg.m_usedIds[Caster::C5]		= usedIds[Caster::C5];  // copy
//		dlg.m_pProps			= &props;
//		dlg.m_isEditing		= false;
//		dlg.m_setInitialId	= true;
//	
//		if ( dlg.DoModal() == IDOK ) 
//			//CreateString(id,props);
//
//
//  Usage: editing
//
//	 // it is recommended you do _not_ edit the id and miscProps of the string
//   //  directly, but rather make copies, then detect changes afterward
//	 // when editing, changes to caster# are not allowed, so we only need to supply
//   //  in-use lineup ids for this string's caster.
//   
//		CCastStringIdDlg dlg;
//
//		CCastStringId id = pString->GetId();
//		CCastStringMiscProps props = pString->GetProps();
//
//		dlg.m_pId						= &id;
//		dlg.m_usedIds[casterNum]		= usedIds;  // copy
//		dlg.m_pProps					= &props;	// non-const accessor
//		dlg.m_isEditing				= true;
//		dlg.m_setInitialId			= false;
//	
//		if ( dlg.DoModal() == IDOK ) {
//
//			if ( id != pString->Id() ) {
//				//	The id has changed.
//			}
//		
//			if ( props != pString->GetProps() ) {
//				// the miscProps has changed
//			}
//		}
//
//
//////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// CCastStringIdDlg dialog


CCastStringIdDlg::CCastStringIdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCastStringIdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCastStringIdDlg)
	m_caster = 0;
	m_num = 0;
	m_week = 0;
	m_year = 0;
	m_numHeats = 0;
	m_tons = 0;
	m_heatSizeAim = 0;
	m_heatSizeMax = 0;
	m_heatSizeMin = 0;
	m_heatSizeMaxLimit = 0;
	m_heatSizeMinLimit = 0;
	m_strand1End = 0;
	m_strand2End = 0;
	m_strand1Start = 0;
	m_strand2Start = 0;
	m_maxMn = 0.0f;
	m_maxCarbon = 0.0f;
	m_minCarbon = 0.0f;
	m_minMn = 0.0f;
	m_spec = _T("");
	m_turnaround = 0;
	//}}AFX_DATA_INIT

	m_pId				= 0;
	m_pProps			= 0;
	m_isEditing		= true;
	m_setInitialId	= false;
}


void CCastStringIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCastStringIdDlg)
	DDX_Control(pDX, IDC_EDIT_TURNAROUND_TIME, m_editTurnaround);
	DDX_Control(pDX, IDC_COMBO_PRESETS, m_comboPresets);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_comboSpec);
	DDX_Control(pDX, IDC_EDIT_START1, m_editStrand1Start);
	DDX_Control(pDX, IDC_EDIT_END1, m_editStrand1End);
	DDX_Control(pDX, IDC_SPIN_END2, m_spinStrand2End);
	DDX_Control(pDX, IDC_SPIN_END1, m_spinStrand1End);
	DDX_Control(pDX, IDC_SPIN_START2, m_spinStrand2Start);
	DDX_Control(pDX, IDC_SPIN_START1, m_spinStrand1Start);
	DDX_Control(pDX, IDC_EDIT_START2, m_editStrand2Start);
	DDX_Control(pDX, IDC_EDIT_END2, m_editStrand2End);
	DDX_Control(pDX, IDC_SPIN_NUM_HEATS, m_spinNumHeats);
	DDX_Control(pDX, IDC_EDIT_TONS, m_editTons);
	DDX_Control(pDX, IDC_EDIT_NUM_HEATS, m_editNumHeats);
	DDX_Control(pDX, IDC_SPIN_YEAR, m_spinYear);
	DDX_Control(pDX, IDC_SPIN_WEEK, m_spinWeek);
	DDX_Control(pDX, IDC_SPIN_NUM, m_spinNum);
	DDX_Control(pDX, IDC_SPIN_CASTER, m_spinCaster);
	DDX_Control(pDX, IDC_EDIT_YEAR, m_editYear);
	DDX_Control(pDX, IDC_EDIT_WEEK, m_editWeek);
	DDX_Control(pDX, IDC_EDIT_NUM, m_editNum);
	DDX_Control(pDX, IDC_EDIT_CASTER, m_editCaster);
	DDX_Text(pDX, IDC_EDIT_CASTER, m_caster);
	DDV_MinMaxInt(pDX, m_caster, Caster::FirstCaster, Caster::LastCaster);
	DDX_Text(pDX, IDC_EDIT_NUM, m_num);
	DDV_MinMaxInt(pDX, m_num, 0, 999);
	DDX_Text(pDX, IDC_EDIT_WEEK, m_week);
	DDV_MinMaxInt(pDX, m_week, 1, 53);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_year);
	DDV_MinMaxInt(pDX, m_year, 1990, 2999);
	DDX_Text(pDX, IDC_EDIT_NUM_HEATS, m_numHeats);
	DDV_MinMaxInt(pDX, m_numHeats, 0, 100);
	DDX_Text(pDX, IDC_EDIT_TONS, m_tons);
	DDV_MinMaxLong(pDX, m_tons, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT_HEAT_SIZE_AIM, m_heatSizeAim);
	DDX_Text(pDX, IDC_EDIT_HEAT_SIZE_MAX, m_heatSizeMax);
	DDX_Text(pDX, IDC_EDIT_HEAT_SIZE_MIN, m_heatSizeMin);
	DDX_Text(pDX, IDC_EDIT_HEAT_SIZE_MAX_LIMIT, m_heatSizeMaxLimit);
	DDX_Text(pDX, IDC_EDIT_HEAT_SIZE_MIN_LIMIT, m_heatSizeMinLimit);
	DDX_Text(pDX, IDC_EDIT_END1, m_strand1End);
	DDV_MinMaxInt(pDX, m_strand1End, 0, 99);
	DDX_Text(pDX, IDC_EDIT_END2, m_strand2End);
	DDV_MinMaxInt(pDX, m_strand2End, 0, 99);
	DDX_Text(pDX, IDC_EDIT_START1, m_strand1Start);
	DDV_MinMaxInt(pDX, m_strand1Start, 0, 99);
	DDX_Text(pDX, IDC_EDIT_START2, m_strand2Start);
	DDV_MinMaxInt(pDX, m_strand2Start, 0, 99);
	DDX_Text(pDX, IDC_EDIT_MAX_MN, m_maxMn);
	DDV_MinMaxFloat(pDX, m_maxMn, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_MAX_CARBON, m_maxCarbon);
	DDV_MinMaxFloat(pDX, m_maxCarbon, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_MIN_CARBON, m_minCarbon);
	DDV_MinMaxFloat(pDX, m_minCarbon, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_MIN_MN, m_minMn);
	DDV_MinMaxFloat(pDX, m_minMn, 0.f, 100.f);
	DDX_CBString(pDX, IDC_COMBO_SPEC, m_spec);
	DDX_Text(pDX, IDC_EDIT_TURNAROUND_TIME, m_turnaround);
	DDV_MinMaxInt(pDX, m_turnaround, 0, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCastStringIdDlg, CDialog)
	//{{AFX_MSG_MAP(CCastStringIdDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_NUM_HEATS, OnKillfocusEditNumHeats)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUM_HEATS, OnDeltaposSpinNumHeats)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CASTER, OnDeltaposSpinCaster)
	ON_EN_KILLFOCUS(IDC_EDIT_CASTER, OnKillfocusEditCaster)
	ON_EN_KILLFOCUS(IDC_EDIT_HEAT_SIZE_AIM, OnKillfocusEditHeatSizeAim)
	ON_EN_CHANGE(IDC_EDIT_NUM, OnChangeEditNum)
	ON_EN_CHANGE(IDC_EDIT_WEEK, OnChangeEditWeek)
	ON_EN_CHANGE(IDC_EDIT_YEAR, OnChangeEditYear)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUM, OnDeltaposSpinNum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WEEK, OnDeltaposSpinWeek)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_YEAR, OnDeltaposSpinYear)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESETS, OnSelchangeComboPresets)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC, OnSelchangeComboSpec)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEC, OnEditchangeComboSpec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCastStringIdDlg message handlers



BOOL CCastStringIdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( m_pId != 0 );

	m_idChanged = false;

	m_dateTimeCtrl.AttachDateTimeCtrl(IDC_EDIT_START_TIME,
									  this,
									  SEC_DTS_UPDOWN | SEC_DTS_CALENDAR);
	m_dateTimeCtrl.SetFormat("MMM/dd/yyyy hh:mm tt");
	m_dateTimeCtrl.EnableFastEntry();

	m_spinYear.SetRange(1990,2100);
	m_spinWeek.SetRange(0,53);
	m_spinNum.SetRange(0,999);
	m_spinCaster.SetRange(Caster::FirstCaster,Caster::LastCaster);
	m_spinNumHeats.SetRange(1,100);


	InitSpecEntries();

	if ( m_setInitialId )
		SetInitialId(m_pId->Caster());
	else {
	
		m_year		= m_pId->Year();
		m_week		= m_pId->Week();
		m_caster	= m_pId->Caster();
		m_num		= m_pId->StringId();
	}

	bool showLimits = (m_pProps != 0);

	if ( showLimits ) {
		m_tons			= m_pProps->Tons();
		m_numHeats		= m_pProps->NumHeats();
		m_heatSizeMin	= m_pProps->MinHeatSize();
		m_heatSizeAim	= m_pProps->AimHeatSize();
		m_heatSizeMax	= m_pProps->MaxHeatSize();
		
		m_strand1Start	= m_pProps->StartWidth(1);
		m_strand2Start	= m_pProps->StartWidth(2);
		m_strand1End	= m_pProps->EndWidth(1);
		m_strand2End	= m_pProps->EndWidth(2);
		
		m_spec			= m_pProps->InitSpec();
		m_minCarbon		= m_pProps->MinCarbon();
		m_maxCarbon		= m_pProps->MaxCarbon();
		m_minMn			= m_pProps->MinMn();
		m_maxMn			= m_pProps->MaxMn();

		m_dateTimeCtrl.SetDateTime(m_pProps->StartTime().GetTime());

		m_turnaround = m_pProps->TurnaroundTime().GetTotalMinutes();

	}
	else {
		m_dateTimeCtrl.SetNull();
	}

	SetChemPresets();	
	SetHeatSizeExtremes(m_caster,false);
	SetWidthSpinLimits(m_caster);

	m_editTons.EnableWindow(showLimits);
	m_editNumHeats.EnableWindow(showLimits);

	m_editStrand1Start.EnableWindow(showLimits);
	m_editStrand2Start.EnableWindow(showLimits);
	m_editStrand1End.EnableWindow(showLimits);
	m_editStrand2End.EnableWindow(showLimits);
	m_editTurnaround.EnableWindow(showLimits);

	m_dateTimeCtrl.EnableWindow(showLimits);

	m_editCaster.EnableWindow(! m_isEditing );


	SetStrand2Enabled(m_caster);

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



//
//  Set the combo box to display the available specs.
//  

void CCastStringIdDlg::InitSpecEntries()
{
	m_comboSpec.ResetContent();

	const set<CString>& specs = TheSnapshot.AvailSteelSpecs();

	for ( set<CString>::const_iterator is = specs.begin();
	      is != specs.end();
		  ++is ) 
		m_comboSpec.AddString((*is));
}



//
//  Set the combo box to display presets matching the selected spec.
//

void CCastStringIdDlg::SetChemPresets()
{
	m_comboPresets.ResetContent();

	m_selectedSpecChemRangePresets.clear();

	CSpecCmp cmp( m_spec );


	const vector<CSpecChemRangePreset>&	presets
		= TheSnapshot.SpecChemRangePresets();

	for ( vector<CSpecChemRangePreset>::const_iterator ip = presets.begin();
		  ip != presets.end();
		  ++ip ) {


		if ( (cmp)((*ip).Spec()) ) {

			m_selectedSpecChemRangePresets.push_back( (*ip) );

			CString str;
			str.Format("C: %4.2f-%4.2f%%, Mn: %4.2f-%4.2f%%", 
					   (*ip).MinC(), 
					   (*ip).MaxC(), 
					   (*ip).MinMn(),
					   (*ip).MaxMn());
			m_comboPresets.AddString( str );

		}
	}
}




//
//  Set the spin limits on the strand width boxes depending on the caster
//

void CCastStringIdDlg::SetWidthSpinLimits(int caster)
{
	// CASTER TODO: What is correct for casters 4,5?
	if ( caster == 1 )  {
		m_spinStrand1Start.SetRange(25,99);
		m_spinStrand1End.SetRange(25,99);
		m_spinStrand2Start.SetRange(25,99);
		m_spinStrand2End.SetRange(25,99);
	}
	else {
		m_spinStrand1Start.SetRange(35,99);
		m_spinStrand1End.SetRange(35,99);
		m_spinStrand2Start.SetRange(35,99);
		m_spinStrand2End.SetRange(35,99);
	}
}



//
//  Tons displayed are computed from #heats and aim heat size.
//

void CCastStringIdDlg::SetTons(int numHeats)
{
	m_tons = numHeats*m_heatSizeAim;
}



//
//  Display allowed range of heat size depending on caster#
//

void CCastStringIdDlg::SetHeatSizeExtremes(int casterNum, bool updateString)
{
	m_heatSizeMinLimit	= GlobalConstants.MinHeatSize(casterNum);
	m_heatSizeMaxLimit	= GlobalConstants.MaxHeatSize(casterNum);

	if ( updateString ) {
		// means that the caster has changed
		m_heatSizeMin = m_heatSizeMinLimit;
		m_heatSizeMax = m_heatSizeMaxLimit;
		m_heatSizeAim = GlobalConstants.AimHeatSize(casterNum);
		SetTons(m_numHeats);
	}
}


//
//  Enable controls related to strand 2 only if caster = 1,3.
//

void CCastStringIdDlg::SetStrand2Enabled(int casterNum)
{	
	bool enabled = m_pProps != 0 && Caster::NumStrandsOnCaster(casterNum) == 2;

	m_editStrand2Start.EnableWindow(enabled);
	m_editStrand2End.EnableWindow(enabled);
}


//
//  Turnaround time is caster dependent
//

void CCastStringIdDlg::SetTurnaroundTime(int casterNum)
{
	// only called when caster has changed, so we need to reset to defaults.

	m_turnaround = GlobalConstants.TurnaroundTime(casterNum).GetTotalMinutes();
}



//
//  Compute and display an initial id.
//  The initial id is computed from existing ids.
//

void CCastStringIdDlg::SetInitialId(int casterNum)
{

	if ( m_idChanged )
		return;

	// Kind of klunky.  Maybe we should allow changes to CCastStringIds,
	// and just prevent changes to the CCastString's id by const&.


	int year, week, stringId;

	CTime now = CTime::GetCurrentTime();
	CWeek currHrYrWk(now.GetTime()+7*24*3600);

	
	if ( m_usedIds[casterNum].size() > 0 ) {
		
		// For initial value, use the last string id, +1
		//   if the week is current, 
		//   else, string #1 of current week

		CCastStringId& lastId = m_usedIds[casterNum].back();

		CWeek lastWeek(lastId.Year(),lastId.Week());
	
		if ( lastWeek < currHrYrWk ) {
			year = currHrYrWk.Year();
			week = currHrYrWk.Week();
			stringId = 1;
		}
		else {
			year = lastId.Year();
			week = lastId.Week();
			stringId = lastId.StringId()+1;
		}
	}
	else {

		// Generate the initial value.
	
		year = currHrYrWk.Year();
		week = currHrYrWk.Week();
		stringId = 1;
	}


	m_year		= year;
	m_week		= week;
	m_num		= stringId;
	m_caster	= casterNum;

}


//
//  Validate input
//  If okay, propagate changes back to id and miscProps
//

void CCastStringIdDlg::OnOK() 
{
	UpdateData(true);

	CCastStringId testId(m_year,m_week,m_caster,m_num);

	if ( find(m_usedIds[m_caster].begin(),
			  m_usedIds[m_caster].end(),
			  testId) != m_usedIds[m_caster].end() ) {
		MessageBox("That id is already in use!\nTry again.");
		return;
	}

	// caster 1 has a different test.
	// removed on 2002-06-23 per Tom Fronczak.
#if 0
	if ( m_caster == 1 ) {
		for ( vector<CCastStringId>::iterator ii = m_usedIds[m_caster].begin();
			  ii != m_usedIds[m_caster].end();
			  ++ii ) {
			if ( (*ii).StringId() == m_num ) {
				ostrstream ostr;
				ostr << "That id is in use by " << m_week << "-" << m_num
					 << ends;
				MessageBox(ostr.str());
				ostr.freeze(false);
				return;
			}
		}
	}
#endif
	

	if (m_heatSizeMin < m_heatSizeMinLimit
		||
		m_heatSizeAim < m_heatSizeMin 
		|| 
		m_heatSizeAim > m_heatSizeMax
		||
		m_heatSizeMax > m_heatSizeMaxLimit ) {
		MessageBox("Heat size: must have limit <= min <= aim <= max <= limit.");
		return;
	}

//	if ( m_spec != "" 
//		 &&
//		 TheSnapshot.SpecMgr().FindSpec(m_spec,m_caster) == 0 ) {
//		MessageBox("You must enter a valid spec!");
//		return;
//	}

	if ( m_pProps != 0 ) {
		if ( m_minCarbon > m_maxCarbon 
			 ||
			 m_minMn > m_maxMn ) {
			MessageBox("Chemistries:  Min should be <= Max!");
			return;
		}
	}


	*m_pId = testId;	// copy

	if ( m_pProps != 0 ) {
		m_pProps->SetNumHeats(m_numHeats);
		m_pProps->SetAimHeatSize(m_heatSizeAim);
		m_pProps->SetStartWidth(1,m_strand1Start);
		m_pProps->SetStartWidth(2,m_strand2Start);
		m_pProps->SetEndWidth(1,m_strand1End);
		m_pProps->SetEndWidth(2,m_strand2End);
		m_pProps->SetInitSpec(m_spec,m_caster);
		m_pProps->SetMinCarbon(m_minCarbon);
		m_pProps->SetMaxCarbon(m_maxCarbon);
		m_pProps->SetMinMn(m_minMn);
		m_pProps->SetMaxMn(m_maxMn);
		SYSTEMTIME stime;
		m_dateTimeCtrl.GetDateTime().GetAsSystemTime(stime);
		m_pProps->SetStartTime( CTime(stime) );

		// per Carey, new strings go to the hold area
		if ( ! m_isEditing )
			m_pProps->SetIsStartTimeValid(false);

		m_pProps->SetTurnaroundTime( CTimeSpan(0,0,m_turnaround,0));
	}
	
	CDialog::OnOK();
}




//////////////////////////////////////////////////////////
//
//  Responses to user input
//
////////////////////////////////////////////////////////


//
//  tons is dependent on number of heats
//

void CCastStringIdDlg::OnKillfocusEditNumHeats() 
{
	if ( UpdateData(true) ) {

		SetTons(m_numHeats);
		UpdateData(false);

	}
}


void CCastStringIdDlg::OnDeltaposSpinNumHeats(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// we are about to change

;
	int newNumHeats = pNMUpDown->iPos + pNMUpDown->iDelta;

	UpdateData(true);
	SetTons(newNumHeats);
	UpdateData(false);
	
	*pResult = 0;
}



//
// Many values are dependent on the caster#
//

void CCastStringIdDlg::OnDeltaposSpinCaster(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int newCaster = pNMUpDown->iPos + pNMUpDown->iDelta;

	if ( newCaster < 1 ) 
		newCaster = 3;
	else if ( newCaster > 3 )
		newCaster = 1;

	UpdateData(true);
	SetInitialId(newCaster);
	SetHeatSizeExtremes(newCaster,true); 
	SetStrand2Enabled(newCaster);
	SetWidthSpinLimits(newCaster);
	SetTurnaroundTime(newCaster);

	UpdateData(false);
	
	*pResult = 0;
}


void CCastStringIdDlg::OnKillfocusEditCaster() 
{
	int oldCaster = m_caster;
	if ( UpdateData(true) 
		 &&
		 oldCaster != m_caster ) {
		SetInitialId(m_caster);
		SetHeatSizeExtremes(m_caster,true);
		SetStrand2Enabled(m_caster);
		SetWidthSpinLimits(m_caster);
		SetTurnaroundTime(m_caster);
		UpdateData(false);
	}
}



//
//  Tons are dependent on aim heat size
//

void CCastStringIdDlg::OnKillfocusEditHeatSizeAim() 
{
	if ( UpdateData(true) ) {

		SetTons(m_numHeats);
		UpdateData(false);

	}	
}



//
//  Note changes to the id
//  The only place this is used is when we change caster 
//     (obviously we are in create mode in this case).
//  If the user has made a change to the id, then we don't recompute
//     the initial id.
//


void CCastStringIdDlg::OnChangeEditNum() 
{
	m_idChanged = true;
	
}

void CCastStringIdDlg::OnChangeEditWeek() 
{
	m_idChanged = true;

}

void CCastStringIdDlg::OnChangeEditYear() 
{
	m_idChanged = true;

}

void CCastStringIdDlg::OnDeltaposSpinNum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	m_idChanged = true;

	*pResult = 0;
}

void CCastStringIdDlg::OnDeltaposSpinWeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	m_idChanged = true;
	
	*pResult = 0;
}

void CCastStringIdDlg::OnDeltaposSpinYear(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	m_idChanged = true;

	*pResult = 0;
}



//
// Change to preset chem range. 
// update the displayed chem ranges.
//

void CCastStringIdDlg::OnSelchangeComboPresets() 
{
	UpdateData(TRUE);

	int index = m_comboPresets.GetCurSel();

	if ( index == CB_ERR )
		return;

	const CSpecChemRangePreset& x = m_selectedSpecChemRangePresets[index];
	m_minCarbon = x.MinC();
	m_maxCarbon = x.MaxC();
	m_minMn     = x.MinMn();
	m_maxMn     = x.MaxMn();

	UpdateData(FALSE);
}


//
//  If spec changes, the list of presets changes.
//

void CCastStringIdDlg::OnSelchangeComboSpec() 
{
	UpdateData(TRUE);
	SetChemPresets();
	
}

void CCastStringIdDlg::OnEditchangeComboSpec() 
{
	UpdateData(TRUE);
	SetChemPresets();
}

