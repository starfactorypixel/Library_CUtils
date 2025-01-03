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

// Структура расчёта тока через шунт + INA180
struct INACurrentCalc
{
	uint16_t adc;		// Разрядность АЦП, указываем как (1 << bits)
	uint32_t vref;		// Опорное напряжение, мВ
	uint16_t shunt;		// Сопротивление шунта, мОм
	uint16_t gain;		// Усиление INA180
	uint32_t k;			// Рассчитанный коэффициент


	constexpr INACurrentCalc(uint16_t adc, uint32_t vref, uint32_t shunt, uint32_t gain) : 
		adc(adc), vref(vref), shunt(shunt), gain(gain), 
		k( ((vref * 1000 * 1000 + ((shunt * gain * adc) / 2)) / (shunt * gain * adc)) )
	{}

	constexpr uint32_t Get_mA(uint16_t adc_value) const
	{
		return (adc_value * k) / 1000;
	}

	constexpr float Get_A(uint16_t adc_value) const
	{
		return (float)(adc_value * k) / (1000 * 1000);
	}
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
