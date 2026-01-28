#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(4, 3);   // RX, TX for GPS
SoftwareSerial gsmSerial(7, 8);   // RX, TX for GSM

int buttonPin = 2;
int buzzerPin = 9;

String latitude = "";
String longitude = "";

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  Serial.println("Women's Security System Ready");
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(buzzerPin, HIGH);
    delay(1000);

    getGPSLocation();
    sendSMS();

    digitalWrite(buzzerPin, LOW);
    delay(5000);
  }
}

void getGPSLocation() {
  while (gpsSerial.available()) {
    String data = gpsSerial.readStringUntil('\n');
    if (data.indexOf("GGA") > 0) {
      Serial.println("GPS Data Received");
      latitude = "18.5204";    // demo value
      longitude = "73.8567";   // demo value
      break;
    }
  }
}

void sendSMS() {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);

  gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\"");
  delay(1000);

  gsmSerial.print("EMERGENCY ALERT!\n");
  gsmSerial.print("I need help.\n");
  gsmSerial.print("Location:\n");
  gsmSerial.print("https://maps.google.com/?q=");
  gsmSerial.print(latitude);
  gsmSerial.print(",");
  gsmSerial.print(longitude);
  gsmSerial.write(26);

  Serial.println("Emergency SMS Sent");
}
