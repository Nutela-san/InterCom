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

void blink(){
  if ((millis() - t) >= (unsigned long)periodo_ms){
    t = millis();
    tougle();
  }
}

void blink_en(){
  en_bl = !en_bl;
}

void list(){
  cmd.list();
}

void setup(){
  pinMode(led, OUTPUT);

  // Inicialización del objeto Serial
  // a velocidad de 115200 bauld.
  Serial.begin(115200);

  cmd.enable_echo(true); // Se habilita el echo del comando.

  //Se definen los comandos
  cmd.addCommand("blink", blink_en);
  cmd.addCommand("list", list);
  cmd.addCommand("p", &periodo_ms);

  // Las funciones de blink_en y list se pueden
  // definir como funciones lambda, de la forma:
  //
  //cmd.addCommand("blink", [](){
  //  en_bl = !en_bl;
  //});
  //cmd.addCommand("list", [](){ cmd.list(); });
  //
  // Esto es especialmente util si la funcion
  // solo se usará con el interprete de comandos.
  
  cmd.begin(&Serial); //Inicializacion de interprete.
  // Se reccomienda inicializar el interprete
  // despues de haber añadido los comandos.
  // Si se usa el objeto serial "Serial", puede
  // condigurarse de forma directa con:
  //
  //cmd.begin(115200);
  //
  // Esto inicializa el objeto Serial a la velocidad
  // dada (en este caso 115200) he inicializa el
  // interprete de comandos.
}

void loop(){
  cmd.listen(); // Verificando por comandos. 
  if (en_bl) blink();
}