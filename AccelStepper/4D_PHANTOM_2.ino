/*
Part1(x, y, z), Part2(Z) 축 제어
최대 입력 펄스 주파수: 500kHz = 500,000Hz
        axis:    x,y,z,Z
        ----------------
keymap: cycle:   z,x,c,v
        verse:   a,s,d,f
        reverse: g,h,j,k
        stop:    q,w,e,r
최대 속도(x,y): step_time: 500 per step_count: 1800 (1cm: 0.9s)
최대 속도(z,Z): step_time: 1500 per step_count: 900 (1cm: 2.4s)
*/

// CW+에 연결 // CW-는 GND에 연결
// CCW+에 연결 // CCW-는 GND에 연결

#define CW_x    22
#define CCW_x   24

#define CW_y    30
#define CCW_y   32

#define CW_z    38
#define CCW_z   40

#define CW_Z    46
#define CCW_Z   48


int step_count = 600; // 500 one cycle (Distance)
int step_time = 2000; // 500 one cycle = (Delay)

unsigned long time_a, time_b, time_c;

long receivedDistance_x, receivedDistance_y, receivedDistance_z, receivedDistance_Z = 0;
long receivedDelay_x, receivedDelay_y, receivedDelay_z, receivedDelay_Z = 0;

bool newData, runallowed_x, runallowed_y, runallowed_z, runallowed_Z = false;
bool runstop_x, runstop_y, runstop_z, runstop_Z = false;
bool concon = false;

bool vector_x, vector_y, vector_z, vector_Z = true;


char receivedCommand;

////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  Serial.println("-----Testing Accelstepper Single Motor-----");
  Serial.println("Default value of step_count: 1800");
  Serial.println("Default value of step_time: 1800");
  Serial.println("Insert parameters(distance, delay):");

  pinMode(CW_x,OUTPUT);
  pinMode(CCW_x,OUTPUT);

  pinMode(CW_y,OUTPUT);
  pinMode(CCW_y,OUTPUT);

  pinMode(CW_z,OUTPUT);
  pinMode(CCW_z,OUTPUT);

  pinMode(CW_Z,OUTPUT);
  pinMode(CCW_Z,OUTPUT);

}


////////////////////////////////////

// cw 1cycle 0.72/7.2 (1:7.2) = 0.1, 360 = 3600*0.1
// ccw 1cycle 0.72/7.2 (1:7.2) = 0.1, 360 = 3600*0.1


void loop()
{
//  if (Serial.available() > 0)
  {
    checkSerial();

    continuousRun_x();
    continuousRun_y();
    continuousRun_z();
    continuousRun_Z();
    continuousRun_xyzZ();

//    Serial.println(time_b - time_a);
//    time_a = millis();
//    Serial.println("정회전 시작");
//    CW_xn(step_count);
//    time_b = millis();
    //
//    Serial.println("역회전 시작");
//    CCW_xn(step_count);
//    time_c = millis();
    //
//    Serial.print("정회전 소요시간: ");
//    Serial.println(time_b-time_a);
//    Serial.print("역회전 소요시간: ");
//    Serial.println(time_c-time_b);
//    delay(100);
  }
}

void CW_xyzZn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_x,HIGH); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_x,LOW); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_y,HIGH); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_y,LOW); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_z,HIGH); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_z,LOW); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_Z,HIGH); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_Z,LOW); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
  }
}

void CCW_xyzZn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_x,HIGH); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_x,LOW); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_y,HIGH); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_y,LOW); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_z,HIGH); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_z,LOW); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_Z,HIGH); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_Z,LOW); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
  }
}
 


//////
void CW_xn(int n, int step_time=step_time, int step_sep=3)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_x,HIGH); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_x,LOW); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    
  }
}

void CW_yn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_y,HIGH); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_y,LOW); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    
  }
}

void CW_zn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_z,HIGH); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_z,LOW); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    
  }
}

void CW_Zn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_Z,HIGH); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_Z,LOW); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
    
  }
}

//////
void CCW_xn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_x,HIGH); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_x,LOW); delayMicroseconds(receivedDelay_x); // pulse = 500,000Hz = 500kHz
  }
}

void CCW_yn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_y,HIGH); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_y,LOW); delayMicroseconds(receivedDelay_y); // pulse = 500,000Hz = 500kHz
  }
}

void CCW_zn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_z,HIGH); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_z,LOW); delayMicroseconds(receivedDelay_z); // pulse = 500,000Hz = 500kHz
  }
}

void CCW_Zn(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_Z,HIGH); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_Z,LOW); delayMicroseconds(receivedDelay_Z); // pulse = 500,000Hz = 500kHz
  }
}


//////
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

    //sample xyzZ axis
    if (receivedCommand == 't') //this is the measure part
    {
      runallowed_x = true;
      runallowed_y = true;
      runallowed_z = true;
      runallowed_Z = true;
      runstop_x = false;
      runstop_y = false;
      runstop_z = false;
      runstop_Z = false;
      concon = true;
      
      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = Serial.parseFloat();
      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = Serial.parseFloat();
      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = Serial.parseFloat();
      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = Serial.parseFloat();
      
      
      Serial.print(receivedDistance_x);
      Serial.print(receivedDelay_x);
      Serial.print(receivedDistance_y);
      Serial.print(receivedDelay_y);
      Serial.print(receivedDistance_z);
      Serial.print(receivedDelay_z);
      Serial.print(receivedDistance_Z);
      Serial.print(receivedDelay_Z);
      Serial.println("xyzZ axis: ");
    }


    // motor-x axis moving
    if (receivedCommand == 'z') //this is x part
    {
      
      runallowed_x = true;
      runstop_x = false;
      
      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = Serial.parseFloat();

      Serial.print("Distance: ");
      Serial.print(receivedDistance_x);
      Serial.print(", Delay: ");
      Serial.println(receivedDelay_x);
      

    }

    // START - verse
    if (receivedCommand == 'a')
    {

      runallowed_x = true;
      runstop_x = true;
      vector_x = true;
      
      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = receivedDistance_x*10;
      
      Serial.print(receivedDistance_x);
      Serial.print(receivedDelay_x);
      Serial.println("Moving ");
      
    }

    // START - reverse
    if (receivedCommand == 'g')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_x = true;
      runstop_x = true;
      vector_x = false;

      receivedDistance_x = Serial.parseFloat();
      receivedDelay_x = receivedDistance_x*10;

      Serial.print(receivedDistance_x);
      Serial.print(receivedDelay_x);
      Serial.println("Moving ");
      
    }


    // motor-y axis moving
    if (receivedCommand == 'x') //this is y-axis part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_y = true;
      runstop_y = false;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = Serial.parseFloat();

      Serial.print(receivedDistance_y);
      Serial.print(receivedDelay_y);
      Serial.println("Measure ");

    }

    // START - reverse
    if (receivedCommand == 's')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_y = true;
      runstop_y = true;
      vector_y = true;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = receivedDistance_y*10;

      Serial.print(receivedDistance_y);
      Serial.print(receivedDelay_y);
      Serial.println("Moving ");
      
    }

    // START - reverse
    if (receivedCommand == 'h')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_y = true;
      runstop_y = true;
      vector_y = false;

      receivedDistance_y = Serial.parseFloat();
      receivedDelay_y = receivedDistance_y*10;

      Serial.print(receivedDistance_y);
      Serial.print(receivedDelay_y);
      Serial.println("Moving ");
      
    }
    

    // motor-z axis moving
    if (receivedCommand == 'c') //this is z-axis part
    {
      runallowed_z = true;
      runstop_z = false;


      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = Serial.parseFloat();

      Serial.print(receivedDistance_z);
      Serial.print(receivedDelay_z);
      Serial.println("Measure ");
      
    }

    // START - verse
    if (receivedCommand == 'd')
    {
      runallowed_z = true;
      runstop_z = true;
      vector_z = true;

      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = receivedDistance_z*10;

      Serial.print(receivedDistance_z);
      Serial.print(receivedDelay_z);
      Serial.println("Moving ");
      
    }

    // START - reverse
    if (receivedCommand == 'j')
    {
      // example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed_z = true;
      runstop_z = true;
      vector_z = false;

      receivedDistance_z = Serial.parseFloat();
      receivedDelay_z = receivedDistance_z*10;

      Serial.print(receivedDistance_z);
      Serial.print(receivedDelay_z);
      Serial.println("Moving ");
      
    }


    // motor-Z axis moving
    if (receivedCommand == 'v') //this is Z-axis part
    {
      // example a 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed_Z = true;
      runstop_Z = false;


      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = Serial.parseFloat();

      Serial.print(receivedDistance_Z);
      Serial.print(receivedDelay_Z);
      Serial.println("Measure ");
      
    }

    // START - verse
    if (receivedCommand == 'f')
    {
      runallowed_Z = true;
      runstop_Z = true;
      vector_Z = true;

      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = receivedDistance_Z*10;

      Serial.print(receivedDistance_Z);
      Serial.print(receivedDelay_Z);
      Serial.println("Moving ");
      
    }

    // START - reverse
    if (receivedCommand == 'k')
    {
      runallowed_Z = true;
      runstop_Z = true;
      vector_Z = false;

      receivedDistance_Z = Serial.parseFloat();
      receivedDelay_Z = receivedDistance_Z*10;

      Serial.print(receivedDistance_Z);
      Serial.print(receivedDelay_Z);
      Serial.println("Moving ");
      
    }


    // STOP
    if (receivedCommand == 'q') //"stop-x"
    {
      runallowed_x = false;
      runstop_x = false;
      vector_x = true;

      receivedDistance_x, receivedDelay_x = 0;

      Serial.println("STOP ");
    }

    // STOP
    if (receivedCommand == 'w') //"stop-y"
    {
      runallowed_y = false;
      runstop_y = false;
      vector_y = true;

      receivedDistance_y, receivedDelay_y = 0;

      Serial.println("STOP ");
    }

    // STOP
    if (receivedCommand == 'e') //"stop-z"
    {
      runallowed_z = false;
      runstop_z = false;
      vector_z = true;

      receivedDistance_z, receivedDelay_z = 0;

      Serial.println("STOP ");
    }

    // STOP
    if (receivedCommand == 'r') //"stop-Z"
    {
      runallowed_Z = false;
      runstop_Z = false;
      vector_Z = true;
      
      receivedDistance_Z, receivedDelay_Z = 0;

      Serial.println("STOP ");
    }


    // STOP
    if (receivedCommand == 'y') //"stop-xyzZ"
    {
      runallowed_x, runallowed_y, runallowed_z, runallowed_Z = false;

      receivedDistance_x, receivedDelay_x, receivedDistance_y, receivedDelay_y = 0;
      receivedDistance_z, receivedDelay_z, receivedDistance_Z, receivedDelay_Z = 0;
      
      concon = false;
      Serial.println("STOP ");
    }

  }
  // after we went through the above tasks, newData becomes false again, so we are ready to receive new commands again.
  newData = false;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void continuousRun_xyzZ()
{
  if (runallowed_x == true && runallowed_y == true && runallowed_z == true && runallowed_Z == true &&
  runstop_x == false && runstop_y == false && runstop_z == false && runstop_Z == false && concon == true)
  {
    time_a = millis();
    CW_xyzZn(step_count);
    time_b = millis();
    CCW_xyzZn(step_count);
    time_c = millis();
    Serial.print("time(b-a): ");
    Serial.println(time_b-time_a);
    Serial.print("time(c-b): ");
    Serial.println(time_c-time_b);
  }
}


void continuousRun_x()
{
  if (runallowed_x == true && runstop_x == false && concon == false)
  {
    time_a = millis();
    CW_xn(receivedDistance_x);
    time_b = millis();
    CCW_xn(receivedDistance_x);
    time_c = millis();
    Serial.print("time(b-a): ");
    Serial.println(time_b-time_a);
    Serial.print("time(c-b): ");
    Serial.println(time_c-time_b);
  }
  else if (runallowed_x == true && runstop_x == true && vector_x == true && concon == false)
  {
    CW_xn(receivedDistance_x);
    runallowed_x = false;
  }
  else if (runallowed_x == true && runstop_x == true && vector_x == false && concon == false)
  {
    CCW_xn(receivedDistance_x);
    runallowed_x = false;
  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}


void continuousRun_y()
{
  if (runallowed_y == true && runstop_y == false && concon == false)
  {
    time_a = millis();
    CW_yn(receivedDistance_y);
    time_b = millis();
    CCW_yn(receivedDistance_y);
    time_c = millis();
    Serial.print("time(b-a): ");
    Serial.println(time_b-time_a);
    Serial.print("time(c-b): ");
    Serial.println(time_c-time_b);
  }
  else if (runallowed_y == true && runstop_y == true && vector_y == true && concon == false)
  {
    CW_yn(receivedDistance_y);
    runallowed_y = false;
  }
  else if (runallowed_y == true && runstop_y == true && vector_y == false && concon == false)
  {
    CCW_yn(receivedDistance_y);
    runallowed_y = false;
  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}


void continuousRun_z()
{
  if (runallowed_z == true && runstop_z == false && concon == false)
  {
    time_a = millis();
    CW_zn(receivedDistance_z);
    time_b = millis();
    CCW_zn(receivedDistance_z);
    time_c = millis();
    Serial.print("time(b-a): ");
    Serial.println(time_b-time_a);
    Serial.print("time(c-b): ");
    Serial.println(time_c-time_b);
  }
  else if (runallowed_z == true && runstop_z == true && vector_z == true && concon == false)
  {
    CW_zn(receivedDistance_z);
    runallowed_z = false;
  }
  else if (runallowed_z == true && runstop_z == true && vector_z == false && concon == false)
  {
    CCW_zn(receivedDistance_z);
    runallowed_z = false;
  }
  else // program enters this part i f the runallowed is FALSE, we do not do anything
  {
    return;
  }
}



void continuousRun_Z()
{
  if (runallowed_Z == true && runstop_Z == false && concon == false)
  {
    time_a = millis();
    CW_Zn(receivedDistance_Z);
    time_b = millis();
    CCW_Zn(receivedDistance_Z);
    time_c = millis();
    Serial.print("time(b-a): ");
    Serial.println(time_b-time_a);
    Serial.print("time(c-b): ");
    Serial.println(time_c-time_b);
  }
  else if (runallowed_Z == true && runstop_Z == true && vector_Z == true && concon == false)
  {
    CW_Zn(receivedDistance_Z);
    runallowed_Z = false;
  }
  else if (runallowed_Z == true && runstop_Z == true && vector_Z == false && concon == false)
  {
    CCW_Zn(receivedDistance_Z);
    runallowed_Z = false;
  }
  else // program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
  }
}
