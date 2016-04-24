// CsdaEventLogSet2.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CsdaEventLogSet2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet2

IMPLEMENT_DYNAMIC(CCsdaEventLogSet2, CRecordset)

CCsdaEventLogSet2::CCsdaEventLogSet2(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCsdaEventLogSet2)
	m_Comment = _T("");
	m_EventType = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;

	m_EventTimestamp = CTime::GetCurrentTime();
}


CString CCsdaEventLogSet2::GetDefaultConnect()
{
	return _T("ODBC;DSN=csdaTest");
}

CString CCsdaEventLogSet2::GetDefaultSQL()
{
	return _T("[EventLog]");
}

void CCsdaEventLogSet2::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCsdaEventLogSet2)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[EventType]"), m_EventType);
	RFX_Date(pFX, _T("[EventTimestamp]"), m_EventTimestamp);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet2 diagnostics

#ifdef _DEBUG
void CCsdaEventLogSet2::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCsdaEventLogSet2::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
