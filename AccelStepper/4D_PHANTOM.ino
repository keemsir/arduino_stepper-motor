/*

Part1(x, y, z), Part2(Z) 축 제어
최대 입력 펄스 주파수: 500kHz = 500,000Hz

최대 속도: step_time: 500 per step_count: 1800
*/


// CW+에 연결 // CW-는 GND에 연결
// CCW+에 연결 // CCW-는 GND에 연결

//#define CW      2
//#define CCW     3

#define CW_x    2
#define CCW_x   3

#define CW_y    4
#define CCW_y   5

#define CW_z    6
#define CCW_z   7

#define CW_Z    10
#define CCW_Z   11

//const int buttonPin = 6;

int step_count = 2000; // 500 one cycle
int step_time = 500; // 500 one cycle

unsigned long time_a, time_b, time_c;

//////
void setup()
{
  Serial.begin(115200);
  Serial.println("-----Testing Accelstepper Single Motor-----");
  Serial.println("Default value of step_count: 1800");
  Serial.println("Default value of step_time: 1800");
  Serial.println("Insert parameters:");

  pinMode(CW_x,OUTPUT);
  pinMode(CCW_x,OUTPUT);

  pinMode(CW_y,OUTPUT);
  pinMode(CCW_y,OUTPUT);

  pinMode(CW_z,OUTPUT);
  pinMode(CCW_z,OUTPUT);

  pinMode(CW_Z,OUTPUT);
  pinMode(CCW_Z,OUTPUT);
}


//////
// 정 1회전 0.72/7.2 (1:7.2) = 0.1, 360 = 3600*0.1
// 역 1회전 0.72/7.2 (1:7.2) = 0.1, 360 = 3600*0.1


void loop()
{
//  if (Serial.available() > 0) 
  {
    time_a = millis();
    CW_n(step_count);
    Serial.println("정회전");
    time_b = millis();
    //
    CCW_n(step_count);
    Serial.println("역회전");
    time_c = millis();
    //
    Serial.print("정회전 소요시간: ");
    Serial.println(time_b-time_a);
    Serial.print("역회전 소요시간: ");
    Serial.println(time_c-time_b);
    Serial.println("Processing...");
    delay(100);
  }
}



//////
void CW_n(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW_x,HIGH); delayMicroseconds(step_time); // pulse = 500,000Hz = 500kHz
    digitalWrite(CW_x,LOW); delayMicroseconds(step_time); // pulse = 500,000Hz = 500kHz
    
  }
}



//////
void CCW_n(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW_x,HIGH); delayMicroseconds(step_time); // pulse = 500,000Hz = 500kHz
    digitalWrite(CCW_x,LOW); delayMicroseconds(step_time); // pulse = 500,000Hz = 500kHz
  }
}
