
// tambahkan library
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// deklarasikan kelas relay
class RelayModule
{
  private:
    int relayPins[8];

  public:
    RelayModule(int* pins) {
      // Initialize the relay pins
      for (int i = 30; i <= 44; i+2) {
        pinMode(pins[i], OUTPUT);
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


// deklarasi pin relay
int pins[8] = {30, 32, 34, 36, 38, 40, 42, 44}; // Change these to your desired pins RELAY


// dekllarasi pin sensor
const int pinSensorAnalog = A0;
const int pinSensorDigital = 26;

// deklarasi jumlah baris dan kolom keypad
const byte ROWS = 4; 
const byte COLS = 4; 

// deklarasi layout keypad
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// deklarasi pin keypad
byte rowPins[ROWS] = {39, 41, 43, 45}; 
byte colPins[COLS] = {31, 33, 35, 37}; 

// buat instance keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x27, 20, 4);
RelayModule relay(pins);
Timer timer;


void setup(){
  // inisialisasi serial monitor dan lcd
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  // deklarasi mode pinout sensor
  pinMode(pinSensorAnalog, INPUT);

  // deklarasi mode pinout relay
  for (int i = 30; i <= 44; i += 2)
  {
    pinMode(i, OUTPUT);
  }

}

void loop(){
  char customKey = readKeypad();

  if (customKey){
    Serial.println(customKey);
  }
}

char readKeypad() 
{
  char customKey = customKeypad.getKey();
  return customKey;
}