// MillOrderChemsSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "MillOrderChemsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMillOrderChemsSet

IMPLEMENT_DYNAMIC(CMillOrderChemsSet, CRecordset)

CMillOrderChemsSet::CMillOrderChemsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CMillOrderChemsSet)
	m_FP_M_ORDER_NO = _T("");
	m_MILL_ORDER_NO = 0;
	m_MORD_INHSE_ORDER_CHEM_EL = _T("");
	m_MORD_INHSE_ORDER_PCT_MAX = 0.0f;
	m_MORD_INHSE_ORDER_PCT_MIN = 0.0f;
	m_MORD_INHSE_ORDER_REAS_CD = _T("");
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CMillOrderChemsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CMillOrderChemsSet::GetDefaultSQL()
{
	return _T("[MillOrderChems]");
}

void CMillOrderChemsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CMillOrderChemsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[FP_M_ORDER_NO]"), m_FP_M_ORDER_NO);
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Text(pFX, _T("[MORD_INHSE_ORDER_CHEM_EL]"), m_MORD_INHSE_ORDER_CHEM_EL);
	RFX_Single(pFX, _T("[MORD_INHSE_ORDER_PCT_MAX]"), m_MORD_INHSE_ORDER_PCT_MAX);
	RFX_Single(pFX, _T("[MORD_INHSE_ORDER_PCT_MIN]"), m_MORD_INHSE_ORDER_PCT_MIN);
	RFX_Text(pFX, _T("[MORD_INHSE_ORDER_REAS_CD]"), m_MORD_INHSE_ORDER_REAS_CD);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMillOrderChemsSet diagnostics

#ifdef _DEBUG
void CMillOrderChemsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CMillOrderChemsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
