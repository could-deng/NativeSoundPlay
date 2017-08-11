#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#endif

#include "WatchFileIO.h"


#include<android/log.h>
// LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
#define LOGD(...) __android_log_print(ANDROID_LOG_VERBOSE, "watch_transfer", __VA_ARGS__)






static char * getOpenMode(enumOpenMode mode){
	char * p = NULL;

	switch(mode){
		case OPENMODE_READ_TEXT:
		default:
			p = "r";
			break;
		case OPENMODE_READ_BIN:
			p = "rb";
			break;
		case OPENMODE_WRITE_TEXT:
			p = "w";
			break;
		case OPENMODE_WRITE_BIN:
			p = "wb";
			break;
		case OPENMODE_APPEND_BIN:
			p = "ab";
			break;
		case OPENMODE_CREATE_WRITE_BIN:
			p = "wb";
			break;
		case OPENMODE_RW:
			p = "rb+";
			break;
	}
	return p;
}
BOOL CreateBinFileOfLength(const char * filename, int iLength, char value){
	char buf[64];
	int iLoop;
	int i, iSize = sizeof(buf);
	void * handle = NULL;

//	if(!autoCreateParent(filename))return FALSE;
	handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);
	if(!handle)return FALSE;

	memset(buf, value, iSize);
	iLoop = iLength / iSize;
	for(i = 0; i < iLoop; i++){
		if(!writeContentByFileHandle(handle, buf, iSize))return FALSE;
	}
	if(iLength % sizeof(buf))
		if(!writeContentByFileHandle(handle, buf, iLength % iSize))return FALSE;

	if(!closeFileHandle(handle))return FALSE;

	return TRUE;
}


BOOL CreateEmptyFile(const char * filename){
	void * handle;

	handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);
	if(!handle)return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

BOOL AppendContent(const char * filename, const void * pContent, int iLen){
	void * handle;

	if(iLen <= 0)return FALSE;
	handle = getFileHandle(filename, OPENMODE_APPEND_BIN);
	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen))return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}


BOOL AppendString(const char * filename, const char * pString){
	return AppendContent(filename, pString, strlen(pString));
}

BOOL WriteFileContentAtPos(const char * filename, const void * pContent, int iLen, long lPos){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;
	handle = getFileHandle(filename, OPENMODE_RW);
	if((!handle) || (!pContent))return FALSE;
	if(!seekByFileHandle(handle, lPos))return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen))return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

BOOL WriteFileContent(const char * filename, void * pContent, int iLen){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;
	handle = getFileHandle(filename, OPENMODE_WRITE_BIN);
	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen))return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;

}
BOOL CreateWriteFileContent(const char * filename, const void * pContent, int iLen){
	void * handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);

	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen))return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;

}

BOOL ReadFileContentAtPos(const char * filename, void * pContent, int iLen, long lPos){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;

	handle = getFileHandle(filename, OPENMODE_READ_BIN);
	if(!handle)return FALSE;
	if(!seekByFileHandle(handle, lPos))return FALSE;
	if(!readContentByFileHandle(handle, pContent, iLen))return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

void * getFileHandle(const char * pFilename, enumOpenMode mode){
	void * handle = NULL;

	FILE  *fp;
//	LOGD("31");
	if(!pFilename)return NULL;
//	LOGD("32");
	fp = fopen(pFilename, getOpenMode(mode));
	handle = fp;
//	LOGD("33");
	return handle;
}

BOOL closeFileHandle(void * fileHandle){
	int result;

	FILE  *fp;
//	LOGD("26");
	if(!fileHandle)return FALSE;
	fp = (FILE *)fileHandle;
	result = fclose(fp);
//	LOGD("27");
	return TRUE;

}

BOOL readContentByFileHandle(void * fileHandle, void * pBuffer, int iLen){
	int result;

	FILE  *fp;

	if((!fileHandle) || (!pBuffer))return FALSE;
	
	fp = (FILE *)fileHandle;
	result = fread(pBuffer, iLen, 1, fp);
	return TRUE;
}

BOOL writeContentByFileHandle(void * fileHandle, const void * pBuffer, int iLen){
	int result;

	FILE  *fp;

	if((!fileHandle) || (!pBuffer))return FALSE;
	fp = (FILE *)fileHandle;
	result = fwrite(pBuffer, iLen, 1, fp);
	return TRUE;

}

BOOL seekByFileHandle(void * fileHandle, long lPos){
	int result;
	FILE  *fp;

	if(!fileHandle)return FALSE;
	fp = (FILE *)fileHandle;
	result = fseek(fp, lPos, SEEK_SET);
	return TRUE;

}

int getFileLength(const char * filename){
	int size;
	void * handle;

	handle = getFileHandle(filename, OPENMODE_READ_BIN);
//	LOGD("22");
	if(!handle)return 0;
	size = GetFileSizeByFileHandle(handle);
	if(!closeFileHandle(handle))return 0;
	return size;
}

int GetFileSizeByFileHandle(void * fileHandle){
	int size;
	if(!fileHandle)return 0;
	if(fseek((FILE *)fileHandle, 0, SEEK_END) != 0)return 0;

	size = ftell((FILE *)fileHandle);
	return size;
}

char * GetFileStringByFileHandle(void * fileHandle, char * pBuffer, int iLen){
	char * p;

	if((!fileHandle) || (!pBuffer) || (iLen <= 0))return NULL;
	p = fgets(pBuffer, iLen, (FILE *)fileHandle);
	return p;
}




