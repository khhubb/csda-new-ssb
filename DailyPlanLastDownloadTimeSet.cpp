// DailyPlanLastDownloadTimeSet.cpp : implementation file
//

#include "stdafx.h"
#include "csda.h"
#include "DailyPlanLastDownloadTimeSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDailyPlanLastDownloadTimeSet

IMPLEMENT_DYNAMIC(CDailyPlanLastDownloadTimeSet, CRecordset)

CDailyPlanLastDownloadTimeSet::CDailyPlanLastDownloadTimeSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDailyPlanLastDownloadTimeSet)
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CDailyPlanLastDownloadTimeSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=dpaTest");
}

CString CDailyPlanLastDownloadTimeSet::GetDefaultSQL()
{
	return _T("[STLMAKE_DAILY_PLAN_LAST_DOWNLOAD_TIME]");
}

void CDailyPlanLastDownloadTimeSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDailyPlanLastDownloadTimeSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, _T("[Last Download]"), m_Last_Download);
	//}}AFX_FIELD_MAP

}

/////////////////////////////////////////////////////////////////////////////
// CDailyPlanLastDownloadTimeSet diagnostics

#ifdef _DEBUG
void CDailyPlanLastDownloadTimeSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CDailyPlanLastDownloadTimeSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
