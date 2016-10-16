// UnappOrderSet.cpp : implementation file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function   :  This program reads in orders (database field exchange) unapplied orders, developed from the Unapplied Orders query or
//                SQL View which is very important in displaying order field data associated with open orders. 
//                table.      
//	Maintenance:
//
//  k. hubbard  11-11-03: Added these 2 fields via MFC Wizard for reprovide limits request m_MORD_PROV_CAST_SL_WTH_MAX 
//	                      m_MORD_PROV_CAST_SL_WTH_MIN  Used MFC Class Wizard's, Member Variables, Update Columns button linked to 
//                        OrdersForDailyPlanOnly fields in newinl via ODBC path. per P. Fronczek
//
//  k. hubbard  12-1-05:  Added critical customer field population and identification from p-mill-order. see comments 12-1-05 k.hubbard
//
//  k. hubbard  01-24-07: Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers and release of 
//	                      CI code = 86 identification. Used MFC Class Wizard's, Member Variables, Update Columns button linked to 
//                        OrdersForDailyPlanOnly VIEW fields in newinl via ODBC path. per T. Fronczek
//
//  k. hubbard  05-20-08: Added these 2 fields via MFC Wizard for material direction limits request m_HEAT_SPEC and m_INCLUSION_MODIFIED 
//	                      Used MFC Class Wizard's, Update Columns and Bind All buttons linked to 
//                        OrdersForDailyPlanOnly fields in newinl via ODBC path. per J. Sarb-Beer
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

#include "stdafx.h"

#include "csda.h"
#include "UnappOrderSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnappOrderSet

IMPLEMENT_DYNAMIC(CUnappOrderSet, CRecordset)

CUnappOrderSet::CUnappOrderSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CUnappOrderSet)
	m_HM_PLAN_MEGA_LINEUP_CD = _T("");
	m_MILL_ORDER_NO = 0;
	m_MORD_AUTOMOTIVE_CODE = 0;
	m_MORD_BOOK_MILL = 0;
	m_MORD_CARBON_MAX = 0.0f;
	m_MORD_CARBON_MIN = 0.0f;
	m_MORD_COLD_BAND_WIDTH = 0.0f;
	m_MORD_CUST_COIL_REQ = 0;
	m_MORD_CUST_PIECE_WGT = 0;
	m_MORD_CUSTOMER_CODE = 0;
	m_MORD_CUSTOMER_NAME = _T("");
	m_MORD_DRAWING_CODE = 0;
	m_MORD_GRADE_CODE = 0;
	m_MORD_HEAT_CODE = 0;
	m_MORD_HOT_BAND_GAUGE = 0.0f;
	m_MORD_HOT_BAND_GAUGE_MAX = 0.0f;
	m_MORD_HOT_BAND_GAUGE_MIN = 0.0f;
	m_MORD_HOT_BAND_WIDTH = 0.0f;
	m_MORD_HOT_BAND_WIDTH_MAX = 0.0f;
	m_MORD_HOT_BAND_WIDTH_MIN = 0.0f;
	m_MORD_HOT_ROLL_WEEK = 0;
	m_MORD_HOT_ROLL_YEAR = 0;
	m_MORD_HS_TEMPERATURE = 0;
	m_MORD_HS_TOLERANCE = 0;
	m_MORD_MANGANESE_MAX = 0.0f;
	m_MORD_MANGANESE_MIN = 0.0f;
	m_MORD_MAXIMUM_SLAB_LENGTH = 0.0f;
	m_MORD_MINIMUM_LOAD = 0;
	m_MORD_MINIMUM_SLAB_LENGTH = 0.0f;
	m_MORD_MISC_PROCESS_CD = 0;
	m_MORD_ORDERED_GAUGE = 0.0f;
	m_MORD_ORDERED_LENGTH = 0.0f;
	m_MORD_ORDERED_SPEC = _T("");
	m_MORD_ORDERED_WEIGHT = 0;
	m_MORD_ORDERED_WIDTH = 0.0f;
	m_MORD_PGM_EXPOSURE = 0;
	m_MORD_PGM_PRODUCT_MOD = 0;
	m_MORD_PGM_QUALITY_GRADE = 0;
	m_MORD_PLAN_PRTY_WK_DAY = 0;
	m_MORD_PLAN_PRTY_YR = 0;
	m_MORD_PROCESS_CODE = 0;
	m_MORD_PRODUCT_CMDTY_CD = 0;
	m_MORD_PRODUCT_CODE = 0;
	m_MORD_PROV_80_HB_ING_CNT = 0.0f;
	m_MORD_PROV_80_HB_ING_POUR_WGT = 0;
	m_MORD_PROV_80_HB_ING_SIZE = 0;
	m_MORD_PROV_80_HB_LO_PCT_LIM = 0.0f;
	m_MORD_PROV_80_HB_PROV_TOL = 0;
	m_MORD_PROV_80_HB_SL_CNT = 0;
	m_MORD_PROV_80_HB_SL_LNTH = 0;
	m_MORD_PROV_80_HB_SL_PC_WGT = 0;
	m_MORD_PROV_80_HB_SL_THK = 0.0f;
	m_MORD_PROV_80_HB_SL_THK_MAX = 0.0f;
	m_MORD_PROV_80_HB_SL_THK_MIN = 0.0f;
	m_MORD_PROV_80_HB_SL_WTH = 0.0f;
	m_MORD_PROV_80_HB_YF_ACCUM = 0.0f;
	m_MORD_PROV_80_HB_YF_PROD = 0;
	m_MORD_PROV_80_HB_YLD_FCTR = 0.0f;
	m_MORD_PROV_APPLY_WTH = 0.0f;
	m_MORD_PROV_CAST_MAX_SLAB_WGT = 0;
	m_MORD_PROV_CAST_MIN_SLAB_WGT = 0;
	m_MORD_PROV_CAST_SL_LNTH = 0;
	m_MORD_PROV_CAST_SL_PC_WGT = 0;
	m_MORD_PROV_CAST_SL_THK = 0.0f;
	m_MORD_PROV_CAST_SL_WTH = 0.0f;
	m_MORD_PROV_HR_PLAN_AMT = 0;
	m_MORD_PROV_HR_TOT_PROD = 0;
	m_MORD_PROV_MAX_SLAB_WGT = 0;
	m_MORD_PROV_MIN_SLAB_WGT = 0;
	m_MORD_PROV_PIW_WGT = 0;
	m_MORD_RESTRICTION_CD = _T("");
	m_MORD_ROUTING_CODE = 0;
	m_MORD_SHIP_MILL = 0;
	m_MORD_SIGNIF_EL_MAX = 0.0f;
	m_MORD_SIGNIF_EL_MIN = 0.0f;
	m_MORD_SIGNIF_EL_SYM = _T("");
	m_MORD_SLAB_APPLY_FULL_CD = _T("");
	m_MORD_SLAB_CONDITION_CD = 0;
	m_MORD_STEEL_APPLY_CODE = 0;
	m_MORD_STEEL_TYPE_CODE = 0;
	m_MORD_TEMPER_ROLL_CD = 0;
	m_PROV_ALT_ING_SIZE_CD = 0;
	m_PROV_HB_YLD_FCTR = 0.0f;
	m_PROV_INDV_SLAB_WGT = 0;
	m_PROV_ING_POUR_WGT = 0;
	m_PROV_ING_SIZE_CD = 0;
	m_PROV_INGOTS_DUE = 0.0f;
	m_PROV_NO_SLABS_RLD = 0;
	m_PROV_SLAB_LENGTH = 0;
	m_PROV_SLAB_THICKNESS = 0.0f;
	m_PROV_SLAB_WGT_DUE = 0;
	m_PROV_SLAB_WIDTH = 0.0f;
	m_PROV_SLAB_YIELD = 0.0f;
	m_PROV_SLABS_DUE = 0;
	m_SL_APPLY_909_PLAN_HB_WGT = 0.0f;
	m_SL_APPLY_910_PLAN_HB_WGT = 0.0f;
	m_SL_APPLY_910_PLAN_INGOT_HB_WGT = 0.0f;
	m_SL_APPLY_911_PLAN_HB_WGT = 0.0f;
	m_SL_APPLY_COND_LEVEL = 0;
	m_SM_PLAN_CAST_SPEC = _T("");
	m_SM_PLAN_DEMAND_DUE_IND = _T("");
	m_SM_PLAN_FACL_PREF_1_CD = _T("");
	m_SM_PLAN_FACL_PREF_2_CD = _T("");
	m_SM_PLAN_FACL_PREF_3_CD = _T("");
	m_SM_PLAN_FACL_PREF_4_CD = _T("");
	m_SM_PLAN_ON_909_IND = _T("");
	m_SM_PLAN_ON_910_IND = _T("");
	m_SM_PLAN_SLAB_UNIT_CD = _T("");
	m_SM_PLAN_SLABS_ON_909 = 0;
	m_SM_PLAN_SLABS_TO_PLAN_IND = _T("");
	m_SM_PLAN_STEEL_SPEC = _T("");
	m_SL_APPLY_NEW_ORDER_CODE = _T("");
	m_MORD_CMS_YIELD_LOSS_FACTOR = 0.0f;
	m_MORD_PGM_PRODUCT = 0;
	m_HM_PLAN_HOT_CHARGE_IND = _T("");
	m_PLAN_910_NO_SL_ORD = 0;
	m_SL_APPLY_910_PLAN_SL_WGT = 0.0f;
	m_HB_TO_SLAB_YIELD = 0.0f;
	m_SM_PLAN_FP_PST_DATE2 = 0;
	m_SM_PLAN_FP_PST_TIME2 = 0;
	m_PROV_HB_PROV_TOL = 0;
	m_FP_ISHRO_PRTY = 0;
	m_FP_LPST_DATE = 0;
	m_FP_M_ORDER_NO = _T("");
	m_FP_MELT_PST_DATE = 0;
	m_FP_MTO_FLOW_ORD_IND = _T("");
	m_FP_PLAN_NO_PCS = 0;
	m_FP_CAST_DEMAND_YR_WK = 0;     // Added 1 field 2-08-05 k. hubbard
	m_FP_PLAN_SLAB_WGT = 0;
	m_FP_TMSTP_DATE_CHGE = 0;
	m_FP_TMSTP_TIME_CHGE = 0;
	m_MORD_PROV_CAST_SL_WTH_MAX = 0.0f;  // Added 2 fields k. hubbard 11-11-03
	m_MORD_PROV_CAST_SL_WTH_MIN = 0.0f;
	m_MORD_CRITICAL_CUST_IND = _T("");  // critical cust req. added 12-1-05 k. hubbard
	m_MORD_HOTROLL_UNIT_NAME = _T("");  // Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers 1-24-07 k. hubbard
	m_HEAT_SPEC = _T("");
	m_INCLUSION_MODIFIED = _T("");
	m_MORD_IH_CAST_UNIT_NAME = _T("");
	m_MORD_AXIOM_ORDER = _T("");
	m_MORD_SMK_SPEC_WEST = _T("");
	m_MORD_SMK_COND_WEST = _T("");
	m_nFields = 141;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CUnappOrderSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CUnappOrderSet::GetDefaultSQL()
{
	return _T("[unappliedorders]");
}

void CUnappOrderSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CUnappOrderSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[HM_PLAN_MEGA_LINEUP_CD]"), m_HM_PLAN_MEGA_LINEUP_CD);
	RFX_Long(pFX, _T("[MILL_ORDER_NO]"), m_MILL_ORDER_NO);
	RFX_Long(pFX, _T("[MORD_AUTOMOTIVE_CODE]"), m_MORD_AUTOMOTIVE_CODE);
	RFX_Long(pFX, _T("[MORD_BOOK_MILL]"), m_MORD_BOOK_MILL);
	RFX_Single(pFX, _T("[MORD_CARBON_MAX]"), m_MORD_CARBON_MAX);
	RFX_Single(pFX, _T("[MORD_CARBON_MIN]"), m_MORD_CARBON_MIN);
	RFX_Single(pFX, _T("[MORD_COLD_BAND_WIDTH]"), m_MORD_COLD_BAND_WIDTH);
	RFX_Long(pFX, _T("[MORD_CUST_COIL_REQ]"), m_MORD_CUST_COIL_REQ);
	RFX_Long(pFX, _T("[MORD_CUST_PIECE_WGT]"), m_MORD_CUST_PIECE_WGT);
	RFX_Long(pFX, _T("[MORD_CUSTOMER_CODE]"), m_MORD_CUSTOMER_CODE);
	RFX_Text(pFX, _T("[MORD_CUSTOMER_NAME]"), m_MORD_CUSTOMER_NAME);
	RFX_Long(pFX, _T("[MORD_DRAWING_CODE]"), m_MORD_DRAWING_CODE);
	RFX_Long(pFX, _T("[MORD_GRADE_CODE]"), m_MORD_GRADE_CODE);
	RFX_Long(pFX, _T("[MORD_HEAT_CODE]"), m_MORD_HEAT_CODE);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE]"), m_MORD_HOT_BAND_GAUGE);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MAX]"), m_MORD_HOT_BAND_GAUGE_MAX);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_GAUGE_MIN]"), m_MORD_HOT_BAND_GAUGE_MIN);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH]"), m_MORD_HOT_BAND_WIDTH);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MAX]"), m_MORD_HOT_BAND_WIDTH_MAX);
	RFX_Single(pFX, _T("[MORD_HOT_BAND_WIDTH_MIN]"), m_MORD_HOT_BAND_WIDTH_MIN);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_WEEK]"), m_MORD_HOT_ROLL_WEEK);
	RFX_Long(pFX, _T("[MORD_HOT_ROLL_YEAR]"), m_MORD_HOT_ROLL_YEAR);
	RFX_Long(pFX, _T("[MORD_HS_TEMPERATURE]"), m_MORD_HS_TEMPERATURE);
	RFX_Long(pFX, _T("[MORD_HS_TOLERANCE]"), m_MORD_HS_TOLERANCE);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MAX]"), m_MORD_MANGANESE_MAX);
	RFX_Single(pFX, _T("[MORD_MANGANESE_MIN]"), m_MORD_MANGANESE_MIN);
	RFX_Single(pFX, _T("[MORD_MAXIMUM_SLAB_LENGTH]"), m_MORD_MAXIMUM_SLAB_LENGTH);
	RFX_Long(pFX, _T("[MORD_MINIMUM_LOAD]"), m_MORD_MINIMUM_LOAD);
	RFX_Single(pFX, _T("[MORD_MINIMUM_SLAB_LENGTH]"), m_MORD_MINIMUM_SLAB_LENGTH);
	RFX_Long(pFX, _T("[MORD_MISC_PROCESS_CD]"), m_MORD_MISC_PROCESS_CD);
	RFX_Single(pFX, _T("[MORD_ORDERED_GAUGE]"), m_MORD_ORDERED_GAUGE);
	RFX_Single(pFX, _T("[MORD_ORDERED_LENGTH]"), m_MORD_ORDERED_LENGTH);
	RFX_Text(pFX, _T("[MORD_ORDERED_SPEC]"), m_MORD_ORDERED_SPEC);
	RFX_Long(pFX, _T("[MORD_ORDERED_WEIGHT]"), m_MORD_ORDERED_WEIGHT);
	RFX_Single(pFX, _T("[MORD_ORDERED_WIDTH]"), m_MORD_ORDERED_WIDTH);
	RFX_Long(pFX, _T("[MORD_PGM_EXPOSURE]"), m_MORD_PGM_EXPOSURE);
	RFX_Long(pFX, _T("[MORD_PGM_PRODUCT_MOD]"), m_MORD_PGM_PRODUCT_MOD);
	RFX_Long(pFX, _T("[MORD_PGM_QUALITY_GRADE]"), m_MORD_PGM_QUALITY_GRADE);
	RFX_Long(pFX, _T("[MORD_PLAN_PRTY_WK_DAY]"), m_MORD_PLAN_PRTY_WK_DAY);
	RFX_Long(pFX, _T("[MORD_PLAN_PRTY_YR]"), m_MORD_PLAN_PRTY_YR);
	RFX_Long(pFX, _T("[MORD_PROCESS_CODE]"), m_MORD_PROCESS_CODE);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CMDTY_CD]"), m_MORD_PRODUCT_CMDTY_CD);
	RFX_Long(pFX, _T("[MORD_PRODUCT_CODE]"), m_MORD_PRODUCT_CODE);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_ING_CNT]"), m_MORD_PROV_80_HB_ING_CNT);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_ING_POUR_WGT]"), m_MORD_PROV_80_HB_ING_POUR_WGT);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_ING_SIZE]"), m_MORD_PROV_80_HB_ING_SIZE);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_LO_PCT_LIM]"), m_MORD_PROV_80_HB_LO_PCT_LIM);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_PROV_TOL]"), m_MORD_PROV_80_HB_PROV_TOL);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_SL_CNT]"), m_MORD_PROV_80_HB_SL_CNT);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_SL_LNTH]"), m_MORD_PROV_80_HB_SL_LNTH);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_SL_PC_WGT]"), m_MORD_PROV_80_HB_SL_PC_WGT);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_SL_THK]"), m_MORD_PROV_80_HB_SL_THK);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_SL_THK_MAX]"), m_MORD_PROV_80_HB_SL_THK_MAX);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_SL_THK_MIN]"), m_MORD_PROV_80_HB_SL_THK_MIN);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_SL_WTH]"), m_MORD_PROV_80_HB_SL_WTH);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_YF_ACCUM]"), m_MORD_PROV_80_HB_YF_ACCUM);
	RFX_Long(pFX, _T("[MORD_PROV_80_HB_YF_PROD]"), m_MORD_PROV_80_HB_YF_PROD);
	RFX_Single(pFX, _T("[MORD_PROV_80_HB_YLD_FCTR]"), m_MORD_PROV_80_HB_YLD_FCTR);
	RFX_Single(pFX, _T("[MORD_PROV_APPLY_WTH]"), m_MORD_PROV_APPLY_WTH);
	RFX_Long(pFX, _T("[MORD_PROV_CAST_MAX_SLAB_WGT]"), m_MORD_PROV_CAST_MAX_SLAB_WGT);
	RFX_Long(pFX, _T("[MORD_PROV_CAST_MIN_SLAB_WGT]"), m_MORD_PROV_CAST_MIN_SLAB_WGT);
	RFX_Long(pFX, _T("[MORD_PROV_CAST_SL_LNTH]"), m_MORD_PROV_CAST_SL_LNTH);
	RFX_Long(pFX, _T("[MORD_PROV_CAST_SL_PC_WGT]"), m_MORD_PROV_CAST_SL_PC_WGT);
	RFX_Single(pFX, _T("[MORD_PROV_CAST_SL_THK]"), m_MORD_PROV_CAST_SL_THK);
	RFX_Single(pFX, _T("[MORD_PROV_CAST_SL_WTH]"), m_MORD_PROV_CAST_SL_WTH);
	RFX_Long(pFX, _T("[MORD_PROV_HR_PLAN_AMT]"), m_MORD_PROV_HR_PLAN_AMT);
	RFX_Long(pFX, _T("[MORD_PROV_HR_TOT_PROD]"), m_MORD_PROV_HR_TOT_PROD);
	RFX_Long(pFX, _T("[MORD_PROV_MAX_SLAB_WGT]"), m_MORD_PROV_MAX_SLAB_WGT);
	RFX_Long(pFX, _T("[MORD_PROV_MIN_SLAB_WGT]"), m_MORD_PROV_MIN_SLAB_WGT);
	RFX_Long(pFX, _T("[MORD_PROV_PIW_WGT]"), m_MORD_PROV_PIW_WGT);
	RFX_Text(pFX, _T("[MORD_RESTRICTION_CD]"), m_MORD_RESTRICTION_CD);
	RFX_Long(pFX, _T("[MORD_ROUTING_CODE]"), m_MORD_ROUTING_CODE);
	RFX_Long(pFX, _T("[MORD_SHIP_MILL]"), m_MORD_SHIP_MILL);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MAX]"), m_MORD_SIGNIF_EL_MAX);
	RFX_Single(pFX, _T("[MORD_SIGNIF_EL_MIN]"), m_MORD_SIGNIF_EL_MIN);
	RFX_Text(pFX, _T("[MORD_SIGNIF_EL_SYM]"), m_MORD_SIGNIF_EL_SYM);
	RFX_Text(pFX, _T("[MORD_SLAB_APPLY_FULL_CD]"), m_MORD_SLAB_APPLY_FULL_CD);
	RFX_Long(pFX, _T("[MORD_SLAB_CONDITION_CD]"), m_MORD_SLAB_CONDITION_CD);
	RFX_Long(pFX, _T("[MORD_STEEL_APPLY_CODE]"), m_MORD_STEEL_APPLY_CODE);
	RFX_Long(pFX, _T("[MORD_STEEL_TYPE_CODE]"), m_MORD_STEEL_TYPE_CODE);
	RFX_Long(pFX, _T("[MORD_TEMPER_ROLL_CD]"), m_MORD_TEMPER_ROLL_CD);
	RFX_Long(pFX, _T("[PROV_ALT_ING_SIZE_CD]"), m_PROV_ALT_ING_SIZE_CD);
	RFX_Single(pFX, _T("[PROV_HB_YLD_FCTR]"), m_PROV_HB_YLD_FCTR);
	RFX_Long(pFX, _T("[PROV_INDV_SLAB_WGT]"), m_PROV_INDV_SLAB_WGT);
	RFX_Long(pFX, _T("[PROV_ING_POUR_WGT]"), m_PROV_ING_POUR_WGT);
	RFX_Long(pFX, _T("[PROV_ING_SIZE_CD]"), m_PROV_ING_SIZE_CD);
	RFX_Single(pFX, _T("[PROV_INGOTS_DUE]"), m_PROV_INGOTS_DUE);
	RFX_Long(pFX, _T("[PROV_NO_SLABS_RLD]"), m_PROV_NO_SLABS_RLD);
	RFX_Long(pFX, _T("[PROV_SLAB_LENGTH]"), m_PROV_SLAB_LENGTH);
	RFX_Single(pFX, _T("[PROV_SLAB_THICKNESS]"), m_PROV_SLAB_THICKNESS);
	RFX_Long(pFX, _T("[PROV_SLAB_WGT_DUE]"), m_PROV_SLAB_WGT_DUE);
	RFX_Single(pFX, _T("[PROV_SLAB_WIDTH]"), m_PROV_SLAB_WIDTH);
	RFX_Single(pFX, _T("[PROV_SLAB_YIELD]"), m_PROV_SLAB_YIELD);
	RFX_Long(pFX, _T("[PROV_SLABS_DUE]"), m_PROV_SLABS_DUE);
	RFX_Single(pFX, _T("[SL_APPLY_909_PLAN_HB_WGT]"), m_SL_APPLY_909_PLAN_HB_WGT);
	RFX_Single(pFX, _T("[SL_APPLY_910_PLAN_HB_WGT]"), m_SL_APPLY_910_PLAN_HB_WGT);
	RFX_Single(pFX, _T("[SL_APPLY_910_PLAN_INGOT_HB_WGT]"), m_SL_APPLY_910_PLAN_INGOT_HB_WGT);
	RFX_Single(pFX, _T("[SL_APPLY_911_PLAN_HB_WGT]"), m_SL_APPLY_911_PLAN_HB_WGT);
	RFX_Long(pFX, _T("[SL_APPLY_COND_LEVEL]"), m_SL_APPLY_COND_LEVEL);
	RFX_Text(pFX, _T("[SM_PLAN_CAST_SPEC]"), m_SM_PLAN_CAST_SPEC);
	RFX_Text(pFX, _T("[SM_PLAN_DEMAND_DUE_IND]"), m_SM_PLAN_DEMAND_DUE_IND);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_1_CD]"), m_SM_PLAN_FACL_PREF_1_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_2_CD]"), m_SM_PLAN_FACL_PREF_2_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_3_CD]"), m_SM_PLAN_FACL_PREF_3_CD);
	RFX_Text(pFX, _T("[SM_PLAN_FACL_PREF_4_CD]"), m_SM_PLAN_FACL_PREF_4_CD);
	RFX_Text(pFX, _T("[SM_PLAN_ON_909_IND]"), m_SM_PLAN_ON_909_IND);
	RFX_Text(pFX, _T("[SM_PLAN_ON_910_IND]"), m_SM_PLAN_ON_910_IND);
	RFX_Text(pFX, _T("[SM_PLAN_SLAB_UNIT_CD]"), m_SM_PLAN_SLAB_UNIT_CD);
	RFX_Long(pFX, _T("[SM_PLAN_SLABS_ON_909]"), m_SM_PLAN_SLABS_ON_909);
	RFX_Text(pFX, _T("[SM_PLAN_SLABS_TO_PLAN_IND]"), m_SM_PLAN_SLABS_TO_PLAN_IND);
	RFX_Text(pFX, _T("[SM_PLAN_STEEL_SPEC]"), m_SM_PLAN_STEEL_SPEC);
	RFX_Text(pFX, _T("[SL_APPLY_NEW_ORDER_CODE]"), m_SL_APPLY_NEW_ORDER_CODE);
	RFX_Single(pFX, _T("[MORD_CMS_YIELD_LOSS_FACTOR]"), m_MORD_CMS_YIELD_LOSS_FACTOR);
	RFX_Long(pFX, _T("[MORD_PGM_PRODUCT]"), m_MORD_PGM_PRODUCT);
	RFX_Text(pFX, _T("[HM_PLAN_HOT_CHARGE_IND]"), m_HM_PLAN_HOT_CHARGE_IND);
	RFX_Long(pFX, _T("[PLAN_910_NO_SL_ORD]"), m_PLAN_910_NO_SL_ORD);
	RFX_Single(pFX, _T("[SL_APPLY_910_PLAN_SL_WGT]"), m_SL_APPLY_910_PLAN_SL_WGT);
	RFX_Single(pFX, _T("[HB_TO_SLAB_YIELD]"), m_HB_TO_SLAB_YIELD);
	RFX_Long(pFX, _T("[SM-PLAN-FP-PST-DATE]"), m_SM_PLAN_FP_PST_DATE2);
	RFX_Long(pFX, _T("[SM-PLAN-FP-PST-TIME]"), m_SM_PLAN_FP_PST_TIME2);
	RFX_Long(pFX, _T("[PROV_HB_PROV_TOL]"), m_PROV_HB_PROV_TOL);
	RFX_Int(pFX, _T("[FP_ISHRO_PRTY]"), m_FP_ISHRO_PRTY);
	RFX_Long(pFX, _T("[FP_LPST_DATE]"), m_FP_LPST_DATE);
	RFX_Text(pFX, _T("[FP_M_ORDER_NO]"), m_FP_M_ORDER_NO);
	RFX_Long(pFX, _T("[FP_MELT_PST_DATE]"), m_FP_MELT_PST_DATE);
	RFX_Text(pFX, _T("[FP_MTO_FLOW_ORD_IND]"), m_FP_MTO_FLOW_ORD_IND);
	RFX_Long(pFX, _T("[FP_PLAN_NO_PCS]"), m_FP_PLAN_NO_PCS);
	RFX_Long(pFX, _T("[FP_CAST_DEMAND_YR_WK]"), m_FP_CAST_DEMAND_YR_WK);   // Added 1 field k. hubbard 2-08-05
	RFX_Long(pFX, _T("[FP_PLAN_SLAB_WGT]"), m_FP_PLAN_SLAB_WGT);
	RFX_Long(pFX, _T("[FP_TMSTP_DATE_CHGE]"), m_FP_TMSTP_DATE_CHGE);
	RFX_Long(pFX, _T("[FP_TMSTP_TIME_CHGE]"), m_FP_TMSTP_TIME_CHGE);
	RFX_Single(pFX, _T("[MORD_PROV_CAST_SL_WTH_MAX]"), m_MORD_PROV_CAST_SL_WTH_MAX);  // Added 2 fields k. hubbard 11-11-03
	RFX_Single(pFX, _T("[MORD_PROV_CAST_SL_WTH_MIN]"), m_MORD_PROV_CAST_SL_WTH_MIN);
	RFX_Text(pFX, _T("[MORD_CRITICAL_CUSTOMER_IND]"), m_MORD_CRITICAL_CUST_IND);  // critical cust req. added 12-1-05 k. hubbard
	RFX_Text(pFX, _T("[MORD_HOTROLL_UNIT_NAME]"), m_MORD_HOTROLL_UNIT_NAME); // Added m_MORD_HOTROLL_UNIT_NAME field via MFC Wizard for use in identification of Slab Sale Customers 1-24-07 k. hubbard
	RFX_Text(pFX, _T("[HEAT_SPEC]"), m_HEAT_SPEC);
	RFX_Text(pFX, _T("[INCLUSION_MODIFIED]"), m_INCLUSION_MODIFIED);
	RFX_Text(pFX, _T("[MORD_IH_CAST_UNIT_NAME]"), m_MORD_IH_CAST_UNIT_NAME);
	RFX_Text(pFX, _T("[MORD_AXIOM_ORDER]"), m_MORD_AXIOM_ORDER);
	RFX_Text(pFX, _T("[MORD_SMK_SPEC_WEST]"), m_MORD_SMK_SPEC_WEST);
	RFX_Text(pFX, _T("[MORD_SMK_COND_WEST]"), m_MORD_SMK_COND_WEST);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CUnappOrderSet diagnostics

#ifdef _DEBUG
void CUnappOrderSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CUnappOrderSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
