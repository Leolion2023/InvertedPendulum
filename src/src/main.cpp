#include "BNO055_support.h"
#include <Wire.h>

#define Kp 1
#define Ki 1
#define Kd 1

#define in1 16
#define in2 17
#define in3 18
#define in4 19
#define PWMA 23
#define PWMB 33

float MotoSpeed = 0;
//float MotoSpeedA = 0;
//float MotoSpeedB = 0;

struct bno055_t myBNO;
struct bno055_euler euler;
struct bno055_gyro gyro;

float PID_Error;
//float negPID_Error;
float prevError = 0;
float zero_point = 0.0;
float dt; 

void setup() {
  Wire.begin();
  Serial.begin(115200);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  BNO_Init(&myBNO);
  bno055_set_operation_mode(OPERATION_MODE_GYRONLY); //Verwendung von nur dem Tiltsensor

  bno055_read_euler_hrp(&euler);

  zero_point = euler.p / 16.0;
  Serial.println("Zero Point: " + String(zero_point)); //Erzeugung von einem Nullpunkt von dem aus Winkel bestimmt werden
  delay(100);
  unsigned char gyro_calib = 0;
  while (gyro_calib != 3) {  // Kalibrierung des Sensors
    bno055_get_gyrocalib_status(&gyro_calib);
    Serial.print("Gyro Calibration Status: ");
    Serial.println(gyro_calib);
    delay(500);
  }
}
void loop() {
  bno055_read_euler_hrp(&euler);
  MotoSpeed = PIDRechnung();
  //negPID_Error = PID_Error * -1;
  analogWrite(PWMA, MotoSpeed);
  analogWrite(PWMB, MotoSpeed);
  if (PID_Error > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  // Wenn die Neigung größer als Null ist drehen die Motoren in die dementsprechende Richtung
  else if (PID_Error < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } 
   // Wenn die Neigung kleiner als Null ist drehen die Motoren in die dementsprechende Richtung
  delay(dt);
}

float PIDRechnung() {
  float Error = (euler.p / 16.0) - zero_point;
  float Ppart = Kp * Error; 
  float Ipart = Ki * Error * dt;
  float Dpart = Kd * (Error - prevError) / dt; 
  PID_Error = Ppart + Ipart + Dpart;
  float prevError = Error;
  //Berechnung der Regelabweichung
  PID_Error = constrain (PID_Error,-50,50); 
  //Der Stellwert soll nicht kleiner als -50 und nicht größer als 50 werden
return PID_Error;
}
