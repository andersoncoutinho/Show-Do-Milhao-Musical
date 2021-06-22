void habilitarControleRemoto(void) {
  results.value = 0;
  if (irrecv.decode(&results))
    irrecv.resume();
}

void controlarVolume(void) {
  if (results.value == AUMENTA_VOLUME)
    mp3_set_volume(++volume);
  if (results.value == DIMINUI_VOLUME)
    mp3_set_volume(--volume);
}

int centralizar(char *texto) {
  int tamanho;
  tamanho = strlen(texto);
  return (20 - tamanho) / 2;
}
void iniciarJogo() {
  results.value = 0;
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
    habilitarControleRemoto();
    if (digitalRead(ENTER) == HIGH or results.value == TECLA_OK) {
      delay(250); break;
    }
  }
}
int aleatorizarMusica(char nivel) {
  int posicao;
  randomSeed(analogRead(A0));
  posicao = nivel + (16 * random(MAXIMO));
  while (posicao == primeira or posicao == segunda or posicao == terceira
         or posicao == quarta or posicao == quinta or posicao == sexta) {
    randomSeed(analogRead(A0));
    posicao = nivel + (16 * random(MAXIMO));
    Serial.println(posicao);
    delay(10);
  }
  switch (contador) {
    case 0: primeira = posicao; contador++; break;
    case 1: segunda = posicao; contador++; break;
    case 2: terceira = posicao; contador++; break;
    case 3: quarta = posicao; contador++; break;
    case 4: quinta = posicao; contador++; break;
    case 5: sexta = posicao; contador = 0; primeira = segunda = terceira = quarta = quinta = sexta = -1;
  }
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
  while (1) {
    habilitarControleRemoto();
    controlarVolume();
    i += 10;
    delay(10);
    if (digitalRead(ENTER) == HIGH or results.value == TECLA_OK or i > Espera) {
      delay(250); break;
    }
  }
}

void imprimirPergunta(char *Pergunta) {
  int i = 0;
  lcd.clear();
  lcd.setCursor(centralizar(Pergunta), 1);
  lcd.print(Pergunta);
  while (1) {
    habilitarControleRemoto();
    controlarVolume();
    i += 10;
    delay(10);
    if (digitalRead(ENTER) == HIGH or results.value == TECLA_OK or i > 3000) {
      delay(250); break;
    }
  }
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

void temporizarPergunta(void) {
  static int segundos;
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
}

char escolherAlternativa(char *a, char *b, char *c, char *d) {
  while (temporizadorPergunta >= 0) {
    habilitarControleRemoto();
    controlarVolume();
    temporizarPergunta();
    if (digitalRead(ALT_A) == HIGH or results.value == TECLA_1) {
      alternativaEscolhida = a;
      return 1;
    } else if (digitalRead(ALT_B) == HIGH or results.value == TECLA_2) {
      alternativaEscolhida = b;
      return 2;
    } else if (digitalRead(ALT_C) == HIGH or results.value == TECLA_3) {
      alternativaEscolhida = c;
      return 3;
    } else if (digitalRead(ALT_D) == HIGH or results.value == TECLA_4) {
      alternativaEscolhida = d;
      return 4;
    } else if (digitalRead(PARAR) == HIGH or results.value == TECLA_0) {
      delay(250);
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
    habilitarControleRemoto();
    if (digitalRead(PARAR) == HIGH || results.value == TECLA_0) {
      delay(250);
      mp3_stop();
      return 0;
    } else if (digitalRead(ENTER) == HIGH || results.value == TECLA_OK) {
      mp3_stop();
      return alternativa;
    }
  }
}

char avaliarResposta(char alternativa, char Resposta) {
  if (alternativa == 6) {
    return 0;
  }
  if (alternativa == 5) {
    vidas--;
    if (vidas == 0) {
      return 0;
    }
  } else if (alternativa == Resposta) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print(F("Certa Resposta"));
    mp3_play(24);
    delay(2000);
    nivel++;
    if (nivel == 16)
      return 0;
    else
      return 1;
  } else {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print(F("Resposta Errada"));
    mp3_play(3);
    delay(2000);
    vidas--;

    if (vidas == 0) {
      return 0;
    }
    return 1;
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
    habilitarControleRemoto();
    if (digitalRead(ENTER) == HIGH or results.value == TECLA_OK)
      break;
  }
}
