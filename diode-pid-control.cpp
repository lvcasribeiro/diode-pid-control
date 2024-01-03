// Pinout:
const int ldr_pin = 15;
const int transistor_pin = 5;

// Setup function:
void setup() {
    pinMode(ldr_pin, INPUT);
    pinMode(transistor_pin, OUTPUT);

    analogWrite(transistor_pin, 0);

    Serial.begin(115200);
}
