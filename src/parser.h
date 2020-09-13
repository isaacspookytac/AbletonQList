#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>
#include "viewfactory.h"
#include "hw.h"
#include "colors.h"

namespace Parser {
	void readCueList(const char*);
	void readCue(const char*);
	void readUpdate(const char*);
	void readPlaying(const char*);
	void readStopped(const char*);

	void parse(const char* line) {
	
		if (strncmp(line, "@CUELIST", 8) == 0) { 
			readCueList(line);  
		} else if (strncmp(line, "@CUE", 4) == 0) {
			readCue(line);
		} else if (strncmp(line, "@UPDATE:", 8) == 0) {
			readUpdate(line);
		} else if (strncmp(line, "@PLAYING", 8) ==0) {
			readPlaying(line);
		} else if (strncmp(line, "@STOPPED", 8) ==0) {
			readStopped(line);
		}

	// add more requests as necessary
	
	}

	void readPlaying(const char* input) {
		ViewFactory::getCueView()->setPlaying(true);
	}
	void readStopped(const char* input) {
		ViewFactory::getCueView()->setPlaying(false);
	}


	void readUpdate(const char* input) {
		size_t arg;

		if (sscanf(input, "@UPDATE:%u\n", &arg) != 1) {
			hw::log.printf("Parser: malformed update.\n");
			return;
		}

		//hw::log.print("Update: "); hw::log.println(arg);
		ViewFactory::getCueView()->setCurrentCue(arg);
	}

	void readCue(const char* input) {
		size_t Q;
		char t[64];
		char prefix[8];
		char name[64];
		int c;
		

		int result = sscanf(input, "@CUE:%u`%[^\n`]`%[^\n`]`%[^\n`]`%d", &Q, t, prefix, name, &c);
		if (result != 5) {
			hw::log.printf("Parser: expected 5 arguments, got %d for Q #%u.\n", result, Q);
			return;
		}

		if (ViewFactory::getCueView()->getListSize()==0) {
			hw::log.printf("Parser: need cuelist size information first.\n");
			return;
		} else if (Q > ViewFactory::getCueView()->getListSize()-1) {
			hw::log.printf("Parser: Q #%u is out of range (max %u).\n", Q, ViewFactory::getCueView()->getListSize());
		}

		//hw::log.printf("%u/%s/%s/%s/%x\n", number, type, prefix, name, c);
		Cue::QType type;
			 if (strcmp(t, "child")==0) type = Cue::TYPE_CHILD;
		else if (strcmp(t, "parent")==0) type = Cue::TYPE_PARENT;
		else if (strcmp(t, "ignore")==0) type = Cue::TYPE_IGNORE;
		else { 
			hw::log.printf("Parser: invalid type '%s' for Q #%u.\n", t, Q);
			type = Cue::TYPE_PARENT;
		}

		uint16_t color;
			 if (c==0)	color = Colors::NONE;
		else if (c==1)	color = Colors::RED;
		else if (c==2)	color = Colors::ORANGE;
		else if (c==3)	color = Colors::GREEN;
		else if (c==4)	color = Colors::BLUE;
		else if (c==5)	color = Colors::PURPLE;
		else {
			hw::log.printf("Parser: invalid color %d for Q #%u.\n", c, Q);
			color = 0xF00F;
		}

		ViewFactory::getCueView()->setCue(Q, Cue(type, prefix, name, color));
		
	}


	void readCueList(const char* input) {
		int arg;

		if (sscanf(input, "@CUELIST:%d\n", &arg) != 1) {
			hw::log.println("Parser: malformed cue count.\n");
			return;
		}

		hw::log.print("Cue list size: "); hw::log.println(arg);
		ViewFactory::getCueView()->setListSize(arg);
	}
}

#endif