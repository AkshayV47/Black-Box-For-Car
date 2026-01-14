
void setup() {
  pinMode(D8, INPUT);  
  Serial.begin(9600);
}

void loop() {
  bool val = digitalRead(D8);
  Serial.println(val);
  delay(100);

}
