void setmove(int spd, float dir){
  
  int motorspeedcompensation = (millis()-starttime)/1000/60*2;
  spd = spd+motorspeedcompensation;

  if (dir>359){
    dir=dir-360;
  }

  //Motor Value Calculation Variables
  int quarter=0;
  float a=0, b=0;
  float constantsin = 0.7660444;
  float constantcos = 0.6427876;

  //Spin Compensation Variables
  int angledev = 0;
  int spindir = 0;
  float compensation = 0;
  float compensationl = 0;
  float compensationr = 0;

  //Adjust Direction
  /*Split 360 degrees into 4 quadrants. In the 4 quadrants, if the basic angle is the same, the motor values are the same, just that the motor direction is different.*/

  if (dir>180&&LeftOut==true){
    dir = 360-dir;
    spd = 3*spd/4;
  }
  else if (dir<180&&RightOut==true&&dir!=0){
    dir = 360-dir;
    spd = 3*spd/4;
  }
  if ((dir<90||dir>270)&&FrontOut==true){
    spd = 3*spd/4;
    if (dir<90){
      dir = 180-dir;
    }
    if (dir>270){
      dir = 540-dir;
    }
  }
  else if (dir>90&&dir<270&&BackOut==true){
    spd = 3*spd/4;
    if (dir>180){
      dir = 540-dir;
    }
    else if (dir<=180){
      dir = 180-dir;
    }
  }

 
  if (dir<90){
    quarter=1;
    dir = 90-dir;
  }
  else if (dir<180){
    quarter=2;
    dir=dir-90;
  }
  else if (dir<270){
    quarter=3;
    dir=270-dir;
  }
  else{
    quarter=4;
    dir=dir-270;
  }
  dir=dir/180*3.1415926; /*Convert direction to radians for trigonometry*/
  previousspd = spd;

  //Calculating expected motor values
  /*By resolving vectors, a equation for motor values can be derived.*/
  
  a = (sin(dir)*spd+motorconstant*cos(dir)*spd)/(4*constantsin);
  b = (sin(dir)*spd-2*constantsin*a)/(2*constantsin);

  //Assigning motor values to motors
  /*Motor values assigned depends on motor quadrant*/
  
  if (quarter==1){
    FLmotor=a;
    FRmotor=b;
    BLmotor=b;
    BRmotor=a;
  }
  else if (quarter==2){
    FLmotor=-b;
    FRmotor=-a;
    BLmotor=-a;
    BRmotor=-b;
  }
  else if (quarter==3){
    FLmotor=-a;
    FRmotor=-b;
    BLmotor=-b;
    BRmotor=-a;
  }
  else if (quarter==4){
    FLmotor=b;
    FRmotor=a;
    BLmotor=a;
    BRmotor=b;
  }
  
  //Compensating for spin of robot
  /*If robot spins to the left, left motors will run faster than right motors to induce a clockwise rotation. If robot spins to the right, right
    motors will run faster than left motors to induce an anti-clockwise roation. Compensation ratio will correspond to angle of deviation.*/

  if (cmpsangle>180&&cmpsangle<(360-cmpsrange)){
    angledev = 360-cmpsrange-cmpsangle;  //Identify angle of deviation
    spindir = 2;  
  }
  else if (cmpsangle<=180&&cmpsangle>cmpsrange){
    angledev = cmpsangle-cmpsrange;
    spindir = 1;
  }

  compensation = compensationmult*(0.00004*angledev*angledev+0.000628*angledev);
  //compensation = angledev*compensationmult/(180-cmpsrange);  //Adjust compensationmult to change degree of compensation
  
  if (compensation>100){
    compensation=100;
  }

  if (spindir==1){
    compensationr = (100+compensation)/100;
    compensationl = (100-compensation)/100;
  }
  else if (spindir==2){
    compensationl = (100+compensation)/100;
    compensationr = (100-compensation)/100;
  }
  else{
    compensationl = 1;
    compensationr = 1;
  }

  if (FLmotor>0){
    FLmotor = compensationl*FLmotor;
  }
  else {
    FLmotor = compensationr*FLmotor;
  }
  if (FRmotor>0){
    FRmotor = compensationr*FRmotor;
  }
  else {
    FRmotor = compensationl*FRmotor;
  }
  if (BLmotor>0){
    BLmotor = compensationl*BLmotor;
  }
  else {
    BLmotor = compensationr*BLmotor;
  }
  if (BRmotor>0){
    BRmotor = compensationr*BRmotor;
  }
  else {
    BRmotor = compensationl*BRmotor;
  }
  
  FLmotor = compensationl*FLmotor;
  BLmotor = compensationl*BLmotor;
  FRmotor = compensationr*FRmotor;
  BRmotor = compensationr*BRmotor;

  /*if (spd==0){
    if (spindir==1){
      FRmotor = compensation;
      BRmotor = compensation;
      FLmotor = -compensation;
      BLmotor = -compensation;
    }
    else if (spindir==2){
      FLmotor = compensation;
      BLmotor = compensation;
      FRmotor = -compensation;
      BRmotor = -compensation;
    }
  }*/


  /*Serial.print(compensation);
  Serial.print("  ");*/
  /*Serial.print(FLmotor);
  Serial.print("   ");
  Serial.print(FRmotor);
  Serial.print("   ");
  Serial.print(BLmotor);
  Serial.print("   ");
  Serial.print(BRmotor);
  Serial.print("   ");*/
  
  //Translating motor values to signals
  /*Analog pins controls speed. Digital pins control direction.*/
  
  analogWrite(FL_spd,abs(FLmotor));digitalWrite(FL_dir,FLmotor<0?HIGH:LOW); 
  analogWrite(FR_spd,abs(FRmotor));digitalWrite(FR_dir,FRmotor<0?HIGH:LOW); 
  analogWrite(BL_spd,abs(BLmotor));digitalWrite(BL_dir,BLmotor<0?HIGH:LOW); 
  analogWrite(BR_spd,abs(BRmotor));digitalWrite(BR_dir,BRmotor<0?HIGH:LOW); 
}

