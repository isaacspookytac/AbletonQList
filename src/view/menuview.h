#ifndef MENUCUELIST_H
#define MENUCUELIST_H

#include "../app.h"
#include "cueview.h"
#include "../hw.h"



class MenuView : public CueView {
protected:
	MenuView() { }			// only for derived classes
public:
	~MenuView();
	MenuView(View* lastView);

	virtual void quit();
	virtual void go();
	virtual void loop();
	void update();
	void draw();

};

#endif
