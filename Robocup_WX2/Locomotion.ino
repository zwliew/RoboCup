#define IRlowspd 50
#define IRhighspd 100
#define IRbackspd 40

#define IRlow 30
#define IRhigh 110

#define FRONTLIM 200
#define LEFTLIM 25
#define BACKLIM 25
#define BACKGOAL 15
#define RIGHTLIM 25

/*
boolean hasBall() {
   return int(analogRead(LDR))<ldrthres;
}
*/

void trackball() {
 // int tempspd=0, tempdir=0;
  //int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR();
  //Serial.println(String(rangeL)+"\t"+String(rangeB)+"\t"+String(rangeR));
  
 // if (bearing<=180 && bearing>compTolerance) northing = pow((bearing/20),2)+12;
 // else if (bearing>180 && bearing<360-compTolerance) northing = -(pow(((bearing-360)/20),2)+12);
 // else northing = 0;
  /*
  if (rangeL > LEFTLIM*1.2 && rangeR > RIGHTLIM*1.2) { // Within goal
    BACKLIM = BACKGOAL;
  }
  */
  if (IRstr < IRhigh) { // Ball not detected
  
  }
  else { // Ball close
    
  }
  /*
  if(rangeR<RIGHTLIM||rangeL<LEFTLIM||rangeB<BACKLIM||rangeB>FRONTLIM){
    tempspd=IRlowspd;
    if(rangeR<RIGHTLIM){
      tempdir=270;
    }
    if(rangeL<LEFTLIM){
      tempdir=90;
    }
     if(rangeB<BACKLIM){
      tempdir=0;
    }
    if(rangeB>FRONTLIM){
      tempdir=180;
      }
  }else{
   tempspd=0;
   tempspd=0; 
  }
*/
  /*  
  // Ultrasonic Limitation
    if (rangeR<=RIGHTLIM) {
      if (tempdir>0 && tempdir<=90) tempdir=345;
      else if (tempdir>90 && tempdir<180) tempdir=195;
      //Serial.println(tempdir);
    } else if (rangeR>=LEFTLIM) {
      if (tempdir<360 && tempdir<270) tempdir=15;
      else if (tempdir<270 && tempdir>180) tempdir=165;
      //Serial.println(tempdir);
    }
    /*
    if (rangeB=>FRONTLIM) {
      tempdir=-1;
    }// else if (rangeB<=BACKLIM) tempdir=-1;
    
    if (tempdir!=-1) setMove(tempspd,tempdir);
    else setMove(0,0);
    
    } else {
      if (rangeR<=RIGHTGOALLIM) setMove(IRhighspeed*1.2, 270);
      else if (rangeR>=LEFTGOALLIM) setMove(IRhighspeed*1.2, 180);
      else setMove(IRhighspeed*1.2,180);
      //setMove(0,0);
    }
  }
  */
  //setMove(tempspd,tempdir);
//setMove(10,0);
/*if(millis()>4000)   
  setMove(0,0);
  else if(millis()>3000)
  setMove(50,270);
  else if(millis()>2000)
  setMove(50,180);
  else if(millis()>1000)
  setMove(50,90);
else
setMove(50,0);*/
  //Serial.print(tempspd);Serial.print("\t");Serial.println(tempdir);
}

