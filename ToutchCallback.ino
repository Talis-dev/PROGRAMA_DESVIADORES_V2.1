extern uint32_t bt0var,bt1var,bt2var,bt3var,bt4var,bt5var,bt6var,bt7var,bt8var,bt9var,bt10var,bt11var,bt12var,atualizaVar;
extern int telaAtiva;
extern bool shiftButton;

void nexattachPops(){
b0.attachPop(b0call, &b0);
vt.attachPop(vtcall, &vt); vt2.attachPop(vt2call, &vt2);vt3.attachPop(vt3call, &vt3);
entra.attachPop(entracall, &entra);
botaoHR.attachPop(botaoHRcall, &botaoHR);
hor.attachPop(horcall, &hor);
bt0.attachPop(bt0call, &bt0);bt7.attachPop(bt7call, &bt7);
bt1.attachPop(bt1call, &bt1);bt8.attachPop(bt8call, &bt8);
bt2.attachPop(bt2call, &bt2);bt9.attachPop(bt9call, &bt9);
bt3.attachPop(bt3call, &bt3);bt10.attachPop(bt10call, &bt10);
bt4.attachPop(bt4call, &bt4);bt11.attachPop(bt11call, &bt11);
bt5.attachPop(bt5call, &bt5);bt12.attachPop(bt12call, &bt12);
bt6.attachPop(bt6call, &bt6);
atualiza.attachPop(atualizacall, &atualiza);
}

void entracall(void *ptr) { // botao na page 3 para ir p/ page 1
telaAtiva = 1;
 Load_timers(); // executa o envio dos dados para tela 1, dlay 500ms
}

void b0call(void *ptr) { // botao na page 0 para ir p/ page 1
 telaAtiva = 3;
 //Load_timers(); // executa o envio dos dados para tela 1, dlay 500ms
}

void atualizacall(void *ptr) {
 atualiza.getValue(&atualizaVar);
}

void vtcall(void *ptr) {// botao voltar na pge 1
 telaAtiva = 0; }
void horcall(void *ptr) {
 telaAtiva = 2; }
void vt2call(void *ptr) { // botao voltar da pge 2
 telaAtiva = 0; }
void vt3call(void *ptr) { // botao voltar da pge 3
 telaAtiva = 0; }

void botaoHRcall(void *ptr) {
 DateHora();}


void bt0call(void *ptr) {
  digitalWrite(WP,LOW);
bt0.getValue(&bt0var);
MyEEPROM.write(0,bt0var);
digitalWrite(WP,HIGH);}

void bt1call(void *ptr) {
  digitalWrite(WP,LOW);
bt1.getValue(&bt1var);
MyEEPROM.write(1,bt1var);
digitalWrite(WP,HIGH);}

void bt2call(void *ptr) {
  digitalWrite(WP,LOW);
bt2.getValue(&bt2var);
MyEEPROM.write(2,bt2var);
digitalWrite(WP,HIGH);}

void bt3call(void *ptr) {
  digitalWrite(WP,LOW);
bt3.getValue(&bt3var);
MyEEPROM.write(3,bt3var);
digitalWrite(WP,HIGH);}

void bt4call(void *ptr) {
  digitalWrite(WP,LOW);
 bt4.getValue(&bt4var);
 MyEEPROM.write(4,bt4var);
 digitalWrite(WP,HIGH);}

void bt5call(void *ptr) {
  digitalWrite(WP,LOW);
 bt5.getValue(&bt5var);
 MyEEPROM.write(5,bt5var);
 digitalWrite(WP,HIGH);}

void bt6call(void *ptr) {
  digitalWrite(WP,LOW);
bt6.getValue(&bt6var);
MyEEPROM.write(6,bt6var);
digitalWrite(WP,HIGH);}

void bt7call(void *ptr) {
  digitalWrite(WP,LOW);
bt7.getValue(&bt7var);
MyEEPROM.write(7,bt7var);
digitalWrite(WP,HIGH);}

void bt8call(void *ptr) {
  digitalWrite(WP,LOW);
bt8.getValue(&bt8var);
MyEEPROM.write(8,bt8var);
digitalWrite(WP,HIGH);}

void bt9call(void *ptr) {
  digitalWrite(WP,LOW);
bt9.getValue(&bt9var);
MyEEPROM.write(9,bt9var);
digitalWrite(WP,HIGH);}

void bt10call(void *ptr) {
  digitalWrite(WP,LOW);
bt10.getValue(&bt10var);
MyEEPROM.write(10,bt10var);
digitalWrite(WP,HIGH);}

void bt11call(void *ptr) {
  digitalWrite(WP,LOW);
bt11.getValue(&bt11var);
MyEEPROM.write(11,bt11var);
digitalWrite(WP,HIGH);}

void bt12call(void *ptr) {
  digitalWrite(WP,LOW);
bt12.getValue(&bt12var);
MyEEPROM.write(12,bt12var);
digitalWrite(WP,HIGH);}


