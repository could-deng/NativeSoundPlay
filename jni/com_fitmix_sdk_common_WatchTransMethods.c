
#include "com_fitmix_sdk_common_WatchTransMethods.h"

#include <stddef.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#endif

#include<android/log.h>
// LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
#define LOGD(...) __android_log_print(ANDROID_LOG_VERBOSE, "watch_transfer", __VA_ARGS__)

#include <string.h>
#include <stdlib.h>

#include "WatchFileIO.h"
#include "WatchSportsLogData.h"
#include "WatchSportsSensorData.h"

#define BUFFER1_LEN (600 * 1024)
#define BUFFER2_LEN (400 * 1024)
#define OUT_BUFFER_LEN (20 * 1024)



JNIEXPORT jint JNICALL Java_com_fitmix_sdk_common_WatchTransMethods_square
  (JNIEnv *env, jclass clazz, jint num){
	return num*num;
}

/*
	����˵��:��IronCloud��GPS���ݺ�Distance�����ںϳ�jason�ļ�
	����˵��:pGPSFile		GPS�����ļ�·��(in)
			 pDistanceFile	Distance�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

*/
JNIEXPORT jint JNICALL Java_com_fitmix_sdk_common_WatchTransMethods_converToGPSJason
  (JNIEnv *env, jclass clazz, jstring pGPSFile, jstring pDistanceFile, jstring pJasonFile){



	const char *c_str_gps = NULL;
	const char *c_str_distance = NULL;
	const char *c_str_jason = NULL;

	char buff[128] = {0};
	jboolean isCopy;    // ����JNI_TRUE��ʾԭ�ַ����Ŀ���������JNI_FALSE��ʾ����ԭ�ַ�����ָ��
	c_str_gps = (*env)->GetStringUTFChars(env, pGPSFile, &isCopy);
	c_str_distance = (*env)->GetStringUTFChars(env, pDistanceFile, &isCopy);
	c_str_jason = (*env)->GetStringUTFChars(env, pJasonFile, &isCopy);


//	LOGD("bbb");
//	LOGD(pGPSFile);
//
//	FILE  *fp;
//	fp = fopen(c_str, "rb+");
//
//	//"/storage/emulated/0/TransferWord/Sensor/gpsSensor.txt"
//
//	if(!fp){
//		LOGD("AAA");
//	}else{
//		LOGD("bbb");
//	}



	char * pBuffer1 = NULL, *pBuffer2 = NULL;
	    char * pOutBuffer = NULL;

	    if(!c_str_gps)return 1;
	    if(!c_str_distance)return 1;
	    if(!c_str_jason)return 1;

	    if(!CreateEmptyFile(c_str_jason))return 2;
	    pBuffer1 = (char *)malloc(BUFFER1_LEN);
	    pBuffer2 = (char *)malloc(BUFFER2_LEN);
	    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
	    if(!pBuffer1 || !pBuffer2 || !pOutBuffer){
	        if(pBuffer1)free(pBuffer1);
	        if(pBuffer2)free(pBuffer2);
	        if(pOutBuffer)free(pOutBuffer);
	        return 3;
	    }
	    if(!parseGPSFileWithSpeed(c_str_gps, pBuffer1, BUFFER1_LEN,
	    		c_str_distance, pBuffer2, BUFFER2_LEN,
	    		c_str_jason, pOutBuffer, OUT_BUFFER_LEN))return 4;

	    if(pBuffer1)free(pBuffer1);
	    if(pBuffer2)free(pBuffer2);
	    if(pOutBuffer)free(pOutBuffer);

	    return 0;
}
/*
	����˵��:��IronCloud��Step���ݺ�Distance����,HR�����ںϳ�jason�ļ�
	����˵��:pStepFile		Step�����ļ�·��(in)
			 pDistanceFile	Distance�����ļ�·��(in)
			 pHRFile		HR�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

*/
JNIEXPORT jint JNICALL Java_com_fitmix_sdk_common_WatchTransMethods_converToStepJason
  (JNIEnv *env, jclass clazz, jstring pStepFile, jstring pDistanceFile, jstring pHRFile, jstring pJasonFile){


	const char *c_str_step = NULL;
	const char *c_str_distance = NULL;
	const char *c_str_hr = NULL;
	const char *c_str_jason = NULL;


	char buff[128] = {0};
	jboolean isCopy;    // ����JNI_TRUE��ʾԭ�ַ����Ŀ���������JNI_FALSE��ʾ����ԭ�ַ�����ָ��
	c_str_step = (*env)->GetStringUTFChars(env, pStepFile, &isCopy);
	c_str_distance = (*env)->GetStringUTFChars(env, pDistanceFile, &isCopy);
	c_str_hr = (*env)->GetStringUTFChars(env, pHRFile, &isCopy);
	c_str_jason = (*env)->GetStringUTFChars(env, pJasonFile, &isCopy);


	char * pBuffer1 = NULL, *pBuffer2 = NULL;
	    char * pOutBuffer = NULL;

	    if(!c_str_step)return 1;
	    if(!c_str_distance)return 1;
	    if(!c_str_hr)return 1;
	    if(!c_str_jason)return 1;

	    if(!CreateEmptyFile(c_str_jason))return 2;
	    pBuffer1 = (char *)malloc(BUFFER1_LEN);
	    pBuffer2 = (char *)malloc(BUFFER2_LEN);
	    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
	    if(!pBuffer1 || !pBuffer2 || !pOutBuffer){
	        if(pBuffer1)free(pBuffer1);
	        if(pBuffer2)free(pBuffer2);
	        if(pOutBuffer)free(pOutBuffer);
	        return 3;
	    }
	    if(!parseDistanceAndStepAndHRFile(c_str_step, pBuffer1, BUFFER1_LEN,
	    		c_str_distance, pBuffer2, BUFFER2_LEN,
	    		c_str_hr, pBuffer2, BUFFER2_LEN,
	    		c_str_jason, pOutBuffer, OUT_BUFFER_LEN))return 4;

	    if(pBuffer1)free(pBuffer1);
	    if(pBuffer2)free(pBuffer2);
	    if(pOutBuffer)free(pOutBuffer);

	    return 0;

}

/*
	����˵��:��Log�����ںϳ�jason�ļ�
	����˵��:pLogFile		Log�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

*/
JNIEXPORT jint JNICALL Java_com_fitmix_sdk_common_WatchTransMethods_converToLogJason
  (JNIEnv *env, jclass clazz, jstring pLogFile, jstring pJasonFile){

	const char *c_str_log = NULL;
	const char *c_str_jason = NULL;

	char buff[128] = {0};
	jboolean isCopy;    // ����JNI_TRUE��ʾԭ�ַ����Ŀ���������JNI_FALSE��ʾ����ԭ�ַ�����ָ��
	c_str_log = (*env)->GetStringUTFChars(env, pLogFile, &isCopy);
	c_str_jason = (*env)->GetStringUTFChars(env, pJasonFile, &isCopy);


	char * pBuffer2 = NULL;
	    char * pOutBuffer = NULL;

	    if(!c_str_log)return 1;
	    if(!c_str_jason)return 1;

	    if(!CreateEmptyFile(c_str_jason))return 2;
	    pBuffer2 = (char *)malloc(BUFFER2_LEN);
	    pOutBuffer = (char *)malloc(OUT_BUFFER_LEN);
	    if(!pBuffer2 || !pOutBuffer){
	        if(pBuffer2)free(pBuffer2);
	        if(pOutBuffer)free(pOutBuffer);
	        return 3;
	    }
	    if(!parseLogFile(c_str_log, pBuffer2, BUFFER2_LEN, pOutBuffer))return 4;
	    AppendString(c_str_jason, pOutBuffer);

	    if(pBuffer2)free(pBuffer2);
	    if(pOutBuffer)free(pOutBuffer);

	    return 0;
}






