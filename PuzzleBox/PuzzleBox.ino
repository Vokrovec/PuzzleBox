#include <LiquidCrystal.h>

// Initialize the library with the interface pins
// RS, Enable, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void print_text(const char * text) {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(text);
}

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  print_text("Ahoj svete!");
}


void loop() {
  delay(10000);
  print_text("Print!");
}
