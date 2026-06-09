# InvertedPendulum
## Concept Idea
A small robot capable of balancing itself so that it remains upright despite having only two wheels.
The motivation behind this is a school project, with the main focus on self-regulating control loops. This robot was our idea for the project.

---

## Language
- Primary Documentation (English): [README.md](README.md)
- German Version: [README.de.md](README.de.md)

---

## Table of Contents

1. [Hardware](#hardware)
   1. [Requirements](#requirements)
   2. [Pinout](#pinout)
   3. [Schematic](#schematic)
   4. [Breadboard](#breadboard)
2. [Software](#software)
   1. [Code](#code)
   2. [Control Loop Diagram](#control-loop-diagram)
   3. [Program Flowchart](#program-flowchart)
3. [Operating Instructions](#operating-instructions)

---

## To Do

- [x] Create a CAD model of the robot
- [x] Make component connections more stable (based on 3 prototypes to ensure maximum reliability, see [Prototypes](/assets/Prototypes.jpg))
- [x] Design for resource efficiency
- [x] Use fewer components for easier assembly and more stable holding
- [x] Fixed sensor mounting on the PCB for stable readings
- [x] Design non-slip tires
- [x] Reuse PCB from an old project to recycle resources
- [x] Write code to keep the robot upright
- [ ] Improve code to enable calibration via smartphone
   - [ ] Connect via BLE to an app **OR**
   - [ ] Open a web server on the local Wi-Fi network
- [ ] Prevent slight tilting
   - Could be related to PID control (unable to compensate for small angles¹)
- [ ] Consider additional parameters in control
   - For example, preventing the robot from covering large distances (or doing so intentionally). Additionally, unintended rotation could be compensated for or controlled.

**Notes:**
1. The code prevents the system from reacting to small angles. However, testing without this hysteresis also failed, as a ~1-2 degree angle only results in a few watts of output to the motor, which is insufficient for compensation. An adjustment should be written to react more aggressively to small values.

---

# Hardware
## Requirements

- 2 x geared motors ~600 RPM
   - 2 x M3 screws, 7.3 mm (flat head/countersunk¹)
- ESP32
- BNO055 9DoF Sensor²
- H-Bridge (e.g., HW-095 L298N)
- CAD 3D-printed parts ([see here](TechDrawRework.pdf))³

**Notes:**
1. The screw type depends on the geared motor used.
2. We used this [self-created wiki](https://github.com/Leolion2023/BNO055) because the Bosch documentation is difficult to understand.
3. If you have issues displaying the technical drawing, please download the file and open it in a modern PDF viewer.

---
## Pinout

| ESP32 Pin | Function          | Additional Info               |
|-----------|-------------------|--------------------------------|
| GPIO16    | H-Bridge IN1      | Used for Motor 1               |
| GPIO17    | H-Bridge IN2      | Used for Motor 1               |
| GPIO23    | H-Bridge ENA      | Used for Motor 1 (PWM)         |
| GPIO18    | H-Bridge IN3      | Used for Motor 2               |
| GPIO19    | H-Bridge IN4      | Used for Motor 2               |
| GPIO33    | H-Bridge ENB      | Used for Motor 2 (PWM)         |
| GPIO21    | BNO055 SDA        |                                |
| GPIO22    | BNO055 SCL        |                                |
| 3.3V      | BNO055 VIN        |                                |
| GND       | BNO055 GND        |                                |
| GND       | BNO055 ADD        |                                |
| ---       | BNO055 INT        | *Not used*                     |
| ---       | BNO055 RST        | *Not used*                     |
| ---       | BNO055 BOOT       | *Not used*                     |

---
## Schematic
![Image](assets/Schematic.png)

---
## Breadboard
![Image](assets/BreadboardView.png)

---
# Software

---
## Code
The main code is located in [src/src/main.cpp](src/src/main.cpp).
We used PlatformIO to flash the ESP32. Adjust as needed to fit your requirements.

---
## Control Loop Diagram
![Image](assets/ControlLoopDiagram.png)

---
## Program Flowchart
Currently only available in German.

![Image](assets/ProgramFlowchart.png)

**Both diagrams were created using [excalidraw.com](https://excalidraw.com).**

---
# Operating Instructions

---
### Electrical Startup

The robot requires two separate power supplies: one for logic voltage and one for motor voltage. The logic voltage can be provided via the ESP32’s USB-C port, the VIN pin (3.3V–5V), or the 3.3V pin. However, we recommend using the connector on the PCB directly next to the ESP32. The motor voltage must be connected either directly to the H-Bridge at the 12V and GND terminals or to the main power supply on the PCB.

Since you will likely need to calibrate the PID values, the easiest way is to connect the ESP32 to a computer with either PlatformIO or the Arduino IDE installed:

**For PlatformIO:**
1. Install PlatformIO on your laptop/PC/etc.
2. Install the custom library for BNO055 (linked above) or the official Bosch library (no guarantee of updates).
3. Adjust the code as described in the next section.
4. Place the robot on a level surface so the wheels are not touching the ground (for initial calibration).
5. Connect the ESP32 and upload the code.

**For Arduino IDE:**
1. Install Arduino IDE.
2. Install the library ([either via Arduino Library Manager or as a file](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/)).
3. Adjust the code as described in the next section.
4. Place the robot on a level surface so the wheels are not touching the ground (for initial calibration).
5. Connect the ESP32 and upload the code.

---
### Code Adjustments

There are a few simple adjustments to be made. At the top, you’ll find the three PID constants, which need to be tuned for your robot. The `constraint` limits the maximum output of the PID controller. Typically, 255.0 is reasonable, as it is the maximum PWM value. The `minConstraint` prevents the motors from spinning at low power, as rapid toggling of the power supply can damage the motors. The `inverted` variable controls the motor rotation direction. If the robot regulates in the wrong direction, change it to `-1.0` or `1.0`.

---
### Hardware Information

To build the robot, you can simply use the technical drawings. Minor adjustments to the variables in the CAD files can improve the connections but may also introduce errors.

**Notes:**
1. We currently have the motors connected in parallel to one channel of the H-Bridge, as they otherwise rotated differently. This could also be balanced by code that reads the other dimensions of the BNO055 and attempts to prevent rotation.