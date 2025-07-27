# OptiComm - Arduino-Based Optical Message Transmission

**OptiComm** is an Arduino-based system that demonstrates one-way optical communication using visible light. The system takes a hardcoded input message, converts it into binary, transmits it using blinking LEDs, and receives it through LDR sensors. The light signals are then decoded back into the original message.

This project was developed as part of a course assignment at **IIT Gandhinagar**, and it showcases the fundamental principles of data encoding, signal transmission, and decoding using light as a medium.

---

## 📌 Features

- 🔠 Hardcoded text message is converted into binary format
- 💡 Each bit is transmitted via LED blinks (ON for 1, OFF for 0)
- 📥 LDR sensors detect the light signal and convert it back into binary
- 🔁 Binary stream is decoded back into the original message
- 🛠 Built using Arduino Uno and basic electronic components
- 📄 Includes source code and full project report

---

## 🧰 Hardware Components Used

- Arduino Uno
- LEDs (for transmission)
- LDRs (for reception)
- Resistors (typically 10kΩ for LDR voltage divider)
- Breadboard & jumper wires

---

## 🚀 How It Works

1. A message is predefined in the Arduino code.
2. The message is converted into its binary representation (ASCII).
3. The LED transmits each bit by turning ON (for 1) or OFF (for 0).
4. LDRs detect the blinking pattern based on changes in light intensity.
5. The Arduino decodes the light signal and reconstructs the original message.
6. The decoded message is printed to the Arduino IDE console (monitor tab).

---

## ⚙️ Setup Instructions

1. Connect the hardware as per the diagram in the project report or code comments.
2. Upload the provided `.ino` file to your Arduino board.
3. Observe the message being transmitted via LED blinks.
4. Open the Arduino console to view the decoded output message after reception.

---

## ⚠️ Important Note on Thresholds

> The LDR readings depend heavily on **ambient lighting conditions**, LED brightness, and hardware setup.  
> You may need to **adjust the light detection threshold** in the code to distinguish between "light ON" and "light OFF" states reliably.

Look for a `threshold` value in the code and test with `analogRead()` to determine a stable cutoff point for your environment.

---

## 📁 Files Included

- `Elec_Project.ino` – Arduino source code
- `Elec Project Report Ujas.pdf` – Full project explanation and setup

---

## 🎓 Project Information

Course : Principles and Applications of Electrical Engineering
This project was developed as part of a course at **IIT Gandhinagar**, aimed at building a basic light-based communication system using embedded hardware. It focuses on real-world problem-solving, system coordination, and application of theoretical concepts in a practical setting.

---
