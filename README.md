## InvertedPendulum
### Concept Idea
A small robot which can balance itself so it always stands up even though it only has two wheels.
The motivation behind this is a school project which main focus is on self regulating loops. Whereas this robot was our idea.

## Contents

1. [Hardware](#hardware)
    1. [Requirements](#requirements)
    2. [Pinout](#pinout)
2. [Software](#software)
    1. [Code](#code)
    2. [Control Loop Diagram](#control-loop-diagram)
    3. [Program Flow Chart](#program-flowchart)
3. [Operating Instructions](#operating-instructions)

## To Do

- [x] Create a CAD Model of the robot
- [x] Code to simply drive the robot so it always stands up
- [] Improve the code so it can be calibrated with phone
    - Make it with BLE connect to an app OR
    - It opens a webserver in it's own WiFi

# Hardware
## Requirements


- 2 x Gearmotors ~600RPM
    - 2 x M3 Screws 7.3mm  --> countersunk head (flat head)¹
- ESP32
- BNO055 9DoF Sensor²
- H-Bridge (f.e. HW-095 L298N)
- CAD 3D prints ([see here](...))


**Notes:**
1. The screws depend on the gearmotor you use.
2. We used this self-created [wiki](https://github.com/Leolion2023/BNO055) to use this sensor as the documentation of Bosch is pretty bad.

## Pinout

| ESP32 Pin | Function | Additional |
|---|---|---|
| GPIO16 | H-Bridge IN1 | Used for Motor 1 |
| GPIO17 | H-Bridge IN2 | Used for Motor 1 |
| GPIO23 | H-Bridge ENA | Used for Motor 1 |
| GPIO18 | H-Bridge IN3 | Used for Motor 2 |
| GPIO19 | H-Bridge IN4 | Used for Motor 2 |
| GPIO33 | H-Bridge ENB | Used for Motor 2 |
| GPIO21 | BNO055 SDA | |
| GPIO22 | BNO055 SCL | |
|  3.3V  | BNO055 VIN | |
|   GND  | BNO055 GND | |
|   GND  | BNO055 ADD | |
|  ---   | BNO055 INT | -not used- |
|  ---   | BNO055 RST | -not used- |
|  ---   | BNO055 BOOT | -not used- |


# Software

## Code
The main code is placed into [src/src/main.cpp](src/src/main.cpp)
We used PlatformIO to flash the ESP32, adapt it to your needs if you want.

## Control loop diagram

![Image](assets/ControlLoopDiagram.png)

## Program Flowchart
Unfortunately currently only in german.

![Image](assets/ProgramFlowchart.png)

**Both diagrams were created with the website [excalidraw.com](https://excalidraw.com).**

# Operating Instructions