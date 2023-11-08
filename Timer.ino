#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,20,4); 
int relay_pin = A0;

long int timer = 0;

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


String input_value;

void setup() {

  lcd.init();                      
  lcd.init();
  pinMode(relay_pin, OUTPUT);

  Serial.begin(9600);
  input_value.reserve(32); // maximum input characters is 33, change if needed
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    lcd.print(key);

    if (key == '*') {
      lcd.clear();
    } else if (key == '#') {

      timer = input_value.toInt();


     timer = timer * 60; 

      digitalWrite(relay_pin, HIGH);
      lcd.print("relay on");

      delay(timer);
      digitalWrite(relay_pin, LOW);
      lcd.print("relay off");

      timer = 0;

      lcd.clear();
    } else {
      input_value += key; // append new character to input value string
    }
  }
}