#pragma once
#include <stdint.h>

class Perlin2DInt
{
	public:

		// Шум возвращает значение в диапазоне [-1024, 1024]
		static int16_t noise2d(int32_t x, int32_t y);
		
	private:

		static int8_t grad(int32_t hash, int32_t x, int32_t y);
		static int32_t fade(int32_t t);
		static int32_t lerp(int32_t a, int32_t b, int32_t t);
		
		static uint8_t perm[256];
		
};
