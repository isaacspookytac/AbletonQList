#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
private:
	static const unsigned long DEBOUNCE_TIME = 150;
	static const unsigned long REPEAT_WAIT = 300;
	static const unsigned long REPEAT_INTERVAL = 75;
	
	bool _lastReading = false;
	unsigned long _lastDownTime = 0;
	unsigned long _lastPressedTime = 0;
	unsigned long _lastRepeatTime = 0;
	
	byte _pin;

	bool readWithDebounce() {
		bool reading = false;
		reading = !(digitalRead(_pin));
		
		// first, lock the reading to TRUE for for a period of time. we need to check _lastDownTime isn't 0
		if ((millis() - _lastDownTime) < DEBOUNCE_TIME && (_lastDownTime != 0)) return true;
		if (reading == true) _lastDownTime = millis();
		
		return reading;
	}
	
public:

	Button(byte p) {
		_pin = p;
		pinMode(p, INPUT_PULLUP);
		
	}

	bool isDown() {
		return readWithDebounce();
	}

	bool wasReleased() {
		bool result = false;
		bool reading = readWithDebounce();
		
		
		only return true if we're moving from unpressed to pressed
		if (reading == false && _lastReading == true) result = true;
		else result = false;
		
		_lastReading = reading;
		return result;
	}

	bool wasPressed() {
		bool result = false;
		bool reading = readWithDebounce();
		
		
		only return true if we're moving from unpressed to pressed
		if (reading == true && _lastReading == false) result = true;
		else result = false;
		
		_lastReading = reading;
		return result;
	}
	
	bool wasPressedWithRepeat() {
		bool result = false;
		bool reading = readWithDebounce();
		

		unsigned long currentTime = millis();
		
		//only return true if we're moving from unpressed to pressed
		if (reading == true) {
			
			if (_lastReading == false) {
				// if reading is true and we had a false reading before, then the button 
				// has just been pushed. save this time
				result = true;
				_lastPressedTime = currentTime;
				_lastRepeatTime = 0;
 
			} else if ( (currentTime - _lastPressedTime) > REPEAT_WAIT && (currentTime - _lastRepeatTime) > REPEAT_INTERVAL) {
				// if we had a true reading before (lastReading == true), and enough time
				// has elapsed to start repeating (lastChange), and enough time has elapsed
				// to send another true (lastRepeat) then we shall
				result = true;
				_lastRepeatTime = currentTime;
			}
			
		} else {
			// either the button is down but not enough time has passed to send another true,
			// or the button is not down
			result = false;
		}
		
		_lastReading = reading;
		return result;
	}
	
};

#endif
