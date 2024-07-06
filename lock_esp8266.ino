// get from blynk app
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

char ssid[] = "BID_mohammad";
char pass[] = "Ds.jk@38";

BlynkTimer timer;
WiFiClientSecure client;
HTTPClient http;

const char* serverName = ""; //your server adress

bool securityLock = false;

const long utcOffsetInSeconds = 3 * 3600 + 1800;

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org",
 utcOffsetInSeconds, 60000); 

SoftwareSerial arduinoSerial(12, 14);

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  arduinoSerial.println("Connected to WiFi");
  timeClient.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  timeClient.update();
  setTime(timeClient.getEpochTime());

  if (arduinoSerial.available()) {
    String message = arduinoSerial.readStringUntil('\n');
    if (message.length() > 0) {
      Serial.println("arduino message: " + message);
      sendLogToServer(message);
      handleSerialMessage(message);
    }
  }
}

void myTimerEvent() {
  Blynk.virtualWrite(V2, millis() / 1000);
}
void sendEventNotification(String eventName) {
  Serial.println("trying to send Event blynk");
  Blynk.logEvent(eventName);
}
BLYNK_CONNECTED() {
  Serial.println("Device connected to Blynk.Cloud");
}



void sendLogToServer(String message) {
  if (message.startsWith("Log")) {
    Serial.println("trying to send log server");
    String time = "";
    int yearMiladi = year();
    int monthMiladi = month();
    int dayMiladi = day();
    int hourNow = hour();
    int minuteNow = minute();
    int secondNow = second();

    time = String(yearMiladi) + "/" + String(monthMiladi) + "/" + String(dayMiladi) + " " + String(hourNow) + ":" + String(minuteNow) + ":" + String(secondNow);

    if (WiFi.status() == WL_CONNECTED) {
      client.setInsecure();
      http.begin(client, serverName); 
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // ارسال داده با کلید log
      String postData = "log=" + message + " at: " + time;
      int httpResponseCode = http.POST(postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}


void handleSerialMessage(String message) {
  message.trim();  
  if (message == "NFC:OPENED") {
    sendEventNotification("open_nfc");
  } 
  else if (message == "NFC:FAILED") {
    sendEventNotification("wrong_nfc");
  } 
  else if (message == "NFC:security_FAILED") {
    sendEventNotification("security_nfc");
  } 
}


BLYNK_WRITE(V0) {
  securityLock = param.asInt();
  if (securityLock) {
    Serial.println("LOCK:ON");
    arduinoSerial.println("LOCK:ON");
  } else {
    Serial.println("LOCK:OFF");
    arduinoSerial.println("LOCK:OFF");
  }
}

BLYNK_WRITE(V1) {
  if (!securityLock && param.asInt() == 1) {
    Serial.println("DOOR:OPEN");
    arduinoSerial.println("DOOR:OPEN");
    sendEventNotification("open_blynk");
  } else if (securityLock && param.asInt() == 1) {
    Serial.println("security on");
    arduinoSerial.println("security on");
    sendEventNotification("security_blynk");
  }
}
