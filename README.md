ESP32 Color Sorting System

An embedded system for color detection and object sorting using an ESP32, TCS3200 color sensor, servo motors, and an I2C LCD. The system identifies red, yellow, and blue objects, controls the sorting mechanism, and tracks the number of objects detected for each color.

Features

•	Color Detection: Detects red, yellow, and blue objects using the TCS3200 color sensor.

•	Automatic Sorting: Controls two servo motors to sort objects based on the detected color.

•	Real-Time Display: Displays the detected color and object counts on a 16×2 I2C LCD.

•	UART Communication: Sends color detection results and object counts to a computer through Serial communication.

•	System Control: Supports START, STOP, and RESET commands through the serial interface.

Hardware Requirements

Component	Description

ESP32 Development Board	Main controller

TCS3200	Color detection sensor

Servo Motor × 2	Object sorting mechanism

16×2 I2C LCD	Color and count display

Power Supply	Suitable supply for the ESP32 and peripherals

Jumper Wires	Hardware connections

Software Requirements

•	Arduino IDE

•	ESP32 board package for Arduino IDE

•	ESP32Servo library

•	LiquidCrystal\_I2C library

•	Wire library (included with the Arduino platform)

Pin Configuration

TCS3200 Color Sensor

TCS3200 Pin	ESP32 GPIO

S0	GPIO 14

S1	GPIO 27

S2	GPIO 26

S3	GPIO 25

OUT	GPIO 34

Servo Motors

Component	ESP32 GPIO

Servo 1	GPIO 18

Servo 2	GPIO 19

I2C LCD

Configure the LCD with I2C address 0x27. The SDA and SCL connections depend on the ESP32 board configuration; the default pins on many ESP32 DevKit boards are GPIO 21 (SDA) and GPIO 22 (SCL).

Important: Connect all devices to a suitable power supply and ensure that the grounds are common. Do not power servo motors directly from the ESP32 3.3 V pin. Check the sensor's output voltage compatibility with the ESP32 before connecting it.

Installation and Setup

1\.	Install the Arduino IDE and configure the ESP32 board package.

2\.	Install the ESP32Servo and LiquidCrystal\_I2C libraries through the Arduino IDE Library Manager.

3\.	Connect the TCS3200, servo motors, and I2C LCD according to the pin configuration above.

4\.	Open ESP32\_Color\_Sorting.ino in the Arduino IDE.

5\.	Select the appropriate ESP32 board and COM port.

6\.	Upload the firmware to the ESP32.

7\.	Open Serial Monitor or the compatible host application at 115200 baud.

Serial Communication Protocol

The ESP32 sends comma-separated data when a recognized color is detected.

Output format:

COLOR,RED\_COUNT,YELLOW\_COUNT,BLUE\_COUNT,TOTAL

Example:

RED,1,0,0,1

BLUE,1,0,1,2

YELLOW,1,1,1,3

The serial interface accepts the following commands, each terminated by a newline character:

Command	Description

START	Resumes color detection and sorting

STOP	Pauses color detection and sorting

RESET	Resets all color counters to zero

How It Works

1\.	The TCS3200 sensor measures the frequency of the reflected-light signals for red, green, and blue filters.

2\.	The ESP32 reads the sensor output and compares the measured values against configured color thresholds.

3\.	The detected color determines the servo motor control sequence.

4\.	The LCD displays the current detected color and the accumulated counts.

5\.	The ESP32 transmits the detected color, individual counts, and total count through UART.

Color Calibration

Color detection depends on sensor characteristics, ambient lighting, object distance, and surface properties. The threshold values in the firmware may need to be calibrated for the actual hardware setup.

Adjust the following variables in ESP32\_Color\_Sorting.ino:

•	redMinR, redMaxR, redMinG, redMaxG, redMinB, redMaxB

•	yellowMinR, yellowMaxR, yellowMinG, yellowMaxG, yellowMinB, yellowMaxB

•	blueMinR, blueMaxR, blueMinG, blueMaxG, blueMinB, blueMaxB

Test multiple samples of each color under consistent lighting conditions before finalizing the thresholds.

Limitations

•	Color classification currently uses fixed threshold ranges.

•	Sorting accuracy depends on sensor calibration and environmental lighting.

•	The firmware uses blocking delays during servo operation, which can temporarily pause sensor readings and serial command processing.

•	UART output is designed for integration with a compatible host application, such as a C# desktop application.

Future Improvements

•	Implement non-blocking servo control using millis().

•	Improve classification accuracy through calibration and normalized RGB measurements.

•	Add a web-based monitoring dashboard.

•	Integrate MQTT for IoT monitoring and remote control.

•	Add object detection and sorting statistics.

License

This project is intended for educational and embedded systems development purposes. Add a license file if you plan to specify permissions for reuse and distribution.





