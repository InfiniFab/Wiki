//include eeprom
#include <EEPROM.h>




const int clockPin = 4;    //74HC595 Pin 11 
const int latchPin = 3;    //74HC595 Pin 12
const int dataPin = 2;     //74HC595 Pin 14
const int taskPin = A0;     //resistor sensor pin
const int startPin = 5;  //start-stop button
const int clearPin = 6;  //clear button
unsigned task = 0 ;
unsigned taskSel=0;

unsigned long taskTab [10];
unsigned long taskTabFix [10];
unsigned long taskTabRet[10];
boolean timeOn[10] = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
};

int timeDig;
boolean startUp = false;

int j=0;
unsigned long time = millis();

const byte digitNum[4] =      //seven segment digits in bits
{
  B11110111, //1 : lowest digit
  B11111011, //2
  B11111101, //3
  B11111110, //4 : task num
};

const byte digit[10] =      //seven segment digits in bits
{
  B11110101, //0
  B00000101, //1
  B01110011, //2
  B01010111, //3
  B10000111, //4
  B11010110, //5
  B11110110, //6
  B01000101, //7
  B11110111, //8
  B11010111, //9
};

const byte stor[3]{
  B11010110, //S
  B10100010, //t
  B00110110, //O
};
int digitBuffer[4] = {
  0};
int digitScan = 0, flag=0,  soft_scaler = 0;

// Variables will change:
// the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void updateTimer(){
  

  int reading = digitalRead(startPin);
  time = millis()/1000;
  unsigned long sec = time;

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if(timeOn[taskSel]==true && digitalRead(startPin)==LOW){
        timeOn[taskSel]=false;
        taskTabFix[taskSel]=taskTab[taskSel];
        Serial.println("time off*********************************************************");
      }
      else if (timeOn[taskSel]==false && digitalRead(startPin)==LOW){
        timeOn[taskSel]=true;
        taskTabRet[taskSel]=sec;
        time=0;
        Serial.println("time on***********************************************************");
      }

      

    }
  }
  
  /*
  Serial.flush();
  delayMicroseconds(100000);
  */
  /*
  for (int k =0; k<10 ; k++){
    Serial.print("time ");
    Serial.print(k);
    Serial.println(" :");
    Serial.println(taskTab[k]);
  
  };
*/
  if (digitalRead(clearPin)==LOW){
        taskTab[taskSel]=0;
        taskTabFix[taskSel]=0;
      }

  
  if (timeOn[taskSel]==true){
    taskTab[taskSel]=taskTabFix[taskSel]+(sec-taskTabRet[taskSel]);
    if (taskSel==0){
      updateMemory();
      timeOn[taskSel]=false;
    }
  }
  
  lastButtonState = reading;
  Serial.println(taskTab[taskSel]);
  
}
 
void updateDispNum(int taskSel){
  //for (digitScan=0;digitScan<4; digitScan++){  
    for(j=0; j<=3; j++){
      digitalWrite(latchPin, LOW); 
      shiftOut(dataPin, clockPin, MSBFIRST, digitNum[j]);
      if (j==3) {
        shiftOut(dataPin, clockPin, MSBFIRST, digit[taskSel] | B00001000);   
      }
      else if (j==0) {
        timeDig = taskTab[taskSel]/60%10;
        if (timeOn[taskSel] == true){
          shiftOut(dataPin, clockPin, MSBFIRST, digit[timeDig] | B00001000);            
        }
        else {
          shiftOut(dataPin, clockPin, MSBFIRST, digit[timeDig]);  
        }
           
      }
      else if (j==1) {
        timeDig = taskTab[taskSel]/600 %10;
        shiftOut(dataPin, clockPin, MSBFIRST, digit[timeDig]);   
      }
      else if (j==2) {
        timeDig = taskTab[taskSel]/6000 %10;
        shiftOut(dataPin, clockPin, MSBFIRST, digit[timeDig]);   
      }
      delayMicroseconds(100);
      digitalWrite(latchPin, HIGH);
    }
      
}

void updateDispStor(){
  //for (digitScan=0;digitScan<4; digitScan++){  
    for(j=0; j<=3; j++){
      digitalWrite(latchPin, LOW); 
      shiftOut(dataPin, clockPin, MSBFIRST, digitNum[j]);
      if (j==3) {
        shiftOut(dataPin, clockPin, MSBFIRST, digit[taskSel] | B00001000);   
      }
      else if (j==0) {
        if (timeOn == true){
          shiftOut(dataPin, clockPin, MSBFIRST, stor[2] | B00001000);            
        }
        else {
          shiftOut(dataPin, clockPin, MSBFIRST, stor[2]);  
        }
           
      }
      else if (j==1) {
        shiftOut(dataPin, clockPin, MSBFIRST, stor[1]);   
      }
      else if (j==2) {
        shiftOut(dataPin, clockPin, MSBFIRST, stor[0]);   
      }
      delayMicroseconds(100);
      digitalWrite(latchPin, HIGH);
    }
      
}

void updateMemory(){
  EEPROM.put(0,taskTab);
  Serial.println("update eeproms++++++++++++++++++++++++++++++++");
}

void chargeMemory(){
  EEPROM.get(0,taskTab);
  EEPROM.get(0,taskTabFix);
  Serial.println("charge from eeproms++++++++++++++++++++++++++++++++");
}
 
/** Fonction setup() **/
void setup() {

  /* Initialisation du port sÃ©rie */
  Serial.begin(115200);
  pinMode(taskPin, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(startPin, INPUT);
  pinMode(clearPin, INPUT);
  digitalWrite(clearPin, HIGH);

}
 
/** Fonction loop() **/
void loop() {
 
  task = analogRead(taskPin);
  task=task*10/11; 
  taskSel=(task/100U) %10;
  if (startUp == false){
    chargeMemory();
    startUp = true;
  }
  //Serial.println(millis());


  updateTimer();
  if (taskSel !=0){
    updateDispNum(taskSel);  
  }
  else {
    updateDispStor();
  }
  //updateDisp();

  //delay(1)
}
