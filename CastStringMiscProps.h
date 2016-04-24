#ifndef _castStringMiscProps_included_
#define _castStringMiscProps_included_

#include "modelTypes.h"

class CSpec;
#include "Chem.h"

 
class CCastStringMiscProps
{
	////////////////////////////////////////////////////////////////
	//	
	//	Captures certain miscellaneous defining properties 
	//		of a cast string
	//	
	////////////////////////////////////////////////////////////////


	//  A CCastString needs access to change this as it pleases
	friend class CCastString;


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	int m_numHeats;
		//  desired number of heats in string 
		//		(as opposed to heats actually in the string)

	int m_aimHeatSize;
	int m_minHeatSize;
	int m_maxHeatSize;
		// used to determine heat boundaries when editing the string

	Width m_startWidths[3]; // strand = 1,2
	Width m_endWidths[3];	// strand = 1,2
		// desired start and end widths for the string

	CString m_initSpec;
	CSpec* m_pInitSpec;
		// the spec to start with

	CString m_comment;
		// hmmm

	CChem::Bound m_minC;
	CChem::Bound m_maxC;
	CChem::Bound m_minMn;
	CChem::Bound m_maxMn;
		// desired chemistry ranges

	CTime m_startTime;
	bool m_isStartTimeValid;
		//  desired start time of cast
		//  For use in the hot metal display

	CTimeSpan m_turnaroundTime;
		// how much time to allow after this string
		// For use in the hot metal display


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////


public:

	int NumHeats() const
	{	return m_numHeats;	}

	void SetNumHeats(int newVal)
	{	m_numHeats = newVal;	}

	Weight Tons() const
	{	return m_numHeats*m_aimHeatSize;	}

	int MinHeatSize() const 
	{	return m_minHeatSize;	}

	void SetMinHeatSize(int newVal)
	{	m_minHeatSize = newVal;	}

	int AimHeatSize() const 
	{	return m_aimHeatSize;	}

	void SetAimHeatSize(int newVal)
	{	m_aimHeatSize = newVal;	}

	int MaxHeatSize() const 
	{	return m_maxHeatSize;	}

	void SetMaxHeatSize(int newVal)
	{	m_maxHeatSize = newVal;	}

	const CString& Comment() const
	{	return m_comment;			}

	void SetComment(CString& newVal)
	{	m_comment = newVal;		}

	Width StartWidth(int strandNum) const
	{	
		assert(strandNum == 1 || strandNum == 2);
		return m_startWidths[strandNum];
	}

	Width EndWidth(int strandNum) const
	{	
		assert(strandNum == 1 || strandNum == 2);
		return m_endWidths[strandNum];
	}

	void SetStartWidth(int strandNum, Width newVal) 
	{	
		assert(strandNum == 1 || strandNum == 2);
		m_startWidths[strandNum] = newVal;		;
	}

	void SetEndWidth(int strandNum, Width newVal) 
	{	
		assert(strandNum == 1 || strandNum == 2);
		m_endWidths[strandNum] = newVal;		;
	}


	const CString& InitSpec() const
	{	return m_initSpec;	}

	CSpec* InitSpecPtr() const
	{	return m_pInitSpec;	}

	void  SetInitSpec( const CString&, int caster);


	CChem::Bound MinCarbon() const
	{	return m_minC;	}

	CChem::Bound MaxCarbon() const
	{	return m_maxC;	}

	CChem::Bound MinMn() const
	{	return m_minMn;	}

	CChem::Bound MaxMn() const
	{	return m_maxMn;	}

	void SetMinCarbon(CChem::Bound newVal)
	{	m_minC = newVal;	}

	void SetMaxCarbon(CChem::Bound newVal)
	{	m_maxC = newVal;	}

	void SetMinMn(CChem::Bound newVal)
	{	m_minMn = newVal;	}

	void SetMaxMn(CChem::Bound newVal)
	{	m_maxMn = newVal;	}

	CTime StartTime() const
	{	return m_startTime;	}

	void SetStartTime(const CTime& newVal)
	{	m_startTime = newVal; m_isStartTimeValid = true;	}

	bool IsStartTimeValid() const
	{	return m_isStartTimeValid;	}

	void SetIsStartTimeValid(bool newVal)
	{	m_isStartTimeValid = newVal;	}


	CTimeSpan TurnaroundTime() const
	{	return m_turnaroundTime;	}

	void SetTurnaroundTime(const CTimeSpan& newVal)
	{	m_turnaroundTime = newVal;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringMiscProps(int h, 
						 int casterNum);

	CCastStringMiscProps(int h, 
						 int minSize,
						 int aimSize,
						 int maxSize,
						 CTimeSpan turnaroundTime);

	// default d-tor, copy c-tor okay.

	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

	bool operator==(const CCastStringMiscProps& x) const
	{
		return m_aimHeatSize == x.m_aimHeatSize
			   &&
			   m_minHeatSize == x.m_minHeatSize
			   &&
			   m_maxHeatSize == x.m_maxHeatSize
			   &&
			   m_numHeats == x.m_numHeats 
			   && 
			   m_initSpec == x.m_initSpec
			   &&
			   m_pInitSpec == x.m_pInitSpec
			   &&
			   m_startWidths[1] == x.m_startWidths[1]
			   &&
			   m_startWidths[2] == x.m_startWidths[2]
			   &&
			   m_endWidths[1] == x.m_endWidths[1]
			   &&
			   m_endWidths[2] == x.m_endWidths[2]
			   &&
			   m_minC == x.m_minC
			   &&
			   m_maxC == x.m_maxC
			   && 
			   m_minMn == x.m_minMn
			   &&
			   m_maxMn == x.m_maxMn
			   &&
			   m_startTime == x.m_startTime
			   &&
			   m_isStartTimeValid == x.m_isStartTimeValid
			   &&
			   m_turnaroundTime == x.m_turnaroundTime;
;
	}

	bool operator!=(const CCastStringMiscProps& x) const
	{
		return ! ( x == *this );
	}

};


#endif // _castStringMiscProps_included_