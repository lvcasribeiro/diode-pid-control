## Light-emitting Diode PID Controller

This repository aims to concentrate relevant information and codes on the development of a proportional integral derivative controller for a light-emitting diode using a LDR (light dependent resistor) and a 2N2222 NPN transistor for brightness control.

##

### Functionality

The PID light controller regulates the brightness of a light source based on the input from the LDR light sensor. The PID constants (**kp**, **ki**, **kd**) and a **setpoint** value can be configured in the code, as follows:

```c
// PID constants:
const float kp = 7.5;
const float ki = 0.2;
const float kd = 0.3;
```

```c
// Global variables:
int setpoint = 0;
```

##

### Wire Connections

Connect the components as follows:

###### 1. LDR Light Sensor: `Pin 15`

- Connect one leg of the LDR to the 3.3V on the ESP32;
- Connect the other leg to one leg of a resistor (e.g., 10k Ohms);
- Connect the other leg of the resistor to the ground (GND) on the ESP32;
- Connect the junction of the LDR and resistor to an analog pin (e.g., 15) on the ESP32.

<br>

<p align="center">
    <img src="https://github.com/lvcasribeiro/automatic-punch-the-clock/assets/96185134/01cdc00b-713f-4e57-aaee-78a87d739669">
</p>

###### 2. 2N2222 NPN Transistor: `Pin 5`

- Connect the **base** of the transistor to a digital GPIO pin (e.g., 5) on the ESP32;
- Connect the **collector** pin of the 2N2222 transistor to the positive terminal of the external power supply;
- Connect the **emitter** of the 2N2222 transistor to the cathode of the diode;
- Connect the negative terminal of the diode to the ground (GND) on the ESP32.

<br>

<p align="center">
    <img src="https://github.com/lvcasribeiro/automatic-punch-the-clock/assets/96185134/f0e1ea71-ff5c-49eb-a06f-cbf09bb91677">
</p>

###### 3. External Power Supply:

- Connect the positive terminal of the external power supply to the **collector** pin of the 2N2222 transistor;
- Connect the negative terminal of the external power supply to the anode of the diode.

Ensure proper power connections and ground.

##

### Code Explanation

The main components of the code include:

- **Pinout:** Configuration of LDR and transistor pins;
- **PID Constants:** Set the proportional, integral, and derivative constants;
- **Time Management:** Define intervals for PID calculations;
- **Global Variables:** Store integral, last error, setpoint, and previous PWM value;
- **Setup Function:** Initialize pin modes and serial communication;
- **Loop Function:** Implement PID control, handle anti-windup, and update the PWM value. Serial monitor displays the light analog value and output.

##

### How to Use

1. Connect the LDR and transistor to the specified pins;
2. Upload the code to your ESP32;
3. Open the serial monitor to view the light analog value and adjusted PWM output.

###### - Serial Monitor Commands:

You can change the setpoint value via the Serial Monitor. Enter a valid integer to adjust the desired light level.

##

### Reminder

Keep in mind that this PID controller code can be utilized generically to regulate various processes, such as temperature control or any system where proportional, integral, and derivative control is applicable. It provides a versatile solution for achieving desired setpoints and maintaining system stability.
