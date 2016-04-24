// HMSupplyActual.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "HMSupplyActual.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMSupplyActual

IMPLEMENT_DYNAMIC(CHMSupplyActual, CRecordset)

CHMSupplyActual::CHMSupplyActual(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CHMSupplyActual)
	m_5_BOF_tons_hr = 0.0f;
	m_6_BOF_tons_hr = 0.0f;
	m_7_BOF_tons_hr = 0.0f;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CHMSupplyActual::GetDefaultConnect()
{
	return _T("ODBC;DSN=hotmetal");
}

CString CHMSupplyActual::GetDefaultSQL()
{
	return _T("[HMSupplyActual]");
}

void CHMSupplyActual::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CHMSupplyActual)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, _T("[Date]"), m_Date);
	RFX_Single(pFX, _T("[5 BOF tons/hr]"), m_5_BOF_tons_hr);
	RFX_Single(pFX, _T("[6 BOF tons/hr]"), m_6_BOF_tons_hr);
	RFX_Single(pFX, _T("[7 BOF tons/hr]"), m_7_BOF_tons_hr);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CHMSupplyActual diagnostics

#ifdef _DEBUG
void CHMSupplyActual::AssertValid() const
{
	CRecordset::AssertValid();
}

void CHMSupplyActual::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
