#ifndef WHIPLASH_H
#define WHIPLASH_H

#include "pid.h"  // Inclui a implementação do PID e movimentação básica

// Estados da máquina de estados
enum WhiplashState {
  WH_AVANCO,      // Estado de avanço em linha reta
  WH_OBSTACULO,   // Estado de desvio de obstáculo
  WH_RETORNO      // Estado de retorno à linha (opcional)
};

class Whiplash {
private:
  WhiplashState estado_atual;
  unsigned long tempo_obstaculo;
  const unsigned long tempo_min_obstaculo = 1000; // Tempo mínimo para considerar obstáculo superado (ms)

public:
  Whiplash() : estado_atual(WH_AVANCO), tempo_obstaculo(0) {}

  void update() {
    leituraSensores();  // Atualiza leitura dos sensores
    
    switch(estado_atual) {
      case WH_AVANCO:
        // Avança em linha reta
        motor.move(1023, 1023);
        
        // Verifica se detectou obstáculo lateral
        if(leitura[0] || leitura[3]) {  // JEsq (0) ou JDir (3)
          estado_atual = WH_OBSTACULO;
          tempo_obstaculo = millis();
        }
        break;
        
      case WH_OBSTACULO:
        // Executa o algoritmo de desvio
        creepingDeath();
        
        // Verifica se passou tempo suficiente sem obstáculos laterais
        if(!leitura[0] && !leitura[3] && (millis() - tempo_obstaculo > tempo_min_obstaculo)) {
          estado_atual = WH_AVANCO;
        }
        break;
        
      case WH_RETORNO:
        // (Opcional) Implementar lógica de retorno à linha se necessário
        break;
    }
  }
  
  // Função para forçar mudança de estado (se necessário)
  void setState(WhiplashState novo_estado) {
    estado_atual = novo_estado;
  }
  
  // Função para obter estado atual (para debug)
  WhiplashState getState() const {
    return estado_atual;
  }
};

#endif