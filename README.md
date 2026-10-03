# Temperature Monitoring and Alarm Device

This repository contains the complete firmware code, simulation setup, and documentation for the Embedded Engineering Internship Technical Assessment[span_7](start_span)[span_7](end_span).

## System Features
- Temperature Sensing: Uses DHT22 sensor to monitor room temperature[span_8](start_span)[span_8](end_span).
- Alarm Logic: Activates LED and 5V Buzzer if temperature stays above 8°C for more than 10 seconds[span_9](start_span)[span_9](end_span).
- MQTT Connectivity: Periodically publishes temperature values over Wi-Fi[span_10](start_span)[span_10](end_span).
- Fail-safe Operation: Monitoring continues even if Wi-Fi or MQTT connection is unavailable[span_11](start_span)[span_11](end_span).

## Electronics & Design Answers

### 1. Component Selection & Driver Circuit
- Microcontroller: ESP32 Dev Module (Integrated Wi-Fi and MQTT support)[span_12](start_span)[span_12](end_span).
- Temperature Sensor: DHT22 (Operating range 0°C to 50°C)[span_13](start_span)[span_13](end_span).
- Buzzer Driver Circuit: GPIO pin delivers max 8mA, whereas the 5V buzzer requires 60mA[span_14](start_span)[span_14](end_span). An NPN transistor (BC547) driver circuit with a 1kΩ base resistor is used to safely switch the buzzer[span_15](start_span)[span_15](end_span).

### 2. Calculations
- LED Series Resistor Calculation:
  $$R = \frac{V_{GPIO} - V_{LED}}{I_{LED}} = \frac{3.3\text{V} - 2.0\text{V}}{0.005\text{A}} = 260\,\Omega$$
  *Selected standard value:* 270 $\Omega$ (or 220 $\Omega$)[span_16](start_span)[span_16](end_span).

- I2C Pull-up Resistors:
  I2C buses use open-drain lines. Extremely high resistance leads to slow signal rise times and bus errors, while extremely low resistance causes excessive power consumption and signal distortion[span_17](start_span)[span_17](end_span).

### 3. Debugging Reset Issue
- Root Cause: Inductive switching noise or power rail voltage dips (brownout resets) caused by high current draw when turning on the buzzer[span_18](start_span)[span_18](end_span).
- Solution: Add a decoupling/bulk capacitor across power rails and a flyback diode across the buzzer coil[span_19](start_span)[span_19](end_span).
