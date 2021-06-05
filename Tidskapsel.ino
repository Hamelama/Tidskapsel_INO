#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int backgroundPin = 4;
int tipPin = 2;
int futurePin = 3;
int busyPin = 6;
int first = 1;

int availableInts[] = {first,first + 1, first + 2, first + 3,first + 4};
int takenInts = 0;

unsigned long timeBefore = 0;

void setup() {
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
  myDFPlayer.playMp3Folder(1);
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
          myDFPlayer.playMp3Folder(first + 5);
          timeBefore = timeNow;
       
    }
      else if(isBttnTipPressed){
        int tipNum = randomWithoutRepetition();
        myDFPlayer.playMp3Folder(tipNum);
        timeBefore = timeNow;
      
      }
      else if(isBttnFuturesPressed){
          myDFPlayer.playMp3Folder(first + 6);
          timeBefore = timeNow;
      }
    }
  
  }

    if(timeNow < timeBefore){
    timeBefore = 0;
  }
}
//Likt en påse som man tar ut godis ur men inte lägger tillbaka
int randomWithoutRepetition(){
  if (takenInts == 5) {
    availableInts[0] = first;
    availableInts[1] = first + 1;
    availableInts[2] = first + 2;
    availableInts[3] = first + 3;
    availableInts[4] = first + 4;
    takenInts = 0;
  }
  bool found = false;
  int i = 0;
  while (!found){
    i = random(first,first + 5);
    if(availableInts[i-1] == i){
      availableInts[i-1] = -1;
      takenInts++;
      found = true;
    }
  }
  return i; 
  
}
