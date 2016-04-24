// SpecDPAChemsSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "SpecDPAChemsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecDPAChemsSet

IMPLEMENT_DYNAMIC(CSpecDPAChemsSet, CRecordset)

CSpecDPAChemsSet::CSpecDPAChemsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSpecDPAChemsSet)
	m_STEEL_SPECIFICATION = 0;
	m_SIXTH_DIGIT_OF_SPEC = _T("");
	m_SEVENTH_DIGIT_OF_SPEC = 0;
	m_CASTER_INDICATOR = _T("");
	m_ELEMENT_SYMBOL = _T("");
	m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT = 0.0f;
	m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT = 0.0f;
	m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT = 0.0f;
	m_SM_SPEC_OPEN_RANGE_IND = _T("");
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSpecDPAChemsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CSpecDPAChemsSet::GetDefaultSQL()
{
	return _T("[SpecChems]");
}

void CSpecDPAChemsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSpecDPAChemsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[STEEL_SPECIFICATION]"), m_STEEL_SPECIFICATION);
	RFX_Text(pFX, _T("[SIXTH_DIGIT_OF_SPEC]"), m_SIXTH_DIGIT_OF_SPEC);
	RFX_Long(pFX, _T("[SEVENTH_DIGIT_OF_SPEC]"), m_SEVENTH_DIGIT_OF_SPEC);
	RFX_Text(pFX, _T("[CASTER_INDICATOR]"), m_CASTER_INDICATOR);
	RFX_Text(pFX, _T("[ELEMENT_SYMBOL]"), m_ELEMENT_SYMBOL);
	RFX_Single(pFX, _T("[SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT]"), m_SM_SPEC_CHEM_GRP_ELMNT_AIM_PCT);
	RFX_Single(pFX, _T("[SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT]"), m_SM_SPEC_CHEM_GRP_ELMNT_MIN_PCT);
	RFX_Single(pFX, _T("[SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT]"), m_SM_SPEC_CHEM_GRP_ELMNT_MAX_PCT);
	RFX_Text(pFX, _T("[SM_SPEC_OPEN_RANGE_IND]"), m_SM_SPEC_OPEN_RANGE_IND);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSpecDPAChemsSet diagnostics

#ifdef _DEBUG
void CSpecDPAChemsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSpecDPAChemsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
