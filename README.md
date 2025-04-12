# STM32F103 FFT Display with MQTT and HTML Frontend

This project demonstrates a low-cost embedded FFT system using an STM32F103RCT6 microcontroller. The system captures analog signals, performs FFT using CMSIS DSP, displays waveform and spectrum on two ST7789 screens, and transmits FFT data via MQTT to a browser-based HTML chart.

> **Note**: The components marked with white circles in the first image are related to a future self-balancing vehicle project and are not involved in this experiment.

---

## 📷 System Overview

### 1. Hardware Architecture
![System Block Diagram](media/20241210-192203.jpg)

- STM32F103RCT6: Core processor
- ST7789 LCD ×2: Display waveform and FFT
- ESP8266: Publishes FFT peak via MQTT
- Microphone module: Captures sound
- Rotary encoder: For optional UI navigation

---

## 📶 Experimental Results (Single-tone Tests)

| Frequency | Image |
|-----------|-------|
| 100 Hz    | ![](media/20241210-155841.jpg) |
| 200 Hz    | ![](media/20241210-155924.jpg) |
| 300 Hz    | ![](media/20241210-160035.jpg) |
| 400 Hz    | ![](media/20241210-160214.jpg) |
| 500 Hz    | ![](media/20241210-160234.jpg) |
| 600 Hz    | ![](media/20241210-160417.jpg) |

Each test confirms successful frequency-domain representation of the input tone.

---

## 🌐 HTML Visualization via MQTT

### 8. MQTT Publishing and Frontend Chart Display
![MQTT + HTML Demo](media/20250407_001815907.jpg)

- **Left Panel**: Real-time Chart.js FFT peak visualization via MQTT.js
- **Right Panel**: Mobile phone plays a 400 Hz tone at reduced volume to simulate cancellation

---

## 💡 Features

- Dual-screen real-time waveform and spectrum display
- Configurable frequency bin mapping
- Lightweight HTML frontend using MQTT.js and Chart.js
- Ready for integration with active noise cancellation projects

---

## 📦 Folder Structure

- `/HARDWARE`: Includes modules like `lv_mainstart`, `fft`, `AD`
- `/USER`: Main control loop in `main.c`
- `/media`: Contains all demo images shown above
- `3.mqtt-js.html`: Simple web MQTT client for FFT visualization

---

Let me know if you'd like a 中文版本的 README，我可以幫你對照撰寫 👌
