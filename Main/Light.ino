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

#ifdef IS_STRIKER
#define LI_THRES 50
#else
#define LI_THRES 110
#endif

void DebugLight() {
  unsigned int readings[13];
  readings[0] = analogRead(LI_FLF);
  readings[1] = analogRead(LI_FRF);
  readings[2] = analogRead(LI_FLB);
  readings[3] = analogRead(LI_FRB);
  readings[4] = analogRead(LI_LL);
  readings[5] = analogRead(LI_LM);
  readings[6] = analogRead(LI_LR);
  readings[7] = analogRead(LI_RL);
  readings[8] = analogRead(LI_RM);
  readings[9] = analogRead(LI_RR);
  readings[10] = analogRead(LI_BF);
  readings[11] = analogRead(LI_BM);
  readings[12] = analogRead(LI_BB);
#ifdef DEBUG_LIGHT
  Serial.println("FLF FRF FLB FRB  LL  LM  LR  RL  RM  RR  BF  BM  BB");
  for (unsigned int i = 0; i < 13; i++) {
    if (readings[i] >= 100) {
    } else if (readings[i] >= 10) {
      Serial.print(" ");
    } else {
      Serial.print("  ");
    }
    Serial.print(readings[i]);
    Serial.print(" ");
  }
  Serial.println();
#endif
}

bool IsFrontOut() {
  static bool prev_out[4] = { false, false, false, false };

  unsigned int readings[4];
  readings[0] = analogRead(LI_FLF);
  readings[1] = analogRead(LI_FRF);
  readings[2] = analogRead(LI_FLB);
  readings[3] = analogRead(LI_FRB);

  for (unsigned int i = 0; i < 4; i++) {
    if (readings[i] >= LI_THRES && !prev_out[i]
        || readings[i] < LI_THRES && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] && prev_out[1] && prev_out[2] && prev_out[3];
}

bool IsLeftOut() {
  static bool prev_out[3] = { false, false, false }; // Left, middle, right

  unsigned int readings[3];
  readings[0] = analogRead(LI_LL);
  readings[1] = analogRead(LI_LM);
  readings[2] = analogRead(LI_LR);

  for (unsigned int i = 0; i < 3; i++) {
    if (readings[i] >= LI_THRES && !prev_out[i]
        || readings[i] < LI_THRES && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] && prev_out[1] && prev_out[2];
}

bool IsRightOut() {
  static bool prev_out[3] = { false, false, false }; // Left, middle, right

  unsigned int readings[3];
  readings[0] = analogRead(LI_RL);
  readings[1] = analogRead(LI_RM);
  readings[2] = analogRead(LI_RR);

  for (unsigned int i = 0; i < 3; i++) {
    if (readings[i] >= LI_THRES && !prev_out[i]
        || readings[i] < LI_THRES && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] && prev_out[1] && prev_out[2];
}

bool IsBackOut() {
  static bool prev_out[3] = { false, false, false };

  unsigned int readings[3];
  readings[0] = analogRead(LI_BF);
  readings[1] = analogRead(LI_BM);
  readings[2] = analogRead(LI_BB);

  for (unsigned int i = 0; i < 3; i++) {
    if (readings[i] >= LI_THRES && !prev_out[i]
        || readings[i] < LI_THRES && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] && prev_out[1] && prev_out[2];
}
