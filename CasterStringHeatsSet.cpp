// CasterStringHeatsSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterStringHeatsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterStringHeatsSet

IMPLEMENT_DYNAMIC(CCasterStringHeatsSet, CRecordset)

CCasterStringHeatsSet::CCasterStringHeatsSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterStringHeatsSet)
	m_Comment = _T("");
	m_CondnCode = 0;
	m_DispCode = 0;
	m_IsMarked = FALSE;
	m_ScenId = 0;
	m_SeqNum = 0;
	m_SignifEl = _T("");
	m_Spec = _T("");
	m_Status = _T("");
	m_StringCaster = 0;
	m_StringId = 0;
	m_StringWeek = 0;
	m_StringYear = 0;
	m_TabNum = 0;
	m_Tons = 0;
	m_Version = 0;
	m_nFields = 16;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCasterStringHeatsSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterStringHeatsSet::GetDefaultSQL()
{
	return _T("[CasterStringHeats]");
}

void CCasterStringHeatsSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterStringHeatsSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	RFX_Int(pFX, _T("[CondnCode]"), m_CondnCode);
	RFX_Int(pFX, _T("[DispCode]"), m_DispCode);
	RFX_Bool(pFX, _T("[IsMarked]"), m_IsMarked);
	RFX_Long(pFX, _T("[ScenId]"), m_ScenId);
	RFX_Int(pFX, _T("[SeqNum]"), m_SeqNum);
	RFX_Text(pFX, _T("[SignifEl]"), m_SignifEl);
	RFX_Text(pFX, _T("[Spec]"), m_Spec);
	RFX_Text(pFX, _T("[Status]"), m_Status);
	RFX_Int(pFX, _T("[StringCaster]"), m_StringCaster);
	RFX_Int(pFX, _T("[StringId]"), m_StringId);
	RFX_Int(pFX, _T("[StringWeek]"), m_StringWeek);
	RFX_Int(pFX, _T("[StringYear]"), m_StringYear);
	RFX_Long(pFX, _T("[TabNum]"), m_TabNum);
	RFX_Long(pFX, _T("[Tons]"), m_Tons);
	RFX_Int(pFX, _T("[Version]"), m_Version);
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
// CCasterStringHeatsSet diagnostics

#ifdef _DEBUG
void CCasterStringHeatsSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterStringHeatsSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
