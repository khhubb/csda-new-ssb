// GanttColorChartDlg.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "GanttColorChartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
#include "OrderColor.h"


/////////////////////////////////////////////////////////////////////////////
//
//  CGanttColorChartDlg dialog
//
//  Displays a chart showing the colors used on the gantt chart display 
//    of the cast string editor.
//
//  Colors are based on the 4-digit slab-condition-code
//  We use the first and second most-significant digits.
//
//  The first digit corresponds to some category of disposition, 
//    such as CMS, to dock, etc.
//
//  The display is organized as a rectangular grid of colors,
//    with strings on the left giving the category name.
//
//
//  Usage:
//    
//      CGanttColorChartDlg dlg;
//		dlg.DoModal();
//
//
//////////////////////////////////////////////////////////////////////////////
//     
//
/////////////////////////////////////////////////////////////////////////////



CGanttColorChartDlg::CGanttColorChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGanttColorChartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGanttColorChartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGanttColorChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGanttColorChartDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGanttColorChartDlg, CDialog)
	//{{AFX_MSG_MAP(CGanttColorChartDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttColorChartDlg message handlers



vector<CString> CGanttColorChartDlg::sm_strings;


void CGanttColorChartDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CSize sqSize(m_stringSize.cy,m_stringSize.cy);

	for ( int i=0; i<10; ++i ) {

		int yCoord	= m_topY + i*(m_margin+m_stringSize.cy);

		CRect rect(CPoint(0,yCoord),m_stringSize);

		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(sm_strings[i],
					rect,
					DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

		for ( int j=0; j<10; ++j ) {
			int xCoord = m_leftX + j*(m_margin+sqSize.cx);

			CRect cRect(CPoint(xCoord,yCoord),sqSize);
			dc.FillSolidRect(cRect,COrderColor::GetColor(i,j));
		}
	}
}



BOOL CGanttColorChartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Determine location of OK button

	CWnd* pOkButton = GetDlgItem(IDOK);

	assert( pOkButton != 0 );

	CRect posn;
	pOkButton->GetWindowRect(posn);
	ScreenToClient(posn);

	// Calculate initial point for drawing color grid

	m_topY = posn.BottomRight().y + 10;
	m_margin = 5;

	if ( sm_strings.size() == 0 ) {
		sm_strings.push_back(            "(0)");	// 0
		sm_strings.push_back(    "to dock (1)");	// 1
		sm_strings.push_back(            "(2)");	// 2
		sm_strings.push_back(            "(3)");	// 3
		sm_strings.push_back("around dock (4)");	// 4
		sm_strings.push_back(        "CMS (5)");	// 5
		sm_strings.push_back(            "(6)");	// 6
		sm_strings.push_back(            "(7)");	// 7
		sm_strings.push_back(   "CMS/flip (8)");	// 8
		sm_strings.push_back(       "flip (9)");	// 9
	}

	CClientDC dc(this);

	m_stringSize.cx = 0;
	m_stringSize.cy = 0;

	for ( vector<CString>::iterator is = sm_strings.begin();
		  is != sm_strings.end();
		  ++is ) {
		
		CSize size = dc.GetTextExtent((*is));
	
		m_stringSize.cx = max(size.cx,m_stringSize.cx);
		m_stringSize.cy = max(size.cy,m_stringSize.cy);
	}

	m_stringSize += CSize(10,0);

	m_leftX	= m_stringSize.cx + m_margin;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CGanttColorChartDlg::OnOK() 
{
	CDialog::OnOK();
}
