// MultiSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "MultiSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
//	CMultiSelectDlg dialog
//
//	Do a multiple selection on a list of strings.
//
//	Usage:
//
//		CMultiSelectDlg dlg;
//		dlg.m_strings = <some set of strings to display>;
//		dlg.m_title = <some CString to use as a title>;
//		dlg.m_posns = <give the indexes of items to select initially>;
//	
//		if ( dlg.DoModal() == IDOK ) {
//			// dlg.m_posns now has the (sorted) set of indexes of selected strings
//		}
//
//
/////////////////////////////////////////////////////////////////////////////


CMultiSelectDlg::CMultiSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMultiSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSelectDlg)
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	//}}AFX_DATA_MAP

	if ( pDX->m_bSaveAndValidate ) {

		// Moving controls to variables
		// Compute the selected positions and put in m_posns

		int count = m_listBox.GetCount();
		m_posns.clear();
		for ( int i=0; i<count; ++i ) 
			if ( m_listBox.GetCheck(i) == 1 )
				m_posns.push_back(i);
	}
	else {
		//  Handle initialization of the list box of strings

		for ( vector<CString>::iterator is = m_strings.begin();
			  is != m_strings.end();
			  ++is )
			m_listBox.AddString( LPCTSTR( (*is) ) );
		
		//  Select the strings that should be selected at the beginning (m_posns)

		for ( vector<int>::iterator ii = m_posns.begin();
			  ii != m_posns.end();
			  ++ii ) 
			m_listBox.SetCheck((*ii),1);
	}
}


BEGIN_MESSAGE_MAP(CMultiSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiSelectDlg)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL, OnButtonDeselectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_HILITED, OnButtonSelectHilited)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSelectDlg message handlers


BOOL CMultiSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// initialization of list taken care of in DDX, above.
	
	if ( m_title.GetLength() != 0 )
		SetWindowText( m_title );

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//
// Special button: deselect all items
//    Set up the list box checks
//
 
void CMultiSelectDlg::OnButtonDeselectAll() 
{
	int count = m_listBox.GetCount();
	for ( int i=0; i<count; ++i ) 
		m_listBox.SetCheck(i,0);
}


//
// Special button: select all items
//    Set up the list box checks
//

void CMultiSelectDlg::OnButtonSelectAll() 
{
	int count = m_listBox.GetCount();
	for ( int i=0; i<count; ++i ) 
		m_listBox.SetCheck(i,1);
}



//
//  We are using a CCheckListBox.
//  It can be a pain to laboriously click the check box
//    of a lot of items.
//  Easier: the user highlights a bunch.
//  This will cause the highlited ones to get checked.
//

void CMultiSelectDlg::OnButtonSelectHilited() 
{
	int count = m_listBox.GetSelCount();
	int* posns = new int[count];

	m_listBox.GetSelItems(count,posns);

	for ( int i=0; i<count; ++i )
		m_listBox.SetCheck( posns[i], 1 );

	delete[] posns;
}

