
#include <Arduino.h>
#include "hw.h"
#include <cmath>

namespace Colors {

	extern const uint16_t RED = 0x5986;// App::tft.color565(90, 50, 50);
	extern const uint16_t ORANGE = 0x5A25;// App::tft.color565(90, 70, 43);
	extern const uint16_t GREEN = 0x32C6;// App::tft.color565(50, 90, 50);
	extern const uint16_t BLUE = 0x31EB;//App::tft.color565(50, 60, 90);
	extern const uint16_t PURPLE =  0x518A;//tft.color565(80, 50, 80);*/
	extern const uint16_t NONE = 0x2945;


	uint16_t Rgb565(byte R8, byte G8, byte B8) {
		byte R5 = ( R8 * 249 + 1014 ) >> 11;
		byte G6 = ( G8 * 253 +  505 ) >> 10;
		byte B5 = ( B8 * 249 + 1014 ) >> 11;

		return (R5 << 11) + (G6 << 5) + (B5);
	}


	uint16_t mixColors(uint16_t A, uint16_t B, float ratio) {
		//ratio 0 = all A, ratio 1 = all B, 0.6 = middle
		const uint16_t maskG = 0b0000011111100000;
		const uint16_t maskB = 0b0000000000011111;

		uint16_t Ar = (A >> 11);
		uint16_t Ag = ((A & maskG) >> 5);
		uint16_t Ab = (A & maskB);

		uint16_t Br = (B >> 11);
		uint16_t Bg = ((B & maskG) >> 5);
		uint16_t Bb = (B & maskB);

	    uint16_t Cr = int(Br - Ar)*ratio + Ar;
    	uint16_t Cg = int(Bg - Ag)*ratio + Ag;
    	uint16_t Cb = int(Bb - Ab)*ratio + Ab;

		return (Cr << 11) + (Cg << 5) + (Cb);
		//return Rgb565(Cr,Cg,Cb);
    	//return new Color(r, g, b);


/*		//average them all
		uint16_t Cr = (Ar+Br) / 2;
		uint16_t Cg = (Ag+Bg) / 2;
		uint16_t Cb = (Ab+Bb) / 2;

		return (Cr << 11) + (Cg << 5) + (Cb);*/
	}

	uint16_t addColors(uint16_t A, uint16_t B) {

		//const uint16_t maskR = 0b1111100000000000;
		const uint16_t maskG = 0b0000011111100000;
		const uint16_t maskB = 0b0000000000011111;

		uint16_t Ar = A >> 11;
		uint16_t Ag = (A & maskG) >> 5;
		uint16_t Ab = A & maskB;

		uint16_t Br = B >> 11;
		uint16_t Bg = (B & maskG) >> 5;
		uint16_t Bb = B & maskB;

		uint16_t Cr = (Ar + Br) < 0b11111 ? (Ar+Br) : 0b11111;
		uint16_t Cg = (Ag + Bg) < 0b111111 ? (Ag+Bg) : 0b111111;
		uint16_t Cb = (Ab + Bb) < 0b11111 ? (Ab+Bb) : 0b11111;

		return (Cr << 11) + (Cg << 5) + (Cb);
		//byte G6 = ( G8 * 253 +  505 ) >> 10;
		//byte B5 = ( B8 * 249 + 1014 ) >> 11;
	}

	uint16_t subtractColors(uint16_t A, uint16_t B) {

		//const uint16_t maskR = 0b1111100000000000;
		const uint16_t maskG = 0b0000011111100000;
		const uint16_t maskB = 0b0000000000011111;

		uint16_t Ar = A >> 11;
		uint16_t Ag = (A & maskG) >> 5;
		uint16_t Ab = A & maskB;

		uint16_t Br = B >> 11;
		uint16_t Bg = (B & maskG) >> 5;
		uint16_t Bb = B & maskB;

		uint16_t Cr = (Ar - Br) > 0 ? (Ar-Br) : 0;
		uint16_t Cg = (Ag - Bg) > 0 ? (Ag-Bg) : 0;
		uint16_t Cb = (Ab - Bb) > 0 ? (Ab-Bb) : 0;

		return (Cr << 11) + (Cg << 5) + (Cb);
		//byte G6 = ( G8 * 253 +  505 ) >> 10;
		//byte B5 = ( B8 * 249 + 1014 ) >> 11;
	}



	uint16_t HsvToRgb565(byte h, byte s, byte v)
	{
		
		byte region, remainder, p, q, t;
		byte r, g, b;

		if (s == 0)
		{
			r = v;
			g = v;
			b = v;
			return Rgb565(r,g,b);
		}

		region = h / 43;
		remainder = (h - (region * 43)) * 6; 

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region)
		{
			case 0:
				r = v; g = t; b = p;
				break;
			case 1:
				r = q; g = v; b = p;
				break;
			case 2:
				r = p; g = v; b = t;
				break;
			case 3:
				r = p; g = q; b = v;
				break;
			case 4:
				r = t; g = p; b = v;
				break;
			default:
				r = v; g = p; b = q;
				break;
		}
		
		return Rgb565(r,g,b);
	}

}

/*
HsvColor RgbToHsv(RgbColor rgb)
{
	HsvColor hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
	rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

	v = rgbMax;
	if (v == 0)
	{
		h = 0;
		s = 0;
		return hsv;
	}

	s = 255 * long(rgbMax - rgbMin) / v;
	if (s == 0)
	{
		h = 0;
		return hsv;
	}

	if (rgbMax == r)
		h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
	else if (rgbMax == g)
		h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
	else
		h = 171 + 43 * (r - g) / (rgbMax - rgbMin);

	return hsv;
}
*/