# STM32 & Arduino CAN Bus Communication Project

This project aims to establish reliable data communication between the **STM32F407** microcontroller and **Arduino Uno** using the **CAN Bus (Controller Area Network)** protocol. The system reads data from a sensor (ADXL345) on the STM32 side, packages it over the CAN line, and displays it on an LCD screen on the Arduino side.

## 📋 Project Overview

The project consists of two main nodes:
1.  **Transmitter - STM32F4:** Reads data from a 3-axis accelerometer (ADXL345) and broadcasts this data via CAN Bus.
2.  **Receiver - Arduino Uno:** Listens to the CAN Bus line, parses the incoming data packet, and presents the results to the user on a 16x2 LCD display.

## 🛠 Hardware Requirements

To replicate this project, you will need the following hardware:

* **STM32 Side:**
    * STM32F407G-DISC1 (Discovery Board)
    * SN65HVD230 CAN Transceiver Module
    * ADXL345 Accelerometer Sensor (I2C/SPI)
* **Arduino Side:**
    * Arduino Uno R3
    * MCP2515 CAN Bus Module (SPI Interface)
    * 16x2 Character LCD Display
    * 10k Potentiometer (for LCD contrast adjustment)
* **General:**
    * Jumper wires
    * Breadboard

## 🔌 Wiring Diagram (Pinout)
*Note: The **CAN_H** and **CAN_L** pins between the two CAN modules must be connected together, and the **GND** lines must be connected to a common ground.*
<img width="3603" height="1572" alt="project wiring diagram" src="https://github.com/user-attachments/assets/a41d413f-3a18-4369-99ce-dd5e2dc45d30" />

## 💻 Software and Libraries

### STM32 (Transmitter)
* **IDE:** STM32CubeIDE 
* **Framework:** HAL Library (Hardware Abstraction Layer)
* **Configuration:**
    * CAN1 active (Prescaler and Time Quanta settings should be configured for 500kbps or 250kbps).
    * I2C1 active (for ADXL345 reading).

### Arduino (Receiver)
* **IDE:** Arduino IDE
* **Libraries:**
    * `mcp_can` (Cory J. Fowler) - For controlling the MCP2515.
    * `LiquidCrystal` - For controlling the LCD display.

## 🚀 Installation and Usage

1.  **Make Connections:** Assemble the circuit on the breadboard according to the diagram and table above.
2.  **Upload STM32 Code:** Compile the project located in the `CAN_COM` folder and flash it to the STM32F4 board.
3.  **Upload Arduino Code:** Open the `.ino` file in the `can_com` folder using the Arduino IDE, install the necessary libraries, and upload it to the board.
4.  **Start the System:** Power up both boards.
5.  **Test:** When you move the ADXL345 sensor, you should see the values changing on the LCD screen connected to the Arduino.
