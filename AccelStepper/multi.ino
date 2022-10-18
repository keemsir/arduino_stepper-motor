//// include library ////

#include <AccelStepper.h>
//#include <MultiStepper.h>

// input PWM setting
AccelStepper stepper1(AccelStepper::FULL4WIRE,2,4,3,5); //5,3,4,2 // in4, in2, in3, in1
AccelStepper stepper2(AccelStepper::FULL4WIRE,6,8,7,9); //9,7,8,6 // in4, in2, in3, in1
AccelStepper stepper3(AccelStepper::FULL4WIRE,10,12,11,13); //13,11,12,10 // in4, in2, in3, in1

//MultiStepper steppers;


// parameter setting
long receivedDistance_1 = 0;
long receivedDistance_2 = 0;
long receivedDistance_3 = 0;

long receivedDelay_1 = 0;
long receivedDelay_2 = 0;
long receivedDelay_3 = 0;

long receivedAcceleration_1 = 0;
long receivedAcceleration_2 = 0;
long receivedAcceleration_3 = 0;

char receivedCommand;
char movedAxis = '0';

//temp
volatile int stepPosition = 0;


//bool newData, runallowed = false; // booleans for new data from serial, and runallowed flag
bool newData, runallowed_x, runallowed_y, runallowed_z = false; // booleans for new data from serial, and runallowed flag
bool trans_dir1, trans_dir2, trans_dir3 = true; // steppermotor direction


// Define two motor objects
// The sequence 1-3-2-4 is required for proper sequencing of 28BYJ48


void setup()
{
  Serial.begin(9600);
  Serial.println("-----Testing Accelstepper Single Motor-----");

  // 1 revolution Motor 1 CW
  stepper1.setMaxSpeed(2000.0); // SPEED = Steps / second
  stepper1.setAcceleration(400.0); // ACCELERATION = Steps / (second)^2
  stepper1.setSpeed(1000);
  stepper1.moveTo(128);
  stepper1.disableOutputs();
  delay(500);

  // 1 revolution Motor 2 CW
  stepper2.setMaxSpeed(2000.0); // SPEED = Steps / second
  stepper2.setAcceleration(400.0); // ACCELERATION = Steps / (second)^2
  stepper2.setSpeed(1000);
  stepper2.moveTo(128);
  stepper2.disableOutputs();
  delay(500);

  // 1 revolution Motor 3 CW
  stepper3.setMaxSpeed(2000.0); // SPEED = Steps / second
  stepper3.setAcceleration(400.0); // ACCELERATION = Steps / (second)^2
  stepper3.setSpeed(1000);
  stepper3.moveTo(128);
  stepper3.disableOutputs();
  delay(500);

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


void loop()
{
  checkSerial();

  continuousRun_x();
  continuousRun_y();
  continuousRun_z();
  
  if (receivedCommand == 'z')
  {
    Serial.print("XX: ");
    Serial.print(receivedCommand);
    Serial.print(receivedDistance_1);
    Serial.println(receivedDelay_1);
    //continuousRun_x();
  }
  if (receivedCommand == 'x')
  {
    Serial.print("YY: ");
    Serial.print(receivedCommand);
    Serial.print(receivedDistance_2);
    Serial.println(receivedDelay_2);
    //continuousRun_y();
  }
  if (receivedCommand == 'c')
  {
    Serial.print("ZZ: ");
    Serial.print(receivedCommand);
    Serial.print(receivedDistance_3);
    Serial.println(receivedDelay_3);
    //continuousRun_z();
  }

}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


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
    // motor 1 moving
    if (receivedCommand == 'z') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_x = true;

      receivedDistance_1 = Serial.parseFloat();
      receivedDelay_1 = Serial.parseFloat();

      Serial.print(receivedDistance_1);
      Serial.print(receivedDelay_1);
      Serial.println("Measure ");
      stepper1.setMaxSpeed(receivedDelay_1);
      stepper1.move(receivedDistance_1);

    }

    // START - reverse
    if (receivedCommand == 'a')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_x = true;

      receivedDistance_1 = Serial.parseFloat();
      receivedDelay_1 = Serial.parseFloat();

      Serial.print(receivedDistance_1);
      Serial.print(receivedDelay_1);
      Serial.println("CLOSE ");
      stepper1.setMaxSpeed(receivedDelay_1);
      stepper1.move(-1 * receivedDistance_1);
      
    }

    // motor 2 moving
    if (receivedCommand == 'x') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_y = true;


      receivedDistance_2 = Serial.parseFloat();
      receivedDelay_2 = Serial.parseFloat();

      Serial.print(receivedDistance_2);
      Serial.print(receivedDelay_2);
      Serial.println("Measure ");
      stepper2.setMaxSpeed(receivedDelay_2);
      stepper2.move(receivedDistance_2);

    }

    // START - reverse
    if (receivedCommand == 's')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_y = true;

      receivedDistance_2 = Serial.parseFloat();
      receivedDelay_2 = Serial.parseFloat();

      Serial.print(receivedDistance_2);
      Serial.print(receivedDelay_2);
      Serial.println("CLOSE ");
      stepper2.setMaxSpeed(receivedDelay_2);
      stepper2.move(-1 * receivedDistance_2);
      
    }

    // motor 3 moving
    if (receivedCommand == 'c') //this is the measure part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_z = true;


      receivedDistance_3 = Serial.parseFloat();
      receivedDelay_3 = Serial.parseFloat();

      Serial.print(receivedDistance_3);
      Serial.print(receivedDelay_3);
      Serial.println("Measure ");
      stepper3.setMaxSpeed(receivedDelay_3);
      stepper3.move(receivedDistance_3);

    }

    // START - reverse
    if (receivedCommand == 'd')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_z = true;

      receivedDistance_3 = Serial.parseFloat();
      receivedDelay_3 = Serial.parseFloat();

      Serial.print(receivedDistance_3);
      Serial.print(receivedDelay_3);
      Serial.println("CLOSE ");
      stepper3.setMaxSpeed(receivedDelay_3);
      stepper3.move(-1 * receivedDistance_3);
      
    }

    // START - STOP
    if (receivedCommand == 'q') //"stop-x"
    {
      runallowed_x = false;

      stepper1.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper1.stop();
      stepper1.disableOutputs();
      
    }

    // START - STOP
    if (receivedCommand == 'w') //"stop-y"
    {
      runallowed_y = false;

      stepper2.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper2.stop();
      stepper2.disableOutputs();
      
    }

    // START - STOP
    if (receivedCommand == 'e') //"stop-z"
    {
      runallowed_z = false;

      stepper3.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper3.stop();
      stepper3.disableOutputs();
      
    }

    
    /*
    // START - OPEN
    if (receivedCommand == 'o') //OPENING
    {
      // example o 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed = true;


      receivedDistance_1 = Serial.parseFloat();
      receivedDelay_1 = Serial.parseFloat();

      Serial.print(receivedDistance_1);
      Serial.print(receivedDelay_1);
      Serial.print("OPEN ");
      stepper1.setMaxSpeed(receivedDelay_1);
      stepper1.move(receivedDistance_1);
      
    }

    // START - CLOSE
    if (receivedCommand == 'c')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed = true;


      receivedDistance_1 = Serial.parseFloat();
      receivedDelay_1 = Serial.parseFloat();

      Serial.print(receivedDistance_1);
      Serial.print(receivedDelay_1);
      Serial.println("CLOSE ");
      stepper1.setMaxSpeed(receivedDelay_1);
      stepper1.move(-1 * receivedDistance_1);
      
    }

    
    
    // START - STOP
    if (receivedCommand == 'n')
    {
      runallowed = false;

      stepper1.setCurrentPosition(0);
      Serial.println("STOP ");
      stepper1.stop();
      stepper1.disableOutputs();
      
    }

    //SET ACCELERATION
    if (receivedCommand == 'a')
    {
      runallowed = false;

      receivedAcceleration_1 = Serial.parseFloat();

      stepper1.setAcceleration(receivedAcceleration_1);

      Serial.println("ACC Updated ");

    }
    */

  }
  // after we went through the above tasks, newData becomes false again, so we are ready to receive new commands again.
  newData = false;

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


void continuousRun_x()
{

  if (runallowed_x == true)
  {
    if (abs(stepper1.currentPosition()) < receivedDistance_1) //abs () is needed because of the '<'
    {
      stepper1.enableOutputs();
      stepper1.run();
    }
    else // program enters this part if the required distance is completed
    {

      runallowed_x = false; //disable running -> the program will not try to enter this if-else anymore
      stepper1.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper1.currentPosition()); //print pos -> this will show you the latest relative number of steps
      stepper1.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper1.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }


  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void continuousRun_y()
{

  if (runallowed_y == true)
  {
    if (abs(stepper2.currentPosition()) < receivedDistance_2) //abs () is needed because of the '<'
    {
      stepper2.enableOutputs();
      stepper2.run();
    }
    else // program enters this part if the required distance is completed
    {

      runallowed_y = false; //disable running -> the program will not try to enter this if-else anymore
      stepper2.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper2.currentPosition()); //print pos -> this will show you the latest relative number of steps
      stepper2.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper2.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }


  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}


void continuousRun_z()
{ 

  if (runallowed_z == true)
  {
    if (abs(stepper3.currentPosition()) < receivedDistance_3) //abs () is needed because of the '<'
    {
      stepper3.enableOutputs();
      stepper3.run();
    }
    else // program enters this part if the required distance is completed
    {

      runallowed_z = false; //disable running -> the program will not try to enter this if-else anymore
      stepper3.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper3.currentPosition()); //print pos -> this will show you the latest relative number of steps
      stepper3.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper3.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }

  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}


//ref url: https://www.youtube.com/watch?v=AR0un3kg-iM
