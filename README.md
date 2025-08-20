# BW16-beacon
Sending beacon frames on BW16 (RTL8720DN)

# Hardware Requirements
- Ai-Thinker BW16 RTL8720DN Development Board

# Setup
1. Download Arduino IDE from [here](https://www.arduino.cc/en/software) according to your Operating System.
2. Install it.
3. Go to `File` → `Preferences` → `Additional Boards Manager URLs`.
4. Paste the following link :
   
   ```
   https://github.com/ambiot/ambd_arduino/raw/master/Arduino_package/package_realtek_amebad_index.json
   ```
5. Click on `OK`.
6. Go to `Tools` → `Board` → `Board Manager`.
7. Search `Realtek Ameba Boards (32-Bits ARM Cortex-M33@200MHz)` by `Realtek`.
8. Install it.
9. Restart the Arduino IDE.
10. Done!

# Install
1. Download or Clone the Repository.
2. Open the folder and open `BW16_beacon.ino` in Arduino IDE.
3. Select board from the `Tools` → `Board` → `AmebaD ARM (32-bits) Boards`.
   - It is `Ai-Thinker BW16 (RTL8720DN)`.
4. Select the port of that board.
5. Go to `Tools` → `Board` → `Auto Flash Mode` and select `Enable`.
6. Upload the code.
   - Firmware will start working immediately.

# Note
According to measurements, the board with my code consumes 165-205mA.
Code for educational purposes, good luck
