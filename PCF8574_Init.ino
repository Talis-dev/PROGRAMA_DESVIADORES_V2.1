// ========== FUNÇÕES DE INICIALIZAÇÃO E DIAGNÓSTICO PCF8574 ==========

void scanI2CBus() {
  dbSerial.println("\n[I2C SCANNER] Procurando dispositivos no barramento...");
  dbSerial.println("Endereço  | Status");
  dbSerial.println("----------|--------");
  
  int deviceCount = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t error = Wire.endTransmission();
    
    if (error == 0) {
      dbSerial.print("0x");
      if (addr < 16) dbSerial.print("0");
      dbSerial.print(addr, HEX);
      dbSerial.print("      | ENCONTRADO");
      
      // Identificar dispositivos conhecidos
      if (addr == 0x38 || addr == 0x39 || addr == 0x20 || addr == 0x21 || 
          addr == 0x22 || addr == 0x23 || addr == 0x24 || addr == 0x25 ||
          addr == 0x26 || addr == 0x27) {
        dbSerial.print(" (PCF8574");
        if (addr >= 0x38) dbSerial.print("A");
        dbSerial.print(")");
      } else if (addr == 0x68) {
        dbSerial.print(" (RTC DS1307)");
      } else if (addr == 0x50 || addr == 0x51 || addr == 0x52 || 
                 addr == 0x53 || addr == 0x54 || addr == 0x55 ||
                 addr == 0x56 || addr == 0x57) {
        dbSerial.print(" (EEPROM 24LC256)");
      }
      dbSerial.println();
      deviceCount++;
    }
    delay(5); // Pequeno delay para estabilidade
  }
  
  dbSerial.println("----------|--------");
  dbSerial.print("Total: ");
  dbSerial.print(deviceCount);
  dbSerial.println(" dispositivo(s) encontrado(s)\n");
  
  // DIAGNÓSTICO DE PROBLEMAS NO BARRAMENTO
  if (deviceCount == 0) {
    dbSerial.println("[ERRO] Nenhum dispositivo I2C encontrado!");
    dbSerial.println("[AÇÃO] Verifique conexões SDA (GPIO21) e SCL (GPIO22)");
  } else if (deviceCount > 10) {
    dbSerial.println("╔═══════════════════════════════════════════════════════╗");
    dbSerial.println("║  ⚠️  ERRO CRÍTICO NO BARRAMENTO I2C DETECTADO  ⚠️   ║");
    dbSerial.println("╚═══════════════════════════════════════════════════════╝");
    dbSerial.println("");
    dbSerial.print("[DIAGNÓSTICO] Detectados ");
    dbSerial.print(deviceCount);
    dbSerial.println(" dispositivos - IMPOSSÍVEL!");
    dbSerial.println("");
    dbSerial.println("CAUSAS PROVÁVEIS:");
    dbSerial.println("  1. ❌ SDA/SCL SEM RESISTORES PULL-UP (4.7kΩ)");
    dbSerial.println("  2. ❌ Curto-circuito nas linhas SDA ou SCL");
    dbSerial.println("  3. ❌ PCF8574 travado/defeituoso puxando linhas");
    dbSerial.println("  4. ❌ Cabos I2C muito longos (>30cm)");
    dbSerial.println("  5. ❌ Interferência elétrica severa");
    dbSerial.println("");
    dbSerial.println("SOLUÇÕES IMEDIATAS:");
    dbSerial.println("  ✅ Adicionar resistor 4.7kΩ entre SDA e 3.3V");
    dbSerial.println("  ✅ Adicionar resistor 4.7kΩ entre SCL e 3.3V");
    dbSerial.println("  ✅ Desconectar TODOS dispositivos I2C");
    dbSerial.println("  ✅ Reconectar 1 por vez para identificar defeituoso");
    dbSerial.println("  ✅ Verificar multímetro: SDA/SCL deve estar ~3.3V");
    dbSerial.println("  ✅ Usar cabos curtos (<20cm) e blindados");
    dbSerial.println("");
    dbSerial.println("TESTE RÁPIDO:");
    dbSerial.println("  1. Desligue o ESP32");
    dbSerial.println("  2. Meça tensão SDA: deve ser ~3.3V (com pull-up)");
    dbSerial.println("  3. Meça tensão SCL: deve ser ~3.3V (com pull-up)");
    dbSerial.println("  4. Se 0V ou flutuante: FALTA PULL-UP!");
    dbSerial.println("");
    dbSerial.println("══════════════════════════════════════════════════════════");
  }
}

bool testPCF8574(uint8_t address, const char* nome) {
  for (int retry = 0; retry < 3; retry++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      // PCF8574 encontrado - tentar ler um pino
      extern PCF8574 PCF_38;
      extern PCF8574 PCF_39;
      
      uint8_t teste;
      if (address == 0x38) {
        teste = PCF_38.read(0);
      } else if (address == 0x39) {
        teste = PCF_39.read(0);
      } else {
        teste = 0;
      }
      
      dbSerial.print("[");
      dbSerial.print(nome);
      dbSerial.print("] OK - Pino 0: ");
      dbSerial.println(teste);
      return true;
    }
    delay(50);
  }
  
  dbSerial.print("[ERRO] ");
  dbSerial.print(nome);
  dbSerial.print(" (0x");
  dbSerial.print(address, HEX);
  dbSerial.println(") NÃO RESPONDE!");
  return false;
}

void initPCF8574() {
  dbSerial.println("[PCF8574] Inicializando módulos I2C...");
  
  extern PCF8574 PCF_38;
  extern PCF8574 PCF_39;
  
  PCF_38.begin(); // Módulo 0x38 (RL[2,4,6,8,10,12,14,16])
  PCF_39.begin(); // Módulo 0x39 (RL[1,3,5,7,9,11,13,15])
  
  delay(100); // Aguardar estabilização I2C
  
  // VERIFICAR COMUNICAÇÃO
  bool pcf38_ok = testPCF8574(0x38, "PCF_38");
  if (!pcf38_ok) {
    dbSerial.println("[WARN] RL[2,4,6,8,10,12,14,16] podem não funcionar");
  }
  
  bool pcf39_ok = testPCF8574(0x39, "PCF_39");
  if (!pcf39_ok) {
    dbSerial.println("[WARN] RL[1,3,5,7,9,11,13,15] podem não funcionar");
  }
  
  // Resumo de inicialização
  if (pcf38_ok && pcf39_ok) {
    dbSerial.println("[OK] Ambos PCF8574 inicializados com sucesso");
  } else {
    dbSerial.println("[ERRO CRÍTICO] Falha na comunicação I2C com PCF8574!");
    dbSerial.println("[INFO] Verifique: cabos SDA/SCL, alimentação 5V, pull-ups 4.7k");
    
    // Executar scanner I2C para diagnóstico
    scanI2CBus();
    
    dbSerial.println("[AÇÃO] PCF8574 pode estar em endereço diferente");
    dbSerial.println("[DICA] Verifique jumpers A0, A1, A2 no módulo PCF8574");
    dbSerial.println("       PCF8574: 0x20-0x27 (jumpers GND)");
    dbSerial.println("       PCF8574A: 0x38-0x3F (jumpers GND)");
    
    // Piscar LED de erro
    extern int Q0;
    for (int i = 0; i < 5; i++) {
      digitalWrite(Q0, LOW);
      delay(100);
      digitalWrite(Q0, HIGH);
      delay(100);
    }
  }
}
