#ifndef sensores_H
#define sensores_H
/* ultra t
#define JEsq 35
#define JFEsq 33
#define JFDir 32
#define JDir 34*/
// caçadora
#define JEsq 33
#define JFEsq 27
#define JFDir 26
#define JDir 32


bool sensores[] = {JEsq, JFEsq, JFDir, JDir};

void setupSensores() {
  pinMode(JEsq, INPUT);
  pinMode(JFEsq, INPUT);
  pinMode(JFDir, INPUT);
  pinMode(JDir, INPUT);
  
}



#endif