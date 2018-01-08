#define GT A15

#define GT_THRES_IN 20

int gt_reading;

void InitGate() {
  gt_reading = 0;
}

int ReadGate() {
  gt_reading = analogRead(GT);
  return gt_reading;
}

bool IsBallInGate() {
  return gt_reading < GT_THRES_IN;
}
