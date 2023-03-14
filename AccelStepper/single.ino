
#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper_x(AccelStepper::FULL4WIRE,4,6,5,7);
AccelStepper stepper_y(AccelStepper::FULL4WIRE,8,10,9,11);

MultiStepper steppers;

// 
// 
// 

int A[3][84] = {{-81, 82, -87, 74, -105, 82, -79, 80, -111, 80, -67, 76, -103, 72, -103, 78, -81, 92, -107, 100, -115, 74, -111, 88, -103, 82, -105, 84, -105, 84, -99, 86, -63, 74, -107, 70, -107, 128, -97, 102, -107, 108, -87, 110, -75, 106, -85, 104, -115, 118, -101, 104, -89, 98, -103, 86, -119, 104, -113, 90, -105, 88, -121, 112, -89, 94, -113, 96, -93, 94, -135, 96, -41, 70, -87, 92, -115, 92, -135, 110, -97, 96, -119, 16},
{650, 2750, 1850, 2450, 1550, 2850, 1500, 2050, 1800, 2800, 1200, 2000, 1800, 2450, 2150, 2400, 1450, 4000, 1200, 2600, 4700, 2750, 1400, 2450, 1600, 2200, 1700, 2100, 1500, 2600, 2150, 2900, 1800, 1950, 1900, 2100, 2150, 2700, 2400, 2200, 1700, 2050, 2050, 2800, 1000, 1750, 1250, 2900, 1700, 2550, 1900, 2150, 2550, 1900, 1900, 2400, 2100, 3150, 2100, 2300, 2200, 2850, 1450, 3750, 1100, 2500, 2100, 2650, 1250, 1900, 2150, 2650, 1100, 1350, 1050, 2400, 1700, 2100, 2050, 2950, 1950, 1800, 2050, 1150},
{1283, 292, 434, 329, 520, 281, 538, 392, 446, 286, 676, 402, 446, 329, 374, 336, 557, 200, 676, 309, 170, 292, 577, 329, 503, 366, 472, 383, 538, 309, 374, 276, 446, 412, 423, 383, 374, 297, 336, 366, 472, 392, 392, 286, 812, 459, 649, 276, 472, 316, 423, 374, 316, 423, 423, 336, 383, 254, 383, 350, 366, 281, 557, 214, 738, 322, 383, 303, 649, 423, 374, 303, 738, 600, 773, 336, 472, 383, 392, 271, 412, 446, 392, 706}};

int B[3][84] = {{-81, 82, -87, 74, -105, 82, -79, 80, -111, 80, -67, 76, -103, 72, -103, 78, -81, 92, -107, 100, -115, 74, -111, 88, -103, 82, -105, 84, -105, 84, -99, 86, -63, 74, -107, 70, -107, 128, -97, 102, -107, 108, -87, 110, -75, 106, -85, 104, -115, 118, -101, 104, -89, 98, -103, 86, -119, 104, -113, 90, -105, 88, -121, 112, -89, 94, -113, 96, -93, 94, -135, 96, -41, 70, -87, 92, -115, 92, -135, 110, -97, 96, -119, 16},
{650, 2750, 1850, 2450, 1550, 2850, 1500, 2050, 1800, 2800, 1200, 2000, 1800, 2450, 2150, 2400, 1450, 4000, 1200, 2600, 4700, 2750, 1400, 2450, 1600, 2200, 1700, 2100, 1500, 2600, 2150, 2900, 1800, 1950, 1900, 2100, 2150, 2700, 2400, 2200, 1700, 2050, 2050, 2800, 1000, 1750, 1250, 2900, 1700, 2550, 1900, 2150, 2550, 1900, 1900, 2400, 2100, 3150, 2100, 2300, 2200, 2850, 1450, 3750, 1100, 2500, 2100, 2650, 1250, 1900, 2150, 2650, 1100, 1350, 1050, 2400, 1700, 2100, 2050, 2950, 1950, 1800, 2050, 1150},
{1283, 292, 434, 329, 520, 281, 538, 392, 446, 286, 676, 402, 446, 329, 374, 336, 557, 200, 676, 309, 170, 292, 577, 329, 503, 366, 472, 383, 538, 309, 374, 276, 446, 412, 423, 383, 374, 297, 336, 366, 472, 392, 392, 286, 812, 459, 649, 276, 472, 316, 423, 374, 316, 423, 423, 336, 383, 254, 383, 350, 366, 281, 557, 214, 738, 322, 383, 303, 649, 423, 374, 303, 738, 600, 773, 336, 472, 383, 392, 271, 412, 446, 392, 706}};


//int B[3][84] = {{-98, 91, -120, 81, -92, 83, -120, 81, -112, 79, -110, 109, -146, 95, -92, 95, -104, 91, -88, 95, -112, 79, -104, 89, -104, 99, -108, 103, -80, 95, -118, 73, -98, 81, -96, 99, -98, 103, -90, 103, -90, 105, -106, 99, -100, 93, -106, 93, -88, 103, -74, 101, -94, 111, -106, 103, -100, 109, -88, 97, -80, 93, -130, 103, -118, 113, -118, 91, -86, 89, -102, 71, -112, 75, -100, 97, -106, 83, -98, 97, -98, 85, -134, 95},
//{1100, 2700, 1750, 2100, 1950, 1650, 2050, 1900, 1850, 2450, 1600, 2500, 1700, 1950, 1600, 2000, 1350, 1750, 1600, 1950, 1700, 2250, 1350, 1950, 1400, 2100, 1500, 2150, 1750, 1800, 3600, 2000, 1550, 2200, 1750, 1850, 1600, 2200, 1450, 1950, 1850, 2550, 2050, 2100, 1800, 2800, 1900, 2050, 1500, 2700, 1550, 1900, 1800, 1800, 1900, 1750, 1700, 1900, 1650, 2300, 1700, 1850, 2000, 1750, 1600, 2200, 1850, 2000, 1700, 2000, 1850, 1750, 1700, 1950, 2200, 1750, 1950, 2250, 2000, 1950, 1950, 2300, 2000, 1750},
//{1554, 597, 932, 775, 836, 989, 795, 858, 881, 660, 1020, 646, 959, 836, 1020, 815, 1228, 932, 1020, 836, 959, 721, 1228, 836, 1178, 775, 1092, 756, 932, 906, 446, 815, 1055, 739, 932, 881, 1020, 739, 1133, 836, 881, 633, 795, 775, 906, 576, 858, 795, 1092, 597, 1055, 858, 906, 906, 858, 932, 959, 858, 989, 705, 959, 881, 815, 932, 1020, 739, 881, 815, 959, 815, 881, 932, 959, 836, 739, 932, 836, 721, 815, 836, 836, 705, 815, 932}};


unsigned long time_a, time_b;

float Speed_weight_x = 1;
float Distance_weight_x = 8.0;

float Speed_weight_y = 1;
float Distance_weight_y = 8.0;

int x_speed, y_speed;

int motorPosition_x;
int motorPosition_y;

//int A_len0 = sizeof(A[0]) / sizeof(int);
//int A_len1 = sizeof(A[1]) / sizeof(int);


/////////////
// Encoder //
/////////////
const int encoderA = 2;
const int encoderB = 3;
const int encoderZ = 4;
volatile long encoderPos = 0;
boolean A_set = false;
boolean B_set = false;
boolean Z_set = false;






void setup()
{
  Serial.begin(115200);
  
  Serial.println("---Start---");

  steppers.addStepper(stepper_x);
  steppers.addStepper(stepper_y);


  stepper_x.setMaxSpeed(x_speed);
//  stepper_x.setAcceleration(800);
//  stepper_x.runToNewPosition(0);

  stepper_y.setMaxSpeed(y_speed);
//  stepper_y.setAcceleration(800);
//  stepper_y.runToNewPosition(0);


  /////////////
  // Encoder //
  /////////////
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(encoderZ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB), doEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderZ), doEncoderZ, CHANGE);

}



void loop()
{

//  if (switchPressed1 == true)
//  {
//    stepper_x.setCurrentPosition(0);
//    stepper_x.stop();
//    switchPressed1 = false;
//  }
  
//  constance_for();

//  Serial.println(encoderPos);
//  Serial.print("A:");
//  Serial.println(digitalRead(encoderA));
//  Serial.print("B:");
//  Serial.println(digitalRead(encoderB));
//  Serial.print("Z:");
//  Serial.println(digitalRead(encoderZ));
//  delay(500);
}



void constance_for()
{
  for(int i=0; i<84-1; i++)
  {
    long positions[2];
//    long speeds[2];

    
    positions[0] = A[0][i]*Distance_weight_x; // A[0][i-1]
    positions[1] = B[0][i]*Distance_weight_y;


//    if (i == 0)
//    {
//      stepper_x.setMaxSpeed(A[2][i]*abs(A[0][i])*0.01);
//      stepper_y.setMaxSpeed(B[2][i]*abs(B[0][i])*0.01);
//      Serial.print("abs value: ");
//      Serial.println(A[2][i]*abs(A[0][i])*0.01);
//    }
//    else
//    {
//      stepper_x.setMaxSpeed(A[2][i]*abs(A[0][i-1] - A[0][i]));
//      stepper_y.setMaxSpeed(B[2][i]*abs(B[0][i-1] - B[0][i]));
//    }

    if (i == 0)
    {
      Speed_weight_x = abs(A[0][i]*0.01);
      Speed_weight_y = abs(B[0][i]*0.01);
      Serial.print(Speed_weight_x, Speed_weight_y);
    }
    else
    {
      Speed_weight_x = abs(A[0][i] - A[0][i-1])*0.01;
      Speed_weight_y = abs(B[0][i] - B[0][i-1])*0.01;
      Serial.print(Speed_weight_x, Speed_weight_y);
    }

    
    stepper_x.setMaxSpeed(A[2][i]*Speed_weight_x);
    stepper_y.setMaxSpeed(B[2][i]*Speed_weight_y);
    
    time_a = millis();

    Serial.print("x-motor current position: ");
    Serial.println(positions[0]);

    
    Serial.print("y-motor current position: ");
    Serial.println(positions[1]);
    
    steppers.moveTo(positions);
    
    steppers.runSpeedToPosition();
    
    time_b = millis();
    
//    delay(50);
    Serial.print("Time is:");
    Serial.print(time_b - time_a);
    Serial.print(", diff:");
    Serial.println(A[1][i]);
    Serial.println("------------------------------------");
  }
}


void doEncoderA()
{
  A_set = digitalRead(encoderA) == HIGH;
  encoderPos += (A_set != B_set) ? 1 : -1;
}


void doEncoderB()
{
  B_set = digitalRead(encoderB) == HIGH;
  encoderPos += (A_set == B_set) ? 1 : -1;
}


void doEncoderZ()
{
  Z_set = digitalRead(encoderZ) == HIGH;
  if (Z_set)
  {
    encoderPos = 0;
  }
}

