// DalyPlanSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDalyPlanSet recordset

class CDalyPlanSet : public CRecordset
{
public:
	CDalyPlanSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDalyPlanSet)

// Field/Param Data
	//{{AFX_FIELD(CDalyPlanSet, CRecordset)
	long	m_MILL_ORDER_NO;
	CString	m_PLAN_909_LOCK_IND;
	CString	m_PLAN_910_REPORT_IND;
	CString	m_PLAN_ADJUSTMENT_KEY;
	long	m_PLAN_CAST_SEQUENCE_NO;
	CString	m_PLAN_CAST_TAB_STATUS_KEY;
	CString	m_PLAN_CAST_UNIT_CODE;
	CString	m_PLAN_CAST_UNIT_HEAT_SEQ_KEY;
	CString	m_PLAN_CAST_UNIT_ORDER_KEY;
	CString	m_PLAN_CASTER_STRAND_NO;
	CString	m_PLAN_CHANGE_INDICATOR;
	long	m_PLAN_EXPECTED_APPLIED_COND_CD;
	CString	m_PLAN_EXPECTED_APPLIED_SPEC;
	CString	m_PLAN_EXPOSURE_CODE;
	long	m_PLAN_HEAT_BACKUP_MOLD_CODE;
	float	m_PLAN_HEAT_CARBON_MAX;
	float	m_PLAN_HEAT_CARBON_MIN;
	long	m_PLAN_HEAT_DISPOSITION_CODE;
	long	m_PLAN_HEAT_GROUP;
	float	m_PLAN_HEAT_MANGANESE_MAX;
	float	m_PLAN_HEAT_MANGANESE_MIN;
	long	m_PLAN_HEAT_PROD_TIME_MINUTES;
	long	m_PLAN_HEAT_SEQUENCE_NO;
	long	m_PLAN_HEAT_SEVENTH_DIGIT_SPEC;
	CString	m_PLAN_HEAT_SIXTH_DIGIT_SPEC;
	long	m_PLAN_HEAT_SPEC;
	CString	m_PLAN_HEAT_STATUS_CODE;
	long	m_PLAN_HEAT_TAB_NUMBER;
	long	m_PLAN_HOT_MILL_CD;
	CString	m_PLAN_LINEUP_COMMENTS;
	long	m_PLAN_LINEUP_ITEM_SEQ_NO;
	long	m_PLAN_LINEUP_NUMBER;
	long	m_PLAN_LOT_DISP_CODE;
	long	m_PLAN_LOT_NUMBER;
	CString	m_PLAN_LOT_SPEC;
	CString	m_PLAN_LOT_TYPE_SLAB;
	CString	m_PLAN_LOT_TYPE_SLIT_CODE;
	CString	m_PLAN_MILL_ORDER_KEY;
	long	m_PLAN_NUMBER_OF_PIECES;
	CString	m_PLAN_ORIGINAL_PLAN_ID;
	long	m_PLAN_SHOP_SCHED_DATE;
	CString	m_PLAN_SHOP_SCHED_KEY;
	long	m_PLAN_SHOP_SCHED_TIME;
	long	m_PLAN_SLAB_CONDITION_CD;
	long	m_PLAN_STEEL_EARLY_START_DATE;
	long	m_PLAN_STEEL_EARLY_START_TIME;
	long	m_PLAN_STEEL_LATEST_END_DATE;
	long	m_PLAN_STEEL_LATEST_END_TIME;
	float	m_PLAN_STEEL_LENGTH;
	float	m_PLAN_STEEL_LENGTH_MAX;
	float	m_PLAN_STEEL_LENGTH_MIN;
	float	m_PLAN_STEEL_THICKNESS;
	float	m_PLAN_STEEL_WGT_TONS;
	float	m_PLAN_STEEL_WIDTH;
	long	m_PLAN_STRING_ID;
	CString	m_PLAN_UNIT_HT_STATUS_TAB_KEY;
	long	m_PLAN_WEEK;
	CString m_SLIT_REASON;     // added following 3 reason fields 5-5-09 k. hubbard   
	CString m_STOCK_REASON;
	CString m_STOCK_COMMERCIAL_REASON;
	long m_PLAN_EXPANDED_DISP_CODE;
	//}}AFX_FIELD

	/////////////////////////BEGIN////////////////////////////////
	//

	CString castUnitParam;		// Used to parametrize fetch on caster

	//
	//////////////////////////END/////////////////////////////////



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDalyPlanSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
