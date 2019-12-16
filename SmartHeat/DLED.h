#ifndef DLED_H
#define DLED_H

#include "Homie.h"

#define OFF     0
#define RED     1
#define GREEN   2
#define YELLOW  3

class DLED : public Property {

  public:
    DLED(PubSubClient *client, Node *parent,char *name, int pin1, int pin2);
    void set(char *cvalue);
    void set(int value);
   

  private:
//    Pin *p1;
//    Pin *p2;
    int pin1;
    int pin2;
   
};


#endif
