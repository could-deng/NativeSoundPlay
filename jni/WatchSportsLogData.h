#ifndef WATCH_SPORTS_DATA_LOG_H
#define WATCH_SPORTS_DATA_LOG_H

#include "WatchDateTime.h"
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
BOOL parseLogFile(const char * pFilename, char * pBuffer, int iBufLen, char * pJasonString);
BOOL isInPauseState(int time);
structTimeStamp getSportStartTime(void);

#if defined(__cplusplus)
}
#endif 

#endif //WATCH_SPORTS_DATA_LOG_H



