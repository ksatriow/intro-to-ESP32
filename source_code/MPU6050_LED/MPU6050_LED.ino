#include <MPU6050_tockn.h>
#include <Wire.h>
int pin [] = {15, 16, 17, 32, 12, 33, 14, 26};

MPU6050 mpu6050(Wire);

long timer = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode (pin[i], OUTPUT);
    digitalWrite(pin[i], 0);
  }

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  int ll = map(mpu6050.getAngleX(), -45, 45, 0, 8);
  for (int i = 0; i < 8; i++){
    if (i < ll){
      digitalWrite(pin[i], 1);
    }
    else {
      digitalWrite(pin[i], 0);
    }
  }
  Serial.print("angleX : "); Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : "); Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : "); Serial.println(mpu6050.getAngleZ());
  Serial.println("");
}
