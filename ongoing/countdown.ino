

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
        return true;
    }
    return false;
    }

    float getCountdownTime()
    {
    return (countdownTime - (millis() - previousMillis)) / 1000; // Return the remaining countdown time in seconds
    }
};



// Deklarasi kelas
Timer timer;
unsigned long previousMillis = 0; 

void setup() {
    Serial.begin(9600);
    timer.setElapsedTime(2);
}

void loop() {
  if (millis() - previousMillis >= 1000) { // If a second has passed
    previousMillis = millis(); // Update the last update time
    Serial.println(timer.getCountdownTime());
  }
}