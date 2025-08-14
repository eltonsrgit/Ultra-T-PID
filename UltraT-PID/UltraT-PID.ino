#include "SumoIR.h"
#include "DRV8833.h"
#include "PID.h"
#include "Whiplash.h"
#include "Empate.h"
#include "LEDFX.h"
#include "RC.h"

#define boot 0 

int strategy = 0;

SumoIR IR;

unsigned long tempoPressionado = 0;  // armazena o tempo que o botão foi pressionado
bool botaoPressionado = false;       // indica se o botão foi pressionado

void setup() {
  Serial.begin(115200);

  IR.begin(15); // sensor conectado no pino 15 (não mudar)
  PS4.begin("60:5b:b4:7e:74:a4");
  motor.begin();
  motor.bip(5, 250, 2500); // motor bipa (x vezes, intervalo (ms), frequencia em Hz)

  setupSensores();

  pinMode(boot, INPUT_PULLUP);

  ////////////////////////////////// para o LED ////////////////////////////////////
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pixels.begin();
  pixels.setBrightness(150);
  pixels.clear();
  ledLight(0, 0, 0);
  //////////////////////////////////////////////////////////////////////////////////
}

void loop() {
  // Verifica se o botão foi pressionado
  if (digitalRead(boot) == LOW && !botaoPressionado) {
    tempoPressionado = millis();  // Armazena o tempo atual
    botaoPressionado = true;      // Marca o botão como pressionado
  }

  // Verifica se o botão foi pressionado por mais de 1000ms (1 segundo)
  if (botaoPressionado && (millis() - tempoPressionado >= 1000) && modoAutonomo && !RC) {
    modoAutonomo = false;  // Alterna o modo
    Serial.print("Modo alternado para: ");
    Serial.println(modoAutonomo ? "Autônomo." : "RC.");

    // Reseta a variável do botão pressionado
    botaoPressionado = false;
  }
  if (botaoPressionado && (millis() - tempoPressionado >= 1000) && !modoAutonomo && RC) {
    modoAutonomo = true;  // Alterna o modo
    Serial.print("Modo alternado para: ");
    Serial.println(modoAutonomo ? "Autônomo. " : "RC. ");
    // Reseta a variável do botão pressionado
    botaoPressionado = false;
  }

  if (!modoAutonomo) {
    RC = true;
    rc(); // Código RC
  }
  else if (modoAutonomo) {
    // Código autônomo
    RC = false;
    PS4.setLed(255, 0, 0);   // seta a cor do led do controle
    PS4.sendToController();  // necessário para enviar o comando acima para o controle

  
    IR.update();
 
  if (IR.prepare()) { // número 1 no controle
    ledLight(150, 0, 0);
    motor.stop();
    Serial.println("-> sumo prepare");
  }
  
  else if (IR.start()) {
    Serial.println("-> sumo start");
  } 
  
  else if (IR.on()) { // número 2 no controle
    pixels.clear();
    ledLight(0, 150, 0);

    switch (strategy) {
      default: //fallthrough
      case 4:
        iSeeYou();
      break;

      case 5:
        paraTras();
      break;

      case 6:
        SeekAndDestroy_L();
      break;

      case 7:
        SeekAndDestroy_R();
      break; 

      case 8:
        whiplash();
      break;
    }
    Serial.println("-> sumo on");
  }
  
  else if (IR.stop()) { // número 3 no controle
    pixels.clear();
    motor.stop();
    Serial.println("-> sumo stop");
  }
  
  else { // robô inicia caindo aqui
    strategySelection();     // seletor de estratégias
    ledDetection();
    
  }
}
}

void strategySelection() {
  int cmd = IR.read();
  if (cmd >= 4 && cmd <= 9) { // faixa de valores validos ( 1, 2 e 3 são reservados pra prepare, start e stop)
    strategy = cmd;
  } else return;

  if (cmd <= 8) {
    const int num_leds = cmd % 8;
    for(uint8_t i = 0; i < num_leds; i++) {
      switch ((cmd-3) % 5) { // Acende o LED de 0 ao número comando numa cor +- diferente cada número
        case 0: pixels.setPixelColor(i, pixels.Color(150, 0,   0  )); break;
        case 1: pixels.setPixelColor(i, pixels.Color(150, 150, 150)); break;
        case 2: pixels.setPixelColor(i, pixels.Color(0,   0,   150)); break;
        case 3: pixels.setPixelColor(i, pixels.Color(255, 150, 0  )); break;
        case 4: pixels.setPixelColor(i, pixels.Color(0,   150, 255)); break;
      } pixels.show();
    }
    delay(80);
    for(uint8_t i = 0; i < num_leds; i++) { 
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Desliga os LEDs
      pixels.show();
    }
    delay(80);
  }
}
