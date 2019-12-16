#ifndef THERMO_H
#define THERMO_H

#include "Homie.h"

#define OFF     0
#define RED     1
#define GREEN   2
#define YELLOW  3

class Thermostat : public Property {

  public:
    Thermostat(PubSubClient *client, Node *parent);
    void set(char *cvalue);
    void set(int value);

  private:
  
   
};


#endif
