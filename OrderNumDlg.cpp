// OrderNumDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "OrderNumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Snapshot.h"
#include "SpecMgr.h"


/////////////////////////////////////////////////////////////////////////////
// COrderNumDlg dialog
//
//  Okay, this is a case of sheer laziness.
//  Originally, designed to allow the user to enter a mill order# and verify
//   that it exists.
//  Later extended to allow the user to enter a spec# and verify that it exists.
//
//  Usage: (order#)
//
//		COrderNumDlg dlg;
//
//		dlg.m_type = COrderNumDlg::TYPE_ORDER_NUM;
//		dlg.m_caption = "Please enter MILL_ORDER_NUM for new order:";
//	
//		if ( dlg.DoModal() == IDOK ) {
//			// dlg.m_pOrder has a pointer to the selected COrder
//		}
//
//
//	Usage: (spec)
//
//		COrderNumDlg dlg;
//
//		dlg.m_type		= COrderNumDlg::TYPE_LOT_SPEC;
//		dlg.m_casterNum	= m_casterNum;
//		dlg.m_caption	= "Please enter lot spec for new order:";
//
//		if ( dlg.DoModal() == IDOK ) {
//			// dlg.m_spec has the spec# (a string)
//		}
//
///////////////////////////////////////////////////////////////////////////////


COrderNumDlg::COrderNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderNumDlg)
	m_orderNum = _T("");
	m_caption = _T("");
	//}}AFX_DATA_INIT

	m_type = TYPE_UNSPECIFIED;
}


void COrderNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderNumDlg)
	DDX_Text(pDX, IDC_EDIT_ORDER_NUM, m_orderNum);
	DDV_MaxChars(pDX, m_orderNum, 7);
	DDX_Text(pDX, IDC_STATIC_ORDER_NUM, m_caption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderNumDlg, CDialog)
	//{{AFX_MSG_MAP(COrderNumDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderNumDlg message handlers



void COrderNumDlg::OnOK() 
{
	
	UpdateData(TRUE);

	switch ( m_type ) {
	case TYPE_ORDER_NUM:
		{
			vector<COrder*>& orders = TheSnapshot.Orders();
		
			// FP change
			//long orderNum = atol(m_orderNum);

			m_pOrder = COrder::FindOrder(m_orderNum,orders);

			if ( m_pOrder == 0 ) {
				MessageBox("No such order.");
				return;
			}
		}
		break;
	
	case TYPE_LOT_SPEC:
		CSpec* pSpec = TheSnapshot.SpecMgr().FindSpecMaybe(m_orderNum,
															m_casterNum);
		if ( pSpec == 0 ) {
			MessageBox("No such spec.");
			return;
		}
		m_spec = m_orderNum;
		break;
	}
	
	CDialog::OnOK();
}



BOOL COrderNumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert ( m_type != TYPE_UNSPECIFIED );

	assert ( m_type != TYPE_LOT_SPEC 
		     || 
			 ( 1 <= m_casterNum && m_casterNum <= 3 ));

	m_pOrder = 0;
	m_spec	 = "";
	
	switch ( m_type ) {
	case TYPE_ORDER_NUM:
		SetWindowText("Enter order number");
		break;
	case TYPE_LOT_SPEC:
		SetWindowText("Enter spec");
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
