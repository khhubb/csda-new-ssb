// DalyPlanSet.cpp : implementation file
//
//  K. Hubbard  05-5-09   Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                        modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                        controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#include "csda.h"
#include "DalyPlanSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDalyPlanSet

IMPLEMENT_DYNAMIC(CDalyPlanSet, CRecordset)

CDalyPlanSet::CDalyPlanSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDalyPlanSet)
	m_MILL_ORDER_NO = 0;
	m_PLAN_909_LOCK_IND = _T("");
	m_PLAN_910_REPORT_IND = _T("");
	m_PLAN_ADJUSTMENT_KEY = _T("");
	m_PLAN_CAST_SEQUENCE_NO = 0;
	m_PLAN_CAST_TAB_STATUS_KEY = _T("");
	m_PLAN_CAST_UNIT_CODE = _T("");
	m_PLAN_CAST_UNIT_HEAT_SEQ_KEY = _T("");
	m_PLAN_CAST_UNIT_ORDER_KEY = _T("");
	m_PLAN_CASTER_STRAND_NO = _T("");
	m_PLAN_CHANGE_INDICATOR = _T("");
	m_PLAN_EXPECTED_APPLIED_COND_CD = 0;
	m_PLAN_EXPECTED_APPLIED_SPEC = _T("");
	m_PLAN_EXPOSURE_CODE = _T("");
	m_PLAN_HEAT_BACKUP_MOLD_CODE = 0;
	m_PLAN_HEAT_CARBON_MAX = 0.0f;
	m_PLAN_HEAT_CARBON_MIN = 0.0f;
	m_PLAN_HEAT_DISPOSITION_CODE = 0;
	m_PLAN_HEAT_GROUP = 0;
	m_PLAN_HEAT_MANGANESE_MAX = 0.0f;
	m_PLAN_HEAT_MANGANESE_MIN = 0.0f;
	m_PLAN_HEAT_PROD_TIME_MINUTES = 0;
	m_PLAN_HEAT_SEQUENCE_NO = 0;
	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC = 0;
	m_PLAN_HEAT_SIXTH_DIGIT_SPEC = _T("");
	m_PLAN_HEAT_SPEC = 0;
	m_PLAN_HEAT_STATUS_CODE = _T("");
	m_PLAN_HEAT_TAB_NUMBER = 0;
	m_PLAN_HOT_MILL_CD = 0;
	m_PLAN_LINEUP_COMMENTS = _T("");
	m_PLAN_LINEUP_ITEM_SEQ_NO = 0;
	m_PLAN_LINEUP_NUMBER = 0;
	m_PLAN_LOT_DISP_CODE = 0;
	m_PLAN_LOT_NUMBER = 0;
	m_PLAN_LOT_SPEC = _T("");
	m_PLAN_LOT_TYPE_SLAB = _T("");
	m_PLAN_LOT_TYPE_SLIT_CODE = _T("");
	m_PLAN_MILL_ORDER_KEY = _T("");
	m_PLAN_NUMBER_OF_PIECES = 0;
	m_PLAN_ORIGINAL_PLAN_ID = _T("");
	m_PLAN_SHOP_SCHED_DATE = 0;
	m_PLAN_SHOP_SCHED_KEY = _T("");
	m_PLAN_SHOP_SCHED_TIME = 0;
	m_PLAN_SLAB_CONDITION_CD = 0;
	m_PLAN_STEEL_EARLY_START_DATE = 0;
	m_PLAN_STEEL_EARLY_START_TIME = 0;
	m_PLAN_STEEL_LATEST_END_DATE = 0;
	m_PLAN_STEEL_LATEST_END_TIME = 0;
	m_PLAN_STEEL_LENGTH = 0.0f;
	m_PLAN_STEEL_LENGTH_MAX = 0.0f;
	m_PLAN_STEEL_LENGTH_MIN = 0.0f;
	m_PLAN_STEEL_THICKNESS = 0.0f;
	m_PLAN_STEEL_WGT_TONS = 0.0f;
	m_PLAN_STEEL_WIDTH = 0.0f;
	m_PLAN_STRING_ID = 0;
	m_PLAN_UNIT_HT_STATUS_TAB_KEY = _T("");
	m_PLAN_WEEK = 0;
	m_SLIT_REASON = _T("");    // added reasons 2-26-10 k. hubbard
	m_STOCK_REASON = _T("");   // added reasons 5-5-09 k. hubbard
	m_STOCK_COMMERCIAL_REASON = _T("");   // added reasons 5-5-09 k. hubbard
	m_nFields = 60;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;


/////////////////////////////BEGIN///////////////////////////////////
//

	m_strFilter = "[PLAN_CAST_UNIT_CODE] = ? AND [PLAN_NUMBER_OF_PIECES] > 0";
	m_strSort =  "[PLAN_CAST_UNIT_CODE], "
				 "[PLAN_WEEK], "
				 "[PLAN_STRING_ID], "
				 "[PLAN_HEAT_SEQUENCE_NO], "
   				 "[PLAN_LOT_NUMBER]";

	m_nParams = 1;

//
//////////////////////////////END////////////////////////////////////

}


CString CDalyPlanSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CDalyPlanSet::GetDefaultSQL()
{
	return _T("[P_STLMAKE_DAILY_PLAN]");
}

void CDalyPlanSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDalyPlanSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Text(pFX, _T("[PLAN_909_LOCK_IND]"), m_PLAN_909_LOCK_IND);
	RFX_Text(pFX, _T("[PLAN_910_REPORT_IND]"), m_PLAN_910_REPORT_IND);
	RFX_Text(pFX, _T("[PLAN_ADJUSTMENT_KEY]"), m_PLAN_ADJUSTMENT_KEY);
	RFX_Long(pFX, _T("[PLAN_CAST_SEQUENCE_NO]"), m_PLAN_CAST_SEQUENCE_NO);
	RFX_Text(pFX, _T("[PLAN_CAST_TAB_STATUS_KEY]"), m_PLAN_CAST_TAB_STATUS_KEY);
	RFX_Text(pFX, _T("[PLAN_CAST_UNIT_CODE]"), m_PLAN_CAST_UNIT_CODE);
	RFX_Text(pFX, _T("[PLAN_CAST_UNIT_HEAT_SEQ_KEY]"), m_PLAN_CAST_UNIT_HEAT_SEQ_KEY);
	RFX_Text(pFX, _T("[PLAN_CAST_UNIT_ORDER_KEY]"), m_PLAN_CAST_UNIT_ORDER_KEY);
	RFX_Text(pFX, _T("[PLAN_CASTER_STRAND_NO]"), m_PLAN_CASTER_STRAND_NO);
	RFX_Text(pFX, _T("[PLAN_CHANGE_INDICATOR]"), m_PLAN_CHANGE_INDICATOR);
	RFX_Long(pFX, _T("[PLAN_EXPECTED_APPLIED_COND_CD]"), m_PLAN_EXPECTED_APPLIED_COND_CD);
	RFX_Text(pFX, _T("[PLAN_EXPECTED_APPLIED_SPEC]"), m_PLAN_EXPECTED_APPLIED_SPEC);
	RFX_Text(pFX, _T("[PLAN_EXPOSURE_CODE]"), m_PLAN_EXPOSURE_CODE);
	RFX_Long(pFX, _T("[PLAN_HEAT_BACKUP_MOLD_CODE]"), m_PLAN_HEAT_BACKUP_MOLD_CODE);
	RFX_Single(pFX, _T("[PLAN_HEAT_CARBON_MAX]"), m_PLAN_HEAT_CARBON_MAX);
	RFX_Single(pFX, _T("[PLAN_HEAT_CARBON_MIN]"), m_PLAN_HEAT_CARBON_MIN);
	RFX_Long(pFX, _T("[PLAN_HEAT_DISPOSITION_CODE]"), m_PLAN_HEAT_DISPOSITION_CODE);
	RFX_Long(pFX, _T("[PLAN_HEAT_GROUP]"), m_PLAN_HEAT_GROUP);
	RFX_Single(pFX, _T("[PLAN_HEAT_MANGANESE_MAX]"), m_PLAN_HEAT_MANGANESE_MAX);
	RFX_Single(pFX, _T("[PLAN_HEAT_MANGANESE_MIN]"), m_PLAN_HEAT_MANGANESE_MIN);
	RFX_Long(pFX, _T("[PLAN_HEAT_PROD_TIME_MINUTES]"), m_PLAN_HEAT_PROD_TIME_MINUTES);
	RFX_Long(pFX, _T("[PLAN_HEAT_SEQUENCE_NO]"), m_PLAN_HEAT_SEQUENCE_NO);
	RFX_Long(pFX, _T("[PLAN_HEAT_SEVENTH_DIGIT_SPEC]"), m_PLAN_HEAT_SEVENTH_DIGIT_SPEC);
	RFX_Text(pFX, _T("[PLAN_HEAT_SIXTH_DIGIT_SPEC]"), m_PLAN_HEAT_SIXTH_DIGIT_SPEC);
	RFX_Long(pFX, _T("[PLAN_HEAT_SPEC]"), m_PLAN_HEAT_SPEC);
	RFX_Text(pFX, _T("[PLAN_HEAT_STATUS_CODE]"), m_PLAN_HEAT_STATUS_CODE);
	RFX_Long(pFX, _T("[PLAN_HEAT_TAB_NUMBER]"), m_PLAN_HEAT_TAB_NUMBER);
	RFX_Long(pFX, _T("[PLAN_HOT_MILL_CD]"), m_PLAN_HOT_MILL_CD);
	RFX_Text(pFX, _T("[PLAN_LINEUP_COMMENTS]"), m_PLAN_LINEUP_COMMENTS);
	RFX_Long(pFX, _T("[PLAN_LINEUP_ITEM_SEQ_NO]"), m_PLAN_LINEUP_ITEM_SEQ_NO);
	RFX_Long(pFX, _T("[PLAN_LINEUP_NUMBER]"), m_PLAN_LINEUP_NUMBER);
	RFX_Long(pFX, _T("[PLAN_LOT_DISP_CODE]"), m_PLAN_LOT_DISP_CODE);
	RFX_Long(pFX, _T("[PLAN_LOT_NUMBER]"), m_PLAN_LOT_NUMBER);
	RFX_Text(pFX, _T("[PLAN_LOT_SPEC]"), m_PLAN_LOT_SPEC);
	RFX_Text(pFX, _T("[PLAN_LOT_TYPE_SLAB]"), m_PLAN_LOT_TYPE_SLAB);
	RFX_Text(pFX, _T("[PLAN_LOT_TYPE_SLIT_CODE]"), m_PLAN_LOT_TYPE_SLIT_CODE);
	RFX_Text(pFX, _T("[PLAN_MILL_ORDER_KEY]"), m_PLAN_MILL_ORDER_KEY);
	RFX_Long(pFX, _T("[PLAN_NUMBER_OF_PIECES]"), m_PLAN_NUMBER_OF_PIECES);
	RFX_Text(pFX, _T("[PLAN_ORIGINAL_PLAN_ID]"), m_PLAN_ORIGINAL_PLAN_ID);
	RFX_Long(pFX, _T("[PLAN_SHOP_SCHED_DATE]"), m_PLAN_SHOP_SCHED_DATE);
	RFX_Text(pFX, _T("[PLAN_SHOP_SCHED_KEY]"), m_PLAN_SHOP_SCHED_KEY);
	RFX_Long(pFX, _T("[PLAN_SHOP_SCHED_TIME]"), m_PLAN_SHOP_SCHED_TIME);
	RFX_Long(pFX, _T("[PLAN_SLAB_CONDITION_CD]"), m_PLAN_SLAB_CONDITION_CD);
	RFX_Long(pFX, _T("[PLAN_STEEL_EARLY_START_DATE]"), m_PLAN_STEEL_EARLY_START_DATE);
	RFX_Long(pFX, _T("[PLAN_STEEL_EARLY_START_TIME]"), m_PLAN_STEEL_EARLY_START_TIME);
	RFX_Long(pFX, _T("[PLAN_STEEL_LATEST_END_DATE]"), m_PLAN_STEEL_LATEST_END_DATE);
	RFX_Long(pFX, _T("[PLAN_STEEL_LATEST_END_TIME]"), m_PLAN_STEEL_LATEST_END_TIME);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH]"), m_PLAN_STEEL_LENGTH);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH_MAX]"), m_PLAN_STEEL_LENGTH_MAX);
	RFX_Single(pFX, _T("[PLAN_STEEL_LENGTH_MIN]"), m_PLAN_STEEL_LENGTH_MIN);
	RFX_Single(pFX, _T("[PLAN_STEEL_THICKNESS]"), m_PLAN_STEEL_THICKNESS);
	RFX_Single(pFX, _T("[PLAN_STEEL_WGT_TONS]"), m_PLAN_STEEL_WGT_TONS);
	RFX_Single(pFX, _T("[PLAN_STEEL_WIDTH]"), m_PLAN_STEEL_WIDTH);
	RFX_Long(pFX, _T("[PLAN_STRING_ID]"), m_PLAN_STRING_ID);
	RFX_Text(pFX, _T("[PLAN_UNIT_HT_STATUS_TAB_KEY]"), m_PLAN_UNIT_HT_STATUS_TAB_KEY);
	RFX_Long(pFX, _T("[PLAN_WEEK]"), m_PLAN_WEEK);
	RFX_Text(pFX, _T("[SLIT_REASON]"), m_SLIT_REASON);      // Added reasons 2-26-10 k. hubbard
	RFX_Text(pFX, _T("[STOCK_REASON]"), m_STOCK_REASON);    // Added reasons 5-5-09 k. hubbard
	RFX_Text(pFX, _T("[STOCK_COMMERCIAL_REASON]"), m_STOCK_COMMERCIAL_REASON);    // Added reasons 5-5-09 k. hubbard
	//}}AFX_FIELD_MAP


	/////////////////////////////BEGIN//////////////////////////////
	//

	pFX->SetFieldType( CFieldExchange::param );				  
	RFX_Text(pFX,"Whatever",castUnitParam);

	//
	//////////////////////////////END///////////////////////////////


}

/////////////////////////////////////////////////////////////////////////////
// CDalyPlanSet diagnostics

#ifdef _DEBUG
void CDalyPlanSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CDalyPlanSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

