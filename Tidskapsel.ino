//unsigned long timeBefore = 0;
//tills nästa
//Varför fungerar inte den ena mp3spelaren? Varför fungerar inte den andra med att byta musik?
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int timeBefore = 0;
int backgroundPin = 9;

void setup() {
  pinMode(backgroundPin, INPUT);
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  myDFPlayer.play(1);
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30

    
}

void loop() {
  bool isBttnBackgroundPressed = false;
//  bool bttnTip = false;
//  bool bttnFutures = false;
  unsigned long timeNow = millis();
  //millis har overflow. timeNow är tillbaka till noll. 
  if(timeNow < timeBefore){
    timeBefore = 0;
  }

  if((timeNow - timeBefore) > 1000){
    isBttnBackgroundPressed = digitalRead(backgroundPin);
    if(isBttnBackgroundPressed){
        myDFPlayer.next();
        timeBefore = timeNow;
  }
//  else if(bttnTip){
//    
//  }
//  else if(bttnFutures){
//    
//  }
  }
  
//  //Passiv
//  else{
//    if((timeNow - timeBefore) > (10*1000)){
//      Serial.println("Here comes the king");
//      timeBefore = timeNow;
//    }
//  }
}
