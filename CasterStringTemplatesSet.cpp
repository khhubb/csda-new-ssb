// CasterStringTemplatesSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CasterStringTemplatesSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplatesSet

IMPLEMENT_DYNAMIC(CCasterStringTemplatesSet, CRecordset)

CCasterStringTemplatesSet::CCasterStringTemplatesSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringTemplatesSet)
	m_Caster = 0;
	m_CurrentHrWeek = 0;
	m_CurrentHrYear = 0;
	m_IncludeNewOrders = FALSE;
	m_IncludeOrders = 0;
	m_MaxCIPriority = 0;
	m_MaxWidth = 0;
	m_MegaLuType = _T("");
	m_MinCIPriority = 0;
	m_MinWidth = 0.0;
	m_ScenId = 0;
	m_StringId = 0;
	m_Use80HsmLuType = FALSE;
	m_UseAllCICodes = FALSE;
	m_UseAllSpecs = FALSE;
	m_UseCrossApp = FALSE;
	m_UseHrWkCurrentBack = FALSE;
	m_UseHrWkFuture = FALSE;
	m_UseHrWkPlus1 = FALSE;
	m_UseHrWkPlus2 = FALSE;
	m_UseHrWkPlus3 = FALSE;
	m_UseHrWkPlus4 = FALSE;
	m_UsePriorityCode = FALSE;
	m_UsePst15to21 = FALSE;
	m_UsePst22to28 = FALSE;
	m_UsePst29to35 = FALSE;
	m_UsePst8to14 = FALSE;
	m_UsePstGt35 = FALSE;
	m_UsePstLt8 = FALSE;
	m_UseWidthRange = FALSE;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_nFields = 33;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CCasterStringTemplatesSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringTemplatesSet::GetDefaultSQL()
{
	return _T("[CasterStringTemplates]");
}

void CCasterStringTemplatesSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringTemplatesSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Int(pFX, _T("[CurrentHrWeek]"), m_CurrentHrWeek);
	RFX_Int(pFX, _T("[CurrentHrYear]"), m_CurrentHrYear);
	RFX_Bool(pFX, _T("[IncludeNewOrders]"), m_IncludeNewOrders);
	RFX_Int(pFX, _T("[IncludeOrders]"), m_IncludeOrders);
	RFX_Int(pFX, _T("[MaxCIPriority]"), m_MaxCIPriority);
	RFX_Long(pFX, _T("[MaxWidth]"), m_MaxWidth);
	RFX_Text(pFX, _T("[MegaLuType]"), m_MegaLuType);
	RFX_Int(pFX, _T("[MinCIPriority]"), m_MinCIPriority);
	RFX_Double(pFX, _T("[MinWidth]"), m_MinWidth);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
	RFX_Bool(pFX, _T("[Use80HsmLuType]"), m_Use80HsmLuType);
	RFX_Bool(pFX, _T("[UseAllCICodes]"), m_UseAllCICodes);
	RFX_Bool(pFX, _T("[UseAllSpecs]"), m_UseAllSpecs);
	RFX_Bool(pFX, _T("[UseCrossApp]"), m_UseCrossApp);
	RFX_Bool(pFX, _T("[UseHrWkCurrentBack]"), m_UseHrWkCurrentBack);
	RFX_Bool(pFX, _T("[UseHrWkFuture]"), m_UseHrWkFuture);
	RFX_Bool(pFX, _T("[UseHrWkPlus1]"), m_UseHrWkPlus1);
	RFX_Bool(pFX, _T("[UseHrWkPlus2]"), m_UseHrWkPlus2);
	RFX_Bool(pFX, _T("[UseHrWkPlus3]"), m_UseHrWkPlus3);
	RFX_Bool(pFX, _T("[UseHrWkPlus4]"), m_UseHrWkPlus4);
	RFX_Bool(pFX, _T("[UsePriorityCode]"), m_UsePriorityCode);
	RFX_Bool(pFX, _T("[UsePst15to21]"), m_UsePst15to21);
	RFX_Bool(pFX, _T("[UsePst22to28]"), m_UsePst22to28);
	RFX_Bool(pFX, _T("[UsePst29to35]"), m_UsePst29to35);
	RFX_Bool(pFX, _T("[UsePst8to14]"), m_UsePst8to14);
	RFX_Bool(pFX, _T("[UsePstGt35]"), m_UsePstGt35);
	RFX_Bool(pFX, _T("[UsePstLt8]"), m_UsePstLt8);
	RFX_Bool(pFX, _T("[UseWidthRange]"), m_UseWidthRange);
	RFX_Int(pFX, _T("[Version]"), m_Version);
	RFX_Int(pFX, _T("[Week]"), m_Week);
	RFX_Int(pFX, _T("[Year]"), m_Year);
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
// CCasterStringTemplatesSet diagnostics

#ifdef _DEBUG
void CCasterStringTemplatesSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringTemplatesSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
