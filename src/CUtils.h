#pragma once

#ifdef HAL_MODULE_ENABLED
//#include "CUtils_Random_STM32.h"
#endif

#define sizeofarray(arr)	(sizeof(arr) / sizeof((arr)[0]))

#include "CUtils_Analog.h"
#include "CUtils_String.h"
#include "CUtils_Math.h"
#include "CUtils_PID.h"
#include "CUtils_CollectionStream.h"
#include "CUtils_MovingAverage.h"
#include "CUtils_Logic.h"
#include "CUtils_CRC16.h"
#include "CUtils_RingBuffer.h"
#include "CUtils_Crypto.h"
