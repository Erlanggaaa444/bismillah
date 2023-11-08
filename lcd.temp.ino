// Deklarasi library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <arduino-timer.h>

// Deklarasi LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Deklarasi variabel
String input_value;
long timerValue = 0;
int sensor_value = 50;
int mode = 1;
int last_mode;

// Create a timer object
Timer<10, millis> myTimer;


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
  // perbaharui nilai pembacaan sensor

  // cek mode
  checkmode(mode);

  // Update the timer
  myTimer.tick();

}


void checkmode(int mode) {
  if (last_mode != mode) {
    lcd.clear();
  }
  if (mode == 0) {
    display_pilih_mode();
    listenmode_0();
  }  else if (mode == 1) {
    display_main_sensor(sensor_value, input_value.charAt(0));
    listenmode_1();
  } else if (mode == 2) {
    display_main_timer(sensor_value, input_value.charAt(0));
    listenmode_1();
  } else if (mode == 3) {
    display_set_timer();
    listenmode_2();
  }
  last_mode = mode;
}

void listenmode_0() {
  // Baca input serial monitor
  if (Serial.available() > 0) {
    char input_value = Serial.read();
    Serial.println(input_value);

    if (input_value == '1') {
      mode = 1;
    } else if (input_value == '2') {
      mode = 3;
    }
  }
}

void listenmode_1() {
  // Baca input serial monitor
  if (Serial.available() > 0) {
    char input_value = Serial.read();
    Serial.println(input_value);

    if (input_value == '0') {
      display_pilih_mode();
      mode = 0;
    }
  }
}


void listenmode_2() {
  // Baca input serial monitor
  static String input_value = "";
  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    
    if (isdigit(c)) {
      input_value += c;
    } else if (c == '\n' && input_value.length() > 0) {
      timerValue = input_value.toInt();
      input_value = "";
      Serial.print("Timer value: ");
      Serial.println(timerValue);

      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("Timer set to ");
      lcd.print(timerValue);
      
      // Start the timer
      myTimer.in(timerValue * 1000, timerCallback);
      mode = 2;
    }
  }
}


void display_main_timer(int sensor_value, char input_value) {
  // line 1
  lcd.setCursor(1, 0);
  lcd.print(String("Mode :Timer ") + input_value + String(" Min"));

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + sensor_value + String("%"));

  // line 4
  lcd.setCursor(1, 3);
  lcd.print("0=Pilih Mode");
}

void display_main_sensor(int sensor_value, char input_value) {
  // line 1
  lcd.setCursor(1, 0);
  lcd.print("Mode :Sensor");

  // line 2
  lcd.setCursor(1, 1);
  lcd.print(String("Humid:") + sensor_value + String("%"));

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

// Timer callback
bool timerCallback(void* opaque) {
  Serial.println("Timer expired!");
  return true; // Repeat the timer
}
