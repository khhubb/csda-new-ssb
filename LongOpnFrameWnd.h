#if !defined(AFX_LONGOPNFRAMEWND_H__BA9FA470_8F57_11D1_8065_006097B38214__INCLUDED_)
#define AFX_LONGOPNFRAMEWND_H__BA9FA470_8F57_11D1_8065_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LongOpnFrameWnd.h : header file
//

class CLongOpnView;

 
/////////////////////////////////////////////////////////////////////////////
// CLongOpnFrameWnd frame
//
//  This class provides a CFrameWnd for the CLongOpn mechanism.
//  
//  See the .cpp file for more details.
//
/////////////////////////////////////////////////////////////////////////////

class CLongOpnFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CLongOpnFrameWnd)
public:
	CLongOpnFrameWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

	
	void SetStrings(vector<CString>& strings);

	int m_step;
		// the current step, i.e., which operation in the sequence we are one

	int m_incr;
		// the index into the bitmap array
		
	UINT m_timerId;
		// the active timer sending messages that allow this to update its display

	vector<CString> m_strings;
		// this list of strings to display in the list

	bool m_bInitialized;
		// flag to indicate if initialization has occurred

	CListCtrl m_list;
		// the list displayed in the frame.

	CImageList m_imageList;
		// the images used to animate the current step

	// ## change return type to LRESULT
	// # change args from (UINT,LONG) to (WPARAM,LPARAM)
		afx_msg LRESULT OnStep(WPARAM, LPARAM);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLongOpnFrameWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLongOpnFrameWnd();

	// Generated message map functions
	//{{AFX_MSG(CLongOpnFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ## Change UINT to UINT_PTR
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LONGOPNFRAMEWND_H__BA9FA470_8F57_11D1_8065_006097B38214__INCLUDED_)
