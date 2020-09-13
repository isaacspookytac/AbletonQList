#ifndef LOGWINDOW_H
#define LOGWINDOW_H

//#include "hw.h"
#include <Arduino.h>

class LogWindow : public Print {
private:

	int _x, _y, _w, _h;
	int _cursorX, _cursorY;
	
	uint16_t _fg, _bg;
	bool _echo;
	
	static const byte CH_WIDTH = 6;
	static const byte CH_HEIGHT = 8;
	int LAST_CHAR_X;
	int LAST_CHAR_Y;
	
	byte _oldTextFont;
	int _saveCursorX, _saveCursorY;
	uint16_t _savetextcolor, _savetextbgcolor;
	bool _wrap;

	void push();
	void pop();
	
public:
 
	LogWindow(int x, int y, int w, int h);

	void setBackgroundColor(uint16_t color);

	void echoToSerial(bool echo);

	void clear();
	
	size_t write(uint8_t c);

	void setTextColor(int16_t fg) {  _fg = fg; }
	void setCursor(int x, int y) { _cursorX = x; _cursorY = y; }
	int getCursorX() { return _cursorX; }
	int getCursorY() { return _cursorY; }
	void setWrap(boolean wrap) { _wrap = wrap; }
	
};

#endif
