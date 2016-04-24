// DocMvcMixin.cpp: implementation of the CDocMvcMixin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "csda.h"
#include "DocMvcMixin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// You shouldn't use this class directly.
// Rather, all document classes should be derived from CDocBase
//   instead of from CDcoment;
// CDocBase derives from this class.
// This class provides a pointer to a CMessageBus and an accessor
//   called GetModel.  The c-tor causes the pointer to be initialized
//   to the singleon instance of CMessageBus.

#include "MessageBus.h"


CDocMvcMixin::CDocMvcMixin()
{
	m_pBus = &TheBus();

}

