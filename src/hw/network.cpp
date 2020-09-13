
#include <ESP8266WiFi.h>
#include "app.h"
#include "hw.h"
#include "network.h"
#include "../parser.h"

namespace hw {
	
	WiFiClient	Network::client;
	char 		Network::NETWORK_SSID[MAX_STRING_LENGTH] = "!default network!";
	char 		Network::NETWORK_PASS[MAX_STRING_LENGTH] = "!default password!";
	char 		Network::HOSTNAME[MAX_STRING_LENGTH] = "Isaac's Arduino";  
	//IPAddress 	Network::SERVER_HOST(192, 168, 0, 20);
	char	 	Network::SERVER_HOST[MAX_STRING_LENGTH] = "192.168.0.20";
	int       	Network::SERVER_PORT = 2505;

	char		Network::temp[MAX_STRING_LENGTH] = "";

	Network::wifiNetwork* Network::networks;
	size_t		Network::networkCount;

	void Network::begin() {
		// load all the stuff from EEPROM
		hw::eeprom.readString(hw::eeprom.NETWORK_SSID, NETWORK_SSID, MAX_STRING_LENGTH);
		hw::eeprom.readString(hw::eeprom.NETWORK_PASS, NETWORK_PASS, MAX_STRING_LENGTH);
		WiFi.mode(WIFI_STA);
		WiFi.hostname(HOSTNAME);
		WiFi.setAutoReconnect(true);
		WiFi.begin(NETWORK_SSID, NETWORK_PASS);

	}

	void Network::setPassword(const char* password) {

		snprintf(NETWORK_PASS, MAX_STRING_LENGTH, "%s", password);
		hw::eeprom.writeString(hw::eeprom.NETWORK_PASS, NETWORK_PASS, MAX_STRING_LENGTH);
		WiFi.disconnect();
		WiFi.setAutoReconnect(true);
		WiFi.begin(NETWORK_SSID, NETWORK_PASS);

	}

	void Network::setSSID(const char* network) {
		snprintf(NETWORK_SSID, MAX_STRING_LENGTH, "%s", network);
		hw::eeprom.writeString(hw::eeprom.NETWORK_SSID, NETWORK_SSID, MAX_STRING_LENGTH);
		WiFi.disconnect();
		WiFi.setAutoReconnect(true);
		WiFi.begin(NETWORK_SSID, NETWORK_PASS);

	}

	void Network::loop() {

		checkWifiConnection();
		if (!isConnectedToWiFi()) return;

		checkServerConnection();
		if (!isConnectedToServer()) return;

		
		char line[1024];
		int n = 0;

		while (client.available()) {    // available() means there is data to be read

			char c = client.read();       // read a char
			line[n] = c;

			if (line[n] == '\n' || n>=1022) {
				line[n+1] = '\0';			 // terminate string
				hw::log.print("Rec: ");      // for debugging
				hw::log.print(line);        
				Parser::parse(line);
				n=0;
				break;
			}

			n++;

		}

		keepAlive();
		
	}

	void Network::checkWifiConnection() {

		static int lastStatus = 255;

		int status = WiFi.status();

		// check for change in status
		if (status != lastStatus) {
			switch(status) {
				case WL_CONNECTED:
					hw::log.print("Wifi: Connected to '"); hw::log.print(Network::getSSID()); hw::log.println("'!");
					break;
				case WL_IDLE_STATUS:
					hw::log.println("Wifi: Idle.");
					WiFi.reconnect();
					break;
				case WL_SCAN_COMPLETED:
					hw::log.println("Wifi: Scan completed.");
					WiFi.reconnect();
					break;
				case WL_NO_SSID_AVAIL:
					hw::log.println("Wifi: No SSID available.");
					WiFi.reconnect();
					break;
				case WL_CONNECT_FAILED:
					hw::log.println("Wifi: Connection failed.");
					WiFi.reconnect();
					break;
				case WL_CONNECTION_LOST:
					hw::log.println("Wifi: Connection lost.");
					WiFi.reconnect();
					break;
				case WL_DISCONNECTED:
					hw::log.println("Wifi: Connecting..."); break;
			}

			if (lastStatus == WL_CONNECTED) {
				//we've been disconnected damn
				onServerDisconnect();
			}
		}

		


		lastStatus = status;

	}
			
	void Network::checkServerConnection() {
		static unsigned long lastAttemptTime = -RETRY_INTERVAL;
		static unsigned int attempt = 0;
		static bool lastStatus = false;

		int status = client.connected();
		
		if (status==true) {
			if (lastStatus==true) {
				return;
			} else {
				hw::log.print("Server: Connected to "); hw::log.print(Network::getServer()); hw::log.println("!");
				lastStatus=true;
				attempt=0;
				Network::onServerConnect();
				return;
			}
		}
		if (lastStatus == true && status == false) {
			onServerDisconnect();
		}
		lastStatus = false;
			// if enough time has elapsed, attempt again.
		if ((millis() - lastAttemptTime) >= RETRY_INTERVAL) {
			attempt++;
			hw::log.print("Server: attempt #"); hw::log.print(attempt); hw::log.println(" to connect...");
			client.connect(SERVER_HOST, SERVER_PORT);
			//client.connect("192.168.0.20", 2505);

			lastAttemptTime = millis();
		}
		
	}

	void Network::onServerConnect() {
		sendMessage("?CUELIST\n");
		App::setView(ViewFactory::getCueView());
	}

	void Network::onServerDisconnect() {
		hw::log.println("Server: disconnected.");
		ViewFactory::getCueView()->lostConnection();
	}

	void Network::keepAlive() {
		// try built in TCP keep alive?
		static unsigned long lastTime=0;
		if (millis() - lastTime > 3000) {
			sendMessage("*\n");
			lastTime = millis();
		}
	}

	int Network::scanWiFi() {


		//WiFi.mode(WIFI_STA);
		//WiFi.disconnect();
		//delay(100);

		networkCount =  WiFi.scanNetworks();	// this could be done asynchronously, but might be too much effort

		// static struct to hold networks in
		if (networks) { delete networks; }
		networks = new wifiNetwork[networkCount];


		for (size_t n = 0; n < networkCount; n++) {
			snprintf(networks[n].ssid, MAX_STRING_LENGTH, "%s", WiFi.SSID(n).c_str());
			networks[n].rssi = WiFi.RSSI(n);
			networks[n].encrypted = (WiFi.encryptionType(n) == ENC_TYPE_NONE) ? false : true;
		}

		sortNetworksSSID();

		return networkCount;

	}

	void Network::sortNetworksSSID() {
		//bubble sort

		
		bool changed = false;
		do {
			// keep looping through the struct, reordering until nothing gets changed
			changed = false;
			for (size_t i = 0; i < networkCount-1; i++) {

				if (strcasecmp((networks[i].ssid), (networks[i+1].ssid)) > 0) {
					
					wifiNetwork temp = networks[i];
					networks[i] = networks[i+1];
					networks[i+1] = temp;
					changed = true;

				}
			}
		} while (changed);

		// yay done
	}

	void Network::sortNetworksRSSI() {
		//bubble sort

		
		bool changed = false;
		do {
			// keep looping through the struct, reordering until nothing gets changed
			changed = false;
			for (size_t i = 0; i < networkCount-1; i++) {
				if (networks[i].rssi > networks[i+1].rssi) {

					wifiNetwork temp = networks[i];
					networks[i] = networks[i+1];
					networks[i+1] = temp;
					changed = true;

				}
			}

		} while (changed);

		// yay done
	}

	void Network::sendMessage(const char* message) {
		if (isConnectedToServer()) client.write(message);
		else hw::log.printf("Network: server not connected, can't send message '%s'.", message);
	}

}