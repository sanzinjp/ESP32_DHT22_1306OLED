#include "Icons.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>

// Pin Definitions
#define DHTPIN 21
#define DHTTYPE DHT22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin not used

// Global Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(13, 14);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      ;
  }

  display.clearDisplay();
  display.display();
  delay(1000);
}

void loop() {
  static unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 20000; // 20 seconds

  if (millis() - lastUpdate >= updateInterval || lastUpdate == 0) {
    lastUpdate = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Set text color to white

    // Temperature icon
    display.drawBitmap(0, 0, tempIcon, 64, 32, SSD1306_WHITE);
    display.setCursor(30, 10);
    display.setTextSize(2.5);
    display.print(temp, 1);

    // Add spacing between temp and °
    int x = display.getCursorX();
    int y = display.getCursorY();

    x += 5;
    display.setCursor(x, y);
    display.drawCircle(x + 2, y + 2, 2, SSD1306_WHITE);
    display.setCursor(x + 7, y);
    display.print("C");

    // Humidity icon
    display.drawBitmap(0, 32, humidIcon, 64, 32, SSD1306_WHITE);
    display.setCursor(30, 42);
    display.setTextSize(2.5);
    display.print(hum, 1);
    display.print(" %");

    display.display();
  }

  // Place other non-blocking tasks here if needed
}
