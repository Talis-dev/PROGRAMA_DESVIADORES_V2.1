# ⚙️ Sistema de Distribuição Automática — ESP32 + Nextion + MQTT + I2C

Este projeto implementa um **controlador inteligente para linhas de distribuição em esteiras industriais**, utilizando **ESP32**, **display Nextion**, **expansores I²C (PCF8574)** e **memória EEPROM 25LC256**.  
O sistema foi desenvolvido para controlar múltiplas saídas temporizadas e comunicar-se via **Wi-Fi / MQTT** para supervisão e ajuste remoto.

---

## 🧩 Visão Geral do Sistema

O controlador é responsável por coordenar **4 esteiras independentes**, cada uma com vários **distribuidores temporizados**, permitindo o controle preciso da liberação de produtos em movimento.

Cada saída é acionada por tempo definido (em milissegundos) e pode ser **ativada ou desativada manualmente** pela IHM ou **remotamente via MQTT**.

O sistema também realiza o **monitoramento do nível da bateria**, **sinal Wi-Fi**, e **pressão de ar comprimido** (sensor industrial 4–20 mA convertido para 0–14 bar).

---

## 🧠 Funcionalidades Principais

### 🔹 Controle de Distribuição
- Até **16 saídas digitais** (via 2 × PCF8574)
- Cada saída possui **temporizador independente** e **botão de habilitação**
- Controle manual pela IHM Nextion
- Controle remoto via MQTT (`set_timer`, `set_button`)

### 🔹 Monitoramento em Tempo Real
- **Tensão da bateria** (indicador visual na IHM)
- **Pressão do ar comprimido** em bar (0 – 14 bar)
- **Sinal Wi-Fi** (ícone com níveis 0–4)
- **Status de conexão MQTT** e envio automático de fila offline

### 🔹 Armazenamento e Persistência
- Memória **EEPROM 25LC256** para salvar:
  - Tempos configurados (`varN0` – `varN11`)
  - Estados dos botões (`bt0` – `bt12`)
- Recuperação automática após reinicialização
- Sistema de **fila de mensagens offline** via SPIFFS (`/queue.log`)  
  → garante que nenhuma publicação MQTT seja perdida em falhas de rede.

### 🔹 Comunicação e Integração
- **MQTT** via Wi-Fi ou Ethernet W5500 (auto fallback)
- Reenvio automático de dados armazenados em fila
- **Comandos remotos JSON**:
  ```json
  {
    "command": "set_timer",
    "index": 3,
    "value": 25
  }

### 🔹 Envio periódico de telemetria com dados de operação e sensores

{
  "command": "set_button",
  "index": 5,
  "state": 1
}


💡 Aplicações

✔️ Linhas de envase ou empacotamento automático
✔️ Distribuição controlada por tempo de produtos ou ração
✔️ Sistemas de corte ou separação por pulso de tempo
✔️ Acionamento de cilindros pneumáticos sincronizados

🖥️ Interface IHM Nextion

A IHM exibe e permite controlar:

Elemento	Função
p13	Indicador gráfico de nível da bateria
p12	Indicador de sinal Wi-Fi (id 2 – 6)
n0 – n11	Temporizadores individuais das saídas
bt0 – bt12	Botões de habilitação de saídas
BAR	Leitura de pressão (em bar)
nm0	Indicador de pressostato ativo
⚡ Hardware Utilizado
Componente	Função
ESP32 DevKit	Unidade principal de controle
Nextion TFT 4.3"	Interface de usuário e ajuste de parâmetros
2 × PCF8574	Expansão de 16 saídas digitais
EEPROM 25LC256	Armazenamento persistente de configurações
Conversor 4–20 mA → 0–3.3 V	Leitura do sensor de pressão industrial
Sensor de bateria (ADC)	Monitoramento da tensão de 7.8 V
W5500 (opcional)	Ethernet alternativa ao Wi-Fi
Fonte 24 VDC / Bateria 7.8 V	Alimentação do sistema e recarga

Tensões internas:

24 V: potência dos relés e válvulas

5 V / 3.3 V: lógica e microcontroladores

7.8 V: bateria recarregável do sistema

🔌 Conexões Importantes
Pino ESP32	Função
GPIO 4	Leitura analógica (bateria)
GPIO 15	Sensor de pressão (ADC)
GPIO 16 / 17	UART2 (Nextion)
I²C SDA/SCL	PCF8574 + EEPROM
SPI	W5500 Ethernet (opcional)

⚠️ Nota: GPIO 4 e 15 não podem ser usados como ADC enquanto o Wi-Fi estiver ativo, pois compartilham canais internos do RF.
Se o Wi-Fi for essencial, utilize outros ADCs como GPIO 32 – 35.

🛰️ MQTT — Tópicos e Estrutura
Tópico	Descrição
distribuidor/command	Recebe comandos JSON de configuração
distribuidor/status	Publica estado geral do sistema
distribuidor/currentDataBasic	Envia dados de sensores e estado atual
distribuidor/queue	Armazena publicações não enviadas


📦 Estrutura de Arquivos no SPIFFS
/queue.log   → mensagens pendentes MQTT
/queue.tmp   → arquivo temporário de reenvio


🔧 Recursos Técnicos

Plataforma: PlatformIO / Arduino Core ESP32

Bibliotecas principais:

PubSubClient — MQTT

SPIFFS — armazenamento local

Wire — comunicação I²C

Ethernet.h — suporte W5500

Nextion.h — interface com display

Preferences — NVS storage (configuração MQTT)



🧾 Licença

Distribuído sob a Licença MIT.
Créditos do desenvolvimento: Thalison Henrique Fernandes.