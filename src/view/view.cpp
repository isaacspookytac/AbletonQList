
#include "../app.h"
#include "view.h"
#include "../hw.h"

View::View(View* lastView) {
	_lastView = lastView;
}


void View::clear() {
	hw::tft.fillRect(VIEW_X, VIEW_Y, VIEW_WIDTH, VIEW_HEIGHT, 0x0000);

}

