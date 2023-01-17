

#include <AccelStepper.h>
#include <MultiStepper.h>


// -
#define CW_x    22
#define CCW_x   24
// +
#define CW_y    30
#define CCW_y   32
// -
#define CW_z    38
#define CCW_z   40
// +
#define CW_Z    46
#define CCW_Z   48


// sample code
AccelStepper stepper_x(AccelStepper::FULL4WIRE,2,4,3,5);
AccelStepper stepper_y(AccelStepper::FULL4WIRE,6,8,7,9);
AccelStepper stepper_z(AccelStepper::FULL4WIRE,10,12,11,13);
AccelStepper stepper_Z(AccelStepper::FULL4WIRE,14,16,15,17);

// for 4D phantom 
//AccelStepper stepper_x(AccelStepper::DRIVER, CW_x, CCW_x); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_y(AccelStepper::DRIVER, CW_y, CCW_y); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_z(AccelStepper::DRIVER, CW_z, CCW_z); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_Z(AccelStepper::DRIVER, CW_Z, CCW_Z); // CW+: PLS(pulse), CCW+: DIR

MultiStepper steppers;

unsigned long MotorSpeed;
unsigned long time_a, time_b;


void setup() {
  Serial.begin(115200);
  MotorSpeed = 850;

  stepper_x.setMaxSpeed(MotorSpeed);
  stepper_y.setMaxSpeed(MotorSpeed);
  stepper_z.setMaxSpeed(MotorSpeed);
  stepper_Z.setMaxSpeed(MotorSpeed);

//  stepper_x.setAcceleration(800);
//  stepper_y.setAcceleration(500);
//  stepper_z.setAcceleration(500);
//  stepper_Z.setAcceleration(500);
  
  steppers.addStepper(stepper_x);
  steppers.addStepper(stepper_y);
  steppers.addStepper(stepper_z);
//  steppers.addStepper(stepper_Z);
}



void loop() {
  long positions[3];

  time_a = millis();

  positions[0] = 100;
  positions[1] = 200;
  positions[2] = 400;
//  positions[3] = 800;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  delay(75);

  time_b = millis();

  positions[0] = -100;
  positions[1] = -200;
  positions[2] = -400;
//  positions[3] = -800;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  delay(75);

  Serial.println(time_b - time_a);

//  positions[0] = 0;
//  positions[1] = 400;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition();
//  delay(100);

//  positions[0] = 0;
//  positions[1] = 500;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition();
//  delay(100);
}
