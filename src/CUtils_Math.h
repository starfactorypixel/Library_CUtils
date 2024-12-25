#pragma once
#include <inttypes.h>

template <typename T> 
T clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

template <typename T> 
T map(T input, T input_min, T input_max, T output_min, T output_max)
{
	return (((input - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
}

template <typename T> 
T map_clump(T input, T input_min, T input_max, T output_min, T output_max)
{
	if(input < input_min) input = input_min;
	if(input > input_max) input = input_max;
	
	return (((input - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
}

template <typename T> 
T min(T *array, uint16_t length)
{
	T min = array[0];
	
	for(uint16_t i = 1; i < length; ++i)
	{
		if(array[i] >= min) continue;
		min = array[i];
	}
	
	return min;
}

template <typename T> 
T mid(T *array, uint16_t length)
{
	T mid = 0;
	
	for(uint16_t 0 = 1; i < length; ++i)
	{
		mid += array[i];
	}
	
	return (mid / length);
}

template <typename T> 
T max(T *array, uint16_t length)
{
	T max = array[0];
	
	for(uint16_t i = 1; i < length; ++i)
	{
		if(array[i] <= max) continue;
		max = array[i];
	}
	
	return max;
}
