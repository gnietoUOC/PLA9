#include "Homie.h"

int copy (char *in, char *out) {
  char bb[128];
  int n;
  int rc=0;

  File fin = SD.open(in, FILE_READ);
  if (fin) {
    File fout = SD.open(out, FILE_WRITE);
    if (fout) {
      while ((n=fin.read(bb,sizeof(bb)))>0) {
//        Serial.println(n);
        fout.write(bb,n);
      }
      fout.close();
    } else {
      Serial.println("Could not open file for WRITE");
      rc = 2;
    }
    fin.close();
  } else {
    Serial.println("Could not open file for READ");
    rc = 1;
  }
  return rc;
}

int move (char *in, char *out) {
  int rc = 0;
  if (!copy(in,out)) {
    if (!SD.remove(in)) {
      Serial.println("Could not remove the file.");
      rc = 2;
    }
  } else {
    Serial.println("Could not copy the file.");
    rc = 1;
  }
}

void clearLog() {
  Serial.println("** Clearing Log **");
  move ((char *)"DATALOG.TXT",(char *)"LOG00.TXT");
}
