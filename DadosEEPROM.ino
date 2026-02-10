extern uint32_t varN0, varN1, varN2, varN3, varN4,  varN5,  varN6, varN7, varN8, varN9, varN10, tempoAtt;
extern uint32_t bt0var,bt1var,bt2var,bt3var,bt4var,bt5var,bt6var,bt7var,bt8var,bt9var,bt10var,bt11var,bt12var;
extern int RL[];
int estado_anterior[30] = {0};

void LoadingEeprom(){

//estado_anterior[29] = autoONvar = MyEEPROM.read(29);

bt0var = MyEEPROM.read(0);
 bt0.setValue(bt0var);
 delay(100);
bt1var = MyEEPROM.read(1);
 bt1.setValue(bt1var);
 delay(100);
bt2var = MyEEPROM.read(2);
 bt2.setValue(bt2var);
 delay(100);
bt3var = MyEEPROM.read(3);
 bt3.setValue(bt3var);
 delay(100);
bt4var = MyEEPROM.read(4);
 bt4.setValue(bt4var);
 delay(100);
bt5var = MyEEPROM.read(5);
 bt5.setValue(bt5var);
 delay(100);
bt6var = MyEEPROM.read(6);
 bt6.setValue(bt6var);
 delay(100);
bt7var = MyEEPROM.read(7);
 bt7.setValue(bt7var);
 delay(100);
bt8var = MyEEPROM.read(8);
 bt8.setValue(bt8var);
 delay(100);
bt9var = MyEEPROM.read(9);
 bt9.setValue(bt9var);
 delay(100);
bt10var = MyEEPROM.read(10);
 bt10.setValue(bt10var);
 delay(100);
bt11var = MyEEPROM.read(11);
 bt11.setValue(bt11var);
 delay(100);
bt12var = MyEEPROM.read(12);
 bt12.setValue(bt12var);



varN0 = MyEEPROM.read(31);
varN1 = MyEEPROM.read(32);
varN2 = MyEEPROM.read(33);
varN3 = MyEEPROM.read(34);
varN4 = MyEEPROM.read(35);
varN5 = MyEEPROM.read(36);
varN6 = MyEEPROM.read(37);
varN7 = MyEEPROM.read(38);
varN8 = MyEEPROM.read(39);
varN9 = MyEEPROM.read(41);
varN10 = MyEEPROM.read(42);
varN11 = MyEEPROM.read(43);

// ========== CRÍTICO: INICIALIZAR RELÉS BASEADO NOS BOTÕES ==========
// Sem isso, relés ficam em estado indefinido até ciclo passar por eles
// Causava bug: RL[6] não operava até bt8 ser ativado

// COXA - RL[1], RL[2], RL[3]
if (bt0var == 1) { RL[1] = 1; } else { RL[1] = 0; }
if (bt1var == 1) { RL[2] = 1; } else { RL[2] = 0; }
if (bt2var == 1) { RL[3] = 1; } else { RL[3] = 0; }

// DORSAL - RL[4]
if (bt4var == 1 && bt5var == 0) { RL[4] = 1; } else { RL[4] = 0; }

// PEITO - RL[5], RL[6], RL[7] - CRÍTICO PARA BUG DO RL[6]
if (bt6var == 1) { RL[5] = 1; } else { RL[5] = 0; }
if (bt7var == 1) { RL[6] = 1; } else { RL[6] = 0; }
if (bt8var == 1) { RL[7] = 1; } else { RL[7] = 0; }

// DESOSSADA - RL[8]
if (bt11var == 0 && bt12var == 1) { RL[8] = 1; } else { RL[8] = 0; }

dbSerial.println("[EEPROM] Estados carregados + Relés inicializados");
dbSerial.print("  Coxa: RL[1]="); dbSerial.print(RL[1]); 
dbSerial.print(" RL[2]="); dbSerial.print(RL[2]); 
dbSerial.print(" RL[3]="); dbSerial.println(RL[3]);
dbSerial.print("  Peito: RL[5]="); dbSerial.print(RL[5]); 
dbSerial.print(" RL[6]="); dbSerial.print(RL[6]); 
dbSerial.print(" RL[7]="); dbSerial.println(RL[7]);

}