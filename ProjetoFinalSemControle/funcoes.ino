int centralizar(char *texto) {
  int tamanho;
  tamanho = strlen(texto);
  return (20 - tamanho) / 2;
}
void iniciarJogo() {
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(F("QUE SOM EH ESSE?"));
  mp3_play(1);
  delay(5000);
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(F("Pressione ENTER"));
  lcd.setCursor(4, 2);
  lcd.print(F("para comecar"));
  while (1) {
    if (digitalRead(ENTER) == HIGH)
      break;
  }
}
int aleatorizarMusica(char nivel) {
  int posicao;
  randomSeed(analogRead(A0));
  posicao = nivel + (16 * random(MAXIMO));
  return posicao;
}

void imprimirNivel(char *Premios) {
  lcd.clear();
  lcd.setCursor((centralizar(Premios) - 1), 1);
  lcd.print(F("R$ "));
  lcd.print(Premios);
  lcd.setCursor(5, 2);
  lcd.print(F("Vidas X "));
  lcd.print(vidas);
}
void reproduzirPremio(int Perguntas, int Espera) {
  int i = 0;
  mp3_play(Perguntas);
  delay(500);
  while (1) {
    i += 10;
    delay(10);
    if (digitalRead(ENTER) == HIGH or i > Espera) {
      mp3_stop();
      break;
    }
  }
}

void imprimirPergunta(char *Pergunta) {
  lcd.clear();
  lcd.setCursor(centralizar(Pergunta), 1);
  lcd.print(Pergunta);
  delay(3000);
}

void imprimirAlternativas(char *a, char *b, char *c, char *d) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("1) "));
  lcd.print(a);

  lcd.setCursor(0, 1);
  lcd.print(F("2) "));
  lcd.print(b);

  lcd.setCursor(0, 2);
  lcd.print(F("3) "));
  lcd.print(c);

  lcd.setCursor(0, 3);
  lcd.print(F("4) "));
  lcd.print(d);
}

char escolherAlternativa(char *a, char *b, char *c, char *d) {
  int segundos = 0;
  while (temporizadorPergunta >= 0) {
    if (temporizadorPergunta == 9) {
      mp3_play(25);
    }
    if (segundos == 1000) {
      if (temporizadorPergunta > 9) {
        lcd.setCursor(18, 2);
        lcd.print(temporizadorPergunta);
      } else {
        lcd.setCursor(18, 2);
        lcd.print(F(" "));
        lcd.print(temporizadorPergunta);
      }
      temporizadorPergunta--;
      segundos = 0;
    }
    delay(100);
    segundos += 100;
    if (digitalRead(ALT_A) == HIGH) {
      alternativaEscolhida = a;
      mp3_stop();
      delay(200);
      return 1;
    } else if (digitalRead(ALT_B) == HIGH) {
      alternativaEscolhida = b;
      mp3_stop();
      delay(200);
      return 2;
    } else if (digitalRead(ALT_C) == HIGH) {
      alternativaEscolhida = c;
      mp3_stop();
      delay(200);
      return 3;
    } else if (digitalRead(ALT_D) == HIGH) {
      alternativaEscolhida = d;
      mp3_stop();
      delay(200);
      return 4;
    } else if (digitalRead(PARAR) == HIGH) {
      delay(200);
      return 6;
    }
  }
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print(F("Tempo Esgotado"));
  mp3_play(26);
  delay(1000);
  return 5;
}

char confirmarAlternativa(char alternativa) {
  char certo;
  if (alternativa == 5)
    return alternativa;
  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print(F("Certeza?"));
  if (alternativa == 6) {
    lcd.setCursor(7, 2);
    lcd.print(F("PARAR"));
  } else {
    lcd.setCursor(centralizar(alternativaEscolhida), 2);
    lcd.print(alternativaEscolhida);
  }
  certo = random(22, 24);
  mp3_play(certo);
  while (1) {
    if (digitalRead(PARAR) == HIGH) {
      mp3_stop();
      delay(300);
      return 0;
    } else if (digitalRead(ENTER) == HIGH) {
      mp3_stop();
      delay(100);
      return alternativa;
    }
  }
}

bool avaliarResposta(char alternativa, char Resposta) {
  if (alternativa == 6) {
    return false;
  }
  if (alternativa == 5) {
    vidas--;
    if (vidas == 0) {
      return false;
    }
  } else if (alternativa == Resposta) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print(F("Certa Resposta"));
    mp3_play(24);
    delay(2000);
    nivel++;
    if (nivel == 16)
      return false;
    else
      return true;
  } else {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print(F("Resposta Errada"));
    mp3_play(3);
    delay(2000);
    vidas--;

    if (vidas == 0) {
      return false;
    }
    return true;
  }
}

void imprimirPremio(char *Premios, char alternativa) {
  if (nivel == 0) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print(F("Premio: R$ 0,00"));
    delay(2000);
  } else if (nivel < 15 or alternativa == 6) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print(F("Premio: "));
    lcd.print(Premios);
    delay(2000);
  } else if (nivel == 16) {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print(F("Parabens"));
    lcd.setCursor(7, 1);
    lcd.print(F("Premio"));
    lcd.setCursor(3, 2);
    lcd.print(F("R$ 1.000.000"));
    mp3_play(20);
    delay(3500);
  }
}

void reiniciar() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(F("Pressione ENTER"));
  lcd.setCursor(3, 2);
  lcd.print(F("para reiniciar"));
  while (1) {
    if (digitalRead(ENTER) == HIGH)
      break;
  }
}
