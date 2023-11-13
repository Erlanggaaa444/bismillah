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

const int pins[8] = {30, 32, 34, 36, 38, 40, 42, 44}; // Change these to your desired pins
RelayModule relay(pins);

void setup()
{
    // No setup needed, the RelayModule constructor sets the pin modes
}

void loop()
{
    relay.setRelayState(0b00000001); // Turn on the 8th relay and turn off all others
    delay(500);
    relay.setRelayState(0b00000010); // Turn on the 7th relay and turn off all others
    delay(500);
    // And so on...
}