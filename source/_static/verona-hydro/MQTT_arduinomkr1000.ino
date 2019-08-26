//library for MQTT protocol
#include <PubSubClient.h>
#include <SPI.h>

//definition for WIFI usage at Plumake
#include <WiFi101.h>
#define wifi_ssid "fablab_guest" // wifi SSID
#define wifi_password "veronafablab"  // wifi pass

//include librairy for deep sleep
#include <RTCZero.h>



//IP Adress of raspberry pi on local network
#define mqtt_server "192.168.50.55"
// #define mqtt_user "guest"     //if mosquitto password protected
// #define mqtt_password "guest" //idem

//for MQTT, definition of each topic used, one sensor valur per topic, "stationMKR" is the 1rst level, several level can be used after, for ex: stationMKR/seedGrow/#
#define temperatura_topic "stationMKR/temperatura"  //Topic temperatura
#define umidita_topic "stationMKR/umidita"        //Topic umidità
#define wtrlvl_topic "stationMKR/wtrlvl"        //Topic water level
#define loopcmd_topic "stationMKR/loopcmd"        //Topic on loop period in minutes

//conf for DHT sensor
#include <Adafruit_Sensor.h>
#include "DHT.h"
#define DHTPIN 7    
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/*
Commented because sleep function isn't implemented yet
// Create an rtc object for sleep mode
RTCZero rtc;

// Change these values to set the current initial time 
const byte seconds = 00;
const byte minutes = 00;
const byte hours = 00;

// Change these values to set the current initial date 
const byte day = 24;
const byte month = 9;
const byte year = 16;

//boolean variable to change before looping into sleep mode
bool matched = false;
*/

//conf for water level sensor
int WTRLVLPIN =6; //pin checked to now if there is still water
int Wtrlvl; //int to store water level result
int WTRPMPCMD =8; //Conf water pump command
int WTRPINCMD =10; //pin to command transistor that allows current to flow in water
unsigned long wtrPmpTmBg; //variable to store the beginning of a pump cycle
int wtrPmpTm ; //period in second on which the water pump cycle should last
int wtrCycCnt =0; //variable that change when a water cycle is launched

//Buffer to store MQTT message recieved
char message_buff[100];

//variable for DHT sensor
float t; //variable for temperature
float h; //variable for humidity

int loopTmCmd =1;  //timing of the loop in minute
unsigned long loopTm=0; //variable to control loop timing

long lastMsg = 0;   //Horodatage du dernier message publi� sur MQTT

bool debug = true;  //display on terminal if true

//creating WIFI object to connect to WIFI
WiFiClient espClient;
//creating object to perform MQTT communication
PubSubClient client(espClient);

void setup() {
	Serial.begin(115200);
	
	setup_wifi();						 //connexion to wifi
	client.setServer(mqtt_server, 1883); //connexion to MQTT server
	client.setCallback(callback);		//callback function is performed at each reception of a message
  
  dht.begin(); //DHT11 init
  
  pinMode(WTRPMPCMD, OUTPUT);         //Set water pump pin as output
  digitalWrite(WTRPMPCMD, LOW);       //default for water pump is LOW so OFF

  delay(5000); //delay so we can see normal current draw, and also having time to upload program if arduino already have sleep mode embeded
   
  pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW); //turn LED off

  /*
  rtc.begin(); //Start RTC library, this is where the clock source is initialized
  matched =true;

  rtc.setTime(hours, minutes, seconds); //set time
  rtc.setDate(day, month, year); //set date, not USED here
  rtc.setAlarmTime(00, 00, 30); //set alarm time to go off in 30 seconds
  //following two lines enable alarm, comment both out if you want to do external interrupt
  rtc.enableAlarm(rtc.MATCH_HHMMSS); //set alarm
  rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
  //comment out the below line if you are using RTC alarm for interrupt
  // extInterrupt(A1); //creates an interrupt source on external pin
  //puts MKR to sleep
  rtc.standbyMode(); //library call
  //samSleep(); //function to show how call works
  */
  
  pinMode(WTRLVLPIN, INPUT);           // set water level to input
  digitalWrite(WTRLVLPIN, HIGH);       // turn on pullup resistors
  pinMode(WTRPINCMD, OUTPUT);           // set water level to input
  digitalWrite(WTRPINCMD, HIGH);       // turn on pullup resistors

}

/*
//interrupt service routine (ISR), called when interrupt is triggered 
//executes after MCU wakes up
void ISR()
{
  matched =true; //to activate sleep mode again at wake up
}

//function that sets up external interrupt
void extInterrupt(int interruptPin) {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin, ISR, LOW);
}
*/

//Connexion to WiFi:
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
	//loop until reconnection is obtained
	while (!client.connected()) {
		Serial.print("Connexion to MQTT...");
		// (client.connect("MKRClient", mqtt_user, mqtt_password)) //For secured MQTT client
		if (client.connect("MKRClient")) {
			Serial.println("connected");
			//Once connected, we can subscribe to a topic
			//for exemple : client.subscribe("outTopic/iotTopic")
      client.subscribe("stationMKR/pumpcmd");
      client.subscribe("stationMKR/loopcmd");
		}
		else {
			Serial.print("failed : ");
			Serial.print(client.state());
			Serial.println("5 seconds before retry");
			delay(5000);
		}
	}
}

//function called to control water level and active pump regardingly
void waterCtrl() {
  digitalWrite(WTRPINCMD, LOW); //command to allow current to go through water
  delay(1000); //a small delay
  Wtrlvl = digitalRead(WTRLVLPIN); //read if there is water between 2 pins of the sensors
  digitalWrite(WTRPINCMD, HIGH); //cut current from water to avoid electrolyse
  
  if (Wtrlvl == 1){ //if there no water the control pin is HIGH
    
    while (millis()-wtrPmpTmBg<wtrPmpTm*1000){ //first loop to turn ON water pump for wtrPmpTm  in seconds
      digitalWrite(WTRPMPCMD, HIGH);
      wtrCycCnt=1; //variable at 1 to allows sending pumping info through MQTT
    }
    while ((wtrPmpTmBg+wtrPmpTm*2000)-millis()>0){ //after pumping make a pause of 2 times wtrPmpTm in seconds
      digitalWrite(WTRPMPCMD, LOW);
    }
  }
  else{
    digitalWrite(WTRPMPCMD, LOW); //keep pump OFF when water is detected
  }
  
}

// Actions performed at message reception from MQTT network
void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  if (debug) {
    Serial.println("Message recu =>  topic: " + String(topic));
    Serial.print(" | longueur: " + String(length, DEC));
  }

  // Lignes de code pour traitement des topics re�us :
  for (i = 0; i<length; i++) {
  message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  if(String(topic)=="stationMKR/pumpcmd"){
    wtrPmpTm = msgString.toInt(); //store message recieved to the right variable
  
  }
  if(String(topic)=="stationMKR/loopcmd"){
    loopTmCmd = msgString.toInt(); //store message recieved to the right variable
  
  }
  if (debug) {
  Serial.println("Payload: " + msgString);
  }
  
}

/*
void sleepMode(){
  if (matched){
  matched=false; //so it goes to this once after a wake up
  rtc.setTime(hours, minutes, seconds); //set time
  rtc.setDate(day, month, year); //set date
  rtc.setAlarmTime(00, 00, 10); //set alarm time to go off in 30 minutes
  rtc.enableAlarm(rtc.MATCH_HHMMSS); //set alarm
  rtc.attachInterrupt(ISR);
  rtc.standbyMode();    // Sleep until next alarm match
  }
}
*/

//function to send sensor value and messages to MQTT
void publishValue(){
  client.publish(temperatura_topic, String(t).c_str(), true);  //sending temp value
  client.publish(umidita_topic, String(h).c_str(), true);      //et l'humidit�

  if (wtrCycCnt==1){ //just one iteration stored on database
    client.publish(wtrlvl_topic, String(wtrPmpTm).c_str(), true); //send data on pumping
    wtrCycCnt=0; //variable put at 1 when pump is ON
  }
  
}

void readTempHum(){
  h = dht.readHumidity(); //read humidity on dht11
  t = dht.readTemperature(); // read temp on dht11
  if (isnan(h) || isnan(t)) { h=0; t=0; } // put temp and humidity at 0 if wrong reading
  delay(2000); //delay for dht11 reading
}

void loop() {
  //loop to make sure MQTT connection is done
  if (!client.connected()) {
      reconnect();
    }
  client.loop();
   
  //condition respecting loop command, will be executed every loopTmCmd in minute
  if((millis()-loopTm)>(loopTmCmd*1000*60)){
    loopTm=millis();
    readTempHum(); //call function to read dht11 value
    waterCtrl(); //call function to read water level
    
    //another loop to make sure MQTT connection is ON
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    
    delay(1000);
    
    publishValue(); //call function to publish data from dht11 and/or waterlevel sensor
  
  }
    
  
  //sleepMode();

}
