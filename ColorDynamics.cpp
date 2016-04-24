//////////////////////////////////////////////////////////////////////////////////////
// CColorDynamics

// MyComboBox.cpp : implementation file
//
#include "stdafx.h"

#include "ColorDynamics.h"

//#include "MyComboBox.h"
//#include "CSOrderParmDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMyComboBox

// CMyComboBox::CMyComboBox(bool bEditable)
CColorDynamics::CColorDynamics(bool bEditable)
{
   // defaults
   m_bEditable = bEditable;
   m_crFGEnabled = ENABLED_FG;
   m_crBGEnabled = ENABLED_BG;
   m_crFGDisabled = DISABLED_FG;
   m_crBGDisabled = DISABLED_BG;

   // to changes the colors dynamic
   m_pbrushEnabled = new CBrush;
   m_pbrushDisabled = new CBrush;
   m_pbrushEnabled->CreateSolidBrush(m_crBGEnabled);
   m_pbrushDisabled->CreateSolidBrush(m_crBGDisabled);
}


//CMyComboBox::~CMyComboBox()
CColorDynamics::~CColorDynamics()
{
   delete m_pbrushEnabled;
   delete m_pbrushDisabled;
}


BEGIN_MESSAGE_MAP(CColorDynamics, CComboBox)
	//{{AFX_MSG_MAP(CColorDynamics)
   ON_WM_CTLCOLOR()
   ON_WM_DESTROY()
   ON_WM_CTLCOLOR_REFLECT()
   ON_WM_ENABLE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CMyComboBox message handlers

HBRUSH CColorDynamics::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   // Setting color in en- disabled mode
   if(IsWindowEnabled())
   {
      pDC->SetTextColor(m_crFGEnabled);
      pDC->SetBkColor(m_crBGEnabled);
      return *m_pbrushEnabled;
   }
   else
   {
      pDC->SetTextColor(m_crFGDisabled);
      pDC->SetBkColor(m_crBGDisabled);
      return *m_pbrushDisabled;
   }
}

HBRUSH CColorDynamics::CtlColor(CDC* pDC, UINT nCtlColor)
{
   // Setting color in en- disabled mode
   if(IsWindowEnabled())
   {
      pDC->SetTextColor(m_crFGEnabled);
      pDC->SetBkColor(m_crBGEnabled);
      return *m_pbrushEnabled;
   }
   else
   {
      pDC->SetTextColor(m_crFGDisabled);
      pDC->SetBkColor(m_crBGDisabled);
      return *m_pbrushDisabled;
   }
   return NULL;
}

void CColorDynamics::OnEnable(BOOL bEnable)
{
   CComboBox::OnEnable(bEnable);

   // TODO: Add your message handler code here

   // The first child is the CEdit
   CEdit* pComboEdit=(CEdit*)GetWindow(GW_CHILD);

   // Setting the edit ctrl always to enable
   pComboEdit->EnableWindow(TRUE);
   pComboEdit->SetReadOnly(!(m_bEditable && bEnable));
   Invalidate();
}

void CColorDynamics::SetReadOnly(bool bReadOnly)
{
   m_bEditable = !bReadOnly;

   // The first child is the CEdit
   CEdit* pComboEdit=(CEdit*)GetWindow(GW_CHILD);

   // Setting the edit ctrl always to enable
   pComboEdit->EnableWindow(TRUE);
   pComboEdit->SetReadOnly(!(m_bEditable && IsWindowEnabled()));
   Invalidate();
}

void CColorDynamics::SetEnabledColor(COLORREF crFG, COLORREF crBG)
{
   // Setting the colors and the brush
   m_crFGEnabled = crFG;
   m_crBGEnabled = crBG;
   delete m_pbrushEnabled;
   m_pbrushEnabled = new CBrush;
   m_pbrushEnabled->CreateSolidBrush(m_crBGEnabled);
}

void CColorDynamics::SetDisabledColor(COLORREF crFG, COLORREF crBG)
{
   // Setting the colors and the brush
   m_crFGDisabled = crFG;
   m_crBGDisabled = crBG;
   delete m_pbrushDisabled;
   m_pbrushDisabled = new CBrush;
   m_pbrushDisabled->CreateSolidBrush(m_crBGDisabled);
}
