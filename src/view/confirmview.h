#ifndef CONFIRMVEW_H
#define CONFIRMVEW_H
#include "../hw.h"
#include "../app.h"
#include "view.h"
#include <functional>

class ConfirmView : public View {
private:

	char _prompt[MAX_STRING_LENGTH];

	View* _lastView;
	std::function<void(bool)> _callback;
	//void (*_methodIfNo)();
public:

	ConfirmView(View* lastView, const char* prompt, std::function<void(bool)>callback)  {
		_lastView = lastView;
		snprintf(_prompt, MAX_STRING_LENGTH, "%s", prompt);
		_callback = callback;

	}

	void loop() {
		if (hw::goButton.wasPressed()) {
			App::setView(_lastView);
			_callback(true);
			delete this;
		}

		if (hw::stopButton.wasPressed()) {
			App::setView(_lastView);
			_callback(false);
			delete this;
		}
	}

	void draw() {
		hw::tft.fillRect(40, 30, 240, 70, 0xFFFF);
		hw::tft.setTextColor(0x0000, 0xFFFF);
		hw::tft.setTextFont(4);
		hw::tft.setCursor(VIEW_WIDTH/2 - hw::tft.textWidth(_prompt) / 2, 40);		// Centered text
		hw::tft.print(_prompt);

	}

	virtual ~ConfirmView() { }
};


#endif