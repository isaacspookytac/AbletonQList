#ifndef CUELIST_H
#define CUELIST_H

#include <Arduino.h>
#include "../palette.h"
#include "view.h"



class Cue {
public:
	static constexpr size_t Q_PREFIX_MAX_LENGTH = 6;
	static constexpr size_t Q_NAME_MAX_LENGTH = 32;

	enum QType { TYPE_PARENT, TYPE_CHILD, TYPE_IGNORE } type;
	char prefix[Q_PREFIX_MAX_LENGTH];
	char name[Q_NAME_MAX_LENGTH];
	int color;

	
	Cue() { }

	Cue(QType t, const char* p, const char* n, int c) {
		type = t;
		snprintf(prefix, Q_PREFIX_MAX_LENGTH, "%s", p);
		snprintf(name, Q_NAME_MAX_LENGTH, "%s", n);
		color = c;
	}
	//Cue(const Cue& Q) { Cue(Q.type, Q.prefix, Q.name, Q.color); }
	Cue operator=( const Cue& Q) {
		type = Q.type;
		snprintf(prefix, Q_PREFIX_MAX_LENGTH, "%s", Q.prefix);
		snprintf(name, Q_NAME_MAX_LENGTH, "%s", Q.name);
		color = Q.color;
		return *this;
	}
	

	

};

class CueView : public View {
	


protected:



	static const byte ROW_PADDING = 3;
	static const byte MARGIN = 23;
	
	static const int FONT_QNAMES = 4;
	static const int FONT_QNUMS = 1;
	static const int FONT_HEIGHT = 29;    //hw::tft.fontHeight(FONT_QNAMES) + ROW_PADDING;
	
	static const int CUES_BEFORE = 2;
	static const int CUES_TOTAL = 6;
 
	int _currentCue = 0;
	size_t _numCues = 0;
	View* _parent = 0;

	bool _playing = false;

	Palette* _palette;
	Cue* _cues = nullptr;

	int getTopCue();
	virtual void redraw();
	virtual void up();
	virtual void down();
	virtual void prev();
	virtual void next();
	virtual void go();
	virtual void stop();
	virtual void toMenu();
	virtual void drawCue(size_t pos, Cue Q, uint16_t fg=0, uint16_t bg=0);

public:

	CueView(View* lastView);
	CueView() { }	
	~CueView();

	size_t getListSize() { return _numCues; }
	void setListSize(size_t n);
	void setCue(size_t Q, Cue cue);
	void setCurrentCue(size_t Q);
	void setPlaying(bool state);

	void lostConnection();

	virtual void loop();
	virtual void draw();

};

#endif
