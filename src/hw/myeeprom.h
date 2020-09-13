#ifndef MYEEPROM_H
#define MYEEPROM_H

#include <EEPROM.h>

class MyEEPROM : EEPROMClass {
public:


	enum location {
		NETWORK_SSID = 0,
		NETWORK_PASS = 64,
		SERVER_IP = 128,
		SERVER_PORT = 192,
		START = 0
	};

	static constexpr size_t EEPROM_SIZE = 1024;

	MyEEPROM() {
		EEPROM.begin(EEPROM_SIZE);
	}

	~MyEEPROM() {
		EEPROM.end();
	}

	void writeString(location position, const char* string, size_t MAX_LENGTH=0) {
		//assert(position < EEPROM_SIZE);
		if (MAX_LENGTH == 0) MAX_LENGTH = strlen(string);
		size_t i = 0;

		while (true) {
			EEPROM.write(position+i, string[i]);
			if (i == MAX_LENGTH-1 || i == EEPROM_SIZE-1) {
				EEPROM.write(position+i, '\0');		// terminate this properly
				break;
			} else if (string[i] == '\0') {
				break;
			}
			i++;
		}

		EEPROM.commit();

	}

	void readString(location position, char* destination, size_t MAX_LENGTH) {
		//assert(position < EEPROM_SIZE);
		size_t i = 0;

		while (true) {
			destination[i] = EEPROM.read(position+i);
			if (i == MAX_LENGTH-1 || i == EEPROM_SIZE-1) {
				destination[i] = '\0';
				break;
			} else if (destination[i] == '\0') {
				break;
			}
			i++;
		}

	}

	

};


#endif