// CasterScenariosMaxIdSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CasterScenariosMaxIdSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosMaxIdSet

IMPLEMENT_DYNAMIC(CCasterScenariosMaxIdSet, CRecordset)

CCasterScenariosMaxIdSet::CCasterScenariosMaxIdSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterScenariosMaxIdSet)
	m_MaxOfId = 0;
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterScenariosMaxIdSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=csdaTest");
}

CString CCasterScenariosMaxIdSet::GetDefaultSQL()
{
	return _T("[MaxCasterId]");
}

void CCasterScenariosMaxIdSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterScenariosMaxIdSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[MaxOfId]"), m_MaxOfId);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosMaxIdSet diagnostics

#ifdef _DEBUG
void CCasterScenariosMaxIdSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterScenariosMaxIdSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
