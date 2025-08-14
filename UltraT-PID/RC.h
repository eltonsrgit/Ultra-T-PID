#ifndef RC_H
#define RC_H

#include <PS4Controller.h>

bool modoAutonomo = true;
bool RC = false;

void rc() {
  if (!PS4.isConnected()) {
    motor.stop();
    Serial.println("Aguardando conexão... Pressione PS...");
    ledBlink(150, 150, 150, 30);
    PS4.setLed(255, 0, 0);
    PS4.sendToController();
    pixels.clear();
    
  } else if (PS4.isConnected()) {
    modoAutonomo = false;
    Serial.println("Controle Conectado. ");
    pixels.clear();
    PS4.setLed(0, 255, 0);
    PS4.sendToController();
    ledLight(0, 150, 0); // Efeito visual indicando que está no modo de controle remoto

    
    int RS = map(PS4.RStickX(), -127, 127, -950, 950);

    int R2 = map(PS4.R2Value(), 0, 255, 0, 1023);
    int L2 = map(PS4.L2Value(), 0, 255, 0, 1023);

    int move = constrain((R2 + L2), -1023, 1023);

    int vel_esq = constrain(-RS + (R2 - L2), -1023, 1023);
    int vel_dir = constrain(RS + (R2 - L2), -1023, 1023);


    motor.move(vel_esq, vel_dir);
    
    Serial.print("L: ");
    Serial.print(vel_esq);
    Serial.print(" R: ");
    Serial.println(vel_dir);
    
  }
}
#endif