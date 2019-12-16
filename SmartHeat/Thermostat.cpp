#include "Thermostat.h"

Thermostat::Thermostat(PubSubClient *client, Node *parent) : Property(client, parent, (char *)"Thermostat", (char *)"ºC", (char *)"Integer", true) {

}

void Thermostat::set(char *value) {

  setIValue(atoi(value));
  
}
