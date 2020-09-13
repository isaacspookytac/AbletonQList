#include "../app.h"
#include "confirmview.h"
#include "inputview.h"

#include "../hw.h"
#include "../colors.h"
#include "../viewfactory.h"


InputView::InputView(View* lastView, const char* prompt, const char* defaultValue, std::function<void(const char*)> callback) {
	
	
	// for inputting strings
	_lastView = lastView;
	_callback = callback;
	App::setTitle(prompt, 0xFFFF, 0xFA00);
	_firstChar = ' ';
	_lastChar = '~';
	

	if (defaultValue[0]=='\0') {
		// give the user a space rather than a \0, which will print weird
		_string[0]=' ';
		_string[1]='\0';
	} else {
		snprintf(_string, MAX_STRING_LENGTH, "%s", defaultValue);
	}

	_cursor = strlen(_string)-1;				// move cursor to end
}

void InputView::loop() {

	// using booleans so we can do a test for multiple buttons without losing the value
	bool upButtonPressed = hw::upButton.wasPressedWithRepeat();
	bool downButtonPressed = hw::downButton.wasPressedWithRepeat();

	if ( (upButtonPressed && hw::downButton.isDown()) || (hw::upButton.isDown() && downButtonPressed) ) deleteAtCursor();

	else if (upButtonPressed)   						up();
	else if (downButtonPressed)     					down();
	else if (hw::goButton.wasPressedWithRepeat())   	left();
	else if (hw::stopButton.wasPressedWithRepeat())    	right();
	else if (hw::funButton.wasPressed())                quit();

}

void InputView::draw() {

	clear();

	size_t stringLength = strlen(_string);
	drawCharFan(_cursor);
	
	for (size_t i=0; i<stringLength; i++) {
		drawChar(i);
	}

}

void InputView::drawChar(size_t c, bool clear) {
	hw::tft.setTextFont(FONT_BIG);
	
	if (clear) {
		hw::tft.setTextColor(0x0000, 0x0000);
	} else if (c == _cursor) {
		hw::tft.setTextColor(0xFFFF, 0x0000);
	} else {
		hw::tft.setTextColor(0xC618, 0x0000);
	}
		
	hw::tft.setCursor(c*CHAR_WIDTH+STRING_X - (hw::tft.charWidth(_string[c])/2), STRING_Y);        // centre
	drawGlyph(_string[c]);
}

void InputView::clearCharFan(size_t c) {
	hw::tft.fillRect(STRING_X+c*CHAR_WIDTH-6, VIEW_Y, 12, VIEW_HEIGHT, 0x0000);
}

void InputView::drawGlyph(char c) {
	/*out[0]=c;
	out[1]='\0';*/
	hw::tft.write(c);
}

void InputView::drawCharFan(size_t c) {

	int ch;
	// clear char fan first
	//clearCharFan(c);
	
	hw::tft.setTextFont(FONT_SMALL);
	for (int i = 1; i<=FAN_SIZE; i++) {
		//write the character above
		ch = (_string[c]-i);
		if (ch < _firstChar) ch =  _lastChar - (_firstChar - ch) +1;       // wraparound

		hw::tft.setCursor(STRING_X+c*CHAR_WIDTH - (hw::tft.charWidth(ch)/2),      // centre
										 STRING_Y-i*CHAR_HEIGHT + 3);
		hw::tft.setTextColor( Colors::HsvToRgb565(ch * (255.0f / (_lastChar-_firstChar)), 255, 255), 0x0000);
		drawGlyph(ch);

		// write the character below
		ch = (_string[c]+i);
		if (ch > _lastChar) ch = _firstChar + (ch - _lastChar) - 1;       // wraparound

		hw::tft.setCursor(STRING_X+c*CHAR_WIDTH - (hw::tft.charWidth(ch)/2),
										 STRING_Y+i*CHAR_HEIGHT + 6);
		hw::tft.setTextColor( Colors::HsvToRgb565(ch * (255.0f / (_lastChar-_firstChar)), 255, 255), 0x0000);
		drawGlyph(ch);
		
		
	}
}

void InputView::up() { 
	clearChar(_cursor);
	
	if (_string[_cursor] > _firstChar) {
		_string[_cursor]--;
	} else {
		_string[_cursor] = _lastChar;
	}
	drawCharFan(_cursor);
	drawChar(_cursor);
	
}

void InputView::down() {
	clearChar(_cursor);
	
	if (_string[_cursor] < _lastChar) {
		_string[_cursor]++;
	} else {
		_string[_cursor] = _firstChar;
	}
	drawCharFan(_cursor);
	drawChar(_cursor);
	
}

void InputView::left() {
	
	if (_cursor == 0) return;    
	_cursor--;
	
	drawCharFan(_cursor);
	clearCharFan(_cursor+1);
	drawChar(_cursor);
	drawChar(_cursor+1);        // redraw old position;
	
}

void InputView::right() {
	if (_cursor >= MAX_STRING_LENGTH-2) return;    
	
	_cursor++;
	
	if (_string[_cursor] == '\0') {
		// extend the string if necessary
		_string[_cursor] = ' ';
		_string[_cursor+1] = '\0';
	}

	drawCharFan(_cursor);
	clearCharFan(_cursor-1);
	drawChar(_cursor);
	drawChar(_cursor-1);        // redraw old position;
	
}

void InputView::quit() {

	//callbackFromConfirm(true);
	App::setView(
		// bind the function call we pass to a member function of this particular instance
		new ConfirmView(this, "Write this shit?", std::bind(&InputView::callbackFromConfirm, this, std::placeholders::_1) )
	);

	//App::setView(_lastView);

}

void InputView::callbackFromConfirm(bool result) {



	if (result==true) {
		//strip whitespace off end of string
		int i = strlen(_string)-1;
		
		while (true) {
			// if it's a space, keep iterating down the string
			// if it isn't, we've found the last relevant character, terminate string
			if (isspace(_string[i]) && i>0) {	
				i--;
			} else {
				_string[i+1] = '\0';
				break;
			}
		}
				
		// pass it back to whoever called us
		_callback(_string);
		
	}
	App::setView(_lastView);

}

void InputView::deleteAtCursor() {
	// delete characters from cursor onward
	size_t stringLength = strlen(_string);
	
	if (stringLength == 1) {
		_string[0]=' ';
		draw();
		return;
	}
	
	clearChar(stringLength-1);       // clear the last char seeing as we're deleting it
	
	for (size_t i=_cursor; i < stringLength; i++) {
		_string[i] = _string[i+1];
	}    
	if (_string[_cursor] == '\0') {
		clearCharFan(_cursor);
		_cursor--;
	}
	
	draw();
}
