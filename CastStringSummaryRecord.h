#ifndef _castStringSummaryRecord_included_
#define _castStringSummaryRecord_included_

#include "modelTypes.h"

class CCSOrder;



class CCastStringSummaryRecord 
{
	////////////////////////////////////////////////////////////////
	//	
	//	Used to record certain summary information for 
	//		a heat, an entire strand, or the string as a whole.
	//	
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

public:
	
	typedef map<CString,Weight> HsmLuTypeToTonsMap;
	typedef map<int,Weight> CICodeToTonsMap;

	Weight m_tons;
	int m_numPieces;

	int m_numRadicals;

	int m_numCMS;
	int m_numCMSFlips;
	int m_numFlips;

	int m_numSlits;
	int m_numSplits;

	int m_numStockPieces;

	int m_numCurrentHRPieces;
	int m_numBackHRPieces;
	int m_numFutureHRPieces;

	// FP Change
	long m_tonsManual;    //FP_ISHRO_PRTY = 1-9
	long m_tonsComputer;  //FP_ISHRO_PRTY = 10
	long m_tonsNonprio;   //FP_ISHRO_PRTY = 91 (or other)

	Width m_avgWidth;

	CTimeSpan m_duration;

	HsmLuTypeToTonsMap m_luTypeTonsMap;
	CICodeToTonsMap m_ciCodeTonsMap;

	

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

	// decided to make the data public.


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

	CCastStringSummaryRecord();


	// who wants this?

	bool operator<(const CCastStringSummaryRecord& x) const
	{
		return true;
	}

	bool operator==(const CCastStringSummaryRecord& x) const
	{
		return false;
	}

	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	void Init();
	void AddCSOrder(CCSOrder*);
	void AddCSOrder(CCSOrder*, int numPieces);

	void Sum(const CCastStringSummaryRecord& x);



	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////

	void AddLuTypeTons(const CString& luType,Weight tons);
	void AddCICodeTons(int ciCode, Weight tons);
};



#endif // _castStringSummaryRecord_included_