#ifndef sensores_H
#define sensores_H

#define JEsq 35
#define JFEsq 33
#define JFDir 32
#define JDir 34



bool sensores[] = {JEsq, JFEsq, JFDir, JDir};

void setupSensores() {
  pinMode(JEsq, INPUT);
  pinMode(JFEsq, INPUT);
  pinMode(JFDir, INPUT);
  pinMode(JDir, INPUT);
  
}



#endif