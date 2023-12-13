/*
  task :
  - tambahkan placeholder output untuk mode timer
  - tambahkan mode sensor
*/

// Deklarasi kelas modul relay
class RelayModule
{
  private:
    int relayPins[3];

  public:
    RelayModule(int pins[3])
    {
      for (int i = 0; i < 3; i++)
      {
        relayPins[i] = pins[i];
        pinMode(relayPins[i], OUTPUT);
      }
    }

    void turnOff()
    {
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(relayPins[i], LOW);
      }
    }

    void turnOffExcept(int relayNumber)
    {
      for (int i = 0; i < 3; i++)
      {
        if (i != relayNumber)
        {
          digitalWrite(relayPins[i], LOW);
        }
        else {
          digitalWrite(relayPins[i], HIGH);
        }
      }
    }


};

// delkarasikan kelas timer
class Timer
{
  private:
    unsigned long previousMillis;
    float elapsedTime;
    float countdownTime; // New variable to save the countdown time
    bool hasTickedFlag;

  public:
    Timer() : previousMillis(0), elapsedTime(0), countdownTime(0) {}



    void setElapsedTime(float input)
    {
      elapsedTime = input * 60 * 1000;
      countdownTime = elapsedTime; // Update the countdown time when elapsedTime is set
    }

    bool hasTicked()
    {
      if (elapsedTime > 0 && millis() - previousMillis >= elapsedTime)
      {
        previousMillis = millis();
        countdownTime -= elapsedTime; // Decrease the countdown time

        if (countdownTime <= 0) // If countdown time is down to 0
        {
          countdownTime = elapsedTime; // Reset it to the last set elapsed time
          hasTickedFlag = true; // Set the hasTickedFlag to true
        }
      }
      bool returnValue = hasTickedFlag;
      hasTickedFlag = false; // Reset the hasTickedFlag to false
      return returnValue;
    }

    float getCountdownTime()
    {
      return (countdownTime - (millis() - previousMillis)) / 1000; // Return the remaining countdown time in seconds
    }
};



// Deklarasi library
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// deklarasi variabel
int pins[8] = {30, 32, 34, 36, 38, 40, 42, 46}; // Change these to your desired pins RELAY

// deklarasi baris dan kolom keypad
const byte ROW_NUM = 4;
const byte COL_NUM = 4;

// deklarasi variabel integer
int mode = 0, selectedRelay = -1, timerValue = 0, switch_state = 0;
const int pinSensor = A0;
uint8_t state = 0b00000001; // Initial state

// deklarasi variabel string
String savedString, inputString, input_value;

// deklarasi variabel int
int sensor_value = 20.0, mapped_value = 0, moist_value = 0, low_moist = 0, high_moist = 0, moist_threshold = 0, thresholdValue = 0, elapseValue = 1;

// deklarasi variabel long
unsigned long previousMillis = 0;

// deklarasi array pin keypad
byte pin_rows[ROW_NUM] = {39, 41, 43, 44};
byte pin_column[COL_NUM] = {31, 33, 35, 37};

// deklarasi array karakter keypad
char keys[ROW_NUM][COL_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


// Deklarasi kelas
Timer timer;
RelayModule relay(pins);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Keypad customKeypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COL_NUM);


void setup()
{
  // inisialisasi serial
  Serial.begin(115200);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  // set pinmode output
  for (int i = 30; i <= 38; i += 2)
  {
    pinMode(i, OUTPUT);
  }

  // set pin sensor input
  pinMode(pinSensor, INPUT);
}

void loop () {
  checkInterrupt();
  checkmode(mode);
}

void checkmode(int mode)
{
  sensor_kelembapan();
  switch (mode)
  {
    case 0:
      listenmode0_low_moist();
      break;
    case 1:
      listenmode0_high_moist();
      break;
    case 2:
      display_set_threshold(mapped_value);
      listenmode1_threshold();
      break;
    case 3:
      display_set_timer();
      listenmode1_timer();
      break;
    case 4:
      tickTimer();
      display_main_screen(mapped_sensor_value(sensor_value, low_moist, high_moist));
      listenmode2();
      break;
  }
}


void checkInterrupt() {
  char key = customKeypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '*') { // If '*' key is pressed
      mode = 0;
      lcd.clear();
    }
  }
}

void sensor_kelembapan()
{
  sensor_value = analogRead(pinSensor);
}

int mapped_sensor_value(int sensor_value, int lowest_value, int highest_value)
{
  // mapped_value = map(sensor_value, lowest_value, highest_value, 0, 100);
  mapped_value = constrain(map(sensor_value, low_moist, high_moist, 0, 100), 0, 100);
  return mapped_value;
}

void tickTimer()
{
  if (timer.hasTicked())
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= timerValue)
    {
      previousMillis = currentMillis;
    }
  }
}

// display lcd


void display_set_threshold(int moistvalue)
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Set Threshold   ");
  lcd.setCursor(0, 1);
  lcd.print("Current Moist : ");
  lcd.print(moistvalue);
  lcd.print(" %");
  lcd.setCursor(0, 2);
  lcd.print("Threshold : .......");
}

void display_set_timer()
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Set Timer");
  lcd.setCursor(0, 2);
  lcd.print("Timer : ....... ");
  lcd.print(" min");
}

void display_main_screen(int moist_value)
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Countdown");
  lcd.setCursor(0, 1);
  lcd.print("Moist : ");
  lcd.print(moist_value);
  lcd.print(" %   ");
  lcd.setCursor(0, 2);
  lcd.print("Timer : ");
  lcd.print(timer.getCountdownTime());
  lcd.print(" s   ");
  lcd.setCursor(0, 3);
  lcd.print("Threshold : ");
  lcd.print(moist_threshold);

}


float readKp4x4_set(int set_value)  {
  if (set_value == thresholdValue) {
    display_set_threshold(mapped_value);
  }
  else if (set_value == elapseValue) {
    display_set_timer();
  }


  while (true) {
    sensor_kelembapan();
    mapped_value = map(sensor_value, low_moist, high_moist, 0, 100);



    // Read input from the keypad
    char customKey = customKeypad.getKey();
    if (customKey) {
      // If the key is '#', save the input string and clear it
      if (customKey == '#') {
        savedString = inputString;
        float number = savedString.toFloat();
        inputString = "";
        return number;
      }
      // If the key is 'C', remove the last character from the input string
      else if (customKey == 'C') {
        if (inputString.length() > 0) {
          inputString.remove(inputString.length() - 1);
          lcd.clear();

          if (set_value == thresholdValue) {
            display_set_threshold(mapped_value);
            lcd.setCursor(12, 2);
            lcd.print(inputString);
          }
          else if (set_value == elapseValue) {
            display_set_timer();
            lcd.setCursor(7, 2);
            lcd.print(inputString);
          }
        }
      }
      else {
        // Otherwise, append the key to the input string
        inputString += customKey;
        if (set_value == thresholdValue) {
          display_set_threshold(mapped_value);
          lcd.setCursor(12, 2);
        }
        else if (set_value == elapseValue) {
          display_set_timer();
          lcd.setCursor(7, 2);
        }
        lcd.print(inputString);
        delay(500); // Add a short delay to ensure the inputted character is displayed
      }
      Serial.println(savedString);
    }
  }
}

void listenmode0_low_moist() {
  int senval = analogRead(pinSensor);
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500; // 500ms interval

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    lcd.setCursor(0, 0);
    lcd.print("Nilai Sensor : ");
    lcd.print(senval);
    lcd.print("  ");

    lcd.setCursor(0, 1);
    lcd.print("tekan '#' untuk atur");
    lcd.setCursor(0, 2);
    lcd.print("nilai batas bawah");
    lcd.setCursor(0, 3);
    lcd.print("kelembapan");
  }

  if (customKeypad.getKey() == '#') {
    low_moist = senval;
    mode = 1;
  }
}

void listenmode0_high_moist() {
  int senval = analogRead(pinSensor);
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500; // 500ms interval

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    lcd.setCursor(0, 0);
    lcd.print("Nilai Sensor : ");
    lcd.print(senval);
    lcd.print("  ");

    lcd.setCursor(0, 1);
    lcd.print("tekan '#' untuk atur");
    lcd.setCursor(0, 2);
    lcd.print("nilai batas atas ");
    lcd.setCursor(0, 3);
    lcd.print("kelembapan");
  }

  if (customKeypad.getKey() == '#') {
    high_moist = senval;
    mode = 2;
    lcd.clear();
  }

}


void listenmode1_threshold()
{
  moist_threshold = (int) readKp4x4_set(thresholdValue);

  mode = 3;
  lcd.clear();
}

void listenmode1_timer()
{
  float elapseTimeSet = readKp4x4_set(elapseValue);

  if (elapseTimeSet > 0) {
    timerValue = elapseTimeSet * 60; // Convert minutes to milliseconds
    timer.setElapsedTime(elapseTimeSet);
  }

  mode = 4;
  lcd.clear();
}

void listenmode2()
{
  if (mapped_value < moist_threshold)
  {
    if (timer.hasTicked()) {
      relay.turnOffExcept(switch_state);
      switch_state += 1;

      // reset switch_state
      if (switch_state > 3) {
        switch_state = 0;
      }
    }
    else
    {
      relay.turnOff();
    }
  }
}
