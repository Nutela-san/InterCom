#include <Arduino.h>
#include <InterCom.h>

SimpleComand cmd;

uint8_t led = 2;
bool state = false, en_bl = false;
unsigned long t;
float periodo_ms = 500;

void tougle()
{
    state = !state;
    digitalWrite(led, state);
}

void blink()
{
    if ((millis() - t) >= (unsigned long)periodo_ms)
    {
        t = millis();
        tougle();
    }
}

void blink_en()
{
    en_bl = !en_bl;
}

void list()
{
    cmd.list();
}

void setup()
{
    pinMode(led, OUTPUT);

    cmd.begin(115200);
    cmd.enable_echo(true);
    cmd.addComand("blink", blink_en);
    cmd.addComand("list", list);
    cmd.addComand("p", &periodo_ms);
}

void loop()
{
    cmd.lisent();
    if (en_bl)
        blink();
}