void initComp(){
  compoffset=readComp();
  prevcompoffset=compoffset;
}

int readComp(){
  byte highByte;
  byte lowByte;

  Wire.beginTransmission(CMPADDR);      //starts communication with cmps03
  Wire.write(2);                         //Sends the register we wish to read
  Wire.endTransmission();

  Wire.requestFrom(CMPADDR, 2);        //requests high byte
  //while(Wire.available() < 2); // Wait for data to arrive
  // Wait for data to arrive and timeout at 1s
  // WAIT(2);
  
  highByte = Wire.read();
  lowByte = Wire.read();
  return (((highByte<<8)+lowByte)/10-compoffset+360)%360;
  //int bearing=(((highByte<<8)+lowByte)/10-compoffset+360)%360;
}

float orbitnorth() {
  if(bearing<=180 && bearing>compTolerance){
    return bearing/50.0;
    //return (bearing*100)/280;
  } else if(bearing>180 && bearing<360-compTolerance){
    return (bearing-360)/50.0;
    //return ((180-bearing)*100)/280;
  }
  return 0;
}

