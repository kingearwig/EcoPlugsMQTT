
// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
bool RelayStatus=LOW;
#define button 13
#define RelayPin 15
float temp=0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int lastTempcheck=0;

AdafruitIO_Feed *EcoPlug4Relay = io.feed("EcoPlug4Relay");

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode (RelayPin,OUTPUT);
  pinMode(button,INPUT);
  digitalWrite(button,HIGH);
  digitalWrite(RelayPin,RelayStatus);
  // connect to io.adafruit.com
  io.connect();
  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  EcoPlug4Relay->onMessage(handleRelay);
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  if(millis()-lastTempcheck>5000)
  {
    sensors.requestTemperatures();
    temp=sensors.getTempFByIndex(0);
    Serial.println(temp);
    
    lastTempcheck=millis();
  }
  if(!digitalRead(button))
  {
    RelayStatus=!RelayStatus;
    EcoPlug4Relay->save(RelayStatus);
    while(!digitalRead(button));
    Serial.println(RelayStatus);
  }
}

void handleRelay(AdafruitIO_Data *data)
{
  RelayStatus=data->toPinLevel();
  digitalWrite(RelayPin,RelayStatus);
}


