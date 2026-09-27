#include <Wire.h>
#include <AccelStepper.h>

#define stepPin 25
#define dirPin 26
#define enPin 27

AccelStepper stepper(AccelStepper::Driver,stepPin,dirPin);
const float speed = 800.0;







const int MPU_ADDR = 0x68;

const float ACCEL_SCALE = 16384.0; // LSB/g,     +/-2g  (power-on default)
const float GYRO_SCALE  = 131.0;   // LSB/deg/s, +/-250 dps (power-on default)
const float ALPHA = 0.98;

float gyroYoffset = 0.0;
float tiltAngle = 0.0;
unsigned long lastMicros;

bool readMPU(int16_t &ax, int16_t &az, int16_t &gy) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, 14, true) != 14) return false;

  uint8_t b[14];
  for (int i = 0; i < 14; i++) b[i] = Wire.read();

  ax = (int16_t)((b[0]  << 8) | b[1]);   // accel X
  az = (int16_t)((b[4]  << 8) | b[5]);   // accel Z
  gy = (int16_t)((b[10] << 8) | b[11]);  // gyro Y
  return true;
}

void calibrateGyroY() {
  const int N = 2000;
  double sum = 0;
  int good = 0;
  int16_t ax, az, gy;

  Serial.println("Calibrating - keep the bot completely still...");
  delay(500);

  for (int i = 0; i < N; i++) {
    if (readMPU(ax, az, gy)) {
      sum += gy / GYRO_SCALE;
      good++;
    }
    delay(1);
  }

  if (good < N * 0.9) {
    Serial.println("WARNING: many failed reads during calibration - check wiring.");
  }

  gyroYoffset = sum / good;
  Serial.print("Gyro Y bias: "); Serial.println(gyroYoffset, 4);
}

float getTiltAngle() {
  int16_t rawAX, rawAZ, rawGY;

  if (!readMPU(rawAX, rawAZ, rawGY)) {
    Serial.println("ERROR: MPU6050 read failed - holding last angle");
    return tiltAngle;  // never hand a control loop a fabricated number
  }

  float accX_g = rawAX / ACCEL_SCALE;
  float accZ_g = rawAZ / ACCEL_SCALE;
  float gyroY_rate = (rawGY / GYRO_SCALE) - gyroYoffset;

  unsigned long now = micros();
  float dt = (now - lastMicros) / 1000000.0;
  lastMicros = now;

  float accelAngle = atan2(accX_g, accZ_g) * RAD_TO_DEG;
  tiltAngle = ALPHA * (tiltAngle + gyroY_rate * dt) + (1.0 - ALPHA) * accelAngle;
  return tiltAngle;
}

void setup() {
pinMode(enPin,OUTPUT);
digitalWrite(enPin,LOW);

stepper.setMaxSpeed(2000);
stepper.setSpeed(speed);
//stepper code above.


  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  Wire.setClock(400000);

  Wire.beginTransmission(MPU_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("CRITICAL: MPU6050 not found. Check wiring on GPIO 21/22.");
    while (1) delay(100);
  }

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); Wire.write(0x00);  // wake up
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1A); Wire.write(0x03);  // DLPF ~44 Hz, cuts vibration noise
  Wire.endTransmission(true);

  delay(100);

  calibrateGyroY();

  int16_t ax, az, gy;
  readMPU(ax, az, gy);
  tiltAngle = atan2(ax / ACCEL_SCALE, az / ACCEL_SCALE) * RAD_TO_DEG; // seed real angle

  lastMicros = micros();
}

void loop() {
  float angle = getTiltAngle();
  Serial.print("angle: ");
  Serial.println(angle);
  delay(10);

if (angle >> 0){

}
}