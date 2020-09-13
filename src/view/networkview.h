#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H
#include "menuview.h"


class NetworkView : public MenuView {
private:
	void callbackFromCustom(const char* network);
public:
	NetworkView(View* lastView);
	void quit();
	void draw();
	void loadNetworks();
	void go();
	
	virtual ~NetworkView() { }
};


#endif