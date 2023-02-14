#include <InterCom.h>

const uint8_t led = LED_BUILTIN;
bool state = false;

SimpleComand terminal;

void tougle(void){
  state = !state;
  digitalWrite(led,state);
}

void setup() {
  pinMode(led,OUTPUT);

  terminal.begin();
  terminal.addComand(0,"led",tougle);
}

void loop() {
  terminal.lisent();
} 