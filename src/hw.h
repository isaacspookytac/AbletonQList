#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#include "hw/mytft.h"
#include "hw/myeeprom.h"
#include "hw/button.h"
#include "hw/logwindow.h"

namespace hw {
	extern Button goButton;
	extern Button stopButton;
	extern Button upButton;
	extern Button downButton;
	extern Button funButton;
	
	extern MyTFT tft;
	extern MyEEPROM eeprom;
	extern LogWindow log;
}

constexpr size_t MAX_STRING_LENGTH=64;


#endif
