#ifndef sensores_H
#define sensores_H

#define JEsq 35
#define JFEsq 33
#define JFDir 32
#define JDir 34

#define LineL 25

bool sensores[] = {JEsq, JFEsq, JFDir, JDir};

void setupSensores() {
  pinMode(JEsq, INPUT);
  pinMode(JFEsq, INPUT);
  pinMode(JFDir, INPUT);
  pinMode(JDir, INPUT);
  pinMode(LineL, INPUT);
}
//bool leitura[4] = {0};


// void leituraSensores() {
//   for (int i = 0; i < 4; i++) {
//     leitura[i] = digitalRead(sensores[i]);
//   }
// }


#endif