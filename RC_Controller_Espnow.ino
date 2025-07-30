/* I Jamal Osler 7-30-2025 will be modifing the existing ESP-NOW joystick code
to include button functionality for an RC controller. And other functionality as well. */

#include <Arduino.h>
#include <EspNowJoystick.hpp>

/* You will need to add the button states to the structure in the library for
Example:
struct JoystickMessage {
    uint8_t ax;        // X-axis value
    uint8_t ay;        // Y-axis value
    uint8_t az;        // Z-axis value
    bool button1;      // State of button 1
    bool button2;      // State of button 2
    // Add more buttons as needed
Add more buttons only if it's necessary.*/

EspNowJoystick joystick;
JoystickMessage jm;
bool receiverConnected;

// Custom MAC address target for recvier
const uint8_t device1[6] = {0x3C, 0x61, 0x05, 0x0c, 0x93, 0xb8};

// Button pins for action or other task for the recvier
const int button1Pin = 2; // Change to your button pin
const int button2Pin = 3; // Change to your button pin

// Button states to debug buttons
bool button1State = false;
bool button2State = false;

// Callback to telemetry values to the reciver
class MyTelemetryCallbacks : public EspNowTelemetryCallbacks {
    void onTelemetryMsg(TelemetryMessage tm) {
        Serial.println("Telemetry msg received");
        receiverConnected = tm.e1;
    }
    void onError(const char *msg) {}
};

void setup() {
    Serial.begin(115200);
    // Initialize button pins
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    
    joystick.setTelemetryCallbacks(new MyTelemetryCallbacks());
    jm = joystick.newJoystickMsg();
    joystick.init();
}

void loop() {
    // Read joystick values (replace with your actual joystick reading logic)
    uint8_t ax = map(random(0, 100), 0, 100, 0, 200);
    uint8_t ay = map(random(0, 100), 0, 100, 0, 200);
    uint8_t az = map(random(0, 100), 0, 100, 0, 200);

    // Read button states
    button1State = digitalRead(button1Pin) == LOW; // LOW when pressed
    button2State = digitalRead(button2Pin) == LOW; // LOW when pressed

    // Fill joystick message
    jm.ax = ax;
    jm.ay = ay;
    jm.az = az;

    // Add button states to the joystick message (you may need to adjust the protocol)
    jm.button1 = button1State; // Assuming you have defined button1 in your protocol
    jm.button2 = button2State; // Assuming you have defined button2 in your protocol

    // Send joystick message to reciver 
    joystick.sendJoystickMsg(jm, device1);
