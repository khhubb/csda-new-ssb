#if !defined(AFX_DRAGSOURCEBUTTON_H__1C24B8A0_53DE_11D1_804F_006097B38214__INCLUDED_)
#define AFX_DRAGSOURCEBUTTON_H__1C24B8A0_53DE_11D1_804F_006097B38214__INCLUDED_
 
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DragSourceButton.h : header file
//


// of course, this isn't really a button anymore
// we subclass other types of windows, typically a CStatic being used as an image,
//   to provide click notification.


class CNotify;


/////////////////////////////////////////////////////////////////////////////
// CDragSourceButton window

class CDragSourceButton : public CButton
{
// Construction
public:
	CDragSourceButton();

// Attributes
public:

	CNotify* m_pNotify;
		// pointer to an object to callback to.

// Operations
public:

	void SetNotify(CNotify* p);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragSourceButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragSourceButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragSourceButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGSOURCEBUTTON_H__1C24B8A0_53DE_11D1_804F_006097B38214__INCLUDED_)
