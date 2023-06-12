#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 22, 21);

// Imprime 4 linhas na tela
#define BUFFER_SIZE 5

String buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;

int contador = 0;

#include <HardwareSerial.h>

// Configuração dos pinos do SIM800L
#define SIM800_TX_PIN 16
#define SIM800_RX_PIN 17

HardwareSerial sim800l(1);

void setup() {
  Serial.begin(115200);

  sim800l.begin(9600, SERIAL_8N1, SIM800_RX_PIN, SIM800_TX_PIN);

  // INICIALIZA O DISPLAY & INVERTE O DISPLAY VERTICALMENTE
  display.init();
  display.flipScreenVertically();
}

void loop() {
  incrementarContador();
  //adicionarValorAoBuffer(String(contador));//
  atualizarTela();

  int sinal = obterSinal();
  bool statusGPRS = obterStatusGPRS();
  String imei = obterIMEI();
  bool simCard = verificarSIMCard();

  adicionarValorAoBuffer("" + imei);
  adicionarValorAoBuffer("Sinal: " + String(sinal));
  adicionarValorAoBuffer("GPRS: " + String(statusGPRS));
  adicionarValorAoBuffer("SIM: " + String(simCard));
  Serial.println("volta");



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
  display.setFont(ArialMT_Plain_10);

  int linha = 0;
  int index = tail;
  while (index != head) {
    display.drawString(0, linha * 14, buffer[index]);
    linha++;
    index = (index + 1) % BUFFER_SIZE;
  }

  display.display();
}

int obterSinal() {
  sim800l.println("AT+CSQ");
  delay(1000);
  while (sim800l.available()) {
    String response = sim800l.readStringUntil('\n');
    if (response.startsWith("+CSQ:")) {
      int commaIndex = response.indexOf(",");
      if (commaIndex != -1) {
        int signalValue = response.substring(commaIndex + 1).toInt();
        return signalValue;
      }
    }
  }
  return 0; // Retornar 0 se não for possível obter o valor do sinal
}

bool obterStatusGPRS() {
  sim800l.println("AT+CGATT?");
  delay(1000);
  while (sim800l.available()) {
    String response = sim800l.readStringUntil('\n');
    if (response.startsWith("+CGATT:")) {
      int commaIndex = response.indexOf(":");
      if (commaIndex != -1) {
        int status = response.substring(commaIndex + 1).toInt();
        return (status == 1);
      }
    }
  }
  return false; // Retornar false se não for possível obter o status GPRS
}

String obterIMEI() {
  sim800l.println("AT+GSN");
  delay(1000);
  while (sim800l.available()) {
    String response = sim800l.readStringUntil('\n');
    response.trim(); // Remover espaços em branco no início e no final da resposta
    if (response.length() == 15) {
      return response;
    }
  }
  return "Desconhecido"; // Retornar "Desconhecido" se não for possível obter o IMEI
}

bool verificarSIMCard() {
  sim800l.println("AT+CPIN?");
  delay(1000);
  while (sim800l.available()) {
    String response = sim800l.readStringUntil('\n');
    response.trim(); // Remover espaços em branco no início e no final da resposta
    if (response.startsWith("+CPIN:")) {
      int colonIndex = response.indexOf(":");
      if (colonIndex != -1) {
        String status = response.substring(colonIndex + 1);
        status.trim();
        return (status == "READY");
      }
    }
  }
  return false; // Retornar false se não for possível obter o status do SIM card
}