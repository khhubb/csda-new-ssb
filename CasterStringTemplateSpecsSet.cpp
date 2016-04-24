// CasterStringTemplateSpecsSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterStringTemplateSpecsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringTemplateSpecsSet

IMPLEMENT_DYNAMIC(CCasterStringTemplateSpecsSet, CRecordset)

CCasterStringTemplateSpecsSet::CCasterStringTemplateSpecsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringTemplateSpecsSet)
	m_Caster = 0;
	m_ScenId = 0;
	m_SeqNum = 0;
	m_Spec = _T("");
	m_StringId = 0;
	m_Version = 0;
	m_Week = 0;
	m_Year = 0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterStringTemplateSpecsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringTemplateSpecsSet::GetDefaultSQL()
{
	return _T("[CasterStringTemplateSpecs]");
}

void CCasterStringTemplateSpecsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringTemplateSpecsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[SeqNum]"), m_SeqNum);
	RFX_Text(pFX, _T("[Spec]"), m_Spec);
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
// CCasterStringTemplateSpecsSet diagnostics

#ifdef _DEBUG
void CCasterStringTemplateSpecsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringTemplateSpecsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
