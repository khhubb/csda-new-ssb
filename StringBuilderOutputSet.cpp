// StringBuilderOutputSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "StringBuilderOutputSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderOutputSet

IMPLEMENT_DYNAMIC(CStringBuilderOutputSet, CRecordset)

CStringBuilderOutputSet::CStringBuilderOutputSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CStringBuilderOutputSet)
	m_PLAN_WEEK = 0;
	m_PLAN_CAST_UNIT_CODE = _T("");
	m_PLAN_CAST_SEQUENCE_NO = 0;
	m_PLAN_STRING_ID = 0;
	m_PLAN_HEAT_GROUP = 0;
	m_PLAN_HEAT_SEQUENCE_NO = 0;
	m_PLAN_HEAT_TAB_NUMBER = 0;
	m_PLAN_HEAT_SPEC = 0;
	m_PLAN_HEAT_SIXTH_DIGIT_SPEC = _T("");
	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC = 0;
	m_PLAN_STEEL_EARLY_START_DATE = 0;
	m_PLAN_STEEL_EARLY_START_TIME = 0;
	m_PLAN_STEEL_LATEST_END_DATE = 0;
	m_PLAN_STEEL_LATEST_END_TIME = 0;
	m_PLAN_HEAT_PROD_TIME_MINUTES = 0;
	m_PLAN_HEAT_DISPOSITION_CODE = 0;
	m_PLAN_HEAT_BACKUP_MOLD_CODE = 0;
	m_PLAN_HEAT_CARBON_MIN = 0.0f;
	m_PLAN_HEAT_CARBON_MAX = 0.0f;
	m_PLAN_HEAT_MANGANESE_MIN = 0.0f;
	m_PLAN_HEAT_MANGANESE_MAX = 0.0f;
	m_PLAN_HEAT_STATUS_CODE = _T("");
	m_PLAN_LINEUP_NUMBER = 0;
	m_PLAN_LINEUP_ITEM_SEQ_NO = 0;
	m_PLAN_LINEUP_COMMENTS = _T("");
	m_PLAN_910_REPORT_IND = _T("");
	m_PLAN_LOT_NUMBER = 0;
	m_MILL_ORDER_NO = 0;
	m_PLAN_STEEL_WIDTH = 0.0f;
	m_PLAN_STEEL_THICKNESS = 0.0f;
	m_PLAN_STEEL_LENGTH = 0.0f;
	m_PLAN_STEEL_WGT_TONS = 0.0f;
	m_PLAN_NUMBER_OF_PIECES = 0;
	m_PLAN_LOT_SPEC = _T("");
	m_PLAN_LOT_DISP_CODE = 0;
	m_PLAN_LOT_TYPE_SLAB = _T("");
	m_PLAN_SLAB_CONDITION_CD = 0;
	m_PLAN_EXPOSURE_CODE = _T("");
	m_PLAN_HOT_MILL_CD = 0;
	m_PLAN_CASTER_STRAND_NO = _T("");
	m_PLAN_CHANGE_INDICATOR = _T("");
	m_PLAN_ORIGINAL_PLAN_ID = _T("");
	m_PLAN_EXPECTED_APPLIED_SPEC = _T("");
	m_PLAN_EXPECTED_APPLIED_COND_CD = 0;
	m_PLAN_TO_BOF_DATE = 0;
	m_PLAN_LOT_TYPE_SLIT_CODE = _T("");
	m_PROV_SLABS_DUE = 0;
	m_PROV_SLAB_WGT_DUE = 0;
	m_PLAN_909_LOCK_IND = _T("");
	m_PLAN_STEEL_LENGTH_MIN = 0.0f;
	m_PLAN_STEEL_LENGTH_MAX = 0.0f;
	m_FILLER0 = _T("");
	m_FP_M_ORDER_NO = _T("");
	m_nFields = 53;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;

	/////////////////////////////BEGIN///////////////////////////////////
//  SQL Order By : m_strSort added 6-13-03 K. Hubbard to keep heats returned to Editor for Singl String Builder
//                 properly sequenced when displayed in editor. Traced (searched) executeSQL statement definition into 
//                 MFC\Include\AFXDB.h file to find Order By C++ Wizard syntax statement to use.  	

//	m_strFilter = "[PLAN_CAST_UNIT_CODE] = ? AND [PLAN_NUMBER_OF_PIECES] > 0";
	m_strSort =  "[PLAN_CAST_UNIT_CODE], "
				 "[PLAN_WEEK], "
				 "[PLAN_STRING_ID], "
				 "[PLAN_HEAT_SEQUENCE_NO], "
   				 "[PLAN_LOT_NUMBER]";

//	m_nParams = 1;

//
//////////////////////////////END////////////////////////////////////

}


CString CStringBuilderOutputSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=dpaTest");
}

CString CStringBuilderOutputSet::GetDefaultSQL()
{
	return _T("[StringBuilderOutput]");
}

void CStringBuilderOutputSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStringBuilderOutputSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
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
	RFX_Text(pFX, _T("[FP_M_ORDER_NO]"), m_FP_M_ORDER_NO);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStringBuilderOutputSet diagnostics

#ifdef _DEBUG
void CStringBuilderOutputSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStringBuilderOutputSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
