/*
 * RGB LED control with menu of SSD1306 OLED
 *
 * Steps through red, green and blue colours on the RGB LED.
 * If button is held down, the LED will fade between the colours, instead of stepping.
 * 
 * Modified 2025 Geraint Huw Rowlands (https://github.com/geraint-huw)
 * 
 * This example code is in the public domain.
 */

//Arduino pins used for input buttons and LED output
#define RED 3
#define GREEN 5
#define BLUE 6
#define RGB_CATHODE 4
#define BUTTON 2
#define UP_BUTTON 16
#define DOWN_BUTTON 15
#define LEFT_BUTTON 14
#define RIGHT_BUTTON 17



//define parameters for the OLED display
#include <Wire.h>
#define SSD1306_NO_SPLASH //reduce program size by removing default splash screen from driver library
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//initial values for RGB LED brightness
int redValue = 10;
int greenValue = 10;
int blueValue = 10;

int menuPosition = 0; //used to scroll left and right through menu options


/*
  Initial setup routine - start I2C interface and OLED, set up input and output pins
*/
void setup() {
  //Outputs for LED control
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RGB_CATHODE, OUTPUT);
  digitalWrite(RGB_CATHODE,0); //Turning this pin to a high voltage would turn off the LED

  //Directional buttons for menu operation
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);

  //Set up OLED display
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
}


/*
 Adds a positive or negative modifier to an LED value, keeping within the range 0-255
*/
void adjustValue(int LED, int modifier) {
  if (LED == 0) {
    //Red LED
    redValue = redValue + modifier;
    if (redValue < 0) {
      redValue = 0;
    }
    else if (redValue > 255) {
      redValue = 255;
    }
  }
  else if (LED == 1) {
    //Green LED
    greenValue = greenValue + modifier;
    if (greenValue < 0) {
      greenValue = 0;
    }
    else if (greenValue > 255) {
      greenValue = 255;
    }
  }
  else if (LED == 2) {
    blueValue = blueValue + modifier;
    if (blueValue < 0) {
      blueValue = 0;
    }
    else if (blueValue > 255) {
      blueValue = 255;
    }
  }
}


/*
  Main program loop
*/
void loop() {
  //Draw menu on-screen
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("RGB LED Colour Mixer");

  oled.setCursor(10, 25);
  oled.print("Red");
  oled.setCursor(10, 35);
  oled.print(redValue);

  oled.setCursor(50, 25);
  oled.print("Green");
  oled.setCursor(50, 35);
  oled.print(greenValue);

  oled.setCursor(90, 25);
  oled.print("Blue");
  oled.setCursor(90, 35);
  oled.print(blueValue);

  switch (menuPosition) {
    case 0: //Red menu arrows
      oled.drawTriangle(15,20, 25,20, 20,15, WHITE);
      oled.drawTriangle(15,46, 25,46, 20,51, WHITE);
      break;
    case 1: //Green menu arrows
      oled.drawTriangle(55,20, 65,20, 60,15, WHITE);
      oled.drawTriangle(55,46, 65,46, 60,51, WHITE);
      break;
    case 2: //Blue menu arrows
      oled.drawTriangle(95,20, 105,20, 100,15, WHITE);
      oled.drawTriangle(95,46, 105,46, 100,51, WHITE);
      break;
    default:
      menuPosition = 0; //should not be outside the range 0-2!
      oled.drawTriangle(15,20, 25,20, 20,15, WHITE);
      oled.drawTriangle(15,46, 25,46, 20,51, WHITE);
  }
  oled.display();

  //update LED output to current values
  analogWrite(RED,redValue);
  analogWrite(GREEN,greenValue);
  analogWrite(BLUE,blueValue);

  //scan for button presses and respond
  if (digitalRead(DOWN_BUTTON) ) {
    adjustValue(menuPosition, -1);
    delay(100); //shorter delay values than left/right to make it quick to scroll up.down through brightnesses
  }
  else if (digitalRead(UP_BUTTON) ) {
    adjustValue(menuPosition, +1);
    delay(100);
  }
  else if (digitalRead(LEFT_BUTTON) ) {
    menuPosition--;
    if (menuPosition < 0) {
      menuPosition = 2; //wrap round the screen from red to blue
    }
    delay(500);
  }
  else if (digitalRead(RIGHT_BUTTON) ) {
    menuPosition++;
    if (menuPosition > 2) {
      menuPosition = 0; //wrap round the screen from blue to red
    }
    delay(500);
  }
  


}
