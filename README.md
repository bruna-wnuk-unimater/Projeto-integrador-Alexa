# Projeto-integrador-Alexa

## Resumo 
- O ESP8266 conecta-se ao Wi-Fi e, em seguida, ao SinricPro.
- Quando um comando para ligar/desligar o dispositivo é recebido via Alexa, onde a função onPowerState é chamada.
- O estado do LED interno é atualizado.
- O status é exibido no monitor serial.
- A função loop mantém a integração ativa.

## Detalhamento do código
``` cpp 
#include <Arduino.h>
#include <ESP8266WiFi.h>  // Para ESP8266
#include <SinricPro.h>
#include <SinricProSwitch.h>
```
**Arduino.h:** Fornece funcionalidades básicas do Arduino, como controle de pinos e comunicação serial.

**ESP8266WiFi.h:** Biblioteca para conectar o ESP8266 a uma rede Wi-Fi.

**SinricPro.h e SinricProSwitch.h:** Bibliotecas para integração com a plataforma SinricPro, permitindo o controle de dispositivos IoT via Alexa ou Google Home.

``` cpp 
const char* ssid = "usuario";
const char* password = "senha";
```
**ssid:** Nome da rede Wi-Fi à qual o ESP8266 deve se conectar.

**password:** Senha da rede Wi-Fi.

``` cpp 
const char* appKey = "9a8a0cc5-80da-413e-96af-cb628ffd4bd4";
const char* appSecret = "6fb96bf9-5962-4429-9eab-dcf8a8606a89-a6dd1695-cd9c-420a-a248-8799130dc4b1";
const char* deviceID = "6733d61d88a12c713ba8e983";
```
**appKey:** Chave de autenticação do aplicativo no SinricPro.

**appSecret:** Chave secreta para validação.

**deviceID:** Identificador único do dispositivo configurado na conta do SinricPro.

``` cpp 
const int output5 = LED_BUILTIN;
```
output5: Define o LED interno do ESP8266 como o dispositivo controlado. O pino LED_BUILTIN é usado para representar o LED embutido no módulo.

``` cpp 

bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == deviceID) {
    digitalWrite(output5, state ? LOW : HIGH);
    Serial.printf("Comando recebido: %s\n", state ? "Ligar" : "Desligar");
    Serial.printf("LED interno está agora %s.\n", state ? "Ligado" : "Desligado");
  }
  return true;
}
```
**Entrada:**

***deviceId:*** Verifica se o comando recebido corresponde ao dispositivo configurado.

***state:*** Estado desejado (true para ligar e false para desligar).

**Função:**

Controla o estado do LED interno (ativo em nível baixo).
Envia mensagens de status para o monitor serial.

**Saída:** Retorna true para confirmar que o comando foi executado

``` cpp 
void setup() {
  Serial.begin(115200);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, HIGH);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  SinricProSwitch &mySwitch = SinricPro[deviceID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(appKey, appSecret);

  Serial.println("SinricPro configurado e pronto para receber comandos.");
}
```
**Configuração Serial:** Inicializa a comunicação serial com a taxa de 115200 baud.

**Configuração do LED:**
Define o pino do LED interno como saída. Garante que o LED começa desligado.

**Conexão Wi-Fi:**
Tenta conectar ao Wi-Fi usando o SSID e senha fornecidos.
Exibe no monitor serial o progresso da conexão.

**Configuração do SinricPro:**
Associa o dispositivo configurado (deviceID) à função de callback onPowerState. E inicializa a conexão com o SinricPro.

``` cpp 
void loop() {
  SinricPro.handle();
}
```
- Monitora e processa eventos recebidos da plataforma SinricPro.

- Mantém o dispositivo em comunicação contínua com os servidores.

# Arquitetura

Este projeto demonstra uma arquitetura de automação utilizando o ESP8266, conectividade Wi-Fi e a plataforma **SinricPro** para integração com assistentes virtuais, como a Alexa.

## Visão Geral

A arquitetura do sistema é composta pelos seguintes componentes principais:

### 1. **Hardware**
- **ESP8266:** Microcontrolador responsável pela conectividade Wi-Fi e pelo processamento dos comandos.
- **LED interno (LED_BUILTIN):** Representa o dispositivo controlado no exemplo.

### 2. **Conexão Wi-Fi**
- O ESP8266 conecta-se a uma rede Wi-Fi utilizando as credenciais fornecidas no código.
- O dispositivo aguarda estabelecer a conexão antes de iniciar as funções de controle remoto.

### 3. **Plataforma SinricPro**
- A plataforma **SinricPro** facilita a comunicação entre dispositivos IoT e assistentes virtuais.
- O dispositivo é identificado por:
  - **Device ID**: Identificador único.
  - **App Key** e **App Secret**: Credenciais para autenticação com a nuvem.

### 4. **Callback de Controle**
- A função `onPowerState` é chamada sempre que um comando é recebido da plataforma.
- No exemplo, o estado do LED interno é alterado (ligado/desligado) conforme o comando recebido.

### 5. **Ciclo de Execução**
- A função `SinricPro.handle()` é chamada continuamente no loop principal, processando eventos e mantendo a conexão ativa com a plataforma.

## Esquema da Arquitetura

Abaixo está o GIF do sistema em funcionamento:

![Gif do Funcionamento](funcionamento.gif)

## Funcionamento do Sistema

1. O ESP8266 conecta-se à rede Wi-Fi.
2. Após conectado, registra-se na plataforma SinricPro.
3. Comandos de controle são enviados pelo assistente virtual (como Alexa) para a nuvem.
4. A plataforma SinricPro retransmite os comandos para o ESP8266, que altera o estado do dispositivo (LED interno no exemplo).