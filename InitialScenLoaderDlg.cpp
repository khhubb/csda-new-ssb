// InitialScenLoaderDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "InitialScenLoaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CasterScenArchiver.h"
#include "UserInfo.h"


/////////////////////////////////////////////////////////////////////////////
//  CInitialScenLoaderDlg dialog
//
//  THis dialog is used as the last step during system boot-up.
//
//  Allows the user to select various saved scenarios to restore.
//
//
//
//	Usage:
//
//		CInitialScenLoaderDlg dlg;
//		dlg.DoModal();
//
//
//////////////////////////////////////////////////////////////////////////////



CInitialScenLoaderDlg::CInitialScenLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitialScenLoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitialScenLoaderDlg)
	m_showCategory = -1;
	//}}AFX_DATA_INIT
}


void CInitialScenLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitialScenLoaderDlg)
	DDX_Control(pDX, IDC_LIST_CASTER_3, m_listCaster3);
	DDX_Control(pDX, IDC_LIST_CASTER_2, m_listCaster2);
	DDX_Control(pDX, IDC_LIST_CASTER_1, m_listCaster1);
	DDX_Radio(pDX, IDC_RB_MOST_RECENT_SCEN, m_showCategory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitialScenLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CInitialScenLoaderDlg)
	ON_BN_CLICKED(IDC_RB_ALL, OnChangeCategory)
	ON_BN_CLICKED(IDC_RB_MOST_RECENT_DAY, OnChangeCategory)
	ON_BN_CLICKED(IDC_RB_MOST_RECENT_SCEN, OnChangeCategory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitialScenLoaderDlg message handlers




BOOL CInitialScenLoaderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// compute the available scenarios	
	m_availScens.clear();
	CCasterScenArchiver::GetAvailScens(m_availScens,true);

	// default to showing only one scenario per caster, the most recent.
	m_showCategory = SHOW_MOST_RECENT;
	
	UpdateData(false);

	SetListHeaders();
	SetLists();

	// Make sure this is on top.
	SetForegroundWindow();
      
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//
//  We only show certain saved scenarios,
//   controlled by m_showCategory:
//
//        == SHOW_ALL:			   show all
//        == SHOW_MOST_RECENT:	   show only the most recently saved
//		  == SHOW_MOST_RECENT_DAY  show all on the same day as most recently saved.
//

void CInitialScenLoaderDlg::FilterScens()
{
	const CString& owner = CUserInfo::TheUser.LoginName();

	m_showScens[1].clear();
	m_showScens[2].clear();
	m_showScens[3].clear();

	// Find most recent, for each caster

	CTime lastTimes[4];
	CTime uninitTime(1970,1,1,0,0,0);

	lastTimes[1] = uninitTime;
	lastTimes[2] = uninitTime;
	lastTimes[3] = uninitTime;

	{
		for ( vector<CAvailCasterScenId>::iterator ia = m_availScens.begin();
			  ia != m_availScens.end();
			  ++ia ) {
	
			if ( (*ia).Owner() != owner ) 
				continue;
	
			int caster = (*ia).Caster();
			
			if ( caster < 1 || caster > 3 ) 
				continue;			

			if ( lastTimes[caster] < (*ia).LastSaveTime() )
				lastTimes[caster] = (*ia).LastSaveTime();
		}
	}


	//  find the scenario ids to display.
	
	{
		for ( vector<CAvailCasterScenId>::iterator ia = m_availScens.begin();
			  ia != m_availScens.end();
			  ++ia) {
	
			if ( (*ia).Owner() != owner ) 
				continue;
	
			int caster = (*ia).Caster();
			
			if ( caster < 1 || caster > 3 ) 
				continue;	
			
			if ( m_showCategory == SHOW_ALL
				 ||
				 ( m_showCategory == SHOW_MOST_RECENT
				   &&
				   lastTimes[caster] == (*ia).LastSaveTime() )
				 ||
				 ( m_showCategory == SHOW_MOST_RECENT_DAY
				   &&
				   lastTimes[caster].GetYear() == (*ia).LastSaveTime().GetYear()
				   &&
				   lastTimes[caster].GetMonth() == (*ia).LastSaveTime().GetMonth()
				   &&
				   lastTimes[caster].GetDay() == (*ia).LastSaveTime().GetDay() ) )

				m_showScens[caster].push_back( (*ia) );
		}
	}
}





void CInitialScenLoaderDlg::SetListHeaders()
{

	SetListHeader(m_listCaster1);
	SetListHeader(m_listCaster2);
	SetListHeader(m_listCaster3);
}


void CInitialScenLoaderDlg::SetListHeader(CListCtrl& clist)
{
	clist.InsertColumn( 0, "Id",			LVCFMT_RIGHT,  40, 0 );
	clist.InsertColumn( 1, "Last saved",	LVCFMT_RIGHT,  90, 1 );
	clist.InsertColumn( 2, "Comment",	LVCFMT_LEFT,  120, 2 );
}


void CInitialScenLoaderDlg::SetLists()
{
	FilterScens();

	SetList(m_listCaster1,1);
	SetList(m_listCaster2,2);
	SetList(m_listCaster3,3);
}


void CInitialScenLoaderDlg::SetList(CListCtrl& clist, int caster)
{
	clist.DeleteAllItems();

	int count = 0;

	for ( vector<CAvailCasterScenId>::iterator ia = m_showScens[caster].begin();
		  ia != m_showScens[caster].end();
		  ++ia, ++count ) 
		
		SetListItem(clist, count, (*ia));


	if ( count > 0 ) {
		clist.SetItemState(count-1,LVIS_SELECTED,LVIS_SELECTED);
		clist.EnsureVisible(count-1,false);
	}
}



void CInitialScenLoaderDlg::SetListItem(CListCtrl& clist, 
										int itemNum, 
										CAvailCasterScenId& id)
{
	CString strId;
	CString strSaved;

	strId.Format("%d",id.Id());

	strSaved = id.LastSaveTime().Format("%H:%M %x");

	clist.InsertItem(itemNum,strId);
	clist.SetItemText(itemNum,1,strSaved);
	clist.SetItemText(itemNum,2,id.Comment());
}



void CInitialScenLoaderDlg::OnChangeCategory() 
{
	UpdateData(true);
	SetLists();	
}


//
//  load the selected scenarios
//

void CInitialScenLoaderDlg::OnOK() 
{
	if ( ! UpdateData(true) )
		return;

	{
		CWaitCursor wait;

		LoadScens(m_listCaster1,1);
		LoadScens(m_listCaster2,2);
		LoadScens(m_listCaster3,3);
	}

	MessageBox("Done loading prior scenarios.","Done",MB_OK);

	CDialog::OnOK();
}


void CInitialScenLoaderDlg::LoadScens(CListCtrl& clist, int caster)
{
	for ( int i = clist.GetNextItem(-1,LVNI_SELECTED);
		  i != -1;
	      i = clist.GetNextItem(i,LVNI_SELECTED) ) {

		CAvailCasterScenId& id = m_showScens[caster][i];

		long loadId = CCasterScenArchiver::LoadCasterScen(id.Id());
			  
		if ( loadId != id.Id() ) {

			ostrstream ostr;
			ostr << "On caster # " << caster 
				 << ", scenario # " << id.Id()
				 << " loaded as # " << loadId
				 << ends;
			MessageBox(ostr.str());
			ostr.freeze(false);
		}
	}
}


