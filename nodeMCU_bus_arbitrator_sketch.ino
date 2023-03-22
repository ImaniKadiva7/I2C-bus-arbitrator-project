#include <Wire.h>

#define outLinePin D5
#define otherMasterPin D3

#define noMasterNoTrigger 0
#define triggerNoMaster 1
#define attemptToBeAMaster 2
#define triggerOtherMaster 3
#define triggerCollision 4
#define triggerIamMaster 5
#define noTriggerOtherMaster 6

bool trigger = false;
int state = 0;
bool otherMaster;

unsigned long samplingTime = 333;
unsigned long previousmillis = 0;
int statechange = 0;
unsigned long previoustime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(outLinePin, OUTPUT);
  pinMode(otherMasterPin, INPUT_PULLUP);
  Wire.begin(D1, D2);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  otherMaster = !digitalRead(otherMasterPin);
  
  switch(state)
  {
    case noMasterNoTrigger:
      digitalWrite(outLinePin, HIGH);
      if(!trigger && !otherMaster)
      {
        state = noMasterNoTrigger;
      }
      else if(trigger)
      {   
        state=triggerNoMaster;    
      }
      else
      {
        state=noTriggerOtherMaster;
      }
      break;

    case triggerNoMaster:
      digitalWrite(outLinePin,HIGH);
      if(trigger && !otherMaster)
      {
        state=attemptToBeAMaster;
      }
      else if(trigger && otherMaster)
      {
        state=triggerOtherMaster;
      }
      else
      {
        state=noMasterNoTrigger;
      }
      break;

    case attemptToBeAMaster:
      digitalWrite(outLinePin,LOW);
      //Wait for a random delay
      wait(random(10,30));
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
      }
      else if(!trigger)
      {
        state=noMasterNoTrigger;
      }
      else
      {
        state=attemptToBeAMaster;
      }
      break;

    case noTriggerOtherMaster:
      digitalWrite(outLinePin,HIGH);
      if(!trigger && !otherMaster)
      {
        state=noMasterNoTrigger;
      }
      else if(trigger && otherMaster)
      {
        state=triggerOtherMaster;
      }
      else if(trigger && !otherMaster)
      {
        state=attemptToBeAMaster;
      }
      break;
    default:
      state=noMasterNoTrigger;
      break;
  }

  if(millis() - previousmillis >= samplingTime)
  {
    trigger = true;
    if(state == triggerIamMaster)
    {
      previousmillis = millis();
      Wire.beginTransmission(8);
      Wire.requestFrom(8, 1);
      while(Wire.available())
      {
        char c = Wire.read();
        Serial.print(c);
      }
      trigger = false;
      Serial.println();
    }
    Serial.println();  
  }
}

void wait(unsigned long RandomNumber){
  if(millis()-previoustime>=RandomNumber){
    previoustime=millis();
    statechange = 1;
  }
  else
  {
    statechange = 0;
  }
}