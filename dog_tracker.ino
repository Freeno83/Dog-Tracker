/*
 * Written by Nicholas Robinson, last updated 27th Sept 2020
 * Hardware used:  Arduino MKR GSM 1400 with MKR GPS mounted as a shield
 * Code purpose: if attached to a dog, provide a way to find the dog when it has escaped
 */

#include <TinyGPS++.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#define THING_ID ""
#define BOARD_ID ""

const char GPRS_APN[]      = "";
const char PINNUMBER[]     = "";
const char GPRS_LOGIN[]    = "";
const char GPRS_PASSWORD[] = "";

float lat;
float lon;
CloudLocation location;

TinyGPSPlus gps;
GSMConnectionHandler ArduinoIoTPreferredConnection(PINNUMBER,GPRS_APN,GPRS_LOGIN, GPRS_PASSWORD); 
                                                   
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //on MKR boards, pins 13 and 14 are used as "Serial1" to communicate with shield
  //while(!Serial);  //This is commented out for use when not conencted to a pc (i.e. auto boot)
  
  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  while(Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      if (gps.location.isValid()){
        lat = gps.location.lat();
        lon = gps.location.lng();
        location = Location(lat, lon);
      }
        
  Serial.println("Latitude: " + String(lat, 6));
  Serial.println("Longitude: " + String(lon, 6));
  delay(30000);
}

void initProperties(){
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(location, READ, ON_CHANGE);
}
