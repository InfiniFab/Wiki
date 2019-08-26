//#include <ESP32HTTPUpdateServer.h>
//#include <EspMQTTClient.h>

/*
Name:		MQTT_ESP12_HTU21
Created:	27/11/2017 12:35:40
Auteur:	limpas guy

Projet objet connect� avec MQTT + Node-RED

*/
//#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi101.h>
//include librairy for dht sensor
#include <DHT.h>
//include librairy for deep sleep
#include <RTCZero.h>


#define wifi_ssid "fablab_guest" // wifi SSID
#define wifi_password "veronafablab"	// wifi pass

#define mqtt_server "192.168.50.55"
// #define mqtt_user "guest"     //if mosquitto password protected
// #define mqtt_password "guest" //idem

#define temperatura_topic "stationMKR/temperatura"  //Topic temperatura
#define umidita_topic "stationMKR/umidita"        //Topic umidità
#define wtrlvl_topic "stationMKR/wtrlvl"        //Topic water level

//conf for DHT sensor
#include <Adafruit_Sensor.h>
#include "DHT.h"
#define DHTPIN 6    
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 00;
const byte minutes = 00;
const byte hours = 00;

/* Change these values to set the current initial date */
const byte day = 24;
const byte month = 9;
const byte year = 16;

//boolean variable to change before looping
bool matched = false;

//conf for water level sensor
int WTRLVLPIN =7;
boolean Wtrlvl;

//Conf water pump command
int WTRPMPCMD =5;

//Buffer qui permet de d�coder les messages MQTT re�us
char message_buff[100];


long lastMsg = 0;   //Horodatage du dernier message publi� sur MQTT

bool debug = true;  //Affiche sur la console si True

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
	Serial.begin(115200);
	
	setup_wifi();						 //connexion to wifi
	client.setServer(mqtt_server, 1883); //connexion to MQTT server
	client.setCallback(callback);		//La fonction de callback qui est execut�e � chaque r�ception de message   
  
  dht.begin(); //DHT11 init
  pinMode(WTRLVLPIN, INPUT);           // set water level to input
  digitalWrite(WTRLVLPIN, HIGH);       // turn on pullup resistors

  pinMode(WTRPMPCMD, OUTPUT); 
  digitalWrite(WTRPMPCMD, LOW);

  delay(5000); //delay so we can see normal current draw
   pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW); //turn LED off

  rtc.begin(); //Start RTC library, this is where the clock source is initialized

  rtc.setTime(hours, minutes, seconds); //set time
  rtc.setDate(day, month, year); //set date

  rtc.setAlarmTime(00, 00, 30); //set alarm time to go off in 30 seconds
  
  //following two lines enable alarm, comment both out if you want to do external interrupt
  rtc.enableAlarm(rtc.MATCH_HHMMSS); //set alarm
  rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
  //comment out the below line if you are using RTC alarm for interrupt
 // extInterrupt(A1); //creates an interrupt source on external pin
  
  //puts SAMD21 to sleep
  rtc.standbyMode(); //library call
  //samSleep(); //function to show how call works
  
}

//interrupt service routine (ISR), called when interrupt is triggered 
//executes after MCU wakes up
void ISR()
{
  matched =true;
  pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW);
  
}

//function that sets up external interrupt
void extInterrupt(int interruptPin) {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, ISR, LOW);
}

//Connexion au r�seau WiFi:
void setup_wifi() {
	delay(10);
	Serial.println();
	Serial.print("Connexion to ");
	Serial.println(wifi_ssid);

	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Connexion WiFi established ");
	Serial.print("=> Addresse IP : ");
	Serial.println(WiFi.localIP());
}

//Reconnexion :
void reconnect() {
	//Boucle jusqu'� obtenur une reconnexion
	while (!client.connected()) {
		Serial.print("Connexion to MQTT...");
		// (client.connect("MKRClient", mqtt_user, mqtt_password)) //For secured MQTT client
		if (client.connect("MKRClient")) {
			Serial.println("connected");
			//Une fois connect�, on peut s'inscrire � un Topic
			//par exemple : client.subscribe("outTopic/iotTopic")
      client.subscribe("stationMKR/pumpcmd");
		}
		else {
			Serial.print("failed : ");
			Serial.print(client.state());
			Serial.println("5 seconds before retry");
			delay(5000);
		}
	}
}

void loop() {
	
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
  
	long now = millis();

  float h = dht.readHumidity(); //read humidity on dht11
  float t = dht.readTemperature(); // read temp on dht11
  if (isnan(h) || isnan(t)) { h=0; t=0; } // put temp and humidity at 0 if wrong reading
  delay(2000); //delay for dht11 reading


  Wtrlvl=digitalRead(WTRLVLPIN); //getting waterlevel sensor value
  
	client.publish(temperatura_topic, String(t).c_str(), true);  //sending temp value
	client.publish(umidita_topic, String(h).c_str(), true);      //et l'humidit�
  client.publish(wtrlvl_topic, String(Wtrlvl).c_str(), true);

  pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW);

  if (matched){
    matched=false; //so it goes to this once after a wake up
    rtc.setTime(hours, minutes, seconds); //set time
    rtc.setDate(day, month, year); //set date

    rtc.setAlarmTime(00, 30, 00); //set alarm time to go off in 10 seconds
    rtc.standbyMode();    // Sleep until next alarm match
  }
}

// D�clenche les actions � la r�ception d'un message
void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  
  /*
	
	if (debug) {
		Serial.println("Message recu =>  topic: " + String(topic));
		Serial.print(" | longueur: " + String(length, DEC));
	}
  */
	// Lignes de code pour traitement des topics re�us :
	for (i = 0; i<length; i++) {
	message_buff[i] = payload[i];
	}
	message_buff[i] = '\0';
  
	String msgString = String(message_buff);
  /*
	if (debug) {
	Serial.println("Payload: " + msgString);
	}
  */
  
	if (msgString == "ON") {
	digitalWrite(WTRPMPCMD, HIGH);
	}
	else {
	digitalWrite(WTRPMPCMD, LOW);
	}
	
}
