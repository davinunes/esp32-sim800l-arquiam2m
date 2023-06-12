#include <HardwareSerial.h>

// Configuração dos pinos do SIM800L
#define SIM800_TX_PIN 16
#define SIM800_RX_PIN 17

HardwareSerial sim800l(1);

void setup() {
  Serial.begin(115200);
  sim800l.begin(9600, SERIAL_8N1, SIM800_RX_PIN, SIM800_TX_PIN);

  delay(1000); // Aguarda 1 segundo para a inicialização do SIM800L
  sim800l.println("AT"); // Envia o comando AT para verificar a comunicação

  delay(1000);
  if (sim800l.find("OK")) {
    Serial.println("Módulo SIM800L conectado com sucesso!");
  } else {
    Serial.println("Falha ao conectar ao módulo SIM800L.");
  }

  // Configura a ponte serial entre Serial Monitor e SIM800L
  while (!Serial);
  Serial.println("Serial Bridge iniciada. Digite comandos AT para o módulo SIM800L:");
}

void loop() {
  // Verifica se há comandos disponíveis no Serial Monitor
  if (Serial.available()) {
    char command = Serial.read();

    // Envia o comando para o módulo SIM800L
    sim800l.write(command);

    // Aguarda a resposta do módulo SIM800L e exibe no Serial Monitor
    delay(10);
    while (sim800l.available()) {
      Serial.write(sim800l.read());
    }
  }

  // Verifica se há resposta disponível do módulo SIM800L
  if (sim800l.available()) {
    Serial.write(sim800l.read());
  }
}
