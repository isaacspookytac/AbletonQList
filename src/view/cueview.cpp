
#include "../app.h"
#include "../hw.h"
#include "menuview.h"
#include "../viewfactory.h"
#include "../hw/network.h"
#include "colors.h"

CueView::CueView(View* lastView) {
	_palette = new Palette;
	//setTitle("Insert name of cue list here");//, this->TITLE_FG, this->TITLE_BG);
}

CueView::~CueView() {
	if (_cues != nullptr) { delete _cues; }
	if (_palette != nullptr) { delete _palette; }
}

void CueView::loop() {
	// do shifted functions first
	//if (hw::funButton.isDown()) {
		//if (hw::upButton.wasPressedWithRepeat())       prev();
		//if (hw::downButton.wasPressedWithRepeat())     next();
		
	//} else {
	if (_numCues) {
		if (hw::upButton.wasPressedWithRepeat()) {
			up();
			//hw::Network::sendMessage("!UP\n");
			char temp[16];
			snprintf(temp, 16, "!UPDATE:%d\n", _currentCue);
			hw::Network::sendMessage(temp);
		}
		if (hw::downButton.wasPressedWithRepeat()) {
			down();

			char temp[16];
			snprintf(temp, 16, "!UPDATE:%d\n", _currentCue);
			hw::Network::sendMessage(temp);
		}

	// }
		
		if (hw::goButton.wasPressed())                 go();
		if (hw::stopButton.wasPressed())               stop();
	}

	if (hw::funButton.wasPressed())                toMenu();
}

void CueView::up() {
	if (_currentCue > 0) {
		_currentCue--;// else _currentCue = _numCues-1;   // wraparound
		redraw();
	}
}


void CueView::down() {
	if (_currentCue < int(_numCues-1)) {
		_currentCue++;// else _currentCue = 0;    // wraparound
		redraw();
	}
}

void CueView::prev() {
	for (size_t Q = _currentCue-1; Q > 0; Q--) {
		if (_cues[Q].type == Cue::TYPE_PARENT) {
			_currentCue = Q;
			redraw();
			return;
		}
	}
	_currentCue = 0;
	redraw();
}

void CueView::next() {
	for (size_t Q = _currentCue+1; Q < _numCues; Q++) {
		if (_cues[Q].type == Cue::TYPE_PARENT) {
			_currentCue = Q;
			redraw();
			return;
		}
	}
	_currentCue = -1;
	redraw();
}

void CueView::toMenu() {

	App::setView(ViewFactory::getMenuView( this  ));

}

void CueView::lostConnection() {
	_currentCue = -1;
	setListSize(0);
	App::setView(ViewFactory::getMenuView(this));
}

void CueView::go() {
	
	if (_cues[_currentCue].type != Cue::TYPE_IGNORE) {
		char temp[16];
		snprintf(temp, 16, "!GO:%d\n", _currentCue);
		hw::Network::sendMessage(temp);
		setPlaying(true);
	}
	next();
}

void CueView::setPlaying(bool state) {

	_playing = state;

	if (_playing) 	drawCue(_currentCue - getTopCue(), _cues[_currentCue], _palette->FG_PLAYHEAD, _palette->BG_PLAYHEAD_PLAY);
	else			drawCue(_currentCue - getTopCue(), _cues[_currentCue], _palette->FG_PLAYHEAD, _palette->BG_PLAYHEAD);
	
} 

void CueView::stop() {
	//_currentCue = 0;
	hw::Network::sendMessage("!STOP\n");
	
	drawCue(_currentCue - getTopCue(), _cues[_currentCue], _palette->FG_PLAYHEAD, _palette->BG_PLAYHEAD_STOP);
	delay(100);
	setPlaying(false);

}

void CueView::draw() {
	clear();
	App::setTitle("Cue list name", _palette->TITLE_FG, _palette->TITLE_BG);
	redraw();
}

void CueView::setListSize(size_t n) {
	if (_cues != nullptr) { delete _cues; }
	_numCues = n;
	_cues = new Cue[_numCues];
}

void CueView::setCue(size_t Q, Cue cue) {
	//assert(Q < _numCues)
	_cues[Q] = cue;
	if (_cues[Q].color==Colors::NONE) _cues[Q].color=_palette->BG_NORMAL;
	if (Q % 2) _cues[Q].color= Colors::mixColors(_cues[Q].color, _palette->BG_CHEQUER, 0.5);

}

int CueView::getTopCue() {
	int start = _currentCue-CUES_BEFORE;
	int end = start+CUES_TOTAL-1;
	
	if (end > (int)_numCues-1) {
		start = _numCues-CUES_TOTAL;
	}

	if (start < 0) start = 0;

	return start;
}

void CueView::redraw() {


	if (_numCues == 0) {
		drawCue(0, Cue(Cue::TYPE_PARENT, "ERR", "No connection.", 0x2945));
		
		return;
	}
	// These calculations allow scrolling to stop at the
	// top and bottom of the list

	
	size_t Q=getTopCue();
	
	for (size_t pos=0; pos<CUES_TOTAL; pos++) {
		
		if (Q > _numCues-1) {
			// draw a blank cue (this wipes if we get to end of list)
			drawCue(pos, Cue(Cue::TYPE_PARENT, "", "", 0x00));
			Q++;
			continue;
		}

		if (int(Q) == _currentCue) {
			if (!_playing) drawCue(pos, _cues[Q], _palette->FG_PLAYHEAD, _palette->BG_PLAYHEAD);
					else   drawCue(pos, _cues[Q], _palette->FG_PLAYHEAD, _palette->BG_PLAYHEAD_PLAY);
		} else {  
			drawCue(pos, _cues[Q]);
		}
		Q++;
	}

	
}

void CueView::drawCue(size_t pos, Cue Q, uint16_t fg, uint16_t bg) {
	
	if (App::getView() != this) { return; }	// double check we have the focus

	if (fg==0 && bg==0) {
		// default values, do default colours
		fg = _palette->FG_NORMAL; bg = Q.color;
	
	}

	if (Q.type == Cue::TYPE_IGNORE) {
		// dull that puppy by 45%
		fg = Colors::mixColors(fg, 0, 0.45);
		bg = Colors::mixColors(bg, 0, 0.45);
		
	}

	hw::tft.setTextColor(fg, bg);
	
	hw::tft.setTextFont(FONT_QNAMES);
	hw::tft.setCursor(VIEW_X, VIEW_Y+pos*FONT_HEIGHT+ROW_PADDING);
	hw::tft.print( (Q.type == Cue::TYPE_CHILD) ? "         " : "     ");             // margin, wipes old content
	hw::tft.print(Q.name);
	hw::tft.print("                                                            "); // wipes old content
	
	hw::tft.setTextFont(FONT_QNUMS);
	hw::tft.setCursor(VIEW_X+MARGIN - hw::tft.textWidth(Q.prefix),
								VIEW_Y+pos*FONT_HEIGHT+ROW_PADDING+7);
	hw::tft.print(Q.prefix);

	hw::tft.fillRect(VIEW_X, VIEW_Y+pos*FONT_HEIGHT+1, VIEW_WIDTH, ROW_PADDING-1, bg);

}

void CueView::setCurrentCue(size_t Q) {
	if (int(Q) == _currentCue) return;		// do nothing
	if (Q < _numCues) {
		_currentCue = Q;
		if (App::getView() == this) { redraw(); }
	} else {
		hw::log.printf("Cuelist: new Q index (%u) is out of range. (max %u)\n", Q, _numCues-1);
	}
}