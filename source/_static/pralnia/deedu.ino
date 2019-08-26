//import the library to control LCD display, this library has been added manually through ZIP file
#include <LiquidCrystal_I2C.h>

//import library for DHT sensor
#include "dht.h"
#define dht_apin A3 // Analog Pin sensor is connected to

//create DHT object
dht DHT;

// create LCD object
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


//declaration of variable for buttond
int ButtonOnePin = 10;  //pin for button 1
int ButtonOneState =0 ; //one variable to store current button state
int ButtonOnePreviousState = 0; //one variable to store previous button state
int ButtonTwoPin = 9; // same for button 2
int ButtonTwoState =0 ;
int ButtonTwoPreviousState = 0;



unsigned long timeBeginDisplay=0;
int timeDisplayPeriod = 6000; //period the 2nd and 3rd screen are displayed
int valToDisplay = 0; //variable to decide what information is to be displayed

int airTemp; //variable to store air temperature measured by DHT11 sensor
int airHumidity; //variable to store air humidity measured by DHT11 sensor

int LedPin =  13; 

int waterPumpPin=7; // pin on which is connected the relay that command water pump
int sprayNum=0; //variable to strore number of spraying 
int pumpOn =false; //variable used for spraying, it stays ON during the spray timing


int MoistureHigh = 800; 
int MoistureGood = 710;
int MoistureLow = 630; //threshold that should launch water spray of the plant
int valSoil ;  //variable to store value measured by moisture sensor
int soilPin = A0; // pin on wich is connected moisture sensor
int soilPower = 12;

int timePeriod = 1000; //variable to store timing in ms, period between each display refresh, a clear is called each time display is called 
int humMeasurePeriod=0; 
unsigned long currentMillis; //variable to store current timing
unsigned long debutMillis=0; //variable used to store timing at the debut of a period
int timePumpPeriod = 3000; //timing for water spraying
unsigned long timePumpDebut = 0; //variable used to store timing value for the debut of a spraying period


int gazPin = 1; //pin on which is connected gaz sensor
int valGaz ; //variable on which is stored gaz sensor value

int lightPin = 2; //pin on which is connected the photoresistor
int valLight ; //variable on which light value is stored

//function to read moisture sensor value
int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    valSoil = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return valSoil;//send current moisture value
}

//function called to refresh LCD screen, displaying wanted screen regarding value of variable "valToDisplay" 
int toDisplay(){

  lcd.clear(); //clear the screen to remove character that might not be overwritten by new message
  
  if (valToDisplay==0){
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("Temp:");
    lcd.setCursor(6,0);
    lcd.print("Hum:");
    lcd.setCursor(11,0);
    lcd.print("Mois:");
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(airTemp);
    lcd.setCursor(2,1);
    lcd.print((char)223); //special character for temperature
    lcd.print("C");
    lcd.setCursor(6,1);  
    lcd.print(airHumidity);
    lcd.setCursor(8,1); 
    lcd.print("%");
    lcd.setCursor(11,1); 
    lcd.print(valSoil); 
  }
  else if (valToDisplay==1 && (currentMillis-timeBeginDisplay)<timeDisplayPeriod){
    lcd.setCursor(0,0);
    lcd.print("CO2 :");
    lcd.setCursor(0,1);
    lcd.print(valGaz);
    lcd.print(" ppm");
    lcd.setCursor(9,0);
    lcd.print("Light:");
    lcd.setCursor(9,1);
    lcd.print(valLight);    
  }
  else if (valToDisplay==2 && (currentMillis-timeBeginDisplay)<timeDisplayPeriod){
    lcd.setCursor(0,0);
    lcd.print("Number of Spray :");
    lcd.setCursor(2,1);
    lcd.print(sprayNum);   
  }  

  if((currentMillis-timeBeginDisplay)>timeDisplayPeriod){ //display original screen when display period is over
    valToDisplay=0;
    //we display in this IF condition the 1rst screen because before there was a blank screen after the timing
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("Temp:");
    lcd.setCursor(6,0);
    lcd.print("Hum:");
    lcd.setCursor(11,0);
    lcd.print("Mois:");
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(airTemp);
    lcd.setCursor(2,1);
    lcd.print((char)223); //special character for temperature
    lcd.print("C");
    lcd.setCursor(6,1);  
    lcd.print(airHumidity);
    lcd.setCursor(8,1); 
    lcd.print("%");
    lcd.setCursor(11,1); 
    lcd.print(valSoil); 
  }
  
}

int readTempHum (){

    DHT.read11(dht_apin); //read value from DHT sensor

    airTemp = DHT.temperature; //store temperature value from sensor
    airHumidity = DHT.humidity; // store humidity value from sensor
    
    //delay(3000);//Wait 3 seconds before accessing sensor again, without timing, sensor measurement is faulty. we commented this delay because this function isn't called often
}

int waterPump(){

  //the condition below to command or not the water pump
  if (valSoil<MoistureLow && pumpOn==false){
    timePumpDebut=currentMillis;
    pumpOn=true; //this value change, so next water pump cycle is launched
  }

  //once the waterpump cycle is launched, this if condition will turn ON the pump on the third of the timePumpPeriod defined, the other 2 third, the water pump will be turned OFF
  if (currentMillis-timePumpDebut<timePumpPeriod && pumpOn==true){
    if((currentMillis-timePumpDebut)<(timePumpPeriod/3)){ //time proportion between spraying and non spraying
      digitalWrite(waterPumpPin,HIGH); //turn ON relay so water pump
    }
    else{
      digitalWrite(waterPumpPin,LOW);  //turn OFF relay so water pump 
    }
  }
  else if (currentMillis-timePumpDebut>=timePumpPeriod && pumpOn==true){
      pumpOn=false; //once pump cycle is over, variable value change to make water spraying dependant on moisture value again
      sprayNum++; //add +1 to the number of spraying performed
  }
}

int waterTankAlarm(){
  if (sprayNum>10){ //when number of spraying is above limit, turn on led to warn user
    digitalWrite(LedPin,HIGH);
  }
  else {
    digitalWrite(LedPin,LOW);
  }
}


void setup() { 
  // put your setup code here, to run once:
  Serial.begin(9600);
  //defining wich pin are IN or OUT
  pinMode(LedPin, OUTPUT);      
  pinMode(ButtonOnePin, INPUT); 
  pinMode(ButtonTwoPin, INPUT); 
  pinMode(waterPumpPin, OUTPUT); 

  // initialize the lcd
  lcd.init();                       
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Waking up");

  //ready first temperature and humidity from DHT sensor to store value (since this function isn't called often)
  readTempHum();
  
  delay(2000);
  lcd.clear();

  
}

void loop() {

  //reading of sensor value, not DHT due to the delay needed to its measurement
  readSoil();
  valGaz = analogRead(gazPin);
  valLight = analogRead(lightPin);

  currentMillis=millis(); //measurement of current time

  //small function to avoid button rebound, when pressing once on the button, arduino reads only one pressing...
  ButtonOneState=digitalRead(ButtonOnePin); //reading of button value
  if (ButtonOneState !=ButtonOnePreviousState){ //comparing with previous state
    if (ButtonOneState == HIGH){ //if previous state was 0 and current is 1, then take button pressing in account
      delay(200);
      valToDisplay=1;
      timeBeginDisplay=currentMillis;
    }
    ButtonOnePreviousState=ButtonOneState;
  }
  //same function for button 2 as button 1
  ButtonTwoState=digitalRead(ButtonTwoPin);
  if (ButtonTwoState !=ButtonTwoPreviousState){
    if (ButtonTwoState == HIGH){
      delay(200);
      valToDisplay=2; //particular value for "valToDisplay"
      timeBeginDisplay=currentMillis;
    }
    ButtonTwoPreviousState=ButtonTwoState;
  }
  
  waterPump(); //calling for water pump function which launch spray cycle if needed

  waterTankAlarm(); //calling function to warn user about water tank level

  //small condition for screen refreshing and measurement of DHT sensor
  if (currentMillis-debutMillis>timePeriod){ //end of a period
    debutMillis=currentMillis; //define the begining of a new period
    toDisplay(); //refresh of the screen regarding "valToDisplay" value
    humMeasurePeriod++; //DHT sensor values are measured every 20 times the screens is refreshed
    if (humMeasurePeriod>20){ //go to measure DHT values
          readTempHum();
          humMeasurePeriod=0;
    }

  }
  
  

}
