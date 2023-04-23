#include <Arduino.h>
#include <InterCom.h>

SimpleCommand cmd;

uint8_t led = LED_BUILTIN;
bool state = false, en_bl = true;
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
    
    Serial.begin(115200);
    cmd.begin(&Serial);
    cmd.enable_echo(true);
    cmd.addCommand("blink", blink_en);
    cmd.addCommand("list", list);
    cmd.addCommand("p", &periodo_ms);
}

void loop()
{
    cmd.listen();
    if (en_bl)
        blink();
}