# Smart Parking System 🚗🅿️

A real-time, hybrid sensor-based smart parking management system prototype built with Arduino. This project was developed for a university project exhibition to demonstrate an efficient, automated approach to monitoring parking slot availability and guiding drivers using an OLED interface and LED indicators.

## 👥 Team 3 Details
* **Deepak Yadav** (25BCE10935) - *Team Leader*
* **Divyanshu Shrivastava** (25BCE11220)
* **Ashutosh Pavaiya** (25BCE10965)
* **Satwik Singh** (25BCE10849)
* **Satyam Choudhary** (25BCE11095)
* **Supervisor:** Dr. S. Poonkuntran

## ✨ Key Features
* **Hybrid Sensor Architecture:** Utilizes a combination of an HC-SR04 Ultrasonic sensor and IR Obstacle sensors to ensure reliable vehicle detection.
* **Software Debounce (Glitch Filter):** Custom algorithmic filter applied to the ultrasonic sensor to eliminate false positives caused by echo overlap or angled vehicle surfaces.
* **Dynamic OLED Dashboard:** A 0.96" I2C OLED display dynamically rotates through available slots and displays a "PARKING FULL" alert when capacity is reached.
* **Visual LED Indicators:** Dedicated solid-state LED indicators for each slot (Glowing = Available, Off = Occupied).

## 🛠️ Hardware Requirements
* 1x Arduino Uno R3
* 1x HC-SR04 Ultrasonic Sensor (Slot 1)
* 3x Flying Fish IR Obstacle Sensors (Slots 2, 3, 4)
* 1x SSD1306 0.96" OLED Display (I2C)
* 4x Red LEDs
* 4x 220Ω Resistors
* Breadboard & Jumper Wires (M-M, F-M)

## 🔌 Circuit Wiring Details

| Component | Pin / Terminal | Arduino Pin |
| :--- | :--- | :--- |
| **HC-SR04 (Slot 1)** | TRIG | D6 |
| | ECHO | D7 |
| **IR Sensor (Slot 2)** | OUT | D3 |
| **IR Sensor (Slot 3)** | OUT | D4 |
| **IR Sensor (Slot 4)** | OUT | D5 |
| **LED 1 (Slot 1)** | Anode (+) | D8 |
| **LED 2 (Slot 2)** | Anode (+) | D9 |
| **LED 3 (Slot 3)** | Anode (+) | D10 |
| **LED 4 (Slot 4)** | Anode (+) | D11 |
| **OLED Display** | SDA | A4 |
| | SCL | A5 |

*(Note: All sensor VCC pins are connected to 5V, and all GND pins to Arduino Ground. LEDs are grounded through 220Ω resistors).*

## 💻 Software & Libraries
* **Arduino IDE** (v2.0+)
* **U8g2 Library** by oliver (for optimized OLED screen drawing)
* **Wire.h** (built-in I2C protocol communication)

## 🚀 Installation & Setup
1. Clone this repository or download the `.ino` source code file.
2. Open the file in the **Arduino IDE**.
3. Go to **Sketch > Include Library > Manage Libraries**, search for `U8g2`, and install it.
4. Ensure your circuit is wired according to the pinout table above.
5. Connect your Arduino Uno to your PC.
6. Select the correct Board (`Arduino Uno`) and Port under the **Tools** menu.
7. Click **Upload**.

## ⚙️ How It Works
1. **Sensor Polling:** The Arduino continuously reads the distance from the ultrasonic sensor and the digital HIGH/LOW states of the IR sensors.
2. **Glitch Filtering:** If the ultrasonic sensor loses sight of a vehicle, the system double-checks 5 times before officially marking the slot as empty, preventing screen flickering.
3. **Logic Update:** The system updates an internal array of available slots.
4. **Hardware Output:** LEDs for empty slots are driven `HIGH`. LEDs for occupied slots are driven `LOW`.
5. **UI Refresh:** The OLED screen refreshes to display total available slots and cycles through the specific gate numbers of empty spaces.
