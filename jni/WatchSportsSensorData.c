#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "MiniDICOM.h"
#include "MiniDICOMFile.h"
#include "WatchFileIO.h"
#include "WatchGPSData.h"
#include "WatchDateTime.h"
#include "WatchSportsLogData.h"
#include "WatchSportsSensorData.h"

static const U16 group = TAG_GROUP_SENSOR_DATA;

static int getSensorCellLen(enumSensor sensor){
	int iCellLen = 0;

	switch(sensor){
		case SENSOR_GPS:
			iCellLen = sizeof(structGPSPos);
			break;
		case SENSOR_HEARTRATE:
		case SENSOR_HUMITY:
			iCellLen = sizeof(U8);
			break;
		case SENSOR_GSENSOR:
		case SENSOR_PRESSURE:
		case SENSOR_GYRO:
		case SENSOR_DISTANCE:
			iCellLen = sizeof(U32);
			break;
		case SENSOR_TEMPRATURE:
		case SENSOR_COMPASS:
			iCellLen = sizeof(U16);
			break;
	}
	return iCellLen;
}



BOOL readSensorFileHead(const char * filename, enumSensor sensor, char * pBuffer, int *pBufLen){
	if(!filename)return FALSE;
	return readMiniDICOMFileHead(filename, pBuffer, pBufLen, getTagByGroupAndCell(group, getCellByBytes((U8)sensor, SENSOR_FIELD_START_ADDRESS)));
}



int getSensorFileDataLen(const char * filename, int iHeadLen){
	if(!filename)return FALSE;
	return getMiniDICOMFileDataLen(filename, iHeadLen);
}




BOOL getSensorField(enumSensor sensor, char * pBuffer, int iBufLen, U8 field, int * pValue){
	char temp[16];

	if(!getMiniDICOMFileTagValue(pBuffer, iBufLen, getTagByGroupAndCell(group, getCellByBytes((U8)sensor, field)), temp, sizeof(temp)))return FALSE;
	*pValue = atoi(temp);
	return TRUE;
}


BOOL getSensorDataVersion(enumSensor sensor, char * pBuffer, int iBufLen, int *pVerNO){
	return getSensorField(sensor, pBuffer, iBufLen, SENSOR_FIELD_VERNO, pVerNO);
}


BOOL getSensorDataTimeSpace(enumSensor sensor, char * pBuffer, int iBufLen, int *pTimeSpace){
	return getSensorField(sensor, pBuffer, iBufLen, SENSOR_FIELD_TIME_SPACE, pTimeSpace);
}
BOOL getSensorDataCellDataLen(enumSensor sensor, char * pBuffer, int iBufLen, int *pCellDataLen){
	return getSensorField(sensor, pBuffer, iBufLen, SENSOR_FIELD_CELL_DATA_LEN, pCellDataLen);
}
static char * getStringOfItem(char * pOut, const char * pTag, const char * pValue){
	char * p = pOut;

	if(!pOut)return p;
	if(!pTag)return p;
	if(!pValue)return p;

	*p++= '\"';
	strcpy(p, pTag);
	p += strlen(pTag);
	*p++= '\"';
	*p++= ':';
	strcpy(p, pValue);
	p += strlen(pValue);
	return p;
}

static BOOL parseSensorFile(enumSensor sensor, const char * pFile, char * pBuffer, int iBufLen, int *pTimeSpace, int *pCount){
	int iHeadLen = iBufLen;
	int iCellLen;

	if(!pFile)return FALSE;
	if(!pBuffer)return FALSE;
	if(!pTimeSpace)return FALSE;
	if(!pCount)return FALSE;

	if(!readSensorFileHead(pFile, sensor, pBuffer, &iHeadLen))return FALSE;
	if(!getSensorDataTimeSpace(sensor, pBuffer, iHeadLen, pTimeSpace))return FALSE;
	if(!getSensorDataCellDataLen(sensor, pBuffer, iHeadLen, &iCellLen))return FALSE;
	if(iCellLen != getSensorCellLen(sensor))return FALSE;
	if(!readMiniDICOMFileWholeData(pFile, pBuffer, &iBufLen, iHeadLen))return FALSE;
	*pCount = getMiniDICOMFileCellCount(pFile, iHeadLen, iCellLen);
	return TRUE;
}
void parseHRFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, const char * pJasonFilename){
	int iHeadLen = iInBufLen;
	enumSensor sensor = SENSOR_HEARTRATE;
	int i, iTimeSpace, iCellLen, iCount;
	char *p = pOutBuffer;
	char * prefix = "\"HeartRateArray\":[";
	int time;
	U8 * pValue;
	structTimeStamp stamp;
	char temp[50];

	if(!pFilename)return;
	if(!pInBuffer)return;
	if(!pOutBuffer)return;
	if(!pJasonFilename)return;

	if(!readSensorFileHead(pFilename, sensor, pInBuffer, &iHeadLen))return;
	if(!getSensorDataTimeSpace(sensor, pInBuffer, iHeadLen, &iTimeSpace))return;
	if(!getSensorDataCellDataLen(sensor, pInBuffer, iHeadLen, &iCellLen))return;
	if(!readMiniDICOMFileWholeData(pFilename, pInBuffer, &iInBufLen, iHeadLen))return;
	iCount = getMiniDICOMFileCellCount(pFilename, iHeadLen, iCellLen);
	strcpy(p, prefix);
	p += strlen(prefix);

	time = 0;
	stamp = getSportStartTime();
	iTimeSpace *= 1000;
	pValue = (U8 *)pInBuffer;
	for(i = 0; i < iCount; i++, stamp += iTimeSpace, pValue++){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';
		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = ',';
		sprintf(temp, "%d", *pValue);
		p = getStringOfItem(p, "heartrate", temp);
		*p++ = '}';

		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}
	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);

}

void parseStepFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename){
	int iHeadLen = iInBufLen;
	enumSensor sensor = SENSOR_GSENSOR;
	int i, iTimeSpace, iCellLen, iCount;
	char *p = pOutBuffer;
	char * prefix = "\"array\":[";
	int time;
	int * pValue;
	structTimeStamp stamp;
	char temp[50];

	if(!pFilename)return;
	if(!pInBuffer)return;
	if(!pOutBuffer)return;
	if(!readSensorFileHead(pFilename, sensor, pInBuffer, &iHeadLen))return;
	if(!getSensorDataTimeSpace(sensor, pInBuffer, iHeadLen, &iTimeSpace))return;
	if(!getSensorDataCellDataLen(sensor, pInBuffer, iHeadLen, &iCellLen))return;
	if(!readMiniDICOMFileWholeData(pFilename, pInBuffer, &iInBufLen, iHeadLen))return;
	iCount = getMiniDICOMFileCellCount(pFilename, iHeadLen, iCellLen);
	strcpy(p, prefix);
	p += strlen(prefix);

	time = 0;
	stamp = getSportStartTime();
	iTimeSpace *= 1000;
	pValue = (int *)pInBuffer;
	for(i = 0; i < iCount; i++, stamp += iTimeSpace, pValue++){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';
		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = ',';
		sprintf(temp, "%d", *pValue);
		p = getStringOfItem(p, "step", temp);
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}

	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);

}
void parseDistanceFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename){
	int iHeadLen = iInBufLen;
	enumSensor sensor = SENSOR_DISTANCE;
	int i, iTimeSpace, iCellLen, iCount;
	char *p = pOutBuffer;
	char * prefix = "\"array\":[";
	int time;
	int * pValue;
	structTimeStamp stamp;
	char temp[50];

	if(!pFilename)return;
	if(!pInBuffer)return;
	if(!pOutBuffer)return;
	if(!readSensorFileHead(pFilename, sensor, pInBuffer, &iHeadLen))return;
	if(!getSensorDataTimeSpace(sensor, pInBuffer, iHeadLen, &iTimeSpace))return;
	if(!getSensorDataCellDataLen(sensor, pInBuffer, iHeadLen, &iCellLen))return;
	if(!readMiniDICOMFileWholeData(pFilename, pInBuffer, &iInBufLen, iHeadLen))return;
	iCount = getMiniDICOMFileCellCount(pFilename, iHeadLen, iCellLen);
	strcpy(p, prefix);
	p += strlen(prefix);

	time = 0;
	stamp = getSportStartTime();
	iTimeSpace *= 1000;
	pValue = (int *)pInBuffer;
	for(i = 0; i < iCount; i++, stamp += iTimeSpace, pValue++){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';
		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = ',';
		sprintf(temp, "%d", *pValue);
		p = getStringOfItem(p, "distance", temp);
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}

	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);

}
void parseGPSFile(const char * pFilename, char * pInBuffer, int iInBufLen, char * pOutBuffer, int iOutBufLen, char * pJasonFilename){
	int iHeadLen = iInBufLen;
	enumSensor sensor = SENSOR_GPS;
	int i, iTimeSpace, iCellLen, iCount;
	char *p = pOutBuffer;
	char * prefix = "\"array\":[";
	int time, iTimeStep;
	structGPSPos * pValue;
	structTimeStamp stamp;
	char temp[50];
	

	if(!pFilename)return;
	if(!pInBuffer)return;
	if(!pOutBuffer)return;
	if(!readSensorFileHead(pFilename, sensor, pInBuffer, &iHeadLen))return;
	if(!getSensorDataTimeSpace(sensor, pInBuffer, iHeadLen, &iTimeSpace))return;
	if(!getSensorDataCellDataLen(sensor, pInBuffer, iHeadLen, &iCellLen))return;
	if(!readMiniDICOMFileWholeData(pFilename, pInBuffer, &iInBufLen, iHeadLen))return;
	iCount = getMiniDICOMFileCellCount(pFilename, iHeadLen, iCellLen);
	strcpy(p, prefix);
	p += strlen(prefix);

	time = 0;
	stamp = getSportStartTime();
	iTimeStep = iTimeSpace;
	iTimeSpace *= 1000;
	pValue = (structGPSPos *)pInBuffer;
	for(i = 0; i < iCount; i++, stamp += iTimeSpace, pValue++, time += iTimeStep){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';
		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = ',';

		if(isInPauseState(time))strcpy(temp, "true");
		else strcpy(temp, "false");
		p = getStringOfItem(p, "sportState", temp);
		*p++ = ',';

		sprintf(temp, "%.6f", pValue->longtitude / 1000000.0f);
		p = getStringOfItem(p, "lng", temp);
		*p++ = ',';

		sprintf(temp, "%.6f", pValue->latitude / 1000000.0f);
		p = getStringOfItem(p, "lat", temp);
		*p++ = ',';

		sprintf(temp, "%d", pValue->accuracy);
		p = getStringOfItem(p, "Accuracy", temp);
		*p++ = ',';

		sprintf(temp, "%.1f", pValue->altitude / 10.0f);
		p = getStringOfItem(p, "altitude", temp);
		*p++ = ',';

		sprintf(temp, "%d", 0);
		p = getStringOfItem(p, "speed", temp);
		*p++ = ',';
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}

	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);
}

BOOL parseGPSFileWithSpeed(const char * pGPSFile, char * pGPSInBuffer, int iGPSInBufLen, \
						   const char * pDistanceFile, char * pDistanceInBuffer, int iDistanceInBufLen, \
						   const char * pJasonFilename, char * pOutBuffer, int iOutBufLen){
	int iGPSTimeSpace, iGPSCount;
	int iDistanceTimeSpace, iDistanceCount, *pDistance,*pDistanceMax;
	structGPSPos * pGPS;
	BOOL bDistanceOK = FALSE;
	int i;
	char *p = pOutBuffer;
	const char * prefix = "{\n\"array\":[";
	int time, timeDistance, iStampSpace;
	structTimeStamp stamp;
	float speed = 0;
	int iDistanceSpace = 0;
	char temp[50];

	

	if(!pOutBuffer)return FALSE;
	if(!pJasonFilename)return FALSE;

	if(!parseSensorFile(SENSOR_GPS, pGPSFile, pGPSInBuffer, iGPSInBufLen, &iGPSTimeSpace, &iGPSCount))return FALSE;
	bDistanceOK = parseSensorFile(SENSOR_DISTANCE, pDistanceFile, pDistanceInBuffer, iDistanceInBufLen, &iDistanceTimeSpace, &iDistanceCount);

	strcpy(p, prefix);
	p += strlen(prefix);

	time = 0;
	timeDistance = 0;
	stamp = getSportStartTime();
	iStampSpace = iGPSTimeSpace * 1000;

	pGPS = (structGPSPos *)pGPSInBuffer;
	if(bDistanceOK){
		pDistance = (int *)pDistanceInBuffer;
		iDistanceSpace = 15 / iDistanceTimeSpace;
		if(15 % iDistanceTimeSpace)iDistanceSpace++;
		pDistanceMax = pDistance + iDistanceCount - 1;
	}
	for(i = 0; i < iGPSCount; i++, stamp += iStampSpace, pGPS++, time += iGPSTimeSpace){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';

		if(bDistanceOK){

			while(timeDistance < time){
				timeDistance += iDistanceTimeSpace;
				pDistance++;
			}
			if(pDistance > pDistanceMax)pDistance = pDistanceMax;
			if(i < iDistanceSpace){
				speed = (*pDistance) * 1.0f / ((i + 1) * iDistanceTimeSpace);
			}else{
				speed = ((*pDistance) - (*(pDistance - iDistanceSpace))) * 1.0f / (iDistanceSpace * iDistanceTimeSpace);
			}

			sprintf(temp, "%.1f", speed);
			p = getStringOfItem(p, "speed", temp);
			*p++ = ',';
		}

		if(isInPauseState(time))strcpy(temp, "true");
		else strcpy(temp, "false");
		p = getStringOfItem(p, "sportState", temp);
		*p++ = ',';

		sprintf(temp, "%.6f", pGPS->longtitude / 1000000.0f);
		p = getStringOfItem(p, "lng", temp);
		*p++ = ',';

		sprintf(temp, "%.6f", pGPS->latitude / 1000000.0f);
		p = getStringOfItem(p, "lat", temp);
		*p++ = ',';

		sprintf(temp, "%d", pGPS->accuracy);
		p = getStringOfItem(p, "Accuracy", temp);
		*p++ = ',';

		sprintf(temp, "%.1f", pGPS->altitude / 10.0f);
		p = getStringOfItem(p, "altitude", temp);
		*p++ = ',';

		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}
	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = '}';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);
	return TRUE;
}

BOOL parseDistanceAndStepAndHRFile(const char * pStepFile, char * pStepInBuffer, int iStepInBufLen, \
						   const char * pDistanceFile, char * pDistanceInBuffer, int iDistanceInBufLen, \
						   const char * pHRFile, char * pHRInBuffer, int iHRInBufLen, \
						   const char * pJasonFilename, char * pOutBuffer, int iOutBufLen){

	BOOL bStepOK = FALSE, bDistanceOK = FALSE, bHROK = FALSE;
	int iStepTimeSpace, iStepCount, timeStep, *pStep, *pStepMax;
	int iDistanceTimeSpace, iDistanceCount, timeDistance, *pDistance, *pDistanceMax;
	int iHRTimeSpace, iHRCount;
	U8 * pHR;
	

	int i, iCount = 0, iTimeSpace = 0;

	char *p = pOutBuffer;
	char * prefix = "{\n\"array\":[";
	char * hrprefix = "\"HeartRateArray\":[";
	int iTimeSecond, time;
	structTimeStamp stamp;
	char temp[256];
	int iSteps, iOldSteps = 0;
	int iDistance, iOldDistance = 0;
	

	if(!pOutBuffer)return FALSE;
	if(!pJasonFilename)return FALSE;
	bStepOK = parseSensorFile(SENSOR_GSENSOR, pStepFile, pStepInBuffer, iStepInBufLen, &iStepTimeSpace, &iStepCount);
	bDistanceOK = parseSensorFile(SENSOR_DISTANCE, pDistanceFile, pDistanceInBuffer, iDistanceInBufLen, &iDistanceTimeSpace, &iDistanceCount);

	strcpy(p, prefix);
	p += strlen(prefix);

	timeStep = 0;
	timeDistance = 0;
	time = 0;
	stamp = getSportStartTime();
	if(bStepOK && bDistanceOK){
		iTimeSecond = iStepTimeSpace > iDistanceTimeSpace ? iDistanceTimeSpace : iStepTimeSpace;
		iCount = iStepTimeSpace > iDistanceTimeSpace ? iDistanceCount : iStepCount;
	}else if(bStepOK){
		iTimeSecond = iStepTimeSpace;
		iCount = iStepCount;
	}else if(bDistanceOK){
		iTimeSecond = iDistanceTimeSpace;
		iCount = iDistanceCount;
	}

	iTimeSpace = iTimeSecond * 1000;
	pStep = (int *)pStepInBuffer;
	pDistance = (int *)pDistanceInBuffer;
	pStepMax = pStep + iStepCount - 1;
	pDistanceMax = pDistance + iDistanceCount - 1;

	for(i = 0; i < iCount; i++, stamp += iTimeSpace){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';

		if(bDistanceOK){
			iDistance = *pDistance;
			if(iDistance < iOldDistance)iDistance = iOldDistance;
			iOldDistance = iDistance;
			sprintf(temp, "%d", iDistance);
			p = getStringOfItem(p, "distance", temp);
			*p++ = ',';
		}
		if(bStepOK){
			iSteps = *pStep;
			if(iSteps < iOldSteps)iSteps = iOldSteps;
			iOldSteps = iSteps;
			sprintf(temp, "%d", iSteps);
			p = getStringOfItem(p, "step", temp);
			*p++ = ',';
		}
		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}
		time += iTimeSecond;

		if(bDistanceOK){
			while(timeDistance < time){
				timeDistance += iDistanceTimeSpace;
				pDistance++;
			}
			if(pDistance > pDistanceMax)pDistance = pDistanceMax;
		}
		if(bStepOK){
			while(timeStep < time){
				timeStep += iStepTimeSpace;
				pStep++;
			}
			if(pStep > pStepMax)pStep = pStepMax;
		}
	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = ',';
	*p++ = '\n';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);

	bHROK = parseSensorFile(SENSOR_HEARTRATE, pHRFile, pHRInBuffer, iHRInBufLen, &iHRTimeSpace, &iHRCount);
	if(!bHROK)iHRCount = 0;
	p = pOutBuffer;
	strcpy(p, hrprefix);
	p += strlen(hrprefix);
	pHR = (U8 *)pHRInBuffer;
	stamp = getSportStartTime();
	iTimeSpace = iHRTimeSpace * 1000;
	for(i = 0; i < iHRCount; i++, pHR++, stamp += iTimeSpace){
		if(i != 0)*p++ = ',';
		*p++ = '\n';
		*p++ = '{';

		sprintf(temp, "%d", *pHR);
		p = getStringOfItem(p, "heartrate", temp);
		*p++ = ',';

		sprintf(temp, "%lld", stamp);
		p = getStringOfItem(p, "time", temp);
		*p++ = '}';
		if(i % 100 == 0){
			*p = 0;
			AppendString(pJasonFilename, pOutBuffer);
			p = pOutBuffer;
		}
	}
	*p++ = '\n';
	*p++ = ']';
	*p++ = '}';
	*p++ = 0;
	AppendString(pJasonFilename, pOutBuffer);
	return TRUE;
}