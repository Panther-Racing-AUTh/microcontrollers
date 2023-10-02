#include <CAN.h> // Include the CAN library

#define RXD2 16 // Define the receive pin for Serial2
#define TXD2 17 // Define the transmit pin for Serial2

#define CAN_ADDRESS 0x12 // Set the CAN address for communication

void setup() {
  Serial.begin(9600); // Initialize the Serial communication with the PC
  while (!Serial); // Wait for the Serial connection to be established

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Initialize Serial2 for communication with another device
  while (!Serial2); // Wait for Serial2 to be ready

  Serial.println("CAN Sender"); // Print a message to Serial monitor

  if (!CAN.begin(1000E3)) { // Initialize the CAN bus at a baud rate of 1,000,000 bps
    Serial.println("Starting CAN failed!"); // Print an error message if initialization fails
    while (1); // Enter an infinite loop if initialization fails
  }
}

// A function to convert a byte to an integer
volatile int toInt(byte b) {
  if (b < '0' || b > '9') {
    return -1;
  }

  return b - '0';
}

int value = 0; // Variable to store the parsed value
int bufferCounter = -1; // A counter to keep track of the incoming data buffer
int incomingByte; // Variable to store the incoming byte
int incomingDigit; // Variable to store the parsed digit

uint8_t* canBytes = new uint8_t[2]; // Create an array to hold two bytes of CAN data
char str[3]; // Create a character array to hold strings

void loop() {
  if (Serial2.available() > 0) { // Check if there is data available on Serial2
    incomingByte = Serial2.read(); // Read the incoming byte
    //    sprintf(str, "%02x\0", (char)incomingByte);
//    Serial.println(str);

    if (bufferCounter == -1 && incomingByte != '\n') {
      return; // Ignore data until a newline character is received
    }

    if (bufferCounter == -1 && incomingByte == '\n') {
      bufferCounter = 0; // Reset the buffer counter when a newline is received
      return;
    }

    if (bufferCounter == 0 || bufferCounter == 1 || bufferCounter == 3) {
      incomingDigit = toInt(incomingByte); // Convert the incoming byte to an integer digit
//      Serial.print("Digit: ");
//      Serial.println(incomingDigit);
      if (incomingDigit < 0) {
        bufferCounter = -1; // Reset the buffer counter if an invalid digit is received
        return;
      }

      switch (bufferCounter) {
        case 0:
          for (int i = 0; i < incomingDigit; i++) value += 100; // Add hundreds to the value
          break;
       case 1:
          for (int i = 0; i < incomingDigit; i++) value += 10; // Add tens to the value
          break;
       case 3:
          for (int i = 0; i < incomingDigit; i++) value += 1; // Add ones to the value
          break;
      }

      bufferCounter++; // Increment the buffer counter
      return;
    }

    if (bufferCounter == 2 && incomingByte == '.') {
      bufferCounter++; // Move to the next buffer position when a dot is received
      return;
    }

    if (bufferCounter == 4 && incomingByte == '\r') {
      bufferCounter++; // Move to the next buffer position when a carriage return is received
      return;
    }

    if (bufferCounter == 5 && incomingByte == '\n') {
      Serial.print("Value => ");
      Serial.println(value); // Print the parsed value to the Serial monitor

      canBytes[0] = (char)((value >> 0) & 0xff); // Extract and store the low byte of the value
      canBytes[1] = (char)((value >> 8) & 0xff); // Extract and store the high byte of the value

      sprintf(str, "%02x\0", (char)canBytes[0]); // Convert the low byte to a hexadecimal string
      Serial.println(str); // Print the hexadecimal string to the Serial monitor
      sprintf(str, "%02x\0", (char)canBytes[1]); // Convert the high byte to a hexadecimal string
      Serial.println(str); // Print the hexadecimal string to the Serial monitor

      sprintf(str, "%04x\0", value); // Convert the entire value to a 4-digit hexadecimal string
      Serial.println(str); // Print the hexadecimal string to the Serial monitor

      CAN.beginPacket(CAN_ADDRESS); // Begin a CAN packet with the specified address
      CAN.write(canBytes, 2); // Write the CAN data bytes
      CAN.endPacket(); // End the CAN packet

      value = 0; // Reset the value
      bufferCounter = 0; // Reset the buffer counter
      return;
    }

    value = 0; // Reset the value
    bufferCounter = -1; // Reset the buffer counter
  }
}
