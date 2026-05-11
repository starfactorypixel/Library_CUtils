#pragma once
#include <inttypes.h>


struct [[deprecated("Use DividerVoltageCalc class")]] volt_calc_t
{
	uint16_t maxadc;	// Максимальное значение АЦП, указываем как ((1 << bits) - 1)
	uint16_t vref;		// Опорное напряжение, мВ
	uint32_t r1;		// Верхний резистор делителя, Ом
	uint32_t r2;		// Нижний резистор делителя, Ом
	int16_t adj;		// Коррекция в промилле

	volt_calc_t(uint16_t m, uint16_t v, uint32_t r1_, uint32_t r2_, int16_t a)
        : maxadc(m), vref(v), r1(r1_), r2(r2_), adj(a) {}
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

[[deprecated("Use DividerVoltageCalc class")]]
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








class DividerVoltageCalc
{
	static constexpr uint16_t _shift = 16;
	
	public:
		
		constexpr DividerVoltageCalc(uint16_t adc_bit, uint16_t vref, uint32_t r1, uint32_t r2) : 
			_max_adc((1U << adc_bit) - 1U), _vref(vref), _r1(r1), _r2(r2), 
			_k( (((uint64_t)vref * (r1 + r2) << _shift) + (r2 * _max_adc) / 2) / (r2 * _max_adc) )
		{}
		
		void SetAdcBits(uint8_t adc_bit)
		{
			_max_adc = ((1U << adc_bit) - 1U);
		}
		
		void SetVref(uint16_t vref)
		{
			_vref = vref;
		}
		
		void SetR1(uint32_t r1)
		{
			_r1 = r1;
		}
		
		void SetR2(uint32_t r2)
		{
			_r2 = r2;
		}

		void ReCalc()
		{
			_k = (((uint64_t)_vref * (_r1 + _r2) << _shift) + (_r2 * _max_adc) / 2) / (_r2 * _max_adc);
		}
		
		inline uint32_t Getmv(uint32_t adc) const
		{
			return (adc * _k) >> _shift;
		}
		
	private:
		
		uint16_t _max_adc;
		uint16_t _vref;
		uint32_t _r1;
		uint32_t _r2;
		uint32_t _k;
};

