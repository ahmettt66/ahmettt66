#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int forwardright_val, forwardleft_val, backwardright_val, backwardleft_val, midleft_val, midright_val = 0;

const byte forwardrightmotorPin = 3;
const byte forwardleftmotorPin = 2;
const byte midrightmotorPin = 1;
const byte midleftmotorPin = 5;
const byte backwardrightmotorPin = 4;
const byte backwardleftmotorPin = 0;
const byte servoPin = 7;

int PrevAngle = 0;
void initializePWM() {
  pwm.begin();
  pwm.setPWMFreq(ESC_FREQ);
  pwm.writeMicroseconds(servoPin, 600);
}


void initializeMotor() {
  pwm.writeMicroseconds(0, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(1, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(2, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(3, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(4, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(5, MOTOR_STOP_VALUE);
  delay(2000);
}

void setXZThrusterSpeed(int valueJoyStick_X_2, int valueJoyStick_Y_2, int valueJoyStick_Y_1) {
  forwardright_val  = 1500 + (valueJoyStick_X_2) - (valueJoyStick_Y_2) - (valueJoyStick_Y_1);
  forwardleft_val   = 1500 + (valueJoyStick_X_2) + (valueJoyStick_Y_2) + (valueJoyStick_Y_1);
  backwardright_val = 1500 + (valueJoyStick_X_2) + (valueJoyStick_Y_2) - (valueJoyStick_Y_1);
  backwardleft_val  = 1500 + (valueJoyStick_X_2) - (valueJoyStick_Y_2) + (valueJoyStick_Y_1);

  if (backwardleft_val > 1500) {
    backwardleft_val += 35;
  }

  if (backwardleft_val < 1500) {
    backwardleft_val -= 35;
  }
}

void setYThrusterSpeed(int valueJoyStick_X_1) {
  midleft_val = 1500 - (valueJoyStick_X_1);
  midright_val = 1500 - (valueJoyStick_X_1); // Son yapilan testte orta sag motor ters yonde dondugu icin midright_val = 1500 - (valueJoyStick_X_1); olan kod satırı midright_val = 1500 + (valueJoyStick_X_1) olarak degistirilimistir.
}

void spinThruster() {

  if (forwardright_val >= MOTOR_MAX) forwardright_val = MOTOR_MAX;
  else if (forwardright_val <= MOTOR_MIN) forwardright_val = MOTOR_MIN;
  if (backwardright_val >= MOTOR_MAX) backwardright_val = MOTOR_MAX;
  else if (backwardright_val <= MOTOR_MIN) backwardright_val = MOTOR_MIN;
  if (forwardleft_val >= MOTOR_MAX) forwardleft_val = MOTOR_MAX;
  else if (forwardleft_val <= MOTOR_MIN) forwardleft_val = MOTOR_MIN;
  if (backwardleft_val >= MOTOR_MAX) backwardleft_val = MOTOR_MAX;
  else if (backwardleft_val <= MOTOR_MIN) backwardleft_val = MOTOR_MIN;

  pwm.writeMicroseconds(forwardrightmotorPin, forwardright_val);
  pwm.writeMicroseconds(forwardleftmotorPin, forwardleft_val);
  pwm.writeMicroseconds(backwardrightmotorPin, backwardright_val);
  pwm.writeMicroseconds(backwardleftmotorPin, backwardleft_val);
  pwm.writeMicroseconds(midrightmotorPin, midright_val);
  pwm.writeMicroseconds(midleftmotorPin, midleft_val);

}

void stopMotor() {
  pwm.writeMicroseconds(forwardrightmotorPin, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(forwardleftmotorPin, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(backwardrightmotorPin, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(backwardleftmotorPin, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(midrightmotorPin, MOTOR_STOP_VALUE);
  pwm.writeMicroseconds(midleftmotorPin, MOTOR_STOP_VALUE);
}

void setServoAngle(int angle) {
  angle = map(angle, 0, 255, 600, 1800);
  pwm.writeMicroseconds(servoPin, angle);
}
