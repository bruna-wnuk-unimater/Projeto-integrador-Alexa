#include <Arduino.h>
#include <ESP8266WiFi.h>  // Para ESP8266
#include <SinricPro.h>
#include <SinricProSwitch.h>

// Configurações Wi-Fi
const char* ssid = "usuario";
const char* password = "senha";

// Configurações SinricPro
const char* appKey = "9a8a0cc5-80da-413e-96af-cb628ffd4bd4";
const char* appSecret = "6fb96bf9-5962-4429-9eab-dcf8a8606a89-a6dd1695-cd9c-420a-a248-8799130dc4b1";
const char* deviceID = "6733d61d88a12c713ba8e983";  // ID do dispositivo "lampadinha"

const int output5 = LED_BUILTIN;    // Define o LED interno como o pino de saída

// Função callback chamada pela Alexa para controlar o estado do dispositivo
bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == deviceID) { // Verifica se é o dispositivo configurado
    digitalWrite(output5, state ? LOW : HIGH); // Controla o LED interno (ativo em nível baixo)
    Serial.printf("Comando recebido: %s\n", state ? "Ligar" : "Desligar");
    Serial.printf("LED interno está agora %s.\n", state ? "Ligado" : "Desligado");
  }
  return true; // Confirma que a ação foi executada
}

void setup() {
  Serial.begin(115200);
  pinMode(output5, OUTPUT);  // Define o LED interno como saída
  digitalWrite(output5, HIGH);  // Inicia com o LED desligado

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  // Configuração do SinricPro
  SinricProSwitch &mySwitch = SinricPro[deviceID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(appKey, appSecret);

  Serial.println("SinricPro configurado e pronto para receber comandos.");
}

void loop() {
  SinricPro.handle(); // Processa eventos do SinricPro
}
