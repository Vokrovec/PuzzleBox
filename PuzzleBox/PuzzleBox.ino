#include <LiquidCrystal.h>

struct vec2 {
  int x;
  int y;
};

const int BUTTON_UP    = 6;
const int BUTTON_DOWN  = 7;
const int BUTTON_LEFT  = 8;
const int BUTTON_RIGHT = 9;
const int BUTTON_PORTAL = 10;

// Initialize the library with the interface pins
// RS, Enable, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
unsigned long lastPressed = 0;
vec2 position;


// 20x20 Game Map (0 = Wall, 1 = Open Space)
const char gameMap[20][20] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0},
  {0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0},
  {0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0},
  {0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,0},
  {0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0},
  {0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0},
  {0,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0},
  {0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0},
  {0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0},
  {0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0},
  {0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,0},
  {0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,0},
  {0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

unsigned long lastPrinted = 0;

void print_position() {
  char numX[3] = "";
  char numY[3] = "";
  numToString(position.x, numX);
  numToString(position.y, numY);
  lcd.setCursor(0, 1);
  char * text = strcat(strcat(numX, " "), numY);
  lcd.print(text);
}

void print_time() {
  lcd.setCursor(9, 0);
  lcd.print(millis() / 1000);
}

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  position = {1, 1};
}



bool validatePosition(vec2 position) {
  if (position.x < 0 || position.y < 0) return false;
  if (position.x > 20 || position.y > 20) return false;
  if (!gameMap[position.x][position.y]) return false;
  return true;
}

vec2 adjustPosition(vec2 direction) {
  vec2 new_position = position;
  new_position.x += direction.x;
  new_position.y += direction.y;
  if (validatePosition(new_position)) return new_position;
  return position;
}

void detect_buttons() {
  if (millis() - lastPressed < 100) return;
  int currentButtonUPState = digitalRead(BUTTON_UP);
  int currentButtonDOWNState = digitalRead(BUTTON_DOWN);
  int currentButtonLEFTState = digitalRead(BUTTON_LEFT);
  int currentButtonRIGHTState = digitalRead(BUTTON_RIGHT);
  if (currentButtonUPState == LOW) {
    lastPressed = millis();
    position = adjustPosition({1, 0});
  }
  if (currentButtonDOWNState == LOW) {
    lastPressed = millis();
    position = adjustPosition({-1, 0});
  }
  if (currentButtonLEFTState == LOW) {
    lastPressed = millis();
    position = adjustPosition({0, -1});
  }
  if (currentButtonRIGHTState == LOW) {
    position = adjustPosition({0, 1});
    lastPressed = millis();
  }
}

void numToString(int num, char * str) { //only for two digit numbers
  str[0] = (num / 10) + '0';
  str[1] = (num % 10) + '0';
  str[2] = '\0';
}

void loop() {
  detect_buttons();
  if (millis() - lastPrinted > 100) {
    lastPrinted = millis();
    print_position();
    print_time();
  }
}
