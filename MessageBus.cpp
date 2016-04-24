// MessageBus.cpp: implementation of the CMessageBus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "MessageBus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
//  The CMessageBus class follows Singleton semantics.
//  To avoid having references to CMessageBus::Instance() all over the place,
//    we have a global function ::TheBus().
//
//  CMessageBus is an MvcModel.  All editors in the system use this as
//  their model, from which they receive messages about changes in the system.
//  All editing operations, loading operations, etc. -- anything which might
//    cause something else to change somewhere -- must post a message on
//    TheBus().
//
//  I may regret this some day.  Maybe not.

//  Singleton semantics


CMessageBus* CMessageBus::m_pInstance = 0;

CMessageBus* CMessageBus::Instance()
{
	if ( m_pInstance == 0 )
		m_pInstance = new CMessageBus;

	return m_pInstance;
}

CMessageBus& ::TheBus()
{	return *CMessageBus::Instance();	}


