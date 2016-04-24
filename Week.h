// Week.h: interface for the CWeek class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_WEEK_H__642EE48A_E2B3_11D0_8019_006097B38214__INCLUDED_)
#define AFX_WEEK_H__642EE48A_E2B3_11D0_8019_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWeek  
{
	////////////////////////////////////////////////////////////////
	//	
	// Represents a week/year.
	//	
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	int m_year;
	int m_week;



	////////////////////////////////////////////////////////////////
	//	
	//	Accessors/modifiers
	//	
	////////////////////////////////////////////////////////////////

public:

	int Year() const { return m_year; }
	int Week() const { return m_week; }

	void Set(int year, int week);


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CWeek(CTime time);
	CWeek();
	CWeek(int year_, int week_)
	{	Set(year_, week_); }

	//  supplied d-tor, copy c-tor, etc okay.


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CWeek& x) const;
	bool operator<=(const CWeek& x) const;
	bool operator>(const CWeek& x) const;
	bool operator>=(const CWeek& x) const;

	bool operator==(const CWeek& x) const;
	bool operator!=(const CWeek& x) const 
	{ return ! operator==(x); }

	int operator-(const CWeek& x) const;
	
	// TODO someday
	//CWeek operator-(int x) const;
	//CWeek operator+(int x) const;
};

#endif // !defined(AFX_WEEK_H__642EE48A_E2B3_11D0_8019_006097B38214__INCLUDED_)
