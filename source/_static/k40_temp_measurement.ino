/**
 * Exemple de code pour lire un unique capteur DS18B20 sur un bus 1-Wire.
 */
 
/* DÃ©pendance pour le bus 1-Wire */
#include <OneWire.h>
 
/*
 * crea
    if(digitScan>3) digitScan=0;
    if(digitScan>3) digitScan=0;als.com
 * Temp
    if(digitScan>3) digitScan=0;ent common anode 
 * 2013
    if(digitScan>3) digitScan=0;
 */
const int digitPins[4] = {
  4,5,6,7};                 //4 common anode pins of the display
const int clockPin = 11;    //74HC595 Pin 11 
const int latchPin = 12;    //74HC595 Pin 12
const int dataPin = 13;     //74HC595 Pin 14
const int tempPin = A0;     //temperature sensor pin
const byte digit[11] =      //seven segment digits in bits
{
  B00111111, //0
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111101, //6
  B00000111, //7
  B01111111, //8
  B01101111, //9
  B00111001  //C
};
int digitBuffer[4] = {
  0};
int digitScan = 0, flag=0,  soft_scaler = 0;

float tempK, tempC, tempF, temp;

float rawHigh = 99.56;
int compteurtemp=0;

 
/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 2;

/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reÃ§ue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};


/* CrÃ©ation de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);
 
void updateDisp(){
  //for (digitScan=0;digitScan<4; digitScan++){  
    for(byte j=0; j<4; j++)  
      digitalWrite(digitPins[j], LOW);
    
    digitalWrite(latchPin, LOW); 
 


    shiftOut(dataPin, clockPin, MSBFIRST, B11111111);
    digitalWrite(latchPin, HIGH);
    
    delayMicroseconds(100);
    digitalWrite(digitPins[digitScan], HIGH); 

    digitalWrite(latchPin, LOW);  
    
    if(digitScan==2)
      shiftOut(dataPin, clockPin, MSBFIRST, ~(digit[digitBuffer[digitScan]] | B10000000)); //print the decimal point on the 3rd digit
    else
      shiftOut(dataPin, clockPin, MSBFIRST, ~digit[digitBuffer[digitScan]]);
    
    
    //shiftOut(dataPin, clockPin, MSBFIRST, ~digit[digitBuffer[digitScan]]);


    digitalWrite(latchPin, HIGH);
  digitScan++;
  if(digitScan>3) digitScan=0; 
  //}    
}
 
/**
 * Fonction de lecture de la tempÃ      unsigned long end = micros();
  unsigned long delta = end - start;

  Serial.println(delta);©rature via un capteur DS18B20.
 */   

 
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : DonnÃ©es lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire dÃ©tectÃ©

  //on hardcode addr car un seul capteur & branchement hdw identique
  addr[0]=40;
  addr[1]=255;
  addr[2]=179;
  addr[3]=38;
  addr[4]=146;
  addr[5]=21;
  addr[6]=1;
  addr[7]=26;
  
  
    
  /* Reset le bus 1-Wire ci nÃ©cessaire (requis pour la lecture du premier capteur) */
  //addr hardcodé, on skipe
  /*
  if (reset_search) {
    ds.reset_search();      unsigned long end = micros();
  unsigned long delta = end - start;

  Serial.println(delta);
  }
  */
 
  /* Recherche le prochain capteur 1-Wire disponible */
  /*
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  */
 /*
  for(int i=0;i<8;i++){

    Serial.print("addr");
    Serial.print(i);
    Serial.println(":");
    Serial.println(addr[i]);
  }

  Serial.print("\n");
  */
  /*
  unsigned long start = micros();
  unsigned long end = micros();
  unsigned long delta = end - start;
  Serial.println(delta);
  */
  /* VÃ©rifie que l'adresse a Ã©tÃ© correctement reÃ§ue */
  //addr hardcodé, on skipe
  /*
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
  */
  /* VÃ©rifie qu'il s'agit bien d'un DS18B20 */
  //on skipe
  /*
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
  */
  
  /* Reset le bus 1-Wire et sÃ©lectionne le capteur */
  ds.reset();
  updateDisp();
  
  ds.select(addr);
  updateDisp();
  
  /* Lance une prise de mesure de tempÃ©rature et attend la fin de la mesure */
  ds.write(0x44, 1);
  updateDisp();

  
  /* Reset le bus 1-Wire, sÃ©lectionne le capteur et envoie une demande de lecture du scratchpad */

  ds.reset();
  updateDisp();
  ds.select(addr);
  updateDisp();
  ds.write(0xBE);
  updateDisp();

 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
    updateDisp();
  }

  /* Calcul de la tempÃ©rature en degrÃ© Celsius */
  *temperature = ((data[1] << 8) | data[0]) * 0.0625; 
  updateDisp();
  
  // Pas d'erreur
  return READ_OK;
}
 
 
/** Fonction setup() **/
void setup() {

  /* Initialisation du port sÃ©rie */
  Serial.begin(115200);
  for(int i=0;i<4;i++)
  {
    pinMode(digitPins[i],OUTPUT);
  }
  pinMode(tempPin, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(tempPin, INPUT);
}

//writes the temperature on display

 
 
/** Fonction loop() **/
void loop() {
  float temperature;
  
   /*Lit la tempÃ©rature ambiante Ã  ~1Hz */
  if (compteurtemp<1000){
    compteurtemp++;
  }
  else{
    if (getTemperature(&temperature, true) != READ_OK) {
      Serial.println(F("Erreur de lecture du capteur"));
      return;
    };
    compteurtemp=0; 
  }
  

  /* Affiche la tempÃ©rature */
  
  Serial.print(F("Temperature : "));
  Serial.print(temperature, 2);
  Serial.write(176); // CaractÃ¨re degrÃ©
  Serial.write('C');
  
    tempK = (((analogRead(tempPin)/ 1023.0) * 5.0) * 100.0);  
  //Converts Kelvin to Celsius minus 2.5 degrees error
  tempC = tempK - 273.0;   
  tempF = ((tempK - 2.5) * 9 / 5) - 459.67;
  //Celsius temperature display
  tempC = int(tempC*100);
  /*
  digitBuffer[3] = int(temperature)/1000;
  digitBuffer[2] = (int(temperature)%1000)/100;
  digitBuffer[1] = (int(temperature)%10Serial.println(addr);0)/10;
  digitBuffer[0] = (int(temperature)%100)%10;
  */
  int nombre = temperature*100;
  int digitnum= (nombre/10)%10;
  digitBuffer[3] = (nombre/1000)%10;
  digitBuffer[2] = (nombre/100)%10;
  digitBuffer[1] = (nombre/10)%10;
  digitBuffer[0] = 10;
  updateDisp();
  Serial.println(digitnum);

  //updateDisp();

  //delay(1);

  /*
  //Fahrenheit temperature display
  tempF = int(tempF*100);
  digitBuffer[3] = int(tempF)/1000;
  digitBuffer[2] = (int(tempF)%1000)/100;
  digitBuffer[1] = (int(tempF)%100)/10;
  digitBuffer[0] = (int(tempF)%100)%10;
  updateDisp();
  delay(2);
*/
}

