void initpixy(){

  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];
  delay(1000);
  //while (blocks==0){
  blocks = pixy.getBlocks();
  //} 

  for (j=0; j<blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
  }
  
  // If there are detect blocks, print them!
  if (blocks){

      for (j=0; j<blocks; j++)
      {
        //Serial.print(pixy.blocks[j].signature);
        //Serial.print("    ");

        if (largestblock<0&&(pixy.blocks[j].signature==1||pixy.blocks[j].signature==2)){
          largestblock = j;
        }
        else{
        if (pixy.blocks[j].width>pixy.blocks[largestblock].width&&(pixy.blocks[j].signature==1||pixy.blocks[j].signature==2)){
          largestblock = j;
        }
        }
 //     goalheight = pixy.blocks[largestblock].y;
      }
  }
  Serial.println(largestblock);
  goal = pixy.blocks[largestblock].signature;
  Serial.println(goal);
  if (largestblock>=0){
    pixyinitiated = true;
  } 
}

void readpixy(){
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];
  int largestblock = -1; 
  
  // grab blocks!
  blocks = pixy.getBlocks();
  
  // If there are detect blocks, print them!
  if (blocks){

      for (j=0; j<blocks; j++)
      {
        //Serial.print(pixy.blocks[j].signature);
        //Serial.print("    ");
        if (pixy.blocks[j].signature==goal){

        if (largestblock<0){
          largestblock = j;
        }
        else{
        if (pixy.blocks[j].width>pixy.blocks[largestblock].width){
          largestblock = j;
        }
        }
      }
 //     goalheight = pixy.blocks[largestblock].y;
      }
  }

  if (largestblock>=0){
    //sprintf(buf, "  block %d: ", j);
    //Serial.print(buf); 
    //pixy.blocks[largestblock].print();
    goallocation = pixy.blocks[largestblock].x;
    goalheight = pixy.blocks[largestblock].height;
    }
    else{
      goallocation=0;
      goalheight=0;
      //Serial.println("No Blocks of the correct colour was detected");
      //goallocation = 0;
    }
}

