// ScenMasterDoc.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "ScenMasterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CScenMasterDoc
//
//  Standard dummy document for MVC.
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CScenMasterDoc, CDocument)

CScenMasterDoc::CScenMasterDoc()
{
	m_fixedTitle = "Scen master";
}



CScenMasterDoc::~CScenMasterDoc()
{
}


BEGIN_MESSAGE_MAP(CScenMasterDoc, CDocument)
	//{{AFX_MSG_MAP(CScenMasterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenMasterDoc diagnostics

#ifdef _DEBUG
void CScenMasterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScenMasterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScenMasterDoc serialization


/////////////////////////////////////////////////////////////////////////////
// CScenMasterDoc commands

