#include <Wire.h>
#include "classAccellerometer.h"

Accellerometer accellerometer(6);

#define outLinePin 9
#define otherMasterPin 8

#define noMasterNoTrigger 0
#define triggerNoMaster 1
#define attemptToBeAMaster 2
#define triggerOtherMaster 3
#define triggerCollision 4
#define triggerIamMaster 5
#define noTriggerOtherMaster 6

unsigned long previousTime = 0;
unsigned long previousMillis = 0;
int statechange = 0;
int sampleTime = 100;

unsigned long RandomNumber;

bool trigger = false,
lock = true;
int state = 0;
bool otherMaster;

char orientationCharacter;

void setup() {
  // put your setup code here, to run once:
  pinMode(outLinePin, OUTPUT);
  pinMode(otherMasterPin, INPUT_PULLUP);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  otherMaster = !digitalRead(otherMasterPin);
  
  switch(state)
  {
    case noMasterNoTrigger:
      digitalWrite(outLinePin, HIGH);
      if(!trigger && otherMaster)
      {
        state = noTriggerOtherMaster;
      }
      else if(trigger)
      {
        state = triggerNoMaster;
      }
      else
      {
        state = noMasterNoTrigger;
      }
      break;

    case triggerNoMaster:
      digitalWrite(outLinePin, HIGH);
      if(trigger && !otherMaster)
      {
        state = attemptToBeAMaster;
      }
      else if(trigger && otherMaster)
      {
        state = triggerOtherMaster;
      }
      else
      {
        state = noMasterNoTrigger;
      }
      break;

    case attemptToBeAMaster:
      digitalWrite(outLinePin, LOW);
      wait(random(10, 30));
      if(trigger && otherMaster)
      {
        state=triggerCollision;
      }
      else if(trigger && !otherMaster)
      {
        state=triggerIamMaster;
      }
      else
      {
        state=noMasterNoTrigger;
      }
      break;

    case triggerOtherMaster:
      digitalWrite(outLinePin,HIGH);
      if(!trigger)
      {
        state=noMasterNoTrigger;
      }
      else if(trigger && !otherMaster)
      {
        state=attemptToBeAMaster;
      }
      else
      {
        state=triggerOtherMaster;
      }
      break;

    case triggerCollision:
      digitalWrite(outLinePin,HIGH);
      //wait for a random delay
      wait(random(10,30));
      if(trigger && !otherMaster)
      {
        state=attemptToBeAMaster;
      }
      else if(!trigger)
      {
        state=noMasterNoTrigger;
      }
      else
      {
        state=triggerCollision;
      }
      break;

    case triggerIamMaster:
      digitalWrite(outLinePin,LOW);
      if(trigger && otherMaster)
      {
        state=triggerCollision;
        RandomNumber = random(10,30);
      }
      else if(!trigger)
      {
        state=noMasterNoTrigger;
      }
      else if(trigger && !otherMaster)
      {
        state=triggerIamMaster;
      }
      break;

    case noTriggerOtherMaster:
      digitalWrite(outLinePin,HIGH);
      if(!trigger && !otherMaster)
      {
        state = noMasterNoTrigger;
      }
      else if(trigger && otherMaster)
      {
        state = triggerOtherMaster;
      }
      else
      {
        state=attemptToBeAMaster;
      }
      break;
      default:
        state=noMasterNoTrigger;
      break;
  }

  if(millis() - previousTime >= sampleTime)
  {
    trigger = true;
    if(state == triggerIamMaster)
    {
      if(lock == true)
      {
        accellerometer.initialize();
        lock = false;
      }
      previousTime = millis();
      accellerometer.checkAngles();
      orientationCharacter = accellerometer.displayOrientation();
      trigger = false;
    }
  }
}

void receiveEvent(int howMany)
{
  while(0 < Wire.available())
  {
    char c = Wire.read();
  }
}

void requestEvent()
{
  Wire.write(orientationCharacter);
}

void wait(unsigned long RandomTime){
  if(millis()-previousMillis>=RandomTime){
    previousMillis=millis();
    statechange = 1;
  }
  else
  {
    statechange = 0;
  }
}