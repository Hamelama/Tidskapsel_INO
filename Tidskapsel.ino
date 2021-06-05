#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int backgroundPin = 4;
int tipPin = 2;
int futurePin = 3;
int busyPin = 6;

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
    if((timeNow - timeBefore) > 1000)
    {
      isBttnBackgroundPressed = digitalRead(backgroundPin);
      isBttnTipPressed = digitalRead(tipPin);
      isBttnFuturesPressed = digitalRead(futurePin);
      /*
       På sd-kortet: 1:a är bakgrund
       2:a är framtidshopp
       3-10 är tips/historier 
       
      */
      if(isBttnBackgroundPressed){
          myDFPlayer.playMp3Folder(1);
          timeBefore = timeNow;
          Serial.println("bak");
    }
      else if(isBttnTipPressed){
          myDFPlayer.playMp3Folder(2);
          timeBefore = timeNow;
           Serial.println("nu");
      }
      else if(isBttnFuturesPressed){
          myDFPlayer.playMp3Folder(3);
          timeBefore = timeNow;
          Serial.println("bak");
      }
    }
  
  }

    if(timeNow < timeBefore){
    timeBefore = 0;
  }
  
//  //Passiv
//  else{
//    if((timeNow - timeBefore) > (10*1000)){
//      Serial.println("Here comes the king");
//      timeBefore = timeNow;
//    }
//  }
}
