// HMSupplyProjected.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "HMSupplyProjected.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMSupplyProjected

IMPLEMENT_DYNAMIC(CHMSupplyProjected, CRecordset)

CHMSupplyProjected::CHMSupplyProjected(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CHMSupplyProjected)
	m_5_BOF_tons_hr = 0.0f;
	m_6_BOF_tons_hr = 0.0f;
	m_7_BOF_tons_hr = 0.0f;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CHMSupplyProjected::GetDefaultConnect()
{
	return _T("ODBC;DSN=hotmetal");
}

CString CHMSupplyProjected::GetDefaultSQL()
{
	return _T("[HMSupplyProjected]");
}

void CHMSupplyProjected::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CHMSupplyProjected)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, _T("[Date]"), m_Date);
	RFX_Single(pFX, _T("[5 BOF tons/hr]"), m_5_BOF_tons_hr);
	RFX_Single(pFX, _T("[6 BOF tons/hr]"), m_6_BOF_tons_hr);
	RFX_Single(pFX, _T("[7 BOF tons/hr]"), m_7_BOF_tons_hr);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CHMSupplyProjected diagnostics

#ifdef _DEBUG
void CHMSupplyProjected::AssertValid() const
{
	CRecordset::AssertValid();
}

void CHMSupplyProjected::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
