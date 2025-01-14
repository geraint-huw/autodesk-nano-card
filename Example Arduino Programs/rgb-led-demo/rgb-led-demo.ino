/*
 * RGB LED Simple Demo
 *
 * Steps through red, green and blue colours on the RGB LED.
 * If button is held down, the LED will fade between the colours, instead of stepping.
 * 
 * Modified 2025 Geraint Huw Rowlands (https://github.com/geraint-huw)
 * 
 * This example code is in the public domain.
 */

//Pin definitions for the LED and button on the board
#define RED 3
#define GREEN 5
#define BLUE 6
#define RGB_CATHODE 4
#define BUTTON 2

int buttonPressed;

//Function to fade LED 1 off, and LED 2 on, over the course of 500ms
void fadeBetweenLEDs(int LED1, int LED2) {
  int fadeCounter = 0;
  while (fadeCounter < 200) {
    analogWrite(LED2, fadeCounter);
    analogWrite(LED1, 200 - fadeCounter);
    delay(2);
    fadeCounter++;
  }
}

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RGB_CATHODE, OUTPUT);
  digitalWrite(RGB_CATHODE,0); //Turning this pin to a high voltage would turn off the LED

  pinMode(BUTTON, INPUT);
}

void loop() {
  //Pure red on LED
  digitalWrite(RED,1);
  digitalWrite(GREEN,0);
  digitalWrite(BLUE,0);

  buttonPressed = digitalRead(BUTTON);
  if (buttonPressed) {
    //Fade to the next colour over the next 500ms
    fadeBetweenLEDs(RED,GREEN);
  }
  else{
    //Stay red for 500ms, then change
    delay(500);
  }

  //Pure green on LED
  digitalWrite(RED,0);
  digitalWrite(GREEN,1);
  digitalWrite(BLUE,0);

  buttonPressed = digitalRead(BUTTON);
  if (buttonPressed) {
    //Fade to the next colour over the next 500ms
    fadeBetweenLEDs(GREEN,BLUE);
  }
  else{
    //Stay red for 500ms, then change
    delay(500);
  }

  //Pure blue on LED
  digitalWrite(RED,0);
  digitalWrite(GREEN,0);
  digitalWrite(BLUE,1);
  
  buttonPressed = digitalRead(BUTTON);
  if (buttonPressed) {
    //Fade to the next colour over the next 500ms
    fadeBetweenLEDs(BLUE,RED);
  }
  else{
    //Stay red for 500ms, then change
    delay(500);
  }
}
