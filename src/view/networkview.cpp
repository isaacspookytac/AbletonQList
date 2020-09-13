#include "view/networkview.h"
#include "hw/network.h"

#include "palette.h"
#include "hw.h"
#include "colors.h"
#include "viewfactory.h"

NetworkView::NetworkView(View* lastView) {
	_lastView = lastView;

	_numCues = 1;
	_cues = new Cue[_numCues];
	_palette = new MenuPalette;
	
	_currentCue = 0;
	_palette->BG_PLAYHEAD = 0xF940;
	_palette->FG_PLAYHEAD = 0xFFFF;
}

void NetworkView::draw() {
	clear();
	App::setTitle("Network selection", _palette->TITLE_FG, 0x7000);
	drawCue(0, Cue(Cue::TYPE_PARENT, "", "Scanning networks...", Colors::NONE));
	loadNetworks();
	redraw();
}

void NetworkView::loadNetworks() {
	int count = hw::Network::scanWiFi();

	setListSize(count+1);
	setCue(0, Cue(Cue::TYPE_PARENT, "", "Custom...", Colors::RED));

	for (int i = 0; i < count; i++) {
		char temp[8];
		snprintf(temp, 8, "%d%c", hw::Network::networks[i].rssi, (hw::Network::networks[i].encrypted ? '*' : ' '));

		setCue(i+1,
		Cue(Cue::TYPE_PARENT,
		temp,
		hw::Network::networks[i].ssid,
		Colors::NONE));
	}


}

void NetworkView::go() {
	if (_currentCue == 0) {
		App::setView(ViewFactory::getInputView(_lastView, "Enter network SSID:", hw::Network::getSSID(), &hw::Network::setSSID));
	} else {
		hw::Network::setSSID(hw::Network::networks[_currentCue-1].ssid);

		if (hw::Network::networks[_currentCue].encrypted) {
			// input the password with a callback to set the password & exit this view
			App::setView(
			ViewFactory::getInputView(_lastView, "Enter network password:", hw::Network::getPassword(), &hw::Network::setPassword));
			
		} else {
			hw::Network::setPassword("");
			App::setView(_lastView);		// exit
		}
	}

	//delete this;
}

void NetworkView::callbackFromCustom(const char* network) {
		// input the password with a callback to set the password & exit this view
	hw::Network::setSSID(network);
	delay(1000);
	quit();
}

void NetworkView::quit() {
	// exit menu by reassigning the q list pointer
	//App::quitView();
	//hw::Network::reconnect();   
	App::setView(_lastView);
}