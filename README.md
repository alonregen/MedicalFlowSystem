# MedicalFlow System

**MedicalFlow** is a sophisticated appointment management system designed specifically for medical facilities, leveraging the power of IoT (Internet of Things) technology. It enhances the efficiency and safety of clinical appointments through features like real-time queue monitoring, room occupancy detection, and emergency alerts.

## Features

- **Real-time Queue Monitoring**: Keeps patients informed about their wait times and queue status.
- **Room Occupancy Detection**: Manages consultation room availability efficiently.
- **Emergency Alerts**: Notifies staff instantly in case of emergencies for swift action.
- **Temperature Checks & Patient Detail Entry**: Streamlines the check-in process with a QR code scan.
- **Email Notifications**: Sends automatic updates to patients about queue progress.

## Getting Started

To get a copy of MedicalFlow running on your local machine for development and testing purposes, follow these instructions.

### Prerequisites

Before installing the MedicalFlow System, ensure you have the following components:

- ESP32 development board
- Adafruit NeoPixel LED strip
- DHT22 temperature and humidity sensor
- Light sensor
- Liquid level sensor
- Active buzzer or speaker for tone output
- Access to a Wi-Fi network

### Installation

1. Clone the repository to your local machine.
2. Ensure you have the Arduino IDE installed, along with the ESP32 board configurations.
3. Install the following libraries through the Arduino Library Manager:
   - `Blynk`
   - `Adafruit_NeoPixel`
   - `ESP32Servo`
   - `DHT sensor library`
4. Configure your Wi-Fi credentials and Blynk token in the provided code.
5. Upload the code to your ESP32 device.

### Configuration

- **LED_PIN (16)**: The pin connected to the NeoPixel LED strip.
- **LED_COUNT (12)**: Number of LEDs on your strip.
- **DHTPIN (4)**: The pin connected to the DHT22 sensor.
- **LIGHT_SENSOR (34)**: The analog pin connected to the light sensor.
- **POWER_PIN (17), SIGNAL_PIN (32)**: Pins for controlling and reading from the liquid level sensor.
- **TONE_OUTPUT_PIN (27)**: The pin connected to the active buzzer or speaker.

### How It Works

The MedicalFlow System integrates various sensors and outputs to monitor and manage patient flow within a medical facility. It uses Blynk for real-time communication and control, providing updates on temperature, light levels, and room occupancy to staff and patients.

## Purpose of the System

The primary goal of the MedicalFlow System is to streamline the appointment process in medical facilities, reducing wait times, enhancing patient safety, and improving the overall efficiency of medical consultations.

## Contributing

We welcome contributions! If you have suggestions or want to improve the MedicalFlow System, please fork the repository and create a pull request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
