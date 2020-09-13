#pragma once
#include <Arduino.h>

namespace Colors {

	extern const uint16_t RED;
	extern const uint16_t ORANGE;
	extern const uint16_t GREEN;
	extern const uint16_t BLUE;
	extern const uint16_t PURPLE;
	extern const uint16_t NONE;

	uint16_t mixColors(uint16_t A, uint16_t B, float ratio);
	uint16_t addColors(uint16_t A, uint16_t B);
	uint16_t subtractColors(uint16_t A, uint16_t B);
	uint16_t Rgb565(byte R8, byte G8, byte B8); 
	uint16_t HsvToRgb565(byte h, byte s, byte v);

}