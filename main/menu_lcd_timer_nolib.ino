/*
task :
- tambahkan placeholder output untuk mode timer
- tambahkan mode sensor



*/

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
#include <LiquidCrystal_I2C.h>

// Deklarasi LCD
Timer timer;
LiquidCrystal_I2C lcd(0x27, 20, 4);

// deklarasi variabel
int mode = 0;
float sensor_value = 20.0f;
const int pinSensor = A0; 
const int relay = 7;  
    

void setup()
{
    // inisialisasi serial
    Serial.begin(9600);

    // Inisialisasi LCD
    lcd.init();
    lcd.backlight();

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
    if (Serial.available() > 0)
    {
        char key = Serial.read();
        return key;
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
    // Serial.print("tick");
    if (timer.hasTicked())
    {
        // taruh template kode output di sini; kode akan dieksekusi setiap timer tick (sesuai time elapse yang ditentukan)
        Serial.println("Tick");
    }
}

void sensor_kelembapan()
{
    int sensorValue = analogRead(pinSensor); 
    float voltage = sensorValue * (5.0 / 1023.0);
    float kelembapan = (voltage - 0.8) * 100.0 / (3.0 - 0.8);
    sensor_value = kelembapan;
    Serial.println(sensor_value);
}


void checkmode(int mode)
{
    switch (mode)
    {
    case 0: // init state / piilh mode
        tickTimer();
        display_pilih_mode();
        listenmode0();
        break;
    case 1: // mode sensor
        sensor_kelembapan();
        display_main_sensor(sensor_value);
        listenmode1();
        break;
    case 2: // mode timer
        tickTimer();
        display_main_timer(sensor_value);
        listenmode2();
        break;
    case 3: // Set timer elapse
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
    while (true)
    {
        if (Serial.available() > 0)
        {
            String str = Serial.readString();
            float input = str.toFloat();
            Serial.println(input);
            if (input > 0)
            {
                float timeSet = input;
                timer.setElapsedTime(timeSet);
                Serial.print("timer is set to : ");
                Serial.println(timeSet);
                lcd.clear();
                mode = 2;
                break;
            }
        }
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