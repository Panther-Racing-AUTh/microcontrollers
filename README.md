# Microcontrollers Project

## Overview

This project is designed to showcase the usage of microcontrollers, specifically the ESP32 microcontroller, to send and receive data over the Controller Area Network (CAN) protocol. The primary focus of this project is to demonstrate how to interface the ESP32 with external devices, parse incoming data, and transmit it using CAN communication. The project utilizes the `CAN.h` library for ESP32 to facilitate CAN communication.

## Project Structure

- **Directory**: `esp32-devkitc-v4--lambda/`
  - **File**: `microcontrollers.ino`

## Getting Started

To set up and run this project, follow these steps:

1. **Hardware Setup**: Ensure you have an ESP32 development board (e.g., ESP32 DevKitC) and the required peripherals, such as a USB cable for programming and Serial communication.

2. **Software Setup**:

   - Install the Arduino IDE: If you haven't already, install the Arduino IDE on your computer.
   
   - Install the ESP32 Board Support Package (BSP): Follow the instructions on the [official ESP32 Arduino Core GitHub repository](https://github.com/espressif/arduino-esp32) to install the ESP32 BSP in the Arduino IDE.

3. **Upload the Code**:

   - Open the `microcontrollers.ino` file in the Arduino IDE located in the "esp32-devkitc-v4--lambda/" directory.

   - Make sure to select the correct ESP32 board from the "Tools" > "Board" menu.

   - Connect your ESP32 to your computer using a USB cable and select the appropriate COM port in the "Tools" > "Port" menu.

   - Upload the code to the ESP32 by clicking the "Upload" button (arrow icon) in the Arduino IDE.

4. **Monitor Serial Output**:

   - Open the Serial Monitor in the Arduino IDE by clicking "Tools" > "Serial Monitor."

   - Set the baud rate to 9600 and ensure that you can view the serial output for debugging and monitoring.

5. **Interact with the Project**:

   - The project is designed to receive data from an external source via Serial2, parse it, and transmit the parsed data using the CAN protocol to the specified CAN address (0x12 in this case).

   - Follow the comments within the code to understand the data format and how it is processed.

6. **Customize and Extend**:

   - You can modify and extend the code to fit your specific requirements or integrate it into larger projects involving microcontrollers and CAN communication.

7. **Cleanup**:

   - Remember to disconnect and power off the hardware when not in use.

## Dependencies

- [CAN.h](https://github.com/sandeepmistry/arduino-CAN): This library is used to enable CAN communication on the ESP32 microcontroller.

## License

This project is open-source and available under the [MIT License](LICENSE). Feel free to use, modify, and distribute this code as needed.

## Author

This project was developed by Panther Racing AUTh.

