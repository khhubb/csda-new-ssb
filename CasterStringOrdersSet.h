#if !defined(AFX_CASTERSTRINGORDERSSET_H__C6D2F26D_46B9_11D1_804A_006097B38214__INCLUDED_)
#define AFX_CASTERSTRINGORDERSSET_H__C6D2F26D_46B9_11D1_804A_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CasterStringOrdersSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCasterStringOrdersSet recordset

class CCasterStringOrdersSet : public CRecordset
{
public:
	CCasterStringOrdersSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCasterStringOrdersSet)

// Field/Param Data
	//{{AFX_FIELD(CCasterStringOrdersSet, CRecordset)
	CString	m_ChangeFlag;
	int		m_CondnCode;
	int		m_DispCode;
	CString	m_ExposureCode;
	int		m_HeatSeqNum;
	CString	m_LotSpec;
	int		m_NumPieces;
	long	m_OrderNum;
	double	m_PieceWgt;
	double	m_PlanSteelLengthMax;
	double	m_PlanSteelLengthMin;
	long	m_ScenId;
	int		m_SeqNum;
	double	m_SlabLength;
	double	m_SlabThickness;
	double	m_SlabWidth;
	CString	m_SlitTypeCode;
	int		m_StrandNum;
	int		m_StringCaster;
	int		m_StringId;
	int		m_StringWeek;
	int		m_StringYear;
	int		m_Version;
	CString	m_FpOrderNum;
	CString	m_StockReason;  // added reason field 5-5-09 k. hubbard
	CString	m_StockCommercialReason;  // added reason field 5-5-09 k. hubbard
	CString	m_SlitReason;   // added reason field 2-26-10 k. hubbard
	//}}AFX_FIELD


	long	m_paramScenId;
	int		m_paramVersion;
	int		m_paramYear;
	int		m_paramWeek;
	int		m_paramCaster;
	int		m_paramStringId;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCasterStringOrdersSet)
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASTERSTRINGORDERSSET_H__C6D2F26D_46B9_11D1_804A_006097B38214__INCLUDED_)
