// Deklarasi penggunaan library
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

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
}

void loop() {
  // Baca keypad
  readKp4x4();
}

void readKp4x4() { 
  // baca input keypad
  char customKey = kp4x4.getKey();
  if (customKey) {
      Serial.println(customKey);
  }
}
