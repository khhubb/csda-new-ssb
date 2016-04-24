// SpecDPASet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "SpecDPASet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecDPASet

IMPLEMENT_DYNAMIC(CSpecDPASet, CRecordset)

CSpecDPASet::CSpecDPASet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSpecDPASet)
	m_SPEC_NAME = _T("");
	m_CASTER_INDICATOR = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSpecDPASet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CSpecDPASet::GetDefaultSQL()
{
	return _T("[SpecsFromCompleteOldSteelSpec]");
}

void CSpecDPASet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSpecDPASet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[SPEC_NAME]"), m_SPEC_NAME);
	RFX_Text(pFX, _T("[CASTER_INDICATOR]"), m_CASTER_INDICATOR);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSpecDPASet diagnostics

#ifdef _DEBUG
void CSpecDPASet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSpecDPASet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
