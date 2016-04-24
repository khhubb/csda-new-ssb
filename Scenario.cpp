// Scenario.cpp: implementation of the CScenario class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "csda.h"
#include "Scenario.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


int CScenario::m_counter = 0;


CScenario::CScenario()
{
	m_id			= ++m_counter;
	m_idAssigned	= false;  // not yet assigned from database
	m_canEdit		= true;
	m_isModified	= false;

	// m_comment (CString)
	// m_owner   (CString)
}


