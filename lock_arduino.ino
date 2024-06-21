#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <SoftwareSerial.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc(pn532_i2c);
byte tagId[8];  // Adjusted size to match typical UID length
bool security_lock = false;
bool try_NFC = false;
bool open_NFC = false;
bool open_Blynk = false;
bool open_Lock = false;
bool try_Blynk = false;
SoftwareSerial espSerial(2, 3);

byte correct_nfcs[][4] = {
  { 0x53, 0xC5, 0x76, 0x34 },
  { 0xF3, 0x3E, 0xFE, 0x0C },
  { 0x08, 0x75, 0xC6, 0x4E } 
};

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  Serial.println("System initialized");
  nfc.begin();
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(4, LOW); // Relay initially off
  digitalWrite(5, LOW); // Relay initially off
}

void loop() {
  readNFC();
  if (espSerial.available()) {
    String message = espSerial.readStringUntil('\n');
    handleSerialMessage(message);
  }
}

void readNFC() {
  if (nfc.tagPresent()) {
    try_NFC = true;
    NfcTag tag = nfc.read();
    tag.getUid(tagId, sizeof(tagId));
    Serial.print("UID: ");
    for (int i = 0; i < sizeof(tagId); i++) {
      Serial.print(tagId[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    checknfc(); 
  }
  delay(1000);
}

void handleSerialMessage(String message) {
  if (message.startsWith("LOCK:")) {
    if (message == "LOCK:ON") {
      security_lock = true;
      Send_Log();
      Serial.println("Security Lock is ON");
    } else if (message == "LOCK:OFF") {
      security_lock = false;
      Send_Log();
      Serial.println("Security Lock is OFF");
    }
  } else if (message.startsWith("DOOR:")) {
    if (message == "DOOR:OPEN") {
      if (!security_lock){
      openlock();
      open_Blynk = true;
      Serial.println("Door is OPENED");
      Send_Log();
      }
      else{
        try_Blynk = true;
        Serial.println("security mode is on! can't open the door");
        Send_Log();
      }
    }
  }
}

void checknfc() {
  bool found = false;

  for (int i = 0; i < sizeof(correct_nfcs) / sizeof(correct_nfcs[0]); i++) {
    if (checkCardID(tagId, correct_nfcs[i], sizeof(correct_nfcs[i]))) {
      found = true;
      break;
    }
  }
  
  if (found) {
    Serial.println("Correct NFC found.");
    if(security_lock){
      espSerial.println("NFC:security_FAILED");
      Send_Log();
    }
    else{
      openlock();
      open_NFC = true;
      espSerial.println("NFC:OPENED");
      Send_Log();
    }
  } else {
      Serial.println("Incorrect NFC found.");
      espSerial.println("NFC:FAILED");
      Send_Log();
  }
}

bool checkCardID(byte *id, byte *correctID, byte size) {
  for (byte i = 0; i < size; i++) {
    if (id[i] != correctID[i]) return false;
  }
  return true;
}

void openlock() {
  if (security_lock) {
    Serial.println("Security lock is on. Can't open the door");
  } else {
    digitalWrite(4, HIGH); 
    delay(5000); 
    digitalWrite(4, LOW); 
    open_Lock = true;
  }
  Send_Log();
}

void Send_Log() {
  String message = "";
  unsigned long currentTime = millis(); 

  String UID = "";
  if (try_NFC || open_NFC) {
    for (int i = 0; i < sizeof(tagId); i++) {
      UID += String(tagId[i], HEX) + " ";
    }
  }

  if (open_NFC) {
    message = "open the door with NFC at: " + String(currentTime) + " UID: " + UID;
    open_NFC = false;
    try_NFC = false;
  } else if (try_NFC && !open_NFC && !security_lock) {
    message = "Try to open the door with wrong NFC at: " + String(currentTime) + " UID: " + UID;
    try_NFC = false;
  } else if (try_NFC && !open_NFC && security_lock) {
    message = "Try to open the door with NFC in security time at: " + String(currentTime) + " UID: " + UID;
    try_NFC = false;
  } else if (open_Blynk) {
    message = "door opened with Blynk app at: " + String(currentTime);
    open_Blynk = false;
  }  else if (security_lock && try_Blynk){
    message = "Try to open the door with Blynk in security time at: " + String(currentTime);
    try_Blynk = false;
    security_lock = false;
  } else if (security_lock) {
    message = "security mode turn on at: " + String(currentTime);
  } else {
    message = "security mode turn off at: " + String(currentTime);
  }
  espSerial.println(message);
  Serial.println("Log: " + message); 
}
