#ifndef pid_H
#define pid_H


#include "sensores.h"
#include "DRV8833.h"
#include "peacefield.h"
DRV8833 motor(19 , 18, 4, 23);


// Leitura dos sensores
int leitura[4]; // [0]=esq, [1]=frente-esq, [2]=frente-dir, [3]=dir

// Velocidades e parâmetros PID
int vel_base = 550;
float erro_linha = 0, erro_linha_anterior = 0;
float P = 0, I = 0, D = 0, PID = 0;
float Kp = 450.0, Ki = 0.0, Kd = 0.0;

// Tempo
unsigned long last_time = 0;



void leituraSensores() {
  leitura[0] = digitalRead(JEsq);
  leitura[1] = digitalRead(JFEsq);
  leitura[2] = digitalRead(JFDir);
  leitura[3] = digitalRead(JDir);
}

void calculoErroSensor() {
  leituraSensores();

  // Peso para cada sensor: esquerda negativo, direita positivo
  float peso[] = {-3, -2, 2, 3}; 
  float soma_pesos = 0;
  int ativos = 0;

  for (int i = 0; i < 4; i++) {
    if (leitura[i]) {
      soma_pesos += peso[i];
      ativos++;
    }
  }

  if (ativos > 0) {
    erro_linha = soma_pesos / ativos;
  } else {
    // Nenhum obstáculo: mantém direção anterior
    erro_linha = erro_linha > 0 ? 5 : -5;
  }
}

void pid() {
  unsigned long current_time = millis();
  float dt = (current_time - last_time) / 1000.0;

  calculoErroSensor();

  P = erro_linha;
  I += erro_linha * dt;
  D = (erro_linha - erro_linha_anterior) / dt;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  erro_linha_anterior = erro_linha;
  last_time = current_time;
}

bool peaceField(){
  bool linha = digitalRead(LineL);
  return linha;
}


void creepingDeath() {
  leituraSensores();  // Lê todos os sensores uma vez
  pid();              // Calcula PID sempre

  int velocidade_esq =  - PID;
  int velocidade_dir =  + PID;

  velocidade_esq = constrain(velocidade_esq, -1023, 1023);
  velocidade_dir = constrain(velocidade_dir, -1023, 1023);

  // Se os dois sensores frontais detectarem, ignora PID no comando
  if (PID == 0) {
    motor.stop();
  } else {
    motor.move(velocidade_esq, velocidade_dir);
  }
}

#endif