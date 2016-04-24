#if !defined(AFX_LONGOPN_H__25484154_66DF_11D1_8055_006097B38214__INCLUDED_)
#define AFX_LONGOPN_H__25484154_66DF_11D1_8055_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LongOpn.h : header file
//


class CLongOpnFrameWnd;


/////////////////////////////////////////////////////////////////////////////
// CLongOpn thread

class CLongOpn : public CWinThread
{
	DECLARE_DYNCREATE(CLongOpn)
public:
	CLongOpn();           // protected constructor used by dynamic creation
	virtual ~CLongOpn();
// Attributes
public:

// Operations
public:

	void Go();
	void Done(int millisecs = 0);
	void SetCurrentStep(int stepNum);


	vector<CString> m_strings;
		// the set of strings to display in the list
		// indicating the different steps in the operation

	CString m_title;
		// title text for the window

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLongOpn)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:



	CLongOpnFrameWnd* m_pFrame;
		// the frame window for the display

	bool m_bInitialized;
		// flag for initialization

	int m_pendingStep;
		// the current step, an index into m_strings;

	// Generated message map functions
	//{{AFX_MSG(CLongOpn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LONGOPN_H__25484154_66DF_11D1_8055_006097B38214__INCLUDED_)
