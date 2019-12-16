#include "DLED.h"

const char * PROGMEM color0 = "OFF";
const char * PROGMEM color1 = "RED";
const char * PROGMEM color2 = "GREEN";
const char * PROGMEM color3 = "YELLOW";

int getVal(char *cvalue) {
  int value = -1;

  if (!strcmp(cvalue, "OFF")) {
    value = 0;
  } else {
    if (!strcmp(cvalue, "RED")) {
      value = 1;
    } else {
      if (!strcmp(cvalue, "GREEN")) {
        value = 2;
      } else {
        if (!strcmp(cvalue, "YELLOW")) {
          value = 3;
        }
      }
    }
  }

  Serial.print(cvalue);
  Serial.print(" -> ");
  Serial.println(value);

  return value;
  
}

const char *getLabel(int value) {
  const char *name;

  switch (value) {
    case 0:
      name = color0; 
      break;
    case 1:
      name = color1; 
      break;
    case 2:
      name = color1; 
      break;
    case 3:
      name = color3; 
      break;
  }

  return name;

}

DLED::DLED(PubSubClient *client, Node *parent, char *name, int pin1, int pin2) : Property(client, parent, name, (char *)"", (char *)"Color", true) {

  this->pin1 = pin1;
  this->pin2 = pin2;

  setIValue(0);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin1,0);
  digitalWrite(pin2,0);
  

}

void DLED::set(char *cvalue) {

  Serial.println("<- DLED.set");

  int val = getVal(cvalue);
  set(val);

  Serial.println("<- DLED.set");
}

void DLED::set(int val) {
  Serial.println("-> DLED.Set");

  setIValue(val);
  Serial.println(val);
  Serial.println(val & 0x01);
  Serial.println(val & 0x02);
  digitalWrite(pin1, val & 0x01);
  digitalWrite(pin2, val & 0x02);
//  digitalWrite(pin1, 0);
//  digitalWrite(pin2, 0);

  Serial.println("<- DLED.Set");
}
