// SpecChemRangePresetSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "SpecChemRangePresetSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecChemRangePresetSet

IMPLEMENT_DYNAMIC(CSpecChemRangePresetSet, CRecordset)

CSpecChemRangePresetSet::CSpecChemRangePresetSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSpecChemRangePresetSet)
	m_Spec = _T("");
	m_Group = 0;
	m_MinCarbon = 0.0;
	m_MaxCarbon = 0.0;
	m_MinMn = 0.0;
	m_MaxMn = 0.0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSpecChemRangePresetSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CSpecChemRangePresetSet::GetDefaultSQL()
{
	return _T("[SpecChemRangePreset]");
}

void CSpecChemRangePresetSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSpecChemRangePresetSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Spec]"), m_Spec);
	RFX_Int(pFX, _T("[Group]"), m_Group);
	RFX_Double(pFX, _T("[MinCarbon]"), m_MinCarbon);
	RFX_Double(pFX, _T("[MaxCarbon]"), m_MaxCarbon);
	RFX_Double(pFX, _T("[MinMn]"), m_MinMn);
	RFX_Double(pFX, _T("[MaxMn]"), m_MaxMn);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSpecChemRangePresetSet diagnostics

#ifdef _DEBUG
void CSpecChemRangePresetSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSpecChemRangePresetSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
