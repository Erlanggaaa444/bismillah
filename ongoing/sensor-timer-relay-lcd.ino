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

  public:
    Timer() : previousMillis(0), elapsedTime(0) {}

    void setElapsedTime(float input)
    {
      elapsedTime = input * 60 * 1000;
    }

    bool hasTicked()
    {
      if (elapsedTime > 0 && millis() - previousMillis >= elapsedTime)
      {
        previousMillis = millis();
        return true;
      }
      return false;
    }
};

// Deklarasi library
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// deklarasi variabel
int pins[8] = {30, 32, 34, 36, 38, 40, 42, 44}; // Change these to your desired pins
const byte ROW_NUM = 4; //four rows
const byte COL_NUM = 4; //four columns
int mode = 0;
String savedString, inputString, input_value;
float sensor_value = 20.0f;
const int pinSensor = A0; 
uint8_t state = 0b00000001; // Initial state
byte pin_rows[ROW_NUM] = {39, 41, 43, 45}; //connect to the row pinouts of the keypad
byte pin_column[COL_NUM] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad
char keys[ROW_NUM][COL_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};


// Deklarasi kelas
Timer timer;
RelayModule relay(pins);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COL_NUM); 



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
      pinMode(pinSensor, INPUT);  
}

void loop()
{
  // check interrupt
  checkInterrupt(); /// intterupt handler

  // check current mode
  checkmode(mode);
}

char readInput()
{
  char key = keypad.getKey();
  if (key)
  {
    Serial.println(key);
    return key;
  } else {
    return 0;
  }
}

int readKp4x4() { 
    while (true) {
        // Read input from the keypad
        char customKey = keypad.getKey();
        if (customKey) {
            // If the key is '#', save the input string and clear it
            if (customKey == '#') {
                savedString = inputString;
                Serial.print("savedString = "); Serial.println(savedString);
                inputString = "";
                int number = savedString.toInt();
                return number;
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
}


void checkInterrupt()
{
  char key = readInput();

  // call listenmode0 if char is 0
  if (key == '0')
  {
    mode = 0;
  }
}

void tickTimer()
{
  if (timer.hasTicked())
  {
    Serial.println("Tick");

    // shift bit setiap pengulangan
    state = (state << 1) | (state >> (8 - 1));

    // reset ketika state kosong (bernilai 0b00000000)
    if (state == 0)
    {
      state = 0b00000001;
    }

    relay.setRelayState(state);
  }
}

void sensor_kelembapan()
{
    int sensorValue = analogRead(pinSensor); 
    float voltage = sensorValue * (5.0 / 1023.0);
    float kelembapan = (voltage - 0.8) * 100.0 / (3.0 - 0.8);
    sensor_value = sensorValue;
    Serial.println(sensor_value);
}

void checkMoist(float sensor_value)
{
  if(sensor_value < 500)
  {
    relay.turnAllOn();
//    timer.setElapsedTime(0.1); // set fixed timer elapse 1 min
  } else {
    relay.turnAllOff();
  }
}

void checkmode(int mode)
{
  sensor_kelembapan();
  switch (mode)
  {
    case 0: // init state / piilh mode
      tickTimer();
      display_pilih_mode();
      listenmode0();
      break;
    case 1: // mode sensor
//      tickTimer();
      checkMoist(sensor_value);
      display_main_sensor(sensor_value);
      listenmode1();
      break;
    case 2: //
      tickTimer();
      display_main_timer(sensor_value);
      listenmode2();
      break;
    case 3: //
      display_set_timer();
      listenmode3();
      break;
    default:
      listenmode0();
      break;
  }
}

void listenmode0()
{
  char key = readInput();

  switch (key)
  {
    case '1': // pilih mode sensor
      lcd.clear();
      mode = 1;
      checkMoist(sensor_value);
      break;
    case '2': // pilih mode timer
      lcd.clear();
      mode = 3;
      break;
  }
}

// set of listen mode call
void listenmode1()
{
  char key = readInput();

  switch (key)
  {
    case '0': // pilih mode sensor
      lcd.clear();
      mode = 0;
      break;
    case '1': // pilih mode sensor
      // kode untuk mengubah threshold sensor
      break;
  }
}

void listenmode2()
{
  char key = readInput();

  switch (key)
  {
    case '0': // pilih mode sensor
      lcd.clear();
      mode = 0;
      break;
  }
}

void listenmode3()
{
  Serial.println("input timer elapse time");
  readKp4x4(); // Read input from the keypad
  int input = savedString.toInt(); // Convert the saved string to an integer
  if (input > 0)
  {
    timer.setElapsedTime(input);
    Serial.print("timer is set to : ");
    Serial.println(input);
    lcd.clear();
    mode = 2;
  }  
}

// set of lcd display call
void display_main_sensor(int sensor_value)
{
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

void display_main_timer(int sensor_value)
{
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

void display_inisialisasi()
{
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Inisialisasi ... ");
}

void display_pilih_mode()
{
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Pilih Mode");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("1=Sensor   2=Timer");
}

void display_set_timer()
{
  // line 2
  lcd.setCursor(1, 1);
  lcd.print("Set Timer ...Mins");

  // line 3
  lcd.setCursor(1, 2);
  lcd.print("Input ...");
}

// set of action
