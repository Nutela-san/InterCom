#include <Arduino.h>
/*
#include <InterCom.h>

SimpleComand terminal;

double dato;

void test(void){
  dato = 1000;
}

void setup() {
  terminal.begin();
  terminal.addComand(0,"LE",&dato);
  terminal.addComand(1,"pos",&dato);
  terminal.addComand(2,"info",&test);
  terminal.addComand(3,"elquesea",&test);
}

void loop() {
  //terminal.lisent();
} 
*/

#include <InterCom.h>

const uint8_t led = 2;
bool state = false, blink_en = false, e= true;

unsigned long last_t;

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
  

  last_t = millis();
}

void loop() {
  terminal.lisent();
  
  if(blink_en && (millis()-last_t)>= 500){
    last_t = millis();
    tougle();
  }
} 