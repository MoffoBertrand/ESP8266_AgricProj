/*
  MQTT Interface for the Agric Project
  Using the ESP8266, this code can publish and subscribe messages through a broker
  The messages published or subscribed are exchange through the UART port of the ESP which is 
  connected to the NIOS Processor.
 */


#include "EspMQTTClient.h"

char Messa[100];
char *ComMes[3];
char *ptr = NULL;

EspMQTTClient client(
	"TECNO_POP_5",
	"hirrkqdpzad9bw3",
	"broker.emqx.io",			// MQTT Broker server ip
	"AgricProjUser",			// Can be omitted if not needed
	"AgricPass",				// Can be omitted if not needed
	"AgricProj",				// Client name that uniquely identify your device
	1883						// The MQTT port, default to 1883. this line can be omitted
);
	

void setup() {
	Serial.begin(115200);
	Serial.setTimeout(1);

	pinMode(LED_BUILTIN, OUTPUT);

	// Optional functionalities of EspMQTTClient
	//client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
	//client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
	//client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
	//client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

void onMessageReceived(const String& topic, const String& payload) {
	Serial.println(topic + "," + payload);
	if (payload == "ON")
		digitalWrite(LED_BUILTIN, HIGH);
	if (payload == "OFF")
		digitalWrite(LED_BUILTIN, LOW);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
	// Subscribe to "mytopic/test" and display received message to Serial
	client.subscribe("AgricProj/#", onMessageReceived);
}

// the loop function runs over and over again forever
void loop() {
	client.loop();
	if (client.isConnected()) {
		if(Serial.available() > 1) {
			int readLegn = Serial.readBytes(Messa, 100);
			byte i = 0;
			Messa[readLegn + 1] = '\0';
			ptr = strtok(Messa, ",");
			while (ptr != NULL) {
				ComMes[i] = ptr;
				i++;
				ptr = strtok(NULL, ",");
			}
			if(*(ComMes[0]) == 'P') {
				client.publish(ComMes[1], ComMes[2]);
			}
			if (*(ComMes[0]) == 'U') {
				client.unsubscribe(ComMes[1]);
			}
		}
		else Serial.flush();
	}
}
