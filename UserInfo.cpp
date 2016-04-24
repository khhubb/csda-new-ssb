// UserInfo.cpp: implementation of the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "UserInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//  Class CUserInfo hold information about a user.
//  At the moment, only minimal information is held.
//  We keep one static variable of this type to hold info on the 
//    the user that logged in as the system started.
//  Eventually, we will support changing the login --
//     then this will represent the currently logged in user.



// static
CUserInfo CUserInfo::TheUser;