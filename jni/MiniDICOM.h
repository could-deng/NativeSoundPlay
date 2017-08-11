#ifndef MINI_DICOM_H
#define MINI_DICOM_H


#include "WatchDefine.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


#define TAG_MIN_LENGTH 10
#define IRON_HEAD_LEN 8
#define CHAPTER_SUMMARY_LEN 26
#define PREFIX_DATA_LENGTH 34
#define TAG_GROUP_LENGTH 10
#define MAX_VALID_CHAPTER_DATA_LENGTH 990

typedef enum{
	TAG_GROUP_RESERVED,					//����
	TAG_GROUP_CMD_BASE,
	TAG_GROUP_REQUEST = TAG_GROUP_CMD_BASE,					//����
	TAG_GROUP_RESPONSE,					//Ӧ��
	TAG_GROUP_CHAPTER,					//�½�

	TAG_GROUP_PHONE_PUSH_BASE = 100,
	TAG_GROUP_NOTIFICATION = TAG_GROUP_PHONE_PUSH_BASE,				// ֪ͨ
	TAG_GROUP_WEATHER,					//����
	TAG_GROUP_CALL,						//ͨ��
	TAG_GROUP_USERINFO,					//�û���Ϣ
	TAG_GROUP_UPGRADE,					//����
	TAG_GROUP_BIND,						//��
	TAG_GROUP_DIAL_PALTE,				//��������
	TAG_GROUP_TRAINING_PROGRAM,		//ѵ���ƻ�
	TAG_GROUP_CHALLENGE,				//��ս
	TAG_GROUP_HEALTH,			//��������

	TAG_GROUP_WATCH_PUSH_BASE = 500,
	TAG_GROUP_FIND  = TAG_GROUP_WATCH_PUSH_BASE,					//�໥����
	TAG_GROUP_ADD_LOG,					//��Ӽ�¼
	TAG_GROUP_REMOVE_LOG,				//ɾ���˶���¼
	TAG_GROUP_DOWNLOAD_LOGS,			//���ؽ����˶���¼
	TAG_GROUP_SPORTS_SUMMARY_DATA,	//��������
	TAG_GROUP_SPORTS_GROUP_DATA,		//�����ͷ����������
	TAG_GROUP_SENSOR_DATA,				//SENSOR����
	TAG_GROUP_LOCALE_SETTINGS,			//��������
	TAG_GROUP_ERROR_REPORT,			// ���󱨸�
	TAG_GROUP_TIME,						//ʱ��ͬ��
	TAG_GROUP_GPS,						//GPSͬ��
	TAG_GROUP_MUSIC,		//����
	TAG_GROUP_SYN_LOG_DONE,	//ͬ�����
	TAG_GROUP_MAX
	}enumTagGroup;

typedef enum{
	PACKAGE_RESULT_SUCCESS,
	PACKAGE_RESULT_FAILURE,
	PACKAGE_RESULT_DECODE_ERROR,
	PACKAGE_RESULT_TIME_OUT,
	PACKAGE_RESULT_MAX
	}enumPackageResult;

typedef enum{
	VR_VOID,					//������
	VR_S32,					//�з���32λ
	VR_U32,					//�޷���32λ
	VR_S16,					//�з���16λ
	VR_U16,					//�޷���16λ
	VR_DOUBLE,				//16�ֽ�˫����
	VR_FLOAT,				//8�ֽڸ���
	VR_STRING_ANSI,			//ANSI�ַ���
	VR_STRING_GB2312,		//GB2312�ַ���
	VR_STRING_BIG5,			//BIG5�ַ���
	VR_STRING_UTF8,			//UTF8�ַ���
	VR_STRING_UTF16_BE,		//UTF16 Big Enddian�ַ���
	VR_STRING_UTF16_LE,		//UTF16 Little Enddian�ַ���
	VR_STRING_UTF32,			//UTF32 �ַ���
	VR_MAX
	}enumVRType;

typedef enum{
	REQUEST_RESERVED,
	REQUEST_TAG,
	REQUEST_POS,
	REQUEST_TOTALLEN,
	REQUEST_MAX
	}enumRequestCell;

typedef enum{
	RESPONSE_RESERVED,
	RESPONSE_RESULT,
	RESPONSE_MAX
	}enumReponseCell;


typedef enum{
	NOTIFICATION_BLE_RESERVED,
	NOTIFICATION_BLE_SMS,		//����
	NOTIFICATION_BLE_QQ,			//QQ
	NOTIFICATION_BLE_WEICHART,	//΢��
	NOTIFICATION_BLE_EMAIL,		//�ʼ�
	NOTIFICATION_BLE_NEWS,		//����
	NOTIFICATION_BLE_WEIBO,		//΢��
	NOTIFICATION_BLE_FITMIX,		//Fitmix�������͵���Ϣ
	NOTIFICATION_BLE_MISSEDCALL,	//δ�ӵ绰
	NOTIFICATION_BLE_WANGWANG,		//����
	NOTIFICATION_BLE_DINGDING,		//����
	NOTIFICATION_BLE_ALIPAY,		//֧����
	NOTIFICATION_BLE_MAX		
	}enumNotificationCell;

typedef enum{
	NOTIFICATION_FIELD_TITLE,		//������
	NOTIFICATION_FIELD_DATE,		//ԭʼ��Ϣ������
	NOTIFICATION_FIELD_TIME,		//ԭʼ��Ϣ��ʱ��
	NOTIFICATION_FIELD_CONTENT,	//��Ϣ����
	NOTIFICATION_FIELD_MAX
	}enumNotificationField;

typedef enum{
	WEATHER_BLE_RESERVED,
	WEATHER_BLE_QUERY,				//�ֱ�˷��͵Ĳ�ѯ����
	WEATHER_BLE_TYPE,				//��������,��Ҫ����ͼ�����ʾ��
	WEATHER_BLE_CITY,				//����
	WEATHER_BLE_WEATHER_DATE,		//��������
	WEATHER_BLE_WEATHER_TIME,		//����ʱ��
	WEATHER_BLE_UPDATE_DATE,		//���ݵĸ�������
	WEATHER_BLE_UPDATE_TIME,		//���ݵĸ���ʱ��
	WEATHER_BLE_TITLE,				//��������
	WEATHER_BLE_TEMPRATURE,		//����
	WEATHER_BLE_PRESSURE,			//��ѹ
	WEATHER_BLE_HUMITY,				//ʪ��
	WEATHER_BLE_WIND,				//��
	WEATHER_BLE_AIR_QUALITY,			//��������
	WEATHER_BLE_PM25,				//PM2.5
	WEATHER_BLE_SPORTS,				//�˶�ָ��
	WEATHER_BLE_ULTRAVIOLET,		//������ǿ��
	WEATHER_BLE_COLD,				//��ð����
	WEATHER_BLE_ALARM,				//����Ԥ��
	WEATHER_BLE_DETAIL,				//��ϸ����
	WEATHER_BLE_MAX
	}enumWeatherCell;

typedef enum{
	CALL_BLE_RESERVED,
	CALL_BLE_TITLE,				//������������
	CALL_BLE_INCOMING,			//����
	CALL_BLE_ANSWER,			//����
	CALL_BLE_DECLINE,			//�Ҷ�
	CALL_BLE_MAX	
	}enumCallCell;

typedef enum{
	USERINFO_BLE_RESERVED,
	USERINFO_BLE_QUERY,			//��ѯ�û���Ϣ
	USERINFO_BLE_SEX,			//�Ա�	0=�� 1=Ů
	USERINFO_BLE_BIRTHDAY,		//����
	USERINFO_BLE_HEIGHT,			//��� cm
	USERINFO_BLE_WEIGHT,		//���� kg
	USERINFO_BLE_MAX
	}enumUserInfoCell;	

typedef enum{
	UPGRADE_BLE_RESERVED,
	UPGRADE_BLE_QUERY,				//��ѯ�汾��Ϣ
	UPGRADE_BLE_VERSIONS_NAME,		//�汾����
	UPGRADE_BLE_VERSION_NO,		//�汾��
	UPGRADE_BLE_VERSION_SUMMARY,	//�汾���
	UPGRADE_BLE_DATA,				//�汾����������
	UPGRADE_BLE_MAX
	}enumUpgradeCell;

typedef enum{
	FIND_BLE_RESERVED,
	FIND_BLE_FINDPHONE,				//Ѱ���ֻ�
	FIND_BLE_FINDWATCH,				//Ѱ���ֱ�
	FIND_BLE_MAX
	}enumFindCell;

typedef enum{
	BIND_BLE_RESERVED,
	BIND_BLE_BIND,					//���ֻ�
	BIND_BLE_UNBIND,				//����ֻ�
	BIND_BLE_BIND_RESULT,			//�󶨽�� 
	BIND_BLE_UNBIND_RESULT,			//����� 
	BIND_BLE_MAX
	}enumBindCell;

typedef enum{
	SPORT_BLE_RESERVED,
	SPORT_BLE_BASE=1,
	SPORT_BLE_TYPE = SPORT_BLE_BASE,		//�˶�����
	SPORT_BLE_START_DATE,						//�˶�����
	SPORT_BLE_START_TIME,						//�˶�ʱ�� 
	SPORT_BLE_DURATION,					//�˶�ʱ��
	SPORT_BLE_TOTAL_CALORIE,				//�˶���·��
	SPORT_BLE_PAUSE_COUNT,			//�˶���ͣ����
	SPORT_BLE_PAUSE_DETAIL,			//�˶���ͣ����(��ʼ����,��������)
	SPORT_BLE_TOTAL_FAT_BURST,		//��֬����
	SPORT_BLE_VO2MAX,				//���������
	SPORT_BLE_HR_STATISTICS,		//����ͳ��

	SPORT_BLE_MOVE_BASE=30,					//�ƶ����˶�
	SPORT_BLE_TOTAL_DISTANCE= SPORT_BLE_MOVE_BASE,		//�˶�����
	SPORT_BLE_AVERAGE_SPEED,						//�˶��ٶ�
	SPORT_BLE_AVERAGE_PACE,							//�˶�����
	SPORT_BLE_AVERAGE_STEP_FREQUENCY,				//�˶���Ƶ
	SPORT_BLE_TOTAL_STEPS,							//�˶�����
	SPORT_BLE_PACE_RECORDS,			//���ٱ�(ʱ��,���)
	SPORT_BLE_GROUP_RECORDS,		//�����¼(ʱ��,���)
	SPORT_BLE_MAX_PACE,				//�������
	SPORT_BLE_MAX_STEP_FREQUENCY,	//��߲�Ƶ
	
	SPORT_BLE_UP_BASE=40,
	SPORT_BLE_AVERAGE_VERTICAL_SPEED = SPORT_BLE_UP_BASE,			//��ֱ�ٶ�
	SPORT_BLE_TOTAL_FOOTSTEPS,					//̨����
	SPORT_BLE_PEAK_ALTITUDE,						//����
	SPORT_BLE_TOTAL_UP_DISTANCE,					//�������
	SPORT_BLE_TOTAL_DOWN_DISTANCE,					//�������
	SPORT_BLE_TROUGH_ALTITUDE,

	SPORT_BLE_RIDE_BASE=50,
	SPORT_BLE_RIDE_POWER=SPORT_BLE_RIDE_BASE,					//���й���
	SPORT_BLE_MAX_CIRCLES_FREQUENCY,			//�������̤Ƶ��
	SPORT_BLE_FEET_CIRCLES,				//���нŵ�Ȧ��
	
	SPORT_BLE_SWIM_BASE=60,
	SPORT_BLE_SWIM_ARM=SPORT_BLE_SWIM_BASE,			//��Ӿ�ӱ۴���
	SPORT_BLE_SWIM_STYLE,			//����
	SPORT_BLE_SWIM_ROUND_TRIPS,			//��������

	SPORT_BLE_OTHER_BASE = 240,

	SPORT_BLE_DETAIL=254,						//�˶���ϸ����
	SPORT_BLE_MAX
	}enumSportCell;

typedef enum{
	REMOVE_LOG_RESERVED,
	REMOVE_LOG_DATETIME,
	REMOVE_LOG_MAX
	}enumRemoveLog;

typedef enum{
	DOWNLOAD_LOG_RESERVED,
	DOWNLOAD_LOG_LOGS,
	DOWNLOAD_LOG_MAX
	}enumDownloadLog;

 typedef enum{
	SUMMARY_RESERVED,
	SUMMARY_QUERY,								//��ѯ��������
	SUMMARY_TOTAL_SPORTS_LOGS,					//���˶���¼��
	SUMMARY_TOTAL_SPORTS_DISTANCE,					//�����
	SUMMARY_TOTAL_SPORTS_DURATION,					//���˶�ʱ��
	SUMMARY_TOTAL_SPORTS_CALORIE,				//�ܿ�·�� 
	SUMMARY_TOTAL_SPORTS_CORE_DATA_VALUES,	//�������ݻ���

	SUMMARY_THIS_MONTH_SPORTS_LOGS,			//�����˶���¼��
	SUMMARY_THIS_MONTH_SPORTS_DISTANCE,		//�����˶����
	SUMMARY_THIS_MONTH_SPORTS_CALORIE,		//�����˶���·��
	SUMMARY_THIS_MONTH_SPORTS_DURATION,		//�����˶�ʱ��
	SUMMARY_THIS_MONTH_SPORTS_CORE_DATA_VALUES,		//�����˶��������ݻ���
	SUMMARY_THIS_WEEK_SPORTS_LOGS,			//�����˶���¼��
	SUMMARY_THIS_WEEK_SPORTS_DISTANCE,			//�����˶����
	SUMMARY_THIS_WEEK_SPORTS_CALORIE,			//���ܿ�·��
	SUMMARY_THIS_WEEK_SPORTS_DURATION,			//�����˶�ʱ��
	SUMMARY_THIS_WEEK_SPORTS_CORE_DATA_VALUES,			//���ܺ������ݻ���

	SUMMARY_SPORT_SYN_DATE,					//��������ͬ�������� 
	SUMMARY_SPORT_SYN_TIME,					//��������ͬ����ʱ��
	SUMMARY_MAX
	}enumSummarySportsData;	


typedef enum{
	GROUP_RESERVED,
	GROUP_QUERY,							//��ѯ��������
	GROUP_TYPE,								//���ݷ�������
	GROUP_THIS_WEEK_SPORTS_LOGS,			//�����˶���¼��
	GROUP_THIS_WEEK_SPORTS_DISTANCE,		//�����˶����
	GROUP_THIS_WEEK_SPORTS_CALORIE,		//�����˶���·��
	GROUP_THIS_WEEK_SPORTS_DURATION,		//�����˶�ʱ��
	GROUP_THIS_WEEK_SPORTS_CORE_DATA_VALUES,		//���ܺ������ݻ���
	GROUP_SPORT_SYN_DATE,					//����ͬ�������� 
	GROUP_SPORT_SYN_TIME,					//����ͬ����ʱ�� 
	GROUP_MAX
	}enumGroupSportsData;	

typedef enum{
	SENSOR_FIELD_RESERVED,		
	SENSOR_FIELD_DATE,				//����VR_STRING_ANSI
	SENSOR_FIELD_TIME,				//ʱ��VR_STRING_ANSI
	SENSOR_FIELD_TIME_SPACE,			//ʱ����VR_STRING_ANSI
	SENSOR_FIELD_CELL_DATA_LEN,			//�������ݳ���VR_STRING_ANSI
	SENSOR_FIELD_IS_SPORT,			//�Ƿ��˶�����VR_STRING_ANSI
	SENSOR_FIELD_START_ADDRESS,		//������ʼλ��VR_STRING_ANSI
	SENSOR_FIELD_TOTAL_DATA_LEN,		//�����ܳ���VR_STRING_ANSI
	SENSOR_FIELD_VERNO,		//�����汾��VR_STRING_ANSI	
	SENSOR_FIELD_VALUE,		//ֵVR_STRING_ANSI	
	SENSOR_FIELD_MAX					
	}enumSensorField;


	typedef enum{
	SENSOR_RESERVED,				
	SENSOR_IDLE_PACKAGE,		//�ճ������ļ���
	SENSOR_IDLE_SENSOR_START = SENSOR_IDLE_PACKAGE,
	SENSOR_GSENSOR,					// GSENSOR����
	SENSOR_TEMPRATURE,				//��������
	SENSOR_PRESSURE,				//��ѹ����
	SENSOR_HEARTRATE,				//��������
	SENSOR_HUMITY,					//ʪ������
	SENSOR_IDLE_SENSOR_END,
	SENSOR_GPS = SENSOR_IDLE_SENSOR_END,						// GPS����
	SENSOR_COMPASS,					// ָ��������
	SENSOR_GYRO,					//����������
	SENSOR_DISTANCE,					//�������
	SENSOR_MAX
	}enumSensor;	


typedef enum{
	DIAL_BLE_RESERVED,
	DIAL_BLE_ID,			//����ID
	DIAL_BLE_DATA,		//���̵����ݰ�
	DIAL_BLE_MAX
	}enumDialCell;

typedef enum{
	HEALTH_BLE_RESERVED,
	HEALTH_BLE_QUERY,					//ˢ�½�������VR_VOID
	HEALTH_BLE_ADIVSE,
	HEALTH_BLE_ITEM_START=100,
	HEALTH_BLE_REST_HR= HEALTH_BLE_ITEM_START,	//��Ϣ����VR_STRING_ANSI
	HEALTH_BLE_VO2MAX,					//���������VR_STRING_ANSI
	HEALTH_BLE_LACTATE_THRESHOLD,		//���ֵᷧVR_STRING_ANSI
	HEALTH_BLE_STRESS_INDEX,				//ѹ��ָ��VR_STRING_ANSI
	HEALTH_BLE_CARDIAC_EFFICIENCY,		//����Ч��VR_STRING_ANSI
	HEALTH_BLE_SLEEP_QUALITY,				//˯������VR_STRING_ANSI
	HEALTH_BLE_CARDIAC_RECOVERY_EFFICIENCY,		//����ָ�Ч��VR_STRING_ANSI
	HEALTH_BLE_BODY_FAT,						//��֬VR_STRING_ANSI
	HEALTH_BLE_BLOOD_SUGAR,						//Ѫ��VR_STRING_ANSI
	HEALTH_BLE_OXYGEN,						//Ѫ��VR_STRING_ANSI
	HEALTH_BLE_BLOOD_PRESSURE,						//ѪѹVR_STRING_ANSI
	HEALTH_BLE_SKIN_CONDUCTANCE,						//Ƥ��VR_STRING_ANSI
	HEALTH_BLE_TEMPRATURE,						//����VR_STRING_ANSI
	HEALTH_BLE_BREATHE_FREQUENCY,				//����Ƶ��VR_STRING_ANSI
	HEALTH_BLE_INDEX_BASE=200,
	HEALTH_BLE_SPORT_INDEX= HEALTH_BLE_INDEX_BASE,	//�˶�ָ��VR_STRING_ANSI
	HEALTH_BLE_HEALTH_INDEX,					//����ָ��VR_STRING_ANSI
	HEALTH_BLE_MAX
	}enumHealthCell;

typedef enum{
	HEALTH_FIELD_RESERVED,		
	HEALTH_FIELD_DATE,				//����VR_STRING_ANSI
	HEALTH_FIELD_TIME,				//ʱ��VR_STRING_ANSI
	HEALTH_FIELD_CELL_DATA_LEN,			//�������ݳ���VR_STRING_ANSI
	HEALTH_FIELD_START_ADDRESS,		//������ʼλ��VR_U32
	HEALTH_FIELD_TOTAL_DATA_LEN,		//�����ܳ���VR_STRING_ANSI
	HEALTH_FIELD_VERNO,		//�����汾��VR_STRING_ANSI
	HEALTH_FIELD_VALUE,		//��������ֵ
	HEALTH_FIELD_MAX					
	}enumHealthField;



typedef enum{
	TIME_BLE_RESERVED,
	TIME_BLE_QUERY,				//�ֱ��͵�ʱ���ѯ
	TIME_BLE_DATE_PART,			//����
	TIME_BLE_TIME_PART,			//ʱ��
	TIME_BLE_MAX
	}enumTimeCell;

typedef enum{
	GPS_BLE_RESERVED,
	GPS_BLE_POS_QUERY,			//GPS��ѯ
	GPS_BLE_EPHEMERIS_QUERY,	//������ѯ
	GPS_BLE_LONGTITUDE,			//����
	GPS_BLE_LATITUDE,			//γ��
	GPS_BLE_DATE,				//����
	GPS_BLE_TIME,				//ʱ��
	GPS_BLE_ALTITUDE,			//����
	GPS_BLE_DECLINATION,			//��ƫ�� 
	GPS_BLE_EPHEMERIS,			//����
	GPS_BLE_MAX,
	}enumGPSCell;

typedef enum{
	ERROR_REPORT_BLE_RESERVED,
	ERROR_REPORT_BLE_LOG,					//��־
	ERROR_REPORT_BLE_MAX,
	}enumErrorReportCell;

typedef enum{
	LOCALE_SETTINGS_BLE_RESERVED,
	LOCALE_SETTINGS_BLE_DATA,		//��������
	LOCALE_SETTINGS_BLE_MAX,
	}enumLocaleSettingsCell;

typedef enum{
	MUSIC_BLE_RESERVED,
	MUSIC_BLE_QUERY,		//��ѯ����VR_VOID
	MUSIC_BLE_VOLUME,		//��ǰ����VR_STRING_ANSI
	MUSIC_BLE_VOLUME_MAX,	//�������VR_STRING_ANSI
	MUSIC_BLE_SONG_NAME,		//������VR_STRING_UTF8
	MUSIC_BLE_AUTHOR,		//����VR_STRING_UTF8
	MUSIC_BLE_ALBUM,		//ר����VR_STRING_UTF8
	MUSIC_BLE_PROGRESS,		//��ǰ����VR_STRING_ANSI(��)
	MUSIC_BLE_DURATION,		//�ܲ���ʱ��VR_STRING_ANSI(��)
	MUSIC_BLE_STATE,		//�ܲ���ʱ��VR_STRING_ANSI 0=��ͣ 1=����
	MUSIC_BLE_PREV,		//��һ��VR_VOID
	MUSIC_BLE_NEXT,		//��һ��VR_VOID
	MUSIC_BLE_PAUSE,		//��ͣVR_VOID
	MUSIC_BLE_PLAY,		//����VR_VOID
	MUSIC_BLE_STOP,		//ֹͣVR_VOID
	MUSIC_BLE_MAX,
	}enumMusicCell;

typedef enum{
	SYN_DONE_BLE_RESERVED,
	SYN_DONE_BLE_DATE,			
	SYN_DONE_BLE_TIME,	
	SYN_DONE_BLE_MAX
	}enumSynDoneCell;

typedef enum{
	DATA_COMPARE_UNUSE,
	DATA_COMPARE_LE,			// <=
	DATA_COMPARE_EQ,		// ==
	DATA_COMPARE_GE,			// >=
	DATA_COMPARE_LT,			// <
	DATA_COMPARE_GT,			// >
	DATA_COMPARE_NOT_EQ,	// !=
	DATA_COMPARE_MAX
	}enumDataCompare;

typedef enum{
WISH_TYPE_INT,
WISH_TYPE_DATE,
WISH_TYPE_TIME,
WISH_TYPE_STRING,
WISH_TYPE_VOID,
WISH_TYPE_MAX
}enumWishType;

typedef struct{
U32 tag;
U16 vr;
int iLen;
char * pBuf;
}structTagParser;

char * getAnsiStringOfMiniDICOMData(char * pData, int iDataLen);
int getIntValueOfMiniDICOMData(char * pData, int iDataLen);
U32 getU32ValueOfMiniDICOMData(char * pData, int iDataLen);
U16 getU16ValueOfMiniDICOMData(char * pData, int iDataLen);
short getI16ValueOfMiniDICOMData(char * pData, int iDataLen);
U8 getU8ValueOfMiniDICOMData(char * pData, int iDataLen);
char getCharValueOfMiniDICOMData(char * pData, int iDataLen);
double getDoubleValueOfMiniDICOMData(char * pData, int iDataLen);
float getFloatValueOfMiniDICOMData(char * pData, int iDataLen);
char * getGB2312StringOfMiniDICOMData(char * pData, int iDataLen);
U16 getUTF8StringOfMiniDICOMData(char * pData, int iDataLen, char * pDest, int iDestLen);
int getDateStringValueOfMiniDICOMData(char * pData, int iDataLen);
int getTimeStringValueOfMiniDICOMData(char * pData, int iDataLen);
BOOL getMiniDICOMValueBySmartMode(int vr, char * pData, int iDataLen, char * pDest, int iDestLen, int iWishType);

U16 getGroupOfTag(U32 tag);
U16 getCellOfTag(U32 tag);
U8 getFirstByteOfCell(U16 cell);
U8 getSecondByteOfCell(U16 cell);
U32 getTagByGroupAndCell(U16 group, U16 cell);
U16 getCellByBytes(U8 b1, U8 b2);
int getFirstGroupPos(void);
int getFirstTagPos(void);

BOOL isGroupFlag(U32 tag);
BOOL parseTagValid(char * pBuf, structTagParser * pParser);
BOOL isGroupValid(char * pBuf, int iBufLen);
BOOL searchTag(U32 tag, char * pBuf, int iLimitLen, structTagParser * pParser, int *pDataPos);


#if defined(__cplusplus)
}
#endif 

#endif //MINI_DICOM_H



