// CIBogieSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "CIBogieSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCIBogieSet

IMPLEMENT_DYNAMIC(CCIBogieSet, CRecordset)

CCIBogieSet::CCIBogieSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCIBogieSet)
	m_CI_CODE = 0;
	m_BOGIE = 0;
	m_ROLLED = 0;
	m_ON_LINEUP = 0;
	m_SLABBED_AVAIL = 0;
	m_CMS = 0;
	m_SHAVES = 0;
	m_OTHER_UNAVAIL = 0;
	m_TONS_910 = 0;
	m_TONS_909 = 0;
	m_TONS_REM_PLAN;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCIBogieSet::GetDefaultConnect()
{
//	return _T("ODBC;DSN=CSDATest");
	return _T("ODBC;DSN=dpaTest");
}

CString CCIBogieSet::GetDefaultSQL()
{
	return _T("[CSD_80_HSM_BOGIES]");
}

void CCIBogieSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCIBogieSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[CI_CODE]"), m_CI_CODE);
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
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCIBogieSet diagnostics

#ifdef _DEBUG
void CCIBogieSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCIBogieSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
