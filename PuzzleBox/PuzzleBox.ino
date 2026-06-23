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
//portal variable
const int PORTAL = 5;

const int WIDTH = 20;
const int HEIGHT = 20;

// Initialize the library with the interface pins
// RS, Enable, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
unsigned long lastPressed = 0;
vec2 position;


// 20x20 Game Map (0 = Wall, 1 = Open Space)
char gameMap[WIDTH][HEIGHT] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,2,1,1,0,1,1,1,1,1,1,1,1,1,2,1,1,4,0},
  {0,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0},
  {0,1,0,1,1,1,3,1,1,1,1,1,0,1,0,1,1,1,1,0},
  {0,4,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0},
  {0,1,1,1,0,1,2,1,0,1,0,1,3,1,0,1,0,1,1,0},
  {0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0},
  {0,1,4,1,1,1,0,1,1,1,1,1,1,3,1,1,1,1,1,0},
  {0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0},
  {0,1,3,1,2,1,1,1,1,0,1,0,1,1,4,1,1,3,1,0},
  {0,1,0,0,0,0,1,0,2,0,1,0,1,0,0,0,0,0,1,0},
  {0,1,1,1,1,0,1,0,1,1,4,1,1,0,1,1,4,1,1,0},
  {0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0},
  {0,1,2,1,1,0,1,1,1,1,1,1,1,2,1,1,4,1,1,0},
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0},
  {0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0},
  {0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,0},
  {0,3,0,1,2,1,0,1,1,1,1,1,0,1,1,1,1,0,1,0},
  {0,1,1,1,0,1,1,4,0,0,0,1,1,1,0,1,1,1,4,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

char portalX[WIDTH][HEIGHT];
char portalY[WIDTH][HEIGHT];

vec2 portalPosition = {-1, -1};

unsigned long lastPrinted = 0;
int points = 30;
unsigned int curr_state = 2;

void print_position() {
  char numX[3] = "";
  char numY[3] = "";
  numToString(position.x, numX);
  numToString(position.y, numY);
  lcd.setCursor(0, 1);
  lcd.print(numX);  // Print X coordinate
  lcd.print(" ");   // Print the space in the middle
  lcd.print(numY);  // Print Y coordinate
}

void print_time() {
  lcd.setCursor(9, 0);
  lcd.print(millis() / 1000);
}
void print_points() {
  lcd.setCursor(0, 0);
  lcd.print(points);
  lcd.print("     ");
}

void print_curr_state() {
  lcd.setCursor(14, 1);
  lcd.print(curr_state);
}
void print_isMakingPortal() {
  lcd.setCursor(15, 1);
  if (portalPosition.x < 0) {
    lcd.print("N");
    return;
  }
    lcd.print("P");
}

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_PORTAL, INPUT_PULLUP);
  position = {1, 1};
}



bool validatePosition(vec2 position) {
  if (position.x < 0 || position.y < 0) return false;
  if (position.x > 20 || position.y > 20) return false;
  if (!gameMap[position.x][position.y]) return false;
  return true;
}

void makePortal() {
  if (points < 10) return;
  if (portalPosition.x < 0 || portalPosition.y < 0) {
    portalPosition = position;
    return;
  }
  if (isSame(position, portalPosition)) return;
  points -= 10;
  print_points();
  gameMap[portalPosition.x][portalPosition.y] = PORTAL;
  portalX[portalPosition.x][portalPosition.y] = position.x;
  portalY[portalPosition.x][portalPosition.y] = position.y;
  portalPosition = {-1, -1};
}

bool isSame(vec2 v1, vec2 v2) {
  if (v1.x == v2.x && v1.y == v2.y) return true;
  return false;
}

void adjustPosition(vec2 direction) {
  bool isTeleported = false;
  vec2 new_position = position;
  new_position.x += direction.x;
  new_position.y += direction.y;
  if (validatePosition(new_position)) position = new_position;
  do {
    print_position();
    delay(100);
    isTeleported = false;
    if (gameMap[position.x][position.y] == curr_state) {
      curr_state += 1;
      if (curr_state == 5) curr_state = 2;
      points++;
      print_points();
      delay(100);
    }
    if (gameMap[position.x][position.y] == PORTAL) {
      print_position();
      isTeleported = true;
      new_position.x = portalX[position.x][position.y];
      new_position.y = portalY[position.x][position.y];
      position = new_position;
    }
  } while (isTeleported);
}

void detect_buttons() {
  if (millis() - lastPressed < 200) return;
  int currentButtonUPState = digitalRead(BUTTON_UP);
  int currentButtonDOWNState = digitalRead(BUTTON_DOWN);
  int currentButtonLEFTState = digitalRead(BUTTON_LEFT);
  int currentButtonRIGHTState = digitalRead(BUTTON_RIGHT);
  int currentButtonPORTALState = digitalRead(BUTTON_PORTAL);
  if (currentButtonUPState == LOW) {
    lastPressed = millis();
    adjustPosition({1, 0});
  }
  if (currentButtonDOWNState == LOW) {
    lastPressed = millis();
    adjustPosition({-1, 0});
  }
  if (currentButtonLEFTState == LOW) {
    lastPressed = millis();
    adjustPosition({0, -1});
  }
  if (currentButtonRIGHTState == LOW) {
    adjustPosition({0, 1});
    lastPressed = millis();
  }
  if (currentButtonPORTALState == LOW) {
    makePortal();
    lastPressed = millis();
  }
}

void numToString(int num, char * str) { //only for two digit numbers
  str[0] = (num / 10) + '0';
  str[1] = (num % 10) + '0';
  str[2] = '\0';
}

void winnerScreen() {
  print_time();
  print_points();
  lcd.setCursor(0, 1);
  lcd.print("Winner!");
  //print_curr_state();
  //print_isMakingPortal();
  while (true) {
    delay(100);
  };
}

void loop() {

  if (millis() - lastPrinted > 100) {
    if (points >= 10000) winnerScreen();
    lastPrinted = millis();
    print_position();
    print_time();
    print_points();
    print_curr_state();
    print_isMakingPortal();
  }

  detect_buttons();
}
