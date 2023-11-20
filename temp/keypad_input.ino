// Deklarasi penggunaan library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Deklarasi dimensi keypad (baris dan kolom)
#define ROWS 4
#define COLS 4

// Deklarasi karakter keypad
const char kp4x4Keys[ROWS][COLS]   = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte rowKp4x4Pin [4] = {9, 8, 7, 6};
byte colKp4x4Pin [4] = {5, 4, 3, 2};

// Deklarasi keypad
Keypad kp4x4  = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);

// Deklarasi LCD
LiquidCrystal_I2C lcd(0x27,20,4); 

// Deklarasi variable
String input_value, savedString, inputString;
long int timer = 0; 

// Deklarasi pin 
int relay_pin = A0;

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(9600);

  // Inisialisasi LCD
  lcd.init();

  // Inisialisasi pinMode
  pinMode(relay_pin, OUTPUT);
}

void loop() {
  // Baca input keypad
  readKp4x4();

}

// fungsi membaca input keypad
void readKp4x4() { 
  // Read input from the keypad
  char customKey = kp4x4.getKey();
  if (customKey) {
      // If the key is '#', save the input string and clear it
      if (customKey == '#') {
          savedString = inputString;
          Serial.print("savedString = "); Serial.println(savedString);
          inputString = "";
      } 
      // If the key is 'C', remove the last character from the input string
      else if (customKey == 'C') {
          if (inputString.length() > 0) {
              inputString.remove(inputString.length() - 1);
          }
      } 
      else {
          // Otherwise, append the key to the input string
          inputString += customKey;
      }
      Serial.println(savedString);
  }
}
