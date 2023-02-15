#include <Arduino.h>
#include <InterCom.h>

const uint8_t led = 13;
bool state = false, blink_en = false, e= true;

unsigned long last_t;

float dato;

SimpleComand terminal;

void tougle(void){
  state = !state;
  digitalWrite(led,state);
}

void blink_enable(void){
  blink_en = !blink_en;
}

void list(void){
  terminal.list();
}

void echo(){
  e = !e;
  terminal.enable_echo(e);
}

void setup() {
  pinMode(led,OUTPUT);

  terminal.begin(115200);
  terminal.enable_echo(e);
  terminal.addComand("blink",blink_enable);
  terminal.addComand("list",list);
  terminal.addComand("led",tougle);
  terminal.addComand("echo",echo);
  terminal.addComand("pos",&dato);

  last_t = millis();
}

void loop() {
  terminal.lisent();
  
  if(blink_en && (millis()-last_t)>= 500){
    last_t = millis();
    tougle();
  }
} 