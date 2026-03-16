/*
 * ============================================================
 * Voice-Activated Home Automation System
 * ============================================================
 * Author      : Bhoomika Sathish Rao (N1195078)
 * Institution : Nottingham Trent University
 * Degree      : MSc Engineering Electronics
 * Supervisor  : Rahma Hammedi
 * Year        : 2024
 *
 * Description:
 * A voice-activated embedded home automation system using
 * Arduino Mega, HC-05 Bluetooth module, PIR motion sensor,
 * capacitive touch sensor, RGB LED, and I2C LCD display.
 * Voice commands are received via a smartphone app (Arduino
 * Bluetooth Controller) and processed locally on the Arduino.
 *
 * Pin Mapping (Arduino Mega):
 *   Pin 10, 11  → HC-05 Bluetooth (SoftwareSerial RX, TX)
 *   Pin 45      → LED (White/Main light)
 *   Pin 40      → RGB Red
 *   Pin 42      → RGB Green
 *   Pin 44      → RGB Blue
 *   Pin 2       → PIR Motion Sensor
 *   Pin 3       → Touch Sensor
 *   I2C (SDA/SCL) → 16x2 LCD Display
 * ============================================================
 */

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── BLUETOOTH SETUP ─────────────────────────────────────────
SoftwareSerial BTSerial(10, 11);  // RX, TX

// ── LCD SETUP (I2C address 0x27, 16 cols, 2 rows) ───────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── PIN DEFINITIONS ─────────────────────────────────────────
const int LED_PIN    = 45;   // Main LED / Light
const int RGB_RED    = 40;   // RGB Red channel
const int RGB_GREEN  = 42;   // RGB Green channel
const int RGB_BLUE   = 44;   // RGB Blue channel
const int PIR_PIN    = 2;    // PIR Motion Sensor
const int TOUCH_PIN  = 3;    // Capacitive Touch Sensor
const int FAN_PIN    = 7;    // Fan relay

// ── STATE VARIABLES ─────────────────────────────────────────
bool lightOn    = false;
bool fanOn      = false;
bool motionMode = true;   // auto motion detection active

// ── FUNCTION PROTOTYPES ─────────────────────────────────────
void setRGB(int r, int g, int b);
void processCommand(String command);
void handleMotionSensor();
void handleTouchSensor();
void displayMessage(String line1, String line2);

// ============================================================
// SETUP
// ============================================================
void setup() {
  // Pin modes
  pinMode(LED_PIN,   OUTPUT);
  pinMode(RGB_RED,   OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE,  OUTPUT);
  pinMode(FAN_PIN,   OUTPUT);
  pinMode(PIR_PIN,   INPUT);
  pinMode(TOUCH_PIN, INPUT);

  // Ensure all outputs start OFF
  digitalWrite(LED_PIN,   LOW);
  digitalWrite(FAN_PIN,   LOW);
  setRGB(0, 0, 0);

  // Serial monitor for debugging
  Serial.begin(9600);

  // Bluetooth communication at 9600 baud
  BTSerial.begin(9600);

  // LCD initialisation
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Awaiting cmd...");

  Serial.println("Voice Home Automation System Started");
  Serial.println("Waiting for Bluetooth commands...");

  delay(2000);
  lcd.clear();
}

// ============================================================
// MAIN LOOP
// ============================================================
void loop() {

  // ── READ BLUETOOTH COMMAND ──────────────────────────────
  if (BTSerial.available()) {
    // Read incoming data until newline character
    String data = BTSerial.readStringUntil('\n');
    data.trim();  // Remove leading/trailing whitespace and newlines

    // Debug: print received command to Serial Monitor
    Serial.print("Received: ");
    Serial.println(data);

    // Process the command
    processCommand(data);
  }

  // ── HANDLE SENSORS ──────────────────────────────────────
  handleMotionSensor();
  handleTouchSensor();

  delay(100);  // Small delay for stability
}

// ============================================================
// PROCESS VOICE COMMANDS
// Matches received Bluetooth text to predefined actions
// ============================================================
void processCommand(String data) {

  // ── LIGHT CONTROL ───────────────────────────────────────
  if (data.equalsIgnoreCase("turn on light")) {
    digitalWrite(LED_PIN, HIGH);
    lightOn = true;
    displayMessage("Light ON", "Voice Command");
    Serial.println("Light turned ON");
  }
  else if (data.equalsIgnoreCase("turn off light")) {
    digitalWrite(LED_PIN, LOW);
    lightOn = false;
    displayMessage("Light OFF", "Voice Command");
    Serial.println("Light turned OFF");
  }

  // ── FAN CONTROL ─────────────────────────────────────────
  else if (data.equalsIgnoreCase("turn on fan")) {
    digitalWrite(FAN_PIN, HIGH);
    fanOn = true;
    displayMessage("Fan ON", "Voice Command");
    Serial.println("Fan turned ON");
  }
  else if (data.equalsIgnoreCase("turn off fan")) {
    digitalWrite(FAN_PIN, LOW);
    fanOn = false;
    displayMessage("Fan OFF", "Voice Command");
    Serial.println("Fan turned OFF");
  }

  // ── RGB LED CONTROL ─────────────────────────────────────
  else if (data.equalsIgnoreCase("turn on red")) {
    setRGB(255, 0, 0);
    displayMessage("RGB: Red", "LED Active");
    Serial.println("Red LED is ON");
  }
  else if (data.equalsIgnoreCase("turn on green")) {
    setRGB(0, 255, 0);
    displayMessage("RGB: Green", "LED Active");
    Serial.println("Green LED is ON");
  }
  else if (data.equalsIgnoreCase("turn on blue")) {
    setRGB(0, 0, 255);
    displayMessage("RGB: Blue", "LED Active");
    Serial.println("Blue LED is ON");
  }
  else if (data.equalsIgnoreCase("turn off rgb")) {
    setRGB(0, 0, 0);
    displayMessage("RGB: OFF", "LED Inactive");
    Serial.println("RGB LED is OFF");
  }

  // ── SYSTEM CONTROL ──────────────────────────────────────
  else if (data.equalsIgnoreCase("turn off all")) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    setRGB(0, 0, 0);
    lightOn = false;
    fanOn   = false;
    displayMessage("All Devices OFF", "System Standby");
    Serial.println("All devices turned OFF");
  }
  else if (data.equalsIgnoreCase("turn on all")) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);
    lightOn = true;
    fanOn   = true;
    displayMessage("All Devices ON", "System Active");
    Serial.println("All devices turned ON");
  }

  // ── MOTION MODE TOGGLE ───────────────────────────────────
  else if (data.equalsIgnoreCase("motion on")) {
    motionMode = true;
    displayMessage("Motion Mode ON", "Auto Detection");
    Serial.println("Motion detection enabled");
  }
  else if (data.equalsIgnoreCase("motion off")) {
    motionMode = false;
    displayMessage("Motion Mode OFF", "Manual Only");
    Serial.println("Motion detection disabled");
  }

  // ── UNKNOWN COMMAND ──────────────────────────────────────
  else {
    displayMessage("Unknown Cmd", data.substring(0, 16));
    Serial.println("Unknown command received");
  }
}

// ============================================================
// SET RGB LED COLOR
// r, g, b values: 0-255
// ============================================================
void setRGB(int r, int g, int b) {
  analogWrite(RGB_RED,   r);
  analogWrite(RGB_GREEN, g);
  analogWrite(RGB_BLUE,  b);
}

// ============================================================
// HANDLE PIR MOTION SENSOR
// Automatically turns on light when motion is detected
// ============================================================
void handleMotionSensor() {
  if (!motionMode) return;  // Skip if motion mode disabled

  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH && !lightOn) {
    digitalWrite(LED_PIN, HIGH);
    lightOn = true;
    displayMessage("Motion Detected!", "Light ON Auto");
    Serial.println("Motion detected — Light turned ON automatically");
    delay(500);  // Debounce
  }
}

// ============================================================
// HANDLE TOUCH SENSOR
// Toggles light on/off when touch is detected
// ============================================================
void handleTouchSensor() {
  int touched = digitalRead(TOUCH_PIN);

  if (touched == HIGH) {
    lightOn = !lightOn;
    digitalWrite(LED_PIN, lightOn ? HIGH : LOW);

    if (lightOn) {
      displayMessage("Touch Detected", "Light ON");
      Serial.println("Touch detected — Light ON");
    } else {
      displayMessage("Touch Detected", "Light OFF");
      Serial.println("Touch detected — Light OFF");
    }
    delay(500);  // Debounce to prevent rapid toggling
  }
}

// ============================================================
// DISPLAY MESSAGE ON I2C LCD
// line1: top row (max 16 chars)
// line2: bottom row (max 16 chars)
// ============================================================
void displayMessage(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, 16));
}
