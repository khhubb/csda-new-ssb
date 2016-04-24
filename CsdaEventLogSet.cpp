// CsdaEventLogSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CsdaEventLogSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet

IMPLEMENT_DYNAMIC(CCsdaEventLogSet, CRecordset)

CCsdaEventLogSet::CCsdaEventLogSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCsdaEventLogSet)
	m_EventId = 0;
	m_EventType = _T("");
	m_Comment = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CCsdaEventLogSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=csdaTest");
}

CString CCsdaEventLogSet::GetDefaultSQL()
{
	return _T("[EventLog]");
}

void CCsdaEventLogSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCsdaEventLogSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[EventId]"), m_EventId);
	RFX_Text(pFX, _T("[EventType]"), m_EventType);
	RFX_Date(pFX, _T("[EventTimestamp]"), m_EventTimestamp);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	//}}AFX_FIELD_MAP

	if ( m_nParams > 0 ) {
		pFX->SetFieldType(CFieldExchange::param);
		RFX_Text(pFX, _T("Whatever"), m_paramEventType);
		RFX_Long(pFX, _T("Whatever"), m_paramLastId);
		RFX_Date(pFX, _T("Whatever"), m_paramLastTimestamp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCsdaEventLogSet diagnostics

#ifdef _DEBUG
void CCsdaEventLogSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCsdaEventLogSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
