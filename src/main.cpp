
// REMOTE DISPLAY
#include <Arduino.h>

#include "app.h"
#include "view/view.h"
#include "hw/network.h"


void setup() {

  App::initialize();

}
void loop() {

  // save these so we can check for combinations
	hw::Network::loop();
	App::getView()->loop();
	
}
