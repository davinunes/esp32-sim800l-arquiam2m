#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 22, 21);

// Imprime 4 linhas na tela
#define BUFFER_SIZE 5

String buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;

int contador = 0;

void setup() {
  Serial.begin(115200);

  // INICIALIZA O DISPLAY & INVERTE O DISPLAY VERTICALMENTE
  display.init();
  display.flipScreenVertically();
}

void loop() {
  incrementarContador();
  adicionarValorAoBuffer(String(contador));
  atualizarTela();

  delay(1000);
}

void incrementarContador() {
  contador++;
  if (contador > 9999) {
    contador = 0;
  }
}

void adicionarValorAoBuffer(String valor) {
  buffer[head] = valor;
  head = (head + 1) % BUFFER_SIZE;
  if (head == tail) {
    tail = (tail + 1) % BUFFER_SIZE;
  }
}

void atualizarTela() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);

  int linha = 0;
  int index = tail;
  while (index != head) {
    display.drawString(0, linha * 14, buffer[index]);
    linha++;
    index = (index + 1) % BUFFER_SIZE;
  }

  display.display();
}