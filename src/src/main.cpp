#include "BNO055_support.h"
#include <Wire.h>

// Define PID constants
#define kP 1.0
#define kI 0.0
#define kD 0.0

#define inverted 1.0 // Variable to control motor direction, can be set to -1.0 to invert controls
#define constraint 255.0 // Maximum absolute value for PID output, used to constrain motor speeds

// Define motor control pins
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define PWMA 23
#define PWMB 33

struct bno055_t myBNO;
struct bno055_euler euler;

// Define global variables
float prev_error = 0.0;
float zero_point = 0.0;
float time_step = 0.01; // Time step for PID calculations (10 ms)

void setup() {
  // Initialize I2C and serial communication
  Wire.begin();
  Serial.begin(115200);

  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Initialize BNO055 sensor
  BNO_Init(&myBNO);
  bno055_set_operation_mode(OPERATION_MODE_NDOF); // Set the sensor to NDOF mode for full sensor fusion
  delay(600); // Allow time for sensor to initialize

  // Read initial Euler angles to establish zero point
  bno055_read_euler_hrp(&euler);

  zero_point = euler.p / 16.0;
  Serial.println((String)"Zero Point: " + zero_point);
  
  for (int i = 5000; i > 0; i -= 1000) {
    Serial.println((String)"Starting in " + i / 1000 + " seconds...");
    delay(1000);
  }
}
void loop() {
  // Read current Euler angles from the sensor
  bno055_read_euler_hrp(&euler);
  float motor_speed = pid_calc(constraint) * inverted; // Calculate motor speed based on PID control, invert if necessary
  float sign = (motor_speed >= 0) ? 1 : -1; // Determine the direction of motor control
  motor_speed = abs(motor_speed); // Use the absolute value of the motor speed for PWM control
  // Set motor speeds using PWM
  analogWrite(PWMA, motor_speed);
  analogWrite(PWMB, motor_speed);
  // Control motor direction based on the sign of the PID error
  if (error >= -0.5 && error <= 0.5) { // If the error is small, stop the motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  else if (error > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (error < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } 
  delay(time_step);
}

/** 
 * @brief Calculates the PID output based on the current error and time step
 * @param constraint The maximum absolute value for the PID output, used to constrain motor speeds
 * @return The calculated PID output, which is constrained as it is used with pwm for motor control
 */
float pid_calc(float constraint) {
  // Calculation of the different PID components based on the current error and time step

  // Calculate the error between the current angle and the zero point, 
  // where euler.p is the pitch angle in degrees multiplied by 16 (as per BNO055 specifications)
  float error = (euler.p / 16.0) - zero_point;
  // Calculate the different PID components
  float P = kP * error;
  float I = kI * error * time_step;
  float D = kD * (error - prev_error) / time_step; 
  float output = P + I + D;
  prev_error = error;
  // Constrain the PID output to prevent excessive motor speeds
  output = constrain(output, -constraint, constraint);
  return output;
}
