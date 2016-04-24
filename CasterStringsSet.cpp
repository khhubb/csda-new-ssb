// CasterStringsSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "CasterStringsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringsSet

IMPLEMENT_DYNAMIC(CCasterStringsSet, CRecordset)

CCasterStringsSet::CCasterStringsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringsSet)
	m_AimHeatSize = 0;
	m_CanEdit = FALSE;
	m_Caster = 0;
	m_FreezeHeats = FALSE;
	m_MaxHeatSize = 0;
	m_MinHeatSize = 0;
	m_NumHeats = 0;
	m_OrigStringStringId = 0;
	m_OrigStringWeek = 0;
	m_OrigStringYear = 0;
	m_ScenId = 0;
	m_Status = _T("");
	m_StringId = 0;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_UseOriginal = FALSE;
	m_HeatLockStatus = 0;
	m_NumHeatsLocked = 0;
	m_EndWidth1 = 0.0;
	m_EndWidth2 = 0.0;
	m_IsValidStartTime = FALSE;
	m_MaxCarbon = 0.0;
	m_MaxMn = 0.0;
	m_MinCarbon = 0.0;
	m_MinMn = 0.0;
	m_Spec = _T("");
	m_StartWidth1 = 0.0;
	m_StartWidth2 = 0.0;
	m_TurnaroundTime = 0;
	m_nFields = 31;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterStringsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringsSet::GetDefaultSQL()
{
	return _T("[CasterStrings]");
}

void CCasterStringsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[AimHeatSize]"), m_AimHeatSize);
	RFX_Bool(pFX, _T("[CanEdit]"), m_CanEdit);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Bool(pFX, _T("[FreezeHeats]"), m_FreezeHeats);
	RFX_Int(pFX, _T("[MaxHeatSize]"), m_MaxHeatSize);
	RFX_Int(pFX, _T("[MinHeatSize]"), m_MinHeatSize);
	RFX_Int(pFX, _T("[NumHeats]"), m_NumHeats);
	RFX_Int(pFX, _T("[OrigStringStringId]"), m_OrigStringStringId);
	RFX_Int(pFX, _T("[OrigStringWeek]"), m_OrigStringWeek);
	RFX_Int(pFX, _T("[OrigStringYear]"), m_OrigStringYear);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Text(pFX, _T("[Status]"), m_Status);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
	RFX_Int(pFX, _T("[Version]"), m_Version);
	RFX_Int(pFX, _T("[Week]"), m_Week);
	RFX_Int(pFX, _T("[Year]"), m_Year);
	RFX_Bool(pFX, _T("[UseOriginal]"), m_UseOriginal);
	RFX_Int(pFX, _T("[HeatLockStatus]"), m_HeatLockStatus);
	RFX_Int(pFX, _T("[NumHeatsLocked]"), m_NumHeatsLocked);
	RFX_Double(pFX, _T("[EndWidth1]"), m_EndWidth1);
	RFX_Double(pFX, _T("[EndWidth2]"), m_EndWidth2);
	RFX_Bool(pFX, _T("[IsValidStartTime]"), m_IsValidStartTime);
	RFX_Double(pFX, _T("[MaxCarbon]"), m_MaxCarbon);
	RFX_Double(pFX, _T("[MaxMn]"), m_MaxMn);
	RFX_Double(pFX, _T("[MinCarbon]"), m_MinCarbon);
	RFX_Double(pFX, _T("[MinMn]"), m_MinMn);
	RFX_Text(pFX, _T("[Spec]"), m_Spec);
	RFX_Date(pFX, _T("[StartTime]"), m_StartTime);
	RFX_Double(pFX, _T("[StartWidth1]"), m_StartWidth1);
	RFX_Double(pFX, _T("[StartWidth2]"), m_StartWidth2);
	RFX_Long(pFX, _T("[TurnaroundTime]"), m_TurnaroundTime);
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
// CCasterStringsSet diagnostics

#ifdef _DEBUG
void CCasterStringsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
