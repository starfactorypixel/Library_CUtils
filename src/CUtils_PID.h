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
