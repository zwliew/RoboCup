void CheckOut(){
  int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR(), rangeF=getRangeF();
  int y2number = 0;
  int xrightnumber = 0;
  int xleftnumber = 0;
  bool xactive = false;
  bool y1active = false; 
  bool y2active = false; 
  bool y3active = false;

  /*Serial.print(rangeL);
  Serial.print("  ");
  Serial.print(rangeR);
  Serial.print("  ");
  Serial.print(rangeB);
  Serial.print("  ");
  Serial.print(rangeF);
  Serial.println("  ");*/

  /*if (rangeR>prevrangeR){
    if ((rangeR-prevrangeR)>100){
      RTooNear=true;
    }
  }

  if (rangeL>prevrangeL){
    if ((rangeL-prevrangeL)>100){
      LTooNear=true;
    }
  }

  if (rangeF>prevrangeF){
    if ((rangeF-prevrangeF)>100){
      FTooNear=true;
    }
  }

  if (rangeB>prevrangeB){
    if ((rangeB-prevrangeB)>100){
      BTooNear=true;
    }
  }*/

  prevrangeB=rangeB;
  prevrangeF=rangeF;
  prevrangeL=rangeL;
  prevrangeR=rangeR;

  if (rangeB<90){
    BTooNear=false;
  }
  if (rangeF<90){
    FTooNear=false;
  }
  if (rangeR<90){
    RTooNear=false;
  }
  if (rangeF<90){
    FTooNear=false;
  }
  
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

  if (lightactive[5]==1){
    y2number++;
  }
  if (lightactive[6]==1){
    y2number++;
  }
  if (lightactive[7]==1){
    y2number++;
  }

  if (lightactive[8]==1){
    xleftnumber++;
  }
  if (lightactive[9]==1){
    xleftnumber++;
  }
  if (lightactive[10]==1){
    xleftnumber++;
  }

  if (lightactive[2]==1){
    xrightnumber++;
  }
  if (lightactive[3]==1){
    xrightnumber++;
  }
  if (lightactive[4]==1){
    xrightnumber++;
  }


  if ((xrightnumber>1&&xleftnumber>0)||(xrightnumber>0&&xleftnumber>1)){
    xactive=true;
    if (FrontOut==false&&BackOut==false){
      FrontOut=true;
      BackOut=true;
      FUSThresh = 40;
      BUSThresh = 38;
    }
    //Serial.println("x");
  }
  else {
    xactive=false;
  }
  
  if (lightactive[11]==1&&lightactive[12]==1){
    y1active=true;
    //Serial.println("y1");
  }
  else {
    y1active=false;
  }

  if (y2number>2){
    y2active=true;
    if (RightOut==false&&LeftOut==false){
      RightOut=true;
      LeftOut=true;
      LUSThresh = 36;
      RUSThresh = 36;
    }
    //Serial.println("y2");
  }
  else {
    y2active=false;
  }
  
  if (lightactive[0]==1&&lightactive[1]==1){
    y3active=true;
    //Serial.println("y3");
  }
  else {
    y3active=false;
  }
  
  if (((lightactive[0]==1&&y3active==false)||(lightactive[1]==1&&y3active==false)||(lightactive[11]==1&&y1active==false)||(lightactive[12]==1&&y1active==false))&&y2active==false&&BackOut==false){
    FUSThresh = rangeF+5;
    if (FUSThresh<48){
      FrontOut=true;
    }
  }

  if ((lightactive[2]==1||lightactive[3]==1||lightactive[4]==1)&&xactive==false&&LeftOut==false){
    RUSThresh = rangeR+5;
    if (RUSThresh<40){
      RightOut=true;
    }
  }

  if ((lightactive[7]==1||lightactive[6]==1||lightactive[5]==1)&&y2active==false&&FrontOut==false){
    BUSThresh = rangeB+5;
    if (BUSThresh<40){
      BackOut=true;
    }
  }

  if ((lightactive[8]==1||lightactive[9]==1||lightactive[10]==1)&&xactive==false&&RightOut==false){
    LUSThresh = rangeL+5;
    if (LUSThresh<40){
      LeftOut=true;
    }
  }

  if (rangeL>LUSThresh&&LeftOut==true&&LTooNear==false){
    LeftOut=false;
    //lcd.backlight();
  }
  
  if (rangeR>RUSThresh&&RightOut==true&&RTooNear==false){
    RightOut=false;
    //lcd.backlight();
  }
  
  if (rangeB>BUSThresh&&BackOut==true&&BTooNear==false){
    BackOut=false;
    //lcd.backlight();
  }
  
  if (rangeF>FUSThresh&&FrontOut==true&&FTooNear==false){
    FrontOut=false;
    //lcd.backlight();
  }

  /*if (RightOut==true){
    Serial.print("RightOut");
    lcd.noBacklight();
    Serial.print("  ");
  }
  if (LeftOut==true){
    Serial.print("LeftOut");
    lcd.noBacklight();
    Serial.print("  ");
  }
  if (BackOut==true){
    Serial.print("BackOut");
    lcd.noBacklight();
    Serial.print("  ");
  }
  if (FrontOut==true){
    Serial.print("FrontOut");
    lcd.noBacklight();
    Serial.print("  ");
  } */

  
}

void CheckBall(){
  if (light[13]<ballinlightvalue){
    ballin = true;
    ballincounter = 10;
  }
  else {
    ballin = false;
  }
}

void initLight(){
  for (int i=0; i<13; i++){
    if (light[i]!=0){
      lightthresh[i]=light[i]+lightrange[i];      
    }
    else {
      lightthresh[i]=200;
    }
  }
}


