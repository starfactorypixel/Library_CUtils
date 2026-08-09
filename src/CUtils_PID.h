#pragma once
#include <inttypes.h>

/*
	KP - При слишком низком 
	: система будет реагировать медленно, оставляя значительную ошибку.
	При слишком высоком 
	: система может начать колебаться или стать нестабильной.

	Ki - При слишком низком 
	: остаточная ошибка может сохраняться.
	При слишком высоком 
	: система становится склонной к осцилляциям и перерегулированию, так как интегральная часть накапливается медленно, но влияет долго.

	Kd - При слишком низком 
	: система может испытывать колебания.
	При слишком высоком 
	: регулятор становится слишком чувствительным к шуму и быстро меняющимся сигналам, что может привести к нестабильности.
*/

template <typename T>
class PIDController
{
	public:
		
		PIDController(T kp, T ki, T kd, T out_min, T out_max) : 
			_kp(kp), _ki(ki), _kd(kd), _out_min(out_min), _out_max(out_max), _integral(0), _previous_error(0)
		{}
		
		T Calculate(T setpoint, T measured_value, T dt)
		{
			T error = setpoint - measured_value;			// Вычисление ошибки
			T derivative = (error - _previous_error) / dt;	// Производная часть
			_integral += error * dt;						// Интегральная часть
			_previous_error = error;
			
			// Рассчитываем выход
			T output = _kp * error + _ki * _integral + _kd * derivative;
			
			return output;
		}
		
		void Reset()
		{
			_integral = (T)0;
			_previous_error = (T)0;
			
			return;
		}
		
	private:
		T _kp, _ki, _kd;		// Коэффициенты PID
		T _out_min, _out_max;	// Лимиты для расчёта PID
		T _integral;			// Накопление для интегральной части
		T _previous_error;		// Предыдущее значение ошибки


};

template <typename T> 
class PIDController2
{
	public:
		PIDController2(T kp, T ki, T kd, T out_min, T out_max) : 
			_kp(kp), _ki(ki), _kd(kd), _out_min(out_min), _out_max(out_max), 
			_integral{}, _previous_measurement{}, _first_run(true)
		{

		}

		T Calculate(T setpoint, T measured_value, T dt)
		{
			if(dt <= T{}) return T{};
			
			const T error = setpoint - measured_value;
			T derivative = T{};
			
			if(!_first_run)
			{
				derivative = -(measured_value - _previous_measurement) / dt;
			}
			_first_run = false;
			_previous_measurement = measured_value;
			
			const T p = _kp * error;
			const T d = _kd * derivative;
			
			T i = _ki * _integral;
			T output = p + i + d;
			
			const bool upper_saturated = (output >= _out_max) && (error > T{});
			const bool lower_saturated = (output <= _out_min) && (error < T{});
			if(!(upper_saturated || lower_saturated))
			{
				_integral += error * dt;
				i = _ki * _integral;
				output = p + i + d;
			}
			
			return clamp(output, _out_min, _out_max);
		}
		
		void Reset()
		{
			_integral = T{};
			_previous_measurement = T{};
			_first_run = true;
		}
	
	private:
	
		T _kp, _ki, _kd;
		T _out_min, _out_max;
		T _integral;
		T _previous_measurement;
		bool _first_run;
};
