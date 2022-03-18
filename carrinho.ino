#include <SoftwareSerial.h>
#include "notas.h"

// ====================================================================
// MODULO BLUETOOTH
const int RX_in = 12;
const int TX_in = 11;
SoftwareSerial hc(RX_in, TX_in);

// ====================================================================
// BUZZER PASSIVO
const int buzzer = 3;

// REALIZA A SEQUENCIA DA BUZINA
void buzinar() {
  tone(buzzer, NOTE_B3 , 300);
  delay(350);
  tone(buzzer, NOTE_B3 , 600);
  delay(350);
  tone(buzzer, NOTE_B3 , 500);
}


// ====================================================================
// MOTORES
class DCMotor {
    int spd = 255, pin1, pin2;
  public:

    void Pinout(int in1, int in2) { // Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
    }
    void Velocidade(int in1) { // Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
      spd = in1;
    }
    void Re() { // Forward é o método para fazer o motor girar para frente
      analogWrite(pin1, spd);
      digitalWrite(pin2, LOW);
    }
    void Avancar() { // Backward é o método para fazer o motor girar para trás
      digitalWrite(pin1, LOW);
      analogWrite(pin2, spd);
    }
    void Freio() { // Stop é o metodo para fazer o motor ficar parado.
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
    }
};

DCMotor MotorEsquerdo, MotorDireito;

// ====================================================================
// FAROL
const int farolDianteiro = 7;
const int farolTraseiro = 8;
boolean isAlertOn = false, isFarolDianteiro = false, isFarolTraseiro = false;

void setup() {
  Serial.begin(9600);
  hc.begin(9600);

  // CONFIGURAÇÕES DOS MOTORES
  MotorEsquerdo.Pinout(5, 6);
  MotorDireito.Pinout(9, 10);

  // CONFIGURAÇÕES DO BUZZER
  pinMode(buzzer, OUTPUT);

  // CONFIGURAÇÕES DOS FAROIS
  pinMode(farolDianteiro, OUTPUT);
  pinMode(farolTraseiro, OUTPUT);
}

void loop() {
  if (hc.available()) {
    // RECEBE A ENTRADA
    int dadoEntrada = hc.read();

    // FAZ O MOTOR IR PRA FRENTE
    if (dadoEntrada == 'F') {
      MotorEsquerdo.Velocidade(255);
      MotorDireito.Velocidade(255);

      MotorEsquerdo.Avancar();
      MotorDireito.Avancar();
    }

    // FAZ O MOTOR IR PRA TRÁS
    else if (dadoEntrada == 'B') {
      MotorEsquerdo.Velocidade(255);
      MotorDireito.Velocidade(255);

      MotorEsquerdo.Re();
      MotorDireito.Re();
    }

    // FAZ OS MOTORES IREM PRA ESQUERDA E DIREITA EM DIREÇÃO PRA FRENTE
    else if (dadoEntrada == 'G' ) {
      MotorEsquerdo.Velocidade(150);
      MotorDireito.Velocidade(255);

      MotorEsquerdo.Avancar();
      MotorDireito.Avancar();
    }
    else if (dadoEntrada == 'I' ) {
      MotorEsquerdo.Velocidade(255);
      MotorDireito.Velocidade(150);

      MotorEsquerdo.Avancar();
      MotorDireito.Avancar();
    }

    // FAZ OS MOTORES IREM PRA ESQUERDA E DIREITA EM DIREÇÃO PRA TRÁS
    else if (dadoEntrada == 'H' ) {
      MotorEsquerdo.Velocidade(150);
      MotorDireito.Velocidade(255);

      MotorEsquerdo.Re();
      MotorDireito.Re();
    }
    else if (dadoEntrada == 'J' ) {
      MotorEsquerdo.Velocidade(255);
      MotorDireito.Velocidade(150);

      MotorEsquerdo.Re();
      MotorDireito.Re();
    }

    // ACENDE O LED TRASEIRO
    else if (dadoEntrada == 'U' ) {
      digitalWrite(farolTraseiro, HIGH);
      isFarolTraseiro = true;
    }
    // DESLIGA O LED TRASEIRO
    else if (dadoEntrada == 'u' ) {
      digitalWrite(farolTraseiro, LOW);
      isFarolTraseiro = true;
    }
    // ACENDE O LED TRASEIRO
    else if (dadoEntrada == 'W' ) {
      isFarolDianteiro = true;
      digitalWrite(farolDianteiro, HIGH);
    }
    // DESLIGA O LED TRASEIRO
    else if (dadoEntrada == 'w' ) {
      isFarolDianteiro = true;
      digitalWrite(farolDianteiro, LOW);
    }
    // PISCA ALERTA
    else if (dadoEntrada == 'X' ) {
      isAlertOn = true;
    }
    else if (dadoEntrada == 'x' ) {
      isAlertOn = false;

      // DEPOIS DE DESLIGAR O PISCA ALERTA, VERIFICA DE DEVE DESLIGAR OS FAROIS OU MANTER LIGADO
      if (isFarolDianteiro == true) {
        digitalWrite(farolDianteiro, HIGH);
      }
      if (isFarolTraseiro == true) {
        digitalWrite(farolTraseiro, HIGH);
      }
    }

    // BUZINA
    else if (dadoEntrada == 'V' ) {
      buzinar();
    }
    else if (dadoEntrada == 'v' ) {
      buzinar();
    }
    
    // PARA O CARRO
    else {
      MotorEsquerdo.Freio();
      MotorDireito.Freio();
    }
  }


  // CONFIGURAÇÕES DO PISCA ALERTA E FAROL
  if (isAlertOn == true) {
    digitalWrite(farolTraseiro, HIGH);
    digitalWrite(farolDianteiro, HIGH);
    delay(500);
    digitalWrite(farolTraseiro, LOW);
    digitalWrite(farolDianteiro, LOW);
    delay(500);
  }



}
