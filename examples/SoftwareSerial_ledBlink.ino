#include <Arduino.h>
#include <InterCom.h>
#include <SoftwareSerial.h>
//La libreria solo funciona para los AVR y la placa bluepill (stm32f103)

SimpleCommand cmd;

const uint8_t s_rx = 11, s_tx = 12;  
//tomar encuenta que pines se puede unsar para Rx
//segun la documentacion de la libreira de SoftwareSerial.h

SoftwareSerial ss_port(s_tx,s_rx);

const uint8_t led = LED_BUILTIN;
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

    ss_port.begin(9600);

    cmd.begin(&ss_port);
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