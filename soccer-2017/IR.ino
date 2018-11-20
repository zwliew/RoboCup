void initIR() {
  Wire.beginTransmission(IRADDR_F);
  Wire.write(14);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_F,1);

  Wire.beginTransmission(IRADDR_B);
  Wire.write(14);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_B,1);
}

int readIRstrF() {
  Wire.beginTransmission(IRADDR_F);
  Wire.write(9);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_F,1);
  //WAIT(1);
  return Wire.read();
}

int readIRdirF() {
  Wire.beginTransmission(IRADDR_F);
  Wire.write(8);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_F,1);
  //WAIT(1);
  return Wire.read();
}

int readIRstrB() {
  Wire.beginTransmission(IRADDR_B);
  Wire.write(9);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_B,1);
  return Wire.read();
}

int readIRdirB() {
  Wire.beginTransmission(IRADDR_B);
  Wire.write(8);
  Wire.endTransmission();
  Wire.requestFrom(IRADDR_B,1);
  return Wire.read();
}

void readIR() {
int IR_Fstr=readIRstrF(), IR_Bstr=readIRstrB();
  if (IR_Fstr>IR_Bstr) {
    IRstr=IR_Fstr;
    IRdir=readIRdirF();
  } else {
    IRstr=IR_Bstr;
    IRdir=15-readIRdirB();
  }
  IRnearThreshval=IRnearThresh[IRdir-1];
  if (IRstr>IRnearThreshval){
    IRnear = true;
  }
  else if (IRstr>(IRnearThreshval-IRanglechange)){
    IRnearish = true;
    IRnear = false;
  }
  else {
    IRnear = false;
    IRnearish = false;
  }
}
