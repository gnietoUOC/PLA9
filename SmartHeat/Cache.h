#ifndef CACHE_H
#define CACHE_H

#include "Homie.h"

class Base;

class Record {

  public:
//    Record(long time, short type, float value);
//    Record(long time, char *type, float value);
    Record(unsigned long time, Base *base, char *type, char* value);

   long getTime();
   Base *getBase();
//   short geType();
   char *getTag();
//   float getValue();
   char *getValue();

  private:
    unsigned long time;
//    float value;
    char value[8];
//    short type;
    char tag[16];
    Base *base;

  
};

class Cache {

  public:
//    void push(float value);
    void push(Record *record);
//    float pull();
//    float peek();
    Record *pull();
    Record *peek();
    int length();
    Cache(int size);
    ~Cache();

  private:  
    Record **items;
    void inc(int *i);

    int w;
    int r;
    int size;
  
};

#endif
