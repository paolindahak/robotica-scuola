#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#define ROWS 4
#define COLS 4
#define PASSWORD_MAX_LENGTH 16
#define LED_PIN 2
#define BLINK_DELAY 500

char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char password[] = "123A";
char input[PASSWORD_MAX_LENGTH+1];
int index = 0;
bool auth = false;
unsigned long blinkTimestamp = 0;
unsigned long cursorTimestamp = 0;
bool showCursor = true;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(LED_PIN, OUTPUT);
  clearInput();
}

void loop() {
  char key = keypad.getKey();

  if (auth && key == '#') {
    digitalWrite(LED_PIN, LOW);
    auth = false;  
    clearInput(); 
    return; 
  }
  
  if (auth) {
    blink();
    return;    
  }

  if (key == '*') {
    clearInput();
  } else if (key == '#') {
    checkPassword();
  } else if (key != NO_KEY) {
    addChar(key);
  }

  if ((millis() - cursorTimestamp) > 500 && index < 16) {
    lcd.setCursor(index, 1);
    if (showCursor) {
      lcd.print("_");
    } else {
      lcd.print(" ");
    }
    showCursor = !showCursor;
    cursorTimestamp = millis();
  }
  
  delay(10);  
}

void clearInput() {
  for (int i = 0; i < PASSWORD_MAX_LENGTH + 1; i++) {
    input[i] = 0;
  }
  index = 0;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("    PASSWORD    ");
}

void checkPassword() {
  if (strcmp(input, password) == 0) {
    lcd.setCursor(0, 0);
    lcd.print("    ACCESSO     ");
    lcd.setCursor(0, 1);
    lcd.print("   CONSENTITO   ");
    auth = true;
  } else {
    lcd.setCursor(0, 0);
    lcd.print("    ACCESSO     ");
    lcd.setCursor(0, 1);
    lcd.print("    NEGATO      ");
    delay(1000);
    clearInput();
  }
}

void addChar(char key) {
  if (index < 16) {
    lcd.setCursor(index, 1);
    lcd.print("*");
  }
  input[index++] = key;
  if (index >= PASSWORD_MAX_LENGTH) {
    checkPassword();
  }
}

void blink() {
  if ((millis() - blinkTimestamp) > BLINK_DELAY) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    blinkTimestamp = millis();
  }
}
