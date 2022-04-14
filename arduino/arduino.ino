/**
 * @file arduino.ino
 * @author cole fuerth
 * @brief debounced input pullups for inputs, and toggleable outputs
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define BRIGHTS_IN 12
#define BRIGHTS_OUT 5
#define LT_IN 10
#define LT_OUT 11
#define RT_IN 8
#define RT_OUT 9
#define UNDERGLOW_OUT 3

#define SPARE1_IN 6
#define SPARE2_IN 4
#define SPARE3_IN 2
#define SPARE1_OUT 7

/**
 * @brief debounced INPUT_PULLUP on a pin; produces a state for outputs to use
 *
 */
class TOGGLE_BUTTON
{
public:
    bool state, input;
    uint8_t pin;
    uint16_t debounce;

    TOGGLE_BUTTON(uint8_t pin, uint16_t debounce_delay = 20)
    {
        this->pin = pin;
        this->debounce = debounce_delay;
        this->state = 0;
        this->input = 0;
    }

    boolean update_state()
    {
        if (oneShot(debounce_func(!digitalRead(pin))))
        {
            state = !state;
        }
        return state;
    }

private:
    boolean OSB, timerInSession, timerMemory;
    unsigned long timerTimer;

    boolean debounce_func(boolean in)
    {
        if (TON(in != input, debounce))
        {
            input = in;
            Serial.println("pin " + String(pin) + " input " + String(input ? "HIGH" : "LOW"));
        }
        return input;
    }

    boolean oneShot(boolean precond)
    {
        // use global memory to keep track of oneshot bits
        if (precond == 1 && this->OSB == 0)
        {
            this->OSB = 1;
            return 1;
        }
        else if (precond == 0 && this->OSB == 1)
        {
            this->OSB = 0;
            return 0;
        }
        else
            return 0;
    }

    boolean TON(boolean input, int preset)
    {
        if (input && !timerInSession)
            timerTimer = millis();
        else if (input && timerMemory)
            return 1;
        else if (input && millis() - timerTimer >= preset)
        {
            timerMemory = 1;
            return 1;
        }
        else
            ;
        timerMemory = 0;
        timerInSession = input;
        return 0;
    }
};

class IO
{
public:
    TOGGLE_BUTTON *control;
    uint8_t pin;
    IO() {}
    ~IO() { delete control; }
    virtual boolean update_state();
};

class BRIGHTS : public IO
{
public:
    /**
     * @brief Construct a new BRIGHTS object
     *
     * @param pin output pin
     * @param input TOGGLE_BUTTON constructed with `new`
     */
    BRIGHTS(uint8_t pin, TOGGLE_BUTTON *input)
    {
        this->pin = pin;
        this->control = input;
    }

    boolean update_state()
    {
        digitalWrite(pin, control->update_state());
    }
};

class TURNSIGNAL : public IO
{
public:
    float hz;

    TURNSIGNAL(uint8_t pin, TOGGLE_BUTTON *input, float hz = 1)
    {
        this->pin = pin;
        this->control = input;
        this->hz = hz;
    }

    boolean update_state()
    {
        digitalWrite(pin, control->update_state() && FlasherBit(hz));
    }

private:
    boolean FlasherBit(float hz)
    {
        int T = round(1000.0 / hz);
        if (millis() % T >= T / 2)
            return 1;
        else
            return 0;
    }
};

BRIGHTS brights(BRIGHTS_OUT, new TOGGLE_BUTTON(BRIGHTS_IN));
TURNSIGNAL lt(LT_OUT, new TOGGLE_BUTTON(LT_IN));
TURNSIGNAL rt(RT_OUT, new TOGGLE_BUTTON(RT_IN));

IO *io_list[] = {&brights, &lt, &rt};

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(1);
    // initialize pinModes (not set when constructing above objects)
    for (auto i : io_list)
    {
        pinMode(i->pin, OUTPUT);
        Serial.println("pin " + String(i->pin) + " output");
        pinMode(i->control->pin, INPUT_PULLUP);
        Serial.println("pin " + String(i->control->pin) + " input");
    }

    // underglow control pin is on all the time for now
    pinMode(UNDERGLOW_OUT, OUTPUT);
    digitalWrite(UNDERGLOW_OUT, HIGH);

    Serial.println("setup complete");
}

void loop()
{
    for (auto i : io_list)
    {
        i->update_state();
    }
    delay(1);
}