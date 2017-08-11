#ifndef WATCH_DEFINE_H
#define WATCH_DEFINE_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#undef I32
#undef U32
#undef I16
#undef U16
#undef U8
#undef I8
#undef I64

#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed int   /*   signed 32 bits. */
#define U32 unsigned int   /* unsigned 32 bits. */

#ifdef WIN32
#define I64 __int64 /* signed 64 bits. */
#else
#define I64 long long  /* signed 64 bits. */
#endif

#define G_VAL (9.8)
#define WATCH_ROW_SPACE 5
typedef unsigned char   BOOL;
#define TRUE 1
#define FALSE 0
#define PI (3.14159265f)
#define MAG 3
#define POWER_LOW_LEVEL 15
#define CHAR_CANNOT_DISPLAY 0xBFDA
#define CHAR_DEGREE_TEMPRATURE 0xA1E6
#define CHAR_DEGREE_ANGLE 0xA1E3
#define LAST_HOURS_FOR_CURVE 12

typedef void  (*pfDefaultFunction)(void);



typedef enum{
SPORTS_TYPE_RUN_OUTDOOR,		//������
SPORTS_TYPE_RUN_INDOOR,			//������
SPORTS_TYPE_WALK,				//ͽ��
SPORTS_TYPE_CROSSCOUNTRY,		//ԽҰ��
SPORTS_TYPE_RIDE,				//����(��·)
SPORTS_TYPE_SWIM_INDOOR,		//������Ӿ
SPORTS_TYPE_SWIM_OUTDOOR,		//������Ӿ
SPORTS_TYPE_ROCK_CLIMB,			//����
SPORTS_TYPE_MOUNTAINS_CLIMB,	//��ɽ
SPORTS_TYPE_SKI,				//��ѩ
SPORTS_TYPE_FISH,				//����
SPORTS_TYPE_JUMP,				//��ɡ
SPORTS_TYPE_ROPE,				//����
SPORTS_TYPE_DIVE,				//Ǳˮ
SPORTS_TYPE_ROW,				//����
SPORTS_TYPE_RIDE_CROSSCOUNTRY,	//ɽ�س�
SPORTS_TYPE_RIDE_INDOOR,		//��������
SPORTS_TYPE_MAX
}enumSportsType;

typedef enum{
SWIM_GESTURE_BREASTSTROKE,		//��Ӿ
SWIM_GESTURE_BACKSTROKE,		//��Ӿ
SWIM_GESTURE_BUTTERFLYSTROKE,		//��Ӿ
SWIM_GESTURE_FREESTYLE,		//����Ӿ
SWIM_GESTURE_DOGSTYLE,		//����
SWIM_GESTURE_TREADWATER,		//��ˮ
SWIM_GESTURE_BACK_FREESTYLE,		//��Ӿ����������
SWIM_GESTURE_DIVE,		//Ǳˮ
SWIM_GESTURE_MAX
}enumSwimGesture;


#if defined(__cplusplus)
}
#endif 

#endif //WATCH_DEFINE_H



