// sample

#define CW          2 // CW+에 연결 // CW-는 GND에 연결
#define CCW         3 // CCW+에 연결 // CCW-는 GND에 연결

const int buttonPin = 6;
int step_count = 1800;


//////
void setup()
{
  Serial.begin(115200);
  Serial.println("-----Testing Accelstepper Single Motor-----");
  pinMode(CW,OUTPUT);
  pinMode(CCW,OUTPUT);
}

//////
void loop()
{
//  if (Serial.available() > 0)
  {
    CW_n(step_count); // 정 1회전 0.72/7.2 (1:7.2) = 0.1, 360 = 3600*0.1
    Serial.println("정회전");
    delay(100);
    //
    CCW_n(step_count); // 역 1회전
    Serial.println("역회전");
    delay(100);
    //
    Serial.println("Processing..");
    delay(100);
  }
}



//////
void CW_n(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CW,HIGH); delayMicroseconds(1800); // pulse = 14400Hz = 14.4khz
    digitalWrite(CW,LOW); delayMicroseconds(1800);
  }
}

//////
void CCW_n(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(CCW,HIGH); delayMicroseconds(1800);
    digitalWrite(CCW,LOW); delayMicroseconds(1800);
  }
}
