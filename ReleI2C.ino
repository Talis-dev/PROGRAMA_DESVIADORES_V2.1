extern PCF8574 PCF_38; // enderesso modulos saidas i2c
extern PCF8574 PCF_39;  //  (used as output)

extern int RL[];

void WriteI2C() {
   if (RL[1] == 1) {
    PCF_39.write(0, LOW);
  } else {
    PCF_39.write(0, HIGH);
  }

  if (RL[2] == 1) {
    PCF_38.write(0, LOW);
  } else {
    PCF_38.write(0, HIGH);
  }

  if (RL[3] == 1) {
    PCF_39.write(1, LOW);
  } else {
    PCF_39.write(1, HIGH);
  }

  if (RL[4] == 1) {
    PCF_38.write(1, LOW);
  } else {
    PCF_38.write(1, HIGH);
  }

  if (RL[5] == 1) {
    PCF_39.write(2, LOW);
  } else {
    PCF_39.write(2, HIGH);
  }

  if (RL[6] == 1) {
    PCF_38.write(2, LOW);
  } else {
    PCF_38.write(2, HIGH);
  }

  if (RL[7] == 1) {
    PCF_39.write(3, LOW);
  } else {
    PCF_39.write(3, HIGH);
  }

  if (RL[8] == 1) {
    PCF_38.write(3, LOW);
  } else {
    PCF_38.write(3, HIGH);
  }

  if (RL[9] == 1) {
    PCF_39.write(4, LOW);
  } else {
    PCF_39.write(4, HIGH);
  }

  if (RL[10] == 1) {
    PCF_38.write(4, LOW);
  } else {
    PCF_38.write(4, HIGH);
  }

  if (RL[11] == 1) {
    PCF_39.write(5, LOW);
  } else {
    PCF_39.write(5, HIGH);
  }

  if (RL[12] == 1) {
    PCF_38.write(5, LOW);
  } else {
    PCF_38.write(5, HIGH);
  }

  if (RL[13] == 1) {
    PCF_39.write(6, LOW);
  } else {
    PCF_39.write(6, HIGH);
  }

  if (RL[14] == 1) {
    PCF_38.write(6, LOW);
  } else {
    PCF_38.write(6, HIGH);
  }

 if (RL[15] == 1) {
    PCF_39.write(7, LOW);
  } else {
    PCF_39.write(7, HIGH);
  }

  if (RL[16] == 1) {
    PCF_38.write(7, LOW);
  } else {
    PCF_38.write(7, HIGH);
  }


}
