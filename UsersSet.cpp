// UsersSet.cpp : implementation file
//

#include "stdafx.h"

#include "csda.h"
#include "UsersSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsersSet

IMPLEMENT_DYNAMIC(CUsersSet, CRecordset)

CUsersSet::CUsersSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CUsersSet)
	m_LoginName = _T("");
	m_Password = _T("");
	m_FirstName = _T("");
	m_LastName = _T("");
	m_SBId = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CUsersSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=CSDATest");
}

CString CUsersSet::GetDefaultSQL()
{
	return _T("[Users]");
}

void CUsersSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CUsersSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[LoginName]"), m_LoginName);
	RFX_Text(pFX, _T("[Password]"), m_Password);
	RFX_Text(pFX, _T("[FirstName]"), m_FirstName);
	RFX_Text(pFX, _T("[LastName]"), m_LastName);
	RFX_Text(pFX, _T("[SBId]"), m_SBId);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CUsersSet diagnostics

#ifdef _DEBUG
void CUsersSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CUsersSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
