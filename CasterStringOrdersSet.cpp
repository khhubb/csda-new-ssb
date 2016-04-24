// CasterStringOrdersSet.cpp : implementation file
//
//  K. Hubbard  05-5-09   Added stock facility reason, and stock commercial reason fields that are assigned during creation or
//                        modification of planned stock within the order parameter window. Also this includes addition of 3 new Combobox 
//                        controls and a new color controlled through new (HBrush) paint to toggle background and foreground colors for stock reason fields.       
//    ** Important Scenario Note ** : In order for 910 transmitted or saved data to be available immediately 
//                                    following validation or saves, field must be also present in appropriate
//                                    location in CasterScenArchiver.cpp !!! Also this will update the desired 
//                                    table in CsdaTest database.   
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#include "csda.h"
#include "CasterStringOrdersSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringOrdersSet

IMPLEMENT_DYNAMIC(CCasterStringOrdersSet, CRecordset)

CCasterStringOrdersSet::CCasterStringOrdersSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringOrdersSet)
	m_ChangeFlag = _T("");
	m_CondnCode = 0;
	m_DispCode = 0;
	m_ExposureCode = _T("");
	m_HeatSeqNum = 0;
	m_LotSpec = _T("");
	m_NumPieces = 0;
	m_OrderNum = 0;
	m_PieceWgt = 0.0;
	m_PlanSteelLengthMax = 0.0;
	m_PlanSteelLengthMin = 0.0;
	m_ScenId = 0;
	m_SeqNum = 0;
	m_SlabLength = 0.0;
	m_SlabThickness = 0.0;
	m_SlabWidth = 0.0;
	m_SlitTypeCode = _T("");
	m_StrandNum = 0;
	m_StringCaster = 0;
	m_StringId = 0;
	m_StringWeek = 0;
	m_StringYear = 0;
	m_Version = 0;
	m_FpOrderNum = _T("");
	m_StockReason = _T("");  // added reason field 5-5-09 k. hubbard
	m_StockCommercialReason = _T("");  // added reason field 5-5-09 k. hubbard
	m_SlitReason = _T("");   // added reason field 2-26-10 k. hubbard
	m_nFields = 27;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CCasterStringOrdersSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringOrdersSet::GetDefaultSQL()
{
	return _T("[CasterStringOrders]");
}

void CCasterStringOrdersSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringOrdersSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ChangeFlag]"), m_ChangeFlag);
	RFX_Int(pFX, _T("[CondnCode]"), m_CondnCode);
	RFX_Int(pFX, _T("[DispCode]"), m_DispCode);
	RFX_Text(pFX, _T("[ExposureCode]"), m_ExposureCode);
	RFX_Int(pFX, _T("[HeatSeqNum]"), m_HeatSeqNum);
	RFX_Text(pFX, _T("[LotSpec]"), m_LotSpec);
	RFX_Int(pFX, _T("[NumPieces]"), m_NumPieces);
	RFX_Long(pFX, _T("[OrderNum]"), m_OrderNum);
	RFX_Double(pFX, _T("[PieceWgt]"), m_PieceWgt);
	RFX_Double(pFX, _T("[PlanSteelLengthMax]"), m_PlanSteelLengthMax);
	RFX_Double(pFX, _T("[PlanSteelLengthMin]"), m_PlanSteelLengthMin);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[SeqNum]"), m_SeqNum);
	RFX_Double(pFX, _T("[SlabLength]"), m_SlabLength);
	RFX_Double(pFX, _T("[SlabThickness]"), m_SlabThickness);
	RFX_Double(pFX, _T("[SlabWidth]"), m_SlabWidth);
	RFX_Text(pFX, _T("[SlitTypeCode]"), m_SlitTypeCode);
	RFX_Int(pFX, _T("[StrandNum]"), m_StrandNum);
	RFX_Int(pFX, _T("[StringCaster]"), m_StringCaster);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
	RFX_Int(pFX, _T("[StringWeek]"), m_StringWeek);
	RFX_Int(pFX, _T("[StringYear]"), m_StringYear);
	RFX_Int(pFX, _T("[Version]"), m_Version);
	RFX_Text(pFX, _T("[FpOrderNum]"), m_FpOrderNum);
	RFX_Text(pFX, _T("[StockReason]"), m_StockReason);  // added reason field 5-5-09 k. hubbard
	RFX_Text(pFX, _T("[StockCommercialReason]"), m_StockCommercialReason);  // added reason field 5-5-09 k. hubbard
	RFX_Text(pFX, _T("[SlitReason]"), m_SlitReason);    // added reason field 2-26-10 k. hubbard
	//}}AFX_FIELD_MAP


	if ( m_nParams > 0 ) {

		pFX->SetFieldType( CFieldExchange::param );				  
		RFX_Long(pFX, _T("Whatever"), m_paramScenId);
		RFX_Int (pFX, _T("Whatever"), m_paramVersion);

		if ( m_nParams > 2 ) {
			RFX_Int(pFX, _T("Whatever"), m_paramYear);
			RFX_Int(pFX, _T("Whatever"), m_paramWeek);
			RFX_Int(pFX, _T("Whatever"), m_paramCaster);
			RFX_Int(pFX, _T("Whatever"), m_paramStringId);
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CCasterStringOrdersSet diagnostics

#ifdef _DEBUG
void CCasterStringOrdersSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringOrdersSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
