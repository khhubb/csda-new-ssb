// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IBOFSpeedCalc wrapper class

class IBOFSpeedCalc : public COleDispatchDriver
{
public:
	IBOFSpeedCalc() {}		// Calls COleDispatchDriver default constructor
	IBOFSpeedCalc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBOFSpeedCalc(const IBOFSpeedCalc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Initialize(LPCTSTR dsn);
	double SlabCastTime(LPCTSTR specName, long caster, long strand, long position, double weight, double width, double thickness, double carbon, double manganese, long heatSize, long* specFound);
	long IsValidSpec(LPCTSTR specName, long caster);

	~IBOFSpeedCalc() 
	{}
};
