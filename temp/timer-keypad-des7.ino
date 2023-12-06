/*
  task :
  - tambahkan placeholder output untuk mode timer
  - tambahkan mode sensor
*/

// Deklarasi kelas modul relay
class RelayModule
{
  private:
    int relayPins[8];

  public:
    RelayModule(int pins[8])
    {
      for (int i = 0; i < 8; i++)
      {
        relayPins[i] = pins[i];
        pinMode(relayPins[i], OUTPUT);
      }
    }

    void turnOn(int channel)
    {
      digitalWrite(relayPins[channel - 1], HIGH);
    }

    void turnOff(int channel)
    {
      digitalWrite(relayPins[channel - 1], LOW);
    }

    void turnAllOn()
    {
      for (int i = 0; i < 8; i++)
      {
        digitalWrite(relayPins[i], HIGH);
      }
    }

    void turnAllOff()
    {
      for (int i = 0; i < 8; i++)
      {
        digitalWrite(relayPins[i], LOW);
      }
    }

    void turnOnSpecific(int index)
    {
      if (index >= 0 && index < 8)
      {
        digitalWrite(relayPins[index], HIGH);
      }
    }

    void turnOffSpecific(int index)
    {
      if (index >= 0 && index < 8)
      {
        digitalWrite(relayPins[index], LOW);
      }
    }

    void turnOnConsecutively(int interval)
    {
      for (int i = 0; i < 8; i++)
      {
        digitalWrite(relayPins[i], HIGH);
        delay(interval);
        digitalWrite(relayPins[i], LOW);
      }
    }

    void setRelayState(int state)
    {
      for (int i = 0; i < 8; i++)
      {
        digitalWrite(relayPins[i], (state >> i) & 1 ? HIGH : LOW);
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
        }

        return true;
      }
      return false;
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
int pins[8] = {30, 32, 34, 36, 38, 40, 42, 44}; // Change these to your desired pins RELAY
const byte ROW_NUM = 4; //four rows
const byte COL_NUM = 4; //four columns
int mode = 0, selectedRelay = -1, timerValue = 0;
String savedString, inputString, input_value;
float sensor_value = 20.0f;
unsigned long previousMillis = 0;
const int pinSensor = A0;
uint8_t state = 0b00000001; // Initial state
byte pin_rows[ROW_NUM] = {39, 41, 43, 45}; //connect to the row pinouts of the keypad
byte pin_column[COL_NUM] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad
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
  for (int i = 30; i <= 44; i += 2)
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
  switch (mode)
  {
    sensor_kelembapan();
    case 0: // init state pilih relay yang akan dikontrol
      display_pilih_relay();
      listenmode0();
      break;
    case 1: // set threshold
      display_set_timer(sensor_value);
      listenmode1();
      break;
    case 2: // execute watering based on threshold
      tickTimer();
      display_main_countdown(sensor_value);
      listenmode2(selectedRelay);
      break;
  }
}


void checkInterrupt() {
  char key = readInput();
  if (key) {
    Serial.println(key);
    if (key == '*') { // If '*' key is pressed
      mode = 0;
      lcd.clear();
    }
  }
}


char readInput() {
  char key = customKeypad.getKey();
  if (key) {
    return key;
  }
  return '\0'; // Return null character if no key is pressed
}


void sensor_kelembapan()
{
  int sensorValue = analogRead(pinSensor);
  float voltage = sensorValue * (5.0 / 1023.0);
  float kelembapan = (voltage - 0.8) * 100.0 / (3.0 - 0.8);
  sensor_value = sensorValue;
}

void tickTimer()
{
  if (timer.hasTicked())
  {

    // Turn on the selected relay when the timer starts
    if (timer.getCountdownTime() == timer.getCountdownTime())
    {
      relay.turnOnSpecific(selectedRelay);
    }

    // Turn off the selected relay when the timer ends
    if (timer.getCountdownTime() <= 0)
    {
      relay.turnOffSpecific(selectedRelay);
    }
  }
}


void display_pilih_relay()
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Pilih Relay");
  lcd.setCursor(0, 1);
  lcd.print("1. Relay 1");
  lcd.setCursor(0, 2);
  lcd.print("2. Relay 2");
  lcd.setCursor(0, 3);
  lcd.print("3. Relay 3");
}


void display_set_timer(int sensor_value)
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Set Timer");
  lcd.setCursor(0, 1);
  lcd.print("Sensor : ");
  lcd.print(sensor_value);
  lcd.print(" %");
  lcd.setCursor(0, 2);
  lcd.print("Timer : ");
  lcd.print(timer.getCountdownTime());
  lcd.print(" s");
}

void display_main_countdown(int sensor_value)
{
  // display lcd
  lcd.setCursor(0, 0);
  lcd.print("Countdown");
  lcd.setCursor(0, 1);
  lcd.print("Sensor : ");
  lcd.print(sensor_value);
  lcd.print(" %");
  lcd.setCursor(0, 2);
  lcd.print("Timer : ");
  lcd.print(timer.getCountdownTime());
  lcd.print(" s");

}

void listenmode0()
{
  char input = readInput();
  if (input != '\0') // Check if the character is not null
  {
    if (isdigit(input)) // Check if the character is a digit
    {
      selectedRelay = input - '0'; // Convert char to int
      mode = 1;
      lcd.clear();
    }
  }
}


void listenmode1()

{
  String inputString = "";
  while (true)
  {
    char key = customKeypad.getKey();
    if (key)
    {
      if (key == '#') // '#' is pressed
      {
        break;
      }
      inputString += key;
    }
  }

  timerValue = inputString.toInt();
  mode = 2;
      lcd.clear();
}


void listenmode2(int relayChoice)
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= timerValue)
  {
    // save the last time the relay was toggled
    previousMillis = currentMillis;

    // if the relay is off turn it on and vice-versa
    if (digitalRead(pins[relayChoice]) == LOW)
    {
      digitalWrite(pins[relayChoice], HIGH);
    }
    else
    {
      digitalWrite(pins[relayChoice], LOW);
      mode = 0; // After one cycle, change mode back to 0
      lcd.clear();
    }
  }
}
