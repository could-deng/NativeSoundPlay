#ifndef WATCH_SPORTS_SENSOR_DATA_H
#define WATCH_SPORTS_SENSOR_DATA_H

//#include "WatchDateTime.h"
//#include "WatchSummaryData.h"
#include "MiniDICOM.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif



BOOL readSensorFileHead(const char * filename, enumSensor sensor, char * pBuffer, int *pBufLen);
int getSensorFileDataLen(const char * filename, int iHeadLen);
BOOL readSensorFileDataChapter(const char * filename, char * pBuffer, int *pBufLen, int iHeadLen, int iChapterSize, int iChapterIndex);
BOOL getSensorField(enumSensor sensor, char * pBuffer, int iBufLen, U8 field, int * pValue);
BOOL getSensorDataVersion(enumSensor sensor, char * pBuffer, int iBufLen, int *pVerNO);
BOOL getSensorDataDate(enumSensor sensor, char * pBuffer, int iBufLen, int *pDate);
BOOL getSensorDataTime(enumSensor sensor, char * pBuffer, int iBufLen, int *pTime);
BOOL getSensorDataTimeSpace(enumSensor sensor, char * pBuffer, int iBufLen, int *pTimeSpace);;
BOOL getSensorDataCellDataLen(enumSensor sensor, char * pBuffer, int iBufLen, int *pCellDataLen);;
BOOL getSensorDataIsSport(enumSensor sensor, char * pBuffer, int iBufLen, int *pIsSport);
BOOL getSensorDataTotalDataLen(enumSensor sensor, char * pBuffer, int iBufLen, int *pTotalDataLen);
BOOL getSensorDataStartAddress(enumSensor sensor, char * pBuffer, int iBufLen, int *pAddress);
int getSportsSensorDataPointsCount(const char * filename, int iHeadLen, int iCellLen);
void parseHRFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, const char * pJasonFilename);
void parseStepFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename);
void parseDistanceFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename);
void parseGPSFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename);
BOOL parseGPSFileWithSpeed(const char * pGPSFile, char * pGPSInBuffer, int iGPSInBufLen, \
						   const char * pDistanceFile, char * pDistanceInBuffer, int iDistanceInBufLen, \
						   const char * pJasonFilename, char * pOutBuffer, int iOutBufLen);
BOOL parseDistanceAndStepAndHRFile(const char * pStepFile, char * pStepInBuffer, int iStepInBufLen, \
						   const char * pDistanceFile, char * pDistanceInBuffer, int iDistanceInBufLen, \
						   const char * pHRFile, char * pHRInBuffer, int iHRInBufLen, \
						   const char * pJasonFilename, char * pOutBuffer, int iOutBufLe);

#if defined(__cplusplus)
}
#endif 

#endif //WATCH_SPORTS_SENSOR_DATA_H



