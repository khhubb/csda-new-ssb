
#ifndef _nTime_included_
#define _nTime_included_



namespace NTime  
{
	////////////////////////////////////////////////////////////////
	//	
	//	Namespace providing some functions for manipulating times.
	//	
	////////////////////////////////////////////////////////////////

 	CTime ThisDayMidnight(CTime now = CTime::GetCurrentTime());
	time_t ThisDayMidnight(time_t now);
		//	Returns the CTime/time_t corresponding to midnight 
		//		beginning the day of the input CTime/time_t.

	int ThisYear(CTime now = CTime::GetCurrentTime());
	int ThisYear(time_t now);
		//	Extract the year from a time_t.
		//	Arg defaults to current time.

	int ThisMonth(CTime now = CTime::GetCurrentTime());
	int ThisMonth(time_t now);
		//	Extract the month from a time_t.
		//	Arg defaults to current time.
	
	CTime NextDayMidnight( CTime time );
	time_t NextDayMidnight( time_t time);
		//	Returns the time_t corresponding to midnight after the day
		//		of the input time_t.

	CTime ThisDayAtHour( CTime time, int hour );
	time_t ThisDayAtHour( time_t time, int hour);
		// Given a time and an hour (0-23), compute a time_t
		//  for that hour on the day of the given time.

	void WeekOfYear(CTime d, int& rWk, int& rYr );
	void WeekOfYear(time_t d, int& rWk, int& rYr);
		// compute week and year of a given date
		// according to Inland's method of counting the first week.


	time_t ConvertTime(int year,
						  int month,
						  int day,
						  int hour,
						  int minute,
						  int second);
	// No need for a CTime equivalent -- it has a c-tor with these args.


	int Print(char* buffer, int size, char const* format, time_t time);
	int Print(char* buffer, int size, char const* format, CTime time);
	void Print(ostream& ostr, char const* format, time_t time);
	void Print(ostream& ostr, char const* format, CTime time);
	void Print(CString& str, char const* format, time_t time);
	void Print(CString& str, char const* format, CTime time);

	int PrintWithHyphens(char* buffer, int size, CTime time);
	int PrintWithHyphens(char* buffer, int size, time_t time );
	void PrintWithHyphens(ostream& ostr, CTime time);
	void PrintWithHyphens(ostream& ostr, time_t time);
	void PrintWithHyphens(CString& str, CTime time);
	void PrintWithHyphens(CString& str, time_t time);

	long PSDateDelta(long psd);
	long PSDateDeltaWeek(long psd);
	long PSDateDeltaWeekTruncated(long psd);

};





#endif //  _nTime_included_
