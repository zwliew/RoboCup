//#define SONAR_NUM 3

int rangeUS[SONAR_NUM]; // 1=US_Left, 2=US_Back, 3=US_Right

void initUS() {
  Wire.beginTransmission(MicroADDR); //Request from slave
  Wire.requestFrom(MicroADDR,SONAR_NUM); //Request 3 values from slave
  /*
  if (Wire.available() == SONAR_NUM) {
    for (int i=1; i<=SONAR_NUM; i++) rangeUS[i]=Wire.read();
  }
  */
  Wire.endTransmission();
}

void readUS() {
  Wire.beginTransmission(MicroADDR); //Request from slave
  Wire.requestFrom(MicroADDR,SONAR_NUM); //Request 3 values from slave
  Wire.endTransmission();

  if (Wire.available() == SONAR_NUM) {
    for (int i=0; i<SONAR_NUM; i++) rangeUS[i]=Wire.read();
  }
}


/*
void readUS() {
  Wire.beginTransmission(121); // Request from slave #121
  Wire.requestFrom(121, 3); // Request 3 values from slave
  
  if (Wire.available() == 3) {
    for (int i=1;i<=3;i++) {
      Serial.print(i);
      Serial.print("=");
      Serial.print(Wire.read());
      Serial.print("\t");
      
      rangeUS[i]=Wire.read();
    }
    
    Serial.print(rangeUS[1]);Serial.print("\t");
    Serial.print(rangeUS[2]);Serial.print("\t");
    Serial.print(rangeUS[3]);Serial.println("\t");
    
  }
  Wire.endTransmission();
}
*/

int lastRangeL=0, lastRangeB=0, lastRangeR=0;

int getRangeL() {
  return rangeUS[0];
  /*
  int rawRangeL = rangeUS[1];
  if (rawRangeL == 0) return lastRangeL;
  else {
    lastRangeL = rawRangeL;
    return rawRangeL;
  }
  */
}

int getRangeB() {
  return rangeUS[1];
  /*
  int rawRangeB = rangeUS[2];
  if (rawRangeB == 0) return lastRangeB;
  else {
    lastRangeB = rawRangeB;
    return rawRangeB;
  }
  */
}

int getRangeR() {
  return rangeUS[2];
  /*
  int rawRangeR = rangeUS[3];
  if (rawRangeR == 0) return lastRangeR;
  else {
    lastRangeR = rawRangeR;
    return rawRangeR;
  }
  */
}

