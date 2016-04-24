// HeatSpecCrossAppSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "HeatSpecCrossAppSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeatSpecCrossAppSet

IMPLEMENT_DYNAMIC(CHeatSpecCrossAppSet, CRecordset)

CHeatSpecCrossAppSet::CHeatSpecCrossAppSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CHeatSpecCrossAppSet)
	m_SPECARD_SPEC = _T("");
	m_SPECARD_CROSS_APPL_SPEC_CD = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CHeatSpecCrossAppSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CHeatSpecCrossAppSet::GetDefaultSQL()
{
	return _T("[HeatSpecCrossApp]");
}

void CHeatSpecCrossAppSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CHeatSpecCrossAppSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[SPECARD_SPEC]"), m_SPECARD_SPEC);
	RFX_Text(pFX, _T("[SPECARD_CROSS_APPL_SPEC_CD]"), m_SPECARD_CROSS_APPL_SPEC_CD);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CHeatSpecCrossAppSet diagnostics

#ifdef _DEBUG
void CHeatSpecCrossAppSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CHeatSpecCrossAppSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
