# 🚗 Motor Health Monitoring System

### MSP430-Based Predictive Maintenance Prototype

**Current Version:** v1.0
**Project Status:** 🟢 Functional Prototype (Under Active Development)

---

## 📖 Project Overview

The Motor Health Monitoring System is an embedded systems project developed using the MSP430 microcontroller. The system continuously monitors the operating condition of a DC motor by measuring temperature and current in real time. If abnormal operating conditions are detected, the system automatically stops the motor and displays the fault on a 16×2 LCD.

This project serves as the first phase of an Industrial Predictive Maintenance System and demonstrates sensor interfacing, ADC programming, LCD communication, motor control, and fault handling in embedded systems.

---

## ✨ Features Implemented (Version 1.0)

* MSP430-based embedded firmware architecture
* 16×2 LCD interfacing in 4-bit mode
* ADC driver implementation
* LM35 temperature monitoring
* ACS712 current monitoring
* Real-time sensor data display
* Push-button-controlled motor startup
* Automatic over-temperature detection and shutdown
* Automatic over-current detection and shutdown
* Continuous monitoring loop
* Modular driver-based firmware design

---

## 🛠️ Hardware Used

* MSP430 LaunchPad
* LM35 Temperature Sensor
* ACS712 Current Sensor
* 16×2 LCD Display
* L298N Motor Driver Module
* 12V DC Motor
* Push Button
* Breadboard and Jumper Wires
* 3 × Li-ion Cells

---

## ⚙️ System Workflow

```text
Power ON
   ↓
System Initialization
   ↓
Display "SYSTEM READY"
   ↓
Wait for Start Button
   ↓
Motor Starts
   ↓
Read Temperature and Current
   ↓
Display Sensor Values
   ↓
Check Safety Limits
   ↓
Fault?
 ├── No  → Continue Monitoring
 └── Yes → Stop Motor and Display Error
```

---

## 🛡️ Safety Limits

| Parameter   | Limit |
| ----------- | ----- |
| Temperature | 50 °C |
| Current     | 1 A   |

*Threshold values can be modified according to motor specifications.*

---

## 🧠 Embedded Concepts Demonstrated

* Embedded C Programming
* Modular Firmware Design
* GPIO Programming
* LCD Interfacing (4-bit Mode)
* ADC Programming
* Sensor Interfacing
* Bit Manipulation and Masking
* Driver Abstraction
* Fault Detection Systems
* State-Based Application Logic
* Motor Control Systems

---

## 🗺️ Development Roadmap

### ✅ Version 1.0 – Current Prototype

* Temperature Monitoring
* Current Monitoring
* Real-Time Display
* Automatic Fault Detection
* Automatic Motor Shutdown

### 🔄 Version 1.1 – Planned Improvements

* Timer-based non-blocking delays
* Improved LCD updates and user interface
* Sensor calibration routines
* Configurable safety thresholds
* Better fault handling and recovery mechanisms
* Status LEDs and buzzer indications

### 📡 Version 2.0 – Advanced Monitoring System

* UART communication with PC
* Serial monitoring using PuTTY
* Data logging and fault history
* Real-time sensor visualization
* EEPROM-based parameter storage
* PWM-based motor speed control

### 🌐 Version 3.0 – Industrial IoT Monitoring Platform

* Vibration sensor integration for bearing health monitoring
* ESP32 connectivity
* Cloud dashboard integration
* Mobile notifications and alerts
* Remote motor monitoring
* Predictive maintenance algorithms

---

## 🔮 Future Scope

The long-term objective of this project is to develop a smart Industrial Predictive Maintenance Platform capable of:

* Monitoring motor temperature, current, and vibration
* Detecting abnormal operating conditions
* Logging operational and fault history
* Sending remote alerts and notifications
* Providing wireless monitoring and cloud connectivity
* Predicting motor failures before breakdown occurs
* Supporting Industrial IoT applications and preventive maintenance strategies

---

## 📌 Note

This repository is under active development. Additional features, sensors, communication modules, PCB revisions, hardware upgrades, and software enhancements will continue to be added in future versions as the project evolves from a prototype into a complete Industrial IoT-based Predictive Maintenance System.

---

## 👨‍💻 Author

**Darshan**
B.Tech Electronics and Communication Engineering (ECE)

**Domains:** Embedded Systems • Predictive Maintenance • IoT • PCB Design • Motor Monitoring • MSP430 Development
