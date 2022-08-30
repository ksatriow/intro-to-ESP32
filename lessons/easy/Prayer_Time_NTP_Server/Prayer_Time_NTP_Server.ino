#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char *ssid     = "anakkendali.com";
const char *password = "bayardikitlimangewu";

const long utcOffsetInSeconds = 25200;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

#define tahun timeClient.getYear()
#define bulan timeClient.getMonth()
#define tanggal timeClient.getDate()
#define hari daysOfTheWeek[timeClient.getDay()]
#define jam timeClient.getHours()
#define menit timeClient.getMinutes()
#define detik timeClient.getSeconds()

char tanggalapi[11];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

unsigned long lasttime = 0;

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  if (millis() - lasttime >= 1000) {
    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    
    if (bulan >= 10 && tanggal >= 10) {
      sprintf (tanggalapi, "%d-%d-%d", tahun, bulan, tanggal);
    }
    else if (bulan >= 10 && tanggal < 10) {
      sprintf (tanggalapi, "%d-%d-0%d", tahun, bulan, tanggal);
    }
    else if (bulan < 10 && tanggal >= 10) {
      sprintf (tanggalapi, "%d-0%d-%d", tahun, bulan, tanggal);
    }
    else if (bulan < 10 && tanggal < 10) {
      sprintf (tanggalapi, "%d-0%d-0%d", tahun, bulan, tanggal);
    }

    Serial.print(tanggalapi);
    Serial.print("\t");
    
    Serial.print(jam);
    Serial.print(":");
    Serial.print(menit);
    Serial.print(":");
    Serial.println(detik);
    Serial.println();
    lasttime = millis();
  }
  req_api();
}

void req_api(){
  StaticJsonDocument<1024> doc;
  HTTPClient http;
  http.begin("http://api.banghasan.com/sholat/format/json/jadwal/kota/687/tanggal/2017-02-07");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST("?");
  String payload = http.getString();
//  Serial.println(payload);

  DeserializationError error = deserializeJson(doc, payload);

  JsonObject results = doc["jadwal"]["data"];
  String ashar = results["ashar"];
  String dhuha = results["dhuha"];
  String dzuhur = results["dzuhur"];
  String imsak = results["imsak"];
  String isya = results["isya"];
  String subuh = results["subuh"];

  Serial.print("Ashar = ");
  Serial.println(ashar);
  Serial.print("Dhuha = ");
  Serial.println(dhuha);
  Serial.print("Dzuhur = ");
  Serial.println(dzuhur);
  Serial.print("Imsak = ");
  Serial.println(imsak);
  Serial.print("Isya = ");
  Serial.println(isya);
  Serial.print("Subuh = ");
  Serial.println(subuh);
  Serial.println();
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

}
