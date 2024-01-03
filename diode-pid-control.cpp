// Pinout:
const int ldr_pin = 15;
const int transistor_pin = 5;

// PID constants:
const float kp = 7.5;
const float ki = 0.2;
const float kd = 0.3;

// Time management:
unsigned long previous_millis = 0;
const long interval = 100;

// Global variables:
float integral = 0;
float last_error = 0;
const int debug = 0;
int setpoint = 0;
int previous_pwm_value = 0;

// Setup function:
void setup() {
    pinMode(ldr_pin, INPUT);
    pinMode(transistor_pin, OUTPUT);

    analogWrite(transistor_pin, 0);

    Serial.begin(115200);
}

// Loop function:
void loop() {
    unsigned long current_millis = millis();

    if (current_millis - previous_millis >= interval) {
        previous_millis = current_millis;

        int light_analog_value = analogRead(ldr_pin);
        float error = setpoint - light_analog_value;

        float proportional = kp * error;
        integral += ki * error;
        float derivative = kd * (error - last_error);

        float output = proportional + integral + derivative;

        if (output >= 255 || output <= 0) {
            integral = 0;
        }

        int pwm_value = constrain(map(output, -100, 100, 0, 255), 0, 255);

        float smoothing_factor = 0.5;
        int smoothed_pwm_value = (1.0 - smoothing_factor) * pwm_value + smoothing_factor * previous_pwm_value;

        analogWrite(transistor_pin, smoothed_pwm_value);

        previous_pwm_value = smoothed_pwm_value;

        last_error = error;

        if (debug >= 2) {
            Serial.print("- Light analog value: ");
            Serial.print(light_analog_value);
            Serial.print(" > Output: ");
            Serial.println(smoothed_pwm_value);
        }
    }

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
        if (debug >= 1) {
            Serial.print("- Setpoint changed to: ");
            Serial.println(setpoint);
        }
    } else {
        if (debug >= 1) {
            Serial.println("- Invalid input. Please enter a valid integer.");
        }
    }
}
