#include <HardwareSerial.h>

#define TINY_GSM_MODEM_SIM800 // definição do modem usado (SIM800L)
#include <TinyGsmClient.h> // biblioteca com comandos GSM

// Configuração dos pinos do SIM800L
#define SIM800_TX_PIN 16
#define SIM800_RX_PIN 17

HardwareSerial SerialGSM(1);

// objeto da bibliteca com as funções GSM
TinyGsm modemGSM(SerialGSM);

// TinyGSM Client for Internet connection
TinyGsmClient client(modemGSM);

// velocidade da serial tanto do SIM800L quanto do monitor serial
const int BAUD_RATE = 9600;

//Access point name da vivo
const char *APN = "zap.vivo.com.br";
//Usuario, se não existir deixe em vazio
const char *USER = "";
//Password, se não existir deixe em vazio
const char *PASSWORD = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Dispositivo iniciado...");

  // inicia e configura o SIM800L
  setupGSM();
}

void loop() {
  delay(1000);
  Serial.println("LOOP!");
  Serial.println(modemGSM.getSimStatus());
}

// inicializa GSM
void setupGSM()
{
  
   
  // inicia serial SIM800L
  SerialGSM.begin(9600, SERIAL_8N1, SIM800_RX_PIN, SIM800_TX_PIN);
  delay(1000);

  // exibe info do modem no monitor serial
  Serial.println(modemGSM.getModemInfo());

  // aguarda network
  if (!modemGSM.waitForNetwork()) 
  {
    
    Serial.println("Falha ao conectar a rede");
    delay(1000);
    return;
  }
  Serial.println("Rede OK");

  // conecta na rede (tecnologia GPRS)
  if(!modemGSM.gprsConnect(APN,USER,PASSWORD))
  {
    
    Serial.println("Falha ao conectar ao GPRS");
    delay(10000);
    return;
  }
  
  Serial.println("GPRS Connect OK");

  //Define modo SMS para texto (0 = PDU mode, 1 = Text mode)
  if(sendAT("AT+CMGF=1").indexOf("OK") < 0)
  {
    
    Serial.println("SMS Txt mode Error");
    delay(10000);
    return;
  }  
  Serial.println("SMS Txt mode OK");
}

//Envia comando AT e aguarda até que uma resposta seja obtida
String sendAT(String command)
{
  String response = "";
  SerialGSM.println(command);
  // aguardamos até que haja resposta do SIM800L
  while(!SerialGSM.available());

  response = SerialGSM.readString();

  return response;
}