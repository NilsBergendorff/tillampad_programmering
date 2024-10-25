/*
* Name: clock and temp project
* Author: Nils Bergendorff
* Date: 2024-10-11
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <U8glib.h>
#include <Servo.h>

// Init constants
int ThermistorPin = A0;
int ledPin = 13; 

// Init global variables
Servo myServo;
int previousMinute = -1; 

// Construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {
  // Init communication
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  myServo.attach(9);

  // Init Hardware
  pinMode(ledPin, OUTPUT);

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont);
}

void loop() {
  DateTime now = rtc.now();
  int currentMinute = now.minute();

  if (currentMinute != previousMinute) {
    blinkLED();
    previousMinute = currentMinute;
  }

  
  oledWrite(getTime(), getTemp());

  delay(100);
}

/*
 * Blinka LED-lampan två gånger när det blir en ny minut
 */
void blinkLED() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

/*
 * This function reads time from a DS3231 module and packages the time as a String
 * Parameters: Void
 * Returns: time in hh:mm:ss as String
 */
String getTime() {
  DateTime now = rtc.now();
  String timeString = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  return timeString;
}

/*
 * This function reads an analog pin connected to an analog temperature sensor and calculates the corresponding temp
 * Parameters: Void
 * Returns: temperature as String
 */
String getTemp() {
  int Vo;
  float R1 = 10000;  // Value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);  // Calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // Temperature in Kelvin
  T = T - 273.15;  // Convert Kelvin to Celsius
  return "Temp: " + String(T, 1) + " C";
}

/*
 * This function takes a string and draws it to an OLED display
 * Parameters: - text1: String to write to display
 * Returns: void
 */
void oledWrite(String text1, String text2) {
  u8g.firstPage();
  do {
    u8g.drawStr(0, 20, text1.c_str());
    u8g.drawStr(0, 40, text2.c_str());
  } while (u8g.nextPage());
}

/*
 * Takes a temperature value and maps it to corresponding degree on a servo
 * Parameters: - value: temperature
 * Returns: void
 */
/*
void servoWrite(float value) {
  int angle = map(value, 0, 50, 0, 180);
  angle = constrain(angle, 0, 180);
  myServo.write(angle);
}
*/