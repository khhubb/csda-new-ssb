#if !defined(AFX_OUTPUTWND_H__1B0305B7_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
#define AFX_OUTPUTWND_H__1B0305B7_325B_11D3_85E7_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputWnd window

class COutputWnd : public CEdit
{
// Construction
public:
	COutputWnd();

	DECLARE_DYNAMIC(COutputWnd)

// Attributes
public:

// Operations
public:

	void ClearText();
	void AddText(LPCTSTR text);
	void AddText(ostrstream& ostr);
	void RemoveLinesIfNecessary();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTWND_H__1B0305B7_325B_11D3_85E7_00104B2D39AC__INCLUDED_)
