#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "WatchFileIO.h"
#include "MiniDICOM.h"


static const char iron_head[8]={'I','R','O','N', 1, 2, 3, 4};

static void sprintf_float(char * pBuffer, float value){
	int iInteger = (int)value;
	int iDot;
	int iLen;
	char * p = NULL;

	iDot = (int)((value - iInteger) * 1000000);
	if(iDot < 0)iDot = -iDot;
	
	sprintf(pBuffer, "%d.%d", iInteger,  iDot);
	iLen = strlen(pBuffer);
	p = pBuffer + iLen - 1;
	while(*p == '0'){
		if(*(p - 1) == '.')break;
		*p-- = 0;
	}
}



char * getAnsiStringOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen <= 0))return NULL;
	return pData;
}

int getIntValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen < sizeof(int)))return 0;
	return *((int *)pData);	
}

U32 getU32ValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen < sizeof(U32)))return 0;
	return *((U32 *)pData);	
}

U16 getU16ValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen < sizeof(U16)))return 0;
	return *((U16 *)pData);	
}

short getI16ValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen < sizeof(short)))return 0;
	return *((I16 *)pData);	
}

U8 getU8ValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen < sizeof(U8)))return 0;
	return *((U8 *)pData);	
}

char getCharValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen <= 0))return 0;
	return *pData;	
}

double getDoubleValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen <= sizeof(double)))return 0;
	return *((double *)pData);	
}

float getFloatValueOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen <= sizeof(float)))return 0;
	return *((float *)pData);	
}

char * getGB2312StringOfMiniDICOMData(char * pData, int iDataLen){
	if((!pData) || (iDataLen <= 0))return 0;
	return pData;
}







BOOL getMiniDICOMValueBySmartMode(int vr, char * pData, int iDataLen, char * pDest, int iDestLen, int iWishType){
	if((!pData) || (!pDest))return FALSE;
	switch(iWishType){
		case WISH_TYPE_INT:
			{
				int value = 0;
				if(iDestLen < sizeof(int))return FALSE;
				switch(vr){
				case VR_U32:
					value = getU32ValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_S32:
					value = getIntValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_S16:
					value = getI16ValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_U16:
					value = getU16ValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_DOUBLE:
					value = (int)getDoubleValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_FLOAT:
					value = (int)getFloatValueOfMiniDICOMData(pData, iDataLen);
					break;
				case VR_STRING_ANSI:
				case VR_STRING_GB2312:
				case VR_STRING_UTF8:
					value = atoi(pData);
					break;
				}
				*((int *)pDest) = value;
			}
			break;
		case WISH_TYPE_DATE:
//			if((vr != VR_STRING_ANSI) && (vr != VR_STRING_GB2312) && (vr != VR_STRING_UTF8))return FALSE;
//			if(iDestLen < sizeof(int))return FALSE;
//			*((int *)pDest) = getDateStringValueOfMiniDICOMData(pData, iDataLen);
			break;
		case WISH_TYPE_TIME:
//			if((vr != VR_STRING_ANSI) && (vr != VR_STRING_GB2312) && (vr != VR_STRING_UTF8))return FALSE;
//			if(iDestLen < sizeof(int))return FALSE;
//			*((int *)pDest) = getTimeStringValueOfMiniDICOMData(pData, iDataLen);
			break;
		case WISH_TYPE_STRING:
			switch(vr){
			case VR_U32:
				if(iDestLen < 12)return FALSE;
				sprintf(pDest, "%u", getU32ValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_S32:
				if(iDestLen < 12)return FALSE;
				sprintf(pDest, "%d", getIntValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_S16:
				if(iDestLen < 7)return FALSE;
				sprintf(pDest, "%d", getI16ValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_U16:
				if(iDestLen < 7)return FALSE;
				sprintf(pDest, "%d", getU16ValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_DOUBLE:
				if(iDestLen < 20)return FALSE;
				sprintf_float(pDest, (float)getDoubleValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_FLOAT:
				if(iDestLen < 20)return FALSE;
				sprintf_float(pDest, getFloatValueOfMiniDICOMData(pData, iDataLen));
				break;
			case VR_STRING_ANSI:
			case VR_STRING_GB2312:
				if(strlen(pData) >= (unsigned)iDestLen)return FALSE;
				strcpy(pDest, pData);
			case VR_STRING_UTF8:
//				getUTF8StringOfMiniDICOMData(pData, iDataLen, pDest, iDestLen);
				break;
			}
			break;
		case WISH_TYPE_VOID:
			if(iDestLen < iDataLen)return FALSE;
			memcpy(pDest, pData, iDataLen);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

U16 getGroupOfTag(U32 tag){
	return (U16)((tag & 0xFFFF0000) >> 16);
}

U16 getCellOfTag(U32 tag){
	return (U16)(tag & 0x0000FFFF);
}

U8 getFirstByteOfCell(U16 cell){
	return (cell & 0xFF00) >> 8;
}

U8 getSecondByteOfCell(U16 cell){
	return (cell & 0x00FF);
}

U32 getTagByGroupAndCell(U16 group, U16 cell){
	return (group << 16) | cell;
}

U16 getCellByBytes(U8 b1, U8 b2){
	return (b1 << 8) | b2;
}

int getFirstGroupPos(void){
	return IRON_HEAD_LEN + CHAPTER_SUMMARY_LEN;
}

int getFirstTagPos(void){
	return getFirstGroupPos() + TAG_GROUP_LENGTH;
}



static BOOL isMixCell(U16 group){
	BOOL bMix = FALSE;

	switch(group){
	case TAG_GROUP_NOTIFICATION:
	case TAG_GROUP_TRAINING_PROGRAM:
	case TAG_GROUP_CHALLENGE:
	case TAG_GROUP_HEALTH:
	case TAG_GROUP_SENSOR_DATA:
		bMix = TRUE;
		break;
	}
	return bMix;
}

BOOL isGroupFlag(U32 tag){
	return getCellOfTag(tag) == 0;
}

BOOL parseTagValid(char * pBuf, structTagParser * pParser){
	U16 group, cell;
	U8 b1 = 0, b2 = 0;
	BOOL bValid = TRUE;

	if((!pBuf) || (!pParser))return FALSE;
	
	pParser->tag = *((U32 * )pBuf);
	pParser->vr = *((U16 * )(pBuf + 4));
	pParser->iLen = *((int * )(pBuf + 6));
#ifdef TEST_IRON_CLOUD
	watch_log_out_i3("parseTagValid", pParser->tag, pParser->vr, pParser->iLen);
#endif
	group = getGroupOfTag(pParser->tag);
	if(group >= TAG_GROUP_MAX)return FALSE;
	if(pParser->vr >= VR_MAX)return FALSE;
	cell = getCellOfTag(pParser->tag);
	if(isMixCell(group)){
		b1 = getFirstByteOfCell(cell);
		b2 = getSecondByteOfCell(cell);
	}

	switch(group){
	case TAG_GROUP_REQUEST:
		if(cell >= REQUEST_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_RESPONSE:
		if(cell >= RESPONSE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_CHAPTER:
		if(cell != 0)bValid = FALSE;
		break;
	case TAG_GROUP_NOTIFICATION:
		if(b1 >= NOTIFICATION_BLE_MAX)bValid = FALSE;
		if(b2 >= NOTIFICATION_FIELD_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_WEATHER:
		if(cell >= WEATHER_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_CALL:
		if(cell >= CALL_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_USERINFO:
		if(cell >= USERINFO_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_UPGRADE:
		if(cell >= UPGRADE_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_FIND:
		if(cell >= FIND_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_BIND:
		if(cell >= BIND_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_ADD_LOG:
		if(cell >= SPORT_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_REMOVE_LOG:
		if(cell >= REMOVE_LOG_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_DOWNLOAD_LOGS:
		if(cell >= DOWNLOAD_LOG_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_SPORTS_SUMMARY_DATA:
		if(cell >= SUMMARY_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_SPORTS_GROUP_DATA:
		if(cell >= GROUP_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_SENSOR_DATA:
		if(b1 >= SENSOR_MAX)bValid = FALSE;
		if(b2 >= SENSOR_FIELD_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_DIAL_PALTE:
		if(cell >= DIAL_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_TRAINING_PROGRAM:
		if(b1 >= SPORT_BLE_MAX)bValid = FALSE;
		if(b2 >= DATA_COMPARE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_CHALLENGE:
		if(b1 >= SPORT_BLE_MAX)bValid = FALSE;
		if(b2 >= DATA_COMPARE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_HEALTH:
		if(b1 >= HEALTH_BLE_MAX)bValid = FALSE;
		if(b2 >= HEALTH_FIELD_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_TIME:
		if(cell >= TIME_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_GPS:
		if(cell >= GPS_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_ERROR_REPORT:
		if(cell >= ERROR_REPORT_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_LOCALE_SETTINGS:
		if(cell >= LOCALE_SETTINGS_BLE_MAX)bValid = FALSE;
		break;
	case TAG_GROUP_MUSIC:
		if(cell >= MUSIC_BLE_MAX)bValid = FALSE;
		break;
	}
	return bValid;
}




BOOL isGroupValid(char * pBuf, int iBufLen){
	U16 group, cell;
	int iLen, iGroupLen;
	structTagParser parser;

#ifdef TEST_IRON_CLOUD
	watch_log_out("isGroupValid begin");
#endif
	if(!pBuf){
		return FALSE;
	}
	if(!parseTagValid(pBuf, &parser)){
		return FALSE;
	}

	group = getGroupOfTag(parser.tag);
	cell = getCellOfTag(parser.tag);
	iGroupLen = parser.iLen;

	if(cell != 0){
		return FALSE;
	}
	if(iGroupLen == 0){
		return FALSE;
	}
	pBuf += TAG_GROUP_LENGTH;
	iLen = 0;
	do{
		if(!parseTagValid(pBuf, &parser)){
			return FALSE;
		}
		if(getGroupOfTag(parser.tag) != group){
			return FALSE;
		}
		if(isGroupFlag(parser.tag)){
			return FALSE;
		}

		if(parser.iLen % 2)parser.iLen++;
		parser.iLen += TAG_MIN_LENGTH;
		pBuf += parser.iLen;
		iLen += parser.iLen;
	}while((iLen < iGroupLen) && (iLen < iBufLen));
#ifdef TEST_IRON_CLOUD
	watch_log_out_i2("isGroupValid", iGroupLen, iLen);
#endif

	if((iBufLen >= iGroupLen) && (iLen != iGroupLen))return FALSE;
	
#ifdef TEST_IRON_CLOUD
	watch_log_out("isGroupValid OK");
#endif
	return TRUE;
}



BOOL searchTag(U32 tag, char * pBuf, int iLimitLen, structTagParser * pParser, int *pDataPos){
	int iLen = 0;

	if((!pBuf) || (!pParser) || (!pDataPos))return FALSE;

	while(iLen < iLimitLen){
		if(!parseTagValid(pBuf, pParser))return FALSE;
		if(isGroupFlag(pParser->tag)){
			iLen += TAG_GROUP_LENGTH;
			pBuf += TAG_GROUP_LENGTH;
			continue;
		}
		if(pParser->tag == tag){
			*pDataPos = iLen + TAG_MIN_LENGTH;
			return TRUE;
		}
		if(pParser->iLen % 2)pParser->iLen++;
		pParser->iLen += TAG_MIN_LENGTH;
		iLen += pParser->iLen;
		pBuf += pParser->iLen;
	}
	return FALSE;
}
