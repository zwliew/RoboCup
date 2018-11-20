int rangeUS[SONAR_NUM]; // 1=US_Left, 2=US_Back, 3=US_Right

void initNano() {  // Not initiation, just for diagnostics to see if ultrasonic actually works
  Wire.beginTransmission(MicroADDR); //Request from slave
  Wire.requestFrom(MicroADDR,18); //Request 18 values from slave
  if (Wire.available() == 18) {
    for (int i=0; i<SONAR_NUM; i++) rangeUS[i]=Wire.read();
    for (int i=0; i<14; i++) light[i]=Wire.read();
  }
  Wire.endTransmission();
}

void readNano() {
  Wire.beginTransmission(MicroADDR); //Request from slave
  Wire.requestFrom(MicroADDR,18); //Request 18 values from slave
  Wire.endTransmission();
  if (Wire.available() == 18) {
    for (int i=0; i<SONAR_NUM; i++) rangeUS[i]=Wire.read();
    for (int i=0; i<14; i++) light[i]=Wire.read();
  }

  for (uint8_t i = 0; i < 14; i++) {
    //Serial.print(i);
    //Serial.print("=");
    //Serial.print(light[i]);
    //Serial.print("  ");
  }
 //Serial.println("");
  
}

int getRangeB() {
  //Serial.print(rangeUS[1]);
  //Serial.print("  ");
 // return (cos(cmpsangleradian)*rangeUS[1]);
  return (rangeUS[1]);
}

int getRangeR() {
  //Serial.print(rangeUS[3]);
  //Serial.print("  ");
 // return (cos(cmpsangleradian)*rangeUS[3]);
  return (rangeUS[3]);
}

int getRangeF() {
  //Serial.print(rangeUS[2]);
  //Serial.print("  ");
 // return (cos(cmpsangleradian)*rangeUS[2]);
  return (rangeUS[2]);
}

int getRangeL() {
  //Serial.print(rangeUS[0]);
  //Serial.print("  ");
 // return (cos(cmpsangleradian)*rangeUS[0]);
  return (rangeUS[0]);
}




