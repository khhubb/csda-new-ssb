// SteelSpecCrossAppSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "SteelSpecCrossAppSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSteelSpecCrossAppSet

IMPLEMENT_DYNAMIC(CSteelSpecCrossAppSet, CRecordset)

CSteelSpecCrossAppSet::CSteelSpecCrossAppSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSteelSpecCrossAppSet)
	m_SPECARD_SPEC = _T("");
	m_SPECARD_CROSS_APPL_SPEC_CD = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSteelSpecCrossAppSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CSteelSpecCrossAppSet::GetDefaultSQL()
{
	return _T("[SteelSpecCrossApp]");
}

void CSteelSpecCrossAppSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSteelSpecCrossAppSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[SPECARD_SPEC]"), m_SPECARD_SPEC);
	RFX_Text(pFX, _T("[SPECARD_CROSS_APPL_SPEC_CD]"), m_SPECARD_CROSS_APPL_SPEC_CD);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSteelSpecCrossAppSet diagnostics

#ifdef _DEBUG
void CSteelSpecCrossAppSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSteelSpecCrossAppSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
