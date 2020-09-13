//Abstract View class
#ifndef VIEW_H
#define VIEW_H
#include <Arduino.h>
#include "../hw.h"

class View {
protected:

	//View();
	View* _lastView;

	//static constexpr size_t MAX_TITLE_LENGTH = 30;
	char _title[MAX_STRING_LENGTH];


	static constexpr int VIEW_X = 0;
	static constexpr int VIEW_Y = 16;
	static constexpr int VIEW_WIDTH = 320;
	static constexpr int VIEW_HEIGHT = 200 - VIEW_Y;
	
	uint16_t TITLE_BG;
	uint16_t TITLE_FG;

public:

	View(View* lastView);
	View() { 			TITLE_BG = 0xFA00;
						TITLE_FG = 0xFFFF;
	}		

	virtual void loop()=0;
	virtual void draw()=0;
	virtual void clear();

};

class NullView : public View {
public:
	void loop() { delay(1); }
	void draw() {
		clear();
		App::setTitle("Null View", 0x0000, 0xFFFF);
		hw::tft.setTextColor(0xFFFF, 0x0000);
		hw::tft.setCursor(0, 90);
		hw::tft.println("Error: no view set.");
		hw::tft.println("This is the view you get when you have no view.");
	}

	//virtual void create()=0;
};



#endif
