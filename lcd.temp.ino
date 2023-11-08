// Deklarasi penggunaan library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Deklarasi LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Deklarasi variabel
String input_value;
long timer = 0;
int sensor_value = 50;

// Deklarasi pin
// int relay_pin = A0;

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(9600);
  Serial.println(F("Initialize System"));

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Memulai program");

  // Inisialisasi pinMode
  //  pinMode(relay_pin, OUTPUT);
}

void loop() {
  // Baca input serial monitor
  if (Serial.available() > 0) {
    char input_value = Serial.read();
    Serial.println(input_value);

    if (input_value == '1') {
      display_main_timer(sensor_value, input_value);
    } else if (input_value == '2') {
      display_2(sensor_value, input_value);
    } else if (input_value == '3') {
      display_3();
    } else if (input_value == '4') {
      display_4();
    } else if (input_value == '5') {
      display_5();
    }
  }



// Perbaharui menu


/*
  jike menu = 1
  jalankan menu 1

  jika menu = 2
  jalankan menu 2

  jika menu = 3
  jalankan menu 3

  jika menu = 4
  jalankan menu 4
*/



}


void display_main_timer(int sensor_value, char input_value) {
  lcd.clear();

  // line 1
  lcd.setCursor(1, 0);
  lcd.print(String("Mode :Timer ") + input_value + String("Min"));

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + sensor_value + String("%"));

  // line 4
  lcd.setCursor(1, 3);
  lcd.print("0=Pilih Mode");
}

void display_2(int sensor_value, char input_value) {
  lcd.clear();

  // line 1
  lcd.setCursor(1, 0);
  lcd.print(String("Mode :Sensor ") + input_value + String("Min"));

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + sensor_value + String("%"));

  // line 4
  lcd.setCursor(1, 3);
  lcd.print("0=Pilih Mode");
}

void display_3() {
  lcd.clear();

  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Inisialisasi ... ");

}

void display_4() {
  lcd.clear();

  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Pilih Mode");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("1=Timer   2=Sensor");

}

void display_5() {
  lcd.clear();

  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Set Timer ...Mins");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("Input ...");

}

// fungsi perbaharui menu



// fungsi menu (tergantung pada menu yang dipilih terdapat aksi: set timer, menyalakan relay, reset)




