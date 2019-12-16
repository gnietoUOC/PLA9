#include "Homie.h"
#include "Cache.h"
#include "DLED.h"
#include "Thermostat.h"

int status = WL_IDLE_STATUS;     

unsigned long next1;
unsigned long next2;
#define PERIOD1 5000
#define PERIOD2 1000

#ifdef SECURED
WiFiSSLClient wifiClient;
#else
WiFiClient wifiClient;
#endif
PubSubClient *client;
Homie *homie;
LED *yl;
LED *rl;
DLED *dl;
Thermostat *th;
Temperature *t;
Relay *r1;
#define OFFSET 0.5


void setup() {
  Serial.begin(9600);
//  while (!Serial);

  Serial.println("*** MKR1000 ***");

  // Inicializamos la tarjeta 
  SD.begin(4);
  connectWiFi();
  dumpWiFi();
//  syncClock();

  client = new PubSubClient(wifiClient);
  client->setServer(MQHOST, MQPORT);

  defineDevice();
  dl->set((char *)"RED");
  homie->dump();

  Serial.print("MQTT: ");
  Serial.println(MQHOST);

  client->setCallback(callback);
  client->subscribe("Homie/+/+/+/Set");
  
  dl->set((char *)"GREEN");
  next1 = millis();
  next2 = millis();
}

void loop() {
  if (millis()>next1) {
    next1 = millis()+PERIOD1;
    homie->update();
  }

  if (millis()>next2) {
    next2 = millis()+PERIOD2;

   // Desactivo el calentador si se ha superado la temperatura
   if (t->getValue() > th->getIValue()+OFFSET) {
      r1->set(false);
      rl->set(false);
    }
    
    if (t->getValue() < th->getIValue()) {
      r1->set(true);
      rl->set(true);
    }
  }
    
  homie->reconnect();

  client->loop();
 
}

// Vuelco información básica de la conexión
void dumpWiFi() {

  // Imprimo la SSID de la conexión
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Imprimo la configuración de red
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address:  ");
  Serial.println(ip);

  // Máscara de red
  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  // Dirección del gateway
  Serial.print("Gateway IP:  ");
  Serial.println((IPAddress)WiFi.gatewayIP());

}

// Conexión al AP mediante WiFi
void connectWiFi() {
  
  // Verificamos que la Ethernet está disponible.
  // En una MKR1000 nunca debería fallar.
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // Nos conectamos al AP (Raspberry)
  while ( status != WL_CONNECTED) {
    Serial.print(".");
    status = WiFi.begin(PISSID,PIPWD);

    // Espero un poco antess de reintentar la conexión
    delay(5000);
  }
  Serial.println("\nConnected.");
}

void syncClock() {
  unsigned long epoch;
  do {
        Serial.print("Sync time ...");
        epoch = WiFi.getTime();
        delay(1500);
        Serial.println("done?");
//        numberOfTries++;
//  } while ((epoch == 0) || (numberOfTries < 6));
  } while (epoch == 0);
  Serial.println(epoch);
}

// Función que es invocada cuando el broker MQTT nos envía un mensaje
// al que previamente nos habíamos suscrito.
// TODO: Registrar directamente un método de la instancia de Homie.
void callback(char* topic, uint8_t* payload, unsigned int length) {
  DPRINTLN("-> Callback");
  yl->set((char *)"ON");
  homie->callback(topic, payload, length);
  yl->set((char *)"OFF");
  DPRINTLN("<- Callback");
 }

// Método que define nuestro dispositivo para que podamos gestionar
// su estado utilizando la convención Homie
void defineDevice() {

  DPRINTLN("-> defineDevice");

  homie = new Homie(client);

  Device *device = new Device(client,homie,(char *)"MKR1000");

  Node *node = new Node(client, device,(char *)"MKRCORE");
  Memory *m = new Memory(client,node);
  th = new Thermostat(client,node);
  dl = new DLED(client,node,(char *)"Status",4,3);
  dl->setPublish(false);
  yl = new LED(client,node,5);
  yl->setPublish(false);
  rl = new LED(client,node,0);
  rl->setPublish(false);
  rl->set(true);

  node = new Node(client, device,(char *)"MKRRELAY");
  r1 = new Relay(client,node,1);
  Relay *r2 = new Relay(client,node,2);

  // Si no está conectado el MKRENV, no añado el módulo de sensores
  // a la definición del dispositivo.
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
  } else {
    node = new Node(client, device,(char *)"MKRENV");
    t = new Temperature(client, node);
    Humidity *h = new Humidity(client, node);
    Pressure *p = new Pressure(client, node);
    Illuminance *i = new Illuminance(client, node);
    UVA *ua = new UVA(client, node);
    UVB *ub = new UVB(client, node);
    UVIndex *u = new UVIndex(client, node);
  }

  DPRINTLN("<- defineDevice");
}
