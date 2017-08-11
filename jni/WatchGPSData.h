#ifndef WATCH_GPS_DATA_H
#define WATCH_GPS_DATA_H

//#include "WatchDateTime.h"
//#include "WatchSummaryData.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


typedef struct{
int longtitude;
int latitude;
short altitude;
U8 accuracy;
U8 unused;
}structGPSPos;


#if defined(__cplusplus)
}
#endif 

#endif //WATCH_GPS_DATA_H



