#include "../hw/logwindow.h"
#include "../hw/mytft.h"
#include "../hw.h"


LogWindow::LogWindow(int x, int y, int w, int h) {

	_x = x; _y = y;
	_w = w; _h = h;
	_cursorX = x; _cursorY = y;

	Serial.begin(74880);
	//Serial.begin(115200);
	Serial.flush();
	
	LAST_CHAR_X = int((floor((w-1) / CH_WIDTH) * CH_WIDTH) + x);
	LAST_CHAR_Y = int((floor((h-1) / CH_HEIGHT) * CH_HEIGHT) + y);

	clear();
	echoToSerial(true);
	//setTextColor(0x07E0);
	setTextColor(0x0442);
	setBackgroundColor(0x0000);
}

void LogWindow::setBackgroundColor(uint16_t color) {
	_bg = color;
}

void LogWindow::echoToSerial(bool echo) {
	_echo = echo;
}

void LogWindow::clear() {
	hw::tft.fillRect(_x,_y,_w,_h,_bg);
	_cursorX = _x; _cursorY = _y;
}

size_t LogWindow::write(uint8_t c) {
	
	push();     // save values for size, color, etc.
	size_t ret = 0;
	
	hw::tft.setTextFont(1);
	hw::tft.setTextColor(_fg, _bg);
	
	if (_cursorY > LAST_CHAR_Y) {
		clear();                                // maybe implement scrolling later.
	}
	
	hw::tft.setCursor(_cursorX, _cursorY);

	if (c != '\n') {
		ret = hw::tft.write(c);
		// move cursor
		_cursorX = _cursorX + CH_WIDTH;
		if ((_cursorX > LAST_CHAR_X) && _wrap) {
			// wrap, new line.
			_cursorX = _x;
			_cursorY = _cursorY + CH_HEIGHT;
		}
		
	} else {
		// new line
		_cursorY = _cursorY + CH_HEIGHT;
		_cursorX = _x;
		ret = 0;
	}

	pop();    // restore values

	if (_echo) { Serial.write(c); }
	return ret;
}

void LogWindow::push() {
	_oldTextFont = hw::tft.textfont;
	_saveCursorX = hw::tft.getCursorX(); _saveCursorY = hw::tft.getCursorY();
	_savetextcolor = hw::tft.textcolor;
	_savetextbgcolor = hw::tft.textbgcolor;
}

void LogWindow::pop() {
	hw::tft.setTextFont(_oldTextFont);
	hw::tft.setCursor(_saveCursorX, _saveCursorY);
	hw::tft.setTextColor(_savetextcolor, _savetextbgcolor);
}
