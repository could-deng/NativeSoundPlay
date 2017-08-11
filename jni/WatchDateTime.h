#ifndef WATCH_DATETIME_H
#define WATCH_DATETIME_H

//#include "GUI.h"
#include "WatchDefine.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

typedef  I64 structTimeStamp;

typedef  I64 structTimeStampDiff;


structTimeStamp getCurrentTime(void);
BOOL isLeapYear(int year);

int getDaysOfMonth(int month, BOOL bLeap);
int getDaysFromStartForYear(int year);
int getDaysFormThisYearForMonth(int year, int month);
int getDaysFromThisYearForDay(int year, int month, int day);
int getDaysFromStart(structTimeStamp time);
int getMillSeconsThatDay(structTimeStamp time);

BOOL isInSameWeek(structTimeStamp time1, structTimeStamp time2);
BOOL isInSameMonth(structTimeStamp time1, structTimeStamp time2);
BOOL isTheSameDay(structTimeStamp time1, structTimeStamp time2);
BOOL isToday(structTimeStamp time);
BOOL isThisWeek(structTimeStamp time);
BOOL isThisMonth(structTimeStamp time);

BOOL getDateOfYear(structTimeStamp time, int *pYear, int *pMonth, int *pDay);
int getYearOfTime(structTimeStamp time);
int getMonthOfTime(structTimeStamp time);
int getDayOfTime(structTimeStamp time);
int getWeekOfTime(structTimeStamp time);
int getHourOfTime(structTimeStamp time);
int getMinuteOfTime(structTimeStamp time);
int getSecondOfTime(structTimeStamp time);
int getMillSecondOfTime(structTimeStamp time);

structTimeStamp getTimeStamp(int days, int millSecons);
int getDaysForTimeStamp(int year, int month, int day);
int getMillSecondsForTimeStamp(int hour, int minute, int second, int millSecons);
structTimeStamp getTimeStampByPara(int year, int month, int day, int hour, int minute, int second, int millSecons);
int getDaysForTimeStampByString(char * pString);
int getMillSecondsForTimeStampByString(char * pString);
BOOL getDateStringForTimeStamp(structTimeStamp stamp, char * pBuffer, int iBufLen);
BOOL getTimeStringForTimeStamp(structTimeStamp stamp, char * pBuffer, int iBufLen);

structTimeStamp addTimeStamp(structTimeStamp stamp, int millSecons);
int compareTimeStamp(structTimeStamp time1, structTimeStamp time2);

structTimeStampDiff getTimeDiffByPara(int days, int millSecons);
structTimeStampDiff getTimeDiff(structTimeStamp stamp1, structTimeStamp stamp2);
structTimeStampDiff addTimeStampDiff(structTimeStampDiff diff1, structTimeStampDiff diff2);

int getYearOfCurrentTime(void);
int getMonthOfCurrentTime(void);
int getDayOfCurrentTime(void);
int getWeekOfCurrentTime(void);
int getHourOfCurrentTime(void);
int getMinuteOfCurrentTime(void);
int getSecondOfCurrentTime(void);
int getMillSecondOfCurrentTime(void);

int getDayOfDiff(structTimeStampDiff diff);
int getHourOfDiff(structTimeStampDiff diff);
int getMinuteOfDiff(structTimeStampDiff diff);
int getSecondOfDiff(structTimeStampDiff diff);
int getMillSecondOfDiff(structTimeStampDiff diff);
int getTotalMillSecondsOfDiff(structTimeStampDiff diff);

BOOL testDateTime(void);

#if defined(__cplusplus)
}
#endif 

#endif //WATCH_DATETIME_H



