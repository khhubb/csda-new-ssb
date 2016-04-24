// CasterStringHeatChemsSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CasterStringHeatChemsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringHeatChemsSet

IMPLEMENT_DYNAMIC(CCasterStringHeatChemsSet, CRecordset)

CCasterStringHeatChemsSet::CCasterStringHeatChemsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringHeatChemsSet)
	m_Caster = 0;
	m_Element = _T("");
	m_HeatSeqNum = 0;
	m_MaxValue = 0.0;
	m_MinValue = 0.0;
	m_ScenId = 0;
	m_StringId = 0;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_AimValue = 0.0;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterStringHeatChemsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringHeatChemsSet::GetDefaultSQL()
{
	return _T("[CasterStringHeatChems]");
}

void CCasterStringHeatChemsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringHeatChemsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Text(pFX, _T("[Element]"), m_Element);
	RFX_Int(pFX, _T("[HeatSeqNum]"), m_HeatSeqNum);
	RFX_Double(pFX, _T("[MaxValue]"), m_MaxValue);
	RFX_Double(pFX, _T("[MinValue]"), m_MinValue);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
	RFX_Int(pFX, _T("[Version]"), m_Version);
	RFX_Int(pFX, _T("[Week]"), m_Week);
	RFX_Int(pFX, _T("[Year]"), m_Year);
	RFX_Double(pFX, _T("[AimValue]"), m_AimValue);
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
// CCasterStringHeatChemsSet diagnostics

#ifdef _DEBUG
void CCasterStringHeatChemsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringHeatChemsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
