// ScenMgrDoc.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "ScenMgrDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenMgrDoc
//
//  Standard dummy document for MVC.
//
/////////////////////////////////////////////////////////////////////////////



IMPLEMENT_DYNCREATE(CScenMgrDoc, CDocument)

CScenMgrDoc::CScenMgrDoc()
{
	m_fixedTitle = "Scenario Manager";
}


CScenMgrDoc::~CScenMgrDoc()
{
}


BEGIN_MESSAGE_MAP(CScenMgrDoc, CDocument)
	//{{AFX_MSG_MAP(CScenMgrDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMgrDoc diagnostics

#ifdef _DEBUG
void CScenMgrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScenMgrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScenMgrDoc serialization


/////////////////////////////////////////////////////////////////////////////
// CScenMgrDoc commands
