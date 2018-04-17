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
#define LI_THRES_FLF 120 // 135
#define LI_THRES_FRF 190 // 205
#define LI_THRES_FLB 250 // 265
#define LI_THRES_FRB 140 // 151
#define LI_THRES_LL 160 // 175
#define LI_THRES_LM 240 // 257
#define LI_THRES_LR 140 // 153
#define LI_THRES_RL 170 // 182
#define LI_THRES_RM 150 //165
#define LI_THRES_RR 190 // 206
#define LI_THRES_BF 400 // 413
#define LI_THRES_BM 230 // 249
#define LI_THRES_BB 340 // 359
#else
#define LI_THRES_FLF 130 // 149
#define LI_THRES_FRF 180 // 190
#define LI_THRES_FLB 190 // 205
#define LI_THRES_FRB 120 // 135
#define LI_THRES_LL 140 // 155
#define LI_THRES_LM 180 // 195
#define LI_THRES_LR 140 // 155
#define LI_THRES_RL 180 // 193
#define LI_THRES_RM 160 // 170
#define LI_THRES_RR 170 // 183
#define LI_THRES_BF 130 // 144
#define LI_THRES_BM 190 // 203
#define LI_THRES_BB 250 // 260
#endif

#define LI_FRONT_NUM 4
#define LI_LEFT_NUM 3
#define LI_RIGHT_NUM 3
#define LI_BACK_NUM 3

const static int li_thres[LI_FRONT_NUM + LI_LEFT_NUM + LI_RIGHT_NUM + LI_BACK_NUM] = {
  LI_THRES_FLF,
  LI_THRES_FRF,
  LI_THRES_FLB,
  LI_THRES_FRB,
  LI_THRES_LL,
  LI_THRES_LM,
  LI_THRES_LR,
  LI_THRES_RL,
  LI_THRES_RM,
  LI_THRES_RR,
  LI_THRES_BF,
  LI_THRES_BM,
  LI_THRES_BB
};

void DebugLight() {
  unsigned int readings[LI_FRONT_NUM + LI_LEFT_NUM + LI_RIGHT_NUM + LI_BACK_NUM];
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

  for (unsigned int i = 0; i < LI_FRONT_NUM; i++) {
    if (readings[i] >= li_thres[i] && !prev_out[i]
        || readings[i] < li_thres[i] && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] || prev_out[1] || prev_out[2] || prev_out[3];
}

bool IsLeftOut() {
  static bool prev_out[3] = { false, false, false }; // Left, middle, right

  unsigned int readings[3];
  readings[0] = analogRead(LI_LL);
  readings[1] = analogRead(LI_LM);
  readings[2] = analogRead(LI_LR);

  for (unsigned int i = 0; i < LI_LEFT_NUM; i++) {
    if (readings[i] >= li_thres[i + LI_FRONT_NUM] && !prev_out[i]
        || readings[i] < li_thres[i + LI_FRONT_NUM] && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] || prev_out[1] || prev_out[2];
}

bool IsRightOut() {
  static bool prev_out[3] = { false, false, false }; // Left, middle, right

  unsigned int readings[3];
  readings[0] = analogRead(LI_RL);
  readings[1] = analogRead(LI_RM);
  readings[2] = analogRead(LI_RR);

  for (unsigned int i = 0; i < LI_RIGHT_NUM; i++) {
    if (readings[i] >= li_thres[i + 7] && !prev_out[i]
        || readings[i] < li_thres[i + 7] && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] || prev_out[1] || prev_out[2];
}

bool IsBackOut() {
  static bool prev_out[3] = { false, false, false };

  unsigned int readings[3];
  readings[0] = analogRead(LI_BF);
  readings[1] = analogRead(LI_BM);
  readings[2] = analogRead(LI_BB);

  for (unsigned int i = 0; i < LI_BACK_NUM; i++) {
    if (readings[i] >= li_thres[i + 10] && !prev_out[i]
        || readings[i] < li_thres[i + 10] && prev_out[i]) {
      prev_out[i] = !prev_out[i];
    }
  }
  return prev_out[0] || prev_out[1] || prev_out[2];
}
