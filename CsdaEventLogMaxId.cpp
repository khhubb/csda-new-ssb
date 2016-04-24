// CsdaEventLogMaxId.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CsdaEventLogMaxId.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogMaxId

IMPLEMENT_DYNAMIC(CCsdaEventLogMaxId, CRecordset)

CCsdaEventLogMaxId::CCsdaEventLogMaxId(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCsdaEventLogMaxId)
	m_MaxOfEventId = 0;
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCsdaEventLogMaxId::GetDefaultConnect()
{
	return _T("ODBC;DSN=csdaTest");
}

CString CCsdaEventLogMaxId::GetDefaultSQL()
{
	return _T("[MaxEventId]");
}

void CCsdaEventLogMaxId::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCsdaEventLogMaxId)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[MaxOfEventId]"), m_MaxOfEventId);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogMaxId diagnostics

#ifdef _DEBUG
void CCsdaEventLogMaxId::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCsdaEventLogMaxId::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
