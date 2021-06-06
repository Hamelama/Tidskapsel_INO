#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int backgroundPin = 4;
int tipPin = 3;
int futurePin = 2;
int busyPin = 6;

int availableInts[] = {1,2, 3, 4, 5, 6, 7, 8, 9, 10};
int takenInts = 0;

unsigned long timeBefore = 0;

void setup() {
  randomSeed(analogRead(A5));
  pinMode(backgroundPin, INPUT);
  pinMode(tipPin, INPUT);
  pinMode(futurePin, INPUT);
  pinMode(busyPin, INPUT);
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
 
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30  
}

void loop() {

  bool isBttnBackgroundPressed = false;
  bool isBttnTipPressed = false;
  //Chamfer
  bool isBttnFuturesPressed = false;
  //Low betyder upptagen
  bool isNotBusy = digitalRead(busyPin);
  unsigned long timeNow = millis();
  //millis har overflow. timeNow är tillbaka till noll. 

  if(isNotBusy)
  {
    
    if((timeNow - timeBefore) > 100)
    {
      isBttnBackgroundPressed = digitalRead(backgroundPin);
      isBttnTipPressed = digitalRead(tipPin);
      isBttnFuturesPressed = digitalRead(futurePin);
      /*
       På sd-kortet: 1-5 är nutid
       6:e är bakgrund
       7 är framtid
       
      */
      if(isBttnBackgroundPressed){
     
          myDFPlayer.playFolder(15, 1);
          timeBefore = timeNow;
          Serial.println("Background");
       
    }
      else if(isBttnTipPressed){
        int tipNum = randomWithoutRepetition();
        myDFPlayer.playMp3Folder(tipNum);
        timeBefore = timeNow;
        Serial.println("Now");
      
      }
      else if(isBttnFuturesPressed){
        
          myDFPlayer.playFolder(15, 2);
          timeBefore = timeNow;
          Serial.println("Future");
      }
    }
  
  }

    if(timeNow < timeBefore){
      Serial.println("Reset time");
    timeBefore = 0;
  }
}
//Likt en påse som man tar ut godis ur men inte lägger tillbaka
int randomWithoutRepetition(){
  //Refill
  if (takenInts == 10) {
    availableInts[0] = 1;
    availableInts[1] = 2;
    availableInts[2] = 3;
    availableInts[3] = 4;
    availableInts[4] = 5;
    availableInts[5] = 6;
    availableInts[6] = 7;
    availableInts[7] = 8;
    availableInts[8] = 9;
    availableInts[9] = 10;
    takenInts = 0;
    Serial.println("REFILL");
   
  }
  bool found = false;
  int i = 0;
  while (!found){
    i = random(1, 11);
    if(availableInts[i-1] == i){
      availableInts[i-1] = -1;
      takenInts++;
      found = true;
    }
  }
  return i; 
  
}
