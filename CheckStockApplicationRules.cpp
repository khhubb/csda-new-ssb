// CheckStockApplicationRules.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "CheckStockApplicationRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckStockApplicationRules

IMPLEMENT_DYNAMIC(CCheckStockApplicationRules, CRecordset)

CCheckStockApplicationRules::CCheckStockApplicationRules(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCheckStockApplicationRules)
	m_CASTER_UNIT = 0;
	m_HEAT_SPEC = _T("");
	m_HEAT_SPEC_NUMERIC_5 = 0;
	m_HEAT_SPEC_6TH_DIGIT = 0;
	m_HEAT_SPEC_7TH_DIGIT = 0;
	m_QUALITY = 0;
	m_INCLUSION_MODIFIED = _T("");
	m_SLAB_BLOOM_MODE = _T("");
	m_NUMBER_OF_FURNACES = 0;
	m_MINIMUM_WIDTH_INCHES = 0;
	m_MAXIMUM_WIDTH_INCHES = 0;
	m_PLANNED_HTS_PER_TD_MIN = 0;
	m_PLANNED_HTS_PER_TD_MAX = 0;
	m_PLANNED_HTS_PER_TD_AIM = 0;
	m_PLANNED_SHROUDCHANGE_MIN = 0;
	m_PLANNED_SHROUDCHANGE_MAX = 0;
	m_PLANNED_SHROUDCHANGE_AIM = 0;
	m_COMMENT = _T("");
	m_AUTHOR = _T("");
	m_SERIAL_NUMBER = 0;
	m_nFields = 21;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCheckStockApplicationRules::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
//	return _T("ODBC;DSN=rules");

}

CString CCheckStockApplicationRules::GetDefaultSQL()
{
//	return _T("[SLAB_APPLICATION]");
	return _T("[CASTER_STRING_LENGTHS_VIEW]");

}

void CCheckStockApplicationRules::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCheckStockApplicationRules)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[CASTER_UNIT]"), m_CASTER_UNIT);
	RFX_Text(pFX, _T("[HEAT_SPEC]"), m_HEAT_SPEC);
	RFX_Long(pFX, _T("[HEAT_SPEC_NUMERIC_5]"), m_HEAT_SPEC_NUMERIC_5);
	RFX_Int(pFX, _T("[HEAT_SPEC_6TH_DIGIT]"), m_HEAT_SPEC_6TH_DIGIT);
	RFX_Int(pFX, _T("[HEAT_SPEC_7TH_DIGIT]"), m_HEAT_SPEC_7TH_DIGIT);
	RFX_Int(pFX, _T("[QUALITY]"), m_QUALITY);
	RFX_Text(pFX, _T("[INCLUSION_MODIFIED]"), m_INCLUSION_MODIFIED);
	RFX_Text(pFX, _T("[SLAB_BLOOM_MODE]"), m_SLAB_BLOOM_MODE);
	RFX_Long(pFX, _T("[NUMBER_OF_FURNACES]"), m_NUMBER_OF_FURNACES);
	RFX_Int(pFX, _T("[MINIMUM_WIDTH_INCHES]"), m_MINIMUM_WIDTH_INCHES);
	RFX_Int(pFX, _T("[MAXIMUM_WIDTH_INCHES]"), m_MAXIMUM_WIDTH_INCHES);
	RFX_Int(pFX, _T("[PLANNED_HTS_PER_TD_MIN]"), m_PLANNED_HTS_PER_TD_MIN);
	RFX_Int(pFX, _T("[PLANNED_HTS_PER_TD_MAX]"), m_PLANNED_HTS_PER_TD_MAX);
	RFX_Int(pFX, _T("[PLANNED_HTS_PER_TD_AIM]"), m_PLANNED_HTS_PER_TD_AIM);
	RFX_Int(pFX, _T("[PLANNED_SHROUDCHANGE_MIN]"), m_PLANNED_SHROUDCHANGE_MIN);
	RFX_Int(pFX, _T("[PLANNED_SHROUDCHANGE_MAX]"), m_PLANNED_SHROUDCHANGE_MAX);
	RFX_Int(pFX, _T("[PLANNED_SHROUDCHANGE_AIM]"), m_PLANNED_SHROUDCHANGE_AIM);
	RFX_Text(pFX, _T("[COMMENT]"), m_COMMENT);
	RFX_Date(pFX, _T("[DATE_ENTERED]"), m_DATE_ENTERED);
	RFX_Text(pFX, _T("[AUTHOR]"), m_AUTHOR);
	RFX_Long(pFX, _T("[SERIAL_NUMBER]"), m_SERIAL_NUMBER);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCheckStockApplicationRules diagnostics

#ifdef _DEBUG
void CCheckStockApplicationRules::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCheckStockApplicationRules::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
