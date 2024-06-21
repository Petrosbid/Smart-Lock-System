#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

char ssid[] = "BID_mohammad";
char pass[] = "Ds.jk@38";

BlynkTimer timer;
WiFiClient client; 
HTTPClient http;

const char* serverName = ""; 

bool securityLock = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    handleSerialMessage(message);
    sendLogToServer(message);
  }
}

void myTimerEvent() {
  Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_CONNECTED() {
  Serial.println("Device connected to Blynk.Cloud");
}

void sendEventNotification(String eventName) {
  delay(3000);
  Blynk.logEvent(eventName);
}

void sendLogToServer(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverName); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "log=" + message;
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

void handleSerialMessage(String message) {
  if (message == "NFC:OPENED") {
    sendEventNotification("lock_opened_with_nfc");
  } else if (message == "NFC:FAILED") {
    sendEventNotification("try_with_wrong_nfc");
  }
  else if("NFC:security_FAILED"){
    sendEventNotification("lock_warning_nfc");
  }
}

BLYNK_WRITE(V0) {
  securityLock = param.asInt();
  if (securityLock) {
    Serial.println("LOCK:ON");
  } else {
    Serial.println("LOCK:OFF");
  }
}

BLYNK_WRITE(V1) {
  if (!securityLock && param.asInt() == 1) {
    Serial.println("DOOR:OPEN");
    sendEventNotification("lock_opened_by_blynk");
  } else if (securityLock && param.asInt() == 1) {
    Serial.println("security on");
    sendEventNotification("security_warning");
  }
}
