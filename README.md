# CAN-Based Intelligent Bus Accident Prevention and Automatic Braking System

## Overview
An embedded automotive safety system using **LPC2129 ARM7 microcontrollers** and **CAN communication** to detect obstacles, monitor vehicle speed, and automatically control motor speed to prevent collisions.

## Hardware Components
- LPC2129 ARM7 microcontrollers (3 ECUs)
- MCP2551 CAN transceivers
- HC-SR04 ultrasonic sensor
- 10K potentiometer
- L293D motor driver
- DC motor
- 16x2 LCD
- Buzzer and LED

## ECU Architecture

| ECU | Function |
|---|---|
| ECU1 | Detects obstacles using an ultrasonic sensor and transmits distance data. |
| ECU2 | Receives distance and speed data, controls motor speed, and provides alerts. |
| ECU3 | Reads simulated vehicle speed using a potentiometer and transmits speed data. |

## CAN Communication

| CAN ID | Data |
|---|---|
| 0x100 | Obstacle distance |
| 0x200 | Vehicle speed |
| 0x300 | Brake status (planned extension) |

## Working Principle
1. ECU1 measures the distance between the vehicle and an obstacle.
2. ECU3 reads the simulated vehicle speed.
3. Both ECUs transmit their data through the CAN bus.
4. ECU2 processes the received information and adjusts motor speed according to obstacle distance.
5. The LCD displays system information, while the buzzer and LED provide warnings when an obstacle is detected nearby.

## Software & Tools
- Embedded C
- Keil µVision 4
- LPC2129 ARM7
- CAN 2.0A communication

## Future Enhancement
Integration of a fourth ECU for driver drowsiness detection using an IR eye-blink sensor.

---
**Project Domain:** Embedded Systems | Automotive Safety | CAN Communication
