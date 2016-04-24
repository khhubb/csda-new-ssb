// SBoutput.h : Implementation of the CSBoutput class



// CSBoutput implementation

// code generated on Sunday, April 24, 2016, 7:33 AM

#include "stdafx.h"
#include "SBoutput.h"
IMPLEMENT_DYNAMIC(CSBoutput, CRecordset)

CSBoutput::CSBoutput(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PLAN_WEEK = 0;
	m_PLAN_CAST_UNIT_CODE = L"";
	m_PLAN_CAST_SEQUENCE_NO = 0;
	m_PLAN_STRING_ID = 0;
	m_PLAN_HEAT_GROUP = 0;
	m_PLAN_HEAT_SEQUENCE_NO = 0;
	m_PLAN_HEAT_TAB_NUMBER = 0;
	m_PLAN_HEAT_SPEC = 0;
	m_PLAN_HEAT_SIXTH_DIGIT_SPEC = L"";
	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC = 0;
	m_PLAN_STEEL_EARLY_START_DATE = 0;
	m_PLAN_STEEL_EARLY_START_TIME = 0;
	m_PLAN_STEEL_LATEST_END_DATE = 0;
	m_PLAN_STEEL_LATEST_END_TIME = 0;
	m_PLAN_HEAT_PROD_TIME_MINUTES = 0;
	m_PLAN_HEAT_DISPOSITION_CODE = 0;
	m_PLAN_HEAT_BACKUP_MOLD_CODE = 0;
	m_PLAN_HEAT_CARBON_MIN = 0.0;
	m_PLAN_HEAT_CARBON_MAX = 0.0;
	m_PLAN_HEAT_MANGANESE_MIN = 0.0;
	m_PLAN_HEAT_MANGANESE_MAX = 0.0;
	m_PLAN_HEAT_STATUS_CODE = L"";
	m_PLAN_LINEUP_NUMBER = 0;
	m_PLAN_LINEUP_ITEM_SEQ_NO = 0;
	m_PLAN_LINEUP_COMMENTS = L"";
	m_PLAN_910_REPORT_IND = L"";
	m_PLAN_LOT_NUMBER = 0;
	m_MILL_ORDER_NO = 0;
	m_PLAN_STEEL_WIDTH = 0.0;
	m_PLAN_STEEL_THICKNESS = 0.0;
	m_PLAN_STEEL_LENGTH = 0.0;
	m_PLAN_STEEL_WGT_TONS = 0.0;
	m_PLAN_NUMBER_OF_PIECES = 0;
	m_PLAN_LOT_SPEC = L"";
	m_PLAN_LOT_DISP_CODE = 0;
	m_PLAN_LOT_TYPE_SLAB = L"";
	m_PLAN_SLAB_CONDITION_CD = 0;
	m_PLAN_EXPOSURE_CODE = L"";
	m_PLAN_HOT_MILL_CD = 0;
	m_PLAN_CASTER_STRAND_NO = L"";
	m_PLAN_CHANGE_INDICATOR = L"";
	m_PLAN_ORIGINAL_PLAN_ID = L"";
	m_PLAN_EXPECTED_APPLIED_SPEC = L"";
	m_PLAN_EXPECTED_APPLIED_COND_CD = 0;
	m_PLAN_TO_BOF_DATE = 0;
	m_PLAN_LOT_TYPE_SLIT_CODE = L"";
	m_PROV_SLABS_DUE = 0;
	m_PROV_SLAB_WGT_DUE = 0;
	m_PLAN_909_LOCK_IND = L"";
	m_PLAN_STEEL_LENGTH_MIN = 0.0;
	m_PLAN_STEEL_LENGTH_MAX = 0.0;
	m_FILLER0 = L"";
	m_nFields = 52;
	m_nDefaultType = snapshot;
}
#error Security Issue: The connection string may contain a password
// The connection string below may contain plain text passwords and/or
// other sensitive information. Please remove the #error after reviewing
// the connection string for any security related issues. You may want to
// store the password in some other form or use a different user authentication.
CString CSBoutput::GetDefaultConnect()
{
	return _T("DSN=csdatest2;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=DM;DATABASE=csd_newinl");
}

CString CSBoutput::GetDefaultSQL()
{
	return _T("[dbo].[SBoutput]");
}

void CSBoutput::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[PLAN_WEEK]"), m_PLAN_WEEK);
	RFX_Text(pFX, _T("[PLAN_CAST_UNIT_CODE]"), m_PLAN_CAST_UNIT_CODE);
	RFX_Long(pFX, _T("[PLAN_CAST_SEQUENCE_NO]"), m_PLAN_CAST_SEQUENCE_NO);
	RFX_Long(pFX, _T("[PLAN_STRING_ID]"), m_PLAN_STRING_ID);
	RFX_Long(pFX, _T("[PLAN_HEAT_GROUP]"), m_PLAN_HEAT_GROUP);
	RFX_Long(pFX, _T("[PLAN_HEAT_SEQUENCE_NO]"), m_PLAN_HEAT_SEQUENCE_NO);
	RFX_Long(pFX, _T("[PLAN_HEAT_TAB_NUMBER]"), m_PLAN_HEAT_TAB_NUMBER);
	RFX_Long(pFX, _T("[PLAN_HEAT_SPEC]"), m_PLAN_HEAT_SPEC);
	RFX_Text(pFX, _T("[PLAN_HEAT_SIXTH_DIGIT_SPEC]"), m_PLAN_HEAT_SIXTH_DIGIT_SPEC);
	RFX_Long(pFX, _T("[PLAN_HEAT_SEVENTH_DIGIT_SPEC]"), m_PLAN_HEAT_SEVENTH_DIGIT_SPEC);
	RFX_Long(pFX, _T("[PLAN_STEEL_EARLY_START_DATE]"), m_PLAN_STEEL_EARLY_START_DATE);
	RFX_Long(pFX, _T("[PLAN_STEEL_EARLY_START_TIME]"), m_PLAN_STEEL_EARLY_START_TIME);
	RFX_Long(pFX, _T("[PLAN_STEEL_LATEST_END_DATE]"), m_PLAN_STEEL_LATEST_END_DATE);
	RFX_Long(pFX, _T("[PLAN_STEEL_LATEST_END_TIME]"), m_PLAN_STEEL_LATEST_END_TIME);
	RFX_Long(pFX, _T("[PLAN_HEAT_PROD_TIME_MINUTES]"), m_PLAN_HEAT_PROD_TIME_MINUTES);
	RFX_Long(pFX, _T("[PLAN_HEAT_DISPOSITION_CODE]"), m_PLAN_HEAT_DISPOSITION_CODE);
	RFX_Long(pFX, _T("[PLAN_HEAT_BACKUP_MOLD_CODE]"), m_PLAN_HEAT_BACKUP_MOLD_CODE);
	RFX_Single(pFX, _T("[PLAN_HEAT_CARBON_MIN]"), m_PLAN_HEAT_CARBON_MIN);
	RFX_Single(pFX, _T("[PLAN_HEAT_CARBON_MAX]"), m_PLAN_HEAT_CARBON_MAX);
	RFX_Single(pFX, _T("[PLAN_HEAT_MANGANESE_MIN]"), m_PLAN_HEAT_MANGANESE_MIN);
	RFX_Single(pFX, _T("[PLAN_HEAT_MANGANESE_MAX]"), m_PLAN_HEAT_MANGANESE_MAX);
	RFX_Text(pFX, _T("[PLAN_HEAT_STATUS_CODE]"), m_PLAN_HEAT_STATUS_CODE);
	RFX_Long(pFX, _T("[PLAN_LINEUP_NUMBER]"), m_PLAN_LINEUP_NUMBER);
	RFX_Long(pFX, _T("[PLAN_LINEUP_ITEM_SEQ_NO]"), m_PLAN_LINEUP_ITEM_SEQ_NO);
	RFX_Text(pFX, _T("[PLAN_LINEUP_COMMENTS]"), m_PLAN_LINEUP_COMMENTS);
	RFX_Text(pFX, _T("[PLAN_910_REPORT_IND]"), m_PLAN_910_REPORT_IND);
	RFX_Long(pFX, _T("[PLAN_LOT_NUMBER]"), m_PLAN_LOT_NUMBER);
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Single(pFX, _T("[PLAN_STEEL_WIDTH]"), m_PLAN_STEEL_WIDTH);
	RFX_Single(pFX, _T("[PLAN_STEEL_THICKNESS]"), m_PLAN_STEEL_THICKNESS);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH]"), m_PLAN_STEEL_LENGTH);
	RFX_Single(pFX, _T("[PLAN_STEEL_WGT_TONS]"), m_PLAN_STEEL_WGT_TONS);
	RFX_Long(pFX, _T("[PLAN_NUMBER_OF_PIECES]"), m_PLAN_NUMBER_OF_PIECES);
	RFX_Text(pFX, _T("[PLAN_LOT_SPEC]"), m_PLAN_LOT_SPEC);
	RFX_Long(pFX, _T("[PLAN_LOT_DISP_CODE]"), m_PLAN_LOT_DISP_CODE);
	RFX_Text(pFX, _T("[PLAN_LOT_TYPE_SLAB]"), m_PLAN_LOT_TYPE_SLAB);
	RFX_Long(pFX, _T("[PLAN_SLAB_CONDITION_CD]"), m_PLAN_SLAB_CONDITION_CD);
	RFX_Text(pFX, _T("[PLAN_EXPOSURE_CODE]"), m_PLAN_EXPOSURE_CODE);
	RFX_Long(pFX, _T("[PLAN_HOT_MILL_CD]"), m_PLAN_HOT_MILL_CD);
	RFX_Text(pFX, _T("[PLAN_CASTER_STRAND_NO]"), m_PLAN_CASTER_STRAND_NO);
	RFX_Text(pFX, _T("[PLAN_CHANGE_INDICATOR]"), m_PLAN_CHANGE_INDICATOR);
	RFX_Text(pFX, _T("[PLAN_ORIGINAL_PLAN_ID]"), m_PLAN_ORIGINAL_PLAN_ID);
	RFX_Text(pFX, _T("[PLAN_EXPECTED_APPLIED_SPEC]"), m_PLAN_EXPECTED_APPLIED_SPEC);
	RFX_Long(pFX, _T("[PLAN_EXPECTED_APPLIED_COND_CD]"), m_PLAN_EXPECTED_APPLIED_COND_CD);
	RFX_Long(pFX, _T("[PLAN_TO_BOF_DATE]"), m_PLAN_TO_BOF_DATE);
	RFX_Text(pFX, _T("[PLAN_LOT_TYPE_SLIT_CODE]"), m_PLAN_LOT_TYPE_SLIT_CODE);
	RFX_Long(pFX, _T("[PROV_SLABS_DUE]"), m_PROV_SLABS_DUE);
	RFX_Long(pFX, _T("[PROV_SLAB_WGT_DUE]"), m_PROV_SLAB_WGT_DUE);
	RFX_Text(pFX, _T("[PLAN_909_LOCK_IND]"), m_PLAN_909_LOCK_IND);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH_MIN]"), m_PLAN_STEEL_LENGTH_MIN);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH_MAX]"), m_PLAN_STEEL_LENGTH_MAX);
	RFX_Text(pFX, _T("[FILLER0]"), m_FILLER0);

}
/////////////////////////////////////////////////////////////////////////////
// CSBoutput diagnostics

#ifdef _DEBUG
void CSBoutput::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSBoutput::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


