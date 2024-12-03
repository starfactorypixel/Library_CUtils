#pragma once
#include <inttypes.h>


struct volt_calc_t
{
	uint16_t maxadc;	// Максимальное значение АЦП, указываем как ((1 << bits) - 1)
	uint16_t vref;		// Опорное напряжение, мВ
	uint32_t r1 = 0;	// Верхний резистор делителя, Ом
	uint32_t r2 = 1;	// Нижний резистор делителя, Ом
	int16_t adj = 0;	// Коррекция в промилле
};

uint32_t VoltageCalculate(const uint32_t adc, const volt_calc_t &params)
{
	const uint32_t dividerCoefficient = ((params.r1 + params.r2) * 1000) / params.r2;
	const uint32_t adcVoltage = (adc * params.vref) / params.maxadc;
	uint32_t inputVoltage = (adcVoltage * dividerCoefficient) / 1000;
	if(params.adj != 0)
	{
		inputVoltage = (inputVoltage * (1000 + params.adj)) / 1000;
	}
	
	return inputVoltage;
}
