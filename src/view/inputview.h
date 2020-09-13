#ifndef INPUTVIEW_H
#define INPUTVIEW_H

#include <EEPROM.h>
#include "view.h"
#include "../hw.h"


class InputView : public View {
protected:
	byte _firstChar = ' ';  //0x20 or 32
	byte _lastChar = '~';   //0x7E or 126
	
	int FAN_SIZE = 6;
	int FONT_BIG = 2;
	int FONT_SMALL = 1;
	int STRING_X = 10;
	int STRING_Y = 100;
	int CHAR_WIDTH = 10;
	int CHAR_HEIGHT = 12;
	

	size_t _cursor;
	char _string[MAX_STRING_LENGTH];

	std::function<void(const char*)> _callback;
	virtual void drawGlyph(char c);
/*
	InputView(const char* prompt, char* destination, byte ascii_from, byte ascii_to) {
		_first = ascii_from;
		_last + ascii_to;
	}*/
	InputView() { }
public:
	
	InputView(View* lastView, const char* prompt, const char* defaultValue, std::function<void(const char*)> callback);
	
	//InputView(View* lastView, const char* prompt, const int defaultValue, unsigned int* destination);
	
	virtual ~InputView() { }

	virtual void loop();

	virtual void draw();
	virtual void drawChar(size_t c, bool clear);
	virtual void drawChar(size_t c) { drawChar(c, false); }
	virtual void clearChar(size_t c) { drawChar(c, true); }
	
	virtual void clearCharFan(size_t c);
	virtual void drawCharFan(size_t c);
	
	virtual void up();
	virtual void down();
	virtual void right();
	virtual void left();
	
	virtual void quit();
	virtual void deleteAtCursor();
	virtual void callbackFromConfirm(bool result);

	
};
#endif