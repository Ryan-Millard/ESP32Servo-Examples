#include <Arduino.h>
#include <ESP32Servo.h>

#define servo1Pin 27  // Servo1 connected to pin 27
Servo servo1;         // Create servo object

void setup() {
	// Enable power rails
	pinMode(16, OUTPUT);  // en8v pin
	pinMode(17, OUTPUT);  // en5v pin
	digitalWrite(16, HIGH);
	digitalWrite(17, HIGH);

	// Initialize servo
	servo1.setPeriodHertz(50);  // Standard 50Hz servo
	servo1.attach(servo1Pin, 500, 2500);  // Attach with min/max pulse widths

	// Seed random number generator with noise from analog pin (if available)
	randomSeed(analogRead(A0));  // Use an unconnected analog pin for entropy
}

void loop() {
	int servo_degree = random(0, 181);  // Generate random angle (0-180)
	servo1.write(servo_degree);
	delay(1000);  // Wait 1 second for servo to move before next command
}
