// ScenMgrMvc.cpp: implementation of the CScenMgrViewport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "ScenMgrMvc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "MessageBus.h"
#include "CasterScenArchiver.h"



#include "Snapshot.h"
#include "CasterScen.h"
#include "ProdnScen.h"
#include "SuperScen.h"
#include "ScenMgr.h"
#include "UserInfo.h"
#include "NOutputWnd.h"


//////////////////////////////////////////////////////////////////////
//
//  CScenMgrViewport
//
//  Manages display for CScenMgrView
//
//
//  Primarily, one big SECTreeCtrl listing available scenarios.
//  Plus a few buttons for loading, deleting, etc.
//
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////
//
//  Standard viewport stuff
//
//////////////////////////////////////////////

void CScenMgrViewport::OnInitialUpdate()
{
	// the view needs to set these before we get here
	assert( m_pTreeCtrl   != 0 );

	// set up the tree control
	m_pTreeCtrl->EnableHeaderCtrl(true);
	m_pTreeCtrl->EnableMultiSelect(true);
	m_pTreeCtrl->ModifyTreeCtrlStyles(0,
									 TVS_HASBUTTONS 
									 | TVS_HASLINES 
									 | TVS_LINESATROOT,
									 0,
									 LVXS_LINESBETWEENCOLUMNS
									 | LVXS_FITCOLUMNSONSIZE
									 | LVXS_LINESBETWEENITEMS
									 | LVXS_HILIGHTSUBITEMS);

	SetScenTreeHeader();


	UpdateScenTree();

	m_pTreeCtrl->ReMeasureAllItems();
	m_pTreeCtrl->Invalidate();
}




CMessageBus* CScenMgrViewport::GetMyModel()
{
	return dynamic_cast<CMessageBus*>(GetModel());
}


BOOL CScenMgrViewport::CreateController()
{
	m_pCtlr = new CScenMgrController;
	m_bAutoDelCtlr = true;
	m_pCtlr->Create(m_pWnd,this);

	return TRUE;
}


BOOL CScenMgrViewport::OnCreate()
{
	// do nothing
	return TRUE;
}


//////////////////////////////////////////////////////////
//
//  Displaying the tree
//
//////////////////////////////////////////////////////////




void CScenMgrViewport::SetScenTreeHeader()
{
	CHeaderInfo info;

	m_allHeaders.resize(NUM_LHS);

	// in allHeaders, each is in order by index.
	
	{
		int index = 0;
		for ( vector<CHeaderInfo>::iterator ih = m_allHeaders.begin();
			  ih != m_allHeaders.end();	
			  ++ih, ++index )
				  (*ih).m_type = E_LIST_HEADER(index);
	}

	m_allHeaders[ LH_VERSION	].m_string = "Version";
	m_allHeaders[ LH_LOADED		].m_string = "Loaded?";
	m_allHeaders[ LH_CREATED	].m_string = "Created";
	m_allHeaders[ LH_SAVED		].m_string = "Last saved";
	m_allHeaders[ LH_COMMENT	].m_string = "Comment";

	m_allHeaders[ LH_CREATED	].m_item = "XX:XX:XX XX-XXX-XXXX";
	m_allHeaders[ LH_SAVED		].m_item = "XX:XX:XX XX-XXX-XXXX";
	m_allHeaders[ LH_COMMENT	].m_item = "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

	CClientDC dc(m_pTreeCtrl);
	THeaderInfoAux::SetWidths<CHeaderInfo>(dc,m_allHeaders);
	THeaderInfoAux::SetHeader(m_pTreeCtrl,m_allHeaders);
	m_pTreeCtrl->SetColumnWidth(0,150);


//	m_pTreeCtrl->InsertColumn( 1, "Version",		LVCFMT_RIGHT,  60, 1 );
//	m_pTreeCtrl->InsertColumn( 2, "Loaded?",		LVCFMT_CENTER, 30, 2 );
//	m_pTreeCtrl->InsertColumn( 3, "Created",		LVCFMT_RIGHT,  90, 3 );
//	m_pTreeCtrl->InsertColumn( 4, "Last saved",	LVCFMT_RIGHT,  90, 4 );
//	m_pTreeCtrl->InsertColumn( 5, "Comment",		LVCFMT_LEFT,  120, 5 );
}


//
//  The tree control displays the available scenarios
//    hierarchically
//    
//    Caster1
//      userA
//         scen1Aa
//         scen1Ab
//      userB
//         scen1Ba
//         scen1Bb
//    Caster2
//      ...
//    Caster3
//      ...
//
//  Nodes in the tree can be caster nodes, user nodes or scenario nodes.
//  An item data is set for each node:
//     caster node   = -1
//     owner node    = -2
//     scenario node = index of scenario in m_ids
//
//  The tree is virtual, so there is a callback to compute the strings
//    to be displayed.
//

#define ostreset() ostr.freeze(false);ostr.seekp(0)

void CScenMgrViewport::UpdateScenTree()
{
	m_pTreeCtrl->DeleteAllItems();

	CCasterScenArchiver::GetAvailScens(m_ids,true);

	// ids will be sorted by caster/owner/id
	// we iterate through in sequence,
	//    establishing new parent nodes at each caster and owner.

	ostrstream ostr;
	CString text;

	int lastCaster		= 0;
	CString lastOwner	= "|||||";
	HTREEITEM hcaster;
	HTREEITEM howner;
	DWORD count = 0;

	for ( vector<CAvailCasterScenId>::iterator is = m_ids.begin();
		  is != m_ids.end();
		  ++is, ++count ) {

		if ( (*is).Caster() != lastCaster ) {
			
			ostr << "Caster # " << (*is).Caster() << ends;
			hcaster = m_pTreeCtrl->InsertItem(ostr.str());
			ostreset();

			m_pTreeCtrl->SetItemData(hcaster,-1);

			lastCaster = (*is).Caster();
			lastOwner = "|||||";
		}

		if ( (*is).Owner() != lastOwner ) {

			howner = m_pTreeCtrl->InsertItem((*is).Owner(),hcaster);
			m_pTreeCtrl->SetItemData(howner,-2);

			lastOwner = (*is).Owner();
		}

		ostr << "#" << (*is).Id() << ends;
		HTREEITEM hitem = m_pTreeCtrl->InsertItem( ostr.str(),howner );
		ostreset();

		m_pTreeCtrl->SetItemData(hitem,count);
	}

	m_pTreeCtrl->Invalidate();
}


//
//  Called by CScenMgrController's callback to compute
//    strings for the tree to display
//

void CScenMgrViewport::OnScenTreeGetListDispInfo(NMHDR* pNMHDR)
{

	LV_DISPINFO* lvdi = (LV_DISPINFO*) pNMHDR;

	int item    = lvdi->item.iItem;
	int subItem = lvdi->item.iSubItem;

	if ( subItem < 1 || subItem > 5 )
		return;

	HTREEITEM hitem = m_pTreeCtrl->GetItemAt(item);

	DWORD datum = m_pTreeCtrl->GetItemData(hitem);

	CString strText;

	// datum out of range means it is a caster or owner item
	if ( 0 <= datum && datum < m_ids.size() ) {

		CAvailCasterScenId& id = m_ids[datum];
	
	
		switch ( subItem  ) {
		case LH_VERSION:
			strText.Format("%d",id.Version());
			break;		

		case LH_LOADED:
			strText = ( id.IsLoaded() ? "Yes" : "No" );
			break;
	
		case LH_CREATED:
			strText = id.CreateTime().Format("%H:%M %x");
			break;
	
		case LH_SAVED:
			strText = id.LastSaveTime().Format("%H:%M %x");
			break;
	
		case LH_COMMENT:
			strText = id.Comment();
			break;
		}
	}

	strncpy(lvdi->item.pszText,strText,lvdi->item.cchTextMax);
	lvdi->item.mask |= LVIF_DI_SETITEM;
}


//////////////////////////////////////////////////////////
//
//  Actions
//
//////////////////////////////////////////////////////////


//
//  Delete selected scenarios from the database
//

void CScenMgrViewport::OnButtonDelete() 
{
	// find the scenarios to be deleted.
	vector<int> selectedScenIndexes;
	GetSelectedScenIndexes(selectedScenIndexes);

	//  get out if none selected
	if ( selectedScenIndexes.size() == 0 ) {
		GetWnd()->MessageBox("You must select on or more scenarios to delete.",
							 "No scenarios selected.",
							  MB_ICONWARNING|MB_OK);
		return;
	}


	// verify with the user

	ostrstream ostr;

	ostr << "Do you want to delete the following: \n";

	{
		for ( vector<int>::iterator ii = selectedScenIndexes.begin();
			  ii != selectedScenIndexes.end();
			  ++ii ) {
		
			CAvailCasterScenId& id = m_ids[(*ii)];
			
			
			ostr << "Caster #" << id.Caster()
				 << ", Scen# " << setw(4) << id.Id()
				 << ", Owner = " << LPCTSTR(id.Owner())
				 << ", Created " << LPCTSTR(id.CreateTime().Format("%H:%M %x"))
				 << ", Saved " << LPCTSTR(id.LastSaveTime().Format("%H:%M %x"))
				 << "\n";
		}
	}
	
	ostr << ends;

	int result = GetWnd()->MessageBox(ostr.str(),
							"Confirm deletion",
							MB_ICONQUESTION|MB_YESNO);

	ostreset();

	if ( result != IDYES )
		return;

	NOutputWnd::ShowMainCheckpointWnd(true);
	NOutputWnd::ClearText(COutputControlBar::OCB_CHECKPOINT);
	NOutputWnd::AddText(COutputControlBar::OCB_CHECKPOINT,
						"Beginning deletions...\n");

	// Do the deletions
	{
		ostrstream ostr;

		for ( vector<int>::iterator ii = selectedScenIndexes.begin();
			  ii != selectedScenIndexes.end();
			  ++ii ) {		
				  
			CAvailCasterScenId& id = m_ids[(*ii)];

			ostr << "Deleting caster scen, caster#" << id.Caster()
				 << ", Scen# " << setw(4) << id.Id()
				 << ", Owner = " << LPCTSTR(id.Owner())
				 << ", Created " << LPCTSTR(id.CreateTime().Format("%H:%M %x"))
				 << ", Saved " << LPCTSTR(id.LastSaveTime().Format("%H:%M %x"))
				 << "..."
				 << ends;

			NOutputWnd::AddText(COutputControlBar::OCB_CHECKPOINT,ostr);
			ostr.freeze(false);
			ostr.seekp(0);


			if ( id.IsLoaded() ) {
	
				NOutputWnd::AddText(COutputControlBar::OCB_CHECKPOINT," (loaded) ");
				CCasterScen* pScen = CScenMgr::CasterScenById(id.Caster(),id.Id());
				assert( pScen != 0 );
	
				CScenMgr::DeleteCasterScen(pScen);
			}
	
			CCasterScenArchiver::DeleteCasterScen(id.Id());
			NOutputWnd::AddText(COutputControlBar::OCB_CHECKPOINT,"done.\n");

		}
	}

	UpdateScenTree();
	NOutputWnd::AddText(COutputControlBar::OCB_CHECKPOINT,"\nDeletions completed.");

}



//
//  Load the selected scenarios
//

void CScenMgrViewport::OnButtonLoad() 
{
	// Determine the selected scenarios
	vector<int> selectedScenIndexes;
	GetSelectedScenIndexes(selectedScenIndexes);

	// if there are none, get out
	if ( selectedScenIndexes.size() == 0 ) {
		GetWnd()->MessageBox("You must select on or more scenarios to load.",
				   "No scenarios selected.",
				   MB_ICONWARNING|MB_OK);
		return;
	}


	// verify with the user

	bool someLoaded = false;
	bool otherOwner = false;
	ostrstream ostr;

	ostr << "Do you want to load the following: \n";

	{
		for ( vector<int>::iterator ii = selectedScenIndexes.begin();
			  ii != selectedScenIndexes.end();
			  ++ii ) {
			
			CAvailCasterScenId& id = m_ids[(*ii)];
			
			ostr << "Caster #" << id.Caster()
				 << ", Scen# " << setw(4) << id.Id()
				 << ", Owner = " << LPCTSTR(id.Owner())
				 << (id.IsLoaded() ? ", already loaded" : "")
				 << "\n";
	
			if ( id.IsLoaded() )
				someLoaded = true;

			if ( id.Owner() != CUserInfo::TheUser.LoginName() )
				otherOwner = true;
		}
	}
	
	if ( someLoaded ) 
		ostr << "\n[Note: any scenario already loaded will be loaded with a new id.]";

	if ( otherOwner )
		 ostr << "\n[Note: any scenario belonging to another user will be loaded with a new id.]";


	ostr << ends;


	int result = GetWnd()->MessageBox(ostr.str(),
							   "Confirm load",
							   MB_ICONQUESTION|MB_YESNO);

	ostreset();

	if ( result != IDYES )
		return;


	// Do the loading.
	// Record the ids of scenarios which load with ids other than 
	//   what they were stored with (due to already loaded or different owner).

	vector<long> oldIds;
	vector<long> newIds;

	{
		CWaitCursor wait;

		for ( vector<int>::iterator ii = selectedScenIndexes.begin();
			  ii != selectedScenIndexes.end();
			  ++ii ) {
	
			CAvailCasterScenId& id = m_ids[(*ii)];
	
			long loadId = CCasterScenArchiver::LoadCasterScen(id.Id());
			  
			if ( loadId != id.Id() ) {
	
				oldIds.push_back(id.Id());
				newIds.push_back(loadId);
				
			}
		
		}
	}

	// inform the user of completion, 

	if ( oldIds.size() == 0 ) {
		GetWnd()->MessageBox("Scenario loaded.");
		return;
	}


	ostr << "The following id substitutions have occurred: \n";

	for ( int i=0; i<oldIds.size(); ++i ) 
		ostr << "Scen # " << setw(4) << oldIds[i]
			 << " ==> " << setw(4) << newIds[i]
			 << "\n";

	ostr << ends;

	GetWnd()->MessageBox(ostr.str());

	ostreset();


	// of course
	UpdateScenTree();
}


//
// the database can be updated by other users.
// we can request an updated list
//

void CScenMgrViewport::OnButtonRefresh() 
{
	UpdateScenTree();	
}



/////////////////////////////////////////////////////////////
//
//  Implementation details
//
/////////////////////////////////////////////////////////////


//
//  Get the indexes of the selected scenarios.
//  Indexes are stored as item data on the tree items
//

void CScenMgrViewport::GetSelectedScenIndexes(vector<int>& indexes)
{
	for ( HTREEITEM hItem = m_pTreeCtrl->GetFirstSelectedItem();
		  hItem != 0;
		  hItem = m_pTreeCtrl->GetNextSelectedItem(hItem) ) {

		DWORD datum = m_pTreeCtrl->GetItemData(hItem);

		if ( 0 <= datum && datum < m_ids.size() )
			indexes.push_back(datum);
	}
}


#if 0
// not needed.
// Keeping this around so I remember how to do it.

void CScenMgrView::ShowContextMenu(CMenu* pMenu)
{
	
	CPoint point(GetCurrentMessage()->pt);  // screen coordinates.

	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
						  point.x,
						  point.y,
						  this);

}

#endif

//////////////////////////////////////////////////////////
//
//  OnUpdate
//
//////////////////////////////////////////////////////////


void CScenMgrViewport::OnUpdate(IMvcSubject* pSubject, IMvcMessage *pMsg)
{
	switch ( pMsg->GetTypeID() ) {

	// any change in scenarios which exist or in our selection

	case MSG_ID_SMGR_CREATE_CASTER_SCEN:
	case MSG_ID_SMGR_DELETE_CASTER_SCEN:
	case MSG_ID_SMGR_SAVE_CASTER_SCEN:
	case MSG_ID_SMGR_CASTER_SCEN_INFO_CHANGED:
	case MSG_ID_SMGR_CREATE_PRODN_SCEN:
	case MSG_ID_SMGR_DELETE_PRODN_SCEN:
	case MSG_ID_SMGR_SAVE_PRODN_SCEN:
	case MSG_ID_SMGR_PRODN_SCEN_INFO_CHANGED:
	case MSG_ID_SMGR_SNAPSHOT_LOADED:
		UpdateScenTree();
		break;

	default:
		// do nothing
		break;
	}
}


 

//////////////////////////////////////////////////////////////////////
//
//  CScenMgrController
//
//  Handles events display for CScenMgrView/Viewport
//
//////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CScenMgrController, MvcController)
	//{{AFX_MSG_MAP(CScenMgrController)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO,IDC_TREE_SCENS,OnScenTreeGetListDispInfo)
END_MESSAGE_MAP()


CMessageBus* CScenMgrController::GetMyModel() 
{	
	return dynamic_cast<CMessageBus*>(m_pModel); 
}


//
// Delegate tree item callbacks to the viewport
//

void CScenMgrController::OnScenTreeGetListDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	GetViewport()->OnScenTreeGetListDispInfo(pNMHDR);
}


//
// Delegate actions to there, too.
//

void CScenMgrController::OnButtonDelete()
{
	GetViewport()->OnButtonDelete();
}

void CScenMgrController::OnButtonLoad()
{
	GetViewport()->OnButtonLoad();
}

void CScenMgrController::OnButtonRefresh()
{
	GetViewport()->OnButtonRefresh();
}
