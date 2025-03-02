#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESP32Servo.h>

// Servo Configuration
#ifndef SERVO_1_PIN
#define SERVO_1_PIN 27  // Servo1 connected to pin 27
#endif
Servo servo1;

// AP Credentials
const char* ssid = "ServoAP";
const char* password = "";

// Web Server and WebSocket
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// HTML Page with Slider
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	<title>Servo Control</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
	<h1>Servo Control</h1>
	<input type="range" min="0" max="180" value="90" id="slider" style="width: 80%">
	<button onClick="servoSlam()">SLAM ATTACK!</button>

	<script>
		var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
		var slider = document.getElementById('slider');

		slider.oninput = function() {
			socket.send(this.value);
		}

		function servoSlam() {
			// Start at top position (0°)
			socket.send("0");

			// Optional: Reset to ready position after attack
			setTimeout(() => { socket.send("180"); }, 600);
		}
	</script>
</body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
	if(type == WStype_TEXT)
	{
		int angle = atoi((char*)payload);
		angle = constrain(angle, 0, 180);
		Serial.println(angle);
		servo1.write(angle);
	}
}

void servoSetup()
{
	// Enable power rails
	pinMode(16, OUTPUT);
	pinMode(17, OUTPUT);
	digitalWrite(16, HIGH);
	digitalWrite(17, HIGH);

	// Initialize servo
	servo1.setPeriodHertz(50);
	servo1.attach(SERVO_1_PIN, 500, 2500);
	servo1.write(90);
}

void setup()
{
	Serial.begin(115200);

	servoSetup();

	// Start AP
	WiFi.softAP(ssid, password);

	// Start Web Server
	server.on("/", []() {
			server.send(200, "text/html", html);
		});
	server.begin();

	// Start WebSocket
	webSocket.begin();
	webSocket.onEvent(webSocketEvent);
}

void loop() {
	server.handleClient();
	webSocket.loop();
}
