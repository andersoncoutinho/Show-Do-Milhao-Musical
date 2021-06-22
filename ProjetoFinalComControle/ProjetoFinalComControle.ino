#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define TECLA_0 0xFF9867
#define TECLA_1 0xFFA25D
#define TECLA_2 0xFF629D
#define TECLA_3 0xFFE21D
#define TECLA_4 0xFF22DD
#define TECLA_OK 0xFF38C7
#define AUMENTA_VOLUME 0xFF18E7
#define DIMINUI_VOLUME 0xFF4AB5
#define ALT_A 7
#define ALT_B 6
#define ALT_C 5
#define ALT_D 4
#define PARAR 3
#define ENTER 2
#define MAXIMO 6
#define numFrases ((16)*(MAXIMO))

#include "globais.h"
#include "perguntasDoJogo.h"
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial mySerial(9, 10); //rx, tx


void setup() {
  Serial.begin (9600);
  mySerial.begin(9600);
  mp3_set_serial (mySerial);
  delay(1);
  mp3_set_volume(volume);
  lcd.begin(20, 4);
  irrecv.enableIRIn(); // Start the receiver  
  pinMode(ALT_A, INPUT);
  pinMode(ALT_B, INPUT);
  pinMode(ALT_C, INPUT);
  pinMode(ALT_D, INPUT);
  pinMode(PARAR, INPUT);
  pinMode(ENTER, INPUT);
}

void loop() {
  char Respostas, Premios[10], Pergunta[19], A[18], B[18], C[18], D[18];
  int Perguntas, Espera, Musicas, posicao;
  
  nivel = 0;
  vidas = 5;
  acertando = 1;
  iniciarJogo();
  while (acertando) {
    char alternativa = 0;
    temporizadorPergunta = 30;

    posicao = aleatorizarMusica(nivel);

    memcpy_P(&Perguntas, &perguntas[nivel], sizeof Perguntas);
    memcpy_P(&Espera, &espera[nivel], sizeof Espera);
    memcpy_P(&Premios, &premios[nivel], sizeof Premios);
    memcpy_P(&Respostas, &respostas[posicao], sizeof Respostas);
    memcpy_P(&Musicas, &musicas[posicao], sizeof Musicas);
    memcpy_P(&Pergunta, &pergunta[posicao], sizeof Pergunta);
    memcpy_P(&A, &a[posicao], sizeof A);
    memcpy_P(&B, &b[posicao], sizeof B);
    memcpy_P(&C, &c[posicao], sizeof C);
    memcpy_P(&D, &d[posicao], sizeof D);

    if (nivel == 15)
      vidas = 1;

    imprimirNivel(Premios);
    reproduzirPremio(Perguntas, Espera);
    mp3_play(Musicas);
    imprimirPergunta(Pergunta);

    while (!alternativa) {
      char resposta;
      imprimirAlternativas(A, B, C, D);
      resposta = escolherAlternativa(A, B, C, D);
      alternativa = confirmarAlternativa(resposta);
    }

    acertando = avaliarResposta(alternativa, Respostas);

    if (!acertando) {
      if (alternativa == 6)
        memcpy_P(&Premios, &premios[nivel - 1], sizeof Premios);
      else if (nivel == 1 || nivel == 15)
        nivel = 0;
      else
        memcpy_P(&Premios, &premios[nivel - 2], sizeof Premios);

      imprimirPremio(Premios, alternativa);
    }
    
  }
  
  reiniciar();
}
