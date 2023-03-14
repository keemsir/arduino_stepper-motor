
// example: constant code b500 500 500 500 400
// serial: b500 500 500 500 500
// 왜 motor 2에 LED가 전부 안들어오는지 ? 

#include <AccelStepper.h>
#include <MultiStepper.h>

//#include <RotaryEncoder.h>


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


/////////////////// for Rotary Encoder //////////////////
/// | int.0 | int.1 | int.2 | int.3 | int.4 | int.5 | ///
/// |   2   |   3   |  21   |  20   |  19   |  18   | ///
/////////////////////////////////////////////////////////


#define encoder0_PinA 0
#define encoder0_PinB 1
#define encoder0_PinZ 2

volatile signed long cnt0 = 0;
volatile signed char dir0 = 1;

//volatile int encoder0_Pos = 0;

//volatile boolean PastA = 0;
//volatile boolean PastB = 0;



////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// for sample
AccelStepper stepper_x(AccelStepper::FULL4WIRE,4,6,5,7);
AccelStepper stepper_y(AccelStepper::FULL4WIRE,8,10,9,11);
AccelStepper stepper_z(AccelStepper::FULL4WIRE,12,14,13,15);
AccelStepper stepper_Z(AccelStepper::FULL4WIRE,16,18,17,19);


// for 4D phantom
//AccelStepper stepper_x(AccelStepper::DRIVER, CW_x, CCW_x); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_y(AccelStepper::DRIVER, CW_y, CCW_y); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_z(AccelStepper::DRIVER, CW_z, CCW_z); // CW+: PLS(pulse), CCW+: DIR
//AccelStepper stepper_Z(AccelStepper::DRIVER, CW_Z, CCW_Z); // CW+: PLS(pulse), CCW+: DIR


MultiStepper steppers;

unsigned long MotorSpeed;
unsigned long MotorSpeed_Acceleration;
unsigned long time_a, time_b;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// accelator parameter setting

long receivedDistance_x = 0;
long receivedDistance_y = 0;
long receivedDistance_z = 0;
long receivedDistance_Z = 0;

long receivedDelay_x = 0;
long receivedDelay_y = 0;
long receivedDelay_z = 0;
long receivedDelay_Z = 0;

long receivedDistance_0;
long receivedDistance_1;
long receivedDistance_2;
long receivedDistance_3;

char receivedCommand;
char movedAxis = '0';

unsigned long time_x, time_x1, time_y, time_y1, time_z, time_z1, time_Z, time_Z1;


bool newData, runallowed_x, runallowed_y, runallowed_z, runallowed_Z = false; // booleans for new data from serial, and runallowed flag
bool run_multistepper = false;
bool RepeatOperation_x, RepeatOperation_y, RepeatOperation_z, RepeatOperation_Z = false;
bool Homing_x, Homing_y, Homing_z, Homing_Z = false;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  
  Serial.println("-----Starting Stepper Motor-----");
  Serial.println("input axis: setMaxSpeed, Move"); // !!! 수정필요
  
  //MotorSpeed = 500; // step per second
  MotorSpeed_Acceleration = 800;
  
  stepper_x.setMaxSpeed(MotorSpeed);
  stepper_y.setMaxSpeed(MotorSpeed);
  stepper_z.setMaxSpeed(MotorSpeed);
  stepper_Z.setMaxSpeed(MotorSpeed);
  
  steppers.addStepper(stepper_x);
  steppers.addStepper(stepper_y);
  steppers.addStepper(stepper_z);
  steppers.addStepper(stepper_Z);


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// for acceleration /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


  stepper_x.setAcceleration(MotorSpeed_Acceleration);
  stepper_y.setAcceleration(MotorSpeed_Acceleration);
  stepper_z.setAcceleration(MotorSpeed_Acceleration);
  stepper_Z.setAcceleration(MotorSpeed_Acceleration);

  stepper_x.disableOutputs();
  stepper_y.disableOutputs();
  stepper_z.disableOutputs();
  stepper_Z.disableOutputs();


////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// for encoder ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


//  pinMode(encoder0_PinA, INPUT);
//  pinMode(encoder0_PinB, INPUT);
//
//  PastA = (boolean)digitalRead(encoder0_PinA);
//  PastB = (boolean)digitalRead(encoder0_PinB);
//
//  attachInterrupt(0, doEncoder_A, RISING);
//  attachInterrupt(1, doEncoder_B, CHANGE);
//
  attachInterrupt(encoder0_PinA, encoderCount_0, FALLING);
  pinMode(encoder0_PinB, INPUT);
  attachInterrupt(encoder0_PinZ, encoderReset_0, FALLING);

}


void loop() {

// Checking the serial
  checkSerial();

// Runing the stepper motor (x,y,z,Z axis)
  continuousRun_const();

  continuousRun_x();
  continuousRun_y();
  continuousRun_z();
  continuousRun_Z();

// Rotary encoder
//  if (digitalRead(Encoder_x1) == HIGH)
//  {
//    Serial.println("NC");
//  }
//
//  if (digitalRead(NO))
//  {
//    Serial.println(NO);
//  }
//  delay(1000);

  GoHome();

//  Serial.println(digitalRead(encoder0_PinA));
//  Serial.println(digitalRead(encoder0_PinB));
//  Serial.println(digitalRead(encoder0_PinZ));
//  Serial.print("cnt: ");
//  Serial.println(cnt0);
//  Serial.print(": ");
//  Serial.println();
//  Serial.print("cnt: ");
//  Serial.println();
//  delay(100);

}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// def setting //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


void encoderCount_0()
{
  dir0 = (digitalRead(encoder0_PinB)==HIGH)? 1:-1;
  cnt0 += dir0;
}


void encoderReset_0()
{
  cnt0 = 0;
}



void checkSerial()
{
  //switch-case would also work, and maybe more elegant

  if (Serial.available() > 0)
  {
    receivedCommand = Serial.read();
    newData = true;
  }

  if (newData == true)
  {
    
    //////////////////////////////////////////////////
    ///////////////// constant speed /////////////////
    //////////////////////////////////////////////////
    
    if (receivedCommand == 'b') /// constant speed ///
    {
      run_multistepper = true;

      receivedDistance_0 = Serial.parseFloat();
      receivedDistance_1 = Serial.parseFloat();
      receivedDistance_2 = Serial.parseFloat();
      receivedDistance_3 = Serial.parseFloat();

      MotorSpeed = Serial.parseFloat();
      stepper_x.setMaxSpeed(MotorSpeed);
      stepper_y.setMaxSpeed(MotorSpeed);
      stepper_z.setMaxSpeed(MotorSpeed);
      stepper_Z.setMaxSpeed(MotorSpeed);

      Serial.print("Distance_0: ");
      Serial.println(receivedDistance_0);
      Serial.print("Distance_1: ");
      Serial.println(receivedDistance_1);
      Serial.print("Distance_2: ");
      Serial.println(receivedDistance_2);
      Serial.print("Distance_3: ");
      Serial.println(receivedDistance_3);
      Serial.print("MotorSpeed: ");
      Serial.println(MotorSpeed);
    }

    if (receivedCommand == 'n') // stop
    {
      Serial.println("STOP ");
      run_multistepper = false;

//      steppers.moveTo(0);
//      steppers.runSpeedToPosition();

    }

    
    //////////////////////////////////////////////////
    //////////////// Accelator speed /////////////////
    //////////////////////////////////////////////////
    
    // motor 1 moving
    if (receivedCommand == 'z') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_x = true;
      RepeatOperation_x = true;

      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = Serial.parseFloat();
      
      Serial.print("Distance: ");
      Serial.print(receivedDistance_x);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_x);
      stepper_x.setCurrentPosition(0);
      stepper_x.move(-1 * receivedDistance_x);
      stepper_x.setMaxSpeed(receivedDelay_x);
    }
    
    // START - verse
    if (receivedCommand == 'a')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_x = true;
      RepeatOperation_x = false;

      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_x);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_x);
      stepper_x.setCurrentPosition(0);
      stepper_x.move(-1 * receivedDistance_x);
      stepper_x.setMaxSpeed(receivedDelay_x);
    }

    // START - reverse
    if (receivedCommand == 'g')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_x = true;
      RepeatOperation_x = false;

      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_x);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_x);
      stepper_x.setCurrentPosition(0);
      stepper_x.move(receivedDistance_x);
      stepper_x.setMaxSpeed(receivedDelay_x);
    }

    // motor 2 moving
    if (receivedCommand == 'x') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_y = true;
      RepeatOperation_y = true;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = Serial.parseFloat();

      Serial.print("Distance: ");
      Serial.print(receivedDistance_y);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_y);
      stepper_y.setCurrentPosition(0);
      stepper_y.move(receivedDistance_y);
      stepper_y.setMaxSpeed(receivedDelay_y);
    }

    // START - reverse
    if (receivedCommand == 's')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_y = true;
      RepeatOperation_y = false;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_y);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_y);
      stepper_y.setCurrentPosition(0);
      stepper_y.move(receivedDistance_y);
      stepper_y.setMaxSpeed(receivedDelay_y);
    }

    // START - reverse
    if (receivedCommand == 'h')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_y = true;
      RepeatOperation_y = false;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_y);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_y);
      stepper_y.setCurrentPosition(0);
      stepper_y.move(-1 * receivedDistance_y);
      stepper_y.setMaxSpeed(receivedDelay_y);
    }
    

    // motor 3 moving
    if (receivedCommand == 'c') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_z = true;
      RepeatOperation_z = true;

      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = Serial.parseFloat();

      Serial.print("Distance: ");
      Serial.print(receivedDistance_z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_z);
      stepper_z.setCurrentPosition(0);
      stepper_z.move(-1 * receivedDistance_z);
      stepper_z.setMaxSpeed(receivedDelay_z);
    }

    // START - reverse
    if (receivedCommand == 'd')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_z = true;
      RepeatOperation_z = false;
      
      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = 500;
      
      Serial.print("Distance: ");
      Serial.print(receivedDistance_z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_z);
      stepper_z.setCurrentPosition(0);
      stepper_z.move(-1 * receivedDistance_z);
      stepper_z.setMaxSpeed(receivedDelay_z);
    }

    // START - reverse
    if (receivedCommand == 'j')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_z = true;
      RepeatOperation_z = false;

      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_z);
      stepper_z.setCurrentPosition(0);
      stepper_z.move(receivedDistance_z);
      stepper_z.setMaxSpeed(receivedDelay_z);
    }


    // Z-axis motor moving
    if (receivedCommand == 'v') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_Z = true;
      RepeatOperation_Z = true;


      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = Serial.parseFloat();

      Serial.print("Distance: ");
      Serial.print(receivedDistance_Z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_Z);
      stepper_Z.setCurrentPosition(0);
      stepper_Z.move(receivedDistance_Z);
      stepper_Z.setMaxSpeed(receivedDelay_Z);
    }

    // START - reverse
    if (receivedCommand == 'f')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_Z = true;
      RepeatOperation_Z = false;

      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_Z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_Z);
      stepper_Z.setCurrentPosition(0);
      stepper_Z.move(receivedDistance_Z);
      stepper_Z.setMaxSpeed(receivedDelay_Z);
    }

    // START - reverse
    if (receivedCommand == 'k')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_Z = true;
      RepeatOperation_Z = false;

      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = 500;

      Serial.print("Distance: ");
      Serial.print(receivedDistance_Z);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_Z);
      stepper_Z.setCurrentPosition(0);
      stepper_Z.move(-1 * receivedDistance_Z);
      stepper_Z.setMaxSpeed(receivedDelay_Z);
    }


    // STOP the stepper motor
    if (receivedCommand == 'q') //"stop-x"
    {
      runallowed_x = false;
      RepeatOperation_x = false;

//      stepper_x.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper_x.stop();
      stepper_x.disableOutputs();
    }

    // STOP the stepper motor
    if (receivedCommand == 'w') //"stop-y"
    {
      runallowed_y = false;
      RepeatOperation_y = false;

//      stepper_y.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper_y.stop();
      stepper_y.disableOutputs();
    }

    // STOP the stepper motor
    if (receivedCommand == 'e') //"stop-z"
    {
      runallowed_z = false;
      RepeatOperation_z = false;

//      stepper_z.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper_z.stop();
      stepper_z.disableOutputs();
    }

    // STOP the stepper motor
    if (receivedCommand == 'r') //"stop-Z"
    {
      runallowed_Z = false;
      RepeatOperation_Z = false;

//      stepper_Z.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper_Z.stop();
      stepper_Z.disableOutputs();
    }


    // Homing the stepper motor x-axis
    if (receivedCommand == 'u')
    {
      Homing_x = true;
      Serial.println("Homing the x-axis motor");
    }

    // Homing the stepper motor y-axis
    if (receivedCommand == 'i')
    {
      Homing_y = true;
      Serial.println("Homing the y-axis motor");
    }

    // Homing the stepper motor z-axis
    if (receivedCommand == 'o')
    {
      Homing_z = true;
      Serial.println("Homing the z-axis motor");
    }

    // Homing the stepper motor Z-axis
    if (receivedCommand == 'p')
    {
      Homing_Z = true;
      Serial.println("Homing the Z-axis motor");
    }

  }
  // after we went through the above tasks, newData becomes false again, so we are ready to receive new commands again.
  newData = false;
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



void continuousRun_const()
{
  if (run_multistepper == true)
  {
    long positions[4];
    
    time_a = millis();
    
    positions[0] = receivedDistance_0;
    positions[1] = -receivedDistance_1;
    positions[2] = receivedDistance_2;
    positions[3] = -receivedDistance_3;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
    
    time_b = millis();
    Serial.print("cycle1 time:");
    Serial.println(time_b - time_a);
    
//    Serial.print("local maximum: ");
//    Serial.println(cnt0);
    
    delay(50);
    
    time_a = millis();
    
    positions[0] = -receivedDistance_0;
    positions[1] = receivedDistance_1;
    positions[2] = -receivedDistance_2;
    positions[3] = receivedDistance_3;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();

    time_b = millis();
    Serial.print("cycle2 time:");
    Serial.println(time_b - time_a);

//    Serial.print("local minimum: ");
//    Serial.println(cnt0);
    
    delay(50);
  }
  
  else if (run_multistepper == false && runallowed_x == false && runallowed_y == false && runallowed_z == false && runallowed_Z == false)
  {
    long positions[4];
    
    positions[0] = 0;
    positions[1] = 0;
    positions[2] = 0;
    positions[3] = 0;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
  }

  else
  {
    return;
  }
}



void continuousRun_x()
{
  if (runallowed_x == true && RepeatOperation_x == true)
  {
    if (stepper_x.distanceToGo() == 0)
    {
      Serial.print("x-axis motor position: ");
      Serial.println(stepper_x.currentPosition());
      stepper_x.moveTo(-stepper_x.currentPosition());

      if (stepper_x.currentPosition() == receivedDistance_x)
      {
        time_x = millis();
        Serial.print("time(x): ");
        Serial.println(time_x-time_x1);
      }
      else if (stepper_x.currentPosition() == -receivedDistance_x)
      {
        time_x1 = millis();
        Serial.print("time(x): ");
        Serial.println(time_x1-time_x);
      }
    }
    stepper_x.enableOutputs();
    stepper_x.run();
  }
  
  else if (runallowed_x == true && RepeatOperation_x == false)
  {
    if (stepper_x.distanceToGo() != 0)
    {
      stepper_x.enableOutputs();
      stepper_x.run();
    }
    else
    {
      stepper_x.stop();
      stepper_x.disableOutputs();
    }
  }

  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void continuousRun_y()
{

  if (runallowed_y == true && RepeatOperation_y == true)
  {
    if (stepper_y.distanceToGo() == 0)
    {
      Serial.print("y-axis motor position: ");
      Serial.println(stepper_y.currentPosition());
      stepper_y.moveTo(-stepper_y.currentPosition());

      if (stepper_y.currentPosition() == receivedDistance_y)
      {
        time_y = millis();
        Serial.print("time(y): ");
        Serial.println(time_y-time_y1);
      }
      else if (stepper_y.currentPosition() == -receivedDistance_y)
      {
        time_y1 = millis();
        Serial.print("time(y): ");
        Serial.println(time_y1-time_y);
      }
    }
    stepper_y.enableOutputs();
    stepper_y.run();
  }

  else if (runallowed_y == true && RepeatOperation_y == false)
  {
    if (stepper_y.distanceToGo() != 0)
    {
      stepper_y.enableOutputs();
      stepper_y.run();
    }
    else
    {
      stepper_y.stop();
      stepper_y.disableOutputs();
    }
  }
  
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void continuousRun_z()
{

  if (runallowed_z == true && RepeatOperation_z == true)
  {
    if (stepper_z.distanceToGo() == 0)
    {
      Serial.print("z-axis motor position: ");
      Serial.println(stepper_z.currentPosition());
      stepper_z.moveTo(-stepper_z.currentPosition());

      if (stepper_z.currentPosition() == receivedDistance_z)
      {
        time_z = millis();
        Serial.print("time(z): ");
        Serial.println(time_z-time_z1);
      }
      else if (stepper_z.currentPosition() == -receivedDistance_z)
      {
        time_z1 = millis();
        Serial.print("time(z): ");
        Serial.println(time_z1-time_z);
      }
    }
    stepper_z.enableOutputs();
    stepper_z.run();
  }

  else if (runallowed_z == true && RepeatOperation_z == false)
  {
    if (stepper_z.distanceToGo() != 0)
    {
      stepper_z.enableOutputs();
      stepper_z.run();
    }
    else
    {
      stepper_z.stop();
      stepper_z.disableOutputs();
    }
  }
  
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void continuousRun_Z()
{

  if (runallowed_Z == true && RepeatOperation_Z == true)
  {
    if (stepper_Z.distanceToGo() == 0)
    {
      Serial.print("Z-axis motor position: ");
      Serial.println(stepper_Z.currentPosition());
      stepper_Z.moveTo(-stepper_Z.currentPosition());

      if (stepper_Z.currentPosition() == receivedDistance_Z)
      {
        time_Z = millis();
        Serial.print("time(Z): ");
        Serial.println(time_Z-time_Z1);
      }
      else if (stepper_Z.currentPosition() == -receivedDistance_Z)
      {
        time_Z1 = millis();
        Serial.print("time(Z): ");
        Serial.println(time_Z1-time_Z);
      }
    }
    stepper_Z.enableOutputs();
    stepper_Z.run();
  }

  else if (runallowed_Z == true && RepeatOperation_Z == false)
  {
    if (stepper_Z.distanceToGo() != 0)
    {
      stepper_Z.enableOutputs();
      stepper_Z.run();
    }
    else
    {
      stepper_Z.stop();
      stepper_Z.disableOutputs();
    }
  }

  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void printVIEW()
{
  Serial.println();
}


void updateSelection()
{
  Serial.println();
}



void GoHome() // homing
{
  if (Homing_x == true)
  {
    if (stepper_x.currentPosition() == 0 && Homing_x == true)
    {
      Serial.println("Going at the x-axis home position.");
      stepper_x.disableOutputs();
      Homing_x = false;
    }
    else
    {
      stepper_x.enableOutputs();
      stepper_x.setMaxSpeed(400);
      stepper_x.moveTo(0);
      stepper_x.run();
    }
  }

  if (Homing_y == true)
  {
    if (stepper_y.currentPosition() == 0 && Homing_y == true)
    {
      Serial.println("Going at the y-axis home position.");
      stepper_y.disableOutputs();
      Homing_y = false;
    }
    else
    {
      stepper_y.enableOutputs();
      stepper_y.setMaxSpeed(400);
      stepper_y.moveTo(0);
      stepper_y.run();
    }
  }

  if (Homing_z == true)
  {
    if (stepper_z.currentPosition() == 0 && Homing_z == true)
    {
      Serial.println("Going at the z-axis home position.");
      stepper_z.disableOutputs();
      Homing_z = false;
    }
    else
    {
      stepper_z.enableOutputs();
      stepper_z.setMaxSpeed(400);
      stepper_z.moveTo(0);
      stepper_z.run();
    }
  }

  if (Homing_Z == true)
  {
    if (stepper_Z.currentPosition() == 0 && Homing_Z == true)
    {
      Serial.println("Going at the Z-axis home position.");
      stepper_Z.disableOutputs();
      Homing_Z = false;
    }
    else
    {
      stepper_Z.enableOutputs();
      stepper_Z.setMaxSpeed(400);
      stepper_Z.moveTo(0);
      stepper_Z.run();
    }
  }
}

