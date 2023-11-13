// Deklarasi library
#include "arduino-timer.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Deklarasi LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);


Timer<> timer; // Create a timer
Timer<>::Task water_relay_task; 

// deklarasi variabel
int mode = 0;
float sensor_value = 20.0f;


void setup() {
  // inisialisasi serial
  Serial.begin(9600);
}

void loop() {
  // check interrupt
  checkInterrupt(); /// intterupt handler

  // check current mode
  checkmode(mode);





}

char readInput () {
  if (Serial.available() > 0) {
    char key = Serial.read();
    return key;
  }
}


void checkInterrupt() {
  char  key = readInput();

  // call listenmode0 if char is 0
  if (key == '0') {
    mode = 0;
  }
}



void checkmode( int mode ) {
  switch (mode) {
    case 0: // init state / piilh mode
      display_pilih_mode();
      listenmode0();
      break;
    case 1: // mode sensor
      display_main_sensor(sensor_value);
      listenmode1();
      break;
    case 2: //
      display_main_timer(sensor_value);
      listenmode2();
      timer.tick();
      break;
    case 3: //
      timer.cancel(water_relay_task);
      display_main_timer(sensor_value);
      listenmode2();
      break;
    default:
      listenmode0();
      break;
  }
}


void listenmode0() {
  char key = readInput();

  switch (key) {
    case '1': // pilih mode sensor
      mode = 1;
      break;
    case '2': // pilih mode timer
      mode = 3;
      break;
  }

}


// set of listen mode call
void listenmode1() {
  char key = readInput();

  switch (key) {
    case '0': // pilih mode sensor
      mode = 0;
      break;
    case '1': // pilih mode sensor
      // kode untuk mengubah threshold sensor
      break;
  }
}


void listenmode2() {
  char key = readInput();

  switch (key) {
    case '0': // pilih mode sensor
      mode = 0;
      break;
    case '1': // pilih mode sensor
      // kode untuk mengubah threshold timer
      break;
  }
}

void listenmode3() {
  if (Serial.available() > 0) {
    int time_interval = Serial.parseInt();

    // You can now use time_interval in your code
    water_relay_task = timer.every(time_interval, water_relay_timer);
    mode = 2;
  }
}



// set of lcd display call
void display_main_sensor(int sensor_value) {
  // line 1
  lcd.setCursor(1, 0);
  lcd.print("Mode :Sensor");

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + String(sensor_value) + String("%"));

  // line 4
  lcd.setCursor(1, 3);
  lcd.print("0=Pilih Mode");
}

void display_main_timer(int sensor_value) {
  // line 1
  lcd.setCursor(1, 0);
  lcd.print("Mode :Timer");

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + String(sensor_value) + String("%"));

  // line 4
  lcd.setCursor(1, 3);
  lcd.print("0=Pilih Mode");
}

void display_inisialisasi() {
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Inisialisasi ... ");

}

void display_pilih_mode() {
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Pilih Mode");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("1=Sensor   2=Timer");

}

void display_set_timer() {
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Set Timer ...Mins");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("Input ...");

}


// set of action
bool water_relay_timer(void*) {
  // do something
  Serial.println("this is water relay timer");
}

bool water_relay_sensor(void*) {
  // do something
  Serial.println("this is water relay sensor");
}

bool (*functionPtr)(void*) = water_relay_timer;
