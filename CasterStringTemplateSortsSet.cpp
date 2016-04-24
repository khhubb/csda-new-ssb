// CasterStringTemplateSortsSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterStringTemplateSortsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplateSortsSet

IMPLEMENT_DYNAMIC(CCasterStringTemplateSortsSet, CRecordset)

CCasterStringTemplateSortsSet::CCasterStringTemplateSortsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringTemplateSortsSet)
	m_Caster = 0;
	m_ScenId = 0;
	m_SeqNum = 0;
	m_SortField = _T("");
	m_SortType = _T("");
	m_StringId = 0;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CCasterStringTemplateSortsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringTemplateSortsSet::GetDefaultSQL()
{
	return _T("[CasterStringTemplateSorts]");
}

void CCasterStringTemplateSortsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringTemplateSortsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[SeqNum]"), m_SeqNum);
	RFX_Text(pFX, _T("[SortField]"), m_SortField);
	RFX_Text(pFX, _T("[SortType]"), m_SortType);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
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
// CCasterStringTemplateSortsSet diagnostics

#ifdef _DEBUG
void CCasterStringTemplateSortsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringTemplateSortsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
