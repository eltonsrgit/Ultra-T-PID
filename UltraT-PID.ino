#include "SumoIR.h"
#include "DRV8833.h"
#include "ledFX.h"
#include "pid.h"
#include "Whiplash.h"

SumoIR IR;


int strategy = 0; // estrategia

void setup(){
  Serial.begin(115200);
  IR.begin(15); // sensor conectado no pino 15
  motor.begin();
  motor.bip(5, 250, 2500);

  setupSensores();

  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pixels.begin();
  pixels.setBrightness(60);
  pixels.clear();
  ledLight(0, 0, 0);

}

void loop() {

    IR.update();

    if (IR.prepare()) {
      ledBlink(150, 150, 0, 100);
      motor.stop();
      Serial.println("-> sumo prepare");
    }
    
    else if (IR.start()) {
      Serial.println("-> sumo start");
    
    } 
    
    else if (IR.on()){
      ledLight(0, 150, 0);
      creepingDeath();
      Serial.println("-> sumo on");
      
    }
    
    else if(IR.stop()){
      ledLight(0, 0, 150);
      motor.stop();
      Serial.println("-> sumo stop");

    }
    
    else {
      ledDetection();
      motor.stop();
      Serial.println("-> sumo off");

    }
}
