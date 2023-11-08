// non-blocking-countdown

unsigned long startTime = 0;
unsigned long countdownTime = 10000; // 10 seconds
bool isCountingDown = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Start the countdown when a character is received on the serial input
  if (Serial.available() > 0) {
    while(Serial.available() > 0) {
      Serial.read(); // read and discard the input
    }
    startTime = millis();
    isCountingDown = true;
  }

  // If we're counting down, print the remaining time every second
  if (isCountingDown) {
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime >= countdownTime) {
      Serial.println("Countdown finished!");
      isCountingDown = false;
    } else if (elapsedTime % 1000 < 50) { // print approximately every second
      Serial.println((countdownTime - elapsedTime) / 1000);
    }
  }

  // Print a message every 5 seconds, but allow it to be interrupted by the countdown
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime >= 5000) {
    printMessageWithInterrupt();
    lastPrintTime = millis();
  }
}

void printMessageWithInterrupt() {
  for (int i = 0; i < 5; i++) {
    if (isCountingDown) {
      return;
    }
    Serial.println("Printing message...");
    delay(1000);
  }
}