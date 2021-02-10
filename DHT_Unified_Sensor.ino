// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(115200);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}


void createJson(int humi, float temp) {
  StaticJsonDocument<256> doc;
  JsonObject root = doc.to<JsonObject>();
  root["device_type"] = 1;
  JsonObject weather = root.createNestedObject("json_msg");
  weather["temp"] = temp;
  weather["humi"] = humi;
  serializeJson(root, Serial);
    Serial.println();
}

int humi;
float temp;
void loop() {

  delay(delayMS);

  sensors_event_t event;
  dht.temperature().getEvent(&event);


  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {

    temp = event.temperature;
    //json += String("{temp:") +  String(event.temperature, DEC);

  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {

    humi = event.relative_humidity+10;
    // json += String(",humi:}");
  }

  // Serial.println(json);

  createJson(humi,temp);
}
