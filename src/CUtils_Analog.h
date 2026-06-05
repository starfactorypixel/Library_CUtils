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
	static constexpr uint16_t _corr_base = 10000;
	
	public:
	
		constexpr DividerVoltageCalc(uint16_t adc_bit, uint16_t vref, uint32_t r1, uint32_t r2, int16_t correction = 0) : 
			_max_adc((1U << adc_bit) - 1U), _vref(vref), _r1(r1), _r2(r2), _correction(_corr_base + correction), 
			_k( (((uint64_t)vref * (r1 + r2) << _shift) + (r2 * _max_adc) / 2) / (r2 * _max_adc) )
		{}
		
		// Разрядность АЦП
		void SetAdcBits(uint8_t adc_bit)
		{
			_max_adc = ((1U << adc_bit) - 1U);
		}
		
		// Опорное напряжение, мВ
		void SetVref(uint16_t vref)
		{
			_vref = vref;
		}
		
		// Верхний резистор, Ом
		void SetR1(uint32_t r1)
		{
			_r1 = r1;
		}
		
		// Нижний резистор, Ом
		void SetR2(uint32_t r2)
		{
			_r2 = r2;
		}

		// Поправка задаётся в сотых долях процента (0.01%)
		// '0' -> без коррекции (100.00%), '125' -> +1.25%, '-50' -> -0.50%
		void SetCorrection(int16_t correction)
		{
			_correction = _corr_base + correction;
		}
		
		// Пересчёт коэфицента. Вызывать при любом вызове Set метода
		void ReCalc()
		{
			_k = (((uint64_t)_vref * (_r1 + _r2) << _shift) + (_r2 * _max_adc) / 2) / (_r2 * _max_adc);
		}
		
		// Получить значение в мВ
		inline uint32_t Getmv(uint32_t adc) const
		{
			uint64_t mv = ((uint64_t)adc * _k) >> _shift;
			if(_correction != _corr_base)
				mv = (mv * _correction + (_corr_base / 2)) / _corr_base;
			
			return (uint32_t)mv;
		}
		
	private:
		
		uint16_t _max_adc;
		uint16_t _vref;
		uint32_t _r1;
		uint32_t _r2;
		uint16_t _correction;
		uint32_t _k;
};

