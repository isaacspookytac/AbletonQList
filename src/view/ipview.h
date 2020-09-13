#ifndef IPVIEW_H
#define IPVIEW_H

class IPView : public InputView {


public:

	IPView(View* lastView, const char* prompt, const char* defaultValue, std::function<void(const char*)> callback) {
		// for inputting strings
		_lastView = lastView;
		_callback = callback;
		App::setTitle(prompt, 0xFFFF, 0xFA00);
		_firstChar = 0;
		_lastChar = 255;

		STRING_X = 30;
		CHAR_WIDTH = 40;

		if (defaultValue[0]=='\0') {
			_string[0] = 0;
			_string[1] = 0;
			_string[2] = 0;
			_string[3] = 0;
		} //else {
			//snprintf(_string, MAX_STRING_LENGTH, "%3u.", defaultValue);
		//}

		_cursor = strlen(_string)-1;				// move cursor to end
	}

	void drawGlyph(char c) {
		char temp[8];
		snprintf(temp, 8, "%3d", (unsigned int)c);
		hw::tft.print(temp);
	}

	void clearCharFan(size_t c) {
		hw::tft.fillRect(STRING_X+c*CHAR_WIDTH-6, VIEW_Y, 12, VIEW_HEIGHT, 0x0000);
	}

	void draw() {

		clear();

		size_t stringLength = strlen(_string);
		drawCharFan(_cursor);
		
		for (size_t i=0; i<stringLength; i++) {
			drawChar(i);
			hw::tft.setCursor(i*CHAR_WIDTH+STRING_X+30, STRING_Y);        // centre
			hw::tft.print(".");
		}

	}

	~IPView() { }

	
};


#endif