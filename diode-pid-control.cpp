// Libraries import:
#include <WiFi.h>
#include <time.h>

// Pinout:
const int ldr_pin = 32;
const int transistor_pin = 5;

// PID constants:
const float kp = 7.5;
const float ki = 0.2;
const float kd = 0.3;

// Time management:
unsigned long previous_millis = 0;
const long interval = 500;

// Local date and time capture and time zone correction:
#define NTP_SERVER "pool.ntp.br"
const int daylight_offset_sec = 0;
const long gmt_offset_sec = -3600*3;

// Wi-fi connection, network SSID and password:
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Global variables:
float integral = 0;
float last_error = 0;
const int debug = 2;
int setpoint = 0;
int pid_control = 1;
int previous_pwm_value = 0;

char current_date[15];
char current_time[15];

char current_hour[3];
char current_minute[3];
char current_week_day[10];

// Setup function:
void setup() {
    pinMode(ldr_pin, INPUT);
    pinMode(transistor_pin, OUTPUT);

    analogWrite(transistor_pin, 0);

    Serial.begin(115200);

    configTime(gmt_offset_sec, daylight_offset_sec, NTP_SERVER);
    connectToWifi();
}

// Loop function:
void loop() {
    unsigned long current_millis = millis();

    if (WiFi.status() == WL_CONNECTED) {
        timeCapture();

        if (strcmp(current_hour, "4") == 0 && strcmp(current_minute, "00") == 0 && pid_control == 0) {
            pid_control = 1;

            if (debug >= 1) {
                Serial.println("- Resuming reading of local lighting values.");
            }
        } else if (strcmp(current_hour, "20") == 0 && strcmp(current_minute, "00") == 0  && pid_control == 1) {
            pid_control = 0;
            analogWrite(transistor_pin, 255);

            if (debug >= 1) {
                Serial.println("- Stopping PID control, assuming low light hours and allowing transistor at maximum operation.");
            }
        }

        if (current_millis - previous_millis >= interval && pid_control == 1) {
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
                Serial.print("- ");
                Serial.print(current_time);
                Serial.print(" - Light analog value: ");
                Serial.print(light_analog_value);
                Serial.print(" - Output: ");
                Serial.println(smoothed_pwm_value);
            }
        }

        if (Serial.available() > 0) {
            readSerialMonitor();
        }
    } else {
        if (debug >= 1) {
            Serial.println("- Loss of Wi-Fi connection.");
        }
        connectToWifi();
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

// Time management function:
void timeCapture() {
    struct tm time_info;

    if (!getLocalTime(&time_info)) {
        if (debug >= 1) {
            Serial.println("- Failed to capture NTP server data.");
        }

        configTime(gmt_offset_sec, daylight_offset_sec, NTP_SERVER);
        
        return;
    }

    strftime(current_date, 15, "%e/%m/%Y", &time_info);
    strftime(current_time, 15, "%H:%M:%S", &time_info);

    strftime(current_hour, 3, "%H", &time_info);
    strftime(current_minute, 3, "%M", &time_info);
    strftime(current_week_day, 10, "%A", &time_info);
}

// Wi-fi connection function:
void connectToWifi() {
    if (debug >= 1) {
        Serial.print("- Connecting to the Wi-Fi network...");
    }

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long wifi_millis = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - wifi_millis < 5000) {
        if (debug >= 1) {
            Serial.print(".");
        }
        
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (debug >= 1) {
            Serial.print("\n- Connected to Wi-Fi network, IP address: ");
            Serial.println(WiFi.localIP());
        }
    } else {
        if (debug >= 1) {
            Serial.println("\n- Failed to connect to Wi-Fi.");
        }
    }
}
