//
//  clipFormat.cpp
//
//  Implementation of CClipFormat -- static class for clipboard format constants

#include "StdAfx.h"


#include "clipFormat.h"


//	CClipFormat is a statics-only class that registers the clipboard formats
//		used for various drag-drop operations.
//
//	One instance of this should be created to get initialization to occur.



CLIPFORMAT CClipFormat::sm_availOrderGroupFormat;
CLIPFORMAT CClipFormat::sm_availOrderOrderFormat;
CLIPFORMAT CClipFormat::sm_casterScenOrderFormat;
CLIPFORMAT CClipFormat::sm_insertStockOrderFormat;
CLIPFORMAT CClipFormat::sm_insertOrderFormat;

	
bool CClipFormat::sm_staticsInitialized = false;


void CClipFormat::InitializeStatics()
{
	sm_staticsInitialized = true;

	sm_availOrderGroupFormat  = CLIPFORMAT(::RegisterClipboardFormat(_T("CSDA availOrderGroup")));
	sm_availOrderOrderFormat  = CLIPFORMAT(::RegisterClipboardFormat(_T("CSDA availOrderOrder")));
	sm_casterScenOrderFormat  = CLIPFORMAT(::RegisterClipboardFormat(_T("CSDA casterScenOrder")));
	sm_insertStockOrderFormat = CLIPFORMAT(::RegisterClipboardFormat(_T("CSDA stockOrderInsert")));
	sm_insertOrderFormat      = CLIPFORMAT(::RegisterClipboardFormat(_T("CSDA orderInsert"     )));
}


CClipFormat::CClipFormat()
{
	if ( ! sm_staticsInitialized )
		InitializeStatics();
}


// we need one of these created in order to get initializations

static CClipFormat x;

