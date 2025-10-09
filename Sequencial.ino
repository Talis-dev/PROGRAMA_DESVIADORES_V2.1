extern int RL[];

extern uint32_t varN0;
extern uint32_t varN1;
extern uint32_t varN2;
extern uint32_t varN3;
extern uint32_t varN4;
extern uint32_t varN5;
extern uint32_t varN6;
extern uint32_t varN7;
extern uint32_t varN8;
extern uint32_t varN9;
extern uint32_t varN10;
//variavel do botao tela 1 //
extern uint32_t bt0var,bt1var,bt2var,bt3var,bt4var,bt5var,bt6var,bt7var,bt8var,bt9var,bt10var,bt11var,bt12var;
extern uint32_t imagem1,imagem2,imagem3,imagem4,imagem5,imagem6,imagem7,imagem8,imagem9,imagem10,imagem11,imagem12,imagem13;
int Ant1=0,Ant2=0,Ant3=0,Ant4=0,Ant5=0,Ant6=0,Ant7=0,Ant8=0,Ant9=0,Ant10=0,Ant11=0,Ant12=0,Ant13=0;
//--variavel entrada de numero ihm -//
extern int tempo0,tempo1,tempo2,tempo3,tempo4,tempo5,tempo6,tempo7,tempo8,tempo9,tempo10,tempo11;

extern int telaAtiva;
// bool int para acionar etapas //
extern bool M1,M2,M3,M4,M5,M6,M7,M8,M9,M10,M11,M12;
bool xt=0;
bool ativo = 0;
bool ativo2 = 0;
long int tmr = 0;


void carregaTempo() { 
if(telaAtiva == 0){

if(imagem1 != Ant1){Ant1 = imagem1;p0.setPic(imagem1);  }

if(imagem2 != Ant2){Ant2 = imagem2;p1.setPic(imagem2);  }

if(imagem3 != Ant3){Ant3 = imagem3;p2.setPic(imagem3);  }

if(imagem4 != Ant4){Ant4 = imagem4;p3.setPic(imagem4);  }

if(imagem5 != Ant5){Ant5 = imagem5;p4.setPic(imagem5);  }

if(imagem6 != Ant6){Ant6 = imagem6;p5.setPic(imagem6);  }

if(imagem7 != Ant7){Ant7 = imagem7;p6.setPic(imagem7);  }

if(imagem8 != Ant8){Ant8 = imagem8;p7.setPic(imagem8);  }

if(imagem9 != Ant9){Ant9 = imagem9;p8.setPic(imagem9);  }

if(imagem10 != Ant10){Ant10 = imagem10;p9.setPic(imagem10);  }

if(imagem11 != Ant11){Ant11 = imagem11;p10.setPic(imagem11);  }

if(imagem12 != Ant12){Ant12 = imagem12;p11.setPic(imagem12);  }

 
}// IF TELA ATIVA

  //------------------------------ TEMPO 1 ----------------//


  if (bt0var == 1) {

  if (M1 == 0) {imagem1 = 0;}

    if (tempo0 >=  varN0 && M1 == 0) {
      tempo1 = 0;
      RL[1]=0;
      M1 = 1;
      imagem1 = 1;
      
    }
  } if (bt0var == 0 && M1 == 0) {
    tempo1 = 0;
    M1 = 1;
  } if (bt0var == 0 && ativo == 1){
       RL[1]=0; 
         imagem1 = 1;  }

  //------------------------------ TEMPO 2 ----------------//
  if (M1 == 1) {
    if (bt1var == 1) {
      if (M2 == 0) {imagem2 = 0;}

      if (tempo1 >= varN1 && M2 == 0) {
        tempo2 = 0;
         RL[2]=0;
        M2 = 1;
          imagem2 = 1;
      }
    } if (bt1var == 0 && M2 == 0) {
      tempo2 = 0;
      M2 = 1;
    } if (bt1var == 0 && ativo == 1){
       RL[2]=0;
         imagem2 = 1;    }
  }
  //------------------------------ TEMPO 3 ----------------//
  if (M2 == 1) {
    if (bt2var == 1) {
      if (M3 == 0) {imagem3 = 0;}
      if (tempo2 >= varN2 && M3 == 0) {
        tempo3 = 0;
        RL[3]=0;
        M3 = 1;
  imagem3 = 1;
      }
    } if (bt2var == 0 && M3 == 0) {
      tempo3 = 0;
      M3 = 1;
    } 
    if (bt2var == 0 && ativo == 1){
       RL[3]=0;   
    imagem3 = 1; }
  }
    
  //------------------------------ TEMPO COXA FINAL RESET ----------------//
 
  if (M3 == 1) {
    imagem4 = 0;
  if (bt3var == 1){
   //  if (tempo3 <= varN3) {
 //imagem4 = 2;
    if (tempo3 >= varN3) { 
      reinicia01();
    }
     //}
  } 
  if (bt3var == 0 && ativo == 1 ){   
       reinicia01();
      }
  }  
  
if (bt0var==1 || bt1var==1 || bt2var==1){
  ativo=1;
}else { ativo=0; }



 //------------------------------ TEMPO 5 dorsal ----------------//

  if (bt4var == 1 && bt5var == 1) {
    if (tempo4 >= varN8 && M4 == 0) {
       RL[4]=1;
      M4 = 1;
      imagem5 = 0;
      imagem6 = 1;
      tempo5 = 0;
     }
  if (M4 == 1) {
      if (tempo5 >= varN9) {
        imagem5 = 1;
        imagem6 = 0;      
         RL[4]=0;
        tempo4=0;
        tempo5=0;
        M4 = 0;
       }
    } 

  }
    
  if (bt4var == 1 && bt5var == 0) {
    RL[4]=1;
   imagem5 = 0;
   imagem6 = 1;
    }
  if (bt5var == 1 && bt4var == 0) {
    RL[4]=0;
   imagem5 = 1;
   imagem6 = 0;
    }
  if (bt5var == 0 && bt4var == 0) {
    RL[4]=0;
   imagem5 = 1;
   imagem6 = 0;
    }
     
                       //------------------------------ TEMPO 1 PEITO ----------------//
 if (bt6var == 1) {
  if (M5 == 0) {imagem7 = 0;}

    if (tempo6 >=  varN4 && M5 == 0) {
      tempo7 = 0;
      RL[5]=0;
      M5 = 1;
      imagem7 = 1;  }
  }

   if (bt6var == 0 && M5 == 0) {
    tempo7 = 0;
    M5 = 1; } 

  if (bt6var == 0 && ativo2 == 1){
       RL[5]=0; 
         imagem7 = 1;  }

                         //------------------------------ TEMPO 2  PEITO ----------------//

  if (M5 == 1) {

    if (bt7var == 1) {
      if (M6 == 0) {imagem8 = 0;}

      if (tempo7 >= varN5 && M6 == 0) {
        tempo8 = 0;
         RL[6]=0;
        M6 = 1;
          imagem8 = 1; } 
     } 

    if (bt7var == 0 && M6 == 0) {
      tempo8 = 0;
      M6 = 1;
    } if (bt7var == 0 && ativo2 == 1){
       RL[6]=0;
         imagem8 = 1;    }
  }

                         //------------------------------ TEMPO 3  PEITO ----------------//

  if (M6 == 1) {

    if (bt8var == 1) {
      if (M7 == 0) {imagem9 = 0;}

      if (tempo8 >= varN6 && M7 == 0) {
        tempo9 = 0;
         RL[7]=0;
        M7 = 1;
          imagem9 = 1; } 
     } 
     
    if (bt8var == 0 && M7 == 0) {
      tempo9 = 0;
      M7 = 1;
    } if (bt8var == 0 && ativo2 == 1){
       RL[7]=0;
         imagem9 = 1;    }
  }

                         //------------------------------ TEMPO 4 FINAL RESET PEITO ----------------//

  if (M7 == 1) {
    imagem10 = 0;
  if (bt9var == 1){
 
    if (tempo9 >= varN7) {
      imagem10 = 1; 
      reinicia02();
     }
  } 
  if (bt9var == 0 && ativo2 == 1 ){ 
      imagem10 = 1;  
       reinicia02();
      }
  }  
  
  if (bt6var==1 || bt7var==1 || bt8var==1){
  ativo2=1;
}else { ativo2=0; }


//------------------------------ TEMPO  coxa DESOSSADA esteira azul ----------------//

 if (bt11var == 1 && bt12var == 1) {
  if(M8 == 0){
 imagem11 = 0;
 imagem12 = 1;
  }else{
imagem11 = 1;
 imagem12 = 0;
  }
    if (tempo10 >= varN10 && M8 == 0) {
       RL[8]=1;
      M8 = 1;
      tempo11 = 0;
     }
  if (M8 == 1) {
      if (tempo11 >= varN11) {     
         RL[8]=0;
        tempo10=0;
        tempo11=0;
        M8 = 0;
       }
    } 

  }
    
  if (bt11var == 0 && bt12var == 1) {
    RL[8]=1;
   imagem11 = 1;
   imagem12 = 0;
    }
  if (bt11var == 1 && bt12var == 0) {
    RL[8]=0;
   imagem11 = 0;
   imagem12 = 1;
    }
  if (bt11var == 0 && bt12var == 0) {
    RL[8]=0;
   imagem11 = 0;
   imagem12 = 1;
    }
     
} // END VOID CARREGA TEMPO

void reinicia01(){
   imagem4 = 1;
  if (bt0var == 1){ RL[1]=1;}
  if (bt1var == 1){ RL[2]=1;}
  if (bt2var == 1){ RL[3]=1;}
  //if (bt3var == 1)
  M1 = 0; M2 = 0; M3 = 0;  
  tempo0 = 0;
  tempo1 = 0;
  tempo2 = 0;
  tempo3 = 0;
  }
void reinicia02(){
   imagem10 = 1;
  if (bt6var == 1){ RL[5]=1;}
  if (bt7var == 1){ RL[6]=1;}
  if (bt8var == 1){ RL[7]=1;}
  //if (bt9var == 1) 
    
 M5 = 0; M6 = 0; M7 = 0;
  tempo6 = 0;
  tempo7 = 0;
  tempo8 = 0;
  tempo9 = 0;
}
