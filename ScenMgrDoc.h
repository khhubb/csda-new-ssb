#if !defined(AFX_SCENMGRDOC_H__4545D594_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_SCENMGRDOC_H__4545D594_A6F5_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScenMgrDoc.h : header file
//

#include "DocBase.h"

/////////////////////////////////////////////////////////////////////////////
// CScenMgrDoc document

class CScenMgrDoc : public CDocBase
{
protected:
	CScenMgrDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScenMgrDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenMgrDoc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScenMgrDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScenMgrDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENMGRDOC_H__4545D594_A6F5_11D0_975C_0000C0C2484C__INCLUDED_)
