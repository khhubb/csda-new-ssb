// AvailCasterScenId.cpp: implementation of the CAvailCasterScenId class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "AvailCasterScenId.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//
// initialize to nominal values
//

CAvailCasterScenId::CAvailCasterScenId()
{
	m_id		= 0;
	m_owner		= "";
	m_caster	= 0;
	m_version	= 0;
	m_comment	= "";
	m_isPublic	= false;
}

CAvailCasterScenId::~CAvailCasterScenId()
{
	// do nothing
}
