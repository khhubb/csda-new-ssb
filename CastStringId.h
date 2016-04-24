#ifndef _castStringId_included_
#define _castStringId_included_

 
class CDalyPlanSet;
class CSSBOutput;


class CCastStringId
{
	
	////////////////////////////////////////////////////////////////
	//	
	//	Identifies a cast string
	//	
	////////////////////////////////////////////////////////////////

	// A CCastString ought to be able to change its id any way it wants
	friend class CCastString;


	////////////////////////////////////////////////////////////////
	//	
	//	Data
	//	
	////////////////////////////////////////////////////////////////

private:

	int m_year;
	int m_week;
	int m_caster;
	int m_stringId;

	////////////////////////////////////////////////////////////////
	//	
	//	Accessors
	//	
	////////////////////////////////////////////////////////////////
	
public:

	int Year() const
	{	return m_year;		}

	int Week() const
	{	return m_week;		}

	int Caster() const
	{	return m_caster;		}

	int StringId() const 
	{	return m_stringId;	}


	////////////////////////////////////////////////////////////////
	//	
	//	C-tors, d-tor
	//	
	////////////////////////////////////////////////////////////////

public:

	CCastStringId();
	CCastStringId(int year_,
			     int week_,
				 int caster_,
				 int stringId_);

	CCastStringId(const CDalyPlanSet&);
	CCastStringId(const CSSBOutput&);

	// default copy c-tor, op=  okay.


	////////////////////////////////////////////////////////////////
	//	
	//	Operators
	//	
	////////////////////////////////////////////////////////////////

public:

	bool operator<(const CCastStringId&) const;
	bool operator==(const CCastStringId&) const;
	bool operator!=(const CCastStringId&) const;

};





#endif // _castStringId_included_