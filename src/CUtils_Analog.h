#pragma once
#include <inttypes.h>

class INACurrentCalc
{
	static constexpr uint16_t _shift = 16;
	static constexpr uint16_t _corr_base = 10000;
	
	public:
		
		constexpr INACurrentCalc(uint16_t adc_bit, uint16_t vref, uint16_t shunt, uint16_t gain, int16_t correction = 0) : 
			_max_adc((1U << adc_bit) - 1U), _vref(vref), _shunt(shunt), _gain(gain), _correction(_corr_base + correction), 
			_k( ((((uint64_t)vref * 1000ULL * 1000ULL) << _shift) + ((uint64_t)shunt * gain * _max_adc) / 2) / ((uint64_t)shunt * gain * _max_adc) )
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
		
		// Сопротивление шунта, мОм
		void SetShunt(uint16_t shunt)
		{
			_shunt = shunt;
		}
		
		// Коэффициент усиления INA
		void SetGain(uint16_t gain)
		{
			_gain = gain;
		}
		
		// Поправка задаётся в сотых долях процента (0.01%)
		// '0' -> без коррекции (100.00%), '125' -> +1.25%, '-50' -> -0.50%
		void SetCorrection(int16_t correction)
		{
			_correction = _corr_base + correction;
		}
		
		// Пересчёт коэффициента. Вызывать при любом вызове Set метода
		void ReCalc()
		{
			_k = ((((uint64_t)_vref * 1000ULL * 1000ULL) << _shift) + ((uint64_t)_shunt * _gain * _max_adc) / 2) / ((uint64_t)_shunt * _gain * _max_adc);
		}
		
		inline uint32_t Get_mA(uint16_t adc_value) const
		{
			return (uint32_t)(_Calculate(adc_value) / 1000ULL);
		}
		
		inline float Get_A(uint16_t adc_value) const
		{
			return (float)(_Calculate(adc_value) / 1000000.0f);
		}
		
	private:
		
		inline uint32_t _Calculate(uint16_t adc_value) const
		{
			uint64_t current_uA = ((uint64_t)adc_value * _k) >> _shift;
			if(_correction != _corr_base)
				current_uA = (current_uA * _correction + (_corr_base / 2)) / _corr_base;
			
			return (uint32_t)current_uA;
		}
		
		uint16_t _max_adc;
		uint16_t _vref;
		uint16_t _shunt;
		uint16_t _gain;
		uint16_t _correction;
		uint32_t _k;
};

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
		
		// Пересчёт коэффициента. Вызывать при любом вызове Set метода
		void ReCalc()
		{
			_k = (((uint64_t)_vref * (_r1 + _r2) << _shift) + (_r2 * _max_adc) / 2) / (_r2 * _max_adc);
		}
		
		// Получить значение в мВ
		inline uint32_t GetmV(uint32_t adc_value) const
		{
			return (uint32_t)_Calculate(adc_value);
		}

		// Получить значение в мВ
		inline float GetV(uint32_t adc_value) const
		{
			return (float)(_Calculate(adc_value) / 1000.0f);
		}
		
	private:
		
		inline uint32_t _Calculate(uint16_t adc_value) const
		{
			uint64_t voltage_mV = ((uint64_t)adc_value * _k) >> _shift;
			if(_correction != _corr_base)
				voltage_mV = (voltage_mV * _correction + (_corr_base / 2)) / _corr_base;
			
			return (uint32_t)voltage_mV;
		}
		
		uint16_t _max_adc;
		uint16_t _vref;
		uint32_t _r1;
		uint32_t _r2;
		uint16_t _correction;
		uint32_t _k;
};
