// CasterStringTemplateChemsSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterStringTemplateChemsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplateChemsSet

IMPLEMENT_DYNAMIC(CCasterStringTemplateChemsSet, CRecordset)

CCasterStringTemplateChemsSet::CCasterStringTemplateChemsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringTemplateChemsSet)
	m_Caster = 0;
	m_Element = _T("");
	m_MaxValue = 0.0;
	m_MinValue = 0.0;
	m_ScenId = 0;
	m_StringId = 0;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterStringTemplateChemsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringTemplateChemsSet::GetDefaultSQL()
{
	return _T("[CasterStringTemplateChems]");
}

void CCasterStringTemplateChemsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringTemplateChemsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Text(pFX, _T("[Element]"), m_Element);
	RFX_Double(pFX, _T("[MaxValue]"), m_MaxValue);
	RFX_Double(pFX, _T("[MinValue]"), m_MinValue);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
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
// CCasterStringTemplateChemsSet diagnostics

#ifdef _DEBUG
void CCasterStringTemplateChemsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringTemplateChemsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
