unsigned long previousMillis = 0;
float elapseTime = 0;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    interrupt();
    tickTimer();
}

void interrupt()
{
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        if (c == 's')
        {
            Serial.println("Interrupt receive 's' char");
            setTimer();
        }
    }
}

void setTimer()
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
                elapseTime = input * 60 * 1000;
                Serial.print("timer is set to : ");
                Serial.println(elapseTime);
                return false;
            }
        }
    }
}

void tickTimer()
{
    if (elapseTime > 0)
    {
        if (millis() - previousMillis >= elapseTime)
        {
            // put the function that want to run on each interval in here
            previousMillis = millis();
            Serial.println("Tick");
        }
    }
}
