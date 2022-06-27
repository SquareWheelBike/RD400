/**
 * @file arduino.ino
 * @author cole fuerth
 * @brief debounced input pullups for inputs, and toggleable outputs
 * @version 0.2
 * @date 2022-04-14
 *
 * @copyright Copyright (c) 2022
 *
 */

// toggle debug mode (define before including rd400.h)
#define __DEBUG
#include "rd400.h"

#define BRIGHTS_IN A1
#define BRIGHTS_OUT 6
#define LT_IN A2
#define LT_OUT 5
#define RT_IN A3
#define RT_OUT 4
#define UNDERGLOW_OUT 8

#define SDA 2
#define SCL 3
#define POWER_OUT 7 // main power

BRIGHTS brights(BRIGHTS_OUT, new TOGGLE_BUTTON(BRIGHTS_IN));
TURNSIGNAL lt(LT_OUT, new TOGGLE_BUTTON(LT_IN));
TURNSIGNAL rt(RT_OUT, new TOGGLE_BUTTON(RT_IN));

IO *io_list[] = {&brights, &lt, &rt};

void setup()
{
#ifdef __DEBUG
    Serial.begin(115200);
    while (!Serial)
        delay(1);
#endif
    // initialize pinModes (not set when constructing above objects)
    for (auto i : io_list)
    {
        pinMode(i->pin, OUTPUT);
        DEBUGLN("pin " + String(i->pin) + " output");
        pinMode(i->control->pin, INPUT_PULLUP);
        DEBUGLN("pin " + String(i->control->pin) + " input");
    }

    // underglow control pin is on all the time for now
    pinMode(UNDERGLOW_OUT, OUTPUT);
    digitalWrite(UNDERGLOW_OUT, HIGH);

    // initialize spares
    pinMode(POWER_OUT, OUTPUT);
    digitalWrite(POWER_OUT, LOW);

    DEBUGLN("setup complete");
}

void loop()
{
    for (auto i : io_list)
    {
        i->update_state();
    }
    delay(1);
}
