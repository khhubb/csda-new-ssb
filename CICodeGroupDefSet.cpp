// CICodeGroupDefSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CICodeGroupDefSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupDefSet

IMPLEMENT_DYNAMIC(CCICodeGroupDefSet, CRecordset)

CCICodeGroupDefSet::CCICodeGroupDefSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCICodeGroupDefSet)
	m_CMDTY_XREF_CMDTY_GRP_CD = 0;
//	m_CMDTY_GRP_NAME = _T("");
//  m_CI_CODE = 0;
	m_BOGIE = 0;
	m_ROLLED = 0;
	m_ON_LINEUP = 0;
	m_SLABBED_AVAIL = 0;
	m_CMS = 0;
	m_SHAVES = 0;
	m_OTHER_UNAVAIL = 0;
	m_TONS_910 = 0;
	m_TONS_909 = 0;
	m_TONS_REM_PLAN = 0;
	m_CMDTY_GRP_NAME = _T("");

	m_nFields = 12;
//	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCICodeGroupDefSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CCICodeGroupDefSet::GetDefaultSQL()
{
	return _T("[CICodeGroupDef]");          // Added 5-4-10 k. hubbard
//	return _T("[P_HMPLAN_CMDTY_GROUP_TBL]");   // Added 5-3-10 k. hubbard
}

void CCICodeGroupDefSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCICodeGroupDefSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[CMDTY_XREF_CMDTY_GRP_CD]"), m_CMDTY_XREF_CMDTY_GRP_CD);
//	RFX_Text(pFX, _T("[CMDTY_GRP_NAME]"), m_CMDTY_GRP_NAME);
//	RFX_Int(pFX,  _T("[CI_CODE]"), m_CI_CODE);
	RFX_Long(pFX, _T("[BOGIE]"), m_BOGIE);
	RFX_Long(pFX, _T("[ROLLED]"), m_ROLLED);
	RFX_Long(pFX, _T("[ON_LINEUP]"), m_ON_LINEUP);
	RFX_Long(pFX, _T("[SLABBED_AVAILABLE]"), m_SLABBED_AVAIL);
	RFX_Long(pFX, _T("[TONS_CMS]"), m_CMS);
	RFX_Long(pFX, _T("[SHAVES]"), m_SHAVES);
	RFX_Long(pFX, _T("[TONS_OTHER]"), m_OTHER_UNAVAIL);
	RFX_Long(pFX, _T("[TONS_910]"), m_TONS_910);
	RFX_Long(pFX, _T("[TONS_909]"), m_TONS_909);
	RFX_Long(pFX, _T("[TONS_REMAIN_TO_PLAN]"), m_TONS_REM_PLAN);
	RFX_Text(pFX, _T("[CMDTY_GRP_NAME]"), m_CMDTY_GRP_NAME);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupDefSet diagnostics

#ifdef _DEBUG
void CCICodeGroupDefSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCICodeGroupDefSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
