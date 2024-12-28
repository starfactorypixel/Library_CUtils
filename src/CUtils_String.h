#pragma once
#include <inttypes.h>

void ArrayReverse(uint8_t *array, uint16_t length)
{
	if(length == 0) return;
	
	uint8_t i = 0;
	uint8_t j = length - 1;
	uint8_t temp;
	
	while(i < j)
	{
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;

		i++;
		j--;
	}
	
	return;
}

template <typename T> 
void array_reverse(T *array, uint8_t count)
{
	uint8_t i = 0;
	uint8_t j = count - 1;
	T temp;
	while(i < j)
	{
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
		
		i++;
		j--;
	}
	
	return;
}

void memcpy_reverse(uint8_t *dest, const uint8_t *src, uint16_t len)
{
	if(len == 0) return;
	
	const uint8_t *src_end = src + len - 1;
	while(len--)
		*dest++ = *src_end--;
	
	return;
}
