// CICodeSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CICodeSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCICodeSet

IMPLEMENT_DYNAMIC(CCICodeSet, CRecordset)

CCICodeSet::CCICodeSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCICodeSet)
	m_CI_CODE = 0;
	m_COMMODITY_NAME = _T("");
	m_CI_CODE_PARENT = 0;
	m_SORT_ORDER = 0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCICodeSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCICodeSet::GetDefaultSQL()
{
	return _T("[CI_CODES]");
}

void CCICodeSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCICodeSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[CI_CODE]"), m_CI_CODE);
	RFX_Text(pFX, _T("[COMMODITY_NAME]"), m_COMMODITY_NAME);
	RFX_Int(pFX, _T("[CI_CODE_PARENT]"), m_CI_CODE_PARENT);
	RFX_Int(pFX, _T("[SORT_ORDER]"), m_SORT_ORDER);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCICodeSet diagnostics

#ifdef _DEBUG
void CCICodeSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCICodeSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
