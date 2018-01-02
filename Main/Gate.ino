#define GT A15

#define THRES_IN 20
#define THRES_OUT 100

int gt_reading;

void InitGate() {
  gt_reading = 0;
}

int ReadGate() {
  gt_reading = analogRead(GT);
  return gt_reading;
}

bool IsBallInGate() {
  return gt_reading < THRES_IN;
}
