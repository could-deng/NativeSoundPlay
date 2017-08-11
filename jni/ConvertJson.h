//
//  ConvertJson.h
//  fitmix_v2.0
//
//  Created by tosakai on 2017/8/4.
//  Copyright © 2017年 lishuzhao. All rights reserved.
//

#ifndef ConvertJson_h
#define ConvertJson_h

#include <stdio.h>

int converToGPSJason(const char * pGPSFile, const char * pDistanceFile, const char * pJasonFile);

int converToStepJason(const char * pStepFile, const char * pDistanceFile, const char * pHRFile, const char * pJasonFile);

int converToLogJason(const char * pLogFile, const char * pJasonFile);

#endif /* ConvertJson_h */
