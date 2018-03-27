#include <Bounce2.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <MmlMusicPWM.h>
#include "config.h"

//IP - Input Pin
const int doorbellIP = D4;
const int intercomIP = D5;

//OP - Output Pin
const int buzzerOP = D6;

WiFiClient client;
Bounce doorbell = Bounce();
Bounce intercom = Bounce();

MmlMusicPWM music(buzzerOP);

void setup() {
  pinMode(intercomIP, INPUT_PULLUP);
  intercom.attach(intercomIP);
  intercom.interval(5);

  pinMode(doorbellIP, INPUT_PULLUP);
  doorbell.attach(doorbellIP);
  doorbell.interval(5);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(921600);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname("doorbell");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  
  sendToPushetta(PUSHETTA_CHANNEL, "Я включилось!");
}

void loop() {
  ArduinoOTA.handle();

  if (intercom.update() && intercom.read() == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
    if (!music.isPlaying()) {
      music.play(INTERCOM_SONG);
    }
    sendToPushetta(PUSHETTA_CHANNEL, "Звонок в домофон!");
  } else if (doorbell.update() && doorbell.read() == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
    if (!music.isPlaying()) {
      music.play(DOORBELL_SONG);
    }
    sendToPushetta(PUSHETTA_CHANNEL, "Звонок в дверь!");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void sendToPushetta(char channel[], String text) {
  client.stop();

  if (client.connect("api.pushetta.com", 80))
  {
    Serial.println("Start");
    client.print("POST /api/pushes/");
    client.print(channel);
    client.println("/ HTTP/1.1");
    client.print("Host: ");
    client.println("api.pushetta.com");
    client.print("Authorization: Token ");
    client.println(PUSHETTA_KEY);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(text.length() + 46);
    client.println();
    client.print("{ \"body\" : \"");
    client.print(text);
    client.println("\", \"message_type\" : \"text/plain\" }");
    client.println();
    Serial.println("End");
  }
}
