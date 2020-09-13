#pragma once

#include <ESP8266WiFi.h>
#include "../hw.h"

namespace hw {
	class Network {
	private:
		static WiFiClient client;
		static const unsigned long RETRY_INTERVAL = 5000;

		static char NETWORK_SSID[MAX_STRING_LENGTH];
		static char NETWORK_PASS[MAX_STRING_LENGTH];
		static char HOSTNAME[MAX_STRING_LENGTH];
		
		static char	SERVER_HOST[MAX_STRING_LENGTH];
		static int       SERVER_PORT;

		static size_t networkCount;
		static uint8_t* wifiBitmap;

		static void sortNetworksSSID();
		static void sortNetworksRSSI();

		static char temp[MAX_STRING_LENGTH];

		static void keepAlive();
		static void onServerConnect();
		static void onServerDisconnect();

	public:
		class wifiNetwork {
		public:
			wifiNetwork() { }
			char ssid[MAX_STRING_LENGTH];
			int rssi;
			bool encrypted;
		};

		static wifiNetwork* networks;

		Network() { }
		static void begin();
		static void loop();
		static void checkWifiConnection();
		static void checkServerConnection();

		static int scanWiFi();
		static bool isConnectedToServer() { return client.connected(); }
		static bool isConnectedToWiFi() { return (WiFi.status()==WL_CONNECTED); }

		
		static void setPassword(const char* password);
		static void setSSID(const char* network);

		static void sendMessage(const char* msg);

		static void reconnect() { WiFi.reconnect(); }

		static const char* getPassword() { return NETWORK_PASS; }
		static const char* getSSID() { return NETWORK_SSID; }
		static const char* getServer() {
			snprintf(temp, MAX_STRING_LENGTH, "%s:%d", SERVER_HOST, SERVER_PORT);
			return temp;
		}
	};
}