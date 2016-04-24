// CastStringHeatValidnError.h: interface for the CCastStringHeatValidnError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTSTRINGHEATVALIDNERROR_H__A0736FA7_99F9_11D1_8066_006097B38214__INCLUDED_)
#define AFX_CASTSTRINGHEATVALIDNERROR_H__A0736FA7_99F9_11D1_8066_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class CCastStringHeat;
class CCSOrder;


class CCastStringHeatValidnError  
{

	////////////////////////////////////////////////////////////////
	//	
	//	Typedefs, etc.
	//	
	////////////////////////////////////////////////////////////////

public:

	enum E_Severity { FATAL, WARNING };
	
	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	CString m_msg;
		// describes the error

	int m_heatIndex;
	int m_strandNum;
	int m_lotIndex;
		// indicates the location of the error, if applicable

	CCastStringHeat* m_pHeat;
	CCSOrder* m_pCSOrder;
		// The head and order involved in the error, if application

	E_Severity m_severity;
		// the severity of the error.


	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////

public:

	CString Msg() const
	{	return m_msg;	}

	const CCastStringHeat* Heat() const
	{	return m_pHeat;	}

	int HeatIndex() const
	{	return m_heatIndex;	}

	const CCSOrder* CSOrder() const
	{	return m_pCSOrder;	}

	int LotIndex() const
	{	return m_lotIndex;	}

	int StrandNum() const
	{	return m_strandNum;	}

	E_Severity Severity() const
	{	return m_severity;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringHeatValidnError() 
		:	m_heatIndex(-1),
			m_strandNum(0),
			m_lotIndex(-1),
			m_pHeat(0),
			m_pCSOrder(0),
			m_msg(""),
			m_severity(FATAL)
	{
	}

	CCastStringHeatValidnError(int heatIndex,
							   CCastStringHeat* pHeat,
							   int strandNum,
							   int lotIndex,
							   CCSOrder* pCSOrder,
							   const CString& string,
							   E_Severity severity)
							   : m_heatIndex(heatIndex),
							     m_strandNum(strandNum),
								 m_lotIndex(lotIndex),
							     m_pHeat(pHeat),
								 m_pCSOrder(pCSOrder),
								 m_severity(severity),
								 m_msg(string)
	{
		// do nothing
	}

	CCastStringHeatValidnError(int heatIndex,
							   CCastStringHeat* pHeat,
							   int strandNum,
							   int lotIndex,
							   CCSOrder* pCSOrder,
							   ostrstream& ostr,
							   E_Severity severity)
							   : m_heatIndex(heatIndex),
							     m_strandNum(strandNum),
								 m_lotIndex(lotIndex),
							     m_pHeat(pHeat),
								 m_pCSOrder(pCSOrder),
								 m_severity(severity),
								 m_msg(ostr.str())
	{
		ostr.freeze(false);
		ostr.seekp(0);
	}
	
	
	~CCastStringHeatValidnError() {}


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	//sigh

	bool operator<(const CCastStringHeatValidnError& x) const
	{
		return  m_heatIndex < x.m_heatIndex
				||
				(m_heatIndex == x.m_heatIndex
				 &&
				 m_strandNum < x.m_strandNum)
				||
				(m_heatIndex == x.m_heatIndex
				 &&
				 m_strandNum == x.m_strandNum
				 &&
				 m_lotIndex < x.m_lotIndex );
	}

	bool operator==(const CCastStringHeatValidnError& x) const
	{
		return m_heatIndex == x.m_heatIndex
			   &&
			   m_strandNum == x.m_strandNum
			   &&
			   m_lotIndex  == x.m_lotIndex 
			   &&
			   m_msg       == x.m_msg ;
	}



	////////////////////////////////////////////////////////////////
	//	
	//	Operations
	//	
	////////////////////////////////////////////////////////////////

public:

	const CString& SeverityAsString() const;
	
	static void SetHeader(CListCtrl& list);
	static void UpdateList(CListCtrl& list, const vector<CCastStringHeatValidnError>& errors);


	////////////////////////////////////////////////////////////////
	//	
	//	Implementation
	//	
	////////////////////////////////////////////////////////////////


};

#endif // !defined(AFX_CASTSTRINGHEATVALIDNERROR_H__A0736FA7_99F9_11D1_8066_006097B38214__INCLUDED_)
