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
2. Open the folder and open `beaconspamv1.ino` or `beaconspamv2` in Arduino IDE.
3. Select board from the `Tools` → `Board` → `AmebaD ARM (32-bits) Boards`.
   - It is `Ai-Thinker BW16 (RTL8720DN)`.
4. Select the port of that board.
5. Go to `Tools` → `Board` → `Auto Flash Mode` and select `Enable`.
6. Upload the code.
   - Firmware will start working immediately.

# Diff between vers1 and vers2
version1 (beaconspamv1.ino) It's made simpler, and to be honest, even taken from another project. <br>
It creates a lot of unnecessary output in the terminal. But it works. <br>

version2 (beaconspamv2.ino) More deeply developed, quite terminal output. <br>
It has additional tools to assist Deauth. For example code send 3 types of packg: <br>
Disassoc, Deauth, cts (0xFFFF) <br>
And thanks to this it works better even with slightly smart routers <br>

# Note
Power consumption ~150-160mA <br>
Code for educational purposes, good luck
