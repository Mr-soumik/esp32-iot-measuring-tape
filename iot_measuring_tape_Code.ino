#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUTTON_PIN 4

bool continuousMode = false;
float frozenDistance = 0;
bool hasFrozenReading = false;

unsigned long lastButtonChange = 0;
unsigned long buttonPressStart = 0;
bool buttonWasPressed = false;
bool longPressHandled = false;
const unsigned long debounceDelay = 50;
const unsigned long longPressDuration = 600;

void showSplash() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("TechTadka");
  display.setCursor(30, 30);
  display.println("360");
  display.setTextSize(1);
  display.setCursor(15, 52);
  display.println("TECH . DIY . REPAIR");
  display.display();
  delay(2500);
}

float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2;
}

void drawMeasurement(float distance, bool frozen) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(frozen ? "MODE: SINGLE" : "MODE: CONTINUOUS");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  if (distance < 0) {
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.println("OUT OF");
    display.setCursor(10, 45);
    display.println("RANGE");
  } else {
    display.setTextSize(2);
    display.setCursor(10, 22);
    display.print(distance, 1);
    display.println(" cm");

    float inches = distance / 2.54;
    display.setTextSize(1);
    display.setCursor(10, 48);
    display.print(inches, 1);
    display.println(" inch");
  }

  display.setCursor(0, 56);
  display.setTextSize(1);
  display.print(frozen ? "Hold: Remeasure" : "Click: Switch Mode");
  display.display();
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showSplash();
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN) == LOW;
  unsigned long now = millis();

  if (buttonState != buttonWasPressed && now - lastButtonChange > debounceDelay) {
    lastButtonChange = now;
    buttonWasPressed = buttonState;

    if (buttonState) {
      buttonPressStart = now;
      longPressHandled = false;
    } else {
      unsigned long pressDuration = now - buttonPressStart;
      if (pressDuration < longPressDuration && !longPressHandled) {
        continuousMode = !continuousMode;
        hasFrozenReading = false;
      }
    }
  }

  if (buttonState && !longPressHandled && (now - buttonPressStart >= longPressDuration)) {
    if (!continuousMode) {
      frozenDistance = getDistanceCM();
      hasFrozenReading = true;
    }
    longPressHandled = true;
  }

  if (continuousMode) {
    float distance = getDistanceCM();
    drawMeasurement(distance, false);
    delay(150);
  } else {
    float distance = hasFrozenReading ? frozenDistance : -1;
    drawMeasurement(distance, true);
    delay(100);
  }
}
