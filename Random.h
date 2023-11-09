#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

class Random
{
public:
    Random() { _SetSeed(HAL_GetUIDw0()); }
    PiRandomxelRandom(uint32_t seed) { _SetSeed(seed); }

    void Set(uint32_t seed) { _SetSeed(seed); }
    uint32_t Get() { return _GetRandom(); }
    uint32_t Get(uint32_t min, uint32_t max) { return (_GetRandom() % (max - min + 1)) + min; }

private:
    uint32_t _GetRandom() { return rand(); }
    void _SetSeed(uint32_t seed) { srand(seed); }
};