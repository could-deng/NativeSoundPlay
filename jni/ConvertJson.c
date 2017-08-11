//
//  ConvertJson.c
//  fitmix_v2.0
//
//  Created by tosakai on 2017/8/4.
//  Copyright © 2017年 lishuzhao. All rights reserved.
//

#include "ConvertJson.h"
#include <string.h>
#include <stdlib.h>

#include "WatchFileIO.h"
#include "WatchSportsLogData.h"
#include "WatchSportsSensorData.h"

#define BUFFER1_LEN (600 * 1024)
#define BUFFER2_LEN (400 * 1024)
#define OUT_BUFFER_LEN (20 * 1024)



/*
	功能说明:将IronCloud的GPS数据和Distance数据融合成jason文件
	参数说明:pGPSFile		GPS数据文件路径(in)
 pDistanceFile	Distance数据文件路径(in)
 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
 1		参数错误
 2		不能创建jason文件
 3		内存分配失败
 4		数据文件解析错误
 
 */
int converToGPSJason(const char * pGPSFile, const char * pDistanceFile, const char * pJasonFile){
    char * pBuffer1 = NULL, *pBuffer2 = NULL;
    char * pOutBuffer = NULL;
    
    if(!pGPSFile)return 1;
    if(!pDistanceFile)return 1;
    if(!pJasonFile)return 1;
    
    if(!CreateEmptyFile(pJasonFile))return 2;
    pBuffer1 = (char *)malloc(BUFFER1_LEN);
    pBuffer2 = (char *)malloc(BUFFER2_LEN);
    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
    if(!pBuffer1 || !pBuffer2 || !pOutBuffer){
        if(pBuffer1)free(pBuffer1);
        if(pBuffer2)free(pBuffer2);
        if(pOutBuffer)free(pOutBuffer);
        return 3;
    }
    if(!parseGPSFileWithSpeed(pGPSFile, pBuffer1, BUFFER1_LEN,
                              pDistanceFile, pBuffer2, BUFFER2_LEN,
                              pJasonFile, pOutBuffer, OUT_BUFFER_LEN))return 4;
    
    if(pBuffer1)free(pBuffer1);
    if(pBuffer2)free(pBuffer2);
    if(pOutBuffer)free(pOutBuffer);
    
    return 0;
}
/*
	功能说明:将IronCloud的Step数据和Distance数据,HR数据融合成jason文件
	参数说明:pStepFile		Step数据文件路径(in)
 pDistanceFile	Distance数据文件路径(in)
 pHRFile		HR数据文件路径(in)
 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
 1		参数错误
 2		不能创建jason文件
 3		内存分配失败
 4		数据文件解析错误
 
 */
int converToStepJason(const char * pStepFile, const char * pDistanceFile, const char * pHRFile, const char * pJasonFile){
    char * pBuffer1 = NULL, *pBuffer2 = NULL;
    char * pOutBuffer = NULL;
    
    if(!pStepFile)return 1;
    if(!pDistanceFile)return 1;
    if(!pHRFile)return 1;
    if(!pJasonFile)return 1;
    
    if(!CreateEmptyFile(pJasonFile))return 2;
    pBuffer1 = (char *)malloc(BUFFER1_LEN);
    pBuffer2 = (char *)malloc(BUFFER2_LEN);
    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
    if(!pBuffer1 || !pBuffer2 || !pOutBuffer){
        if(pBuffer1)free(pBuffer1);
        if(pBuffer2)free(pBuffer2);
        if(pOutBuffer)free(pOutBuffer);
        return 3;
    }
    if(!parseDistanceAndStepAndHRFile(pStepFile, pBuffer1, BUFFER1_LEN,
                                      pDistanceFile, pBuffer2, BUFFER2_LEN,
                                      pHRFile, pBuffer2, BUFFER2_LEN,
                                      pJasonFile, pOutBuffer, OUT_BUFFER_LEN))return 4;
    
    if(pBuffer1)free(pBuffer1);
    if(pBuffer2)free(pBuffer2);
    if(pOutBuffer)free(pOutBuffer);
    
    return 0;
}

/*
	功能说明:将Log数据融合成jason文件
	参数说明:pLogFile		Log数据文件路径(in)
 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
 1		参数错误
 2		不能创建jason文件
 3		内存分配失败
 4		数据文件解析错误
 
 */
int converToLogJason(const char * pLogFile, const char * pJasonFile){
    char * pBuffer2 = NULL;
    char * pOutBuffer = NULL;
    
    if(!pLogFile)return 1;
    if(!pJasonFile)return 1;
    
    if(!CreateEmptyFile(pJasonFile))return 2;
    pBuffer2 = (char *)malloc(BUFFER2_LEN);
    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
    if(!pBuffer2 || !pOutBuffer){
        if(pBuffer2)free(pBuffer2);
        if(pOutBuffer)free(pOutBuffer);
        return 3;
    }
    if(!parseLogFile(pLogFile, pBuffer2, BUFFER2_LEN, pOutBuffer))return 4;
    AppendString(pJasonFile, pOutBuffer);
    
    if(pBuffer2)free(pBuffer2);
    if(pOutBuffer)free(pOutBuffer);
    
    return 0;
}


