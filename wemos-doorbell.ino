#include <Bounce2.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <MmlMusicPWM.h>
#include <ArduinoJson.h>
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
DynamicJsonBuffer jsonBuffer;

enum NotificationType {
  POWER,
  DORRBELL,
  INTERCOM
};

void pushettaSendMesage(String channel, String text) {
  if (channel != "") {
    JsonObject& message = jsonBuffer.createObject();
    message["body"] = text;
    message["message_type"] = "text/plain";
    String json;
    message.printTo(json);

    HTTPClient pushetta;

    pushetta.begin("http://api.pushetta.com/api/pushes/" + PUSHETTA_CHANNEL + "/");
    pushetta.addHeader("Content-Type", "application/json");
    pushetta.addHeader("Authorization", "Token " + PUSHETTA_KEY);
    pushetta.end();
  }
}

void slackSendMesage(String channel, String text) {
  if (channel != "") {
    JsonObject& message = jsonBuffer.createObject();
    message["channel"] = channel;
    message["text"] =  "<!everyone|everyone> " + text;
    message["as_user"] = "true";
    String json;
    message.printTo(json);

    HTTPClient slack;

    slack.begin("https://slack.com/api/chat.postMessage", SLACK_FINGERPRINT);
    slack.addHeader("Content-Type", "application/json");
    slack.addHeader("Authorization", "Bearer " + SLACK_TOKEN);
    slack.POST(json);
    slack.end();
  }
}

void pingSendGet(String url) {
  if (url != "") {
    HTTPClient ping;

    ping.begin(url);
    ping.GET();
    ping.end();
  }
}

void pingSendPost(String url) {
  if (url != "") {
    HTTPClient ping;

    ping.begin(url);
    ping.POST("");
    ping.end();
  }
}

void notification(NotificationType notification) {
  String message;
  switch (notification)
  {
    case POWER:
      message = "Я включилось!";
      music.play(DOORBELL_SONG);
      pushettaSendMesage(PUSHETTA_CHANNEL, message);
      slackSendMesage(SLACK_CHANNEL, message);
      break;
    case DORRBELL:
      message = "Звонок в дверь!";
      music.play(DOORBELL_SONG);
      pushettaSendMesage(PUSHETTA_CHANNEL, message);
      slackSendMesage(SLACK_CHANNEL, message);
      pingSendGet(DOORBELL_PING_GET);
      pingSendPost(DOORBELL_PING_POST);
      break;
    case INTERCOM:
      message = "Звонок в домофон!";
      music.play(INTERCOM_SONG);
      pushettaSendMesage(PUSHETTA_CHANNEL, message);
      slackSendMesage(SLACK_CHANNEL, message);
      pingSendGet(INTERCOM_PING_GET);
      pingSendPost(INTERCOM_PING_POST);
      break;
    default:
      break;
  }
}

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

  notification(POWER);
}

void loop() {
  ArduinoOTA.handle();

  if (intercom.update() && intercom.read() == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
    if (!music.isPlaying()) {
      notification(INTERCOM);
    }
  } else if (doorbell.update() && doorbell.read() == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
    if (!music.isPlaying()) {
      notification(DORRBELL);
    }
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
