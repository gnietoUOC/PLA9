#include "Homie.h"

Cache::Cache(int size) {
  this->size = size;
  r = 0;
  w = 0;
//  items = (float *)malloc(size*sizeof(float));
  items = (Record **)malloc(size*sizeof(Record *));
}

Cache::~Cache() {
  free(items);
}

//Record::Record(long time, short type, float value) {
//Record::Record(long time, char *tag, float value) {
Record::Record(long time, char *tag, char *value) {
  this->time = time;
//  this->type = type;
  strcpy(this->tag,tag);
//  this->value = value;
  strcpy(this->value,value);
}

long Record::getTime() {
  return time;
}

//short Record::geType() {
//  return type;
//}

char *Record::getTag() {
  return tag;
}


//float Record::getValue() {
//  return value;
//}

char *Record::getValue() {
  return value;
}


/*
void Cache::push(float value) {
  DPRINTLN("-> push");
  items[w]=value;
  inc(&w);
  DPRINTLN("<- push");
}

float Cache::peek() {
  return items[r];
}

float Cache::pull() {
  DPRINTLN("-> pull");
  float value = items[r];
  inc(&r);
  DPRINTLN("<- pull");
  return value;
}
*/

void Cache::push(Record *record) {
  DPRINTLN("-> push");
  items[w]=record;
  inc(&w);
  Serial.print("Push: ");
  Serial.println(w);
  DPRINTLN("<- push");
}

Record *Cache::peek() {
  return items[r];
}

Record *Cache::pull() {
  DPRINTLN("-> pull");
  Record *record= items[r];
  inc(&r);
  DPRINTLN("<- pull");
  return record;
}

int Cache::length() {
/*  
  Serial.print("Size: ");
  Serial.print(w);
  Serial.print(" <-> ");
  Serial.print(r);
  Serial.print(": ");
*/  
  int s = w>=r? w-r:size+w-r;
  
//  Serial.println(s);
  return s;
}

void Cache::inc(int *i) {
  DPRINTLN("-> inc");
  if ((*i)==size) {
    (*i)=0;
  } else {
    (*i)++;
  }
  DPRINTLN("<- inc");
}
