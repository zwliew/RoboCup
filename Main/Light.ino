#define LI_FLF A6
#define LI_FLB A13

#define LI_FRF A8
#define LI_FRB A9

#define LI_LL A3
#define LI_LM A4
#define LI_LR A5

#define LI_RR A10
#define LI_RM A11
#define LI_RL A12

#define LI_BB A0
#define LI_BM A1
#define LI_BF A2

// Array positions
#define LI_ARR_F 0
#define LI_ARR_L 4
#define LI_ARR_R 7
#define LI_ARR_B 10
#define LI_ARR_END 13

#define LI_THRES 50

int li_readings[13];

void InitLight() {
  for (int i = 0; i < 13; i++) {
    li_readings[i] = 0;
  }
  Serial.println("Initialized lights.");
}

void ReadLight() {
  li_readings[LI_ARR_F] = analogRead(LI_FLF);
  li_readings[LI_ARR_F + 1] = analogRead(LI_FLB);

  li_readings[LI_ARR_F + 2] = analogRead(LI_FRF);
  li_readings[LI_ARR_F + 3] = analogRead(LI_FRB);

  li_readings[LI_ARR_L] = analogRead(LI_LL);
  li_readings[LI_ARR_L + 1] = analogRead(LI_LM);
  li_readings[LI_ARR_L + 2] = analogRead(LI_LR);

  li_readings[LI_ARR_R] = analogRead(LI_RR);
  li_readings[LI_ARR_R + 1] = analogRead(LI_RM);
  li_readings[LI_ARR_R + 2] = analogRead(LI_RL);

  li_readings[LI_ARR_B] = analogRead(LI_BB);
  li_readings[LI_ARR_B + 1] = analogRead(LI_BM);
  li_readings[LI_ARR_B + 2] = analogRead(LI_BF);

#ifdef DEBUG_LIGHT
  Serial.println("FLF FLB FRF FRB  LL  LM  LR  RR  RM  RL  BB  BM  BF");
  for (int i = 0; i < sizeof(li_readings) / sizeof(*li_readings); i++) {
    if (li_readings[i] >= 100) {
    } else if (li_readings[i] >= 10) {
      Serial.print(" ");
    } else {
      Serial.print("  ");
    }
    Serial.print(li_readings[i]);
    Serial.print(" ");
  }
  Serial.println();
#endif
}

bool IsSideOut(int side_start, int side_end) {
  for (int i = side_start; i < side_end; i++) {
    if (li_readings[i] > LI_THRES) {
      return true;
    }
  }
  return false;
}

bool IsFrontOut() {
  return IsSideOut(LI_ARR_F, LI_ARR_L);
}

bool IsLeftOut() {
  return IsSideOut(LI_ARR_L, LI_ARR_R);
}

bool IsRightOut() {
  return IsSideOut(LI_ARR_R, LI_ARR_B);
}

bool IsBackOut() {
  return IsSideOut(LI_ARR_B, LI_ARR_END);
}

bool IsAnyOut() {
  return IsFrontOut() ||
      IsLeftOut() ||
      IsRightOut() ||
      IsBackOut();
}
