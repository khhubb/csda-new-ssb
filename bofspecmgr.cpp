// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "bofspecmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IBOFSpeedCalc properties

/////////////////////////////////////////////////////////////////////////////
// IBOFSpeedCalc operations

void IBOFSpeedCalc::Initialize(LPCTSTR dsn)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dsn);
}

double IBOFSpeedCalc::SlabCastTime(LPCTSTR specName, long caster, long strand, long position, double weight, double width, double thickness, double carbon, double manganese, long heatSize, long* specFound)
{
	double result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_I4 VTS_PI4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		specName, caster, strand, position, weight, width, thickness, carbon, manganese, heatSize, specFound);
	return result;
}

long IBOFSpeedCalc::IsValidSpec(LPCTSTR specName, long caster)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		specName, caster);
	return result;
}
