void CheckOut2(){
  int y2number = 0;
  int xrightnumber = 0;
  int xleftnumber = 0;
  bool xactive = false;
  bool y1active = false; 
  bool y2active = false; 
  bool y3active = false;
  
  for (int i=0; i<13; i++){
    if (light[i]>lightthresh[i]){
      if (lightonce[i]==1){
        lightactive[i]=1;
        //Serial.print(i);
        //Serial.print("  ");
      }
      else {
        lightonce[i]=1;
      }
      
    }
    else {
      lightactive[i]=0;
      lightonce[i]=0;
    }
  }

  
  if ((lightactive[0]==1||lightactive[11]==1)&&BackOut==false){
    if (keepinspd==0){
       keepinspd = 1.2*previousspd;
    }
    FrontOut=true;
    outtime[0] = millis();
    //lcd.noBacklight();
    //Serial.println("FrontOut");
  }
  if (lightactive[2]==1&&LeftOut==false){
    if (keepinspd==0){
       keepinspd = 1.2*previousspd;
    }
    RightOut=true;
    outtime[1] = millis();
    //lcd.noBacklight();
    //Serial.println("RightOut");
  }  
  if (lightactive[5]==1&&FrontOut==false){
    if (keepinspd==0){
       keepinspd = 1.2*previousspd;
    }
    BackOut=true;
    outtime[2] = millis();
    //lcd.noBacklight();
    //Serial.println("BackOut");
  }  
  /*if (lightactive[8]==1&&RightOut==false){
    if (keepinspd==0){
       keepinspd = 1.2*previousspd;
    }
    LeftOut=true;
    outtime[3] = millis();
    lcd.noBacklight();
    Serial.println("LeftOut");
  }  */
}
