void score() {
  int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR(), rangeF=getRangeF();
  pixycount++;
  if (pixycount==1&&pixyinitiated==true){
    readpixy();
  }
  else if (pixycount==5){
    pixycount=0;
  }
  float goalangle;

  if (goalheight>40&&140<goallocation<160){
    goalnear=true;
  }
  else {
    goalnear=false;
  }

  if (goalnear==true&&recentshot==false){
    //Serial.print("kick");
    //Serial.print("  ");
    digitalWrite(SolPin,HIGH);
    delay(50);
    digitalWrite(SolPin,LOW);
    recentshot=true;
    recentshotcounter=0;
  }
  
  if (goallocation==0){
    if (rangeL>60&&rangeR>60){
      digitalWrite(SolPin,HIGH);
      delay(50);
      digitalWrite(SolPin,LOW);
      recentshot=true;
      recentshotcounter=0;
      aimingright=false;
      aimingrightcounter=0;
      aimingleft=false;
      aimingleftcounter=0;
    }
    else if (rangeL<40){
      setmove(115,35);
    }
    else if (rangeR<40){
      setmove(115,325);
    }
    setmove(140,0);
  }
  else if (goallocation>165){
    if (goallocation<220&&aimingright==false){
      goalangle = (goallocation-165)/55*40;
      aimingrightcounter++;
    }
    else if (goallocation<220&&aimingright==true){
      if (goallocation<220&goallocation>190){
        goalangle = (goallocation-190)/55*40;
      }
      else if (goallocation<=190){
        goalangle = 345;
      }
    }
    else {
      goalangle=40;
      aimingright=true;
    }
    setmove(115,goalangle);
    if (aimingrightcounter>150){
      aimingright=true;
    }
  }
  
  else if (goallocation<135){
    if (goallocation>80&&aimingleft==false){
      goalangle = 360-(135-goallocation)/55*40;
      aimingleftcounter++;
    }
    else if (goallocation>80&&aimingleft==true){
      if (goallocation>80&&goallocation<110){
        goalangle = 360-(110-goallocation)/55*40;
      }
      else if (goallocation>=110){
        goalangle = 15;
      }
    }
    else {
      goalangle=320;
      aimingleft=true;
    }
    setmove(115,goalangle);
    if (aimingleftcounter>150){
      aimingleft=true;
    }
  }
  
  else {
    setmove(140,0);
    aimingright=false;
    aimingrightcounter=0;
    aimingleft=false;
    aimingleftcounter=0;
  }

  /*Serial.print(goal);
  Serial.print("   ");*/
  //Serial.print(goallocation);
  //Serial.print("  ");
  //Serial.print(goalheight);
  //Serial.print("  ");
  /*Serial.print(goalangle);
  Serial.print("  ");
  Serial.println(recentshot);*/
  
}

void trackball() {
  int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR(), rangeF=getRangeF();
  float rangeslow = 0;
  float motorspeed = 135;
  int motordir = 0;

  if (rangeR>rangeL){
    rangeslow=rangeL;
  }
  else {
    rangeslow=rangeR;
  }

  if (rangeslow<=40){
    motorspeed=90;
  }
  else if (rangeslow<60){
    motorspeed=90+(rangeslow-40)/20*45;
  }
  
  if (IRdir==1){
    if (IRnear==true){
      motordir = 180;
    }
    else if (IRnearish==true){
      motordir = 180+(285-180)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 285;
    }
  }
  else if (IRdir==2){
    if (IRnear==true){
      motordir = 225;
    }
    else if (IRnearish==true){
      motordir = 225+(312-225)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 312;
    }
  }
  else if (IRdir==3){
    if (IRnear==true){
      motordir = 320;
    }
    else if (IRnearish==true){
      motordir = 320+(338-320)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 338;
    }
  }
  else if (IRdir==4){
    motordir = 0;
  }
  else if (IRdir==5){
    if (IRnear==true){
      motordir = 40;
    }
    else if (IRnearish==true){
      motordir = 40-(40-27)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 27;
    }
  }
  else if (IRdir==6){
    if (IRnear==true){
      motordir = 135;
    }
    else if (IRnearish==true){
      motordir = 135-(135-60)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 60;
    }
  }
  else if (IRdir==7){
    if (IRnear==true){
      motordir = 180;
    }
    else if (IRnearish==true){
      motordir = 180-(180-80)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 80;
    }
  }
  else if (IRdir==8){
    if (IRnear==true){
      motordir = 180;
    }
    else if (IRnearish==true){
      motordir = 180-(180-100)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 100;
    }
  }
  else if (IRdir==9){
    if (IRnear==true){
      motordir = 225;
    }
    else if (IRnearish==true){
      motordir = 225-(225-125)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 125;
    }
  }
  else if (IRdir==10){
    if (IRnear==true){
      motordir = 270;
    }
    else if (IRnearish==true){
      motordir = 270-(270-157)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 157;
    }
  }
  else if (IRdir==11){
    if (IRnear==true){
      if (rangeL>rangeR){
        motordir=270;
      }
      else{
        motordir=90;
      }
    }
    else if (IRnearish==true){
      if (rangeL>rangeR){
        motordir = 270-(270-180)*(IRnearThreshval-IRstr)/IRanglechange;
        motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
      }
      else {
        motordir = 90+(180-90)*(IRnearThreshval-IRstr)/IRanglechange;
      }
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 180;
    }
  }
  else if (IRdir==12){
    if (IRnear==true){
      motordir = 90;
    }
    else if (IRnearish==true){
      motordir = 90+(202-90)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 202;
    }
  }
  else if (IRdir==13){
    if (IRnear==true){
      motordir = 135;
    }
    else if (IRnearish==true){
      motordir = 135+(237-135)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 237;
    }
  }
  else if (IRdir==14){
    if (IRnear==true){
      motordir = 180;
    }
    else if (IRnearish==true){
      motordir = 180+(260-180)*(IRnearThreshval-IRstr)/IRanglechange;
      motorspeed = motorspeed-(motorspeed-50)*(IRnearThreshval-IRstr)/IRanglechange;
    }
    else {
      motordir = 260;
    }
  }
  setmove(motorspeed,motordir);
  
}

void returnhome(){
  int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR(), rangeF=getRangeF();
  
  /*if (rangeB>(HomeBack+30)){
    setmove(70,180);
  }
  else if (rangeB<(HomeBack-10)){
    setmove(70,0);
  }
  else if (rangeL<(HomeLeft-10)){
    setmove(70,90);
  }
  else if (rangeL>(HomeLeft+10)){
    setmove(70,270);
  }
  else if (rangeB>(HomeBack+10)){
    setmove(70,180);
  }
  else {
    setmove(0,0);
  }*/ //Robot 1

  if (rangeB>(HomeBack+30)){
    setmove(70,180);
  }
  else if (rangeB<(HomeBack-10)){
    setmove(70,0);
  }
  else if (rangeR<(HomeRight-10)){
    setmove(70,270);
  }
  else if (rangeR>(HomeRight+10)){
    setmove(70,90);
  }
  else if (rangeB>(HomeBack+10)){
    setmove(70,180);
  }
  else {
    setmove(0,0);
  } //Robot 2
}

