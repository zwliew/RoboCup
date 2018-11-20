///program for bot1

#include <Wire.h>
//#include <NewPing.h>
#include <math.h>
int pl=1000,pr=1000,pb=1000,pf=1000;
#define IRlow 30
#define IRhigh 110
// Compass Address
#define CMPADDR 0x60
#define compTolerance 0
int compoffset=0, prevcompoffset=0, bearing=0;
float northing=0;

// Slave i2c port
#define MicroADDR 121
#define FAST 150
#define SLOW 100
// IR Addresses
#define IRADDR_F 0x02
#define IRADDR_B 0x01
int IRdir=0, IRstr=0, IRFB=0;

// Ultrasound
#define SONAR_NUM 3
//int rangeUS[SONAR_NUM];

// LDR Pin
int ldrthres=60;

// Movement Pins
//Robot 1
#define PINF_BL 45
#define PINB_FL 47
#define PINF_FR 51
#define PINB_BR 53

#define abs(x) ((x)>0?(x):-(x))

#define PWM_BL 9
#define PWM_FL 8
#define PWM_FR 7
#define PWM_BR 6

#define LB A0
#define LL A1
#define LR A3
#define LF A2

#define compensationmult 100
#define FLBRfast 1
int SmotorFL=0, SmotorFR=0, SmotorBL=0, SmotorBR=0;
bool inf=true, inb=true,inr=true,inl=true;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(PINB_FL,OUTPUT);;pinMode(PINF_BL,OUTPUT);
  pinMode(PINF_FR,OUTPUT);pinMode(PINB_BR,OUTPUT);
   
  delay(500);
  Serial.println("Initialising sensors");
  Serial.println("1.Compass"); initComp();
  Serial.println("2. Infraredlol"); initIR();
  //Serial.println("3. Ultrasonic"); initUS();
  Serial.println("Initialisation Complete!");
//  SmotorFR=90;
  //analogWrite(PWM_FL,abs(SmotorFL));digitalWrite(PIN_FL,SmotorFL>0?HIGH:LOW);
//  analogWrite(PWM_FR,abs(SmotorFR));digitalWrite(PIN_FR,SmotorFR>0?HIGH:LOW);
 // analogWrite(PWM_BL,abs(SmotorBL));digitalWrite(PIN_BL,SmotorBL>0?HIGH:LOW);
 // analogWrite(PWM_BR,abs(SmotorBR));digitalWrite(PIN_BR,SmotorBR>0?HIGH:LOW);
  
}

void setMove(int spd,float dir){
  int NSEW = 0;
  float SmotorFL=0, SmotorFR=0, SmotorBL=0, SmotorBR=0;
  float expectedspd = 0;
  float compensation = 0;
  float compensationm = 0;
  float compensationl = 0;
  float speedincreaseratio = 0;
  float a,b;
  b=sin((45-dir)*3.1415926/180);
  a=sin((45+dir)*3.1415926/180);
  float term1 = 49.536, term2 = 112.24, e =  2.718;
  if (abs(a)>abs(b)){
    speedincreaseratio = spd/a;
    a = spd;
   /* Serial.print(speedincreaseratio);
    Serial.print(a);*/
    b = b*speedincreaseratio;
  }
  else {
    speedincreaseratio = spd/b;
    b = spd;
    /*Serial.print(speedincreaseratio);
    Serial.print(a);*/
    a = a*speedincreaseratio;
  }
  if(dir<=90||dir>270){
  SmotorFL=-a;
  SmotorBR=a;
  SmotorFR=b;
  SmotorBL=-b;
  }else{
  SmotorFL=a;
  SmotorBR=-a;
  SmotorFR=-b;
  SmotorBL=b;    
  }
  if (bearing>compTolerance&&bearing<180){
   // anticlockwisecounter++;
    compensation = ((bearing-compTolerance)*compensationmult)/(180-compTolerance);
/*    if (compensation>20){
      compensation = 20;
    }*/
    /*lcd.print("     ");*/
   // lcd.print(compensation);
    compensationl = (100+compensation)/100;
    compensationm = (100-compensation)/100;
    if (SmotorFL<0) {
    SmotorFL = compensationm*SmotorFL;
    }
    else {
     SmotorFL = compensationl*SmotorFL; 
    }
    if (SmotorFR>0) {
    SmotorFR = compensationl*SmotorFR;
    }
    else {
     SmotorFR = compensationm*SmotorFR; 
    }
    if (SmotorBL<0) {
    SmotorBL = compensationm*SmotorBL;
    }
    else {
     SmotorBL = compensationl*SmotorBL; 
    }
    if (SmotorBR>0) {
    SmotorBR = compensationl*SmotorBR;
    }
    else {
    SmotorBR = compensationm*SmotorBR; 
    }
    
  }
  if (bearing<(360-compTolerance)&&bearing>=180){
   // clockwisecompensation = 100;
    compensation = ((360-bearing-compTolerance)*compensationmult)/(180-compTolerance);
   /* if (compensation>20){
      compensation = 20;
    }*/
    /*lcd.print("     ");
    lcd.print(compensation);*/
  //  lcd.print(compensation);
    compensationl = (100-compensation)/100;
    compensationm = (100+compensation)/100;
    if (SmotorFL<0) {
    SmotorFL = compensationm*SmotorFL;
    }
    else {
     SmotorFL = compensationl*SmotorFL; 
    }
    if (SmotorFR>0) {
    SmotorFR = compensationl*SmotorFR;
    }
    else {
     SmotorFR = compensationm*SmotorFR; 
    }
    if (SmotorBL<0) {
    SmotorBL = compensationm*SmotorBL;
    }
    else {
     SmotorBL = compensationl*SmotorBL; 
    }
    if (SmotorBR>0) {
    SmotorBR = compensationl*SmotorBR;
    }
    else {
    SmotorBR = compensationm*SmotorBR; 
    }
  }
  
  /*Serial.print(SmotorFLO);
  Serial.print("   ");
  Serial.print(SmotorFRO);
  Serial.print("   ");
  Serial.print(SmotorBLO);
  Serial.print("   ");
  Serial.print(SmotorBRO);
  Serial.print("   ");*/
  /*
  if (counterclockwisecompensation>0&&(cmpsangle>=(360-cmpsrange)||cmpsangle<=cmpsrange)){
    counterclockwisecompensation = counterclockwisecompensation-1;
    compensation = 40;
    lcd.print("hello");
    if (SmotorFLO>0) {
    SmotorFLO = compensationm*SmotorFLO;
    }
    else {
     SmotorFLO = compensationl*SmotorFLO; 
    }
    if (SmotorFRO>0) {
    SmotorFRO = compensationl*SmotorFRO;
    }
    else {
     SmotorFRO = compensationm*SmotorFRO; 
    }
    if (SmotorBLO>0) {
    SmotorBLO = compensationm*SmotorBLO;
    }
    else {
     SmotorBLO = compensationl*SmotorBLO; 
    }
    if (SmotorBRO>0) {
    SmotorBRO = compensationl*SmotorBRO;
    }
    else {
    SmotorBRO = compensationm*SmotorBRO; 
    } 
  }

  if (clockwisecompensation>0&&(cmpsangle<=cmpsrange||cmpsangle>=(360-cmpsrange))){
    clockwisecompensation = clockwisecompensation-1;
    compensation = 40;
    if (SmotorFLO>0) {
    SmotorFLO = compensationl*SmotorFLO;
    }
    else {
     SmotorFLO = compensationm*SmotorFLO; 
    }
    if (SmotorFRO>0) {
    SmotorFRO = compensationm*SmotorFRO;
    }
    else {
     SmotorFRO = compensationl*SmotorFRO; 
    }
    if (SmotorBLO>0) {
    SmotorBLO = compensationl*SmotorBLO;
    }
    else {
     SmotorBLO = compensationm*SmotorBLO; 
    }
    if (SmotorBRO>0) {
    SmotorBRO = compensationm*SmotorBRO;
    }
    else {
    SmotorBRO = compensationl*SmotorBRO; 
    }
    lcd.print("hello");
    
  }
  */
 // SmotorFR*=1.25;
 // SmotorBL*=1.25;
  int fl=SmotorFL,fr=SmotorFR,bl=SmotorBL,br=SmotorBR;
  if(SmotorFL!=0) fl = pow(e,(abs(fl)+term2)/term1);
  if(SmotorFR!=0)  fr = pow(e,(abs(fr)+term2)/term1);
  if(SmotorBL!=0) bl = pow(e,(abs(bl)+term2)/term1);
  if(SmotorBR!=0) br = pow(e,(abs(br)+term2)/term1);
  if(SmotorFL<0)SmotorFL=-fl;
  else SmotorFL=fl;
  if(SmotorFR<0)SmotorFR=-fr;
  else SmotorFR=fr;
  if(SmotorBR<0)SmotorBR=-br;
  else SmotorBR=br;
  if(SmotorBL<0)SmotorBL=-bl;
  else SmotorBL=bl;/*
  Serial.print(dir);
  Serial.print("\t");
  Serial.print(bearing);
  Serial.print("\t");
  Serial.print(SmotorFL);
  Serial.print("\t");
  Serial.print(SmotorFR);
  Serial.print("\t");
  Serial.print(SmotorBL);
  Serial.print("\t");
  Serial.println(SmotorBR);*/
  analogWrite(PWM_FL,abs(SmotorFL));
  if(SmotorFL>0){
    digitalWrite(PINB_FL,LOW);
  }
  else{
    digitalWrite(PINB_FL,HIGH);
  }
  analogWrite(PWM_FR,abs(SmotorFR));
  if(SmotorFR>0){
    digitalWrite(PINF_FR,HIGH);
  }
  else{
    digitalWrite(PINF_FR,LOW);
  }
  analogWrite(PWM_BL,abs(SmotorBL));
  if(SmotorBL>0){
    digitalWrite(PINF_BL,LOW);
  }
  else{
    digitalWrite(PINF_BL,HIGH);
  }
  analogWrite(PWM_BR,abs(SmotorBR));
  if(SmotorBR>0){
    digitalWrite(PINB_BR,HIGH);
  }
  else{
    digitalWrite(PINB_BR,LOW);
  }
}
void strike(){
  int mov=0,d=0;
  if(IRstr<IRlow){
    mov=0;
    d=0;
  }else if(IRstr<IRhigh){
    if(IRdir==1){
      mov=FAST;
      d=90;
    }else if(IRdir==2){
      mov=FAST;
      d=70;
    }else if(IRdir==3){
      mov=FAST;
      d=50;
    }else if(IRdir==4){
      mov=FAST;
      d=0;
    }else if(IRdir==5){
      mov=FAST;
      d=310;
    }else if(IRdir==6){
      mov=FAST;
      d=290;
    }else if(IRdir==7){
      mov=FAST;
      d=270;
    }else if(IRdir==8){
      mov=FAST;
      d=250;
    }else if(IRdir==9){
      mov=FAST;
      d=210;
    }else if(IRdir==10){
      mov=FAST;
      d=190;
    }else if(IRdir==11){
      mov=FAST;
      d=200;
    }else if(IRdir==12){
      mov=FAST;
      d=170;
    }else if(IRdir==13){
      mov=FAST;
      d=150;
    }else{
      mov=FAST;
      d=110;
    }
    
  }else{
    if(IRdir==1){
      mov=SLOW;
      d=110;
    }else if(IRdir==2){
      mov=SLOW;
      d=100;
    }else if(IRdir==3){
      mov=SLOW;
      d=90;
    }else if(IRdir==4){
      mov=SLOW;
      d=0;
    }else if(IRdir==5){
      mov=SLOW;
      d=270;
    }else if(IRdir==6){
      mov=SLOW;
      d=260;
    }else if(IRdir==7){
      mov=SLOW;
      d=250;
    }else if(IRdir==8){
      mov=SLOW;
      d=160;
    }else if(IRdir==9){
      mov=SLOW;
      d=140;
    }else if(IRdir==10){
      mov=SLOW;
      d=120;
    }else if(IRdir==11){
      mov=SLOW;
      d=90;
    }else if(IRdir==12){
      mov=SLOW;
      d=240;
    }else if(IRdir==13){
      mov=SLOW;
      d=260;
    }else{
      mov=SLOW;
      d=280;
    }
  }
  if(analogRead(LR)>220&&pr<=220){
    inr=!inr;
  }
  if(analogRead(LL)>220&&pl<=220){
    inl=!inl;
  }
  if(analogRead(LF)>220&&pf<=220){
    inf=!inf;
  }
  if(analogRead(LB)>700&&pb<=700){
    inb=!inb;
  }
  pr=analogRead(LR);
  pl=analogRead(LL);
  pb=analogRead(LB);
  pf=analogRead(LF);
  if(!inl){
    mov=175;
    d=90;
  }
  else if(!inr){
    mov=175;
    d=270;
  }
  else if(!inf){
    mov=175;
    d=180;
  }
  else if(!inb){
    mov=175;
    d=0;
  }
    setMove(mov,d);
// Serial.print(mov);
//  Serial.print("\t");
//  Serial.println(d);

}
void loop() {
  bearing =readComp();
  readIR();/*
int a=analogRead(LR);
int b=analogRead(LL);
int c=analogRead(LF);
int d=analogRead(LB);
Serial.print(b);
Serial.print('\t');
Serial.print(a);
Serial.print('\t');
Serial.print(c);
Serial.print('\t');
Serial.print(d);
Serial.println('\t');*/
 // setMove(0,0);
 //Serial.print(bearing); Serial.println("\t");
//  readUS();
//setMove(175,0);
//setMove(175,270);
strike();
  //Serial.println(String(SmotorBR)+"\t"+String(SmotorBL)+"\t"+String(SmotorFR)+"\t"+String(SmotorFL));
  
  // GO!
  /*
  analogWrite(PWM_FL,abs(SmotorFL));digitalWrite(PIN_FL,SmotorFL>0?HIGH:LOW);
  analogWrite(PWM_FR,abs(SmotorFR));digitalWrite(PIN_FR,SmotorFR>0?HIGH:LOW);
  analogWrite(PWM_BL,abs(SmotorBL));digitalWrite(PIN_BL,SmotorBL>0?HIGH:LOW); 
  analogWrite(PWM_BR,abs(SmotorBR));digitalWrite(PIN_BR,SmotorBR>0?HIGH:LOW);*/
  
 // trackball();
  
// SmotorFL=-75;
// SmotorBL=-75;
// SmotorBR=-75;
// SmotorFR=-75;/*
/*
 analogWrite(PWM_FL,75);
  if(SmotorFL>0){
    digitalWrite(PINF_FL,HIGH);
  }
  else{
    digitalWrite(PINF_FL,LOW);
  }


  
  analogWrite(PWM_FR,75);
  if(SmotorFR>0){
    digitalWrite(PINB_FR,LOW);
  }
  else{
    digitalWrite(PINB_FR,HIGH);
  }
  analogWrite(PWM_BL,75);
  if(SmotorBL>0){
    digitalWrite(PINF_BL,HIGH);
  }
  else{
    digitalWrite(PINF_BL,LOW);
  }
  analogWrite(PWM_BR,75);
  if(SmotorBR>0){
    digitalWrite(PINF_BR,HIGH);
    digitalWrite(PINB_BR,LOW);
  }
  else{
    digitalWrite(PINF_BR,LOW);
    digitalWrite(PINB_BR,HIGH);
  }*/
//  Serial.print(IRFB);Serial.print("\t");
//  Serial.print(IRstr);Serial.print("\t");
//  Serial.print(IRdir);Serial.println("\t");
  
//  Serial.print(bearing);Serial.print("\t");
//  Serial.print(getRangeL());Serial.print("\t");
//  Serial.print(getRangeB());Serial.print("\t");
//  Serial.print(getRangeR());Serial.println("\t");
  

}

