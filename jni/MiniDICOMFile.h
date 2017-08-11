#ifndef MINI_DICOM_FILE_H
#define MINI_DICOM_FILE_H


#include "WatchDefine.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define MAX_MINI_DICOM_FILE_HEAD_LEN 256

BOOL isDataIncludeGroupHead(char * pBuffer, int iBufLen);
BOOL getMiniDICOMFileTagValue(char * pBuffer, int iBufLen, U32 tag, char * pValue, int iValueLen);
BOOL readMiniDICOMFile(const char * pFilename, char * pBuffer, int *pBufLen, int iOffset, int iDataLen);
BOOL readMiniDICOMFileWhole(const char * pFilename, char * pBuffer, int *pBufLen);
BOOL readMiniDICOMFileHead(const char * pFilename, char * pBuffer, int *pBufLen, U32 tagStartAddress);
BOOL readMiniDICOMFileWholeData(const char * pFilename, char * pBuffer, int *pBufLen, int iHeadLen);
int getMiniDICOMFileDataLen(const char * pFilename, int iHeadLen);
int getMiniDICOMFileCellCount(const char * pFilename, int iHeadLen, int iCellLen);

#if defined(__cplusplus)
}
#endif 

#endif //MINI_DICOM_FILE_H



