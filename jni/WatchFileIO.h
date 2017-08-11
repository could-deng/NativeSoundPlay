#ifndef WATCH_FILEIO_H
#define WATCH_FILEIO_H

#include "WatchDefine.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define LINE_CONTENT_MAX_CHARACTERS 64
#define MAX_FILENAME_LENGTH 64
	

typedef enum{
OPENMODE_READ_TEXT,
OPENMODE_READ_BIN,
OPENMODE_WRITE_TEXT,
OPENMODE_WRITE_BIN,
OPENMODE_APPEND_BIN,
OPENMODE_CREATE_WRITE_BIN,
OPENMODE_RW,
OPENMODE_MAX
}enumOpenMode;


BOOL isFileExist(const char * filename);
BOOL CreateBinFileOfLength(const char * filename, int iLength, char value);
int getFileLength(const char * filename);
BOOL getTextFileContent(const char * filename, char * pBuffer, int iBufSize);

BOOL CreateEmptyFile(const char * filename);
BOOL AppendContent(const char * filename, const void * pContent, int iLen);
BOOL AppendString(const char * filename, const char * pString);
BOOL ReadFileContentAtPos(const char * filename, void * pBuf, int iLen, long lPos);
BOOL WriteFileContentAtPos(const char * filename, const void * pBuf, int iLen, long lPos);
BOOL WriteFileContent(const char * filename, void * pContent, int iLen);
BOOL CreateWriteFileContent(const char * filename, const void * pContent, int iLen);

void * getFileHandle(const char * pFilename, enumOpenMode mode);
BOOL closeFileHandle(void * fileHandle);
BOOL readContentByFileHandle(void * fileHandle, void * pBuffer, int iLen);
BOOL writeContentByFileHandle(void * fileHandle, const void * pBuffer, int iLen);
BOOL seekByFileHandle(void * fileHandle, long lPos);
char * GetFileStringByFileHandle(void * fileHandle, char * pBuffer, int iLen);
int GetFileSizeByFileHandle(void * fileHandle);



#if defined(__cplusplus)
}
#endif 

#endif //WATCH_FILEIO_H



