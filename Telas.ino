extern uint32_t atualizaVar;
extern uint32_t nh0V,nh1V,nh2V,nh3V,nh4V,nh5V,nh6V,nh7V,nh8V,nh9V,nh10V,nh11V,nh12V,nh13V;
extern uint32_t imagem1,imagem2,imagem3,imagem4,imagem5,imagem6,imagem7,imagem8,imagem9,imagem10,imagem11,imagem12,imagem13;
extern uint32_t botaoHRvar,diavar,mesvar,anovar,horavar,minutvar,segunvar;
extern int Numsemana;
int sethra[5] = {0};
extern int  soma = 0, telaAtiva = 0;
extern int BAR;
char time_buf[30] = {0};
int Tempo_anterior = 0, soma2 = 0;
int long dlyihm = 0, tempo_espera = 0,tempo_espera2 = 0, dlay = 0, dlay4 = 0;
extern float tensaoMedia;
//----------------void atualiza imagem --------------//


void Varredura() {  // le a tela selecionada 100ms

if(atualizaVar ==1){
  Set1(); // le os dados da tela 1 seta e salva na eprom
}


if(millis() - dlay4 > 1000){ // faz a conversão a cada 1 seg
dlay4 = millis();


time_buf[30] = {0};
String HR;
HR += "DATA ";                 /// formatação da hora e data
HR += String(Timex[0],DEC);
HR += "/";
HR += String(Timex[1],DEC);
HR += "/";
HR += String(Timex[2],DEC);
HR += " HORA ";
HR += String(Timex[3],DEC);
HR += ":";
HR += String(Timex[4],DEC);
HR += ":";
HR += String(Timex[5],DEC);
HR.toCharArray(time_buf, 30); // converte string para char com tamanho 30
}


if(telaAtiva==0){               // sequencial de telas // carrega somente a tela aberta
// tela home 

if(millis() - tempo_espera > 1010){
tempo_espera = millis();

hor.setText(time_buf); // envia txt formato char data e hora
nm0.setValue(BAR); // envia dados do pressostato
Num7.setValue(Numsemana+1);
/*
char bat_buf[5];
voltFloat.toCharArray(bat_buf, 5);
bat.setText(bat_buf);*/

if (tensaoMedia < 4) {
  p13.setPic(7);
}
if (tensaoMedia > 4 && tensaoMedia < 4.5) {
  p13.setPic(8);
}
else if (tensaoMedia >= 4.5 && tensaoMedia < 6) {
  p13.setPic(9);
}
else if (tensaoMedia >= 6.1 && tensaoMedia < 6.5) {
  p13.setPic(10);
}
else if (tensaoMedia >= 6.6 && tensaoMedia < 7) {
  p13.setPic(11);
}
else if (tensaoMedia >= 7.1 && tensaoMedia < 7.8) {
  p13.setPic(13);
}
else if (tensaoMedia >= 7.8) {
  p13.setPic(14);
}

    // ---------- WI-FI ----------
    int rssi = WiFi.RSSI();  // pega intensidade do sinal (dBm)

    if (rssi <= -90 || rssi == 0) {
      p12.setPic(2); // sem sinal ou erro
    }
    else if (rssi > -90 && rssi <= -75) {
      p12.setPic(3); // sinal fraco
    }
    else if (rssi > -75 && rssi <= -60) {
      p12.setPic(4); // sinal médio
    }
    else if (rssi > -60 && rssi <= -45) {
      p12.setPic(5); // sinal bom
    }
    else if (rssi > -45) {
      p12.setPic(6); // sinal cheio
    }

}

}
}

void Load_timers(){
delay(200);
n00.setValue(varN0);
delay(10);
n1.setValue(varN1);
delay(10);
n2.setValue(varN2);
delay(10);
n3.setValue(varN3);
delay(10);
n4.setValue(varN4);
delay(10);
n5.setValue(varN5);
delay(10);
n6.setValue(varN6);
delay(10);
n7.setValue(varN7);
delay(10);
n8.setValue(varN8);
delay(10);
n9.setValue(varN9);
delay(10);
n10.setValue(varN10);
delay(10);
n11.setValue(varN11);
delay(10);

}


void DateHora(){ //ajusta hora rtc
dia.getValue(&diavar);
sethra[0] = diavar;
mes.getValue(&mesvar);
sethra[1] = mesvar;
ano.getValue(&anovar);
sethra[2] = anovar;
hora.getValue(&horavar);
sethra[3] = horavar;
minut.getValue(&minutvar);
sethra[4] = minutvar;
segun.getValue(&segunvar);
sethra[5] = segunvar;
delay(100);

rtc.adjust(DateTime(sethra[2],sethra[1],sethra[0],sethra[3],sethra[4],sethra[5])); // sethra rtc ano/mes/dia  /hora/minuto/segundo
titulo.setText("SUCESSO"); 

  delay(100);
}

