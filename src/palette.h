#ifndef PALETTE_H
#define PALETTE_H
#include <Arduino.h>

class Palette {
public:
		uint16_t TITLE_BG;
		uint16_t TITLE_FG;
		uint16_t BG_NORMAL;
		uint16_t FG_NORMAL;
		uint16_t FG_IGNORE;
		uint16_t BG_PLAYHEAD;
		uint16_t BG_PLAYHEAD_PLAY;
		uint16_t BG_PLAYHEAD_STOP;
		uint16_t FG_PLAYHEAD;
		uint16_t BG_CHEQUER;

		Palette() {
			TITLE_BG = 0x1086;
			TITLE_FG = 0xFFFF;
			BG_NORMAL=0x2945;
			FG_NORMAL=0xC618;
			FG_IGNORE=0x5AEB;
			BG_PLAYHEAD=0x2335;
			BG_PLAYHEAD_PLAY = 0x3400;
			BG_PLAYHEAD_STOP = 0xF800;
			FG_PLAYHEAD=0xFFFF;
			BG_CHEQUER=0x10A2;
		}
 };

 

class MenuPalette : public Palette {
public:
		MenuPalette() { 
					 
			BG_NORMAL=0x0000;
			BG_PLAYHEAD=0xD420;
			FG_PLAYHEAD=0x0000;
		}

};
#endif
