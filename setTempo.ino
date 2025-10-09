extern uint32_t varN0, varN1, varN2, varN3, varN4,  varN5,  varN6, varN7, varN8, varN9, varN10, tempoAtt, atualizaVar;
uint32_t ds0 = 0;

void Set1(){
 digitalWrite(WP,LOW);
delay(10);
n00.getValue(&varN0);
delay(10);
n1.getValue(&varN1);
delay(10);
n2.getValue(&varN2);
delay(10);
n3.getValue(&varN3);
delay(10);
n4.getValue(&varN4);
delay(10);
n5.getValue(&varN5);
delay(10);
n6.getValue(&varN6);
delay(10);
n7.getValue(&varN7);
delay(10);
n8.getValue(&varN8);
delay(10);
n9.getValue(&varN9);
delay(10);
n10.getValue(&varN10);
delay(10);
n11.getValue(&varN11);
delay(10);
tempoAtt=1;
eeprom_tempos();

    va0.setValue(tempoAtt);         //variavel de retorno para nextion confirmando atualizacao
      atualiza.setValue(ds0); // retorna estado 0 para o botao ds  
      atualizaVar = 0; //zera variavel int nextion 
tempoAtt=0;
Load_timers(); // executa o envio dos dados para tela 1, dlay 500ms

}


void eeprom_tempos(){
delay(10);
MyEEPROM.write(31,varN0);
delay(10);
MyEEPROM.write(32,varN1);
delay(10);
MyEEPROM.write(33,varN2);
delay(10);
MyEEPROM.write(34,varN3);
delay(10);
MyEEPROM.write(35,varN4);
delay(10);
MyEEPROM.write(36,varN5);
delay(10);
MyEEPROM.write(37,varN6);
delay(10);
MyEEPROM.write(38,varN7);
delay(10);
MyEEPROM.write(39,varN8);
delay(10);
MyEEPROM.write(41,varN9);
delay(10);
MyEEPROM.write(42,varN10);
delay(10);
MyEEPROM.write(43,varN11);
delay(10);
digitalWrite(WP,HIGH);

}
