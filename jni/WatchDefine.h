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
SPORTS_TYPE_RUN_OUTDOOR,		//室外跑
SPORTS_TYPE_RUN_INDOOR,			//室内跑
SPORTS_TYPE_WALK,				//徒步
SPORTS_TYPE_CROSSCOUNTRY,		//越野跑
SPORTS_TYPE_RIDE,				//骑行(公路)
SPORTS_TYPE_SWIM_INDOOR,		//室内游泳
SPORTS_TYPE_SWIM_OUTDOOR,		//室外游泳
SPORTS_TYPE_ROCK_CLIMB,			//攀岩
SPORTS_TYPE_MOUNTAINS_CLIMB,	//爬山
SPORTS_TYPE_SKI,				//滑雪
SPORTS_TYPE_FISH,				//钓鱼
SPORTS_TYPE_JUMP,				//跳伞
SPORTS_TYPE_ROPE,				//跳绳
SPORTS_TYPE_DIVE,				//潜水
SPORTS_TYPE_ROW,				//划船
SPORTS_TYPE_RIDE_CROSSCOUNTRY,	//山地车
SPORTS_TYPE_RIDE_INDOOR,		//室内骑行
SPORTS_TYPE_MAX
}enumSportsType;

typedef enum{
SWIM_GESTURE_BREASTSTROKE,		//蛙泳
SWIM_GESTURE_BACKSTROKE,		//仰泳
SWIM_GESTURE_BUTTERFLYSTROKE,		//蝶泳
SWIM_GESTURE_FREESTYLE,		//自由泳
SWIM_GESTURE_DOGSTYLE,		//狗刨
SWIM_GESTURE_TREADWATER,		//踩水
SWIM_GESTURE_BACK_FREESTYLE,		//仰泳中自由姿势
SWIM_GESTURE_DIVE,		//潜水
SWIM_GESTURE_MAX
}enumSwimGesture;


#if defined(__cplusplus)
}
#endif 

#endif //WATCH_DEFINE_H



