Voice-Activated Home Automation System
MSc Final Thesis Project — Nottingham Trent University (2024)
A low-cost embedded home automation system controlled by voice commands via Bluetooth and automated by real-time sensor inputs. Built on Arduino Mega using Embedded C++, the system was showcased at NTU's university exhibition and received positive feedback for its real-world practicality.

🛠️ Tools & Technologies

Microcontroller: Arduino Mega
Language: Embedded C++
Communication: HC-05 Bluetooth Module (Android app integration)
Sensors: PIR Motion Sensor, Capacitive Touch Sensor
Display: 16x2 I2C LCD
Other: Relay module, RGB LED, buzzer, fans, lights


🎯 Objective
To develop a voice-activated embedded home automation system using Bluetooth communication and sensor-based automation, aimed at increasing convenience and energy efficiency in smart homes.

⚙️ Key Features
FeatureDescriptionVoice ControlAndroid app sends voice commands via HC-05 Bluetooth to ArduinoPIR AutomationPresence-based device switching using passive infrared sensorTouch SensorManual device control via capacitive touch inputLCD FeedbackReal-time status display of system state and active devicesError HandlingLogic for command overlap, invalid inputs, and audio-visual confirmationPower ManagementSafety cutoff and multi-device relay switching

🧠 Core Logic Snippet
cppif (voiceCommand == "turn on fan") {
  digitalWrite(FAN_PIN, HIGH);
  lcd.print("Fan ON");
} else if (PIR_detected) {
  digitalWrite(LIGHT_PIN, HIGH);
}

📊 Outcome

Delivered a scalable, low-cost home automation system using natural voice interaction
Reduced response latency compared to manual switching systems
Demonstrated effective integration across hardware, firmware, and user interface
Showcased at NTU University Exhibition — received positive feedback for practicality and real-world relevance


🔌 System Architecture
Android App (Voice Input)
        ↓ Bluetooth (HC-05)
   Arduino Mega
        ↓
  ┌─────────────────────┐
  │  PIR Sensor         │ → Presence detection → Light control
  │  Touch Sensor       │ → Manual override
  │  Relay Module       │ → Fan / Light switching
  │  I2C LCD Display    │ → Real-time status
  │  RGB LED + Buzzer   │ → Audio-visual confirmation
  └─────────────────────┘

📁 Repository Structure
voice-home-automation/
├── src/
│   └── main.ino              # Main firmware
├── circuit/
│   └── circuit_diagram.png   # Wiring diagram
├── docs/
│   └── project_report.pdf    # Full MSc report
├── images/
│   └── hardware_photos/      # Real build photos
└── README.md

👩‍💻 About
Built by Bhoomika Sathish Rao as MSc Final Thesis at Nottingham Trent University.

📧 bhoomikasrao2k@gmail.com
🔗 LinkedIn
🗂️ Full Portfolio
