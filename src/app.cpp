
#include "app.h"

#include "hw.h"
#include "view/view.h"
#include "view/cueview.h"
#include "view/menuview.h"
#include "viewfactory.h"
#include "hw/network.h"
/*
View* App::viewStack[App::MAX_VIEWS];
int App::view = -1;
*/
View* App::view = nullptr;

const char* App::APP_NAME = "Portable Ableton Controller";
const char* App::APP_VER = "v0.9";


void App::setTitle(const char* title, uint16_t fg, uint16_t bg) {

	hw::tft.fillRect(0, 0, hw::tft.getWidth(), 16, bg);
	hw::tft.setTextFont(2);
	hw::tft.setTextColor(fg, bg);
	hw::tft.setCursor(0,0);
	hw::tft.println(title);
}


void App::initialize() {
	//static App* _instance = new App();
	hw::log.print(App::APP_NAME); hw::log.print(" "); hw::log.println(App::APP_VER);
	hw::tft.print(App::APP_NAME);
	hw::tft.print(" ");
	hw::tft.println(App::APP_VER);
	hw::Network::begin();
	setView(ViewFactory::getMenuView(ViewFactory::getCueView()));
	//setView(ViewFactory::getMenuView(nullptr));//ViewFactory::getCueView()));
}


View* App::getView() {
	if (view != nullptr) return view;
	else return ViewFactory::getNullView();
	
}

void App::setView(View* newView) {
	if (newView != nullptr) {
		view = newView;
		view->draw();
	} else {
		view = nullptr;
		ViewFactory::getNullView()->draw();
	}
}

