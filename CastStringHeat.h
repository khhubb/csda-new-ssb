// CastStringHeat.h: interface for the CCastStringHeat class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_CASTSTRINGHEAT_H__923F7F3F_7E17_11D1_805C_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGHEAT_H__923F7F3F_7E17_11D1_805C_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Chem.h"
#include "CSOrder.h"

class CCSOrder;
class CDalyPlanSet;
class CSSBOutput;
class CCasterStringHeatsSet;
class CChemRange;


class CCastStringHeat 
{
	////////////////////////////////////////////////////////////////
	//	
	//	A heat in a cast string
	//	
	////////////////////////////////////////////////////////////////

	//  A CCastString needs to manipulate at will
	friend class CCastString;


	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_HeatStatus { HEAT_STATUS_910,
					  HEAT_STATUS_909,
					  HEAT_STATUS_NEW };
	

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CTimeSpan m_dpaDuration;
		// the downloaded daily plan has this field
		// not used.

	CTimeSpan m_duration;
		//  The computed duration of the heat.

	double m_relStartTime;
	double m_relFinishTime;
		// relative start and finish times, in seconds from the start of the string

	CTime m_absStartTime;
	CTime m_absFinishTime;
		// Absolute start and finish times, as calculated on the hot metal editor

	long m_heatTons;
		// Number of tons assigned to this heat.

	CString m_spec;
	CSpec* m_pSpec;
		// the heat spec

	CChem::Bound m_minChems[CChem::NumElements];
	CChem::Bound m_maxChems[CChem::NumElements];
	CChem::Bound m_aimChems[CChem::NumElements];
		// chemistry bounds on the heat.
	
	CChem::Element m_signifEl;
		// computed by mysterious means, the significant element

	int m_caster;
		// the caster this heat is for, from the CCastString this heat belongs to

	long m_tabNum;
		// the tab# assigned to this heat

	long m_origTabNum;
		// the tab# assigned to this heat when we downloaded from the DPA database

	int m_condnCode;
	int m_dispCode;
		// heat condition and disposition codes

	vector<CCSOrder*>::iterator m_strandBegins[3]; // strandNum = 1, 2
	vector<CCSOrder*>::iterator m_strandEnds[3];   // strandNum = 1, 2
		// position 0 not used
		// each CCastString keeps vectors of CSOrders for each strand.
		// heats are contiguous sets of orders from each strand
		// these delimit the set of CSOrders assigned to this heat.

	bool m_isMarked;  
		// marked for handoff, i.e. to be sent as 910

	E_HeatStatus m_status;
		// the heat status, 910 or not.

	CString m_comment;
		// The user can assign a comment per heat.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	E_HeatStatus Status() const
	{	return m_status;			}
	
	void Status(E_HeatStatus newVal)
	{	m_status = newVal;		}


	CTimeSpan Duration() const
	{	return m_duration;		}

	double RelStartTime() const
	{	return m_relStartTime;	}

	double RelFinishTime() const
	{	return m_relFinishTime;	}

	CTime AbsStartTime() const
	{	return m_absStartTime;	}

	CTime AbsFinishTime() const
	{	return m_absFinishTime;	}


	long HeatTons() const
	{	return m_heatTons;		}

	int CondnCode() const
	{	return m_condnCode;		}

	void CondnCode(int newVal)
	{	m_condnCode = newVal;		}

	int DispCode() const
	{	return m_dispCode;		}

	void DispCode(int newVal)
	{	m_dispCode = newVal;		}

	const CString& Spec() const
	{	return m_spec;			}

	void SetSpec(CString newVal, int caster);
	
	CChem::Element SignifEl() const
	{	return m_signifEl;		}

	CSpec* SpecPtr() const
	{	return m_pSpec;			}

	CChem::Bound MinChem(CChem::Element elt) const
	{	return m_minChems[elt];	}

	CChem::Bound MaxChem(CChem::Element elt) const
	{	return m_maxChems[elt];	}

	CChem::Bound AimChem(CChem::Element elt) const
	{	return m_aimChems[elt];	}

	void MinChem(CChem::Element elt, CChem::Bound newVal)
	{	m_minChems[elt] = newVal;	}

	void AimChem(CChem::Element elt, CChem::Bound newVal)
	{	m_aimChems[elt] = newVal;	}

	void MaxChem(CChem::Element elt, CChem::Bound newVal)
	{	m_maxChems[elt] = newVal;	}


	long TabNum() const
	{	return m_tabNum;			}


	void TabNum(long newVal )
	{	m_tabNum = newVal;		}

	long OrigTabNum() const
	{	return m_origTabNum;		}


	const CString& Comment() const
	{	return m_comment;		}

	void Comment(const CString& newVal)
	{	m_comment = newVal;	}

	vector<CCSOrder*>::const_iterator StrandBegin(int strandNum) const
	{
		assert( strandNum == 1 || strandNum == 2 );
		return m_strandBegins[strandNum];
	}

	vector<CCSOrder*>::iterator StrandBegin(int strandNum)
	{
		assert( strandNum == 1 || strandNum == 2 );
		return m_strandBegins[strandNum];
	}


	void SetStrandBegin(int strandNum, vector<CCSOrder*>::iterator io)
	{
		assert( strandNum == 1 || strandNum == 2 );
		m_strandBegins[strandNum] = io;
	}
	
	vector<CCSOrder*>::const_iterator StrandEnd(int strandNum) const
	{
		assert( strandNum == 1 || strandNum == 2 );
		return m_strandEnds[strandNum];
	}

	vector<CCSOrder*>::iterator StrandEnd(int strandNum)
	{
		assert( strandNum == 1 || strandNum == 2 );
		return m_strandEnds[strandNum];
	}

	void SetStrandEnd(int strandNum, vector<CCSOrder*>::iterator io)
	{
		assert( strandNum == 1 || strandNum == 2 );
		m_strandEnds[strandNum] = io;
	}

	bool IsMarked() const
	{	return m_isMarked;	}

	void IsMarked(bool newVal) 
	{	m_isMarked = newVal;	}



	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

	CCastStringHeat();
	CCastStringHeat(CCasterStringHeatsSet& rHeatsSet,
					CCastString* pCastString);


	// copy c-tor, op= okay	
	// d-tor okay.

	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CCastStringHeat& x) const
	{	return m_relStartTime < x.m_relStartTime;	}

	bool operator==(const CCastStringHeat& x) const
	{	return m_relStartTime == x.m_relStartTime;	}


	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

	void Init(int caster,
			  CDalyPlanSet& planSet);

	void Init(int caster,
			  CSSBOutput& sbSet);

	void Init(CCasterStringHeatsSet& rHeatsSet,
			  CCastString* pCastString);


	void SetSpecPtr(int caster);

	void NullifySpecPtr()
	{	m_pSpec = 0;	}


	void ResetChems();

	bool SetChemsFromHeatSpec(ostrstream& ostr, int heatNum, int caster);
	bool FixAimChems(ostrstream& ostr, int heatNum, int caster);
	bool FixAimChem(CChem::Element elt, int caster);
	bool FixAimChem(const CChemRange* pRange,
					CChem::Element elt, 
					int caster);

	bool IntersectChems( const CCSOrder* pOrder, vector<bool>& badChems );

	bool IsSameHeat( const CCastStringHeat& rHeat ) const;


	void ComputeApproximateStrandBalance(int aimHeatSize, int& strand1Tons, int& strand2Tons) const;

	void ComputePlannedLatestStart(long& startDate, long& startTime);

};



#endif // !defined(AFX_CASTSTRINGHEAT_H__923F7F3F_7E17_11D1_805C_006097B38214__INCLUDED_)
