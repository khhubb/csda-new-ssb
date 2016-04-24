// CasterScenariosSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CasterScenariosSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosSet

IMPLEMENT_DYNAMIC(CCasterScenariosSet, CRecordset)

CCasterScenariosSet::CCasterScenariosSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCasterScenariosSet)
	m_Caster = 0;
	m_Comment = _T("");
	m_Id = 0;
	m_IsPublic = FALSE;
	m_Owner = _T("");
	m_Version = 0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CCasterScenariosSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CCasterScenariosSet::GetDefaultSQL()
{
	return _T("[CasterScenarios]");
}

void CCasterScenariosSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCasterScenariosSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Int(pFX, _T("[Caster]"), m_Caster);
	RFX_Text(pFX, _T("[Owner]"), m_Owner);
	RFX_Date(pFX, _T("[CreateTime]"), m_CreateTime);
	RFX_Date(pFX, _T("[LastSaveTime]"), m_LastSaveTime);
	RFX_Long(pFX, _T("[Version]"), m_Version);
	RFX_Bool(pFX, _T("[IsPublic]"), m_IsPublic);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCasterScenariosSet diagnostics

#ifdef _DEBUG
void CCasterScenariosSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCasterScenariosSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
