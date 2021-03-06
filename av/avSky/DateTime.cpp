/* -*-c++-*- OpenSceneGraph - Ephemeris Model Copyright (C) 2005 Don Burns
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include "stdafx.h"
#include "DateTime.h"

using namespace avSky;

char *DateTime::weekDayNames[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

char *DateTime::monthNames[12] = {
    "January",
    "Feburary",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

DateTime::DateTime(
            int year,    
            int month,   
            int day,     
            int hour,    
            int minute,  
            int second  )
{
    _tm.tm_year = year - 1900;
    _tm.tm_mon  = month - 1;
    _tm.tm_mday = day;
    _tm.tm_hour = hour;
    _tm.tm_min  = minute;
    _tm.tm_sec  = second;
    _tm.tm_wday = -1;
    _tm.tm_yday = -1;
    _tm.tm_isdst= -1;

    mktime(&_tm);
}

DateTime::DateTime( const DateTime &dt ):
    _tm(dt._tm)
{
    mktime(&_tm);
}

DateTime::DateTime() 
{ 
    //now();
}

DateTime::DateTime( const struct tm &tm )
{
    _tm = tm;
    mktime( &_tm );
}

void DateTime::now()
{
    time_t utc = time(0L);
    localtime_s(&_tm,&utc);
}

void DateTime::setYear( int year  )
{
    _tm.tm_year = year - 1900;
    mktime(&_tm);
}

int DateTime::getYear() const
{
    return _tm.tm_year + 1900;
}

void DateTime::setMonth(int month)
{
    _tm.tm_mon = month - 1;
    mktime(&_tm);
}
int DateTime::getMonth() const
{
    return _tm.tm_mon + 1;
}

std::string DateTime::getMonthString() const
{
    return std::string( monthNames[_tm.tm_mon] );
}

std::string DateTime::getMonthString(int month) // Will pass in 1-12
{
    return std::string( monthNames[(month-1)%12] );
}

void DateTime::setDayOfMonth(int day) 
{
    _tm.tm_mday = day;
    mktime(&_tm);
}

int DateTime::getDayOfMonth() const
{
    return _tm.tm_mday;
}

int DateTime::getDayOfYear() const
{
    return _tm.tm_yday;
}

int DateTime::getDayOfWeek() const
{
    return _tm.tm_wday;
}

std::string DateTime::getDayOfWeekString() const
{
    return std::string(weekDayNames[getDayOfWeek()]);
}

std::string DateTime::getDayOfWeekString(int wday) 
{
    return std::string(weekDayNames[wday%7]);
}

void DateTime::setHour( int hour )
{
    _tm.tm_hour = hour;
    mktime(&_tm);
}

int DateTime::getHour() const
{
    return _tm.tm_hour;
}

void DateTime::setMinute( int minute )
{
    _tm.tm_min = minute;
    mktime(&_tm);
}

int DateTime::getMinute() const
{
    return _tm.tm_min;
}

void  DateTime::setSecond( int second )
{
    _tm.tm_sec = second;
    mktime(&_tm);
}

int DateTime::getSecond() const
{
    return _tm.tm_sec;
}

bool DateTime::isDaylightSavingsTime() const
{
    return (_tm.tm_isdst != 0);
}

double DateTime::getModifiedJulianDate() const
{
    double mjd;
    int b, d, m, y;
    long c;

    // attention - time is UTC already!
    struct tm gmt = _tm;
    mktime(&gmt);

    double day   =  (double)(gmt.tm_mday) +           // Day
                    (double(gmt.tm_hour)/24.0) +      // hour
                    (double(gmt.tm_min)/(24.0*60)) +   // minutes
                    (double(gmt.tm_sec)/(24*3600.0));  // seconds

    int month = gmt.tm_mon + 1;
    m = month;
    int year = gmt.tm_year + 1900;
    y = (year < 0) ? year + 1 : year;
    if( month < 3)
    {
        m += 12;
        y -= 1;
    }

    if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day < 15))))
    {
        b = 0;
    }
    else
    {
        int a;
        a = y/100;
        b = 2 - a + a/4;
    }

    if (y < 0)
    {
        c = (long)((365.25*y) - 0.75) - 694025L;
    }
    else
    {
        c = (long)(365.25*y) - 694025L;
    }

    d = (int)(30.6001*(m+1));

    mjd = b + c + d + day - 0.5;

    return mjd;
}

DateTime DateTime::getGMT() const
{
    struct tm gmt = _tm;

    _tzset();
    long timezone_difference = 0;
    _get_timezone(&timezone_difference);
    gmt.tm_sec += timezone_difference;

    mktime(&gmt);

    return DateTime(gmt);
}

