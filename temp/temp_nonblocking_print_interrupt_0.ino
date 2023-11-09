// template non blocking code with interrupt


int interval = 1000; // default interval in milliseconds
unsigned long lastPrintTime = 0; // last time the sentences were printed

void setup() {
  Serial.begin(9600); // initialize serial communication
  pinMode(A1, INPUT); // set A1 pin as input
}

void loop() {
  // check if it's time to print the sentences
  if (millis() - lastPrintTime >= interval) {
    printSentences();
    lastPrintTime = millis();
  }

  // check if the interrupt function was called
  if (interrupted()) {
    Serial.println("Printing interrupted");
    delay(1000); // wait for 1 second to avoid multiple interrupts
  }

  // read the analog value of A1 pin and map it to the interval range
  int analogValue = analogRead(A1);
  interval = map(analogValue, 0, 1023, 500, 5000);
}

void printSentences() {
  printSentenceWithInterrupt("This is sentence 1");
  printSentenceWithInterrupt("This is sentence 2");
  printSentenceWithInterrupt("This is sentence 3");
}


bool interrupted() {
  // check if the interrupt pin is high
  if (Serial.available() > 0) {
    return true;
  } else {
    return false;
  }
}



void printSentenceWithInterrupt(String sentence) {
  Serial.println(sentence);
  for (int i = 0; i < 5000; i++) {
    if (interrupted()) {
      Serial.println("Printing interrupted");
      return;
    }
    delay(1);
  }
}