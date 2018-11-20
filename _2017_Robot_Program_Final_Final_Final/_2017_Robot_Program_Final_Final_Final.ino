#include <Wire.h>
#include <SPI.h>  
#include <Pixy.h>
#include <LiquidCrystal_I2C.h>

Pixy pixy;
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Motor Constants
#define BL_dir 2
#define BL_spd 3
#define FL_dir 4
#define FL_spd 5
#define BR_dir 6
#define BR_spd 7
#define FR_dir 8
#define FR_spd 9

//Compass Constants
#define CMPSADDR 0x60

//IR Sensor Constants
#define IRADDR_F 0x01
#define IRADDR_B 0x02

//Ultrasonic Constants
#define SONAR_NUM 4
#define MicroADDR 121

//SpinCompensation Constants
#define cmpsrange 0

//Trackball Constants
//#define IRThresh 10 //Robot 1
#define IRThresh 15 //Robot 2
#define IRanglechange 30
//#define ballinlightvalue 40 //Robot 1
#define ballinlightvalue 15 //Robot 2

//Solenoid Constants
#define SolPin 11

//Home Constans

#define HomeBack 45
#define HomeRight 65
#define HomeLeft 65


//General Variables
int starttime = 0;
int circle = 0;

//Locomotion Variables
int FLmotor = 0, FRmotor = 0, BLmotor = 0, BRmotor = 0;
int compensation = 0;
int compensationtime = 0;
int previousdir = 0, previousspd = 0;
float motorconstant = 1.1917535;
int keepinspd = 0;
int keepindir = 0;
int outtime[4] = {0,0,0,0};
int keepincount = 0;
bool keepin = false;

//Compass Variables
int cmpsoffset = 0;
byte highByte, lowByte, fine;              // highByte and lowByte store high and low bytes of the bearing and fine stores decimal place of bearing
char pitch, roll;                          // Stores pitch and roll values of CMPS10, chars are used because they support signed value
int cmpsangle;                               // Stores full bearing
float cmpsangleradian;
int cmpscount = 0;

//IR Variables
int IRdir=0, IRstr=0;
//int IRnearThresh[14] = {80,80,80,70,65,65,70,85,100,100,110,120,110,100}; //Robot 1
int IRnearThresh[14] = {70,70,70,60,55,55,55,80,95,95,105,115,105,95}; //Robot 2
int IRnearThreshval = 0;
bool IRnear = false;
bool IRnearish = false;

//Pixy Variables
float goallocation;
int goalheight;
int goalwidth;
bool goalnear = false;
int pixycount=0;
bool pixyinitiated = false;
int pixyfailcount=0;
int largestblock = -1;
int goal;

//Light Sensor Variables
int light[14];
//int lightthresh[13] = {70,70,80,90,45,160,180,100,55,100,100,100,35};  //Robot1
//int lightthresh[13] = {100,55,30,70,120,35,150,200,75,80,100,55,95};  //Robot2
int lightthresh[13];
int lightrange[13] = {15,15,15,15,15,15,15,15,15,15,15,15,15}; //Robot 1
//int lightrange[13] = {30,25,15,30,50,15,60,15,30,32,32,25,45}; //Robot 2
int lightactive[13];
int lightonce[13];

//Compensation Variables
int compensationmult = 150;
bool aimingleft=false;
bool aimingright=false;
int aimingleftcounter=0;
int aimingrightcounter=0;

int counters = 0;

//Trackball Variables
bool ballin = false;
bool RightOut = false;
bool LeftOut = false;
bool FrontOut = false;
bool BackOut = false;
bool recentshot = false;
int recentshotcounter=0;
int ballincounter = 0;
bool frontout = false;

//Ultrasonic Variables
int FUSThresh = 37;
int LUSThresh = 35;
int BUSThresh = 35;
int RUSThresh = 35;
bool  FTooNear = false;
bool  LTooNear = false;
bool  RTooNear = false;
bool  BTooNear = false;
int prevrangeF = 0;
int prevrangeL = 0;
int prevrangeR = 0;
int prevrangeB = 0;
  
void setup() {
 Serial.begin(9600);
 Wire.begin();
 pixy.init();
 while (pixyinitiated==false&&pixyfailcount<=7){
  initpixy();
  pixyfailcount++;
 }
 delay(1000);
 pinMode(BL_dir,OUTPUT);
 pinMode(FL_dir,OUTPUT);
 pinMode(FR_dir,OUTPUT);
 pinMode(BR_dir,OUTPUT);
 pinMode(SolPin,OUTPUT);
 initcomp();
 initIR();
 initNano();
 initLight();
 Serial.println("initiated pixy part 2");
 lcd.begin();
 Serial.println("initiated");
 if (goal==1){
    lcd.print("yellow");
  }
  else if (goal==2){
    lcd.print("blue");
  }
  else if (pixyinitiated==false){
    lcd.print("pixy failed");
  }
 starttime = millis(); 
}

void loop() {
  keepindir = 0;
  keepincount = 0;

  /*if ((millis()-starttime)<1000){
    counters++;
  }
  else{
    Serial.println(counters);
  }*/
    
  if (recentshot==true){
    recentshotcounter++;
  }
  
  if (recentshotcounter>250){
    recentshot=false;
  }
  if (ballincounter>0){
    ballin=true;
    ballincounter=ballincounter-1;
  }
  //cmpscount++;

  //if (cmpscount>5){
    readcomp();
  //  cmpscount=0;
  //}
  CheckBall();
  readIR();
  readNano();

  CheckOut();

  
  //Serial.print(cmpsangle);
  //Serial.print("  ");
  //Serial.print(IRstr);
  //Serial.print(" ");
  //Serial.print(IRdir);
  //Serial.print(" ");
  Serial.print(light[13]);
  Serial.print("  ");
  //Serial.print(ballin);
  //Serial.print(IRnear);
  //Serial.print("  ");
  //Serial.print(cmpsangle);
  //Serial.print("  ");
  //Serial.print(getRangeF());
  //Serial.print(goallocation);
  //Serial.println("  ");
  //Serial.print(goalheight);
  //Serial.print("  ");

  /*if (ultrasonicsfail==true&&(RightOut==true||LeftOut==true||FrontOut==true||BackOut==true)){
    if (BackOut==true){
      keepindir = keepindir+360;
      keepincount++;
    }
    if (RightOut==true){
      keepindir = keepindir+270;
      keepincount++;
    }
    if (LeftOut==true){
      keepindir = keepindir+90;
      keepincount++;
    }
    if (FrontOut==true){
      keepindir = keepindir+180;
      keepincount++;
    }
    keepindir = keepindir/keepincount;
    compensationmult=300;
    setmove(keepinspd,keepindir);
    Serial.println(keepindir);
  }
  else {
    setmove(120,0);
  }*/
  if (ballin==true&&(IRdir==3||IRdir==4||IRdir==5)&&IRstr>IRThresh){
    score();
  }
  else if (IRstr>IRThresh){
    trackball();
  }
  else {
    returnhome();
  }
  Serial.println(ballin);
  /*if (IRstr>IRThresh){
    trackball();
  }
  else {
    setmove(0,0);
  }*/
  //returnhome();
  //score();
  /*if (FrontOut==false&&BackOut==false&&LeftOut==false&&RightOut==false){
    keepinspd=0;
    lcd.backlight();
    keepin=false;
    compensationmult=200;
  }*/

  /* CHECK 
  if (RightOut==true){
    frontout=true;
    setmove(0,0);
  }
  else if (frontout==false){
    setmove(150,90);
  }
  else if (frontout==true){
    setmove(0,0);
  }
  */
}
