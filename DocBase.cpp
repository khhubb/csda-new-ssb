// DocBase.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "DocBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MessageBus.h"


/////////////////////////////////////////////////////////////////////////////
// CDocBase

IMPLEMENT_DYNCREATE(CDocBase, CDocument)



BEGIN_MESSAGE_MAP(CDocBase, CDocument)
	//{{AFX_MSG_MAP(CDocBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocBase diagnostics

#ifdef _DEBUG
void CDocBase::AssertValid() const
{
	CDocument::AssertValid();
}

void CDocBase::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDocBase serialization


/////////////////////////////////////////////////////////////////////////////
// CDocBase commands

void CDocBase::SetTitle(LPCTSTR lpszTitle) 
{	
	CDocument::SetTitle(m_fixedTitle);
}


BOOL CDocBase::IsModified()
{	
	return CDocument::IsModified() ||  GetModel()->IsModified(); 
}
