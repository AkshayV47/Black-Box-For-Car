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

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Thresholds for detection
const float GYRO_FIRE_THRESHOLD = 5.0; // rad/s magnitude for fire detection (adjust as needed)
const float ACCEL_TILT_THRESHOLD = 0.7; // g for tilt detection (if |Z| < threshold, tilted)
const int VIBRATION_HIT_THRESHOLD = HIGH; // Digital high for hit detection

// For speed approximation from accelerometer (simple magnitude, scaled to 0-130 km/h)
// Note: This is a rough approximation; real speed requires integration and calibration
float currentSpeed = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(D8, INPUT);  
  Serial.begin(9600);
  Wire.begin(D1, D2); // SDA = D1 (GPIO5), SCL = D2 (GPIO4)
  // Removed A0 as potentiometer is replaced
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.begin(9600);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print sensor data for debugging
  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s², ");
  Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(" m/s², ");
  Serial.print("Z: "); Serial.println(a.acceleration.z); Serial.print(" m/s²");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x); Serial.print(" rad/s, ");
  Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(" rad/s, ");
  Serial.print("Z: "); Serial.println(g.gyro.z); Serial.print(" rad/s");

  Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" °C");

  Serial.println("-----------------------------");

  // Speed using accelerometer (approximate magnitude of acceleration, mapped to 0-130)
  float accelMagnitude = sqrt(a.acceleration.x * a.acceleration.x +
                              a.acceleration.y * a.acceleration.y +
                              a.acceleration.z * a.acceleration.z);
  // Normalize to g (9.81 m/s²) and map to speed range (rough estimate)
  float accelG = accelMagnitude / 9.81;
  currentSpeed = map(accelG * 100, 0, 100, 0, 130); // Scale arbitrarily
  if (currentSpeed < 0) currentSpeed = 0;
  if (currentSpeed > 130) currentSpeed = 130;
  Blynk.virtualWrite(V0, currentSpeed);
  Serial.print("Speed: "); Serial.println(currentSpeed);

  // Vibration sensor for hit detection (D8)
  bool vibrationVal = digitalRead(D8);
  Serial.print("Vibration: "); Serial.println(vibrationVal);
  // Assuming hit if HIGH; you can integrate this into fire or tilt logic if needed
  // For now, print only; add Blynk.virtualWrite(V3, vibrationVal ? "Hit" : "No Hit"); if desired

  // Fire detection using gyro (high rotation magnitude)
  float gyroMagnitude = sqrt(g.gyro.x * g.gyro.x + g.gyro.y * g.gyro.y + g.gyro.z * g.gyro.z);
  String fireStatus = (gyroMagnitude > GYRO_FIRE_THRESHOLD) ? "Fired" : "No Fire";
  Blynk.virtualWrite(V2, fireStatus);
  Serial.print("Fire Status: "); Serial.println(fireStatus);

  // Tilt detection using accelerometer (check Z-axis for upright position)
  // Assuming Z should be around 1g when upright; if |Z| < threshold, tilted
  float accelZ_G = a.acceleration.z / 9.81;
  String tiltStatus = (abs(accelZ_G) < ACCEL_TILT_THRESHOLD) ? "Tilted" : "Safe";
  Blynk.virtualWrite(V1, tiltStatus);
  Serial.print("Tilt Status: "); Serial.println(tiltStatus);

  Blynk.run();
  delay(100);
}