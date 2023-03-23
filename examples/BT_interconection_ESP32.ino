#include <Arduino.h>
#include <InterCom.h>
#include <BluetoothSerial.h>

SimpleCommand cmd;
BluetoothSerial bt_port;

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

    bt_port.begin("InterCominicador");

    cmd.begin(&bt_port);
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