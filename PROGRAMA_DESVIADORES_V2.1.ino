#include "PCF8574.h" 
#include <24LC256.h>
#include <SPI.h>
#include  "Nextion.h"   
#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Configurações MQTT
const char* mqtt_server = "192.168.3.200";
const uint32_t mqtt_port = 1883;
const char* mqtt_client_id = "ESP32_DESVIADOR";
const char* mqtt_topic_status = "desviador/status";
const char* mqtt_topic_command = "desviador/command";

// Variáveis de controle WiFi/MQTT
bool wifiConnected = false;
bool mqttConnected = false;
unsigned long lastWifiCheck = 0;
unsigned long lastMqttCheck = 0;
const unsigned long wifiCheckInterval = 30000; // 30 segundos
const unsigned long mqttCheckInterval = 5000;  // 5 segundos

WiFiClient espClient;
PubSubClient client(espClient);

// ========== CONTROLE DUAL-CORE ESP32 ==========
TaskHandle_t TaskControleHandle;  // Task no Core 0 (Controle Crítico)
SemaphoreHandle_t xMutex;          // Mutex para proteção de variáveis compartilhadas

// Variáveis de monitoramento de performance
unsigned long loopCore0Count = 0;
unsigned long loopCore1Count = 0;
unsigned long lastPerfReport = 0;

extern PCF8574 PCF_38(0x38); // enderesso modulos saidas i2c
extern PCF8574 PCF_39(0x39);  //  (used as output)
extern int RL[17] = {0};
 
E24LC256 MyEEPROM(0x57); //define o endereço base da EEPROM 

int PinCorrente = 0;
extern int BAR = 0;
#define WP 25
#define ANALOG_PIN_0 32  //15
#define ANALOG_PIN_1 33 //4
#define RXD2 16  // Pino RX para Serial2 (ajuste conforme necessário)
#define TXD2 17  // Pino TX para Serial2 (ajuste conforme necessário)
#include "RTClib.h"
RTC_DS1307 rtc;
char DiasDaSemana[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"}; // array 7 caracteres de valor 12 caracteres
char semanaN[1] = {0};
extern int Numsemana = 0;
extern int Timex[6] = {0};
int Q0 = 2; //saida pino led int monitora dbserial ihm

float tensaoEntrada = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float tensaoMedida = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR
float valorR1 = 30000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float valorR2 = 7500.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO
float tensaoMedia = 0.0, ResultMedia = 0.0;
float bufferMedia[6] = {0.0};
int bufferVR = 0;
char voltC[4];
//----------------------------------------------------ENDEREÇO NEXTION IHM tela 0 home ---------------------------------------------------//

NexNumber nm0 = NexNumber(0, 7, "nm0"); //nivel de ar do compresor

NexText hor = NexText(0, 1, "hor"); // hora
NexText bat = NexText(0, 34, "bat"); // bateria

NexVariable Num7 = NexVariable(0, 12, "Num7"); // variavel dias da semana
//NexVariable autoON = NexVariable(0, 25, "autoON"); // variavel compressor automatico ligado

NexButton b0 = NexButton(0, 5, "b0");

NexDSButton bt0   = NexDSButton(0, 13, "bt0");
NexDSButton bt1   = NexDSButton(0, 15, "bt1");
NexDSButton bt2   = NexDSButton(0, 17, "bt2");
NexDSButton bt3   = NexDSButton(0, 19, "bt3");
NexDSButton bt4   = NexDSButton(0, 21, "bt4");
NexDSButton bt5   = NexDSButton(0, 23, "bt5");
NexDSButton bt6   = NexDSButton(0, 25, "bt6");
NexDSButton bt7   = NexDSButton(0, 27, "bt7");
NexDSButton bt8   = NexDSButton(0, 29, "bt8");
NexDSButton bt9   = NexDSButton(0, 31, "bt9");
NexDSButton bt10   = NexDSButton(0, 32, "bt10");
NexDSButton bt11   = NexDSButton(0, 36, "bt11");
NexDSButton bt12   = NexDSButton(0, 38, "bt12");

extern uint32_t bt0var = 0,bt1var = 0,bt2var = 0,bt3var = 0,bt4var = 0,bt5var = 0,bt6var = 0,bt7var = 0,
bt8var = 0,bt9var = 0,bt10var = 0,bt11var = 0,bt12var = 0;
extern uint32_t imagem1=1,imagem2=1,imagem3=1,imagem4=1,imagem5=1,imagem6=1,imagem7=1,imagem8=1,imagem9=1,imagem10=1,imagem11=1,imagem12=1,imagem13=1;

NexPicture p0 = NexPicture(0, 8, "p0"); // img 01
NexPicture p1 = NexPicture(0, 14, "p1"); // img 02
NexPicture p2 = NexPicture(0, 16, "p2"); // img 03
NexPicture p3 = NexPicture(0, 18, "p3"); // img 04
NexPicture p4 = NexPicture(0, 20, "p4"); // img 05
NexPicture p5 = NexPicture(0, 22, "p5"); // img 06
NexPicture p6 = NexPicture(0, 24, "p6"); 
NexPicture p7 = NexPicture(0, 26, "p7"); 
NexPicture p8 = NexPicture(0, 28, "p8"); 
NexPicture p9 = NexPicture(0, 30, "p9");
NexPicture p10 = NexPicture(0, 35, "p10"); 
NexPicture p11 = NexPicture(0, 37, "p11");  
NexPicture p12 = NexPicture(0, 41, "p12");  
NexPicture p13 = NexPicture(0, 42, "p13");  

//--------------------------------------------------varieaveis nextion-----------------------------------//
uint32_t ds_var;          //armazena o estado do botão
uint32_t com1_var;        //armazena o estado do botão
uint32_t num_n7; // variavel semana
uint32_t tempoAtt;


//----------------------------------------------------ENDEREÇO NEXTION IHM tela 1 tempos --------------------------------------------------//
NexDSButton atualiza   = NexDSButton(1, 31, "atualiza");
extern uint32_t atualizaVar = 0;
NexButton vt = NexButton(1,30, "vt");

NexNumber n00 = NexNumber(1, 4, "n00");
NexNumber n1 = NexNumber(1, 6, "n1");
NexNumber n2 = NexNumber(1, 8, "n2");
NexNumber n3 = NexNumber(1, 10, "n3");
NexNumber n4 = NexNumber(1, 12, "n4");
NexNumber n5 = NexNumber(1, 14, "n5");
NexNumber n6 = NexNumber(1, 16, "n6");
NexNumber n7 = NexNumber(1, 18, "n7");
NexNumber n8 = NexNumber(1, 20, "n8");
NexNumber n9 = NexNumber(1, 24, "n9");
NexNumber n10 = NexNumber(1, 27, "n10");
NexNumber n11 = NexNumber(1, 32, "n11");

NexVariable va0 = NexVariable(1, 23, "va0"); // variavel de retorno

extern uint32_t varN0 = 10, varN1 = 10, varN2 = 10, varN3 = 10, varN4 = 10, varN5 = 10, varN6 = 10,
 varN7 = 10, varN8 = 10, varN9 = 10, varN10 = 10, varN11 = 11;



//----------------------------------------------------ENDEREÇO NEXTION IHM tela 2 ajustar hora rtc ---------------------------------------------------//

NexButton vt2   = NexButton(2, 3, "b0"); // botao voltar
NexButton botaoHR   = NexButton(2, 13, "botaoHR"); // botao para atualizar

NexText titulo = NexText(7, 2, "titulo"); 
NexNumber dia = NexNumber(7, 7, "dia");
NexNumber mes = NexNumber(7, 8, "mes");
NexNumber ano = NexNumber(7, 9, "ano");
NexNumber hora = NexNumber(7, 10, "hora");
NexNumber minut = NexNumber(7, 11, "minut");
NexNumber segun = NexNumber(7, 12, "segun");


//----------------------------------------------------ENDEREÇO NEXTION IHM tela3 loguin admin ---------------------------------------------------//
NexButton entra   = NexButton(3, 6, "entra");
NexButton vt3   = NexButton(3, 5, "vt3");
//----------------------------------------------------                                            ---------------------------------------------------//
extern uint32_t botaoHRvar = 0;
extern uint32_t diavar = 0;
extern uint32_t mesvar = 0;
extern uint32_t anovar = 0;
extern uint32_t horavar = 0;
extern uint32_t minutvar = 0;
extern uint32_t segunvar = 0;
bool carregaeeprom = 0;

int horaAnterior = 0;
long int cloock = 0, clk1 = 0; // milles
long int Tboucing = 0; //milles
long int Tvarre = 0;   //armazena milles
bool abilita=0; //variavel para abilitar distribuidores
extern bool shiftButton = 0;
extern int telaAtiva;

extern int tempo0=0, tempo1=0, tempo2=0, tempo3=0, tempo4=0, tempo5=0, tempo6=0, tempo7=0, tempo8=0, tempo9=0, tempo10=0,tempo11=0;
extern bool M1=0, M2=0, M3=0, M4=0, M5=0, M6=0, M7=0, M8=0, M9=0, M10=0, M11=0, M12=0;

// Declaração de variáveis de proteção (definidas em Sequencial.ino)
extern bool emResetCoxa;
extern bool emResetPeito;


NexTouch *nex_listen_list[] = 
{
 &atualiza,&b0,&vt, //page 1
&bt0,&bt1,&bt2,&bt3,&bt4,&bt5,&bt6,&bt7,&bt8,&bt9,&bt10,&bt11,&bt12,&hor,//page 0
&vt2, &botaoHR,// page 2
&entra,&vt3, // page 3
    NULL
};

void addTimersToJson(JsonDocument& doc) {
  JsonObject timers = doc.createNestedObject("timers");
  timers["varN0"] = varN0;
  timers["varN1"] = varN1;
  timers["varN2"] = varN2;
  timers["varN3"] = varN3;
  timers["varN4"] = varN4;
  timers["varN5"] = varN5;
  timers["varN6"] = varN6;
  timers["varN7"] = varN7;
  timers["varN8"] = varN8;
  timers["varN9"] = varN9;
  timers["varN10"] = varN10;
  timers["varN11"] = varN11;
}
void addButtonsToJson(JsonDocument& doc) {
  JsonObject buttons = doc.createNestedObject("buttons");
  buttons["bt0var"] = bt0var;
  buttons["bt1var"] = bt1var;
  buttons["bt2var"] = bt2var;
  buttons["bt3var"] = bt3var;
  buttons["bt4var"] = bt4var;
  buttons["bt5var"] = bt5var;
  buttons["bt6var"] = bt6var;
  buttons["bt7var"] = bt7var;
  buttons["bt8var"] = bt8var;
  buttons["bt9var"] = bt9var;
  buttons["bt10var"] = bt10var;
  buttons["bt11var"] = bt11var;
  buttons["bt12var"] = bt12var; 
}

void addRelaysToJson(JsonDocument& doc) {
  JsonObject relays = doc.createNestedObject("relays");
  relays["RL1"] = RL[1];
  relays["RL2"] = RL[2];
  relays["RL3"] = RL[3];
  relays["RL4"] = RL[4];
  relays["RL5"] = RL[5];
  relays["RL6"] = RL[6];
  relays["RL7"] = RL[7];
  relays["RL8"] = RL[8];
  relays["RL9"] = RL[9];
  relays["RL10"] = RL[10];
  relays["RL11"] = RL[11];
  relays["RL12"] = RL[12];
  relays["RL13"] = RL[13];
  relays["RL14"] = RL[14];
  relays["RL15"] = RL[15];
  relays["RL16"] = RL[16];
}

void addPerformanceToJson(JsonDocument& doc) {
  JsonObject performance = doc.createNestedObject("performance");
  performance["core0_cycles"] = loopCore0Count;
  performance["core1_cycles"] = loopCore1Count;
  
  // Cálculo de frequência (ciclos por segundo)
  static unsigned long lastCalc = 0;
  unsigned long elapsed = millis() - lastCalc;
  if (elapsed > 0) {
    performance["core0_hz"] = (loopCore0Count * 1000) / elapsed;
    performance["core1_hz"] = (loopCore1Count * 1000) / elapsed;
  }
}

void addWiFiInfoToJson(JsonDocument& doc) {
  if (wifiConnected) {
    JsonObject wifi = doc.createNestedObject("wifi");
    wifi["rssi"] = WiFi.RSSI();  // Nível de sinal em dBm
    wifi["quality"] = map(constrain(WiFi.RSSI(), -100, -50), -100, -50, 0, 100); // 0-100%
    wifi["ip"] = WiFi.localIP().toString();
    wifi["ssid"] = WiFi.SSID();
  }
}


// Função para publicar status MQTT (se conectado)
void publishStatus() {
  // PROTEÇÃO: Só publicar se MQTT realmente conectado
  if (!mqttConnected || !client.connected()) {
    return; // Sai imediatamente se desconectado
  }
  
  StaticJsonDocument<768> doc;  // Aumentado para comportar relays
  
  // Dados básicos
  doc["bat_voltage"] = tensaoMedia;
  doc["pressure"] = BAR;
  doc["wifi_connected"] = wifiConnected;
  doc["uptime"] = millis() / 1000;
  
  // Adicionar timers e botões
  addTimersToJson(doc);
  addButtonsToJson(doc);
  
  // Adicionar estados dos relés (CRÍTICO para diagnóstico)
  addRelaysToJson(doc);
  
  // Adicionar performance dual-core
  addPerformanceToJson(doc);
  
  // Adicionar informações WiFi
  addWiFiInfoToJson(doc);

  String output;
  serializeJson(doc, output);
  
  // PROTEÇÃO: Verificar se mensagem não está muito grande
  if (output.length() > 1000) {
    dbSerial.println("[WARN] Mensagem MQTT muito grande - não enviando");
    return;
  }
  
  // PROTEÇÃO: Verificar se client ainda está conectado antes de publicar
  if (client.connected()) {
    bool published = client.publish(mqtt_topic_status, output.c_str());
    if (!published) {
      dbSerial.println("[WARN] Falha ao publicar MQTT - buffer cheio?");
    }
  }
    
    // Log opcional no Serial (comente se não quiser)
    // dbSerial.print("[MQTT] Status publicado: ");
    // dbSerial.println(output);
  }



// Função callback MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  dbSerial.print("Mensagem MQTT recebida [");
  dbSerial.print(topic);
  dbSerial.print("]: ");
  dbSerial.println(message);
  
  // Processar comandos MQTT aqui
if (String(topic) == mqtt_topic_command) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    dbSerial.print("Erro ao ler JSON: ");
    dbSerial.println(error.f_str());
    return;
  }

  String cmd = doc["command"] | "";
  
  // === Comando: ajustar data/hora ===
  if (cmd == "set_time") {
    String datetime = doc["datetime"] | "";
    if (datetime.length() < 19) {
      dbSerial.println("Formato de data/hora inválido!");
      hor.setText("Formato de data/hora inválido!"); 
      return;
    }

    // Esperado: "YYYY-MM-DDTHH:MM:SS"
    int ano   = datetime.substring(0, 4).toInt();
    int mes   = datetime.substring(5, 7).toInt();
    int dia   = datetime.substring(8, 10).toInt();
    int hora  = datetime.substring(11, 13).toInt();
    int minut = datetime.substring(14, 16).toInt();
    int segun = datetime.substring(17, 19).toInt();

    rtc.adjust(DateTime(ano, mes, dia, hora, minut, segun));

    dbSerial.println("Hora do RTC ajustada via MQTT:");
      hor.setText("Hora do RTC ajustada via server"); 
    dbSerial.printf("%04d/%02d/%02d %02d:%02d:%02d\n", ano, mes, dia, hora, minut, segun);

    // Envia confirmação de volta ao broker
    StaticJsonDocument<100> resp;
    resp["status"] = "time_updated";
    resp["datetime"] = datetime;
    String jsonOut;
    serializeJson(resp, jsonOut);
    client.publish(mqtt_topic_status, jsonOut.c_str());
    
    // Publicar status completo imediatamente após comando
    delay(50);
    publishStatus();
  }


// === Comando: ajustar valor de timer ===
else if (cmd == "set_timer") {
  int index = doc["index"] | -1;
  int value = doc["value"] | -1;

  if (index >= 0 && index <= 11) {
    switch (index) {
      case 0: varN0 = value; n00.setValue(value); break;
      case 1: varN1 = value; n1.setValue(value); break;
      case 2: varN2 = value; n2.setValue(value); break;
      case 3: varN3 = value; n3.setValue(value); break;
      case 4: varN4 = value; n4.setValue(value); break;
      case 5: varN5 = value; n5.setValue(value); break;
      case 6: varN6 = value; n6.setValue(value); break;
      case 7: varN7 = value; n7.setValue(value); break;
      case 8: varN8 = value; n8.setValue(value); break;
      case 9: varN9 = value; n9.setValue(value); break;
      case 10: varN10 = value; n10.setValue(value); break;
      case 11: varN11 = value; n11.setValue(value); break;
    }
    dbSerial.printf("Timer varN%d atualizado para %d\n", index, value);
  } else {
    dbSerial.println("Índice de timer inválido!");
  }
  StaticJsonDocument<100> resp;
resp["status"] = "timer_updated";
resp["index"] = index;
resp["value"] = value;
String jsonOut;
serializeJson(resp, jsonOut);
client.publish(mqtt_topic_status, jsonOut.c_str());

  // Publicar status completo imediatamente após comando
  delay(50);
  publishStatus();
}

// === Comando: alterar estado de botão ===
else if (cmd == "set_button") {
  int index = doc["index"] | -1;
  int state = doc["state"] | 0;

  if (index >= 0 && index <= 12) {
    switch (index) {
      case 0: bt0var = state; bt0.setValue(state); break;
      case 1: bt1var = state; bt1.setValue(state); break;
      case 2: bt2var = state; bt2.setValue(state); break;
      case 3: bt3var = state; bt3.setValue(state); break;
      case 4: bt4var = state; bt4.setValue(state); break;
      case 5: bt5var = state; bt5.setValue(state); break;
      case 6: bt6var = state; bt6.setValue(state); break;
      case 7: bt7var = state; bt7.setValue(state); break;
      case 8: bt8var = state; bt8.setValue(state); break;
      case 9: bt9var = state; bt9.setValue(state); break;
      case 10: bt10var = state; bt10.setValue(state); break;
      case 11: bt11var = state; bt11.setValue(state); break;
      case 12: bt12var = state; bt12.setValue(state); break;
    }
    dbSerial.printf("Botão bt%d atualizado para %d\n", index, state);
  } else {
    dbSerial.println("Índice de botão inválido!");
  }
  StaticJsonDocument<100> resp;
resp["status"] = "button_updated";
resp["index"] = index;
resp["state"] = state;
String jsonOut;
serializeJson(resp, jsonOut);
client.publish(mqtt_topic_status, jsonOut.c_str());

  // Publicar status completo imediatamente após comando
  delay(50);
  publishStatus();
}


}
}

// Função para conectar ao MQTT (não-bloqueante)
void connectMQTT() {
  if (!wifiConnected) return;
  
  if (!client.connected()) {
    dbSerial.print("Tentando conectar MQTT...");
    if (client.connect(mqtt_client_id)) {
      dbSerial.println(" conectado!");
      client.subscribe(mqtt_topic_command);
      client.publish(mqtt_topic_status, "online");
      mqttConnected = true;
    } else {
      dbSerial.print(" falha mqtt, client state = ");
      dbSerial.println(client.state());
      mqttConnected = false;
    }
  }
}

// Função para inicializar WiFi (100% NÃO-BLOQUEANTE)
void initWiFi() {
  dbSerial.println("[WiFi] Iniciando em background (não-bloqueante)...");
  
  // Configurações WiFi básicas
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  
  // Tenta conectar com credenciais salvas (não-bloqueante)
  WiFi.begin();
  
  dbSerial.println("[WiFi] Tentando conectar em background...");
  dbSerial.println("[WiFi] Sistema de controle continuará funcionando normalmente");
  
  wifiConnected = false; // Será atualizado pelo checkConnections()
}

// Função para iniciar portal de configuração WiFi (apenas se solicitado)
void startWiFiConfig() {
  dbSerial.println("[WiFi] Iniciando portal de configuração...");
  
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);  // 3 minutos
  wm.startConfigPortal("DESVIADOR_ESTEIRAS");
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    client.setBufferSize(1024);
    dbSerial.println("[WiFi] Configurado com sucesso!");
  }
}

// Função para verificar conexões periodicamente (NÃO-BLOQUEANTE)
void checkConnections() {
  static unsigned long wifiReconnectAttempt = 0;
  static int wifiRetryCount = 0;
  unsigned long currentTime = millis();
  
  // ===== VERIFICAÇÃO WiFi a cada 30 segundos =====
  if (currentTime - lastWifiCheck > wifiCheckInterval) {
    lastWifiCheck = currentTime;
    
    wl_status_t status = WiFi.status();
    
    if (status != WL_CONNECTED) {
      if (wifiConnected) {
        // Perdeu conexão
        dbSerial.println("[WiFi] Conexão perdida");
        wifiConnected = false;
        mqttConnected = false;
      }
      
      // Tentativa de reconexão NÃO-BLOQUEANTE
      if (currentTime - wifiReconnectAttempt > 60000) { // A cada 60s
        wifiReconnectAttempt = currentTime;
        wifiRetryCount++;
        
        dbSerial.print("[WiFi] Tentativa de reconexão #");
        dbSerial.println(wifiRetryCount);
        
        WiFi.disconnect();
        WiFi.begin(); // Reconecta com credenciais salvas - NÃO BLOQUEANTE
        
        // Watchdog: Resetar WiFi se muitas falhas
        if (wifiRetryCount > 10) {
          dbSerial.println("[WiFi] Muitas falhas - resetando WiFi");
          WiFi.mode(WIFI_OFF);
          // REMOVIDO delay(1000) - BLOQUEANTE!
          WiFi.mode(WIFI_STA);
          WiFi.setAutoReconnect(true);
          WiFi.begin();
          wifiRetryCount = 0;
        }
      }
      
    } else {
      // WiFi conectado
      if (!wifiConnected) {
        // Reconectou!
        wifiConnected = true;
        wifiRetryCount = 0;
        
        dbSerial.println("[WiFi] Reconectado com sucesso!");
        dbSerial.print("[WiFi] IP: ");
        dbSerial.println(WiFi.localIP());
        dbSerial.print("[WiFi] RSSI: ");
        dbSerial.print(WiFi.RSSI());
        dbSerial.println(" dBm");
        
        // Reconfigurar MQTT
        client.setServer(mqtt_server, mqtt_port);
        client.setCallback(callback);
        client.setBufferSize(1024);
      }
    }
  }
  
  // ===== VERIFICAÇÃO MQTT a cada 5 segundos =====
  if (wifiConnected && (currentTime - lastMqttCheck > mqttCheckInterval)) {
    lastMqttCheck = currentTime;
    
    if (!client.connected()) {
      mqttConnected = false;
      connectMQTT(); // Já é não-bloqueante
    } else {
      mqttConnected = true;
    }
  }
}



// ========== TASK CORE 0 - CONTROLE CRÍTICO (IHM + RELÉS + SEQUENCIAL) ==========
void TaskControle(void *pvParameters) {
  dbSerial.println("[CORE 0] Task Controle iniciada");
  
  unsigned long Tvarre_local = 0;
  unsigned long cloock_local = 0;
  bool carregaeeprom_local = false;
  
  // Loop infinito da Task
  for(;;) {
    loopCore0Count++;
    
    // ===== IHM NEXTION - MÁXIMA PRIORIDADE =====
    nexLoop(nex_listen_list);
    
    // ===== CARREGAMENTO INICIAL EEPROM =====
    if(!carregaeeprom_local) {
      carregaeeprom_local = true;
      LoadingEeprom();
    }
    
    // ===== VARREDURA 100ms - CONTROLE DE SAÍDAS =====
    if (millis() - Tvarre_local > 100 && shiftButton == 0) {
      Tvarre_local = millis();
      
      // Usar mutex para ler variáveis compartilhadas
      if (xSemaphoreTake(xMutex, 10 / portTICK_PERIOD_MS)) {
        Varredura();
        
        if(bt10var == 1) {
          carregaTempo();
        }
        
        WriteI2C(); // CRÍTICO: Escrita nos relés I2C
        
        xSemaphoreGive(xMutex);
      }
    }
    
    // ===== INCREMENTO DE TIMERS 1s =====
    if (millis() - cloock_local > 1000) {
      if (xSemaphoreTake(xMutex, 10 / portTICK_PERIOD_MS)) {
        
        if(bt10var == 1) {  
          // INCREMENTAR APENAS SE NÃO ESTIVER EM RESET
          if(bt0var == 1 && !emResetCoxa){tempo0++;}
          if(bt1var == 1 && !emResetCoxa){tempo1++;}
          if(bt2var == 1 && !emResetCoxa){tempo2++;}
          if(bt3var == 1 && !emResetCoxa){tempo3++;}
          
          if(bt4var == 1){tempo4++;}
          if(bt5var == 1){tempo5++;}
          
          if(bt6var == 1 && !emResetPeito){tempo6++;}
          if(bt7var == 1 && !emResetPeito){tempo7++;}
          if(bt8var == 1 && !emResetPeito){tempo8++;}
          if(bt9var == 1 && !emResetPeito){tempo9++;}
          
          if(bt11var == 1){tempo10++;}
          if(bt12var == 1){tempo11++;}
          
        } else {
          // Sistema parado - garantir estado seguro
          RL[1]=0; RL[2]=0; RL[3]=0; RL[4]=0; RL[5]=0; RL[6]=0; RL[7]=0; RL[8]=0;
          imagem4=0; imagem6=0; imagem10=0; imagem11=0;
        }
        
        xSemaphoreGive(xMutex);
      }
      
      cloock_local = millis();
    }
    
    // Delay mínimo para não travar o watchdog
    vTaskDelay(1 / portTICK_PERIOD_MS); // 1ms
  }
}

// ========== SETUP ==========
void setup() {
   esp_log_level_set("i2c.master", ESP_LOG_NONE);
  esp_log_level_set("i2c", ESP_LOG_NONE);
  esp_log_level_set("driver", ESP_LOG_NONE);

  delay(2000);
 Serial.begin(115200);
Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); 
dbSerialBegin(115200); // Para monitoramento no PC
  delay(1000);
dbSerial.println("=== INICIANDO SISTEMA DESVIADOR ESTEIRAS ===");
nexInit();  //inicializa o tft
nexattachPops();
MyEEPROM.init();
pinMode(WP,OUTPUT); // pin proteçao da eeprom
digitalWrite(WP,HIGH); // EEPROM MODO PROTEGIDO
pinMode(Q0, OUTPUT);
digitalWrite(Q0,HIGH);
  if (! rtc.begin()) { 
     dbSerial.println("falha no RTC");
  }
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // ========== INICIALIZAR E VERIFICAR PCF8574 ==========
  initPCF8574();
  
  dbSerial.println("a carregar dados NVRAM values:");
  dbSerial.println("CARREGANDO IHM...");
  delay(100); // aguarde equanto a ihm inicializa

  // ========== CRIAR MUTEX PARA PROTEÇÃO DE VARIÁVEIS ==========
  xMutex = xSemaphoreCreateMutex();
  if (xMutex == NULL) {
    dbSerial.println("[ERRO] Falha ao criar Mutex!");
  } else {
    dbSerial.println("[OK] Mutex criado com sucesso");
  }
  
  // ========== CRIAR TASK NO CORE 0 (CONTROLE CRÍTICO) ==========
  xTaskCreatePinnedToCore(
    TaskControle,          // Função da Task
    "TaskControle",        // Nome da Task
    10000,                 // Stack size (bytes)
    NULL,                  // Parâmetros
    2,                     // Prioridade (2 = Alta, maior que loop)
    &TaskControleHandle,   // Handle da Task
    0                      // Core 0 (dedicado ao controle)
  );
  
  if (TaskControleHandle == NULL) {
    dbSerial.println("[ERRO] Falha ao criar Task Core 0!");
  } else {
    dbSerial.println("[OK] Task Core 0 criada - Controle Crítico ativo");
  }
  
  delay(500); // Aguardar Task inicializar
  
  // ===== INICIAR WiFi em BACKGROUND (NÃO BLOQUEIA) =====
  // WiFi conectará automaticamente em segundo plano
  // Sistema de controle funciona INDEPENDENTE do WiFi
  initWiFi();

  dbSerial.println("\n=== SISTEMA DUAL-CORE INICIADO ===");
  dbSerial.println("CORE 0: IHM + Relés + Sequencial (Tempo Real)");
  dbSerial.println("CORE 1: WiFi + MQTT + Sensores (Background)");
  dbSerial.println("\n[INFO] Sistema de controle ativo - WiFi conectando em background");
  dbSerial.println("[INFO] Desviadores funcionam mesmo sem WiFi\n");

} // end setup

// ========== LOOP CORE 1 - BACKGROUND (WiFi/MQTT/Sensores) ==========
void loop() {
  loopCore1Count++;
  
  static unsigned long cloock_sensors = 0;
  
  // ===== LEITURA DE SENSORES 1s (com mutex) =====
  if (millis() - cloock_sensors > 1000) {
    if (xSemaphoreTake(xMutex, 20 / portTICK_PERIOD_MS)) {
      if(telaAtiva == 0) {
        calculos(); // RTC, Bateria, Pressão
      }
      xSemaphoreGive(xMutex);
    }
    cloock_sensors = millis();
  }
  
  // ===== VERIFICAR CONEXÕES WiFi/MQTT =====
  checkConnections();
  
  // ===== MANTER CONEXÃO MQTT ATIVA (COM PROTEÇÃO) =====
  if (mqttConnected && client.connected()) {
    client.loop(); // Processa mensagens MQTT de forma não-bloqueante
  } else if (mqttConnected && !client.connected()) {
    // Conexão MQTT perdida - atualizar estado
    mqttConnected = false;
    dbSerial.println("[MQTT] Conexão perdida - marcando como desconectado");
  }
  
  // ===== PUBLICAR STATUS MQTT 10s =====
  static unsigned long lastMqttPublish = 0;
  if (millis() - lastMqttPublish > 10000) {
    lastMqttPublish = millis();
    if (xSemaphoreTake(xMutex, 100 / portTICK_PERIOD_MS)) {
      publishStatus();
      xSemaphoreGive(xMutex);
    } else {
      // Mutex ocupado - pular publicação para não travar
      dbSerial.println("[WARN] Mutex ocupado - pulando publicação MQTT");
    }
  }
  
  // ===== RELATÓRIO DE PERFORMANCE 30s (Debug) =====
  if (millis() - lastPerfReport > 30000) {
    dbSerial.println("\n=== PERFORMANCE DUAL-CORE ===");
    dbSerial.print("Core 0 (Controle): ");
    dbSerial.print(loopCore0Count);
    dbSerial.println(" ciclos/30s");
    dbSerial.print("Core 1 (WiFi/MQTT): ");
    dbSerial.print(loopCore1Count);
    dbSerial.println(" ciclos/30s");
    
    loopCore0Count = 0;
    loopCore1Count = 0;
    lastPerfReport = millis();
  }
  
  // Delay para não sobrecarregar Core 1
  delay(10);
  
}// end loop

void calculos(){

DateTime now = rtc.now();
Timex[0] = now.day(), DEC;
Timex[1] = now.month(), DEC;
Timex[2] = now.year(), DEC;
Timex[3] = now.hour(), DEC;
Timex[4] = now.minute(), DEC;
Timex[5] = now.second(), DEC;

if(Timex[3] != horaAnterior){
semanaN[1] = now.dayOfTheWeek();
dbSerial.println(DiasDaSemana[semanaN[1]]);
Numsemana = semanaN[1],DEC;
horaAnterior = Timex[3];
}
BAR = analogRead(ANALOG_PIN_0);     // a cada 1 seg le pino  estrada concersor 4 a 20ma
BAR = map(BAR, 100, 4095, 0, 188);  // conersao para bar >> psi

bateria();
}//end calculos


void bateria(){
tensaoEntrada = analogRead(ANALOG_PIN_1)*3.3/3750; //VARIÁVEL RECEBE O RESULTADO D

dbSerial.print("tensao Entrada: ");
dbSerial.print(tensaoEntrada);
tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2)); //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
 // tensaoMedida = tensaoEntrada / 0,2; 
 //tensaoMedida =+ 0.5;
dbSerial.print("   tensao Calculada: ");
dbSerial.print(tensaoMedida);
bufferVR++;

if (bufferVR < 6){
  bufferMedia[bufferVR] = tensaoMedida;
}else{
 for (int i = 0; i < 6; ++i) {
 ResultMedia += bufferMedia[i];
  }
 tensaoMedia = ResultMedia / 5;
 bufferVR = 0;
 ResultMedia = 0.0;

}

dbSerial.print("   tensao Média: ");
dbSerial.println(tensaoMedia);
}
