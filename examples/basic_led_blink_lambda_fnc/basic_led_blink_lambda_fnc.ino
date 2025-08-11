#include <Arduino.h>
#include <InterCom.h>

SimpleCommand cmd;

uint8_t led = LED_BUILTIN;
bool state = false, en_bl = false;
unsigned long t;
float periodo_ms = 500;

void tougle(){
  state = !state;
  digitalWrite(led, state);
}

void setup(){
  pinMode(led, OUTPUT);

  // Se inicializara automaticamente en cmd.begin();
  //Serial.begin(115200); 

  cmd.enable_echo(true); // Se habilita el echo del comando.

  //Se definen los comandos
  cmd.addCommand("list",[](){ cmd.list(); });
  cmd.addCommand("blink", [](){ en_bl = !en_bl; });
  cmd.addCommand("lambda", [](){
    Serial.println("Hola, soy una funcion lambda.");
    Serial.print("El estado actual del led es: ");
    Serial.println((digitalRead(led)) ? "ON": "OFF");
  });
  cmd.addCommand("p", &periodo_ms);

  // Inicializacion de interprete
  // (Usando Serial por default).
  cmd.begin(115200);
  // Se reccomienda inicializar el interprete
  // despues de haber añadido los comandos.
}

void loop(){
  cmd.listen(); // Verificando por comandos. 
  if (en_bl){
    //funcionalida de blink
    if ((millis() - t) >= (unsigned long)periodo_ms){
      t = millis();
      tougle();
    }
  }
}