#define pinrlA 32
#define pinrlB 14
#define pinrlC 27
#define pinrlD 33

#include <WiFi.h>
#include <FirebaseESP32.h>

//1. Change the following info
#define WIFI_SSID "Private_Wifi_"
#define WIFI_PASSWORD "zelwata202010"
#define FIREBASE_HOST "klinik-tanaman-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Kxd2b6VoqKnjOwrL4bTIwVIVnmLYKa6ffHqGRV1o"

FirebaseData fbdo;

void setup()
{
  pinMode(pinrlA, OUTPUT);
  pinMode(pinrlB, OUTPUT);
  pinMode(pinrlC, OUTPUT);
  pinMode(pinrlD, OUTPUT);
  digitalWrite(pinrlA, 1);
  digitalWrite(pinrlB, 1);
  digitalWrite(pinrlC, 1);
  digitalWrite(pinrlD, 1);
  
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop()
{
  if(Firebase.getString(fbdo, "/relay1"))
  {
    Serial.print("Get int data A success, str = ");
    Serial.println(fbdo.stringData());
    digitalWrite(pinrlA, fbdo.stringData().toInt());
  }else{
    Serial.print("Error in getString, ");
    Serial.println(fbdo.errorReason());
  }

  if(Firebase.getString(fbdo, "/relay2"))
  {
    Serial.print("Get int data B success, str = ");
    Serial.println(fbdo.stringData());
    digitalWrite(pinrlB, fbdo.stringData().toInt());
  }else{
    Serial.print("Error in getString, ");
    Serial.println(fbdo.errorReason());
  }

  if(Firebase.getString(fbdo, "/relay3"))
  {
    Serial.print("Get int data C success, str = ");
    Serial.println(fbdo.stringData());
    digitalWrite(pinrlC, fbdo.stringData().toInt());
  }else{
    Serial.print("Error in getString, ");
    Serial.println(fbdo.errorReason());
  }

  if(Firebase.getString(fbdo, "/relay4"))
  {
    Serial.print("Get int data D success, str = ");
    Serial.println(fbdo.stringData());
    digitalWrite(pinrlD, fbdo.stringData().toInt());
  }else{
    Serial.print("Error in getString, ");
    Serial.println(fbdo.errorReason());
  }
}
