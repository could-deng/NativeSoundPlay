#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "WatchFileIO.h"
#include "MiniDICOM.h"
#include "MiniDICOMFile.h"

#include<android/log.h>
// LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
#define LOGD(...) __android_log_print(ANDROID_LOG_VERBOSE, "watch_transfer", __VA_ARGS__)

BOOL isDataIncludeGroupHead(char * pBuffer, int iBufLen){
	U32 tag = *((U32 *)pBuffer);
	return isGroupFlag(tag);
}

BOOL getMiniDICOMFileTagValue(char * pBuffer, int iBufLen, U32 tag, char * pValue, int iValueLen){
	structTagParser parser;
	int iPos;

	if((!pBuffer) || (!pValue))return FALSE;
	if(iBufLen > MAX_MINI_DICOM_FILE_HEAD_LEN)iBufLen = MAX_MINI_DICOM_FILE_HEAD_LEN;
	if(!searchTag(tag, pBuffer, iBufLen, &parser, &iPos))return FALSE;
	if(!getMiniDICOMValueBySmartMode(parser.vr, pBuffer + iPos, parser.iLen, pValue, iValueLen, WISH_TYPE_VOID))return FALSE;
	return TRUE;
}

BOOL readMiniDICOMFile(const char * pFilename, char * pBuffer, int *pBufLen, int iOffset, int iDataLen){
	int iFileLen, iBufLen;

//	LOGD("11");
	if((!pBuffer) || (!pBufLen) || (!pFilename))return FALSE;
	iBufLen = *pBufLen;
//	LOGD("12");
	iFileLen = getFileLength(pFilename);
	if(iFileLen <= 0)return FALSE;
//	LOGD("13");
	if(iOffset >= iFileLen)return FALSE;
	if(iDataLen < 0)iDataLen = iFileLen;
	if((iOffset + iDataLen) > iFileLen){
		iDataLen = iFileLen - iOffset;
	}
//	LOGD("14");
	if(iDataLen > iBufLen)return FALSE;

	if(!ReadFileContentAtPos(pFilename, pBuffer, iDataLen, iOffset))return FALSE;
	*pBufLen = iDataLen;
//	LOGD("15");
	return TRUE;
}


BOOL readMiniDICOMFileWhole(const char * pFilename, char * pBuffer, int *pBufLen){
	return readMiniDICOMFile(pFilename, pBuffer, pBufLen, 0, -1);
}


BOOL readMiniDICOMFileHead(const char * pFilename, char * pBuffer, int *pBufLen, U32 tagStartAddress){
	int iStartAddress;
	int iBufLen;
//	LOGD("1");
	if(!readMiniDICOMFile(pFilename, pBuffer, pBufLen, 0, MAX_MINI_DICOM_FILE_HEAD_LEN))return FALSE;
//	LOGD("2");
	iBufLen = *pBufLen;
	if(!getMiniDICOMFileTagValue(pBuffer, iBufLen, tagStartAddress, (char *)&iStartAddress, sizeof(int)))return FALSE;
//	LOGD("3");
	*pBufLen = iStartAddress;
	return TRUE;
}

BOOL readMiniDICOMFileWholeData(const char * pFilename, char * pBuffer, int *pBufLen, int iHeadLen){
	if(isDataIncludeGroupHead(pBuffer, *pBufLen))iHeadLen += TAG_GROUP_LENGTH;
	return readMiniDICOMFile(pFilename, pBuffer, pBufLen, iHeadLen, -1);
}

int getMiniDICOMFileDataLen(const char * pFilename, int iHeadLen){
	int iLen = getFileLength(pFilename);

	if(iLen <= iHeadLen)return 0;
	return iLen - iHeadLen;
}

int getMiniDICOMFileCellCount(const char * pFilename, int iHeadLen, int iCellLen){
	int iDataLen = 0;
	if(iCellLen <= 0)return 0;
	iDataLen = getMiniDICOMFileDataLen(pFilename, iHeadLen);
//	if(iDataLen  % iCellLen)return 0;
	return iDataLen / iCellLen;
}



