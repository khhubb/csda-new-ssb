// HMStartTimeDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "HMStartTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CastString.h"

/////////////////////////////////////////////////////////////////////////////
//  CHMStartTimeDlg dialog
//
//  This is a rather special purpose dialog.
//  Designed to be used non-modal.
//  Displays a string id and a time.
//  Used to track the start/end time of a string as it is dragged around.
//
//  Usage:
//
//		CHMStartTimeDlg dlg;
//		// note that due to the non-modal nature of the dialog,
//      //   the dialog object itself is typically used in several methods.
//      // therefore, it cannot have scope local to one of those methods.
//
//
//      //  initialize:	
//
//		dlg.m_pString = <pointer to CCastString being tracked>;
//		dlg.SetTime( <a CTime, the start Time> ,
//					 <a CTime, the finish Time> );
//      // or
//		dlg.SetTimeInvalid();
//
//		dlg.Create(CPoint(x,y),<a CWnd* to be parent>);
//
//
//      // while tracking
//
//		dlg.SetTime( <a CTime, the start Time> ,
//					 <a CTime, the finish Time> );
//      // or
//		dlg.SetTimeInvalid();
//
//		
//		// done
//
//		dlg.DestroyWindow();
//
//
//////////////////////////////////////////////////////////////////////////////


CHMStartTimeDlg::CHMStartTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHMStartTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHMStartTimeDlg)
	//}}AFX_DATA_INIT

	m_pString = 0;
}


void CHMStartTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHMStartTimeDlg)
	DDX_Control(pDX, IDC_EDIT_FINISH_TIME, m_editFinishTime);
	DDX_Control(pDX, IDC_EDIT_START_TIME, m_editStartTime);
	DDX_Control(pDX, IDC_STATIC_STRING_ID, m_staticId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHMStartTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CHMStartTimeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMStartTimeDlg message handlers

BOOL CHMStartTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	assert( m_pString != 0 );

	SetIdText();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CHMStartTimeDlg::SetTime(CTime startTime, CTime finishTime)
{
	CString str;
	
	str = startTime.Format("%H:%M, %a, %b %d, %Y");
	m_editStartTime.SetWindowText(str);

	str = finishTime.Format("%H:%M, %a, %b %d, %Y");
	m_editFinishTime.SetWindowText(str);
}


void CHMStartTimeDlg::SetIdText()
{
	CString str;
	str.Format("Editing week %02d, #%d",
				m_pString->Id().Week(),
				m_pString->Id().StringId());
	m_staticId.SetWindowText(str);
}

void CHMStartTimeDlg::SetTimeInvalid()
{	
	m_editStartTime.SetWindowText("<no time>");
	m_editFinishTime.SetWindowText("<no time>");
}


BOOL CHMStartTimeDlg::Create(const CPoint& pt, CWnd* pParentWnd) 
{
	if ( ! CDialog::Create(IDD, pParentWnd) )
		return FALSE;

	SetWindowPos(&wndTop,
				 pt.x,pt.y,
					 0,0,
					 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);


	return TRUE;
}

