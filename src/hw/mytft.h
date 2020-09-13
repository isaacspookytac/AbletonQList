#pragma once
#include <TFT_eSPI.h>


class MyTFT : public TFT_eSPI {
public:

	int getWidth() {
		return 320;
	}

	int getHeight() {
		return 240;
	}

	MyTFT() {
		init();
		setRotation(3);
		setTextFont(2);
		fillScreen(TFT_BLACK);
		setTextWrap(false, false);
		setTextDatum(TL_DATUM);

	}

	int16_t charWidth(char c) {
		char temp[] = {c, '\0'};
		return textWidth(temp);
	}
};
