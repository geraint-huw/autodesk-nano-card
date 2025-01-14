/*
 * Phototransistor liht sensor test
 *
 * Reads an analog value in from the phototransistor (light sensor), and prints the value on screen.
 * The range of values obtained can be calibrated by adjusting the potentiomenter on the board.
 * 
 * Modified 2025 Geraint Huw Rowlands (https://github.com/geraint-huw)
 * 
 * This example code is in the public domain.
 */

#include <Wire.h>
#define SSD1306_NO_SPLASH //reduce program size by removing default splash screen from driver library
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int sensorValue=0;

void setup() {
  // Setup OLED display
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);

  pinMode(A7,INPUT);
}

void loop() {
  // Read in value from the sensor, and redraw the display
  sensorValue = analogRead(A7);

  oled.clearDisplay();
  oled.setCursor(0, 10);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.print("Light sensor reading: ");
  oled.println(sensorValue);
  oled.display();
  delay(200);

}
