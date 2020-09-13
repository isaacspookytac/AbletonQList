#ifndef VIEW_FACTORY_H
#define VIEW_FACTORY_H

#include "view/menuview.h"
#include "view/cueview.h"
#include "view/inputview.h"
#include "view/confirmview.h"
#include "view/networkview.h"
#include "view/ipview.h"


class ViewFactory { 
public:
	static MenuView* getMenuView(View* lastView) {
		static MenuView* instance = nullptr;
		if (instance == nullptr) instance = new MenuView(lastView);
		return instance;

	}

	static CueView* getCueView(View* lastView=nullptr) {
		static CueView* instance = nullptr;
		if (instance == nullptr) instance = new CueView(lastView);
		return instance;

	}

	static InputView* getInputView(View* lastView, const char* prompt, const char* defaultValue, std::function<void(const char*)> callback) {
		static InputView* instance = nullptr;
		if (instance != nullptr) { 
			delete instance;
		}
		instance = new InputView(lastView, prompt, defaultValue, callback);
		return instance;
		//return new InputView(lastView, prompt, callback);
	}

	static IPView* getIPView(View* lastView, const char* prompt, const char* defaultValue, std::function<void(const char*)> callback) {
		static IPView* instance = nullptr;
		if (instance != nullptr) { 
			delete instance;
		}
		instance = new IPView(lastView, prompt, defaultValue, callback);
		return instance;
	}

	static NetworkView* getNetworkView(View* lastView=nullptr) {
		static NetworkView* instance = nullptr;
		if (instance==nullptr) instance = new NetworkView(lastView);
		return instance;
	}

	static NullView* getNullView() {
		
		static NullView* instance = nullptr;
		if (instance==nullptr) instance = new NullView();
		return instance;
	}

	


};


#endif