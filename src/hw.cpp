#include "hw.h"

namespace Colors {

}

namespace hw {

	Button goButton = Button(D1);
	Button stopButton = Button(D2);
	Button upButton = Button(D3);
	Button downButton = Button(D4);
	Button funButton = Button(10);
	
	MyTFT tft;
	MyEEPROM eeprom;
	LogWindow log(0, 200, 320, 40);

}

