#include <Arduino.h>
#include <EEPROM.h>

// Define pins and variables
const int ledPins[] = {2, 3, 4, 5};      // Pins connected to the LEDs
const int ldrPins[] = {A0, A1, A2, A3};  // Pins connected to the LDRs
const int clockPin = 6;                  // Pin connected to the clock LED
int thresholds[4];                       // Thresholds for each LDR

String message;
String receivedMessage = "";
int receivedBits[4] = {0, 0, 0, 0};

// Number of readings per channel for threshold calibration
const int numReadings = 3;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("System Ready...");

  // Initialize thresholds based on current light conditions
  calibrateThresholds();

  if (EEPROM.read(0) == 1) {
    replayLastMessage();  // Replay the stored message
  } else {
    Serial.println("Please enter your message:");
  }
}

void loop() {
  if (Serial.available()) {
    message = Serial.readString();
    Serial.print("Message entered: ");
    Serial.println(message);

    saveMessageToEEPROM(message);
    EEPROM.write(0, 1); // Set replay flag
    Serial.println("Press the reset button to replay the message!");

    transmitAndReceiveMessage(); // Transmission and timing handled inside
    Serial.println("Transmission complete. Enter a new message:");
  }
}

void calibrateThresholds() {
  Serial.println("Calibrating LDRs...");

  // For each LDR channel, take several ON and OFF readings and calculate the average
  for (int i = 0; i < 4; i++) {
    int totalReadingOn = 0;
    int totalReadingOff = 0;

    // Calibrate ON (LED on) readings
    for (int j = 0; j < numReadings; j++) {
      digitalWrite(ledPins[i], HIGH);  // Turn on the LED
      int ldrValueOn = analogRead(ldrPins[i]);
      totalReadingOn += ldrValueOn;
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(" - ON Reading ");
      Serial.print(j + 1);
      Serial.print(": ");
      Serial.println(ldrValueOn);
      delay(100);  // Delay between readings
    }

    // Calibrate OFF (LED off) readings
    for (int j = 0; j < numReadings; j++) {
      digitalWrite(ledPins[i], LOW);  // Turn off the LED
      int ldrValueOff = analogRead(ldrPins[i]);
      totalReadingOff += ldrValueOff;
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(" - OFF Reading ");
      Serial.print(j + 1);
      Serial.print(": ");
      Serial.println(ldrValueOff);
      delay(100);  // Delay between readings
    }

    // Calculate the average ON and OFF readings
    int averageOn = totalReadingOn / numReadings;
    int averageOff = totalReadingOff / numReadings;

    // Set the threshold as the average of the ON and OFF averages
    thresholds[i] = (averageOn + averageOff) / 2;
    Serial.print("Channel ");
    Serial.print(i + 1);
    Serial.print(" - Calculated Threshold: ");
    Serial.println(thresholds[i]);
  }
}

void saveMessageToEEPROM(String msg) {
  for (int i = 0; i < msg.length(); i++) {
    EEPROM.write(i + 1, msg[i]);
  }
  EEPROM.write(msg.length() + 1, '\0');
}

void replayLastMessage() {
  char savedMessage[100];
  int i = 1;
  while (EEPROM.read(i) != '\0') {
    savedMessage[i - 1] = EEPROM.read(i);
    i++;
  }
  savedMessage[i - 1] = '\0';

  message = savedMessage;
  Serial.print("Replaying Last Message: ");
  Serial.println(message);

  transmitAndReceiveMessage(); // Timing handled inside
}

void transmitAndReceiveMessage() {
  Serial.print("Transmitting Message: ");
  Serial.println(message);

  receivedMessage = "";

  unsigned long startTime = millis(); // Start time measurement

  for (int charIndex = 0; charIndex < message.length(); charIndex++) {
    char character = message[charIndex];
    int ascii = int(character);

    int bits[4] = {
      ascii & 0x03,
      (ascii >> 2) & 0x03,
      (ascii >> 4) & 0x03,
      (ascii >> 6) & 0x03
    };

    Serial.print("Transmitting Character: ");
    Serial.print(character);
    Serial.print(" (ASCII: ");
    Serial.print(ascii);
    Serial.println(")");

    for (int i = 0; i < 4; i++) {
      receivedBits[i] = 0;
    }

    for (int bitIndex = 0; bitIndex < 2; bitIndex++) {
      for (int channel = 0; channel < 4; channel++) {
        digitalWrite(ledPins[channel], (bits[channel] >> bitIndex) & 1);
      }

      digitalWrite(clockPin, HIGH);
      delay(5);

      for (int channel = 0; channel < 4; channel++) {
        int value = analogRead(ldrPins[channel]);
        Serial.print("Channel ");
        Serial.print(channel + 1);
        Serial.print(" - LDR Value: ");
        Serial.println(value);

        if (value > thresholds[channel]) {
          receivedBits[channel] |= (1 << bitIndex);
        }
      }

      digitalWrite(clockPin, LOW);
      delay(5);
    }

    int reconstructedAscii = (receivedBits[3] << 6) | (receivedBits[2] << 4) | (receivedBits[1] << 2) | receivedBits[0];
    Serial.print("Received Character: ");
    Serial.print(char(reconstructedAscii));
    Serial.print(" (ASCII: ");
    Serial.print(reconstructedAscii);
    Serial.println(")");

    receivedMessage += char(reconstructedAscii);
  }

  unsigned long endTime = millis(); // End time measurement

  Serial.print("Received Message: ");
  Serial.println(receivedMessage);

  Serial.print("Time Taken: ");
  Serial.print(endTime - startTime);
  Serial.println(" ms");

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
