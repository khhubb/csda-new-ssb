// Select909sDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "Select909sDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CasterScen.h"


/////////////////////////////////////////////////////////////////////////////
// CSelect909sDlg dialog
///
//  Not currently used.
//  Don't remember exactly what this was for.
//
/////////////////////////////////////////////////////////////////////////////

CSelect909sDlg::CSelect909sDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelect909sDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelect909sDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelect909sDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelect909sDlg)
	DDX_Control(pDX, IDC_LIST_910s, m_list910s);
	DDX_Control(pDX, IDC_LIST_909s, m_list909s);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelect909sDlg, CDialog)
	//{{AFX_MSG_MAP(CSelect909sDlg)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL, OnButtonDeselectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelect909sDlg message handlers



BOOL CSelect909sDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	assert( m_pScen910 != 0 );
	assert( m_pScen909 != 0 );

	PopulateList(m_pScen910,
				 CCastString::STRING_STATUS_910,
				 m_910sVec,
				 m_list910s);
	
	PopulateList(m_pScen909,
				 CCastString::STRING_STATUS_909,
				 m_909sVec,
				 m_list909s);

	OnButtonSelectAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSelect909sDlg::PopulateList(CCasterScen* pScen,
								  CCastString::E_StringStatus status,
								  vector<int>& vec,
								  CListBox& listBox)
{
	ostrstream ostr;

	for ( vector<CCastString*>::iterator is = pScen->CastStrings().begin();
		  is != pScen->CastStrings().end();
		  ++is ) {

		if ( (*is)->Status() == status ) {
			
			vec.push_back( is- pScen->CastStrings().begin() );
			
			ostr.fill('0');
			ostr << setw(4) << (*is)->Id().Year()
				 << "-"
				 << setw(2) << (*is)->Id().Week()
				 << "-"
				 << setw(2) << (*is)->Id().StringId()
				 << ", ";
			ostr.fill(' ');
			ostr << setw(4) << (*is)->StringSummary().m_tons
				 << " tons, "
				 << setw(2) << (*is)->Heats().size()
				 << ends;

			listBox.AddString( ostr.str() );
			ostr.freeze(false);
			ostr.seekp(0);
		}
	}
}

			

void CSelect909sDlg::OnButtonDeselectAll() 
{
	m_list909s.SelItemRange(FALSE,
							0,
							m_list909s.GetCount()-1);
}



void CSelect909sDlg::OnButtonSelectAll() 
{
	m_list909s.SelItemRange(TRUE,
							0,
							m_list909s.GetCount()-1);
}



void CSelect909sDlg::OnOK() 
{
	int count = m_list909s.GetSelCount();
	int* indexes = new int [count];

	m_list909s.GetSelItems(count,indexes);

	for ( int i=0; i<count; ++ i )
		m_indexes909.push_back( m_909sVec[ indexes[ i ] ] );

	delete [] indexes;

	CDialog::OnOK();
}

