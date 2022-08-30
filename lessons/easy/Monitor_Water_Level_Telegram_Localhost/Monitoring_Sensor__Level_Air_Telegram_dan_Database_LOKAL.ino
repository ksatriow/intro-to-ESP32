#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#include <utlgbotlib.h>
#define pintriger 14
#define pinecho 26
long durasi;
float cm, wl;
#define WIFI_SSID "anakkendali.com"
#define WIFI_PASS "mbuhbliweruh"
#define MAX_CONN_FAIL 50
#define MAX_LENGTH_WIFI_SSID 31
#define MAX_LENGTH_WIFI_PASS 63
#define TLG_TOKEN "1254655498:AAH0yMF9u9WNok2ekS9LE89-Qe7fewELGTo"
#define DEBUG_LEVEL_UTLGBOT 0
#define PIN_LED 2
const char TEXT_START[] =
  "Hello, im a Bot running in an ESP microcontroller that let you turn on/off a LED/light.\n"
  "\n"
  "Check /help command to see how to use me.";
const char TEXT_HELP[] =
  "Available Commands:\n"
  "\n"
  "/start - Show start text.\n"
  "/help - Show actual text.\n"
  "/ledon - Turn on the LED.\n"
  "/ledoff - Turn off the LED.\n"
  "/datawater - Show Water Level.\n"
  "/ledstatus - Show actual LED status.";
char buff[30];
void wifi_init_stat(void);
bool wifi_handle_connection(void);
uTLGBot Bot(TLG_TOKEN);
uint8_t led_status;
void setup(void)
{
  Bot.set_debug(DEBUG_LEVEL_UTLGBOT);
  Serial.begin(9600);
  digitalWrite(PIN_LED, LOW);
  pinMode(PIN_LED, OUTPUT);
  led_status = 0;
  wifi_init_stat();
  Serial.println("Waiting for WiFi connection.");
  while (!wifi_handle_connection())
  {
    Serial.print(".");
    delay(500);
  }
  Bot.getMe();
  pinMode (pintriger, OUTPUT);
  pinMode (pinecho, INPUT);
}
void srf() {
  digitalWrite (pintriger, 0);
  delayMicroseconds(2);
  digitalWrite (pintriger, 1);
  delayMicroseconds(10);
  digitalWrite (pintriger, 0);
  delayMicroseconds(2);
  durasi = pulseIn(pinecho, HIGH);
  cm = (durasi * 0.0343) / 2;
  wl = 74.57 - cm; // 74.57 didapat dari pembacaan antara jarak sensor dengan dasar air
  Serial.print("Jarak Sensor : ");
  Serial.print(cm);
  Serial.print(" Cm");
  Serial.print("\t");
  Serial.print("Ketinggian Air : ");
  Serial.print(wl);
  Serial.println(" Cm");
  send_data(wl);
}
void loop()
{
  srf();
  telegram();
}

void telegram(){
  if (!wifi_handle_connection())
  {
    // Wait 100ms and check again
    delay(100);
    return;
  }
  while (Bot.getUpdates())
  {
    // Show received message text
    Serial.println("");
    Serial.println("Received message:");
    Serial.println(Bot.received_msg.text);
    Serial.println("");
    if (strncmp(Bot.received_msg.text, "/start", strlen("/start")) == 0)
    {
      Bot.sendMessage(Bot.received_msg.chat.id, TEXT_START);
    }
    else if (strncmp(Bot.received_msg.text, "/help", strlen("/help")) == 0)
    {
      Bot.sendMessage(Bot.received_msg.chat.id, TEXT_HELP);
    }
    else if (strncmp(Bot.received_msg.text, "/ledon", strlen("/ledon")) == 0)
    {
      led_status = 1;
      digitalWrite(PIN_LED, HIGH);
      Serial.println("Command /ledon received.");
      Serial.println("Turning on the LED.");
      Bot.sendMessage(Bot.received_msg.chat.id, "Led turned on.");
    }
    else if (strncmp(Bot.received_msg.text, "/ledoff", strlen("/ledoff")) == 0)
    {
      led_status = 0;
      digitalWrite(PIN_LED, LOW);
      Serial.println("Command /ledoff received.");
      Serial.println("Turning off the LED.");
      Bot.sendMessage(Bot.received_msg.chat.id, "Led turned off.");
    }
    else if (strncmp(Bot.received_msg.text, "/ledstatus", strlen("/ledstatus")) == 0)
    {
      if (led_status)
        Bot.sendMessage(Bot.received_msg.chat.id, "The LED is on.");
      else
        Bot.sendMessage(Bot.received_msg.chat.id, "The LED is off.");
    }
    else if (strncmp(Bot.received_msg.text, "/datawater", strlen("/datawater")) == 0)
    {
      sprintf(buff, "Ketinggian Air : %f Cm\n", wl);
      Bot.sendMessage(Bot.received_msg.chat.id, buff);
    }
    yield();
  }
}

void wifi_init_stat(void)
{
  Serial.println("Initializing TCP-IP adapter...");
  Serial.print("Wifi connecting to SSID: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("TCP-IP adapter successfuly initialized.");
}
bool wifi_handle_connection(void)
{
  static bool wifi_connected = false;
  // Device is not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wifi_connected)
    {
      Serial.println("WiFi disconnected.");
      wifi_connected = false;
    }
    return false;
  }
  else
  {
    if (!wifi_connected)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      wifi_connected = true;
    }
    return true;
  }
}

void send_data(float val) {
  String datareq = "http://192.168.1.12/waterlevel/api_data.php?";
  datareq += "sensor=";
  datareq += val;
  Serial.println(datareq);
  HTTPClient http;
  http.begin(datareq);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  
}
