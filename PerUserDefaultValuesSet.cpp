// PerUserDefaultValuesSet.cpp : implementation file
//

#include "stdafx.h"


#include "csda.h"
#include "PerUserDefaultValuesSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPerUserDefaultValuesSet

IMPLEMENT_DYNAMIC(CPerUserDefaultValuesSet, CRecordset)

CPerUserDefaultValuesSet::CPerUserDefaultValuesSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPerUserDefaultValuesSet)
	m_LoginName = _T("");
	m_Caster1HeatSizeMin = 0;
	m_Caster1HeatSizeAim = 0;
	m_Caster1HeatSizeMax = 0;
	m_Caster2HeatSizeMin = 0;
	m_Caster2HeatSizeAim = 0;
	m_Caster2HeatSizeMax = 0;
	m_Caster3HeatSizeMin = 0;
	m_Caster3HeatSizeAim = 0;
	m_Caster3HeatSizeMax = 0;
	m_nFields = 10;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;

	m_loginNameParm = _T("");

	++m_nParams;

	m_strFilter = "loginName = ?";
}


CString CPerUserDefaultValuesSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CPerUserDefaultValuesSet::GetDefaultSQL()
{
	return _T("[PerUserDefaultValues]");
}

void CPerUserDefaultValuesSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPerUserDefaultValuesSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[LoginName]"), m_LoginName);
	RFX_Int(pFX, _T("[Caster1HeatSizeMin]"), m_Caster1HeatSizeMin);
	RFX_Int(pFX, _T("[Caster1HeatSizeAim]"), m_Caster1HeatSizeAim);
	RFX_Int(pFX, _T("[Caster1HeatSizeMax]"), m_Caster1HeatSizeMax);
	RFX_Int(pFX, _T("[Caster2HeatSizeMin]"), m_Caster2HeatSizeMin);
	RFX_Int(pFX, _T("[Caster2HeatSizeAim]"), m_Caster2HeatSizeAim);
	RFX_Int(pFX, _T("[Caster2HeatSizeMax]"), m_Caster2HeatSizeMax);
	RFX_Int(pFX, _T("[Caster3HeatSizeMin]"), m_Caster3HeatSizeMin);
	RFX_Int(pFX, _T("[Caster3HeatSizeAim]"), m_Caster3HeatSizeAim);
	RFX_Int(pFX, _T("[Caster3HeatSizeMax]"), m_Caster3HeatSizeMax);
	//}}AFX_FIELD_MAP

	pFX->SetFieldType(CFieldExchange::inputParam);
	
	RFX_Text(pFX,"whatever",m_loginNameParm);

}

/////////////////////////////////////////////////////////////////////////////
// CPerUserDefaultValuesSet diagnostics

#ifdef _DEBUG
void CPerUserDefaultValuesSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPerUserDefaultValuesSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
