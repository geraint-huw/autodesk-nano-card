/*
 * Snake
 *
 * Runs a game of Snake on an SSD1306-compatible OLED display
 *
 *
 * Copyright (c) 2022 Adrian (enfchAddi, https://github.com/enfchAddi)
 * Copyright (c) 2025 Geraint Huw Rowlands (https://github.com/geraint-huw)
 * 
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.  
 */



//Arduino pins used for input buttons and random entropy
#define UP_BUTTON 16
#define DOWN_BUTTON 15
#define LEFT_BUTTON 14
#define RIGHT_BUTTON 17
#define START_BUTTON 2
#define GAME_LED 13
#define RANDOM_ANALOG A6 //disconnected pin - used as source of randomness!




#include <Wire.h>
#define SSD1306_NO_SPLASH //reduce program size by removing default splash screen from driver library
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define upward 1
#define downward 2
#define leftward 3
#define rightward 4

//Game variables (initialised by titleScreen() for each new game)
int direction,x,y,bodyLength;
int highScore=0;
int body[100][2]{};  //limit max body size to 100 - get beyond this and you can crash the game as a reward!

int food_x, food_y;
bool foodGen;

int randomNumber{random()};

//Snake logo shown on title screen
#define LOGO_HEIGHT   15
#define LOGO_WIDTH    24
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b00000111, 0b10000000,
  0b00000000, 0b11001111, 0b11000000,
  0b00000001, 0b11101111, 0b11100000,
  0b00000001, 0b11101110, 0b11100110,
  0b00000011, 0b01101100, 0b01101111,
  0b01110110, 0b01101110, 0b01101101,
  0b11111110, 0b01101110, 0b01001100,
  0b11111100, 0b01100110, 0b11011100,
  0b01110000, 0b11100110, 0b11011000,
  0b00000001, 0b11000110, 0b11011000,
  0b00000001, 0b11000110, 0b11111000,
  0b00000001, 0b11001110, 0b11110000,
  0b00000001, 0b11111110, 0b01100000,
  0b00000000, 0b11111100, 0b00000000,
  0b00000000, 0b01111000, 0b00000000};


/*
*  Random number generation, used for placing new food on the game screen
*/
void randomUpdate(int a = 1, int b = 100) {
  randomNumber = random(a, b);
}


/*
  Subroutine to draw a border around the edge of the display, to mark the game boundary
*/
void drawBorder() {
  oled.drawFastHLine(0, 0, SCREEN_WIDTH, SSD1306_WHITE);
  oled.drawFastHLine(0, SCREEN_HEIGHT -1, SCREEN_WIDTH, SSD1306_WHITE);
  oled.drawFastVLine(0, 0, SCREEN_HEIGHT, SSD1306_WHITE);
  oled.drawFastVLine(SCREEN_WIDTH -1, 0, SCREEN_HEIGHT, SSD1306_WHITE);
}


/*
  Movement subroutines. Steps two at a time, to effectively reduce the resolution of the play area
*/
void moveUpward() {  y -= 2; }

void moveDownward() {  y += 2; }

void moveLeftward() {  x -= 2; }

void moveRightward() {  x += 2; }


/*
  Add new coordinates for the snake at the current location, and shunt the old coordinates through the array of coordinates.
  If the snake's body length has just increased (by eating a fruit), then the oldest coordinate will be kept, and the snake will grow.
  Otherwise, it will be discarded as the snake moves across the map.
*/
void addBody() {
  
  for (int i=bodyLength; i >= 1; i--) {

    
    body[i][0] = body[i - 1][0];
    body[i][1] = body[i - 1][1];
  }
  body[0][0] = x;
  body[0][1] = y;
}


/*
  Iterates through all the coordinates for the snake's body, and draws them as pixels on the screen
*/
void drawSnake() {
  oled.setCursor(0,0);
  for (int i{ 0 }; i <= bodyLength; i++) {
    oled.drawPixel(body[i][0], body[i][1], SSD1306_WHITE);
  }
}


/*
  Draw food pixel on screen.
  If the food's just been eaten, generate new coordinates first
  */
void generateFood() {
  if (foodGen) {
    randomUpdate(1, SCREEN_WIDTH-2);
    food_x = randomNumber;
    randomUpdate(1, SCREEN_HEIGHT-2);
    food_y = randomNumber;
    foodGen = false;

    //make sure the food will be placed on an even row/column
    if (food_x == 1) {
      food_x = 2;
    }
    if (food_y == 1) {
      food_y = 2;
    }
    if (food_x % 2 != 0) {
      food_x--;
    }
    if (food_y % 2 != 0) {
      food_y--;
    }
    
  }

  oled.drawPixel(food_x, food_y, SSD1306_WHITE);
}


/*
  Check if the snake head has reached the food.
  If so the snake grows, and new food needs to be generated.
*/
void foodCheck() {
  if (x == food_x && y == food_y) {
    foodGen = true;
    bodyLength++;
  }
}


/*
  Display welcome screen, and wait for button press to start game.
  Then initialises game variables ready for the main loop to run the game.
*/
void titleScreen() {
  oled.clearDisplay();

  oled.setCursor((SCREEN_WIDTH  - 30 ) / 2, 0); //30 is width of "Snake" text, i.e. print it center aligned
  oled.println("Snake");

  oled.drawBitmap(
    (SCREEN_WIDTH - LOGO_WIDTH ) / 2,
    (LOGO_HEIGHT / 2) + 5,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1); //center align horizontally, print just below "Snake" text

  oled.setCursor(0, LOGO_HEIGHT+20);
  oled.println("Press a button...");

  if (highScore) { //only show this if a high score has been set
    oled.setCursor(15, LOGO_HEIGHT+40);
    oled.print("High score: ");
    oled.println(highScore);
  }
  oled.display();

  //loop until any button is pressed
  while ( (digitalRead(UP_BUTTON)
    || digitalRead(DOWN_BUTTON)
    || digitalRead(LEFT_BUTTON)
    || digitalRead(RIGHT_BUTTON)
    || digitalRead(START_BUTTON) ) == false)
    { randomUpdate(); } //move the random number generator along while we wait
  oled.clearDisplay();
  delay(500);

  //set initial conditions for new game
  direction = rightward;
  bodyLength = 3;
  foodGen=1;

  //new snake starts in the middle of the screen
  if (SCREEN_WIDTH % 2 != 0) {
    x = (SCREEN_WIDTH /2) -1;
  }
  else {
    x = SCREEN_WIDTH /2;
  }
  if (SCREEN_HEIGHT % 2 != 0) {
    y = (SCREEN_HEIGHT /2) -1;
  }
  else {
    y = SCREEN_HEIGHT /2;
  }
  
  //erase the previous snake's body from memory - otherwise the old snake parts will be drawn at the start of a new game!
  for (int i=0; i <= bodyLength; i++) {
    body[i][0] = NULL,
    body[i][1] = NULL;
  }
}


/*
  Called when the snake hits the screen boundary or its own tail
  The score displayed is equal tothe number of fruit eaten during the game
  After a short delay, the title screen is shown again
*/
void gameOver()
{
  digitalWrite(GAME_LED, 0); //turn off the game-in-progress LED
  oled.clearDisplay();
  oled.setCursor(0, 10);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.println("Game Over!");
  oled.print("Your Score: ");
  oled.println(bodyLength - 3);
  oled.display();
  delay(2000);

  if (bodyLength - 3 > highScore){
    highScore = bodyLength - 3;
  }

  titleScreen();
}


/*
  Initial setup routine - start I2C interface and OLED, seed the random number generator
*/
void setup() {
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  DDRD = 0;
  DDRB = 0;
  pinMode(GAME_LED,OUTPUT);
  pinMode(A6, INPUT); //unconnected analog input, used for random seeding
  randomSeed(analogRead(A6)); 

  titleScreen();
}

/*
  Main program loop
*/
void loop() {
  digitalWrite(GAME_LED, 1); //Game is in progress, so indicate this
  oled.clearDisplay(); //wipe the display clear ready for updates
  drawBorder(); //put the border back
  randomUpdate(); //advance the random number generator
  generateFood(); //place new food if we need to
  foodCheck(); //check if the snake has reached the food
  addBody(); //update the location of the snake's body segments
  drawSnake(); //draw the snake onscreen

  //Move the snake in the current direction
  switch (direction) {
    case upward:
      moveUpward();
      break;
    case downward:
      moveDownward();
      break;
    case leftward:
      moveLeftward();
      break;
    case rightward:
      moveRightward();
      break;
  }

  oled.display(); //draw all the updates to the display

  //scan buttons to update the direction of travel
  if (digitalRead(LEFT_BUTTON) && direction != rightward) {
    direction = leftward;
  }
  if (digitalRead(DOWN_BUTTON) && direction != upward) {
    direction = downward;
  }
  if (digitalRead(UP_BUTTON) && direction != downward) {
    direction = upward;
  }
  if (digitalRead(RIGHT_BUTTON) && direction != leftward) {
    direction = rightward;
  }

  delay( 60 ); //reduce this value if you want to make the game quicker - and harder!

  //Check if the head of the snake has hit the screen border, or it's own body segments
  bool crashed=0;
  if (x < 1 || y < 1 || x > SCREEN_WIDTH-1 || y > SCREEN_HEIGHT-1) {
    crashed=1;
  }
  for(int i=1; i <= bodyLength; i++)
  {
    if(body[0][0] == body[i][0] && body[0][1] == body[i][1])
    {
      crashed=1;
    }
  }
  if (crashed) {
    gameOver(); //Shows score, and calls the titleScreen() function to reinitialise everything for the next game
  }
}