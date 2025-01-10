#pragma once
#include <inttypes.h>

template <typename T> 
bool IsOneBitSet(T value)
{
	return (value & (value - 1)) == 0 && value != 0;
}
