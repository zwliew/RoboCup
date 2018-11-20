#include "Compass.h"
#include "Infrared.h"
#include "Light.h"

// Debug controls
//#define DEBUG_SENSOR_READINGS

#define BAUD_RATE 9600

// Motor pins
#define PIN_BL 45
#define PIN_FL 47
#define PIN_FR 51
#define PIN_BR 53

static Compass compass;
struct compassValues {
  int bearing;
} static cp;

static Infrared infrared;
struct infraredValues {
  int direction;
  int strength;
} static ir;

static Light light;
struct lightValues {
  int location; // 0 -> Front; 1 -> Right; 2 -> Back; 3 -> Left
} static li;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Initializing...");

  Wire.begin();

  // Configure motors
  pinMode(PIN_FL, OUTPUT);
  pinMode(PIN_FR, OUTPUT);
  pinMode(PIN_BL, OUTPUT);
  pinMode(PIN_BR, OUTPUT);

  // Initialize sensors
  compass.init();
  infrared.init();

  Serial.println("Initialization complete!");
}

void loop() {
  // Read sensor values
  compass.read(&cp.bearing);
  infrared.read(&ir.strength, &ir.direction);
  light.read(&li.location);

#ifdef DEBUG_SENSOR_READINGS
  Serial.print("Bearing=");
  Serial.print(cp.bearing);
  Serial.print("   Strength=");
  Serial.print(ir.strength);
  Serial.print("   Direction=");
  Serial.print(ir.direction);
  Serial.print("   Location=");
  Serial.println(li.location);
#endif
}
