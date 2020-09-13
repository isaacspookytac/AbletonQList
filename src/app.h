#ifndef APP_H
#define APP_H
#include <Arduino.h>

class View;

class App {
private:
	App();
	~App();
public:

	static const char* APP_NAME;
	static const char* APP_VER;


	static View* view;
	static View* getView();
	static void setView(View* newView);
	static void setTitle(const char* title, uint16_t fg, uint16_t bg);
	static void initialize();

};

#endif