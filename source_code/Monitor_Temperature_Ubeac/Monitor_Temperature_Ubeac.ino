#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <Adafruit_Sensor.h>
#include "DHT.h"

#define pinAdc0 39

#define DHTPIN 5
#define DHTTYPE    DHT11
int adcVal;
DHT dht(DHTPIN, DHTTYPE);

const char *url = "http://anakkendali.hub.ubeac.io/ESP32anakkendali";

const char *ssid = "Private_Wifi_";  //Nama Wifi
const char *password = "zelwata202010"; // pass wifi

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    return;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  analogReadResolution(10);
  float volt, suhucel;

  adcVal = analogRead(pinAdc0);
  volt = (adcVal * 5.0) / 1023;
  suhucel = 100 * volt + 5.0 ;

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C\t"));

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%\t"));

  Serial.print("Tegangan :");
  Serial.print(volt);
  Serial.print("\tSuhu :");
  Serial.print(suhucel);
  Serial.println(" ^C");
  senUbeac(suhucel, t, h);

}

void senUbeac(float a, float b, float c) {
  String postData = (String)"{\"uid\": \"ESP32\",\"ts\": 1542326605,\"sensors\":[";
  postData += (String)"{\"name\": \"Suhu Celcius LM35\",\"unit\": 2,\"prefix\": 0,\"type\": 4,\"value\":" + a + "},";
  postData += (String)"{\"name\": \"Suhu Celcius DHT11\",\"unit\": 2,\"prefix\": 0,\"type\": 4,\"value\":" + b + "},";
  postData += (String)"{\"name\": \"Kelembaban DHT11\",\"unit\": 20,\"prefix\": 0,\"type\": 5,\"value\":" + c + "}";
  postData += "]}";

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(postData);
  Serial.println(postData);

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.printf("Error occurred while sending HTTP POST: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
