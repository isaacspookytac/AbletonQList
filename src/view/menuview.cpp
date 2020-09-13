#include "menuview.h"
#include "hw/network.h"
#include "../viewfactory.h"
#include "colors.h"

void MenuView::update() {
	char item[MAX_STRING_LENGTH];
	setListSize(5);
	
	snprintf(item, 10, "Steve!");
	snprintf(item, MAX_STRING_LENGTH, "Network: %s", hw::Network::getSSID());
	setCue(0, Cue(Cue::TYPE_PARENT, "", item, Colors::NONE));

	
	snprintf(item, MAX_STRING_LENGTH, "Password: %s", hw::Network::getPassword());
	setCue(1, Cue(Cue::TYPE_PARENT, "", item, Colors::NONE));

	snprintf(item, MAX_STRING_LENGTH, "Server: %s", hw::Network::getServer());
	setCue(2, Cue(Cue::TYPE_PARENT, "", item, Colors::NONE));

	setCue(3, Cue(Cue::TYPE_PARENT, "", "Load fake cue list", Colors::NONE));
	setCue(4, Cue(Cue::TYPE_PARENT, "", "Delete cue list", Colors::NONE));
/*	setCue(0, Cue(Cue::TYPE_PARENT, "", "Select WiFi SSID...", Colors::NONE));
	setCue(1, Cue(Cue::TYPE_PARENT, "", "Set WiFi password...", Colors::NONE));
	setCue(2, Cue(Cue::TYPE_PARENT, "", "Set server IP address...", Colors::NONE));
	setCue(3, Cue(Cue::TYPE_PARENT, "", "Set server port...", Colors::NONE)); */	
	
	
}

MenuView::MenuView(View* lastView) {
	_lastView = lastView;
	_palette = new MenuPalette;

	TITLE_BG = 0x81E1;
	TITLE_FG = 0xFFFF;
	//App::setTitle("MENUVIEW WAS MADE", _palette->TITLE_FG, _palette->TITLE_BG);
	update();

}

MenuView::~MenuView() {
	delete _cues;
	delete _palette;
}

void MenuView::draw() {
	update();
	clear();
	App::setTitle("Settings", TITLE_FG, TITLE_BG);
	redraw();
}


void MenuView::quit() {
	// exit menu by reassigning the q list pointer
	//App::quitView();   
	App::setView(_lastView);
}

void foo(const char* value) {
	hw::log.println("Foo!");
}

void MenuView::go() {
	CueView* cueList = (ViewFactory::getCueView(nullptr));

	drawCue(_currentCue - getTopCue(), _cues[_currentCue], 0x00FF, 0x0000);

	switch (_currentCue) {
		case 0:
			App::setView(ViewFactory::getNetworkView(this));
			break;
		case 1:
			// network password
			App::setView(
			ViewFactory::getInputView(this, "Enter network password:", hw::Network::getPassword(), std::bind(&hw::Network::setPassword, std::placeholders::_1))
				);
			break;
		case 2:
			App::setView(
			ViewFactory::getIPView(this, "Enter server IP:", hw::Network::getPassword(), std::bind(&hw::Network::setPassword, std::placeholders::_1))
				);
			break;
		case 3:
			// load fake cue list

			cueList->setListSize(10);
			cueList->setCue(0, Cue(Cue::TYPE_PARENT, "0", "This is a great cue", Colors::NONE));
			cueList->setCue(1, Cue(Cue::TYPE_PARENT, "1", "Indeed I agree", Colors::NONE));
			cueList->setCue(2, Cue(Cue::TYPE_PARENT, "2", "But tell me more", Colors::NONE));
			cueList->setCue(3, Cue(Cue::TYPE_PARENT, "3", "Yes please do", Colors::NONE));
			cueList->setCue(4, Cue(Cue::TYPE_CHILD, "", "Do you say?", Colors::NONE));
			cueList->setCue(5, Cue(Cue::TYPE_PARENT, "4", "Okay perhaps I will", Colors::NONE));
			cueList->setCue(6, Cue(Cue::TYPE_PARENT, "4.5", "More stuff", Colors::NONE));
			cueList->setCue(7, Cue(Cue::TYPE_CHILD, "4.6", "We love the cues", Colors::NONE));
			cueList->setCue(8, Cue(Cue::TYPE_CHILD, "4.7", "Yes we do", Colors::NONE));
			cueList->setCue(9, Cue(Cue::TYPE_PARENT, "5", "Last cue", Colors::NONE));

			App::setView(cueList);
			break;
		case 4:
			// delete cue list
			cueList->setListSize(0);
	}

}


void MenuView::loop() {

	if (hw::upButton.wasPressedWithRepeat())	up();
	if (hw::downButton.wasPressedWithRepeat())	down();
	if (hw::goButton.wasPressed())				go();
	if (hw::stopButton.wasPressed())			quit();
	if (hw::funButton.wasPressed())				quit();

}