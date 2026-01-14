#define BLYNK_TEMPLATE_ID "TMPL3XIDyN2L6"
#define BLYNK_TEMPLATE_NAME "Black Box"
#define BLYNK_AUTH_TOKEN "LH2OYS1id0v5rVqtiWco41L9Kzw313C2"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>    
#include <SoftwareSerial.h>
const char* ssid = "OPPOReno8T5G";
const char* password ="OPPO8T5G";

// const char* ssid = "oneplus11r5g";
// const char* password ="288560123";

void setup() {
  pinMode(A0, INPUT);  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(A0);
  float Speed = map(potValue, 0, 1023, 0, 130);
  Blynk.virtualWrite(V0, Speed);
  Serial.println(Speed);
  Blynk.run();
  delay(500);
}
