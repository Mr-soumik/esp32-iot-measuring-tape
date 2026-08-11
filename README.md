# ESP32 IoT Measuring Tape

An ultrasonic distance-measuring tape built on ESP32, with an OLED readout and a button-toggled Single / Continuous measuring mode.

**TechTadka360** — TECH . DIY . REPAIR
## Connect with TechTadka360

- YouTube: [@techtadka360official](https://youtube.com/@techtadka360official?si=GdlIntZKv30kPgBk)
- Instagram: [@techtadka360official](https://www.instagram.com/techtadka360official?igsh=cWR4bnhjdWw1MHdh)
- Facebook: [TechTadka360](https://www.facebook.com/share/1EkKAJNLdB/)

## Features

- Single mode: long-press the button to take a fresh reading, held on screen until the next press
- Continuous mode: live distance tracking, smoothed with a moving average so the number doesn't jitter
- Short press toggles between modes
- Distance shown in cm and inches
- Custom TechTadka360 splash screen on boot
- Adjustable calibration offset to zero out sensor tolerance

## Parts List

- ESP32 dev board
- HC-SR04 ultrasonic sensor
- 0.96" SSD1306 OLED display (I2C)
- Push button
- 1kΩ and 1.8kΩ resistors (for the Echo voltage divider)

## Wiring

**HC-SR04**
- VCC → 5V
- GND → GND
- Trig → GPIO5
- Echo → GPIO18 (through a 1kΩ/1.8kΩ voltage divider — Echo outputs 5V and ESP32 GPIOs are 3.3V only)

**SSD1306 OLED (I2C)**
- VCC → 3.3V
- GND → GND
- SDA → GPIO21
- SCL → GPIO22

**Push button**
- One leg → GPIO4
- Other leg → GND
- Uses the ESP32's internal pull-up resistor, no external resistor needed

## Calibration

Measure a known distance (e.g. exactly 30cm with a ruler) and compare it to the display. Adjust `CALIBRATION_OFFSET_CM` in the sketch to correct any consistent offset from the sensor's own tolerance.

## Libraries Required

- Adafruit_GFX
- Adafruit_SSD1306

Install both via Arduino IDE Library Manager.

## License

MIT
