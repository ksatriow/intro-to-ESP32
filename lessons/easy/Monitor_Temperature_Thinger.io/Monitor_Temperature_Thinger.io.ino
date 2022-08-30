#include "DHT.h"
#include <ThingerESP32.h>

#define DHTPIN 5
#define DHTTYPE DHT11 

#define USERNAME "anakkendali"
#define DEVICE_ID "esp32"
#define DEVICE_CREDENTIAL "esp32_dht11"

#define SSID "anakkendali.com"
#define SSID_PASSWORD "bayardikitlimangewu"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#define relayac 14
#define relaydc 26

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  thing.add_wifi(SSID, SSID_PASSWORD);
  dht.begin();
  pinMode (relayac, OUTPUT);
  pinMode (relaydc, OUTPUT);

  digitalWrite (relayac, 1);
  digitalWrite (relaydc, 1);
  
  thing["relayac"] << (digitalPin(relayac));
  thing["relaydc"] << (digitalPin(relaydc));
  
  thing["DHT11"] >> [](pson& out){
    out["kelembaban"] = dht.readHumidity();
    out["suhu_celcius"] = dht.readTemperature();  
  };
}

void loop() {
  thing.handle();
}
