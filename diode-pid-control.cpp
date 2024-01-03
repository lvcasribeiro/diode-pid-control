// Pinout:
const int ldr_pin = 15;
const int transistor_pin = 5;

// Global variables:
int setpoint = 0;

// Setup function:
void setup() {
    pinMode(ldr_pin, INPUT);
    pinMode(transistor_pin, OUTPUT);

    analogWrite(transistor_pin, 0);

    Serial.begin(115200);
}

// Loop function:
void loop() {
    if (Serial.available() > 0) {
        readSerialMonitor();
    }
}

// Serial monitor reading function:
void readSerialMonitor() {
    String user_input = Serial.readStringUntil('\n');
    int new_setpoint = user_input.toInt();
    
    if (new_setpoint != 0) {
        setpoint = new_setpoint;
        Serial.print("- Setpoint changed to: ");
        Serial.println(setpoint);
    } else {
        Serial.println("- Invalid input. Please enter a valid integer.");
    }
}
