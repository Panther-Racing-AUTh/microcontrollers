#include <CAN.h>

#define RXD2 16
#define TXD2 17

#define CAN_ADDRESS 0x12

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  while(!Serial2);

  Serial.println("CAN Sender");

  if (!CAN.begin(1000E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

volatile int toInt(byte b) {
  if (b < '0' || b > '9') {
    return -1;
  }

  return b - '0';
}

int value = 0;
int bufferCounter = -1;
int incomingByte;
int incomingDigit;

uint8_t* canBytes = new uint8_t[2];
char str[3];

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0) {
    incomingByte = Serial2.read();
//    sprintf(str, "%02x\0", (char)incomingByte);
//    Serial.println(str);

    if (bufferCounter == -1 && incomingByte != '\n') {
      return;
    }

    if (bufferCounter == -1 && incomingByte == '\n') {
      bufferCounter = 0;
      return;
    }

    if (bufferCounter == 0 || bufferCounter == 1 || bufferCounter == 3) {
      incomingDigit = toInt(incomingByte);
//      Serial.print("Digit: ");
//      Serial.println(incomingDigit);

      if (incomingDigit < 0) {
        bufferCounter = -1;
        return;
      }

      switch (bufferCounter) {
        case 0:
          for (int i = 0; i < incomingDigit; i++) value += 100;
          break;
       case 1:
          for (int i = 0; i < incomingDigit; i++) value += 10;
          break;
       case 3:
          for (int i = 0; i < incomingDigit; i++) value += 1;
          break;
      }

      bufferCounter++;
      return;
    }

    if (bufferCounter == 2 && incomingByte == '.') {
      bufferCounter++;
      return;
    }

    if (bufferCounter == 4 && incomingByte == '\r') {
      bufferCounter++;
      return;
    }

    if (bufferCounter == 5 && incomingByte == '\n') {
      Serial.print("Value => ");
      Serial.println(value);

      canBytes[0] = (char)((value >> 0) & 0xff);
      canBytes[1] = (char)((value >> 8) & 0xff);

      sprintf(str, "%02x\0", (char)canBytes[0]);
      Serial.println(str);
      sprintf(str, "%02x\0", (char)canBytes[1]);
      Serial.println(str);

      sprintf(str, "%04x\0", value);
      Serial.println(str);

      CAN.beginPacket(CAN_ADDRESS);
      CAN.write(canBytes, 2);
      CAN.endPacket();

      value = 0;
      bufferCounter = 0;
      return;
    }

    value = 0;
    bufferCounter = -1;
  }
}
