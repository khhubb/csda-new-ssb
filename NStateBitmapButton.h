#if !defined(AFX_NSTATEBITMAPBUTTON_H__D1E65AFB_9AC9_11D1_8066_006097B38214__INCLUDED_)
#define AFX_NSTATEBITMAPBUTTON_H__D1E65AFB_9AC9_11D1_8066_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NStateBitmapButton.h : header file
//
 
/////////////////////////////////////////////////////////////////////////////
// CNStateBitmapButton window

class CNStateBitmapButton : public CButton
{
// Construction
public:
	CNStateBitmapButton();

// Attributes
public:


	void SetBitmaps(vector<int>& ids);

	void SetCurrentState(int newVal);

	int CurrentState() const
	{	return m_currentState;	}

	//## int to size_t
	size_t NumStates() const
	{	return m_bitmaps.size();	}

private:

	vector<CBitmap*> m_bitmaps;
		// the bitmaps to use for the various states.
		// the length of this vector determines the number of states

	bool m_isInitialized;
		// flag to indicate if bitmaps have been set

	int m_currentState;
		// the state currently displayed.


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNStateBitmapButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNStateBitmapButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNStateBitmapButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NSTATEBITMAPBUTTON_H__D1E65AFB_9AC9_11D1_8066_006097B38214__INCLUDED_)
