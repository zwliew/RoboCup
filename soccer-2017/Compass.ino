void initcomp() {
  readcomp();
  cmpsoffset = cmpsangle;
  cmpsoffset = cmpsoffset;
}

void readcomp() {
  
  Wire.beginTransmission(CMPSADDR);           //starts communication with CMPS10
  Wire.write(2);                              //Sends the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(CMPSADDR, 4);              // Request 4 bytes from CMPS10
  while(Wire.available() < 4);               // Wait for bytes to become available
  highByte = Wire.read();           
  lowByte = Wire.read();            
  pitch = Wire.read();              
  roll = Wire.read();               

  cmpsangle = ((highByte<<8)+lowByte)/10;      // Calculate full cmpsangle
  fine = ((highByte<<8)+lowByte)%10;         // Calculate decimal place of cmpsangle

  if (cmpsangle<cmpsoffset){
    cmpsangle = cmpsangle+360;
    cmpsangle = cmpsangle - cmpsoffset;
  }
  else {
    cmpsangle = cmpsangle - cmpsoffset;
  }
  
  cmpsangleradian = 3.141592654/180*cmpsangle;

}
