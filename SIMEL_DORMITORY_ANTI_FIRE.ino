#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int flameSensorPin = 13;   // Flame sensor input (simulate with HIGH/LOW logic)
const int gasSensorPin = A0;     // Gas sensor input (simulate with HIGH/LOW logic)
const int relayPin = 8;          // Relay to activate fire extinguisher
const int buzzerPin = 10;        // Buzzer
const int servoPin = 9;          // Servo motor for door

// GSM Pins
SoftwareSerial gsmSerial(12, 11);  // RX, TX pins for GSM module (e.g., SIM800L or SIM900)

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set LCD address to 0x27, and size to 16x2
Servo doorServo;                   // Servo object for the door

// Thresholds for detection (simulated by HIGH/LOW logic for flame and gas)
int flameThreshold = 1;  // Simulated value (1 = fire detected, 0 = no fire)
int gasThreshold = 1;    // Simulated value (1 = gas detected, 0 = no gas)

// GSM Phone number to send message to
String phoneNumber = "+2547O4537044"; // Replace with the phone number to receive the SMS

void setup() {
  pinMode(flameSensorPin, INPUT);
  pinMode(gasSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin(16, 2); // Initialize LCD
  lcd.print("System Initializing...");
  
  doorServo.attach(servoPin); // Attach the servo to the specified pin
  
  // Initial door position (Closed)
  doorServo.write(0);
  
  delay(200); // Wait for the system to initialize
  
  lcd.clear();
  lcd.print("System Ready!");
  delay(200);
  
  gsmSerial.begin(9600); // Initialize GSM serial communication
  sendSMS("Fire Safety System Ready!");
}

void loop() {
  int flameSensorValue = digitalRead(flameSensorPin); // Read flame sensor (simulated HIGH/LOW)
  int gasSensorValue = digitalRead(gasSensorPin);     // Read gas sensor (simulated HIGH/LOW)
  
  // Check flame sensor and gas sensor status
  if (flameSensorValue == HIGH || gasSensorValue == HIGH) {
    if (flameSensorValue == HIGH) {
      lcd.clear();
      lcd.print("Fire Detected!");
      sendSMS("ALERT: Fire detected in the dormitory!");
    }
    else if (gasSensorValue == HIGH) {
      lcd.clear();
      lcd.print("Gas Detected!");
      sendSMS("ALERT: Gas detected in the dormitory!");
    }

    // Activate fire extinguisher
    digitalWrite(relayPin, HIGH);   
    digitalWrite(buzzerPin, HIGH);           // Sound buzzer
    doorServo.write(90);             // Open door (if you want)

    delay(200); // Reduce delay time after detection

  } else {
    // No fire or gas detected
    lcd.clear();
    lcd.print("System Normal");
    digitalWrite(relayPin, LOW);    // Deactivate fire extinguisher
    digitalWrite(buzzerPin,LOW);              // Stop buzzer
    doorServo.write(0);             // Keep the door closed
  }
  
  delay(200); // Regular check interval (500ms)
}

// Function to send SMS
void sendSMS(String message) {
  gsmSerial.println("AT");         // Test communication with GSM module
  delay(100);
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(100);
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");  // Specify the recipient's phone number
  delay(100);
  gsmSerial.println(message);      // The message to send
  delay(100);
  gsmSerial.write(26);             // Ctrl+Z to send the SMS
  delay(100);
}
