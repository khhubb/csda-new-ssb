// SpecSelectionDlg.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "SpecSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Snapshot.h"
#include "spec.h"
#include "SpecMgr.h"


/////////////////////////////////////////////////////////////////////////////
//	CSpecSelectionDlg dialog
//
//  
//
//	Usage:
//
//		CSpecSelectionDlg dlg( <CWnd* parent> );
//
//		dlg.m_caster = caster;
//		dlg.m_specs = <a set of spec names, those on the cross-app list>
//		dlg.m_orderedSpecs = <a set of spec names, those actually on the orders>
//		dlg.m_caption = <CString, descriptive label>
//		dlg.SetInitialPos();  // call this only if dialog is be positioned at lower right corner of parent
//		dlg.m_selectedSpec = <spec name>; // set if some spec is the default choice
//
//		switch ( dlg.DoModal() ) {
//		case IDOK:
//			// user picked a spec
//			// dlg.m_selectedSpec is it.
//			break;
//		case IDC_BUTTON_SET_ALL:
//			// user wants this spec used for all subsequent heats
//			// dlg.m_selectedSpec; is it.
//			break;
//		default:
//			// user cancelled.
//			break;
//		}
//
///////////////////////////////////////////////////////////////////////////////



CSpecSelectionDlg::CSpecSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpecSelectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecSelectionDlg)
	m_caption = _T("");
	m_selectedSpec = _T("");
	//}}AFX_DATA_INIT

	m_bClientPos = false;

}


void CSpecSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecSelectionDlg)
	DDX_Control(pDX, IDC_LIST_CROSSAPP_SPECS, m_listCrossAppSpecs);
	DDX_Control(pDX, IDC_LIST_ORDERED_SPECS, m_listOrderedSpecs);
	DDX_Text(pDX, IDC_STATIC_CAPTION, m_caption);
	DDX_Text(pDX, IDC_EDIT_SPEC, m_selectedSpec);
	DDV_MaxChars(pDX, m_selectedSpec, 7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecSelectionDlg)
	ON_LBN_SELCHANGE(IDC_LIST_CROSSAPP_SPECS, OnSelchangeListCrossappSpecs)
	ON_LBN_SELCHANGE(IDC_LIST_ORDERED_SPECS, OnSelchangeListOrderedSpecs)
	ON_BN_CLICKED(IDC_BUTTON_SET_ALL, OnButtonSetAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecSelectionDlg message handlers



BOOL CSpecSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( m_caster == 1 || m_caster == 2 || m_caster == 3 );

	for ( set<CString>::iterator is = m_specs.begin();
		  is != m_specs.end();
		  ++is ) 
		m_listCrossAppSpecs.AddString((*is));

	for ( set<CString>::iterator iv = m_orderedSpecs.begin();
		  iv != m_orderedSpecs.end();
		  ++iv )
		m_listOrderedSpecs.AddString( (*iv) );


	if ( m_bClientPos ) {

		// this is a very specialized function.
		// we want this dialog to appear at the lower left corner
		//   of its parent.

		CRect parentRect;
		GetParent()->GetWindowRect(&parentRect);

		CRect dlgRect;
		GetWindowRect(&dlgRect);

		CPoint parentBR = parentRect.BottomRight();
		CPoint clientTL = parentBR - dlgRect.Size();;

		SetWindowPos(&wndTop,
					 clientTL.x,clientTL.y,
					 0,0,
					 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSpecSelectionDlg::SetInitialPos()
{
	m_bClientPos = true;
}


void CSpecSelectionDlg::OnOK() 
{
	UpdateData(true);

	if ( m_selectedSpec.GetLength() != 7 ) {
		MessageBox("You must enter a 7-digit spec.");
		return;
	}

	if ( TheSnapshot.SpecMgr().FindSpecMaybe(m_selectedSpec,m_caster) == 0 ) {
		MessageBox("That is not a valid spec.");
		return;
	}

	
	CDialog::OnOK();
}




void CSpecSelectionDlg::OnSelchangeListCrossappSpecs() 
{
	SetSpecFromList(m_listCrossAppSpecs);	
}

void CSpecSelectionDlg::OnSelchangeListOrderedSpecs() 
{
	SetSpecFromList(m_listOrderedSpecs);
}

void CSpecSelectionDlg::SetSpecFromList(CListBox& lb)
{
	int index = lb.GetCurSel();

	if ( index == LB_ERR )
		return;

	lb.GetText(index,m_selectedSpec);
	UpdateData(FALSE);
}
void CSpecSelectionDlg::OnButtonSetAll() 
{
	EndDialog(IDC_BUTTON_SET_ALL);	
}
