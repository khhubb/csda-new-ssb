// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F60F118D_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
#define AFX_STDAFX_H__F60F118D_A60D_11D0_975C_0000C0C2484C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _CRT_SECURE_NO_WARNINGS

#include "SupportedPlatforms.h"

#pragma warning(disable:4786)
#pragma warning(disable:4800)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#include <afxmt.h>			// MFC Multithreading classes



#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



#include <afxole.h>			// for drag-drop

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <functional>
#include <algorithm>


#include <strstream>
#include <iomanip>
#include <fstream>

#include <assert.h>

//version 5 of ot pro
//#include "config\default.h"


// version 6 of ot pro and version 7 of grid
//#include "config\cmn_default.h"
//#include "config\mvc_default.h"
//#include "config\ot_default.h"
//#include "secall.h"
//
//#include "config\OG Lite.h"
//#include "gxall.h"

// version 7 of otpro and version 8 of grid
//#include "foundation\config\default.h"
//#include "toolkit\config\ot_default.h"
#include "toolkit\secall.h"
#include "grid\config\gxstndrd.h"
#include "grid\gxall.h"
using namespace stingray::foundation;

using namespace std;

#include "stlext.h"
#include "stlUtils.h"

#include "ManifestDefs.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.




#endif // !defined(AFX_STDAFX_H__F60F118D_A60D_11D0_975C_0000C0C2484C__INCLUDED_)
