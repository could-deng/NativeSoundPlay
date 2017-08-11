#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "WatchFileIO.h"
#include "MiniDICOM.h"
#include "MiniDICOMFile.h"
#include "WatchDateTime.h"
#include "WatchSportsLogData.h"

#define MAX_PAUSE_COUNT 10
static structTimeStamp static_Starttime;
static int static_pauseDetail[MAX_PAUSE_COUNT * 2];
static int static_pauseCount;
static int static_iFlag;

typedef struct{
int cell;
char * tag;
}structJasonTag;

static const structJasonTag static_arrayJasonNormal[]={
{SPORT_BLE_TYPE, "sportType"}
,{SPORT_BLE_DURATION, "totalDuration"}
,{SPORT_BLE_TOTAL_CALORIE, "totalCalorie"}
//,{SPORT_BLE_PAUSE_COUNT, "pauseCount"}
,{SPORT_BLE_TOTAL_FAT_BURST, "fatBurst"}
,{SPORT_BLE_TOTAL_DISTANCE, "totalDistance"}
,{SPORT_BLE_AVERAGE_SPEED, "averageSpeed"}	//??
,{SPORT_BLE_AVERAGE_PACE, "averagePace"}	//??
,{SPORT_BLE_AVERAGE_STEP_FREQUENCY, "averageStepFrequency"}	//???
,{SPORT_BLE_TOTAL_STEPS, "totalSteps"}
,{SPORT_BLE_MAX_PACE, "maxPace"}
,{SPORT_BLE_MAX_STEP_FREQUENCY, "maxStepFrequency"}
,{SPORT_BLE_AVERAGE_VERTICAL_SPEED, "averageVerticalSpeed"}//??
,{SPORT_BLE_TOTAL_FOOTSTEPS, "totalFootSteps"}
,{SPORT_BLE_PEAK_ALTITUDE, "peakAltitude"}
,{SPORT_BLE_TOTAL_UP_DISTANCE, "totalUp"}
,{SPORT_BLE_TOTAL_DOWN_DISTANCE, "totalDown"}
,{SPORT_BLE_TROUGH_ALTITUDE, "TroughAltitude"}
,{SPORT_BLE_RIDE_POWER, "ridePower"}
,{SPORT_BLE_MAX_CIRCLES_FREQUENCY, "circlesFrquency"}	//
,{SPORT_BLE_FEET_CIRCLES, "feetCircles"}
,{SPORT_BLE_SWIM_ARM, "swimArm"}
,{SPORT_BLE_SWIM_STYLE, "swimStyle"}
,{SPORT_BLE_SWIM_ROUND_TRIPS, "swimTrips"}
};

static const structJasonTag static_arrayJasonSpecial[]={
{SPORT_BLE_START_DATE, "startDate"}
,{SPORT_BLE_START_TIME, "startTime"}
,{SPORT_BLE_VO2MAX, "Vo2max"}
};

static const structJasonTag static_arrayJasonVoid[]={
{SPORT_BLE_PAUSE_DETAIL, "pause_detail"}
,{SPORT_BLE_PACE_RECORDS, "pace_records"}
,{SPORT_BLE_GROUP_RECORDS, "group_records"}
,{SPORT_BLE_HR_STATISTICS, "hrStatistics"}
};


static structJasonTag * getJasonTagByCell(U16 cell, structJasonTag * pJason, int iCount){
	int i;
	
	if(!pJason)return NULL;
	for(i = 0; i < iCount; i++, pJason++){
		if(pJason->cell == cell)return pJason;
	}
	return NULL;
}

static const char * getTagOfJasonForNormal(U16 cell){
	structJasonTag * pJason = getJasonTagByCell(cell, (structJasonTag * )static_arrayJasonNormal, sizeof(static_arrayJasonNormal) / sizeof(static_arrayJasonNormal[0]));
	if(!pJason)return NULL;
	return pJason->tag;
}
static const char * getTagOfJasonForSpecial(U16 cell){
	structJasonTag * pJason = getJasonTagByCell(cell, (structJasonTag * )static_arrayJasonSpecial, sizeof(static_arrayJasonSpecial) / sizeof(static_arrayJasonSpecial[0]));
	if(!pJason)return NULL;
	return pJason->tag;
}

static const char * getTagOfJasonForVoid(U16 cell){
	structJasonTag * pJason = getJasonTagByCell(cell, (structJasonTag * )static_arrayJasonVoid, sizeof(static_arrayJasonVoid) / sizeof(static_arrayJasonVoid[0]));
	if(!pJason)return NULL;
	return pJason->tag;
}
structTimeStamp getSportStartTime(void){
	return static_Starttime;
}

BOOL isInPauseState(int time){
	int * p = static_pauseDetail;
	int iCount = static_pauseCount;
	int i;
	int start, duration;

	for(i = 0; i < iCount; i++, p += 2){
		start = *p;
		duration = *(p + 1);
		if(time >= start && time <= (start + duration))return TRUE;
	}
	return FALSE;
}
static BOOL getValueOfNormalTag(char * pOut, int vr, int iInLen, char * pIn){
	BOOL bOK = TRUE;

	if(!pOut)return FALSE;
	if(!pIn)return FALSE;
	switch(vr){
	case VR_S32:
		sprintf(pOut, "%d", *((I32 *)pIn));
		break;
	case VR_U32:
		sprintf(pOut, "%u", *((U32 *)pIn));
		break;
	case VR_S16:
		sprintf(pOut, "%d", *((I16 *)pIn));
		break;
	case VR_U16:
		sprintf(pOut, "%u", *((U16 *)pIn));
		break;
	case VR_DOUBLE:
		sprintf(pOut, "%f", *((double *)pIn));
		break;
	case VR_FLOAT:
		sprintf(pOut, "%f", *((float *)pIn));
		break;
	case VR_STRING_ANSI:
		strncpy(pOut, pIn, iInLen);
		break;
	default:
		bOK = FALSE;
		break;
	}
	return bOK;
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

static char * getDataOfPauseDetail(char * pOut, int iInLen, char * pIn){
	int i, iCount;
	char * p = pOut;
	char temp[20];

	if(!pOut)return p;
	if(!pIn)return p;
	p = pOut;

	*p++ = '[';

	memcpy(static_pauseDetail, pIn, iInLen);
	iCount = iInLen / 8;
	static_pauseCount = iCount;
	for(i = 0; i < iCount; i++, pIn += 8){
		*p++ = '\n';
		if(i != 0)*p++= ',';
		*p++ = '{';

		sprintf(temp, "%d", *((I32 *)pIn));
		p = getStringOfItem(p, "time", temp);

		*p++= ',';

		*p++ = '\n';
		sprintf(temp, "%d", *((I32 *)(pIn + 4)));
		p = getStringOfItem(p, "duration", temp);

		*p++ = '\n';
		*p++ = '}';

	}
	*p++ = '\n';
	*p++ = ']';
	return p;	
}
static char * getDataOfGroupDetail(char * pOut, int iInLen, char * pIn){
	int i, iCount;
	char * p = pOut;
	char temp[20];

	if(!pOut)return p;
	if(!pIn)return p;
	p = pOut;

	*p++ = '[';

	iCount = iInLen / 8;
	for(i = 0; i < iCount; i++, pIn += 8){
		*p++ = '\n';
		if(i != 0)*p++= ',';
		*p++ = '{';

		sprintf(temp, "%d", *((I32 *)pIn));
		p = getStringOfItem(p, "time", temp);

		*p++= ',';

		*p++ = '\n';
		sprintf(temp, "%d", *((I32 *)(pIn + 4)));
		p = getStringOfItem(p, "distance", temp);

		*p++ = '\n';
		*p++ = '}';

	}
	*p++ = '\n';
	*p++ = ']';
	return p;	
}
static char * getDataOfPaceDetail(char * pOut, int iInLen, char * pIn){
	int i, iCount;
	char * p = pOut;
	char temp[20];

	if(!pOut)return p;
	if(!pIn)return p;
	p = pOut;

	*p++ = '[';

	iCount = iInLen / 8;
	for(i = 0; i < iCount; i++, pIn += 8){
		*p++ = '\n';
		if(i != 0)*p++= ',';
		*p++ = '{';

		sprintf(temp, "%d", *((I32 *)pIn));
		p = getStringOfItem(p, "distance", temp);

		*p++= ',';

		*p++ = '\n';
		sprintf(temp, "%d", *((I32 *)(pIn + 4)));
		p = getStringOfItem(p, "time", temp);

		*p++ = '\n';
		*p++ = '}';

	}
	*p++ = '\n';
	*p++ = ']';
	return p;	
}
static char * getDataOfHRStastics(char * pOut, int iInLen, char * pIn){
	int i, iCount;
	char * p = pOut;
	char temp[20];

	if(!pOut)return p;
	if(!pIn)return p;
	p = pOut;

	*p++ = '[';

	iCount = iInLen / 8;
	for(i = 0; i < iCount; i++, pIn += 4){
		*p++ = '\n';
		if(i != 0)*p++= ',';
		*p++ = '{';

		sprintf(temp, "%d", *((I32 *)pIn));
		p = getStringOfItem(p, "time", temp);


		*p++ = '\n';
		*p++ = '}';

	}
	*p++ = '\n';
	*p++ = ']';
	return p;	
}
static char * getStringOfVoidItem(char * pOut, const char * pTag, U16 cell, int iInLen, char * pIn){
	char * p = pOut;

	if(!pTag)return p;
	*p++= '\"';
	strcpy(p, pTag);
	p += strlen(pTag);
	*p++= '\"';
	*p++= ':';

	switch(cell){
		case SPORT_BLE_PAUSE_DETAIL:
			p = getDataOfPauseDetail(p, iInLen, pIn);
			break;
		case SPORT_BLE_PACE_RECORDS:
			p = getDataOfPaceDetail(p, iInLen, pIn);
			break;
		case SPORT_BLE_GROUP_RECORDS:
			p = getDataOfGroupDetail(p, iInLen, pIn);
			break;
		case SPORT_BLE_HR_STATISTICS:
			p = getDataOfHRStastics(p, iInLen, pIn);
			break;
	}
	return p;
}

static char * getStringOfSpecialItem(char * pOut, const char * pTag, U16 cell, int iInLen, char * pIn){
	char temp[50];
	int value;
	char * p = pOut;

	switch(cell){
		case SPORT_BLE_START_DATE:
			static_iFlag |= 0x01;
			static_Starttime += (I64)getDaysForTimeStampByString(pIn) * 3600 * 24 * 1000;
			if((static_iFlag & 0x03) == 0x03){
				sprintf(temp, "%lld", static_Starttime); 
				p = getStringOfItem(p, "startTime", temp);
			}
			break;
		case SPORT_BLE_START_TIME:
			static_iFlag |= 0x02;
			static_Starttime += getMillSecondsForTimeStampByString(pIn);
			if((static_iFlag & 0x03) == 0x03){
				sprintf(temp, "%lld", static_Starttime); 
				p = getStringOfItem(p, "startTime", temp);
			}
			break;
		case SPORT_BLE_VO2MAX:
			value = atoi(pIn);
			sprintf(temp, "%d.%d", value / 10, value % 10); 
			p = getStringOfItem(p, "Vo2max", temp);
			break;

	}
	return p;
}
BOOL parseLogFile(const char * pFilename, char * pBuffer, int iBufLen, char * pJasonString){
	int iBufPos = 0;
	U32 tag;
	U16 group;
	U16 cell;
	structTagParser parser;
	char *pInBuffer;
	char * pTag;
	char * p, *pOld;
	char temp[20];

	if(!pFilename)return FALSE;
	if(!pJasonString)return FALSE;
	if(!readMiniDICOMFileWhole(pFilename, pBuffer, &iBufLen))return FALSE;
	if(!isGroupValid(pBuffer, iBufLen))return FALSE;

	static_iFlag = 0;
	static_pauseCount = 0;
	parser.pBuf = NULL;
	p = pJasonString;
	*p++= '{';
	*p++= '\n';
	iBufPos += TAG_GROUP_LENGTH;
	pOld = p;
	while(iBufPos < iBufLen){
		if(!parseTagValid(pBuffer + iBufPos, &parser))return FALSE;
		if((iBufPos + parser.iLen + TAG_MIN_LENGTH) > iBufLen)return FALSE;
		pInBuffer = pBuffer + iBufPos + TAG_MIN_LENGTH;
		tag = parser.tag;
		group = getGroupOfTag(tag);
		cell = getCellOfTag(tag);
		if(group != TAG_GROUP_ADD_LOG)return FALSE;

		if(pOld != p){
			*p++ = ',';
			*p++ = '\n';
			pOld = p;
		}
		pTag = (char *)getTagOfJasonForNormal(cell);
		if(pTag){
			getValueOfNormalTag(temp, parser.vr, parser.iLen, pInBuffer);
			p = getStringOfItem(p, pTag, temp);
		}else{
			pTag = (char *)getTagOfJasonForSpecial(cell);
			if(pTag)p = getStringOfSpecialItem(p, pTag, cell, parser.iLen, pInBuffer);
			else{
				pTag = (char *)getTagOfJasonForVoid(cell);
				if(pTag)p = getStringOfVoidItem(p, pTag, cell, parser.iLen, pInBuffer);
			}
		}

		if(parser.iLen % 2)parser.iLen++;
		parser.iLen += TAG_MIN_LENGTH;
		iBufPos += parser.iLen;
	};
	*p++= '\n';
	*p++= '}';
	*p = 0;
	return TRUE;
}


