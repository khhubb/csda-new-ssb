#if !defined(AFX_SCENMASTERDOC_H__BEF847D9_AAFE_11D1_8068_006097B38214__INCLUDED_)
#define AFX_SCENMASTERDOC_H__BEF847D9_AAFE_11D1_8068_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMasterDoc.h : header file
//

#include "DocBase.h"

/////////////////////////////////////////////////////////////////////////////
// CScenMasterDoc document

class CScenMasterDoc : public CDocBase
{
public:



protected:
	CScenMasterDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScenMasterDoc)

// Attributes
public:



// Operations
public:



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMasterDoc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScenMasterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScenMasterDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMASTERDOC_H__BEF847D9_AAFE_11D1_8068_006097B38214__INCLUDED_)
