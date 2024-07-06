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

byte correct_nfcs[][8] = { 
  { 0x53, 0xC5, 0x76, 0x34, 0x00, 0x00, 0x00, 0x00 },
  { 0x08, 0x75, 0xC6, 0x4E, 0x00, 0x00, 0x00, 0x00 },
  { 0x08, 0xFE, 0x84, 0x95, 0x00, 0x00, 0x00, 0x00 },
  { 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 }
};

void setup() {
  Serial.begin(9600);
  Serial.println("System initialized");
  nfc.begin();
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
}

void loop() {
  readNFC();
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();
    Serial.println("Received message: '" + message + "'");
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
  if (message == "LOCK:ON") {
    security_lock = true;
    Serial.println("Security Lock is ON");
    Send_Log();
  } 
  else if (message == "LOCK:OFF") {
    security_lock = false;
    Serial.println("Security Lock is OFF");
    Send_Log();
  }
  else if (message == "DOOR:OPEN") {
    open_Blynk = true;
    openlock();
    Serial.println("Door is OPENED");
    Send_Log();
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
    open_NFC = true;
    openlock();
    if(!security_lock){
      Serial.println("NFC:OPENED");
      delay(2000);
    }
  } 
  else{
    Serial.println("Incorrect NFC found.");
    Serial.println("NFC:FAILED");
    delay(2000);
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
    Serial.println("NFC:security_FAILED");
    delay(2000);
    Send_Log();
  } else {
    digitalWrite(4, LOW); 
    delay(5000); 
    digitalWrite(4, HIGH); 
    open_Lock = true;
    Send_Log();
  }
}

void Send_Log() {
  String message = "";

  String UID = "";
  if (try_NFC || open_NFC) {
    for (int i = 0; i < sizeof(tagId); i++) {
      UID += String(tagId[i], HEX) + " ";
    }
  }

  if (open_NFC && !security_lock) {
    message = "Door opened with NFC. UID: " + UID;
    open_NFC = false;
  } else if (try_NFC && !open_NFC && !security_lock) {
    message = "Try to open the door with wrong NFC. UID: " + UID;
  } else if (try_NFC && security_lock) {
    message = "Try to open the door with NFC in security time. UID: " + UID;
  } else if (open_Blynk) {
    message = "Door opened with Blynk app.";
    open_Blynk = false;
    
  } else if (security_lock && try_Blynk) {
    message = "Try to open the door with Blynk in security time.";
  } else if (security_lock) {
    message = "Security mode turn on.";
  } else {
    message = "Security mode turn off.";
  }

  try_NFC = false;
  try_Blynk = false;
  
  Serial.println("Log: " + message); 
}
