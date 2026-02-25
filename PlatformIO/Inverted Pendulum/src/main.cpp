#include "BNO055_support.h"
#include <Wire.h>

struct bno055_t myBNO;
struct bno055_accel accel;
struct bno055_gyro gyro;
struct bno055_mag mag;
struct bno055_euler euler;


float zero_point = 0.0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  
  BNO_Init(&myBNO);
  bno055_set_operation_mode(OPERATION_MODE_NDOF);

  bno055_read_euler_hrp(&euler);

  zero_point = euler.p / 16.0;
  Serial.println("Zero Point: " + String(zero_point));
  delay(100);

  Serial.println("Calibrating... Please keep the sensor still.");

  unsigned char gyro_calib = 0;
  while (gyro_calib != 3) {  // 3 bedeutet vollständig kalibriert
    bno055_get_gyrocalib_status(&gyro_calib);
    Serial.print("Gyro Calibration Status: ");
    Serial.println(gyro_calib);
    delay(500);
  }
}

void loop() {
  // Alle Daten auslesen
  bno055_read_euler_hrp(&euler);

  float Error = (euler.p / 16.0) - zero_point;  // Fehler in Grad
  
  Serial.println(Error);
  delay(1000);
}