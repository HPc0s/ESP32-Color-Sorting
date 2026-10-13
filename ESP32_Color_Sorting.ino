#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== THIẾT BỊ =====
Servo servo1;
Servo servo2;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== BIẾN ĐẾM =====
int countRed = 0;
int countYellow = 0;
int countBlue = 0;

// ===== UART CONTROL =====
String cmd = "";
bool systemRunning = true;

// ===== CHÂN TCS3200 =====
const int S0 = 14;
const int S1 = 27;
const int S2 = 26;
const int S3 = 25;
const int out = 34;

// ===== GIÁ TRỊ RGB =====
int redR, greenR, blueR;

// ===== NGƯỠNG MÀU =====

// RED
int redMinR = 50;
int redMaxR = 190;

int redMinG = 250;
int redMaxG = 400;

int redMinB = 120;
int redMaxB = 290;

// YELLOW
int yellowMinR = 60;
int yellowMaxR = 210;

int yellowMinG = 90;
int yellowMaxG = 270;

int yellowMinB = 100;
int yellowMaxB = 260;

// BLUE
int blueMinR = 120;
int blueMaxR = 300;

int blueMinG = 90;
int blueMaxG = 330;

int blueMinB = 65;
int blueMaxB = 210;

// ===== SETUP =====
void setup() {

  Serial.begin(115200);

  // ===== SERVO =====
  servo1.attach(18);
  servo2.attach(19);

  servo1.write(180);
  servo2.write(180);

  // ===== TCS3200 =====
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);

  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(out, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
 
  // ===== LCD =====
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Color Detection");

  delay(1500);
  lcd.clear();



}

// ===== LOOP =====
void loop() {

  // ===== NHẬN LỆNH TỪ C# =====
  while (Serial.available()) {

    char c = Serial.read();

    if (c == '\n') {

      cmd.trim();

      if (cmd == "STOP") {
        systemRunning = false;
      }

      if (cmd == "START") {
        systemRunning = true;
      }

      if (cmd == "RESET") {

        countRed = 0;
        countYellow = 0;
        countBlue = 0;
      }

      cmd = "";
    }
    else {
      cmd += c;
    }
  }

  // ===== STOP HỆ THỐNG =====
  if (!systemRunning) {
    return;
  }

  // ===== ĐỌC RGB =====
  redR = getRedValue();
  greenR = getGreenValue();
  blueR = getBlueValue();

    // ===== LCD HIỂN THỊ =====

  // ----- DÒNG 1: MÀU ĐANG NHẬN DIỆN -----
  lcd.setCursor(0, 0);

  if (isRed(redR, greenR, blueR)) {

    lcd.print("Color: RED    ");
  }

  else if (isBlue(redR, greenR, blueR)) {

    lcd.print("Color: BLUE   ");
  }

  else if (isYellow(redR, greenR, blueR)) {

    lcd.print("Color: YELLOW ");
  }

  else {

    lcd.print("Color: NONE   ");
  }

  // ----- DÒNG 2: SỐ LƯỢNG -----
  lcd.setCursor(0, 1);

  lcd.print("R:");
  lcd.print(countRed);

  lcd.print(" B:");
  lcd.print(countBlue);

  lcd.print(" Y:");
  lcd.print(countYellow);

  lcd.print("   ");

  // ===== XỬ LÝ =====
  String detectedColor = "NONE";

  // ===== RED =====
  if (isRed(redR, greenR, blueR)) {


    servo1.write(90);

    delay(2500);

    servo1.write(180);

    detectedColor = "RED";

    countRed++;
  }

  // ===== BLUE =====
  else if (isBlue(redR, greenR, blueR)) {


    servo2.write(180);

    delay(2500);

    servo2.write(90);

    detectedColor = "BLUE";

    countBlue++;
  }

  // ===== YELLOW =====
  else if (isYellow(redR, greenR, blueR)) {


    servo1.write(180);
    servo2.write(90);

    delay(500);

    detectedColor = "YELLOW";

    countYellow++;
  }

  // ===== NONE =====
  else {

  }

  // ===== GỬI UART CHO C# =====
  if (detectedColor != "NONE") {

    int total = countRed + countYellow + countBlue;

    Serial.print(detectedColor);
    Serial.print(",");

    Serial.print(countRed);
    Serial.print(",");

    Serial.print(countYellow);
    Serial.print(",");

    Serial.print(countBlue);
    Serial.print(",");

    Serial.println(total);
  }

  delay(200);
}

// ===== HÀM ĐỌC RED =====
int getRedValue() {

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  return pulseIn(out, LOW, 30000);
}

// ===== HÀM ĐỌC GREEN =====
int getGreenValue() {

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  return pulseIn(out, LOW, 30000);
}

// ===== HÀM ĐỌC BLUE =====
int getBlueValue() {

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  return pulseIn(out, LOW, 30000);
}

// ===== KIỂM TRA RED =====
bool isRed(int r, int g, int b) {

  return (
           r >= redMinR && r <= redMaxR &&
           g >= redMinG && g <= redMaxG &&
           b >= redMinB && b <= redMaxB
         );
}

// ===== KIỂM TRA YELLOW =====
bool isYellow(int r, int g, int b) {

  return (
           r >= yellowMinR && r <= yellowMaxR &&
           g >= yellowMinG && g <= yellowMaxG &&
           b >= yellowMinB && b <= yellowMaxB
         );
} 

// ===== KIỂM TRA BLUE =====
bool isBlue(int r, int g, int b) {

  return (
           r >= blueMinR && r <= blueMaxR &&
           g >= blueMinG && g <= blueMaxG &&
           b >= blueMinB && b <= blueMaxB
         );
}