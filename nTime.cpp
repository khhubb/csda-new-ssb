#include "stdafx.h"



#ifndef _nTime_included_
#include "nTime.h"
#endif


#include <iostream>


//
//  Retrieve the current time and extract the year.
//

int NTime::ThisYear(time_t now)
{
	struct tm* tmPtr = localtime(&now);

	return 1900+tmPtr->tm_year;
}

int NTime::ThisYear(CTime now)
{
	return now.GetYear();
}


//
//  Retrieve the current time and extract the month.
//

int NTime::ThisMonth(time_t now)
{
	struct tm *tmPtr = localtime(&now);

	return tmPtr->tm_mon;
}

int NTime::ThisMonth(CTime now)
{
	return now.GetMonth();
}


//
// Returns the time_t corresponding to midnight beginning the day
//   of the input time_t.
//

time_t NTime::ThisDayMidnight( time_t time)
{
	struct tm *timeptr = localtime(&time);
	
	timeptr->tm_hour = 0;
	timeptr->tm_min  = 0;
	timeptr->tm_sec  = 0;

	return mktime(timeptr);
}


CTime NTime::ThisDayMidnight(CTime time)
{
	return CTime( time.GetYear(),
				  time.GetMonth(),
				  time.GetDay(),
				  0,0,0 );
}


//
// Returns the time_t corresponding to midnight after the day
//   of the input time_t.
//

time_t NTime::NextDayMidnight( time_t time)
{
	struct tm *timeptr = localtime(&time);
	
	timeptr->tm_hour = 0;
	timeptr->tm_min  = 0;
	timeptr->tm_sec  = 0;
	timeptr->tm_mday++;

	return mktime(timeptr);
}


CTime NTime::NextDayMidnight( CTime time )
{
	return ThisDayMidnight(time) + CTimeSpan(1,0,0,0);
}


//
// Given a time and an hour (0-23), compute a time_t
//  for that hour on the day of the given time.
//

time_t NTime::ThisDayAtHour( time_t time, int hour)
{
	struct tm *timeptr = localtime(&time);
	
	timeptr->tm_hour = hour;
	timeptr->tm_min  = 0;
	timeptr->tm_sec  = 0;

	return mktime(timeptr);
}

CTime NTime::ThisDayAtHour(CTime time, int hour)
{
	return CTime( time.GetYear(),
				  time.GetMonth(),
				  time.GetDay(),
				  hour,
				  0,0);
}

//
//  Calculation of the week
//
//   By Inland standards, the week of Jan 1 in a given year
//       is week 1 if Jan 1 falls on Su/Mo/Tu/We, or it
//       is the last week of the previous year if Jan 1 falls on Th/Fr/Sa.
//
//   So we need to return both the week # and the year when calculating.
//
//   To make the calcuation:
//
//   Let  d  = the day we wish to calculate the time for.
//   Let  dy = the day of the year of d (0-365, 0=Jan 1)
//   Let  dw = the day of the week of d (0-6, 0=Sun)
//   Let  ds = the day of the year of sunday of the week d falls in
//           = dy-dw
//
//   Let  bw = the day of the week of Jan 1 of the given year (0-6, 0=Sun)
//   Let  bs = the day of the year of sunday of the week b falls in
//             (since this is Jan 1, this ranges from 0 (if Jan 1 is on Sunday)
//                                                to -6 (if Jan 1 is on Saturday)
//           = -bw
//
//   Let  nw = number of elapsed weeks from Jan 1's Sunday to d's Sunday 
//           = (ds - bs)/7.
//
//   Thus, if bw <= 3 (so Jan 1 falls on Su/Mo/Tu/We)
//            then  week = nw + 1
//                  year = this year
//
//         else (Jan 1 falls on Th/Fr/Sa)
//               if nw == 0 
//                  then compute week of Dec 31 of previous year
//               else week = nw
//                    year = this year
//
      
void NTime::WeekOfYear(time_t d, int& rWk, int& rYr)
{
	int year = ThisYear(d);

	struct tm* dtmPtr = localtime(&d);
	int ds = dtmPtr->tm_yday - dtmPtr->tm_wday;

	time_t jan1 = ConvertTime(year,1,1,0,0,0);

	struct tm* btmPtr = localtime(&jan1);
	int bw = btmPtr->tm_wday;
	int bs = - bw;

	int nw = (ds-bs)/7;

	if ( bw <= 3 ) {

		rYr = year;
		rWk = nw+1;

	}
	else if ( nw == 0 ) {

		time_t dec31previous = ConvertTime(year-1,12,31,0,0,0);
		WeekOfYear(dec31previous,rWk,rYr);

	}
	else {

		rYr = year;
		rWk = nw;

	}
}


void NTime::WeekOfYear(CTime d, int& rWk, int& rYr)
{
	WeekOfYear(d.GetTime(),rWk,rYr);
}



//
//  Convert values representing YYYY/MM/DD/HH:MM:SS to a time_t
//  Years can either be relative to 1900 (if <= 1900)
//     or absolute (if > 1900).
//

time_t NTime::ConvertTime(int year,
						  int month,
						  int day,
						  int hour,
						  int minute,
						  int second)
{
	struct tm tmStruct;

	if ( year > 1900 )
		year -= 1900;

	tmStruct.tm_year = year;
	tmStruct.tm_mon  = month-1;
	tmStruct.tm_mday = day;
	tmStruct.tm_hour = hour;
	tmStruct.tm_min  = minute;
	tmStruct.tm_sec  = second;

	time_t converted = mktime(&tmStruct);
	struct tm* reconverted = localtime(&converted);

	converted = converted - (reconverted->tm_hour - hour)*3600;

	return(converted);
}




//
//  We have several methods of printing out times.
//  All methods rely on strftime to do the formatting.
//
//  One method prints the Y/m/d/H:M separated by a -.
//  This tends to find use when we are incorporating a time into a filename.
//  
//  One method prints them separated by a |.
//  This can be read back in using the ifRead mechanism.
//
//  We also allow the user to specify the format string directly.
//  This becomes a direct call to strftime if we are writing to a char* buffer.
//  We supply a stream-oriented version.
//


int NTime::Print(char* buffer, int size, char const* format, time_t time)
{
	// ## add cast to int
	return (int)strftime(buffer,size,format,localtime(&time));
}

int NTime::Print(char* buffer, int size, char const* format, CTime time)
{
	return Print(buffer,size,format,time.GetTime());
}

void NTime::Print(ostream& ostr, char const* format, time_t time)
{
	char buffer[100];
	strftime(buffer,100,format,localtime(&time));
	ostr << buffer;
}  

void NTime::Print(ostream& ostr, char const* format, CTime time)
{
	Print(ostr,format,time.GetTime());
}  

void NTime::Print(CString& str, char const* format, CTime time)
{
	str = time.Format(format);
}

void NTime::Print(CString& str, char const* format, time_t time)
{
	Print(str,format,CTime(time));
}


int NTime::PrintWithHyphens(char *buffer, int size, time_t time )
{
	// ## add cast to int
	return (int)strftime(buffer,size,"%Y-%m-%d-%H-%M",localtime(&time));
}

int NTime::PrintWithHyphens(char *buffer, int size, CTime time )
{
	return PrintWithHyphens(buffer,size,time.GetTime());
}


void NTime::PrintWithHyphens(ostream& ostr, time_t time)
{
	char buffer[100];
	strftime(buffer,100,"%Y-%m-%d-%H-%M",localtime(&time));
	ostr << buffer;
}

void NTime::PrintWithHyphens(ostream& ostr, CTime time)
{
	PrintWithHyphens(ostr,time.GetTime());
}

void NTime::PrintWithHyphens(CString& str, CTime time)
{
	str = time.Format("%Y-%m-%d-%H-%M");
}

void NTime::PrintWithHyphens(CString& str, time_t time)
{
	PrintWithHyphens(str,CTime(time));
}



//  Given a long representing a PST date (YYYYMMDD),
//  compute in days from today.

//  Per request Kevin Hubbard, 99-07-01, changed to make all negatives come out as 9999

long NTime::PSDateDelta(long psd)
{
	if ( psd == 0 )
		//return -9999;
		return 9999;
	else {
		// psd = YYYYMMDD

		int year = psd / 10000;
		int month = (psd / 100) % 100;
		int day = psd % 100;
	
		CTime now = CTime::GetCurrentTime();
		CTime today(now.GetYear(),now.GetMonth(),now.GetDay(),0,0,0);

		int yearDelta =  year - today.GetYear();
		if ( yearDelta > 5 )
			return 9999;
		else if ( yearDelta < -5 )
			//return -9999;
			return 9999;

		CTime time(year,month,day,0,0,0);
		CTimeSpan delta = time - today;
		//return delta.GetDays();
		if ( delta.GetDays() < 0 )
			return 9999;
		else
			return delta.GetDays();
	}
}


// Number of weeks from today.
//   difference  ==> result
//   ----------      ------
//        0- 7          0
//        8-14          1
//       15-21          2
//   etc

long NTime::PSDateDeltaWeek(long psd) 
{
	long delta = PSDateDelta(psd);
	
	return (delta == 0 ? 0 : (delta-1) / 7);
}


//  Lump together all results bigger than one month away.

long NTime::PSDateDeltaWeekTruncated(long psd)
{
	long week = PSDateDeltaWeek(psd);
	return min(week,4);
}