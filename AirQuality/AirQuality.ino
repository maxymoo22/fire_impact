/*#include <WiFi.h>
#include <HTTPClient.h>

// howsmyssl.com root certificate authority
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
"-----END CERTIFICATE-----\n";

void setup() {
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	Serial.begin(9600);

	function();
}

void function() {
	Serial.println("Scanning for networks");
	int16_t networks = WiFi.scanNetworks(false, true);
	Serial.println("Scan complete\n\nNetworks within range:");

	int viableNetworks[networks] = {};
	int index = 0;
	
	for(int i = 0; i < networks; i++) {
		if(WiFi.RSSI(i) >= -70) {
			viableNetworks[index] = i;
			index++;
			Serial.println(String(i + 1) + ") " + WiFi.SSID(i) + ", " + (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secure"));
		}
	}

	label1:
	Serial.println("\nPlease type the corresponding number to choose the network:");
	while(Serial.available() <= 0) {}
	int choice = Serial.parseInt();

	// parseInt returns 0 when it times out so thats ultra handy!!! It will be caught by this statement id the user enters a non int
	if(choice < 1 or choice > index) {
		Serial.println("\nThat wasn't a valid choice!");
		goto label1;
	}

	// Get the number of the network from our choice.
	choice = viableNetworks[choice - 1];

	if(WiFi.encryptionType(choice) != WIFI_AUTH_OPEN) {
		label2:
		Serial.println("\nThis network requires a password. Please enter it now:");
		while(Serial.available() <= 0) {}
		String password = Serial.readString();

		if(WiFi.begin(WiFi.SSID(choice).c_str(), password.c_str()) != WL_CONNECTED) {
			Serial.print("\nSomething went wrong. You probably entered an incorrect password. Please try again.");
			goto label2;
		}
	}
	else {
		label3:
		if(WiFi.begin(WiFi.SSID(choice).c_str()) != WL_CONNECTED) {
			Serial.println("\nSomething went wrong. Attempting to reconnect...");
			goto label3;
		}
	}

	Serial.println("\nSuccessfully connected to the network!\nSending request to howsmyssl.com\n");

	HTTPClient http;
 
		http.begin("https://www.howsmyssl.com/a/check", root_ca); //Specify the URL and certificate
		int httpCode = http.GET();
 
		if (httpCode > 0) { //Check for the returning code
				String payload = http.getString();
				Serial.println(httpCode);
				Serial.println(payload);
			}
 
		else {
			Serial.println("Error on HTTP request");
		}
 
		http.end(); //Free the resources
}

void loop() {}*/


#include <WiFi.h>
#include <WiFiUdp.h>

// The amount of clients that have connected
byte clientCount = 0;
// This holds all of the clients that have connected to the tcp server. At the moment I'll keep it at a maximum of 5
WiFiClient clients[5];

// Create udp instance
WiFiUDP udp;
// Create tcp server instance. We only want a maximum of 5 clients
WiFiServer TCPServer(1305, 5);

// We use this to determine if enough time has passed to update the sensor data
unsigned long startMillis = 0;

bool connectedToUserWifi = false;
bool incorrectWifiPassword = false;

void setup() {
	Serial.begin(9600);
	
	// Setup an access point. This will be connected to by users, who will in turn give us their wifi password
	//WiFi.mode(WIFI_AP_STA);
	WiFi.softAP("FireImpact-WiFi", "xp59vg7p");

	Serial.println("\nWifi access point setup");
	Serial.println(WiFi.localIP());

	createServers();
}

void loop() {
	WiFiClient incomingClient = TCPServer.available();
	// If we have an incoming client and they're sending us data, its the app on the users phone.
	if(incomingClient) {
		bool clientAlreadyConnected = false;
		for(uint8_t i = 0; i < clientCount; i++) {
			if(clients[i] == incomingClient) {
				clientAlreadyConnected = true;
				break;
			} 
		}
		
		// Store the client if we dont have them already
		if(!clientAlreadyConnected) {
			// Clear the input buffer from the client so its not data wating to be read
			uint8_t unusedBuffer[4];
			incomingClient.readBytes(unusedBuffer, 4);
			clients[clientCount] = incomingClient;

			Serial.println("Client connected. Client number: " + String(clientCount));
			clientCount++;
		}
	}

	// If a second has passed then we need to run updates for the tcp server
	if(millis() - startMillis >= 1000) {
		for(byte i = 0; i < clientCount; i++) {
			// If they aren't connected anymore then we can remove them and move on to the next client
			if(clients[i].connected() == false) {
				Serial.println("Client " + String(i) + " disconnected");
				clients[i].stop();
				removeClient(i);
				Serial.println("Client count: " + String(clientCount));
				continue;
			}

			int availableBytes = clients[i].available();

			// If the client is sendig us data then it will be the name ssid and password for their wifi network
			if(availableBytes > 0) {
				Serial.print("Received WiFi details.\nAvailable bytes: ");
				Serial.println(availableBytes);
				incorrectWifiPassword = false;

				// Read in password and name
				/*unsigned char nameBuffer[255];
				unsigned char passwordBuffer[255];
				clients[i].readBytesUntil(1, nameBuffer, 255);
				clients[i].readBytesUntil(1, passwordBuffer, 255);

				Serial.print("Password buffer: ");
				Serial.println((char*)passwordBuffer);
				Serial.print("Name buffer: ");
				Serial.println((char*)nameBuffer);

				for(int qwerty = 0; i < 256; i++) {
					Serial.println((int)nameBuffer[i]);
					Serial.println((int)passwordBuffer[i]);
					Serial.println("\n");
				}*/

				/*unsigned char* buffer = new unsigned char[availableBytes];
				clients[i].readBytes(buffer, availableBytes);
				
				char* nameBuffer;
				char* passwordBuffer;
				nameBuffer = strtok((char*)buffer, (char*)1);
				passwordBuffer = strtok(NULL, (char*)1);

				Serial.print("password buffer: ");
				Serial.println((char*)passwordBuffer);
				Serial.print("Name buffer: ");
				Serial.println((char*)nameBuffer);*/

				uint8_t nameBufLength = clients[i].read();
				uint8_t passwordBufLength = availableBytes - nameBufLength - 3;

				Serial.print("Name buffer length: ");
				Serial.println(nameBufLength);
				Serial.print("Password buffer length: ");
				Serial.println(passwordBufLength);

				char* nameBuffer = new char[nameBufLength];
				char* passwordBuffer = new char[passwordBufLength];
				Serial.println(clients[i].readBytesUntil(1, nameBuffer, nameBufLength));
				clients[i].read();
				Serial.println(clients[i].readBytesUntil(1, passwordBuffer, passwordBufLength));
				clients[i].read();

				Serial.print("Password buffer: ");
				Serial.println(passwordBuffer);
				Serial.print("Name buffer: ");
				Serial.println(nameBuffer);

				// We want to try and connect to the wifi now
				//WiFi.mode(WIFI_AP);
				WiFi.begin(nameBuffer, passwordBuffer);
				//Serial.println((int)WiFi.waitForConnectResult());

				delete[] nameBuffer;
				delete[] passwordBuffer;

				// Wait for connection.
				while (WiFi.status() != WL_CONNECTED) {
					//Serial.println((int)WiFi.status());
					delay(250);
					//yield();

					//TODO: Probably need to check for incorrect wifi password (for polish), but maybe not actually. Cant be bothered.
					if(WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_NO_SSID_AVAIL) {
						incorrectWifiPassword = true;
						Serial.println("Connect failed");

						// When the connection fails, we notify the app
						clients[i].write("i");
						return;
					}
				}

				Serial.println("correcto password");
				clients[i].write("c");
				WiFi.softAPdisconnect(true);
				clientCount = 0;
				createServers();
				connectedToUserWifi = true;
			}

			// Read sensors. We only need to do this if we are actually connected to the user's wifi
			if(connectedToUserWifi) {
				int value1 = analogRead(36);
				int value2 = analogRead(39);
				// Send the data to the app
				clients[i].print(String(value1) + "," + String(value2));
			}
		}

		startMillis = millis();
	}

	// We still want to listen for discovery broadcasts and respond to them
	broadcastListener();
}

// Small util function
void removeClient(byte index) {
	// Move every client down by one in the clients array
	for(byte i = index; i < clientCount - 1; i++) {
		clients[i] = clients[i + 1];
	}
	// We now have one less client
	clientCount--;
}

// Function to receive UDP broadcasts from clients who want to connect
void broadcastListener() {
	// if there's data available, read a packet
	int packetSize = udp.parsePacket();
	if (packetSize) {
		
		// Buffer to hold incoming packet
		char packetBuffer[255];

		// read the packet into packetBufffer
		int len = udp.read(packetBuffer, 255);
		
		Serial.print("Received packet. Contentsss: ");
		Serial.println(packetBuffer);
		Serial.println("BOI");

		// We don't want to reply to a random packet
		if(strcmp(packetBuffer, "areYouTheArduino")) return;
		
		// We now need to send a reply to the IP address and port that sent us the packet. We can begin the packet with this call
		udp.beginPacket(udp.remoteIP(), 1304);
		// The reply that we are sending
		String reply = connectedToUserWifi ? "iAmAnArduino" : "iAmAnArduinonp";
		Serial.println("Sending " + reply);
		udp.print(reply.c_str());
		// endPacket sends the packet to the client
		udp.endPacket();
	}
}

// This will create the UDP listener to listen for broadcasts, and a TCP socket to listen for passwords and send data
void createServers() {
	udp.begin(WiFi.localIP(), 1304);
	// Start the socket server for when we want to send data to clients
	TCPServer.begin();
}

/*int findSeparator(unsigned char* buffer, char* nameBuffer, ) {

}*/