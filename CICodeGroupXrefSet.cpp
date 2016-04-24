// CICodeGroupXrefSet.cpp : implementation file
//

#include "stdafx.h"



#include "csda.h"
#include "CICodeGroupXrefSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupXrefSet

IMPLEMENT_DYNAMIC(CCICodeGroupXrefSet, CRecordset)

CCICodeGroupXrefSet::CCICodeGroupXrefSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCICodeGroupXrefSet)
	m_CMDTY_XREF_CMDTY_GRP_CD = 0;
	m_CMDTY_XREF_CMDTY_CD = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCICodeGroupXrefSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=DPATest");
}

CString CCICodeGroupXrefSet::GetDefaultSQL()
{
//	return _T("[CICodeGroupXref]");
	return _T("[P_HMPLAN_CMDTY_XREF_TBL]");    // Added 5-3-10 k. hubbard
}

void CCICodeGroupXrefSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCICodeGroupXrefSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[CMDTY_XREF_CMDTY_GRP_CD]"), m_CMDTY_XREF_CMDTY_GRP_CD);
	RFX_Text(pFX, _T("[CMDTY_XREF_CMDTY_CD]"), m_CMDTY_XREF_CMDTY_CD);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCICodeGroupXrefSet diagnostics

#ifdef _DEBUG
void CCICodeGroupXrefSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCICodeGroupXrefSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
