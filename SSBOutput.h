// SSBOutput.h : Declaration of the CSSBOutput

#pragma once

// code generated on Sunday, April 24, 2016, 1:26 PM

class CSSBOutput : public CRecordset
{
public:
	CSSBOutput(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSSBOutput)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_PLAN_WEEK;
	CString	m_PLAN_CAST_UNIT_CODE;
	long	m_PLAN_CAST_SEQUENCE_NO;
	long	m_PLAN_STRING_ID;
	long	m_PLAN_HEAT_GROUP;
	long	m_PLAN_HEAT_SEQUENCE_NO;
	long	m_PLAN_HEAT_TAB_NUMBER;
	long	m_PLAN_HEAT_SPEC;
	CString	m_PLAN_HEAT_SIXTH_DIGIT_SPEC;
	long	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC;
	long	m_PLAN_STEEL_EARLY_START_DATE;
	long	m_PLAN_STEEL_EARLY_START_TIME;
	long	m_PLAN_STEEL_LATEST_END_DATE;
	long	m_PLAN_STEEL_LATEST_END_TIME;
	long	m_PLAN_HEAT_PROD_TIME_MINUTES;
	long	m_PLAN_HEAT_DISPOSITION_CODE;
	long	m_PLAN_HEAT_BACKUP_MOLD_CODE;
	float	m_PLAN_HEAT_CARBON_MIN;
	float	m_PLAN_HEAT_CARBON_MAX;
	float	m_PLAN_HEAT_MANGANESE_MIN;
	float	m_PLAN_HEAT_MANGANESE_MAX;
	CString	m_PLAN_HEAT_STATUS_CODE;
	long	m_PLAN_LINEUP_NUMBER;
	long	m_PLAN_LINEUP_ITEM_SEQ_NO;
	CString	m_PLAN_LINEUP_COMMENTS;
	CString	m_PLAN_910_REPORT_IND;
	long	m_PLAN_LOT_NUMBER;
	long	m_MILL_ORDER_NO;
	float	m_PLAN_STEEL_WIDTH;
	float	m_PLAN_STEEL_THICKNESS;
	float	m_PLAN_STEEL_LENGTH;
	float	m_PLAN_STEEL_WGT_TONS;
	long	m_PLAN_NUMBER_OF_PIECES;
	CString	m_PLAN_LOT_SPEC;
	long	m_PLAN_LOT_DISP_CODE;
	CString	m_PLAN_LOT_TYPE_SLAB;
	long	m_PLAN_SLAB_CONDITION_CD;
	CString	m_PLAN_EXPOSURE_CODE;
	long	m_PLAN_HOT_MILL_CD;
	CString	m_PLAN_CASTER_STRAND_NO;
	CString	m_PLAN_CHANGE_INDICATOR;
	CString	m_PLAN_ORIGINAL_PLAN_ID;
	CString	m_PLAN_EXPECTED_APPLIED_SPEC;
	long	m_PLAN_EXPECTED_APPLIED_COND_CD;
	long	m_PLAN_TO_BOF_DATE;
	CString	m_PLAN_LOT_TYPE_SLIT_CODE;
	long	m_PROV_SLABS_DUE;
	long	m_PROV_SLAB_WGT_DUE;
	CString	m_PLAN_909_LOCK_IND;
	float	m_PLAN_STEEL_LENGTH_MIN;
	float	m_PLAN_STEEL_LENGTH_MAX;
	CString	m_FILLER0;
	CString	m_FP_M_ORDER_NO;
	long	m_USER_ID;

	/////////////////////////BEGIN////////////////////////////////
	//

	long m_userIdParam;		// Used to parametrize fetch on userId

	//
	//////////////////////////END/////////////////////////////////

// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


