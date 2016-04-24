//////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_ColorDynamics_H__75106EA1_6649_11D1_95E5_0060971F6E1E__INCLUDED_)
#define AFX_ColorDynamics_H__75106EA1_6649_11D1_95E5_0060971F6E1E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ColorDynamics.h : header file
//

// CMyComboBox window

// Author: Robert Cremer

// A ComboBox with easy to switch between edit mode and non edit mode.
// (i. e. works like a drop down or a drop list)
// The ComboBox has real coloring, working in drop down and drop list mode.
// !!! The Box must not be a drop list (CBS_DROPDOWNLIST) !!!
// Working as a drop list can be done with a call to SetReadOnly()
// or with the CTor param false.
class CColorDynamics : public CComboBox
{
// Attributes
private:
   // default colors
   enum
   {
      ENABLED_FG = RGB(0,0,0), // black
      ENABLED_BG = RGB(255,255,255), // white
      DISABLED_FG = RGB(0,0,0), // black
      DISABLED_BG = RGB(192,192,192), // light grey
   };

   // edit mode
   bool m_bEditable;

   // the actual colors
   COLORREF m_crFGEnabled;
   COLORREF m_crBGEnabled;
   COLORREF m_crFGDisabled;
   COLORREF m_crBGDisabled;

   // Background brush
   CBrush *m_pbrushDisabled;
   // Foreground brush
   CBrush *m_pbrushEnabled;

// Operations
public:
   void SetReadOnly(bool bReadOnly = true);
   void SetEnabledColor(COLORREF crFG = ENABLED_FG, COLORREF crBG = ENABLED_BG);
   void SetDisabledColor(COLORREF crFG = DISABLED_FG, COLORREF crBG =
DISABLED_BG);

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CColorDynamics)
//}}AFX_VIRTUAL

// Implementation
public:
   // Construction
   CColorDynamics(bool bEditable = true);
   virtual ~CColorDynamics();

   // Generated message map functions
protected:
   //{{AFX_MSG(CColorDynamics)
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   afx_msg void OnEnable(BOOL bEnable);
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ColorDynamics window

class ColorDynamics : public CComboBox
{
// Construction
public:
	ColorDynamics();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColorDynamics)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ColorDynamics();

	// Generated message map functions
protected:
	//{{AFX_MSG(ColorDynamics)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //!defined(AFX_ColorDynamics_H__75106EA1_6649_11D1_95E5_0060971F6E1E__INCLUDED_)

//////////////////////////////////////////////////////////////////////////////////////
