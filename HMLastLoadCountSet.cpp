// HMLastLoadCountSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "HMLastLoadCountSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMLastLoadCountSet

IMPLEMENT_DYNAMIC(CHMLastLoadCountSet, CRecordset)

CHMLastLoadCountSet::CHMLastLoadCountSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CHMLastLoadCountSet)
	m_Actual_L_C = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CHMLastLoadCountSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=hotmetal");
}

CString CHMLastLoadCountSet::GetDefaultSQL()
{
	return _T("[LastLoadCount]");
}

void CHMLastLoadCountSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CHMLastLoadCountSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Actual L/C]"), m_Actual_L_C);
	RFX_Date(pFX, _T("[Date]"), m_Date);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CHMLastLoadCountSet diagnostics

#ifdef _DEBUG
void CHMLastLoadCountSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CHMLastLoadCountSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
