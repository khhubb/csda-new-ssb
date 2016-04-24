#if !defined(AFX_DOCBASE_H__AEF38697_E7EC_11D3_862C_00104B2D39AC__INCLUDED_)
#define AFX_DOCBASE_H__AEF38697_E7EC_11D3_862C_00104B2D39AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DocBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDocBase document

#include "DocMvcMixin.h"

class CDocBase : public CDocument, public CDocMvcMixin
{
protected:
	CDocBase() {}
	DECLARE_DYNCREATE(CDocBase)

	//override
	BOOL IsModified();


	//  Initialize the fixed title in the c-tor for each derived class.
	CString m_fixedTitle;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocBase)
	public:
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDocBase() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDocBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCBASE_H__AEF38697_E7EC_11D3_862C_00104B2D39AC__INCLUDED_)
